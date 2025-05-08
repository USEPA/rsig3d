/******************************************************************************
PURPOSE: SwathDataset.C - Dataset for swath satellite data.
HISTORY: 2013-10-14 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fscanf().
#include <string.h> // For memset(), strstr().
#include <limits.h> // For INT_MAX.
#include <float.h>  // For FLT_MAX.
#include <unistd.h> // For unlink().

#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <memory>    // For std::auto_ptr.

#include <Assertions.h> // For PRE(), POST(), AND(), IN_RANGE().
#include <KMLFile.h>    // For write*KML().

#include <DataUtilities.h>      // For isNan(), isLongitude(), clampedToRange()
#include <ResourceHandler.h>    // For ResourceHandler.
#include <Bounds.h>             // For Bounds.
#include <Timestamp.h>          // For Timestamp.
#include <QueryParameters.h>    // For QueryParameters.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <NetCDFUtilities.h>    // For *NetCDF*() routines.
#include <SwathDataset.h>       // For SwathDataset.

namespace emvl {


//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~SwathDataset - Destruct a SwathDataset.
******************************************************************************/

SwathDataset::~SwathDataset() {
}



/******************************************************************************
PURPOSE: SwathDataset - Construct a SwathDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

SwathDataset::SwathDataset( FILE* stream ) {
  PRE0( stream );

  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = QUADRILATERAL;
  hasElevation_     = false;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;
  totalPoints_      = 0;
  maximumPoints_    = 0;

  readXDR( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid Swath data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: SwathDataset - Construct a SwathDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

SwathDataset::SwathDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = QUADRILATERAL;
  hasElevation_     = false;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;
  totalPoints_      = 0;
  maximumPoints_    = 0;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid Swath data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: SwathDataset - Copy-construct a SwathDataset.
INPUTS:  const SwathDataset& other  Object to copy.
******************************************************************************/

SwathDataset::SwathDataset( const SwathDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    buffer_        = other.buffer_;
    timestamps_    = other.timestamps_;
    points_        = other.points_;
    totalPoints_   = other.totalPoints_;
    maximumPoints_ = other.maximumPoints_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a SwathDataset.
INPUTS:  const SwathDataset& other  Object to copy.
RETURNS: SwathDataset& *this.
******************************************************************************/

SwathDataset& SwathDataset::operator=( const SwathDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    buffer_        = other.buffer_;
    timestamps_    = other.timestamps_;
    points_        = other.points_;
    totalPoints_   = other.totalPoints_;
    maximumPoints_ = other.maximumPoints_;
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

Dataset* SwathDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: SwathDataset::sample()" ); //x
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

Dataset* SwathDataset::regrid( const int regridMethod,
                              const CMAQGridParameters&
                                cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: SwathDataset::regrid()" ); //x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: write - Write dataset to a directory file in the specified format.
INPUTS:  const std::string& directory  Name of directory to write to.
         const int format              Format of file to write.
NOTES:   The name of the file is determined by the dataset name, variable, etc.
         Throws exception upon failure.
******************************************************************************/

void SwathDataset::write( const std::string& directory,
                          const int format ) const {
  PRE2( directory.length(),
        IN_RANGE( format, 0, QueryParameters::FILE_FORMATS - 1 ) );

  const std::string fileName( pathedFileName( directory, format ) ); //x
  DEBUG( fprintf( stderr, "Writing file '%s'...\n", fileName.c_str() ); )

  if ( IN3( format, QueryParameters::XDR, QueryParameters::ASCII ) ) {
    const char* const name = fileName.c_str();
    ResourceHandler<FILE*> fileCloser( fopen( name, "wb"), fclose );
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
  } else if ( format == QueryParameters::KML ) {
    writeKML(); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool SwathDataset::invariant() const {
  const bool result =
    AND17( Dataset::invariant(),
           cellType_== QUADRILATERAL,
           hasElevation_ == false,
           hasArea_ == true,
           hasVolume_ == false,
           isStationary_ == false,
           isGroup_ == true,
           isStructuredGrid_ == false,
           timestamps_.size(),
           points_.size() == timestamps_.size(),
           emvl::minimum( &points_[ 0 ], points_.size() ) > 0,
           totalPoints_,
           totalPoints_ == sum( &points_[0], points_.size() ),
           maximumPoints_,
           maximumPoints_ <= totalPoints_,
           buffer_.size() == 8 * maximumPoints_,
           IMPLIES_ELSE( dataFileOffset_,
                       AND3( data_.size() == variables_ * maximumPoints_,
                             coordinates_.size() == maximumPoints_ * 4,
                             lastDataTimestep_ < timestamps_.size() ),
                       AND2( data_.size() == variables_ * totalPoints_,
                             coordinates_.size() == totalPoints_ * 4 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool SwathDataset::operator==( const SwathDataset& other ) const {
  PRE( other.invariant() );
  bool result = this == &other;

  if ( ! result ) {
    result =
      AND5( equal( other ),
            timestamps_    == other.timestamps_,
            points_        == other.points_,
            totalPoints_   == other.totalPoints_,
            maximumPoints_ == other.maximumPoints_ );
  }

  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool SwathDataset::operator!=( const SwathDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: scans - Number of satellite scans.
RETURNS: size_t Number of satellite scans.
******************************************************************************/

size_t SwathDataset::scans() const {
  PRE( true );
  const size_t result = timestamps_.size();
  POST( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: timestamp - Timestamp of a satellite scan.
INPUTS:  const size_t scan  Index of scan.
RETURNS: Timestamp of a scan.
******************************************************************************/

Timestamp SwathDataset::timestamp( const size_t scan ) const {
  PRE( scan < timestamps_.size() );
  const Timestamp result = timestamps_[ scan ];
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: points - Points in a satellite scan.
INPUTS:  const size_t scan  Index of scan.
RETURNS: size_t number of points in a scan.
******************************************************************************/

size_t SwathDataset::points( const size_t scan ) const {
  PRE( scan < points_.size() );
  const size_t result = points_[ scan ];
  POST( result >= 1 );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: findNearestPoint - Find scan cell surrounding probe location.
INPUTS:  const size_t scan        Index of scan to search.
         const size_t skipPoints  Sum of all previous/loaded scans points.
         const double longitude   Longitude of probe.
         const double latitude    Latitude of probe.
OUTPUTS: size_t& point            Index of scan point.
RETURNS: bool true if the probe location is within a cell, else false.
******************************************************************************/

bool SwathDataset::probedPoint( const size_t scan,
                                const size_t skipPoints,
                                const double longitude,
                                const double latitude,
                                size_t& point ) const {

  PRE3( scan < points_.size(), isLongitude( longitude ), isLatitude(latitude));

  const double nearEnough = 1e-2; // Probe close enough to data point.
  bool result = false;
  point = 0;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {
    const size_t scanPoints = points_[ scan ];
    const size_t verticesPerCell = 4; // QUADRILATERAL.

    for ( point = 0; AND2( ! result, point < scanPoints ); ++point ) {
      const size_t pointIndex = skipPoints + point;
      const size_t cellIndex = pointIndex * verticesPerCell;
      CHECK( cellIndex <= coordinates_.size() - verticesPerCell );
      result = pointInCell( longitude, latitude, MISSING, verticesPerCell,
                            &coordinates_[ cellIndex ] );
    }

    if ( result ) {
      --point;
    } else {
      point = 0;
    }
  }

  POST( IMPLIES_ELSE( result,
                      AND2( points_[ scan ], point < points_[ scan ] ),
                      point == 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: vprobe - Return nearest data point to time-space location.
INPUTS:  const Timestamp& timestamp  Timestamp of probe.
         const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double elevation      Elevation (ignored).
         const int probeLayer        0-based index of variable or IMISSING.
         const int variable          0-based index of variable.
RETURNS: double Data value nearest probe or MISSING if not near any data.
NOTES:   Called by base class member function probe().
         Template Method Design Pattern.
******************************************************************************/

double SwathDataset::vprobe( const Timestamp timestamp,
                             const double longitude,
                             const double latitude,
                             const double elevation,
                             const int probeLayer,
                             const int variable ) const {
  PRE4( timestamp.invariant(),
        isLongitude( longitude ),
        isLatitude( latitude ),
        IN_RANGE( variable, 0, variables() - 1 ) );

  double result = MISSING;
  const double nearEnough = 1e-2; // Probe close enough to data point.

  if ( isInBounds( longitude, latitude, nearEnough ) ) {
    size_t first = 0;
    size_t last = 0;
    const bool hasMatchingTimestamps =
      matchingTimestamps( timestamp, timestamps_, first, last );

    if ( hasMatchingTimestamps ) {
      CHECK( firstDataTimestep_ < points_.size() );
      size_t skipPoints =
        first == 0 ? 0
        : dataFileOffset_ == 0 ? sum( &points_[ 0 ], first )
        : first <= firstDataTimestep_ ? 0
        : sum( &points_[ firstDataTimestep_ ], first - firstDataTimestep_ );

      for ( size_t scan = first; scan <= last; ++scan ) {
        size_t nearestPoint = 0;
        const bool inScan =
          probedPoint( scan, skipPoints, longitude, latitude, nearestPoint );

        if ( inScan ) {
          const size_t pointIndex = skipPoints + nearestPoint;
          const size_t dataIndex =
            dataFileOffset_ == 0 ?
              variable * totalPoints_   + pointIndex
            : variable * maximumPoints_ + pointIndex;
          CHECK( dataIndex < data_.size() );
          result = data_[ dataIndex ];
          scan = last; // Stop looping.
        } else {
          skipPoints += points_[ scan ];
        }
      }
    }
  }

  POST( OR2( result == MISSING,
             IN_RANGE( result, minimum( variable ), maximum( variable ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: vsubset - Extract a reference to the subset of data for the given
         time-range for data with time-varying coordinates.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of subset.
         const Timestamp& endTimestamp    End timestamp of subset.
         const int variable               0-based index of data variable.
RETURNS: std::vector<SubsetPointers>  Pointers to coordinate and data
                                          subset and count
                                          or empty if there is no data within
                                          the specified time range.
NOTES:   Called by public member function subsetData().
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
SwathDataset::vsubset( const Timestamp beginTimestamp,
                       const Timestamp endTimestamp,
                       const int variable ) const {
  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<Dataset::SubsetPointers> result;
  Timestamp beginHour( beginTimestamp ); // Adjusted to begin/end of hour.
  Timestamp endHour( endTimestamp );

  if ( overlappedTimeRange( beginHour, endHour ) ) {

    /*
     * If non-hourly data and [beginHour, endHour] is 1 hour then
     * expand [beginHour, endHour] to full day.
     */

    if ( AND2( timestepSize_ != HOURS, beginHour.sameDay( endHour ) ) ) {
      beginHour = beginHour.startOfDay();
      endHour   = endHour.endOfDay();
    }

    // Get first and last scan indices within the subset time range:

    size_t first = 0;
    size_t last = 0;
    const bool hasMatchingTimestamps =
      matchingTimestamps( beginHour, endHour, timestamps_, first, last );

    if ( hasMatchingTimestamps ) {
      const size_t verticesPerCell = 4; // QUADRILATERAL.
      const int variable0 = variable == IMISSING ? dataVariable_ : variable;
      CHECK( firstDataTimestep_ < points_.size() );
      size_t skipPoints =
        first == 0 ? 0
        : dataFileOffset_ == 0 ? sum( &points_[ 0 ], first )
        : first <= firstDataTimestep_ ? 0
        : sum( &points_[ firstDataTimestep_ ], first - firstDataTimestep_ );

      for ( size_t scan = first; scan <= last; ++scan ) {
        CHECK( scan < points_.size() );
        const size_t scanPoints = points_[ scan ];
        const size_t dataIndex =
          dataFileOffset_ == 0 ?
            variable0 * totalPoints_   + skipPoints
          : variable0 * maximumPoints_ + skipPoints;
        const size_t coordinateIndex = skipPoints * verticesPerCell;
        CHECK4( dataIndex < data_.size(),
                IN_RANGE( dataIndex + scanPoints, 1, data_.size() ),
                coordinateIndex < coordinates_.size(),
                IN_RANGE( coordinateIndex + scanPoints * verticesPerCell, 1,
                          coordinates_.size() ) );
        const Dataset::SubsetPointers subsetPointerGroup = {
          scanPoints * verticesPerCell,
          &coordinates_[ coordinateIndex ],
          scanPoints,
          &data_[ dataIndex ],
          0,
          0
        };
        result.push_back( subsetPointerGroup ); //x
        skipPoints += scanPoints;
      }
    }
  }

  POST( IMPLIES( result.size(),
                 AND20( result[ 0 ].dataCount,
                        result[ 0 ].coordinatesCount ==
                          result[ 0 ].dataCount * cellVertexCount(),
                        result[ 0 ].coordinates,
                        result[ 0 ].data,
                        isPoint( result[ 0 ].coordinates[ 0 ] ),
                        isPoint( result[ 0 ].coordinates[
                                   result[ 0 ].coordinatesCount - 1 ] ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::minimum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::minimum( result[0].data,
                                                          result[0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::maximum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::maximum( result[0].data,
                                                          result[0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        result[ 0 ].notesCount == 0,
                        result[ 0 ].notes == 0,
                        result[ result.size() - 1 ].dataCount,
                        result[ result.size() - 1 ].coordinatesCount ==
                          result[ result.size() - 1 ].dataCount *
                          cellVertexCount(),
                        result[ result.size() - 1 ].coordinates,
                        result[ result.size() - 1 ].data,
                        isPoint( result[ result.size() - 1 ].coordinates[ 0 ]),
                        isPoint( result[ result.size() - 1 ].coordinates[
                          result[ result.size() - 1 ].coordinatesCount - 1 ] ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::minimum(
                                         result[result.size() - 1].data,
                                         result[result.size() - 1].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::minimum(
                                          result[result.size() - 1].data,
                                          result[result.size() - 1].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::maximum(
                                        result[ result.size() - 1 ].data,
                                        result[ result.size() - 1 ].dataCount )
                                       > MISSING ),
                                 IN_RANGE( emvl::maximum(
                                          result[result.size() - 1].data,
                                          result[result.size() - 1].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        result[ result.size() - 1 ].notesCount == 0,
                        result[ result.size() - 1 ].notes == 0 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: readXDR - Read XDR ASCII header and binary data from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: name_
         description_
         variableNames_
         variableUnits_
         variables_
         dataVariable_
         timesteps_
         rows_
         columns_
         startingTimestamp_
         endingTimestamp_
         timestamps_
         points_
         coordinates_
         data_
NOTES:   Throws exception upon failure to read valid header/data from a stream.
******************************************************************************/

void SwathDataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read XDR ASCII header that looks like this:

  Swath 2.0
  http://www.ssd.noaa.gov/PS/FIRE/GASP/gasp.html,GASPSubset
  2008-06-21T12:00:00-0000
  # Dimensions: variables timesteps scans:
  11 1 2
  # Variable names:
  longitude latitude aod longitude_sw longitude_se longitude_nw longitude_ne \
   latitude_sw latitude_se latitude_nw latitude_ne
  # Variable units:
  deg deg - deg deg deg deg deg deg deg deg
  # Domain: <min_lon> <min_lat> <max_lon> <max_lat>
  -75.03 39 -75 39.1
  # MSB 64-bit integers (yyyydddhhmm) timestamps[scans] and
  # MSB 64-bit integers points[scans] and
  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   data_S[variables][points_S]:

  */

  readAndMatchLine( stream, "Swath 2.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  size_t dimensions[ 3 ] = { 0, 0, 0 };
  readDimensions( stream, sizeof dimensions / sizeof *dimensions,
                  dimensions ); //x
  const size_t tempVariables = dimensions[ 0 ];
  timesteps_ = dimensions[ 1 ];
  const size_t scans = dimensions[ 2 ];
  timestamps_.resize( scans ); //x
  points_.resize( scans ); //x
  readVariablesAndUnits( stream, tempVariables ); //x

  // Remove longitude and latitude variables:

  if ( AND12( variables_ == (int) tempVariables,
              variables_ >= 11,
              variableNames_[ 0 ] == "Longitude",
              variableNames_[ 1 ] == "Latitude",
              variableNames_[ variables_ - 8 ] == "Longitude_SW",
              variableNames_[ variables_ - 7 ] == "Longitude_SE",
              variableNames_[ variables_ - 6 ] == "Longitude_NW",
              variableNames_[ variables_ - 5 ] == "Longitude_NE",
              variableNames_[ variables_ - 4 ] == "Latitude_SW",
              variableNames_[ variables_ - 3 ] == "Latitude_SE",
              variableNames_[ variables_ - 2 ] == "Latitude_NW",
              variableNames_[ variables_ - 1 ] == "Latitude_NE" ) ) {
    CHECK2( variableNames_.size() == tempVariables,
            variableUnits_.size() == tempVariables );

    /*
     * Remove first 2 variables (longitude, latitude) and
     * last 8 (longitude_sw .. latitude_ne):
     */

    variableNames_.resize( variableNames_.size() - 8 );
    variableUnits_.resize( variableUnits_.size() - 8 );
    variableNames_.erase( variableNames_.begin(), variableNames_.begin() + 2 );
    variableUnits_.erase( variableUnits_.begin(), variableUnits_.begin() + 2 );
    variables_ -= 10;
    CHECK3( variables_ > 0,
            (size_t) variables_ == variableNames_.size(),
            (size_t) variables_ == variableUnits_.size() );
    dataVariable_ = variables_ - 1;
  } else {
    throw std::runtime_error( "Read invalid Swath variables/units." ); //x
  }

  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.
  skipLine( stream ); //x
  skipLine( stream ); //x
  readAndMatchLine( stream,
                    "# MSB 64-bit integers (yyyydddhhmm) "
                    "timestamps[scans] and\n" ); //x
  readAndMatchLine( stream, "# MSB 64-bit integers points[scans] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "data_1[variables][points_1] ... "
                    "data_S[variables][points_S]:\n" ); //x

  // Read binary data:

  readScanTimestamps( stream ); //x
  readIntegers( stream, 1LL, LLONG_MAX, points_ ); //x
  readScanData( stream ); //x

  POST021( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ >= 1,
           dataVariable_ == variables_ - 1,
           timesteps_ >= 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           cellType_== QUADRILATERAL,
           hasElevation_ == false,
           hasArea_ == true,
           hasVolume_ == false,
           isStationary_ == false,
           isGroup_ == true,
           isStructuredGrid_ == false,
           timestamps_.size() >= 1,
           points_.size() == timestamps_.size(),
           isValidCoordinates(),
           isValidData(),
           isValidRange() );
}



/******************************************************************************
PURPOSE: readScanTimestamps - Read and convert scan timestamps.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: timestamps_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void SwathDataset::readScanTimestamps( FILE* stream ) {
  PRE02( stream, timestamps_.size() >= 1 );

  /* Read XDR ASCII header that looks like this:
  # MSB 64-bit integers (yyyydddhhmm) timestamps[scans] and
  */

  const size_t scans = timestamps_.size();
  std::vector<long long> yyyydddhhmm( scans ); //x

  if ( fread( &yyyydddhhmm[ 0 ], sizeof yyyydddhhmm[ 0 ], scans, stream )
       == scans ) {
    reverse8ByteWordsIfLittleEndian( &yyyydddhhmm[ 0 ], scans );

    for ( size_t scan = 0; scan < scans; ++scan ) {
      const int yyyyddd = yyyydddhhmm[ scan ] / 10000;
      const int hhmmss  = yyyydddhhmm[ scan ] % 10000 * 100;

      if ( AND2( Timestamp::isYYYYDDD( yyyyddd ),
                 Timestamp::isHHMMSS( hhmmss ) ) ) {
        timestamps_[ scan ].yyyydddhhmmss( yyyyddd, hhmmss );
      } else {
        throw std::runtime_error("Failed to read valid Swath scan timestamp.");
        //x
      }
    }
  }

  POST02( timestamps_[ 0 ].invariant(),
          timestamps_[ timestamps_.size() - 1 ].invariant() );
}



/******************************************************************************
PURPOSE: readScanData - Read and convert scan data.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: coordinates_
         data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void SwathDataset::readScanData( FILE* stream ) {
  PRE07( stream,
         variables_ >= 1,
         coordinates_.size() == 0,
         data_.size() == 0,
         emvl::minimum( &points_[ 0 ], points_.size() ) >= 1,
         minimums_.size() == 0,
         maximums_.size() == 0 );

  /* Read XDR data arrays that looks like this:
  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   data_S[variables][points_S]:
  */

  const size_t scans = points_.size();
  totalPoints_ = sum( &points_[ 0 ], scans );
  maximumPoints_ = computeMaximumPointsPerHour();
  const size_t scanVariables = 2 + variables_ + 8; // lon,lat, aod,... lat_nw
  const size_t datasetBytes = totalPoints_ * scanVariables * sizeof (double);
  const bool useBuffering =
    AND2( ! fileName_.empty(), datasetBytes > PAGE_THREHOLD_GB );
  const size_t verticesPerCell = 4; // QUADRILATERAL: SW, SE, NE, NW.
  const size_t unbufferedDataSize = variables_ * totalPoints_;
  const size_t bufferedDataSize   = variables_ * maximumPoints_;
  const size_t dataSize = useBuffering ? bufferedDataSize : unbufferedDataSize;
  const size_t unbufferedCoordinatesSize = totalPoints_ * verticesPerCell;
  const size_t bufferedCoordinatesSize = maximumPoints_ * verticesPerCell;
  const size_t coordinatesSize =
    useBuffering ? bufferedCoordinatesSize : unbufferedCoordinatesSize;
  buffer_.resize( verticesPerCell * 2 * maximumPoints_ ); //x lon_sw..lat_ne
  data_.resize( dataSize, MISSING ); //x Use MISSING for computeVariableRanges()
  coordinates_.resize( coordinatesSize ); //x
  bool ok = true;

  if ( useBuffering ) {
    CHECK( ! fileName_.empty() );
    const long long offset = FTELL( stream );
    ok = offset > 0;

    if ( ok ) {
      dataFileOffset_ = offset;
      ok = readBufferedDataSubset( 0, true );
    }
  } else { // Read entire dataset into memory:
    ok = readScans( stream, 0, scans - 1, true );
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid Swath data." ); //x
  }

  // Copy first point over uninitialized end points for computeBounds():

  if ( dataFileOffset_ ) {
    const size_t allocatedCoordinatesSize = coordinates_.size();
    const size_t initializedCoordinatesSize = points_[ 0 ] * verticesPerCell;
    const Point& firstPoint = coordinates_[ 0 ];

    for ( size_t point = initializedCoordinatesSize;
          point < allocatedCoordinatesSize;
          ++point ) {
      coordinates_[ point ] = firstPoint;
    }
  }

  computeBounds();
  computeVariableRanges();

  if ( dataFileOffset_ ) { // Compute range over rest of timesteps:
    std::vector<double> minimums( minimums_ ); //x
    std::vector<double> maximums( maximums_ ); //x
    const size_t scans = timestamps_.size();

    for ( size_t scan = 1; AND2( ok, scan < scans ); ++scan ) {
      ok = readBufferedDataSubset( scan, false );

      if ( ok ) {
        computeVariableRanges(); //x Only computes range of loaded data.

        for ( int variable = 0; variable < variables_; ++variable ) {
          const double min = minimums_[ variable ];
          const double max = maximums_[ variable ];

          if ( AND2( min != MISSING,
                     OR2( minimums[ variable ] == MISSING,
                          min < minimums[ variable ] ) ) ) {
            minimums[ variable ] = min;
          }

          if ( max > maximums[ variable ] ) {
            maximums[ variable ] = max;
          }
        }
      }
    }

    for ( int variable = 0; variable < variables_; ++variable ) {
      minimums_[ variable ] = minimums[ variable ];
      maximums_[ variable ] = maximums[ variable ];
    }

    ok = readBufferedDataSubset( 0, false );
  }
}



/******************************************************************************
PURPOSE: computeMaximumPointsPerHour - Compute the maximum over all hours
         of the number of scan points for each hour.
INPUTS:  timestamps_, points_.
RETURNS: size_t the maximum number of scan points.
******************************************************************************/

size_t SwathDataset::computeMaximumPointsPerHour() const {
  PRE02( timestamps_.size(), points_.size() == timestamps_.size() );
  size_t result = 0;
  const size_t scans = timestamps_.size();

  for ( size_t scan = 0; scan < scans; ++scan ) {
    const Timestamp scanTimestamp( timestamps_[ scan ] );
    const size_t scanPoints = points_[ scan ];
    size_t maximumPointsThisHour = scanPoints;

    for ( size_t scan2 = scan + 1; scan2 < scans; ++scan2 ) {
      const Timestamp scanTimestamp2( timestamps_[ scan2 ] );

      if ( scanTimestamp2.sameHour( scanTimestamp ) ) {
        const size_t scanPoints2 = points_[ scan2 ];
        maximumPointsThisHour += scanPoints2;
      }
    }

    if ( maximumPointsThisHour > result ) {
      result = maximumPointsThisHour;
    }
  }

  DEBUG( fprintf( stderr, "maximum( scans[] ) = %" LU ", "
                  "computeMaximumPointsPerHour() = %" LU "\n",
                  emvl::maximum( &points_[ 0 ], points_.size() ), result ); )

  CHECK( result >= emvl::maximum( &points_[ 0 ], points_.size() ) );

  POST0( result );
  return result;
}



/******************************************************************************
PURPOSE: loadedDataSubset - Determine if data is available for the given time
         range and, if available and not already in data_ then read it from
         fileName_ into data_.
INPUTS:  const Timestamp firstTimestamp  Timestamp of start of data needed.
         const Timestamp lastTimestamp   Timestamp of end   of data needed.
OUTPUTS: firstDataTimestep_, lastDataTimestep_, data_.
RETURNS: bool true if data is available, else false if no data is available or
         failed to read from fileName_.
NOTES:   This is called by Dataset::probe() and Dataset::subset().
         If a file open/seek/read failure occurs then a failure message is
         printed to stdout.
         If a partial read occurs then damaged values in data_ are zeroed.
******************************************************************************/

bool SwathDataset::loadedDataSubset( const Timestamp firstTimestamp,
                                     const Timestamp lastTimestamp ) const {
  PRE03( timestamps_.size(), points_.size() == timestamps_.size(),
         IMPLIES( dataFileOffset_, ! fileName_.empty() ) );
  bool result = dataFileOffset_ == 0;

  if ( ! result ) {
    size_t first = 0;
    size_t last = 0;
    result =
      matchingTimestamps( firstTimestamp, lastTimestamp, timestamps_,
                          first, last );

    if ( AND2( result,
               OR2( last == first,
                    timestamps_[ last ].sameHour( timestamps_[ first ] ) ) )) {
      const bool outside = // Is loaded data outside the requested time-range?
        OR2( lastDataTimestep_  < first, firstDataTimestep_ > last );
      result = ! outside; // Nothing to do if requested data is already loaded.

      DEBUG( fprintf( stderr, "SwathDataset::loadedDataSubset(): "
                      "firstDataTimestep_ = %" LU ", lastDataTimestep_ = %" LU ", "
                      "first = %" LU ", last = %" LU ", "
                      "outside = %d\n",
                      firstDataTimestep_, lastDataTimestep_,
                      first, last,
                      outside ); )

      if ( outside ) { // Must read XDR-format data from fileName_ into data_.
        result = readBufferedDataSubset( first, false );
      }
    }
  }

  POST02( isValidData(),
          timestamps_[ firstDataTimestep_ ].sameHour(
            timestamps_[ lastDataTimestep_ ] ) );
  return result;
}



/******************************************************************************
PURPOSE: readBufferedDataSubset - Read variables * BUFFERED_DATA_TIMESTEPS from
         fileName_ into data_.
INPUTS:  const size_t first          0-based index of 1st scan of data to read.
         const bool checkAll         Check all values?
                                     If false just check sampled values.
OUTPUTS: firstDataTimestep_, lastDataTimestep_, data_.
RETURNS: bool true if successful, else false if failed to read from fileName_.
NOTES:   This is called initially by readXDRData() and by loadedDataSubset().
         If a file open/seek/read failure occurs then a failure message is
         printed to stdout.
         If a partial read occurs then damaged values in data_ are zeroed.
******************************************************************************/

bool SwathDataset::readBufferedDataSubset( const size_t first,
                                           const bool checkAll ) const {
  PRE06( dataFileOffset_,
         ! fileName_.empty(),
         GT_ZERO6( variables_, timesteps_, timestamps_.size(), points_.size(),
                   totalPoints_, maximumPoints_ ),
         buffer_.size() == 8 * maximumPoints_,
         data_.size() ==
           variables_ * BUFFERED_DATA_TIMESTEPS * maximumPoints_,
         coordinates_.size() == BUFFERED_DATA_TIMESTEPS * maximumPoints_ * 4 );

  bool result = false;
  ResourceHandler<FILE*> fileCloser( fopen( fileName_.c_str(), "rb" ), fclose);
  FILE* const file = fileCloser.get();

  if ( file ) {

    // Compute index of last scan matching hour of first:

    size_t last = first;
    Timestamp firstTimestamp( timestamps_[ first ] );
    const size_t scans = timestamps_.size();

    for ( size_t scan = first + 1;
          AND2( scan < scans, timestamps_[ scan ].sameHour( firstTimestamp ) );
          ++scan ) {
      last = scan;
    }

    result = readScans( file, first, last, checkAll );

    if ( result ) {
      firstDataTimestep_ = first;
      lastDataTimestep_  = last;
    }
  }

  POST0( IMPLIES( result, AND2( isValidCoordinates(), isValidData() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: readScans - Read scans fileName_ into data_ and coordinates_.
INPUTS:  FILE* stream          File/stream to read from.
         const size_t first    0-based index of first scan of data to read.
         const size_t last     0-based index of last  scan of data to read.
         const bool checkAll   Check all values?
                               If false just check sampled values.
OUTPUTS: data_, coordinates_
RETURNS: bool true if successful, else false if failed to read from fileName_.
******************************************************************************/

bool SwathDataset::readScans( FILE* stream,
                              const size_t first,
                              const size_t last,
                              const bool checkAll ) const {

  PRE07( stream,
         IMPLIES( first, dataFileOffset_ ),
         GT_ZERO6( variables_, timesteps_, timestamps_.size(), points_.size(),
                   totalPoints_, maximumPoints_ ),
         first <= last,
         last < points_.size(),
         buffer_.size() == 8 * maximumPoints_,
         IMPLIES_ELSE( dataFileOffset_,
                       AND4( ! fileName_.empty(),
                             timestamps_[ first ].sameHour( timestamps_[last]),
                             data_.size() == variables_ * maximumPoints_,
                             coordinates_.size() == maximumPoints_ * 4 ),
                       AND2( data_.size() == variables_ * totalPoints_,
                             coordinates_.size() == totalPoints_ * 4 ) ) );
  bool result = true;

  /*
   * Read XDR-format data from fileName_ into data_.
   * XDR file data looks like this:
   * # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   * data_S[variables][points_S]:
   */

  const size_t skipPoints = first > 0 ? sum( &points_[ 0 ], first ) : 0;
  const size_t scanVariables = 2 + variables_ + 8; // lon,lat, aod,... lat_nw
  size_t coordinateIndex = 0;
  size_t dataIndex = 0;
  double west = MISSING;
  double east = MISSING;
  double south = MISSING;
  double north = MISSING;

  DEBUG( fprintf( stderr, "readScans( first = %" LU ", last = %" LU ", checkAll = %d )"
                  " dataFileOffset_ = %" LU ", maximumPoints_ = %" LU ", buffer_.size() = %" LU ".\n",
                  first, last, checkAll, dataFileOffset_, maximumPoints_,
                  buffer_.size() ); )

  if ( dataFileOffset_ ) {
    const size_t seekOffset =
      dataFileOffset_ + scanVariables * skipPoints * sizeof (double);
    result = FSEEK( stream, seekOffset, SEEK_SET ) == 0;
    DEBUG( fprintf( stderr, "seek to file byte %" LU " of first scan.\n",
                    seekOffset ); )
  }

  for ( size_t scan = first; AND2( result, scan <= last ); ++scan ) {
    const size_t scanPoints = points_[ scan ];

    DEBUG( fprintf( stderr, "scanPoints[%" LU "] = %" LU "\n", scan, scanPoints ); )

    for ( size_t scanVariable = 0, dataVariable = 0;
          AND2( result, scanVariable < scanVariables );
          ++scanVariable ) {
      double minimumValue = MISSING;
      double maximumValue = MISSING;

      DEBUG( fprintf( stderr, "scanVariable = %" LU "\n", scanVariable ); )

      if ( scanVariable < 2 ) { // Skip lon or lat:

        if ( dataFileOffset_ ) { // Use fseek() to skip:
          const size_t skipBytes = 2 * scanPoints * sizeof (double);
          DEBUG( fprintf( stderr, "Seek/skipping %" LU " bytes\n", skipBytes ); )
          result = FSEEK( stream, skipBytes, SEEK_CUR ) == 0;
        } else { // Must use fread() into buffer:
          const size_t readWords = 2 * scanPoints;

          DEBUG( fprintf( stderr,
                         "Read/skip lon, lat: Reading/ignoring %" LU " words\n",
                          readWords ); )
          CHECK( readWords <= buffer_.size() );
          result =
            fread( &buffer_[ 0 ], sizeof buffer_[ 0 ], readWords, stream )
            == readWords;
        }

        ++scanVariable; // Skip ahead since 2 variables were processed.
      } else {
        const size_t readWords =
          scanVariable < scanVariables - 8 ? scanPoints : scanPoints * 8;
        DEBUG( fprintf( stderr, "Reading %" LU " words\n", readWords ); )
        CHECK( readWords <= buffer_.size() );
        result = fread( &buffer_[ 0 ], sizeof buffer_[ 0 ], readWords, stream )
                 == readWords;

        if ( result ) {
          reverse8ByteWordsIfLittleEndian( &buffer_[ 0 ], readWords );

         if ( scanVariable >= scanVariables - 8 ) {

            // Store 8 corner coords longitude_sw...lat_ne in coordinates_:

            const double* const buffer0 = &buffer_[ 0 ];
            const double* const longitudesSW = buffer0;
            const double* const longitudesSE = buffer0 + scanPoints;
            const double* const longitudesNW = buffer0 + scanPoints * 2;
            const double* const longitudesNE = buffer0 + scanPoints * 3;
            const double* const latitudesSW  = buffer0 + scanPoints * 4;
            const double* const latitudesSE  = buffer0 + scanPoints * 5;
            const double* const latitudesNW  = buffer0 + scanPoints * 6;
            const double* const latitudesNE  = buffer0 + scanPoints * 7;

            for ( size_t point = 0;
                  AND2( result, point < scanPoints );
                  ++point ) {
              CHECK( point < buffer_.size() );
              const double longitudeSW = longitudesSW[ point ];
              const double longitudeSE = longitudesSE[ point ];
              const double longitudeNW = longitudesNW[ point ];
              const double longitudeNE = longitudesNE[ point ];
              const double latitudeSW  = latitudesSW[  point ];
              const double latitudeSE  = latitudesSE[  point ];
              const double latitudeNW  = latitudesNW[  point ];
              const double latitudeNE  = latitudesNE[  point ];

              result =
                AND8( isLongitude( longitudeSW ),
                      isLongitude( longitudeSE ),
                      isLongitude( longitudeNW ),
                      isLongitude( longitudeNE ),
                      isLatitude( latitudeSW ),
                      isLatitude( latitudeSE ),
                      isLatitude( latitudeNW ),
                      isLatitude( latitudeNE ) );

              if ( result ) {
                const Point pointSW = { longitudeSW, latitudeSW, 0.0 };
                const Point pointSE = { longitudeSE, latitudeSE, 0.0 };
                const Point pointNW = { longitudeNW, latitudeNW, 0.0 };
                const Point pointNE = { longitudeNE, latitudeNE, 0.0 };

                // Store vertices in counter-clockwise order:

                CHECK( coordinateIndex + 3 < coordinates_.size() );
                coordinates_[ coordinateIndex++ ] = pointSW;
                coordinates_[ coordinateIndex++ ] = pointSE;
                coordinates_[ coordinateIndex++ ] = pointNE;
                coordinates_[ coordinateIndex++ ] = pointNW;

                // Update bounds:

                if ( west < -180.0 ) { // Initialize to first point:
                  west  = longitudeSW;
                  east  = longitudeSE;
                  south = latitudeSW;
                  north = latitudeSE;
                } else {
                  west  = std::min<double>( west, longitudeSW );
                  west  = std::min<double>( west, longitudeNW );
                  east  = std::max<double>( east, longitudeSE );
                  east  = std::max<double>( east, longitudeNE );
                  south = std::min<double>( south, latitudeSW );
                  south = std::min<double>( south, latitudeSE );
                  north = std::max<double>( north, latitudeNW );
                  north = std::max<double>( north, latitudeNE );
                }
              }
            }

            scanVariable = scanVariables; // Stop looping.
          } else {

            // Store data variable into data_:

            for ( size_t point = 0;
                  AND2( result, point < scanPoints );
                  ++point ) {
              CHECK( point < buffer_.size() );
              const double value = buffer_[ point ];
              result = IMPLIES( checkAll, isFinite( value ) );

              if ( result ) {
                CHECK( dataIndex < data_.size() );
                data_[ dataIndex++ ] = value;

                // Update variable range:

                if ( minimumValue == MISSING ) { // Initialize to first value:
                  minimumValue = maximumValue = value;
                } else {
                  minimumValue = std::min<double>( minimumValue, value );
                  maximumValue = std::max<double>( maximumValue, value );
                }
              }
            }

            // Expand variable ranges to loaded scans:

            if ( dataVariable < minimums_.size() ) {
              minimums_[ dataVariable ] =
                std::min<double>( minimums_[ dataVariable ], minimumValue );
              maximums_[ dataVariable ] =
                std::max<double>( maximums_[ dataVariable ], maximumValue );

              DEBUG( fprintf( stderr, "updated minmax[%" LU "] = [%f, %f]\n",
                              dataVariable,
                              minimums_[ dataVariable ],
                              maximums_[ dataVariable ] ); )

              ++dataVariable;
            }
          }
        }
      }
    } // End loop on scan variable.
  } // End loop on scan.

  if ( result ) {

    if ( ! checkAll ) {
      result = isValidData(); // Check only sample values (for speed).
    }

    // Expand bounds to bounds of loaded scans:

    west  = std::min<double>( west, east );
    east  = std::max<double>( west, east );
    south = std::min<double>( south, north );
    north = std::max<double>( south, north );

    if ( west < bounds_.west() ) {
      bounds_.west( west );
    }

    if ( east > bounds_.east() ) {
      bounds_.east( east );
    }

    if ( south < bounds_.south() ) {
      bounds_.south( south );
    }

    if ( north > bounds_.north() ) {
      bounds_.north( north );
    }

    DEBUG( fprintf( stderr, "updated bounds_ = [%f, %f][%f, %f]\n",
                    bounds_.west(), bounds_.east(),
                    bounds_.south(), bounds_.north() ); )
  }

  if ( ! result ) {
    fprintf( stderr,
             "\nI'm sorry, Failed to read valid Swath data for scans "
             "[%" LU ", %" LU "].\n", first, last );
  }

  POST0( IMPLIES( result, AND2( isValidCoordinates(), isValidData() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: writeXDR - Write XDR format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SwathDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  bool ok = false;

  if ( ! fileName_.empty() ) { // If source xdr file exists then copy it:
    ResourceHandler<FILE*> fileCloser( fopen(fileName_.c_str(), "rb"), fclose);
    FILE* const input = fileCloser.get();
    ok = input != 0;

    if ( ok ) {
      ok = copyStream( input, stream );
    }
  } else {

    /* Write XDR header that looks like this:

    Swath 2.0
    http://www.ssd.noaa.gov/PS/FIRE/GASP/gasp.html,GASPSubset
    2008-06-21T12:00:00-0000
    # Dimensions: variables timesteps scans:
    11 1 2
    # Variable names:
    longitude latitude aod longitude_sw longitude_se longitude_nw longitude_ne \
     latitude_sw latitude_se latitude_nw latitude_ne
    # Variable units:
    deg deg - deg deg deg deg deg deg deg deg
    # Domain: <min_lon> <min_lat> <max_lon> <max_lat>
    -75.03 39 -75 39.1
    # MSB 64-bit integers (yyyydddhhmm) timestamps[scans] and
    # MSB 64-bit integers points[scans] and
    # IEEE-754 64-bit reals data_1[variables][points_1] ... \
     data_S[variables][points_S]:

    */

    Timestamp::UTCString timestamp = "";
    startingTimestamp_.toUTCString( timestamp );
    const char* const format1 =
      "Swath 2.0\n"
      "%s\n"
      "%s\n"
      "# Dimensions: variables timesteps scans:\n"
      "%d %" LU " %" LU "\n";
    ok =
      fprintf( stream, format1,
               description_.c_str(),
               timestamp,
               2 + variables_ + 8, timesteps_, timestamps_.size() )
      > (int) strlen( format1 );

    if ( ok ) { // Write variable names:
      const char* const format2 = "# Variable names:\nLongitude Latitude";

      ok = fprintf( stream, format2 ) == (int) strlen( format2 );

      for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
        const char* const name = variableNames_[ variable ].c_str();
        ok = fprintf( stream, " %s", name ) == (int) ( 1 + strlen( name ) );
      }

      const char* const extraVariableNames =
        " Longitude_SW Longitude_SE Longitude_NW Longitude_NE"
        " Latitude_SW Latitude_SE Latitude_NW Latitude_NE";

      ok = AND2( ok, fprintf( stream, "%s\n", extraVariableNames ) ==
                     (int) ( 1 + strlen( extraVariableNames ) ) );

      if ( ok ) { // Write variable units:
        const char* const format3 = "# Variable units:\ndeg deg";
        ok = fprintf( stream, format3 ) == (int) strlen( format3 );

        for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
          const char* const units = variableUnits_[ variable ].c_str();
          ok = fprintf( stream, " %s", units ) == (int) ( 1 + strlen( units ) );
        }

        const char* const extraVariableUnits =
          " deg deg deg deg deg deg deg deg";

        ok = AND2( ok, fprintf( stream, "%s\n", extraVariableUnits ) ==
                       (int) ( 1 + strlen( extraVariableUnits ) ) );

        if ( ok ) { // Write Domain:
          const char* const format4 =
            "# Domain: <min_lon> <min_lat> <max_lon> <max_lat>\n";
          ok = fprintf( stream, format4 ) == (int) strlen( format4 );
          ok = AND2( ok, fprintf( stream, "%lg %lg %lg %lg\n",
                                  bounds_.west(), bounds_.south(),
                                  bounds_.east(), bounds_.north() ) > 4 );

          if ( ok ) {
            const char* const end =
              "# MSB 64-bit integers (yyyydddhhmm) timestamps[scans] and\n"
              "# MSB 64-bit integers points[scans] and\n"
              "# IEEE-754 64-bit reals data_1[variables][points_1] ... "
              "data_S[variables][points_S]:\n";
            ok = fprintf( stream, "%s", end ) == (int) strlen( end );

            if ( ok ) {
              writeScanTimestamps( stream ); //x
              writeIntegers( stream, 1LL, LLONG_MAX, points_ ); //x
              writeScanData( stream ); //x
            }
          }
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Swath data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeScanTimestamps - Write XDR format scan timestamps to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SwathDataset::writeScanTimestamps( FILE* stream ) const {
  PRE( stream );

  /* Write XDR timestamps that looks like this:
  # MSB 64-bit integers (yyyydddhhmm) timestamps[scans] and
  */

  const size_t scans = timestamps_.size();
  std::vector<long long> yyyydddhhmm( scans ); //x

  for ( size_t scan = 0; scan < scans; ++scan ) {
    const Timestamp timestamp = timestamps_[ scan ];
    const long long yyyyddd = timestamp.yyyyddd();
    const long long hhmm    = timestamp.hhmmss() / 100;
    const long long timestamp2 = yyyyddd * 10000LL + hhmm;
    CHECK2( Timestamp::isYYYYDDD( timestamp2 / 10000 ),
            Timestamp::isHHMMSS( timestamp2 % 10000 * 100 ) );
    yyyydddhhmm[ scan ] = timestamp2;
  }

  reverse8ByteWordsIfLittleEndian( &yyyydddhhmm[ 0 ], scans );
  const bool ok =
    fwrite( &yyyydddhhmm[ 0 ], sizeof yyyydddhhmm[0], scans, stream ) == scans;

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Swath timestamps." );
    //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeScanData - Write XDR format scan data to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SwathDataset::writeScanData( FILE* stream ) const {
  PRE( stream );

  /* Write XDR data that looks like this:

  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   data_S[variables][points_S]:

  */

  const size_t scans = points_.size();
  const size_t scanVariables = 2 + variables_ + 8; // lon,lat, aod,... lat_ne
  const size_t verticesPerCell = 4; // QUADRILATERAL.
  bool ok = true;

  for ( size_t scan = 0,
       dataIndex = 0,
       longitudeIndex = 0,
       latitudeIndex = 0,
       longitudeSWIndex = 0,
       longitudeSEIndex = 0,
       longitudeNEIndex = 0,
       longitudeNWIndex = 0,
       latitudeSWIndex = 0,
       latitudeSEIndex = 0,
       latitudeNEIndex = 0,
       latitudeNWIndex = 0;
        AND2( ok, scan < scans );
        ++scan ) {
    const size_t scanPoints = points_[ scan ];

    if ( dataFileOffset_ ) { // If paging load dataset if not already loaded
      const bool loaded = IN_RANGE(scan, firstDataTimestep_, lastDataTimestep_);

      if ( ! loaded ) {
        ok = readBufferedDataSubset( scan, true );
        dataIndex = 0;
        longitudeIndex = 0;
        latitudeIndex = 0;
        longitudeSWIndex = 0;
        longitudeSEIndex = 0;
        longitudeNEIndex = 0;
        longitudeNWIndex = 0;
        latitudeSWIndex = 0;
        latitudeSEIndex = 0;
        latitudeNEIndex = 0;
        latitudeNWIndex = 0;
      }
    }

    for ( size_t scanVariable = 0; AND2( ok, scanVariable < scanVariables );
          ++scanVariable ) {

      for ( size_t point = 0; point < scanPoints; ++point ) {
        double value = 0.0;

        if ( scanVariable == 0 ) { // longitude:
          CHECK( longitudeIndex + 3 < coordinates_.size() );
          value = 0.25 * (
            coordinates_[ longitudeIndex     ].longitude +
            coordinates_[ longitudeIndex + 1 ].longitude +
            coordinates_[ longitudeIndex + 2 ].longitude +
            coordinates_[ longitudeIndex + 3 ].longitude );
          longitudeIndex += verticesPerCell;
        } else if ( scanVariable == 1 ) { // latitude:
          CHECK( latitudeIndex + 3 < coordinates_.size() );
          value = 0.25 * (
            coordinates_[ latitudeIndex     ].latitude +
            coordinates_[ latitudeIndex + 1 ].latitude +
            coordinates_[ latitudeIndex + 2 ].latitude +
            coordinates_[ latitudeIndex + 3 ].latitude );
          latitudeIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 8 ) { // longitude_sw:
          CHECK( longitudeSWIndex + SW < coordinates_.size() );
          value = coordinates_[ longitudeSWIndex + SW ].longitude;
          longitudeSWIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 7 ) { // longitude_se:
          CHECK( longitudeSEIndex + SE < coordinates_.size() );
          value = coordinates_[ longitudeSEIndex + SE ].longitude;
          longitudeSEIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 6 ) { // longitude_nw:
          CHECK( longitudeNWIndex + NW < coordinates_.size() );
          value = coordinates_[ longitudeNWIndex + NW ].longitude;
          longitudeNWIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 5 ) { // longitude_ne:
          CHECK( longitudeNEIndex + NE < coordinates_.size() );
          value = coordinates_[ longitudeNEIndex + NE ].longitude;
          longitudeNEIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 4 ) { // latitude_sw:
          CHECK( latitudeSWIndex + SW < coordinates_.size() );
          value = coordinates_[ latitudeSWIndex + SW ].latitude;
          latitudeSWIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 3 ) { // latitude_se:
          CHECK( latitudeSEIndex + SE < coordinates_.size() );
          value = coordinates_[ latitudeSEIndex + SE ].latitude;
          latitudeSEIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 2 ) { // latitude_nw:
          CHECK( latitudeNWIndex + NW < coordinates_.size() );
          value = coordinates_[ latitudeNWIndex + NW ].latitude;
          latitudeNWIndex += verticesPerCell;
        } else if ( scanVariable == scanVariables - 1 ) { // latitude_ne:
          CHECK( latitudeNEIndex + NE < coordinates_.size() );
          value = coordinates_[ latitudeNEIndex + NE ].latitude;
          latitudeNEIndex += verticesPerCell;
        } else { // Data:
          CHECK( dataIndex < data_.size() );
          value = data_[ dataIndex ];
          ++dataIndex;
        }

        CHECK( point < buffer_.size() );
        buffer_[ point ] = value;
      }

      CHECK( scanPoints < buffer_.size() );
      reverse8ByteWordsIfLittleEndian( &buffer_[ 0 ], scanPoints );
      ok = fwrite( &buffer_[ 0 ], sizeof buffer_[ 0 ], scanPoints, stream )
           == scanPoints;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Swath data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SwathDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  Longitude(deg)  Latitude(deg)   AOD(-)
   2011-08-29T22:00:00-0000  -91.03617  31.00816   0.05632
   ...

  */

  // Write header row:

  const char* const header = "Timestamp(UTC)\tLongitude(deg)\tLatitude(deg)";
  bool ok = fprintf( stream, "%s", header ) == (int) strlen( header );
  const char* const headerFormat = "\t%s(%s)";

  for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
    ok = fprintf( stream, headerFormat,
                  variableNames_[ variable ].c_str(),
                  variableUnits_[ variable ].c_str() ) > 4;
  }

  if ( ok ) { // Also write corner coordinates header line:
    const char* const header_corners =
      "\tLongitude_SW(deg)\tLongitude_SE(deg)"
      "\tLongitude_NW(deg)\tLongitude_NE(deg)"
      "\tLatitude_SW(deg)\tLatitude_SE(deg)"
      "\tLatitude_NW(deg)\tLatitude_NE(deg)\n";
    ok = fprintf( stream, "%s", header_corners ) ==
         (int) strlen( header_corners );
  }

  // Write data rows:

  if ( ok ) {
    const char* const dataFormatF = "\t%11.5f";
    const char* const dataFormatE = "\t%11.5e";
    const size_t wordSize = 48;
    char word[ wordSize ] = "";
    const size_t verticesPerCell = 4; // QUADRILATERAL.
    const size_t scans = points_.size();
    const size_t bufferSize = // Time,lon,lat + vars + corner-lons + corner-lats
      ( 3 + variables_ + 4 + 4 ) * maximumPoints_ * wordSize + 1;
    std::vector<char> buffer( bufferSize ); //x
    size_t pointIndex = 0;

    for ( size_t scan = 0; AND2( ok, scan < scans ); ++scan ) {
      const size_t points = points_[ scan ];
      const Timestamp timestamp = timestamps_[ scan ];
      Timestamp::UTCString utcString = "";
      timestamp.toUTCString( utcString );
      buffer.assign( buffer.size(), '\0' );
      char* outputBuffer = &buffer[ 0 ];

      if ( dataFileOffset_ ) { // If paging load dataset if not already loaded
        const bool loaded =
          IN_RANGE( scan, firstDataTimestep_, lastDataTimestep_ );

        if ( ! loaded ) {
          ok = readBufferedDataSubset( scan, true );
          pointIndex = 0;
        }
      }

      if ( ok ) {

        // Write one scan worth of buffered text (for performance):

        for ( size_t point = 0; point < points; ++point, ++pointIndex ) {
          strncpy( outputBuffer, utcString, Timestamp::UTC_STRING_LENGTH );
          outputBuffer += Timestamp::UTC_STRING_LENGTH;

          // Compute and write (quadrillateral center) longitude and latitude:

          const size_t coordinateIndex = pointIndex * verticesPerCell;
          CHECK( coordinateIndex < coordinates_.size() );
          const Point& pointSW = coordinates_[ coordinateIndex + SW ];
          const Point& pointSE = coordinates_[ coordinateIndex + SE ];
          const Point& pointNE = coordinates_[ coordinateIndex + NE ];
          const Point& pointNW = coordinates_[ coordinateIndex + NW ];
          const double longitude = 0.25 * (
            pointSW.longitude + pointSE.longitude +
            pointNE.longitude + pointNW.longitude );
          const double latitude = 0.25 * (
            pointSW.latitude + pointSE.latitude +
            pointNE.latitude + pointNW.latitude );

          size_t count =
            snprintf( word, sizeof word / sizeof *word, dataFormatF, longitude);
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count =
            snprintf( word, sizeof word / sizeof *word, dataFormatF, latitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          // Write data variables:

          for ( int variable = 0; variable < variables_; ++variable ) {
            const size_t variableSize =
              dataFileOffset_ == 0 ? totalPoints_ : maximumPoints_;
            const size_t dataIndex = variable * variableSize + pointIndex;
            CHECK( dataIndex < data_.size() );
            count = snprintf( word, sizeof word / sizeof *word, dataFormatE,
                              data_[ dataIndex ] );
            CHECK2( count < sizeof word / sizeof *word,
                    strlen( word ) == count );
            strncpy( outputBuffer, word, count );
            outputBuffer += count;
          }

          // Write corner coordinates:

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                           pointSW.longitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                            pointSE.longitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                            pointNW.longitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                            pointNE.longitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                            pointSW.latitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                            pointSE.latitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                            pointNW.latitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                            pointNE.latitude );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          strcpy( outputBuffer, "\n" ); // End of spreadsheet row.
          ++outputBuffer;
        }

        // Write buffered output to stream:

        const size_t bufferLength = (size_t) ( outputBuffer - &buffer[ 0 ] );
        CHECK2( bufferLength <= bufferSize,
                strlen( &buffer[ 0 ] ) < bufferSize );
        ok = fwrite( &buffer[ 0 ], sizeof (char), bufferLength, stream )
               == bufferLength;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write ASCII format Swath data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SwathDataset::writeCOARDS( const std::string& fileName ) const {
  PRE( fileName.length() );

  const size_t outputVariables =
    // yyyymmdd,hhmmss,time + lon,lat + aod,... + corner-lons + corner-lats.
    3 + 2 + variables_ + 4 + 4;
  const size_t fileSizeEstimate = outputVariables * totalPoints_ * 4 + 5000;
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    const char* const dimensionName = "points";
    int dimensionId = -1;
    const size_t dimension = totalPoints_;

    if ( createNetCDFDimensions( file, 1, &dimensionName, &dimension,
                                 &dimensionId ) ) {

      // Create temporary copy of timestamps per point:

      std::vector<Timestamp> timestamps( totalPoints_ ); //x
      const size_t scans = points_.size();

      for ( size_t scan = 0, index = 0; scan < scans; ++scan ) {
        const size_t scanPoints = points_[ scan ];
        const Timestamp scanTimestamp = timestamps_[ scan ];

        for ( size_t point = 0; point < scanPoints; ++point, ++index ) {
          timestamps[ index ] = scanTimestamp;
        }
      }

      writeCOARDSCommon( file, 0, false, true, false, dataFileOffset_ == 0, 1,
                         1, &dimensionId, &dimension, &dimensionName,
                         timestamps ); //x

      if ( dataFileOffset_ ) {
        writeBufferedCOARDSCoordinatesAndVariables( file ); //x
      }

      ok = true;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create NetCDF-COARDS file for "
                              "Swath data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeBufferedCOARDSCoordinatesAndVariables - Write COARDS-format
         coordinates and variables using buffer_.
INPUTS:  const int file  NetCDF file to write to.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SwathDataset::writeBufferedCOARDSCoordinatesAndVariables( const int file )
  const {

  PRE5( file > 0, dataFileOffset_, ! fileName_.empty(), maximumPoints_,
        buffer_.size() >= maximumPoints_ );

  /*
   * File data looks like this:
   *  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   *    data_S[variables][points_S]:
   */

  ResourceHandler<FILE*> fileCloser( fopen( fileName_.c_str(), "rb" ), fclose );
  FILE* const input = fileCloser.get();
  bool ok = input != 0;

  if ( ok ) {
    const char* const coordinateVariableNames[ 10 ] = {
      "longitude",
      "latitude",
      "longitudeSW",
      "longitudeSE",
      "longitudeNW",
      "longitudeNE",
      "latitudeSW",
      "latitudeSE",
      "latitudeNW",
      "latitudeNE"
    };
    const size_t scanVariables =
      sizeof coordinateVariableNames / sizeof coordinateVariableNames[ 0 ] +
      variables_;
    const size_t scans = points_.size();
    ok = FSEEK( input, dataFileOffset_, SEEK_SET ) == 0;

    for ( size_t scan = 0, pointsWritten = 0; AND2(ok, scan < scans); ++scan) {
      const size_t pointsInScan = points_[ scan ];

      for ( size_t scanVariable = 0, coordinateVariable = 0, dataVariable = 0;
            AND2( ok, scanVariable < scanVariables );
            ++scanVariable ) {
        CHECK( pointsInScan <= buffer_.size() );
        ok = fread( &buffer_[ 0 ], sizeof buffer_[ 0 ], pointsInScan, input )
             == pointsInScan;

        if ( ok ) {
          reverse8ByteWordsIfLittleEndian( &buffer_[ 0 ], pointsInScan );
          CHECK( pointsWritten <= totalPoints_ - pointsInScan );
          const char* variableName = 0;

          if ( OR2( scanVariable < 2, scanVariable >= scanVariables - 8 ) ) {
            CHECK( coordinateVariable <
                     sizeof coordinateVariableNames /
                     sizeof coordinateVariableNames[ 0 ] );
            variableName = coordinateVariableNames[ coordinateVariable ];
            ++coordinateVariable;
          } else {
            CHECK( dataVariable < variableNames_.size() );
            variableName = variableNames_[ dataVariable ].c_str();
            ++dataVariable;
          }

          CHECK2( variableName, *variableName );
          ok = writeNetCDFSomeDoubleData( file, variableName,
                                          pointsWritten, &pointsInScan,
                                          &buffer_[ 0 ] );
        }
      }

      pointsWritten += pointsInScan;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write NetCDF-COARDS variables for "
                              "Swath data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void SwathDataset::writeKML() const {
  PRE( outputKMLFile_ );
  const char* const variable = variableNames_[ dataVariable_ ].c_str();
  std::string source( name_ ); //x
  const size_t index = source.rfind( variable );

  if ( index != std::string::npos && index > 1 ) {
    source.erase( index - 1 );
  }

  const char* const units = variableUnits_[ dataVariable_ ].c_str();
  const char* const dataType = "Quadrilateral";
  const char* const description =
    "Satellite-measured quadrilateral cell data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;

  writeStartFolderKML( outputKMLFile_, source.c_str(), variable, units,
                       dataType, description );

  const double variableMinimum = minimum();
  const double variableMaximum = maximum();
  const size_t scans = timestamps_.size();
  const size_t verticesPerCell = 4; // QUADRILATERAL.
  size_t pointIndex = 0;
  bool ok = true;

  for ( size_t scan = 0; AND2( ok, scan < scans ); ++scan ) {
    CHECK2( scan < timestamps_.size(), scan < points_.size() );
    const Timestamp timestamp = timestamps_[ scan ];
    const int yyyymmdd = timestamp.yyyymmdd();
    const int hhmmss = timestamp.hhmmss();
    const int valueTimestamp =
      timestepSize_ == HOURS ? yyyymmdd * 100 + hhmmss / 10000
      : timestepSize_ == DAYS ? yyyymmdd
      : timestepSize_ == MONTHS ? yyyymmdd / 100
      : timestepSize_ == TRIMONTHS ? yyyymmdd / 100
      : yyyymmdd / 10000;
    const size_t scanPoints = points_[ scan ];

    if ( dataFileOffset_ ) { // If paging load dataset if not already loaded
      const bool loaded =
        IN_RANGE( scan, firstDataTimestep_, lastDataTimestep_ );

      if ( ! loaded ) {
        ok = readBufferedDataSubset( scan, true );
        pointIndex = 0;
      }
    }

    for ( size_t point = 0; AND2( ok, point < scanPoints );
          ++point, ++pointIndex ) {
      size_t coordinateIndex = pointIndex * verticesPerCell;
      CHECK( coordinateIndex < coordinates_.size() );
      const double longitudeSW = coordinates_[ coordinateIndex ].longitude;
      const double latitudeSW  = coordinates_[ coordinateIndex ].latitude;
      const double elevationSW = coordinates_[ coordinateIndex ].elevation;
      ++coordinateIndex;
      const double longitudeSE = coordinates_[ coordinateIndex ].longitude;
      const double latitudeSE  = coordinates_[ coordinateIndex ].latitude;
      const double elevationSE = coordinates_[ coordinateIndex ].elevation;
      ++coordinateIndex;
      const double longitudeNE = coordinates_[ coordinateIndex ].longitude;
      const double latitudeNE  = coordinates_[ coordinateIndex ].latitude;
      const double elevationNE = coordinates_[ coordinateIndex ].elevation;
      ++coordinateIndex;
      const double longitudeNW = coordinates_[ coordinateIndex ].longitude;
      const double latitudeNW  = coordinates_[ coordinateIndex ].latitude;
      const double elevationNW = coordinates_[ coordinateIndex ].elevation;
      const double quadCoordinates[ 4 * 3 ] = {
        longitudeSW, latitudeSW, elevationSW,
        longitudeSE, latitudeSE, elevationSE,
        longitudeNE, latitudeNE, elevationNE,
        longitudeNW, latitudeNW, elevationNW
      };
      const size_t variableSize =
        dataFileOffset_ == 0 ? totalPoints_ : maximumPoints_;
      const size_t dataIndex = dataVariable_ * variableSize + pointIndex;
      CHECK( dataIndex < data_.size() );
      const double value = data_[ dataIndex ];

      if ( value > MISSING ) {
        const Color color =
         dataColorFunction( value, variableMinimum, variableMaximum );

        writeCellDataToKML( outputKMLFile_, source.c_str(), variable, units,
                            valueTimestamp, 1, MISSING, value, MISSING, MISSING,
                            4, quadCoordinates, color );
      }
    }
  }

  writeEndFolderKML( outputKMLFile_ );

  POST( true );
}



} // namespace emvl.



