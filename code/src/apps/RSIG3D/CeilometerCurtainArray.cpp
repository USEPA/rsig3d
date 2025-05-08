
/******************************************************************************
PURPOSE: CeilometerCurtainArray.h - Define constants, global variables and
         routines to map ceilometer profile data into arrays used for rendering
         2d 'curtains'.
NOTES:   Used by PointRenderer.cpp.
HISTORY: 2016-09-23 freeman.matt@epa.gov
         2022-03-21 plessel.todd@epa.gov
STATUS: inchoate
******************************************************************************/

//================================ INCLUDES ===================================

#include <QDebug>
#include <QString>

#include <assert.h> // For macro assert().
#include <string.h> // For memset().

#include <vector>    // For std::vector.
#include <string>    // For std::string.
#include <algorithm> // For std::find().
#include <iostream>  // For std::cerr, std::endl.
#include <exception> // for std::exception.

#include <Assertions.h>     // For assert_static(), PRE(), CHECK().
#include <DataUtilities.h>  // For emvl::getHomeDirectory(), emvl::slash.
#include <Dataset.h>        // For emvl::Dataset.
#include <DatasetManager.h> // For emvl::DatasetManager.
#include <ProfileDataset.h> // For emvl::ProfileDataset.

#include "RSIGGlobal.h"             // For RSIGState::getInstance().
#include "GetData.h"                // For getDatasetManager().
#include "CeilometerCurtainArray.h" // For public interface.



//=========================== FORWARD DECLARATIONS ============================

static void initializeTicks( int& secondsPerBin, int& metersPerBin );

static size_t skipSubsetPoints( const std::vector<emvl::Dataset::SubsetPointers>&
                                  subsets, const int profile );

static size_t
sumProfilePoints( const emvl::ProfileDataset* const profileDataset,
                  const std::string& ceilometerName );

namespace cca {

assert_static( SECONDS_PER_DAY   % SIZE_X == 0 );
assert_static( MAXIMUM_ELEVATION % SIZE_Y == 0 );

//============================ GLOBAL VARIABLES ===============================

float Xticks[ SIZE_X ]; // Seconds along x axis where ticks appear.
float Yticks[ SIZE_Y ]; // Meters  along y axis where ticks appear.

double CCA[ SIZE_X ][ SIZE_Y ]; // Ceilometer Curtain Array
static int CCA_counts[ SIZE_X ][ SIZE_Y ]; // For average.

std::vector<long long> MLA_times;   // MLA_times[ points[ profile ] ].
std::vector<double> MLA_elevations; // MLA_elevations[ points[ profile ] ].

//================================ FUNCTIONS ==================================



/******************************************************************************
PURPOSE: haveCeilometerNonMixingLayerHeightDataset - Does DatasetManager have
         any ceilometer dataset that is not mixing_layer_height?
OUTPUTS: size_t& index  Index in DatasetManager of first ceilometer
                        non-mixing_layer_height dataset.
RETURNS: bool true if successful else false.
******************************************************************************/

bool haveCeilometerNonMixingLayerHeightDataset( size_t& index ) {
  bool result = false;
  emvl::DatasetManager& datasetManager = *getDatasetManager(); // Singleton.
  const size_t datasetCount = datasetManager.count();
  index = 0;

  for ( size_t datasetIndex = 0; datasetIndex < datasetCount; ++datasetIndex ) {
    const emvl::Dataset& dataset = datasetManager[ datasetIndex ];
    const emvl::ProfileDataset* const profileDataset =
      dynamic_cast<const emvl::ProfileDataset*>( &dataset );

    if ( profileDataset ) {
      const std::string& datasetName = dataset.name();

      if ( datasetName.find( "ceilomenter" ) == 0 ) {
        const std::string& variableName( dataset.variableName() );

        if ( variableName != "mixing_layer_height" ) {
          result = true;
          index = datasetIndex;
          datasetIndex = datasetCount;
        }
      }
    }
  }

  POST0( IMPLIES( result,
                  datasetManager[ index ].variableName() !=
                    "mixing_layer_height" ) );
  return result;
}



/******************************************************************************
PURPOSE: haveCeilometerMixingLayerHeightDataset - Does DatasetManager have
         ceilometer mixing layer height?
OUTPUTS: size_t& index  Index in DatasetManager of first ceilometer mixing
                        layer height dataset.
RETURNS: bool true if successful else false.
******************************************************************************/

bool haveCeilometerMixingLayerHeightDataset( size_t& index ) {
  bool result = false;
  emvl::DatasetManager& datasetManager = *getDatasetManager(); // Singleton.
  const size_t datasetCount = datasetManager.count();
  index = 0;

  for ( size_t datasetIndex = 0; datasetIndex < datasetCount; ++datasetIndex ) {
    const emvl::Dataset& dataset = datasetManager[ datasetIndex ];
    const emvl::ProfileDataset* const profileDataset =
      dynamic_cast<const emvl::ProfileDataset*>( &dataset );

    if ( profileDataset ) {
      const std::string& variableName( dataset.variableName() );

      if ( variableName == "mixing_layer_height" ) {
        result = true;
        index = datasetIndex;
        datasetIndex = datasetCount;
      }
    }
  }

  POST0( IMPLIES( result,
                  datasetManager[ index ].variableName() ==
                    "mixing_layer_height" ) );
  return result;
}



/******************************************************************************
PURPOSE: loadCeilometerMixingLayerHeightDataset - Load from file or webservice
         ceilometer mixing layer height dataset.
INPUTS:  const emvl::Timestamp& startTime  Start time of subset.
         const emvl::Timestamp& startTime  End   time of subset.
         const emvl::Bounds& bounds        Bounds of subset.
OUTPUTS: size_t& index                     Index in DatasetManager of dataset.
RETURNS: bool true if successful else false.
******************************************************************************/

bool loadCeilometerMixingLayerHeightDataset( const emvl::Timestamp& startTime,
                                             const emvl::Timestamp& endTime,
                                             const emvl::Bounds& bounds,
                                             size_t& index ) {

  PRE0( startTime <= endTime );

  bool result = false;
  index = 0;

  try {
    const emvl::CMAQGridParameters defaultCMAQGridParameters;
    const emvl::DataFilterParameters defaultDataFilterParameters;
    const double boundsZ[ 2 ] = { 0.0, MAXIMUM_ELEVATION };
    static const std::string downloadDirectory( emvl::getHomeDirectory() +
                                                emvl::slash +
                                                ".rsig" ); // UGLY.
    const std::string coverage( "ceilometer.mixing_layer_height" );
    const emvl::QueryParameters
      queryParameters( startTime, endTime, bounds, boundsZ, coverage,
                       emvl::QueryParameters::XDR, downloadDirectory,
                       false, true, false,
                       defaultDataFilterParameters,
                       emvl::QueryParameters::AGGREGATE_NONE,
                       emvl::QueryParameters::REGRID_NONE,
                       emvl::QueryParameters::REGRID_AGGREGATE_NONE,
                       defaultCMAQGridParameters );
    emvl::DatasetManager& datasetManager = *getDatasetManager();
    const std::string pathedFileName( queryParameters.pathedFileName() );

    if ( ! fileExists( pathedFileName.c_str() ) ) {
      datasetManager.retrieveDatasetToFile( queryParameters ); //x
    }

    emvl::Dataset* const dataset = datasetManager.readDataset( pathedFileName ); //x

    const size_t start = pathedFileName.rfind( emvl::slash ) + 1;
    const size_t end = pathedFileName.find( '.', start );
    const size_t length = end - start;
    const std::string name( pathedFileName.substr( start, length ) );

    dataset->name( name ); // Reset name to match saved metadata file.
    datasetManager.append( dataset ); // DatasetManager manages datasets!
    CHECK( datasetManager.count() >= 1 );
    index = datasetManager.count() - 1;
    CHECK( datasetManager[ index ].variables() > 1 );
    CHECK( datasetManager[ index ].variableName( 1 ) == "mixing_layer_height");
    result = true;
  } catch ( std::exception& e ) {
    std::cerr << e.what() << std::endl;
  }

  return result;
}



/******************************************************************************
PURPOSE: getCeilometerInfo - Get info on each unique ceilometer in the
         DatasetManager.
OUTPUTS: std::vector<size_t>& indices         Dataset index.
         std::vector<std::string>& variables  Variable name.
         std::vector<std::string>& units      Variable units.
         std::vector<long long>& ids          Ceilometer id.
         std::vector<double>& longitudes      Longitude of ceilometer.
         std::vector<double>& latitudes       Latitude of ceilometer.
         std::vector<std::string>& names      Ceilometer name.
******************************************************************************/

void getCeilometerInfo( std::vector<size_t>& indices,
                        std::vector<std::string>& variables,
                        std::vector<std::string>& units,
                        std::vector<long long>& ids,
                        std::vector<double>& longitudes,
                        std::vector<double>& latitudes,
                        std::vector<std::string>& names ) {

  enum { LIKELY_MAXIMUM_CEILOMETERS = 256 };

  indices.reserve(    LIKELY_MAXIMUM_CEILOMETERS ); //x
  variables.reserve(  LIKELY_MAXIMUM_CEILOMETERS ); //x
  units.reserve(      LIKELY_MAXIMUM_CEILOMETERS ); //x
  ids.reserve(        LIKELY_MAXIMUM_CEILOMETERS ); //x
  longitudes.reserve( LIKELY_MAXIMUM_CEILOMETERS ); //x
  latitudes.reserve(  LIKELY_MAXIMUM_CEILOMETERS ); //x
  names.reserve(      LIKELY_MAXIMUM_CEILOMETERS ); //x

  indices.clear();
  variables.clear();
  units.clear();
  ids.clear();
  longitudes.clear();
  latitudes.clear();
  names.clear();

  emvl::DatasetManager& datasetManager = *getDatasetManager(); // Singleton.
  const size_t datasetCount = datasetManager.count();

  for ( size_t datasetIndex = 0; datasetIndex < datasetCount; ++datasetIndex ) {
    const emvl::Dataset& dataset = datasetManager[ datasetIndex ];
    const emvl::ProfileDataset* const profileDataset =
      dynamic_cast<const emvl::ProfileDataset*>( &dataset );

    // Only include profileDatasets with name starting with "ceilometer" or "pandora":

    if ( profileDataset ) {
      const std::string& datasetName = dataset.name();

      //if ( datasetName.find( "ceilometer" ) == 0 || datasetName.find( "pandora" ) == 0 ) {
      if ( datasetName.find( "ceilometer" ) == 0 || datasetName.find( "pandora" ) == 0 || datasetName.find( "hourly_pandora" ) == 0 ) {
        const emvl::Timestamp startTime( dataset.startingTimestamp() );
        const emvl::Timestamp endTime( dataset.endingTimestamp() );
        const std::vector<emvl::Dataset::SubsetPointers> subsets(
          dataset.subset( startTime, endTime, 0 ) ); // Variable 0 = Id.
        const size_t profiles = subsets.size();

        if ( profiles ) {
          const std::string variableName = dataset.variableName();
          const std::string variableUnits = dataset.variableUnits();

          for ( size_t profile = 0; profile < profiles; ++profile ) {
            const emvl::Dataset::SubsetPointers& subsetPointers =
              subsets[ profile ];
            CHECK3( subsetPointers.coordinatesCount,
                    subsetPointers.dataCount,
                    subsetPointers.notesCount == 1 );
            indices.push_back( datasetIndex );
            variables.push_back( variableName );
            units.push_back( variableUnits );
            const long long id = subsetPointers.data[ 0 ];
            ids.push_back( id ); // Ceilometer id.
            const double longitude = subsetPointers.coordinates[ 0 ].longitude;
            const double latitude  = subsetPointers.coordinates[ 0 ].latitude;
            const std::string& note = subsetPointers.notes[ 0 ];
            longitudes.push_back( longitude );
            latitudes.push_back( latitude );
            names.push_back( note ); // Ceilometer name.
          }
        }
      }
    }
  }

  POST06( variables.size()  == indices.size(),
          units.size()      == indices.size(),
          ids.size()        == indices.size(),
          longitudes.size() == indices.size(),
          latitudes.size()  == indices.size(),
          names.size()      == indices.size() );
}



/******************************************************************************
PURPOSE: createCCA - Reinitialize CCA for all profiles of a given named
         ceilometer.
INPUTS:  const emvl::Timestamp& startTime    Start time of subset.
         const emvl::Timestamp& endTime      End time of subset.
         const emvl::Dataset& dataset        Dataset to bin/aggregate into CCA.
         const std::string& ceilometerName   Ceilometer name.
OUTPUTS: double CCA[ SIZE_X ][ SIZE_Y ]      2D grid of averaged values of
                                             profile variable for ceilometer.
RETURNS: bool true if successful else false.
******************************************************************************/

bool createCCA( const emvl::Timestamp& startTime,
                const emvl::Timestamp& endTime,
                const emvl::Dataset& dataset,
                const std::string& ceilometerName ) {

  PRE04( startTime <= endTime,
         dataset.variables() > 1,
         dynamic_cast<const emvl::ProfileDataset*>( &dataset ),
         ! ceilometerName.empty() );

  bool result = false;

  // Reinitialize arrays to zero:

  memset( CCA_counts, 0, sizeof CCA_counts );
  memset( CCA,        0, sizeof CCA );
  memset( Xticks,     0, sizeof Xticks );
  memset( Yticks,     0, sizeof Yticks );

  int secondsPerBin = 0;
  int metersPerBin = 0;
  initializeTicks( secondsPerBin, metersPerBin );

  try {
    const emvl::ProfileDataset* const profileDataset =
      dynamic_cast<const emvl::ProfileDataset*>( &dataset );
    CHECK( profileDataset );
    const std::vector<emvl::Dataset::SubsetPointers> subsets(
      dataset.subset( startTime, endTime ) );
    const size_t profiles = subsets.size();


    //qDebug() <<  QString::fromStdString(dataset.name());
    bool isCeilometer = QString::fromStdString(dataset.name()).startsWith("ceilometer");
    //bool isPandora = QString::fromStdString(dataset.name()).startsWith("pandora");
    bool isPandora = QString::fromStdString(dataset.name()).contains("pandora"); // gets pandora or hourly_pandora

    
    for ( size_t profile = 0; profile < profiles; ++profile ) {
      const emvl::Dataset::SubsetPointers& subsetPointers = subsets[ profile];
      const size_t subsetPoints = subsetPointers.dataCount;
      CHECK2( subsetPoints, subsetPointers.notesCount == 1 );
      const std::string& thisProfileName = subsetPointers.notes[ 0 ];

      if ( thisProfileName == ceilometerName ) {

        // Bin and average this ceilometer profile data into curtain array:

        const long long startTimeSeconds = startTime.seconds();
        const size_t skipPoints = skipSubsetPoints( subsets, profile );
        const double* data = subsetPointers.data;
        const emvl::Point* coordinate = subsetPointers.coordinates;

        for ( size_t point = 0; point < subsetPoints;
              ++point, ++data, ++coordinate ) {
          const size_t timestampIndex = skipPoints + point;
          const emvl::Timestamp
            timestamp( profileDataset->timestamp( timestampIndex ) );
          const long long timestampSeconds = timestamp.seconds();
          const long long relativeSeconds = timestampSeconds - startTimeSeconds;
          const long long xbin = relativeSeconds / secondsPerBin;
          const long long ybin = coordinate->elevation / metersPerBin;
          const double value = *data;
          double maxValue = 1e30;

          if (isCeilometer) {
            maxValue = 9e5; //Why?
          } else if (isPandora) {
            maxValue = 1e23;
          }

          if ( AND3( IN_RANGE( xbin, 0, SIZE_X - 1 ),
                     IN_RANGE( ybin, 0, SIZE_Y - 1 ),
                     value < maxValue ) ) { // Why?
            double* const cell = &CCA[ xbin ][ ybin ];
            int* const count = &CCA_counts[ xbin ][ ybin ];

            // Compute running mean to avoid possible summation overflow?

            *cell *= *count;
            *cell += value;
            *count += 1;
            *cell /= *count;
          }
        }

        result = true;
      }
    }
  } catch ( std::exception& e ) {
    std::cerr << e.what() << std::endl;
  }

  return result;
}



/******************************************************************************
PURPOSE: createMLA - Reinitialize MLA for all profiles of a given named
         ceilometer.
INPUTS:  const emvl::Timestamp& startTime    Start time of subset.
         const emvl::Timestamp& endTime      End time of subset.
         const emvl::Dataset& dataset        Dataset to bin/aggregate into MLA.
         const std::string& ceilometerName   Ceilometer name.
OUTPUTS: std::vector<double> MLA_elevations  Mixing layer height elevations of
                                             ceilometer.
         std::vector<long long> MLA_times    Mixing layer height seconds of
                                             ceilometer.
RETURNS: bool true if successful else false.
******************************************************************************/

bool createMLA( const emvl::Timestamp& startTime,
                const emvl::Timestamp& endTime,
                const emvl::Dataset& dataset,
                const std::string& ceilometerName ) {

  PRE05( startTime <= endTime,
         dataset.variables() > 1,
         dataset.variableName( 1 ) == "mixing_layer_height",
         dynamic_cast<const emvl::ProfileDataset*>( &dataset ),
         ! ceilometerName.empty() );

  bool result = false;

  try {
    const emvl::ProfileDataset* const profileDataset =
      dynamic_cast<const emvl::ProfileDataset*>( &dataset );
    CHECK( profileDataset );

    // (Re)allocate and/or initialize arrays to zero:

    const size_t sumPoints = sumProfilePoints( profileDataset, ceilometerName );
    MLA_times.clear();      // Set size to 0 and keep capacity
    MLA_elevations.clear(); // (does not deallocate).
    MLA_times.reserve( sumPoints ); //x      Reallocate only if larger.
    MLA_elevations.reserve( sumPoints ); //x than current capacity.

    int secondsPerBin = 0;
    int metersPerBin = 0;
    initializeTicks( secondsPerBin, metersPerBin );

    const std::vector<emvl::Dataset::SubsetPointers> subsets(
      dataset.subset( startTime, endTime ) );
    const size_t profiles = subsets.size();

    for ( size_t profile = 0; profile < profiles; ++profile ) {
      const emvl::Dataset::SubsetPointers& subsetPointers = subsets[ profile ];
      const size_t subsetPoints = subsetPointers.dataCount;
      CHECK2( subsetPoints, subsetPointers.notesCount == 1 );
      const std::string& thisProfileName = subsetPointers.notes[ 0 ];

      if ( thisProfileName == ceilometerName ) {

        // Copy seconds offset and elevation from profile into MLA arrays:

        const long long startTimeSeconds = startTime.seconds();
        const size_t skipPoints = skipSubsetPoints( subsets, profile );
        const emvl::Point* coordinate = subsetPointers.coordinates;

        for ( size_t point = 0; point < subsetPoints; ++point, ++coordinate ) {
          MLA_elevations.push_back( coordinate->elevation );
          const size_t timestampIndex = skipPoints + point;
          const emvl::Timestamp
            timestamp( profileDataset->timestamp( timestampIndex ) );
          const long long timestampSeconds = timestamp.seconds();
          const long long relativeSeconds = timestampSeconds - startTimeSeconds;
          MLA_times.push_back( relativeSeconds );
        }

        result = true;
      }
    }
  } catch ( std::exception& e ) {
    std::cerr << e.what() << std::endl;
  }

  return result;
}



} // End namespace cca.


//============================ PRIVATE FUNCTIONS ==============================



/******************************************************************************
PURPOSE: initializeTicks - Initialize Xticks and Yticks.
OUTPUTS: int& secondsPerBin      Seconds per curtain bin.
         int& metersPerBin       Meters  per curtain bin.
         float Xticks[ SIZE_X ]  Total seconds per tick.
         float Yticks[ SIZE_X ]  Meters per tick.
******************************************************************************/

static void initializeTicks( int& secondsPerBin, int& metersPerBin ) {
  const long long days = RSIGStateSingleton::getInstance()->mDuration;
  CHECK( days > 0 );
  const long long totalSeconds = days * cca::SECONDS_PER_DAY;
  secondsPerBin = totalSeconds / cca::SIZE_X;
  metersPerBin = cca::MAXIMUM_ELEVATION / cca::SIZE_Y;

  for ( long long sum = 0, bin = 0; bin < cca::SIZE_X;
        ++bin, sum += secondsPerBin ) {
    cca::Xticks[ bin ] = sum;
  }

  for ( long long sum = 0, bin = 0; bin < cca::SIZE_Y;
        ++bin, sum += metersPerBin ) {
    cca::Yticks[ bin ] = sum;
  }

  POST06( secondsPerBin > 0,
          metersPerBin > 0,
          cca::Xticks[ 0 ] == 0,
          cca::Yticks[ 0 ] == 0,
          cca::Xticks[ cca::SIZE_X - 1 ] >=
            cca::SECONDS_PER_DAY / cca::SIZE_X * ( cca::SIZE_X - 1 ),
          cca::Yticks[ cca::SIZE_Y - 1 ] >=
            cca::MAXIMUM_ELEVATION / cca::SIZE_Y * ( cca::SIZE_Y - 1 ) );
}



/******************************************************************************
PURPOSE: skipSubsetPoints - Sum number of profile points before given profile.
INPUTS:  const std::vector<emvl::Dataset::SubsetPointers>& subsets
         const int profile   Profile index to search up to.
RETURNS: size_t Total number of subset points before profile.
******************************************************************************/

static size_t
skipSubsetPoints( const std::vector<emvl::Dataset::SubsetPointers>& subsets,
                  const int profile ) {

  PRE02( profile >= 0, (size_t) profile < subsets.size() );
  size_t result = 0;

  for ( size_t index = 0; index < (size_t) profile; ++index ) {
    result += subsets[ index ].dataCount;
  }

  return result;
}



/******************************************************************************
PURPOSE: sumProfilePoints - Sum number of profile points for given ceilometer.
 INPUTS:  const emvl::ProfileDataset* const profileDataset  Dataset to query.
          const std::string& ceilometerName  Ceilometer to match.
RETURNS: size_t Total number of data points for ceilometer.
******************************************************************************/

static size_t
sumProfilePoints( const emvl::ProfileDataset* const profileDataset,
                  const std::string& ceilometerName ) {

  PRE03( profileDataset, profileDataset->invariant(), ! ceilometerName.empty());

  const size_t profiles = profileDataset->profiles();
  size_t result = 0;

  for ( size_t profile = 0; profile < profiles; ++profile ) {
    const std::string& note = profileDataset->note( profile );

    if ( note == ceilometerName ) {
      result += profileDataset->points( profile );
    }
  }

  return result;
}



