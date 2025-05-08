/******************************************************************************
PURPOSE: PolygonDataset.C - Dataset for 2D polygon data.
HISTORY: 2018-11-21 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fscanf().
#include <string.h> // For memset(), memcpy(), strstr().
#include <math.h>   // For sqrt().
#include <float.h>  // For FLT_MAX.
#include <unistd.h> // For unlink().

#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <algorithm> // For std::stable_sort().

#include <Assertions.h> // For PRE(), POST(), AND(), IN_RANGE().
#include <Shapefile.h>  // For PolygonShape, copy/deallocatePolygons().
#include <KMLFile.h>    // For writePolygonsToKML().

#include <DataUtilities.h>      // For isNan(), isLongitude(), clampedToRange()
#include <ResourceHandler.h>    // For ResourceHandler.
#include <Bounds.h>             // For Bounds.
#include <Timestamp.h>          // For Timestamp.
#include <QueryParameters.h>    // For QueryParameters.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <NetCDFUtilities.h>    // For *NetCDF*() routines.
#include <PolygonDataset.h>     // For PolygonDataset.

enum { NOTE_LENGTH = 80 };

namespace emvl {

//============================ PRIVATE FUNCTIONS ==============================


static Color reverseGrayColor( double value, double minimum, double maximum );


// Passed to stable_sort() in vsubset():

static bool compareSubsetPointers( const Dataset::SubsetPointers& a,
                                   const Dataset::SubsetPointers& b ) {
  return a.data[ 0 ] < b.data[ 0 ];
}



/******************************************************************************
PURPOSE: deallocate - Deallocate a PolygonShapeData.
******************************************************************************/

void PolygonDataset::PolygonShapeData::deallocate() {

  if ( polygonShapes ) {
    deallocatePolygons( (int) count, (PolygonShape*) polygonShapes );
    polygonShapes = 0;
  }

  if ( shapeData ) {
    deallocateShapeData( (ShapeData*) shapeData );
    shapeData = 0;
  }

  count = 0;
}



/******************************************************************************
PURPOSE: copy - Copy a PolygonShapeData.
INPUTS:  const PolygonShapeData& other  Object to copy.
******************************************************************************/

void PolygonDataset::PolygonShapeData::copy( const PolygonShapeData& other ) {

  if ( this != &other ) {
    deallocate();

    if ( other.polygonShapes ) {
      polygonShapes =
        copyPolygons( (int) other.count, (PolygonShape*) other.polygonShapes );

      if ( AND2( polygonShapes, other.shapeData ) ) {
        shapeData = copyShapeData( (const ShapeData*) other.shapeData );
      }

      if ( OR2( polygonShapes == 0, shapeData == 0 ) ) {
        throw std::runtime_error( "Failed to copy polygon data." ); //x
      }

      count = other.count;
    }
  }
}



/******************************************************************************
PURPOSE: operator== - Are a PolygonShapeData objects equal?
INPUTS:  const PolygonShapeData& other  Object to compare.
******************************************************************************/

bool PolygonDataset::PolygonShapeData::operator==(
  const PolygonShapeData& other ) const {
  bool result = this == &other;

  if ( ! result ) {
    result =
      AND3( count == other.count,
           ( polygonShapes != 0 ) == ( other.polygonShapes != 0 ),
           ( shapeData != 0 ) == ( other.shapeData != 0 ) );

    if ( result ) {
      const ShapeData* const thisShapeData  = (ShapeData*) shapeData;
      const ShapeData* const otherShapeData = (ShapeData*) other.shapeData;
      result = equalShapeData( thisShapeData, otherShapeData ) == 1;
    }
  }

  return result;
}



//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~PolygonDataset - Destruct a PolygonDataset.
******************************************************************************/

PolygonDataset::~PolygonDataset() {
  PRE( true );
}



/******************************************************************************
PURPOSE: PolygonDataset - Construct a PolygonDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid point data.
******************************************************************************/

PolygonDataset::PolygonDataset( FILE* stream ) {
  PRE0( stream );

  variables_        = 0;
  dataVariable_     = 0;
  timesteps_        = 0;
  cellType_         = POLYGONS;
  hasElevation_     = false;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;
  isVector_         = false;
  readXDRHeader( stream ); //x
  readXDRData( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid polygon data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: PolygonDataset - Construct a PolygonDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid point data.
******************************************************************************/

PolygonDataset::PolygonDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  variables_        = 0;
  dataVariable_     = 0;
  timesteps_        = 0;
  cellType_         = POLYGONS;
  hasElevation_     = false;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;
  isVector_         = false;
  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDRHeader( file ); //x    
    readXDRData( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid polygon data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: PolygonDataset - Copy-construct a PolygonDataset.
INPUTS:  const PolygonDataset& other  Object to copy.
******************************************************************************/

PolygonDataset::PolygonDataset( const PolygonDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    startingTimestamps_ = other.startingTimestamps_;
    endingTimestamps_   = other.endingTimestamps_;
    polygonShapeData_   = other.polygonShapeData_; //x
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a PolygonDataset.
INPUTS:  const PolygonDataset& other  Object to copy.
RETURNS: PolygonDataset& *this.
******************************************************************************/

PolygonDataset& PolygonDataset::operator=( const PolygonDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    startingTimestamps_ = other.startingTimestamps_;
    endingTimestamps_   = other.endingTimestamps_;
    polygonShapeData_ = other.polygonShapeData_; //x
  }

  POST( *this == other );
  return *this;
}



/******************************************************************************
PURPOSE: sample - Sample other dataset at this dataset's coordinates.
INPUTS:  const Dataset& other  Dataset to sample.
RETURNS: Dataset* sampled dataset. Deallocate when no longer needed.
NOTES:   Throws exception upon failure.
******************************************************************************/

Dataset* PolygonDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: PolygonDataset::sample()" ); //x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: regrid - Regrid this dataset's data onto given grid.
INPUTS:  const int regridMethod  REGRID_NEAREST...REGRID_WEIGHTED.
         const CMAQGridParameters& cmaqGridParameters  Grid parameters.
RETURNS: Dataset* regridded dataset. Deallocate when no longer needed.
NOTES:   Throws exception upon failure.
******************************************************************************/

Dataset* PolygonDataset::regrid( const int regridMethod,
                                 const CMAQGridParameters& cmaqGridParameters )
  const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: PolygonDataset::regrid()" ); //x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: write - Write dataset to a directory file in the specified format.
INPUTS:  const std::string& directory  Name of directory to write to
                                       or, if KML format, name of KML file to
                                       append to.
         const int format              Format of file to write.
NOTES:   The name of the file is determined by the dataset name, variable, etc.
         Throws exception upon failure.
******************************************************************************/

void PolygonDataset::write( const std::string& directory,
                            const int format ) const {
  PRE2( directory.length(),
        IN_RANGE( format, 0, QueryParameters::FILE_FORMATS - 1 ) );

  const std::string fileName( pathedFileName( directory, format ) ); //x
  DEBUG( fprintf( stderr, "Writing file '%s'...\n", fileName.c_str() ); )

  if ( IN3( format, QueryParameters::XDR, QueryParameters::ASCII ) ) {
    const char* const name = fileName.c_str();
    ResourceHandler<FILE*> fileCloser( fopen( name, "wb"), fclose);
    FILE* const file = fileCloser.get();
    fileCloser.handler2( name, unlink ); // Remove file upon failure.

    if ( ! file ) {
      throw std::runtime_error( "Failed to open writable file." ); //x
    } else if ( format == QueryParameters::XDR ) {
      writeXDR( file ); //x
    } else if ( format == QueryParameters::ASCII ) {
      writeASCII( file ); //x
    }

    fileCloser.handler2( 0, 0 ); // Successful so don't remove file.
  } else if ( format == QueryParameters::NETCDF_COARDS ) {
    writeCOARDS( fileName ); //x
  } else if ( format == QueryParameters::SHAPEFILE ) {
    writeShapefile( fileName ); //x
  } else if ( format == QueryParameters::KML && outputKMLFile_ != 0 ) {
    writeKML(); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool PolygonDataset::invariant() const {
  const bool result =
    AND16( Dataset::invariant(),
           variables_        == 1,
           cellType_         == POLYGONS,
           hasArea_          == true,
           hasVolume_        == false,
           isGroup_          == true,
           isStructuredGrid_ == false,
           startingTimestamps_.size() > 0,
           endingTimestamps_.size() == startingTimestamps_.size(),
           coordinates_.size() >= 3,
           data_.size() == startingTimestamps_.size(),
           fileName_.length() > 4,
           polygonShapeData_.count,
           polygonShapeData_.polygonShapes,
           polygonShapeData_.shapeData,
           isValidShapeData( (const ShapeData*) (polygonShapeData_.shapeData)));
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool PolygonDataset::operator==( const PolygonDataset& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND4( equal( other ),
               startingTimestamps_ == other.startingTimestamps_,
               endingTimestamps_ == other.endingTimestamps_,
               polygonShapeData_ == other.polygonShapeData_ ) );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool PolygonDataset::operator!=( const PolygonDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: vprobe - Return nearest data point to time-space location.
INPUTS:  const Timestamp& timestamp  Timestamp of probe.
         const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double elevation      Elevation of probe or MISSING.
                                     Ignored if 2D.
         const int probeLayer        0-based index of variable or IMISSING.
         const int variable          0-based index of variable or IMISSING.
RETURNS: double Data value nearest probe or MISSING if not near any data.
NOTES:   Called by base class member function probe().
         Template Method Design Pattern.
******************************************************************************/

double PolygonDataset::vprobe( const Timestamp timestamp,
                               const double longitude,
                               const double latitude,
                               const double elevation,
                               const int probeLayer,
                               const int variable ) const {
  PRE6( timestamp.invariant(),
        isLongitude( longitude ),
        isLatitude( latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ),
        OR2( probeLayer == IMISSING,
             AND2( probeLayer >= 0, (size_t) probeLayer < layers() ) ),
        OR2( variable == IMISSING,
             IN_RANGE( variable, 0, variables() - 1 ) ) );

  double result = MISSING;
  size_t first = 0;
  size_t last = 0;
  const bool foundMatchingTimestamps =
    matchingTimestamps( timestamp,
                        startingTimestamps_, endingTimestamps_,
                        first, last );

  if ( foundMatchingTimestamps ) {
    const int subsetCount = 1 + last - first;
    const PolygonShape* const polygonShapes = (PolygonShape*)
      polygonShapeData_.polygonShapes;
    const int shape = // Maybe SLOW to search/test triangles for intersection:
      pointInTriangles(longitude, latitude, subsetCount, polygonShapes + first);

    if ( shape > -1 ) {
      CHECK2( shape >= 0, (size_t) shape < data_.size() );
      result = data_[ shape ];
    }
  }

  POST( OR2( result == MISSING,
             IN_RANGE( result, minimum( 0 ), maximum( 0 ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: vsubset - Extract a reference to the subset of data for the given
         time-range for data with stationary (non-time-varying) coordinates.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of subset.
         const Timestamp& endTimestamp    End timestamp of subset.
         const int variable               Index of data variable.
RETURNS: std::vector<Dataset::SubsetPointers>  Array of 1 set of pointers to
                                          data/coordinate subset.
                                          or empty vector if there are no data
                                          within the specified time range.
NOTES:   Called by public member function subsetData().
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
PolygonDataset::vsubset( const Timestamp beginTimestamp,
                         const Timestamp endTimestamp,
                         const int variable ) const {

  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<SubsetPointers> result;
  size_t first = 0;
  size_t last = 0;
  const bool foundMatchingTimestamps =
    matchingTimestamps( beginTimestamp, endTimestamp,
                        startingTimestamps_, endingTimestamps_,
                        first, last );

  if ( foundMatchingTimestamps ) {

    // For each data value (= row = shape) append a group with coordinates:

    const PolygonShape* const polygonShapes = (PolygonShape*)
      polygonShapeData_.polygonShapes;
    const size_t shapes = polygonShapeData_.count;
    size_t offset = 0;

    for ( size_t shape = 0; shape < shapes; ++shape ) {
      const PolygonShape* const thisPolygonShape = polygonShapes + shape;
      const gpc_tristrip thisGPCTristrip = thisPolygonShape->triangles;
      const int num_strips = thisGPCTristrip.num_strips;

      for ( int strip = 0; strip < num_strips; ++strip ) {
        const gpc_vertex_list* const thisGPCVertexList =
          thisGPCTristrip.strip + strip;
        const int num_vertices = thisGPCVertexList->num_vertices;
        const size_t subsetCount = num_vertices;

        if ( IN_RANGE( shape, first, last ) ) {
          CHECK2( shape < data_.size(),
                  offset + subsetCount <= coordinates_.size() );
          const Dataset::SubsetPointers subsetPointers = {
            subsetCount,
            &coordinates_[ offset ],
            1,
            &data_[ shape ],
            0,
            0
          };
          result.push_back( subsetPointers ); //x
        }

        offset += num_vertices;
      }
    }
  }

  // Since smoke plumes overlap, and we want to see the higher values on top,
  // sort by data value from low to high:

  std::stable_sort( result.begin(), result.end(), compareSubsetPointers );

  POST( IMPLIES( result.size(),
                 AND7( result[ 0 ].dataCount == 1,
                       result[ 0 ].coordinatesCount >= 3,
                       result[ 0 ].data,
                       isPoint( result[ 0 ].coordinates[ 0 ] ),
                       isPoint( result[ 0 ].coordinates[
                                  result[ 0 ].coordinatesCount - 1 ] ),
                       IMPLIES( emvl::minimum( result[ 0 ].data,
                                               result[ 0 ].dataCount )
                                  > MISSING,
                                IN_RANGE( emvl::minimum( result[ 0 ].data,
                                                         result[ 0].dataCount ),
                                          minimum( 0 ),
                                          maximum( 0 ) ) ),
                       IMPLIES( emvl::maximum( result[ 0 ].data,
                                               result[ 0 ].dataCount )
                                  > MISSING,
                                IN_RANGE( emvl::maximum( result[ 0 ].data,
                                                         result[ 0].dataCount ),
                                          minimum( 0 ),
                                          maximum( 0 ) ) ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: readXDRHeader - Read XDR header from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: name_
         description_
         variableNames_
         variableUnits_
         variables_
         dataVariable_
         timesteps_
         startingTimestamp_
         endingTimestamp_
         isVector_
         timestepSize_
         startingTimestamps_
         endingTimestamps_
NOTES:   Throws exception upon failure to read valid header from a stream.
******************************************************************************/

void PolygonDataset::readXDRHeader( FILE* stream ) {
  PRE0( stream );

  /* Read XDR header that looks like this:

  Polygon 1.0
  https://www.ospo.noaa.gov/Products/land/hms.html,hmsserver
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Variable names:
  smoke
  # Variable units:
  ug/m3
  # name shxdatabytes shpdatabytes dbfdatabytes and
  # shxfiledata shpfiledata dbffiledata:

  */

  readAndMatchLine( stream, "Polygon 1.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, true ); //x
  readVariablesAndUnits( stream, 1 ); //x
  readAndMatchLine( stream,
                   "# name shxdatabytes shpdatabytes dbfdatabytes and\n" ); //x
  readAndMatchLine( stream, "# shxfiledata shpfiledata dbffiledata:\n" ); //x

  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_

  if ( timestepSize_ == DAYS ) {
    timesteps_ /= 24;
  }

  if ( timesteps_ == 0 ) {
    timesteps_ = 1;
  }

  if ( fileName_.empty() ) { // HACK: Must have fileName_.
    std::string directory( homeDirectory() ); //x
    directory += "/.rsig";
    fileName_ = pathedFileName( directory, QueryParameters::XDR ); //x
  }

  POST011( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ == 1,
           dataVariable_ == variables_ - 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           endingTimestamp_ > startingTimestamp_,
           timesteps_ > 0,
           startingTimestamps_.size() == 0,
           endingTimestamps_.size() == 0 );
}



/******************************************************************************
PURPOSE: readXDRData - Read XDR data from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: coordinates_
         data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void PolygonDataset::readXDRData( FILE* stream ) {
  POST012( stream,
           isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ == 1,
           dataVariable_ == variables_ - 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           endingTimestamp_ > startingTimestamp_,
           timesteps_ > 0,
           startingTimestamps_.size() == 0,
           endingTimestamps_.size() == 0 );

  /* Data looks like this:

  name shxdatabytes shpdatabytes dbfdatabytes
  shxfiledata shpfiledata dbffiledata

  DBF looks like:

  YYYYDDD1 HHMM1 YYYYDDD2 HHMM2 DENS_UGM3
  2018181  1615  2018181  1815   5
  ...
  2018182   130  2018182   130  27

  */

  unbinShapefile( stream ); //x Read from stream and create shx, shp, dbf.
  std::string baseFileName( fileName_, 0, fileName_.length() - 4 ); // No ".xdr"
  int count = 0;
  PolygonShape* const polygonShapes =
    readAndTriangulateShapes( baseFileName.c_str(), &count ); // Can be SLOW.
  bool ok = AND2( polygonShapes != 0, count > 0 );

  if ( ok ) {
    polygonShapeData_.polygonShapes = (void*) polygonShapes;
    polygonShapeData_.count = count;
    ShapeData* const shapeData = readDBF( baseFileName.c_str() );
    ok =
      AND13( shapeData != 0,
             shapeData->rows == count,
             shapeData->columns == 5,
             ! strcmp( shapeData->columnNames[ 0 ], "YYYYDDD1" ),
             ! strcmp( shapeData->columnNames[ 1 ], "HHMM1" ),
             ! strcmp( shapeData->columnNames[ 2 ], "YYYYDDD2" ),
             ! strcmp( shapeData->columnNames[ 3 ], "HHMM2" ),
             ! strcmp( shapeData->columnNames[ 4 ], "DENS_UGM3" ),
             shapeData->columnTypes[ 0 ] == FTInteger,
             shapeData->columnTypes[ 1 ] == FTInteger,
             shapeData->columnTypes[ 2 ] == FTInteger,
             shapeData->columnTypes[ 3 ] == FTInteger,
             shapeData->columnTypes[ 4 ] == FTInteger );

    if ( ok ) {
      polygonShapeData_.shapeData = (void*) shapeData;

      // Read dbf data and convert and store timestamps and data:

      const size_t shapes = count;
      startingTimestamps_.reserve( shapes ); //x
      endingTimestamps_.reserve( shapes ); //x
      data_.reserve( shapes ); //x
      size_t index = 0;
      Timestamp timestamp; // Use a single temp. Default time is now (expensive)
      double minimumValue = MISSING;
      double maximumValue = MISSING;

      for ( size_t row = 0; AND2( ok, row < shapes ); ++row ) {
        const int yyyyddd1 = shapeData->values[ index++ ].i;
        const int hhmmss1  = shapeData->values[ index++ ].i * 100;
        const int yyyyddd2 = shapeData->values[ index++ ].i;
        const int hhmmss2  = shapeData->values[ index++ ].i * 100;
        const int ivalue   = shapeData->values[ index++ ].i;
        ok =
          AND4( Timestamp::isYYYYDDD( yyyyddd1 ),
                Timestamp::isHHMMSS( hhmmss1 ),
                Timestamp::isYYYYDDD( yyyyddd2 ),
                Timestamp::isHHMMSS( hhmmss2 ) );

        if ( ok ) {
          const double value = ivalue > IMISSING ? ivalue : MISSING;
          data_.push_back( value );

          // Compute range:

          if ( minimumValue == MISSING ) {
            minimumValue = maximumValue = value;
          } else if ( value < minimumValue ) {
            minimumValue = value;
          } else if ( value > maximumValue ) {
            maximumValue = value;
          }

          timestamp.yyyydddhhmmss( yyyyddd1, hhmmss1 );
          startingTimestamps_.push_back( timestamp );
          timestamp.yyyydddhhmmss( yyyyddd2, hhmmss2 );
          endingTimestamps_.push_back( timestamp );
        }
      }

      if ( ok ) {

        // Count vertices:

        size_t totalVertices = 0;

        for ( size_t shape = 0; shape < shapes; ++shape ) {
          const PolygonShape* const thisPolygonShape = polygonShapes + shape;
          const gpc_tristrip thisGPCTristrip = thisPolygonShape->triangles;
          const int num_strips = thisGPCTristrip.num_strips;

          for ( int strip = 0; strip < num_strips; ++strip ) {
            const gpc_vertex_list* const thisGPCVertexList =
              thisGPCTristrip.strip + strip;
            const int num_vertices = thisGPCVertexList->num_vertices;
            totalVertices += num_vertices;
          }
        }

        // Copy vertices into coordinates_ and compute bounds_:

        coordinates_.reserve( totalVertices ); //x
        double minimumLongitude = MISSING;
        double maximumLongitude = MISSING;
        double minimumLatitude  = MISSING;
        double maximumLatitude  = MISSING;

        for ( size_t shape = 0; shape < shapes; ++shape ) {
          const PolygonShape* const thisPolygonShape = polygonShapes + shape;
          const gpc_tristrip thisGPCTristrip = thisPolygonShape->triangles;
          const int num_strips = thisGPCTristrip.num_strips;

          for ( int strip = 0; strip < num_strips; ++strip ) {
            const gpc_vertex_list* const thisGPCVertexList =
              thisGPCTristrip.strip + strip;
            const int num_vertices = thisGPCVertexList->num_vertices;

            for ( int vertex = 0; vertex < num_vertices; ++vertex ) {
              const gpc_vertex* const thisGPCVertex =
                thisGPCVertexList->vertex + vertex;
              const double x = thisGPCVertex->x;
              const double y = thisGPCVertex->y;

              if ( ! isValidLongitudeLatitude( x, y ) ) {
                ok = false;
                vertex = num_vertices; // Stop all looping.
                strip = num_strips;
                shape = shapes;
              } else {
                const Point point = { x, y, 0.0 };
                coordinates_.push_back( point );

                if ( minimumLongitude == MISSING ) { // Initialize minmax.
                  minimumLongitude = maximumLongitude = x;
                  minimumLatitude  = maximumLatitude  = y;
                } else { // Update minmax:

                  if ( x < minimumLongitude ) {
                    minimumLongitude = x;
                  } else if ( x > maximumLongitude ) {
                    maximumLongitude = x;
                  }

                  if ( y < minimumLatitude ) {
                    minimumLatitude = y;
                  } else if ( y > maximumLatitude ) {
                    maximumLatitude = y;
                  }
                }
              }
            }
          }
        }

        if ( ok ) {
          minimums_.resize( 1 ); //x
          maximums_.resize( 1 ); //x
          minimums_[ 0 ] = minimumValue;
          maximums_[ 0 ] = maximumValue;
          bounds_.west( minimumLongitude );
          bounds_.east( maximumLongitude );
          bounds_.south( minimumLatitude );
          bounds_.north( maximumLatitude );
        }
      }
    }
  }

  if ( ! AND4( ok, isValidCoordinates(), isValidData(), isValidRange() ) ) {
    throw std::runtime_error( "Failed to read valid polygon data." ); //x
  }

  POST03( coordinates_.size() >= 3,
          data_.size() == startingTimestamps_.size() * variables_,
          isValidRange() );

}



/******************************************************************************
PURPOSE: writeXDR - Write XDR format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void PolygonDataset::writeXDR( FILE* stream ) const {
  PRE( stream );
  const bool ok = streamFile( fileName_.c_str(), stream );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write polygon data as XDR." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
         Writes columns of start/end time and lon-lat bounds per data value.
******************************************************************************/

void PolygonDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   StartTime(UTC)  EndTime(UTC)  West(deg)  East(deg)   South(deg)  North(deg)   smoke(ug/m3)
   2018-11-26T16:15:00-0000    2018-11-26T18:45:00-0000    -86.8100    -85.8100    33.5500    34.5500    5.0000
   ...

  */

  const char* const header =
    "StartTime(UTC)\tEndTime(UTC)\tWest(deg)\tEast(deg)\tSouth(deg)\tNorth(deg)\t%s(%s)\n";
  const char* const format = "%s\t%s\t%11.5f\t%11.5f\t%11.5f\t%11.5f\t%11.5f\n";
  bool ok =
    fprintf( stream, header,
             variableNames_[ 0 ].c_str(), variableUnits_[ 0 ].c_str() )
    >= (int) strlen( header ) - 10;

  if ( ok ) {
    const PolygonShape* const polygonShapes = (PolygonShape*)
      polygonShapeData_.polygonShapes;
    const size_t shapes = polygonShapeData_.count;

    for ( size_t shape = 0; AND2( ok, shape < shapes ); ++shape ) {
      const double value = data_[ shape ];

      if ( value > MISSING ) {
        const PolygonShape* const polygonShape = polygonShapes + shape;
        const double west  = polygonShape->bounds[ LONGITUDE ][ MINIMUM ];
        const double east  = polygonShape->bounds[ LONGITUDE ][ MAXIMUM ];
        const double south = polygonShape->bounds[ LATITUDE  ][ MINIMUM ];
        const double north = polygonShape->bounds[ LATITUDE  ][ MAXIMUM ];
        const Timestamp startingTimestamp( startingTimestamps_[ shape ] );
        const Timestamp endingTimestamp(   endingTimestamps_[   shape ] );
        Timestamp::UTCString startingTimestampString = "";
        Timestamp::UTCString endingTimestampString = "";
        startingTimestamp.toUTCString( startingTimestampString );
        endingTimestamp.toUTCString( endingTimestampString );
        ok = fprintf( stream,
                      format,
                      startingTimestampString, endingTimestampString,
                      west, east, south, north, value );
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write polygon data as ASCII." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void PolygonDataset::writeCOARDS( const std::string& fileName ) const {

  PRE( fileName.length() );

  bool ok = false;
  typedef struct {
    const char* const name;
    const char* const units;
    const char* const description;
    const char* const attributeName;
    const char* const attributeValue;
    const int type;
  } VariableInfo;
  const VariableInfo variableInfo[] = {
    { "west", "degrees_east", "Western extent of smoke plume.",
      "range", "[-180.0, 180.0]", NC_DOUBLE },
    { "east", "degrees_east", "Eastern extent of smoke plume.",
      "range", "[-180.0, 180.0]", NC_DOUBLE },
    { "south", "degrees_north", "Southern extent of smoke plume.",
      "spheroid", "WGS84", NC_DOUBLE },
    { "north", "degrees_north", "Northern extent of smoke plume.",
      "spheroid", "WGS84", NC_DOUBLE },
    { "yyyymmdd1", "yyyymmdd", "Starting date of smoke plume.", 0, 0, NC_INT },
    { "hhmmss1",   "hhmmss",   "Starting time of smoke plume.", 0, 0, NC_INT },
    { "yyyymmdd2", "yyyymmdd", "Ending date of smoke plume.",   0, 0, NC_INT },
    { "hhmmss2",   "hhmmss",   "Ending time of smoke plume.",   0, 0, NC_INT },
    { "smoke",     "ug/m3",   "Smoke density.", 0, 0, NC_DOUBLE }
  };
  const size_t outputVariables = sizeof variableInfo / sizeof *variableInfo;
  const size_t points = startingTimestamps_.size();
  const size_t fileSizeEstimate =
    points * outputVariables * 4 + 10000; // header.
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.

  if ( file ) {
    enum { POINTS, DIMENSIONS };
    const char* const dimensionNames[ DIMENSIONS ] = { "points" };
    int dimensionIds[ DIMENSIONS ] = { -1 };
    ok = createNetCDFDimensions( file, 1, dimensionNames, &points,
                                 dimensionIds ) == 1;

    for ( size_t variable = 0; AND2( ok, variable < outputVariables );
          ++variable ) {
      const VariableInfo* const info = variableInfo + variable;
      const int id =
        createNetCDFVariable( file, info->name, info->units, info->type, 0, 1,
                              dimensionIds );
      ok = id != -1;

      if ( ok ) {
        ok = writeNetCDFTextAttribute( file, id, "units", info->units );

        if ( ok ) {
          ok = writeNetCDFTextAttribute( file, id, "description",
                                         info->description );

          if ( AND2( ok, info->attributeName ) ) {
            ok = writeNetCDFTextAttribute( file, id, info->attributeName,
                                           info->attributeValue );
          }
        }
      }
    }

    if ( ok ) {
      const double domain[ 2 ][ 2 ] = {
        { bounds_.west(), bounds_.east() },
        { bounds_.south(), bounds_.north() }
      };
      std::string history;
      history.reserve( 255 ); //x
      history += description_;
      history += ",RSIG3D";

      ok = writeNetCDFCOARDSAttributes( file, domain, history.c_str() ) == 1;

      if ( ok ) {
        ok = endNetCDFHeaderDefinition( file );

        if ( ok ) {

          // Copy data to temporary output arrays:

          std::vector<float> buffer( outputVariables * points ); //x
          float* const west  = &buffer[ 0 ];
          float* const east  = west  + points;
          float* const south = east  + points;
          float* const north = south + points;
          float* const value = north + points;
          int* const yyyymmdd1 = (int*) ( value + points );
          int* const hhmmss1   = yyyymmdd1 + points;
          int* const yyyymmdd2 = hhmmss1   + points;
          int* const hhmmss2   = yyyymmdd2 + points;

          const PolygonShape* const polygonShapes = (PolygonShape*)
            polygonShapeData_.polygonShapes;
          const size_t shapes = polygonShapeData_.count;
          CHECK( shapes == points );

          for ( size_t shape = 0; shape < shapes; ++shape ) {
            const PolygonShape* const polygonShape = polygonShapes + shape;
            west[  shape ] = polygonShape->bounds[ LONGITUDE ][ MINIMUM ];
            east[  shape ] = polygonShape->bounds[ LONGITUDE ][ MAXIMUM ];
            south[ shape ] = polygonShape->bounds[ LATITUDE  ][ MINIMUM ];
            north[ shape ] = polygonShape->bounds[ LATITUDE  ][ MAXIMUM ];
            value[ shape ] = data_[ shape ];
            const Timestamp startingTimestamp( startingTimestamps_[ shape ] );
            const Timestamp endingTimestamp(   endingTimestamps_[   shape ] );
            yyyymmdd1[ shape ] = startingTimestamp.yyyymmdd();
            hhmmss1[   shape ] = startingTimestamp.hhmmss();
            yyyymmdd2[ shape ] = endingTimestamp.yyyymmdd();
            hhmmss2[   shape ] = endingTimestamp.hhmmss();
          }

          for ( size_t variable = 0; AND2( ok, variable < outputVariables );
                ++variable ) {
            const VariableInfo* const info = variableInfo + variable;
            const char* const variableName = info->name;
            const size_t variableOffset = variable * points;

            if ( info->type == NC_INT ) {
              ok = writeNetCDFAllIntData( file, variableName, &points,
                                         (int*) ( &buffer[ variableOffset ] ) )
                   == 1;
            } else {
              ok = writeNetCDFAllFloatData( file, variableName, &points,
                                            &buffer[ variableOffset ] ) == 1;
            }
          }
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to create NetCDF file for polygon data.");//x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeShapefile - Write dataset as shapefile (shx, shp, dbf).
INPUTS:  const std::string& fileName  Pathed name of file to write.
NOTES:   The name of the file is determined by the dataset name, variable and
         time range and regridding option.
         Throws exception upon failure.
******************************************************************************/

void PolygonDataset::writeShapefile( const std::string& fileName )
  const {

  PRE2( fileName.find_last_of( ".shp" ) == fileName.length() - 4,
        fileName_.length() > 4 );

  std::string outputFileName( fileName ); //x
  const size_t outputLength = outputFileName.length();
  std::string inputFileName( fileName_ ); //x Pathed name of xdr file.
  const size_t inputLength = inputFileName.length();
  inputFileName.replace( inputLength - 3, 3, "shp" ); // Change xdr to shp.
  bool ok = copyFile( inputFileName.c_str(), outputFileName.c_str()) == 1; //shp

  if ( ok ) {
    inputFileName[   inputLength - 1 ] = 'x';
    outputFileName[ outputLength - 1 ] = 'x';
    ok = copyFile( inputFileName.c_str(), outputFileName.c_str() ) == 1; // shx

    if ( ok ) {
      inputFileName.replace(   inputLength - 3, 3, "dbf" );
      outputFileName.replace( outputLength - 3, 3, "dbf" );
      ok = copyFile( inputFileName.c_str(), outputFileName.c_str() ) == 1; //dbf
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to copy polygon shapefile" ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: unbinShapefile - Read from a stream and create shx shp and dbf files.
INPUTS:  FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure.
******************************************************************************/

void PolygonDataset::unbinShapefile( FILE* stream ) const {
  POST02( stream, fileName_.length() > 4 );

  /* Data looks like this:
  name shxdatabytes shpdatabytes dbfdatabytes
  shxfiledata shpfiledata dbffiledata
  */

  size_t shxBytes = 0;
  size_t shpBytes = 0;
  size_t dbfBytes = 0;
  bool ok = fscanf( stream, "%*s %" LU " %" LU " %" LU "\n",
                    &shxBytes, &shpBytes, &dbfBytes ) == 3;

  if ( AND4( ok, shxBytes, shpBytes, dbfBytes ) ) {
    std::string outputFileName( fileName_ ); //x
    const size_t length = outputFileName.length();
    outputFileName.replace( length - 3, 3, "shx" ); // Change xdr to shx.
    ok = emvl::copyFileBytes( stream, outputFileName.c_str(), shxBytes ) == 1;

    if ( ok ) {
      outputFileName[ length - 1 ] = 'p'; // Change shx to shp.
      ok = emvl::copyFileBytes( stream, outputFileName.c_str(), shpBytes ) == 1;

      if ( ok ) {
        outputFileName.replace( length - 3, 3, "dbf" ); // Change shp to dbf.
        ok = emvl::copyFileBytes(stream, outputFileName.c_str(), dbfBytes) == 1;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to unbin polygon shapefile data." ); //x
  }
}



/******************************************************************************
PURPOSE: writeKML - Write dataset as KML file (.kml).
NOTES:   Throws exception upon failure.
******************************************************************************/

void PolygonDataset::writeKML() const {
  PRE( outputKMLFile_ );
  const char* const variable = variableNames_[ dataVariable_ ].c_str();
  std::string source( name_ ); //x
  const size_t index = source.rfind( variable );

  if ( index != std::string::npos && index > 1 ) {
    source.erase( index - 1 );
  }

  const PolygonShape* const polygonShapes = (PolygonShape*)
  polygonShapeData_.polygonShapes;
  const ShapeData* const shapeData = (ShapeData*)
    polygonShapeData_.shapeData;
  const int shapes = (int) polygonShapeData_.count;
  const double dataRange[ 2 ] = { minimum(), maximum() };
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_
    : reverseGrayColor;

  writePolygonsToKML( outputKMLFile_,
                      source.c_str(),
                      variable,
                      variableUnits_[ dataVariable_ ].c_str(),
                      0, 0, shapes, polygonShapes, shapeData,
                      shapeData->columns - 1, -9999.0,
                      dataRange, dataColorFunction );

  POST( true );
}



/******************************************************************************
PURPOSE: reverseGrayColor - Map value in range [minimum, maximum] to
         reverse grayscale range: 0.25 to 0.75.
INPUTS:  double value    Data value to map.
         double minimum  Minimum value for data.
         double maximum  Maximum value for data.
RETURNS: Color mapped value.
******************************************************************************/

static Color reverseGrayColor( double value, double minimum, double maximum ) {
  PRE03( ! isNan( value ), ! isNan( minimum ), ! isNan( maximum ) );
  Color result = { 0.0, 0.0, 0.0 };

  if ( AND2( value != -9999.0, value != MISSING ) ) {
    const double range = maximum - minimum;
    double t = range <= 0.0 ? 0.0 : ( value - minimum ) / range;

    if ( t < 0.0 || isNan( t ) ) {
      t = 0.0;
    } else if ( t > 1.0 ) {
      t = 1.0;
    }

    if ( t <= 0.333333 ) {
      result.r = result.g = result.b = 0.75;
    } else if ( t <= 0.666666 ) {
      result.r = result.g = result.b = 0.5;
    } else {
      result.r = result.g = result.b = 0.25;
    }
  }

  POST0( isValidColor( result ) );
  return result;
}




} // namespace emvl.


