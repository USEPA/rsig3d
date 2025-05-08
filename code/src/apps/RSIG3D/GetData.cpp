/******************************************************************************
PURPOSE: GetData.C - call utility function from Todd. Modified from Example.C
HISTORY: 2013-04-23 zhang.nan@epa.gov
NOTES:
******************************************************************************/

#include <assert.h>   // For assert().
#include <stdio.h>    // For FILE, fopen(), fflush(), fclose().
#include <string.h>   // For strncpy().
#include <stdlib.h>   // For getenv().
#include <sys/stat.h> // For stat(), mkdir().

#if defined( __MINGW32__ )
#include <io.h> // For mkdir().
#define MKDIR( path, unused_ ) mkdir( path )
#else
#define MKDIR mkdir
#endif

#include <iostream>  // For std::cout, std::cerr, std::endl.
#include <exception> // For std::exception.
#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.

#include <QDebug>
#include <QDir>
#include <QRectF>

#include <KMLFile.h>              // For write*KML().
#include <ResourceHandler.h>      // For emvl::ResourceHandler.
#include <DataUtilities.h>        // For emvl::directoryListing().
#include <Bounds.h>               // For emvl::Bounds.
#include <Timestamp.h>            // For emvl::Timestamp.
#include <MapProjection.h>        // For emvl::MapProjection.
#include <DataFilterParameters.h> // For emvl::DataFilterParameters.
#include <CMAQGridParameters.h>   // For emvl::CMAQGridParameters.
#include <QueryParameters.h>      // For emvl::QueryParameters.
#include <Dataset.h>              // For emvl::Dataset.
#include <DatasetManager.h>       // For emvl::DatasetManager.

#include "RSIGGlobal.h"           // For RSIGState::getProgramDirectory().
#include "GeoDataList.h"          // For GeoDataList.
#include "GeoDataObject.h"        // For GeoDataObject.
#include "CeilometerCurtainArray.h" // For cca::*().
#include "GetData.h"              // For public interface.

#include "RGBColormap.h"
#include "VisDataObject.h"
#include "VisDataList.h"
#include "LookupTableWidget.h"      //LookupTableWidget class
#include "ControlDockWidget.h"
#include "MainWin.h"
#include "VisOptions.h"

#include "ui_controldockwidget.h"

static const char* CALIPSOUncertaintyVariable( const char* const variable );

// Use this directory to cache downloaded data files:

static const std::string downloadDirectory =
  QDir::homePath().toStdString() + "/.rsig";



static void ensureDownloadDirectoryExists() {
  struct stat unused;

  if ( stat( downloadDirectory.c_str(), &unused ) ) { // Does not yet exist.

    if ( MKDIR( downloadDirectory.c_str(), 0755 ) ) {
      throw std::runtime_error( "Failed to create download directory: " + downloadDirectory ); //x
    }
  }
}



static void urlCallback( const char* const url ) {
  //std::cout << url << std::endl;
  MainWin& mainWin = MainWin::getInstance();
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
  QString buf = QString("WCS: ");
  buf.append(QString(url));
  controlDockWidget->postMessage(buf, RSIGState::NORMAL_TEXT);

}



emvl::DatasetManager* getDatasetManager() {
  const bool printURL = true; // Print WCS URLs to stdout when retrieving data?
  static emvl::DatasetManager datasetManager( printURL, urlCallback );
  return &datasetManager;
}



void setRSIGServerHost() {
  const char* const path = RSIGState::getProgramDirectory();
  emvl::DatasetManager::checkAndSetServerHost( path );
}



const char* RSIGServerHost() {
  static std::string serverName( emvl::DatasetManager::serverHost() );
  return serverName.c_str();
}



const char* RSIGWebservice() {
  static std::string result( emvl::DatasetManager::webservice() );
  return result.c_str();
}



// E.g., retrieve download/RSIG3D_Darwin.zip to /Users/plessel/RSIG3D.zip:

bool downloadFile( const std::string& remoteFileName,
                   const std::string& localFileName ) {
  bool result = false;

  try {
    emvl::DatasetManager& datasetManager = *getDatasetManager();
    datasetManager.retrieveFile( remoteFileName, localFileName ); //x
    result = true;
  } catch (...) {
  }

  return result;
}



/*
downloadData - Retrieve a dataset or metadata from the web service.
If getMetadata then write metadata file to saveDirectory
else write dataset (xdr file) to downloadDirectory and then read it into memory.
*/

emvl::Dataset* downloadData( const bool getMetadata,
                             const bool compress,
                             const int format,
                             const char* const saveDirectory,
                             const char* variableName,
                             const char* startTime,
                             const char* endTime,
                             double west, double east,
                             double south, double north,
                             int layer,
                             int aggregate,
                             int regridMethod,
                             int regridAggregation,
                             emvl::CMAQGridParameters gridParameters,
                             emvl::DataFilterParameters filterParameters ) {

  emvl::DatasetManager& datasetManager = *getDatasetManager();
  emvl::Timestamp::UTCString startTimestamp = "";
  emvl::Timestamp::UTCString endTimestamp = "";
  memset( startTimestamp, 0, sizeof startTimestamp );
  memset( endTimestamp,   0, sizeof endTimestamp );
  strncpy( startTimestamp, startTime, sizeof startTimestamp / sizeof *startTimestamp - 1 );
  strncpy( endTimestamp, endTime, sizeof endTimestamp / sizeof *endTimestamp - 1 );
  const emvl::Bounds bounds( west, east, south, north );
  CHECK2( emvl::Timestamp::isUTCString( startTime ),
          emvl::Timestamp::isUTCString( endTime ) );
  //const int regridMethod = _regridMethod;
  double boundsZ[ 2 ] = { emvl::MISSING, emvl::MISSING };
  
  // HACK UNIMPLEMENTED: hard-code vertical subset for subsettable 3D data:
 
  bool integrateCMAQLayers = false;
  
  if ( strstr( variableName, "cmaq." ) ) {
    
    if ( layer < 0 ) {
      integrateCMAQLayers = true;
    } else if (layer > 0) {
      boundsZ[ 0 ] = layer;
      boundsZ[ 1 ] = layer;
    } 
  } else if ( strstr( variableName, "calipso." ) ) {
    boundsZ[ 0 ] = 0.0;
    boundsZ[ 1 ] = 40000.0;
  } else if ( strstr( variableName, "neubrew." ) ) {
    boundsZ[ 0 ] = 0.0;
    boundsZ[ 1 ] = 51000.0;
  } else if ( strstr( variableName, "ceilometer." ) ) {
    boundsZ[ 0 ] = 0.0;
    boundsZ[ 1 ] = 15000.0;
  }

  std::string directory;

  if ( getMetadata || format == emvl::QueryParameters::ORIGINAL ) {
    directory = saveDirectory;
  } else {
    directory = downloadDirectory;
  }

  // HACK to make metadata request work with vis file formats.

  const int requestFormat =
    AND2( getMetadata,
          ! IN6( format,
                 emvl::QueryParameters::XDR,
                 emvl::QueryParameters::ASCII,
                 emvl::QueryParameters::NETCDF_COARDS,
                 emvl::QueryParameters::NETCDF_IOAPI,
                 emvl::QueryParameters::ORIGINAL ) ) ?
          emvl::QueryParameters::XDR : format;

  int aggregate2 = aggregate;
  
  if ( strstr( variableName, "purpleair" ) == variableName ) {
    const RSIGState* const theRSIGState = RSIGStateSingleton::getInstance();

    if ( theRSIGState->mTimebaseType == RSIGState::TIMEBASE_DAILY ) {
      aggregate2 = emvl::QueryParameters::AGGREGATE_DAILY_MEAN;
    } else if ( theRSIGState->mTimebaseType == RSIGState::TIMEBASE_DAILY_LST ) {
      aggregate2 = emvl::QueryParameters::AGGREGATE_DAILY_MEAN;
    } else {
      aggregate2 = emvl::QueryParameters::AGGREGATE_NONE; // Hourly.
    }
  }

  const emvl::QueryParameters query( startTimestamp, endTimestamp,
                                     bounds, boundsZ,
                                     std::string( variableName ),
                                     requestFormat,
                                     directory,
                                     getMetadata,
                                     compress,
                                     integrateCMAQLayers,
                                     filterParameters,
                                     aggregate2,
                                     regridMethod,
                                     regridAggregation,
                                     gridParameters );

  emvl::Dataset* dataset = 0;

  try {
    ensureDownloadDirectoryExists(); //x
    datasetManager.retrieveDatasetToFile( query ); //x

    /*
     * Special logic for saving certain CALIPSO data:
     * Per NASA Langley CALIPSO Team, users must also save the associated
     * Uncertainty variable (non-regridded/non-aggregated), if available.
     */

    if ( getMetadata && strstr( variableName, "calipso." ) ) {
      const char* const uncertaintyVariable =
        CALIPSOUncertaintyVariable( variableName );

      if ( uncertaintyVariable ) {
        emvl::QueryParameters queryUncertainty( query );
        queryUncertainty.variables( std::string( uncertaintyVariable ) );
        queryUncertainty.getMetadata( false );
        queryUncertainty.aggregateMethod( emvl::QueryParameters::AGGREGATE_NONE );
        queryUncertainty.regridMethod( emvl::QueryParameters::REGRID_NONE );
        queryUncertainty.regridAggregateMethod( emvl::QueryParameters::REGRID_AGGREGATE_NONE );

        if ( queryUncertainty.format() == emvl::QueryParameters::NETCDF_IOAPI ) {
          queryUncertainty.format( emvl::QueryParameters::NETCDF_COARDS );
        }

        datasetManager.retrieveDatasetToFile( queryUncertainty ); //x
      }
    }

    if ( ! ( getMetadata || format == emvl::QueryParameters::ORIGINAL ) ) {
      const std::string pathedFileName( query.pathedFileName() );
      const size_t start = pathedFileName.rfind( emvl::slash ) + 1;
      const size_t end = pathedFileName.find( '.', start );
      const size_t length = end - start;
      const std::string name( pathedFileName.substr( start, length ) );
      dataset = datasetManager.readDataset( pathedFileName ); //x
      dataset->name( name ); // Reset name to match saved metadata file.
      datasetManager.append( dataset ); // DatasetManager manages datasets!
    }
  } catch ( std::exception& e ) {

    if (QString(e.what()).contains("std::bad_alloc")) {
      MainWin& mainWin = MainWin::getInstance();
      ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
      QString message = "Data object too big to load into memory.";
      controlDockWidget->postMessage(message, RSIGState::WARNING_TEXT);

      if (QString(variableName).contains("cmaq.")) {
        QString message2 = "You can reduce the memory footprint by restricting CMAQ variables to a specified layer.";
        controlDockWidget->postMessage(message2, RSIGState::WARNING_TEXT);
      }
    }

    std::cerr << e.what() << std::endl;
  }

  return dataset;
}



// Return uncertainty variable for CALIPSO variable or 0 if none:

static const char* CALIPSOUncertaintyVariable( const char* const variable ) {
  static const char* const uncertaintyVariables[] = {
    "calipso.l1.Depolarization_Gain_Ratio_Uncertainty_532",
    "calipso.l1.Calibration_Constant_Uncertainty_532",
    "calipso.l1.Calibration_Constant_Uncertainty_1064",
    "calipso.l2_05kmapro.Column_Optical_Depth_Cloud_Uncertainty_532",
    "calipso.l2_05kmapro.Column_Optical_Depth_Aerosols_Uncertainty_532",
    "calipso.l2_05kmapro.Column_Optical_Depth_Stratospheric_Uncertainty_532",
    "calipso.l2_05kmapro.Column_Optical_Depth_Aerosols_Uncertainty_1064",
    "calipso.l2_05kmapro.Column_Optical_Depth_Stratospheric_Uncertainty_1064",
    "calipso.l2_05kmapro.Total_Backscatter_Coefficient_Uncertainty_532",
    "calipso.l2_05kmapro.Perpendicular_Backscatter_Coefficient_Uncertainty_532",
    "calipso.l2_05kmapro.Particulate_Depolarization_Ratio_Uncertainty_532",
    "calipso.l2_05kmapro.Extinction_Coefficient_Uncertainty_532",
    "calipso.l2_05kmapro.Backscatter_Coefficient_Uncertainty_1064",
    "calipso.l2_05kmapro.Extinction_Coefficient_Uncertainty_1064",
    "calipso.l2_05kmcpro.Column_Optical_Depth_Cloud_Uncertainty_532",
    "calipso.l2_05kmcpro.Column_Optical_Depth_Aerosols_Uncertainty_532",
    "calipso.l2_05kmcpro.Column_Optical_Depth_Stratospheric_Uncertainty_532",
    "calipso.l2_05kmcpro.Column_Optical_Depth_Aerosols_Uncertainty_1064",
    "calipso.l2_05kmcpro.Column_Optical_Depth_Stratospheric_Uncertainty_1064",
    "calipso.l2_05kmcpro.Total_Backscatter_Coefficient_Uncertainty_532",
    "calipso.l2_05kmcpro.Perpendicular_Backscatter_Coefficient_Uncertainty_532",
    "calipso.l2_05kmcpro.Particulate_Depolarization_Ratio_Uncertainty_532",
    "calipso.l2_05kmcpro.Extinction_Coefficient_Uncertainty_532",
    "calipso.l2_05kmcpro.Ice_Water_Content_Profile_Uncertainty",
    "calipso.l2_01kmclay.Parallel_Column_Reflectance_Uncertainty_532",
    "calipso.l2_01kmclay.Perpendicular_Column_Reflectance_Uncertainty_532",
    "calipso.l2_01kmclay.Integrated_Attenuated_Backscatter_Uncertainty_532",
    "calipso.l2_01kmclay.Integrated_Attenuated_Backscatter_Uncertainty_1064",
    "calipso.l2_01kmclay.Integrated_Volume_Depolarization_Ratio_Uncertainty",
    "calipso.l2_01kmclay.Integrated_Attenuated_Total_Color_Ratio_Uncertainty",
    "calipso.l2_05kmalay.Parallel_Column_Reflectance_Uncertainty_532",
    "calipso.l2_05kmalay.Perpendicular_Column_Reflectance_Uncertainty_532",
    "calipso.l2_05kmalay.Column_Optical_Depth_Cloud_Uncertainty_532",
    "calipso.l2_05kmalay.Column_Optical_Depth_Aerosols_Uncertainty_532",
    "calipso.l2_05kmalay.Column_Optical_Depth_Stratospheric_Uncertainty_532",
    "calipso.l2_05kmalay.Column_Optical_Depth_Aerosols_Uncertainty_1064",
    "calipso.l2_05kmalay.Column_Optical_Depth_Stratospheric_Uncertainty_1064",
    "calipso.l2_05kmalay.Integrated_Attenuated_Backscatter_Uncertainty_532",
    "calipso.l2_05kmalay.Integrated_Attenuated_Backscatter_Uncertainty_1064",
    "calipso.l2_05kmalay.Integrated_Volume_Depolarization_Ratio_Uncertainty",
    "calipso.l2_05kmalay.Integrated_Attenuated_Total_Color_Ratio_Uncertainty",
    "calipso.l2_05kmalay.Measured_Two_Way_Transmittance_Uncertainty_532",
    "calipso.l2_05kmalay.Feature_Optical_Depth_Uncertainty_532",
    "calipso.l2_05kmalay.Integrated_Particulate_Depolarization_Ratio_Uncertainty",
    "calipso.l2_05kmalay.Feature_Optical_Depth_Uncertainty_1064",
    "calipso.l2_05kmalay.Integrated_Particulate_Color_Ratio_Uncertainty",
    "calipso.l2_05kmclay.Parallel_Column_Reflectance_Uncertainty_532",
    "calipso.l2_05kmclay.Perpendicular_Column_Reflectance_Uncertainty_532",
    "calipso.l2_05kmclay.Column_Optical_Depth_Cloud_Uncertainty_532",
    "calipso.l2_05kmclay.Column_Optical_Depth_Aerosols_Uncertainty_532",
    "calipso.l2_05kmclay.Column_Optical_Depth_Stratospheric_Uncertainty_532",
    "calipso.l2_05kmclay.Column_Optical_Depth_Aerosols_Uncertainty_1064",
    "calipso.l2_05kmclay.Column_Optical_Depth_Stratospheric_Uncertainty_1064",
    "calipso.l2_05kmclay.Integrated_Attenuated_Backscatter_Uncertainty_532",
    "calipso.l2_05kmclay.Integrated_Attenuated_Backscatter_Uncertainty_1064",
    "calipso.l2_05kmclay.Integrated_Volume_Depolarization_Ratio_Uncertainty",
    "calipso.l2_05kmclay.Integrated_Attenuated_Total_Color_Ratio_Uncertainty",
    "calipso.l2_05kmclay.Measured_Two_Way_Transmittance_Uncertainty_532",
    "calipso.l2_05kmclay.Feature_Optical_Depth_Uncertainty_532",
    "calipso.l2_05kmclay.Integrated_Particulate_Depolarization_Ratio_Uncertainty",
    "calipso.l2_333mclay.Perpendicular_Column_Reflectance_Uncertainty_532",
    "calipso.l2_333mclay.Integrated_Attenuated_Backscatter_Uncertainty_532",
    "calipso.l2_333mclay.Integrated_Attenuated_Backscatter_Uncertainty_1064",
    "calipso.l2_333mclay.Integrated_Volume_Depolarization_Ratio_Uncertainty",
    "calipso.l2_333mclay.Integrated_Attenuated_Total_Color_Ratio_Uncertainty",
  };
  const size_t count = sizeof uncertaintyVariables / sizeof *uncertaintyVariables;
  const char* result = 0;

  for ( size_t index = 0; index < count; ++index ) {
    const char* const uncertaintyVariable = uncertaintyVariables[ index ];
    std::string v( uncertaintyVariable ); //x
    const size_t i = v.find( "_Uncertainty" );
    assert( i < v.length() );
    v.erase( i, 12 );

    if ( ! strcmp( variable, v.c_str() ) ) {
      result = uncertaintyVariable;
      index = count;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: createKMLFileContent - Write initial content of output KML file
         including colors, overall bounds and any map images.
INPUTS:  emvl::ResourceHandler<FILE*>& fileCloser  KML file closer.
         const char* const inputDirectory  Path to existing image files to copy.
         const char* const outputDirectory Path to write copies of image files.
         const Bounds bounds               Bounding box of data/images.
******************************************************************************/

static void createKMLFileContent( emvl::ResourceHandler<FILE*>& fileCloser,
                                  const char* inputDirectory,
                                  const char* outputDirectory,
                                  const Bounds bounds ) {
  assert( inputDirectory ); assert( *inputDirectory );
  assert( outputDirectory ); assert( *outputDirectory );
  assert( strcmp( inputDirectory, outputDirectory ) );
  FILE* outputKMLFile = fileCloser.get();

  if ( ! outputKMLFile ) {
    throw std::runtime_error( "Failed to open KML file for writing." ); //x
  }

  writeStartKML( outputKMLFile, "RSIG", "RSIG Data", bounds ); //x
  writeMapImagesToKML( outputKMLFile, inputDirectory,
                       "modisTrueColor", ".png", outputDirectory, bounds );
  fflush( outputKMLFile ); // Ensure content is written before appends.
}



/******************************************************************************
PURPOSE: finishKMLFile - Write final content to KML file and close the file.
INPUTS:  FILE* outputKMLFile  File to write final content to.
OUTPUTS: FILE* outputKMLFile  Closed.
******************************************************************************/

static void finishKMLFile( FILE* outputKMLFile ) {

  if ( outputKMLFile ) {
    writeEndKML( outputKMLFile );
    fclose( outputKMLFile );
    outputKMLFile = 0;
  }
}



/******************************************************************************
PURPOSE: saveData - Write each dataset to the specified output directory in the
         given file format.
INPUTS:  emvl::Dataset* datasets[ datasetCount ]  Array of datasets to write.
         const int datasetCount                   Number of datasets.
         const int colormaps[ datasetCount ]      Index of colormap used.
         const Bounds bounds                      Bounding box of data/images.
         const char* const outputDirectory    Pathed directory to write files.
         const int format Dataset format: emvl::QueryParameters::XDR, etc.
******************************************************************************/

bool saveData( emvl::Dataset* datasets[], const int datasetCount,
               const int colormaps[], const Bounds bounds,
               const char* const outputDirectory, const int format ) {

  bool result = true;

  try {

    if ( format == emvl::QueryParameters::KML ) { // Special logic for KML:

      /*
       * When saving datasets in KML format, there is a single .kml file
       * created with initial content (including colors and map images) and
       * then each dataset is first given the FILE* to this opened .kml file
       * and then each dataset writes its data to the opened KML file
       * then the final content is written and the file is closed.
       */

      // Generate the name of the single output .kml file:

      std::string outputKMLFileName;
      outputKMLFileName.reserve( 256 ); //x
      outputKMLFileName = outputDirectory;
      outputKMLFileName += "/rsig.kml";

      // Use a resource handler to ensure output file is finalized and closed:

      emvl::ResourceHandler<FILE*> fileCloser( fopen( outputKMLFileName.c_str(),
                                                      "wb" ),
                                               finishKMLFile );

      // Get the directory that may contain map image files to include in KML:

      const QString downloadDirectory( QDir::homePath() + "/.rsig" );
      const std::string copy( downloadDirectory.toStdString() );
      const char* const inputDirectory = copy.c_str();

      // Create the single KML output file and write initial content:

      createKMLFileContent( fileCloser, inputDirectory, outputDirectory,
                            bounds ); //x

      // Write each dataset to the same output KML file:

      for ( int datasetIndex = 0; datasetIndex < datasetCount; ++datasetIndex) {
        const int colormapIndex = colormaps[ datasetIndex ];
        DataColor dataColorFunction = dataColor; // Default one from Utilities.h.

        if ( colormapIndex > -1 &&
             colormapIndex < (int) emvl::getRGBColormapCount() ) {
          dataColorFunction = emvl::getRGBColormapFunction( colormapIndex );
        }

        datasets[ datasetIndex ]->outputKMLDataColorFunction(dataColorFunction);
        datasets[ datasetIndex ]->outputKMLFile( fileCloser.get() ); // FILE*
        datasets[ datasetIndex ]->write( outputDirectory, format ); //x
      }

      // outputKMLFile is finalized and closed at the end of 'if' block.

    } else {

      // For all other formats, each dataset writes to its own output file:

      for ( int datasetIndex = 0; datasetIndex < datasetCount; ++datasetIndex) {
        datasets[ datasetIndex ]->write( outputDirectory, format ); //x
      }
    }
  } catch (std::exception &e) {
    qDebug() << "Error in saveData: " << e.what();
    result = false;
  }

  return result;
}



std::vector<std::string> fileNamesOfLoadedDatasets() {
  std::vector<std::string> result;
  GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();

  for ( int index = 0; index < geoDataList->size(); ++index ) {
    const GeoDataObject* const geoDataObject = geoDataList->getObject( index );
    const emvl::Dataset* const dataset = geoDataObject->model();
    const std::string& fileName = dataset->fileName();

    if ( ! fileName.empty() ) {
      result.push_back( fileName ); //x
    }
  }

  return result;
}



std::vector<bool> regridFlagsOfLoadedDatasets() {
  std::vector<bool> result;
  GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();

  for ( int index = 0; index < geoDataList->size(); ++index ) {
    const GeoDataObject* const geoDataObject = geoDataList->getObject( index );
    //const emvl::Dataset* const dataset = geoDataObject->model();
    const QString& name = geoDataObject->varName();
    const bool regridFlag =
      name.startsWith( "regridded_" )
      // HACK: false for already gridded data such as regridded monthly tropomi
      && ! name.contains( "_monthly_" ) && ! name.contains( "_seasonal_" );
    result.push_back( regridFlag ); //x
  }

  return result;
}



void reloadDatasets( QRectF& boundingBox,
                     const std::vector<std::string>& datasetNames,
                     const std::vector<std::string>& fileNames ) {

  emvl::DatasetManager* const datasetManager = getDatasetManager();

  for ( size_t index = 0; index < fileNames.size(); ++index ) {
    const std::string& fileName = fileNames[ index ];

    if ( ! fileName.empty() ) {

      try {

        QString objectName( datasetNames[ index ].c_str() );

        // get the desired dataset name from VisOptions
        VisOptions *vOptions = VisOptionsSingleton::getInstance();
        QStringList dataSourceList = vOptions->getDataSourceList();
        QString desiredName;

        for (int ind=0; ind<dataSourceList.size(); ind++) {
          QStringList thisEntry = dataSourceList.at(ind).split("#");

          if (thisEntry[0] == objectName) {
            desiredName = thisEntry[1];
            break;
          }
        }

        emvl::Dataset* const dataset = datasetManager->readDataset( fileName ); //x
        dataset->name( desiredName.toUtf8().constData() );
        datasetManager->append( dataset ); //x DatasetManager manages datasets!
        GeoDataObject* const geoDataObject = new GeoDataObject( 0, dataset, &boundingBox ); //x
        MainWin& mainWin = MainWin::getInstance();
        ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
        controlDockWidget->loadGeoDataObject( geoDataObject );
      } catch ( ... ) {
        QString message( "Failed to reload dataset: " );
        const char* name = fileName.c_str();
        message.append( name );
        qDebug() << message;
      }
    }
  }
}



/*
 * If DatasetManager has a non-MixingLayerHeight ceilometer dataset
 * and MixingLayerHeight is not already loaded then load it from file or web
 * so it is ready to overlay on ceilometer pop-up plots.
 */

void checkAndLoadCeilometerMixingLayerHeightDataset() {

  try {
    size_t firstCeilometerDatasetIndex = 0;

    if ( cca::haveCeilometerNonMixingLayerHeightDataset(
                                          firstCeilometerDatasetIndex ) ) {
      size_t mixingLayerHeightDatasetIndex = 0;

      if ( ! cca::haveCeilometerMixingLayerHeightDataset(
                                        mixingLayerHeightDatasetIndex ) ) {
        const RSIGState* const rsigState =
          RSIGStateSingleton::getInstance();
        const emvl::Timestamp
          globalStartTime( rsigState->getStartingTimestamp() );
        const emvl::Timestamp
          globalEndTime( rsigState->getEndingTimestamp() );
        emvl::DatasetManager& datasetManager = *getDatasetManager();
        const emvl::Dataset& ceilometerDataset =
          datasetManager[ firstCeilometerDatasetIndex ];
        const emvl::Bounds& bounds( ceilometerDataset.bounds() );
        MainWin& mainWin = MainWin::getInstance();
        ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
        controlDockWidget->postMessage(
          "Loading ceilometer.mixing_layer_height...",
          RSIGState::NORMAL_TEXT );
        const bool ok =
          cca::loadCeilometerMixingLayerHeightDataset( globalStartTime,
                                                       globalEndTime,
                                                       bounds,
                                               mixingLayerHeightDatasetIndex );

        if ( ok ) {
          controlDockWidget->postMessage( "Finished.", RSIGState::NORMAL_TEXT);
        } else {
          controlDockWidget->postMessage(
            "Failed to retrieve ceilometer.mixing_layer_height.",
            RSIGState::WARNING_TEXT );
        }
      }
    }
  } catch ( std::exception& e ) {
    std::cerr << e.what() << std::endl;
  }
}


