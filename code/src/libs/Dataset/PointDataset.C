/******************************************************************************
PURPOSE: PointDataset.C - Dataset for 2D/3D point data.
HISTORY: 2018-11-06 plessel.todd@epa.gov
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
#include <PointDataset.h>       // For PointDataset.

enum { NOTE_LENGTH = 80 };

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~PointDataset - Destruct a PointDataset.
******************************************************************************/

PointDataset::~PointDataset() {
  PRE( true );
}



/******************************************************************************
PURPOSE: PointDataset - Construct a PointDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid point data.
******************************************************************************/

PointDataset::PointDataset( FILE* stream ) {
  PRE0( stream );

  variables_        = 0;
  dataVariable_     = 0;
  timesteps_        = 0;
  cellType_         = POINT;
  hasElevation_     = false;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = false;
  isStructuredGrid_ = false;
  isVector_         = false;
  readXDRHeader( stream ); //x
  readXDRData( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid point data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: PointDataset - Construct a PointDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid point data.
******************************************************************************/

PointDataset::PointDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  variables_        = 0;
  dataVariable_     = 0;
  timesteps_        = 0;
  cellType_         = POINT;
  hasElevation_     = false;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = false;
  isStructuredGrid_ = false;
  isVector_         = false;
  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDRHeader( file ); //x    
    readXDRData( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid point data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: PointDataset - Copy-construct a PointDataset.
INPUTS:  const PointDataset& other  Object to copy.
******************************************************************************/

PointDataset::PointDataset( const PointDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    notes_ = other.notes_;
    idNotes_ = other.idNotes_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a PointDataset.
INPUTS:  const PointDataset& other  Object to copy.
RETURNS: PointDataset& *this.
******************************************************************************/

PointDataset& PointDataset::operator=( const PointDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    notes_ = other.notes_;
    idNotes_ = other.idNotes_;
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

Dataset* PointDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: PointDataset::sample()" ); //x
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

Dataset* PointDataset::regrid( const int regridMethod,
                              const CMAQGridParameters&
                                cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: PointDataset::regrid()" ); //x
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

void PointDataset::write( const std::string& directory,
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
  } else if ( format == QueryParameters::KML ) {
    writeKML(); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool PointDataset::invariant() const {
  bool result = Dataset::invariant();

  if ( result ) {
    const bool hasId = ! strcmp( variableNames_[0].c_str(), "id" );
    const bool hasCount =
      AND3( variables_ > 2 + isVector_, hasId,
            ! strcmp( variableNames_[1].c_str(), "count" ) );
    result =
      AND10( variables_      == hasId + hasCount + 1 + isVector_,
           cellType_         == POINT,
           hasArea_          == false,
           hasVolume_        == false,
           isGroup_          == false,
           isStructuredGrid_ == false,
           timestamps_.size() > 0,
           coordinates_.size() == timestamps_.size(),
           data_.size() == variables_ * timestamps_.size(),
           OR2( notes_.size() == 0,
                AND6( notes_.size() == timestamps_.size(),
                      notes_[ 0 ].length() > 0,
                      notes_[ notes_.size() - 1 ].length() > 0,
                      idNotes_.size() == timestamps_.size(),
                      idNotes_[ 0 ].length() > 0,
                      idNotes_[ idNotes_.size() - 1 ].length() > 0 ) ) );
  }

  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool PointDataset::operator==( const PointDataset& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND4( equal( other ),
               timestamps_ == other.timestamps_,
               notes_ == other.notes_,
               idNotes_ == other.idNotes_ ) );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool PointDataset::operator!=( const PointDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: notes - Number of notes.
RETURNS: size_t number of notes or 0 if none.
******************************************************************************/

size_t PointDataset::notes() const {
  PRE( true );
  const size_t result = notes_.size();
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: note - Note of a data point.
INPUTS:  const size_t point  Index of data point.
RETURNS: const std::string& note of a data point.
******************************************************************************/

const std::string& PointDataset::note( const size_t point ) const {
  PRE( point < notes() - 1 );
  const std::string& result = notes_[ point ];
  POST( result.length() );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: findNearestPoint - Find index of point point nearest a probe location.
         const double longitude   Longitude of point.
         const double latitude    Latitude of probe.
         const double elevation   Elevation of probe or MISSING (ignored if 2D).
         const double nearEnough  Distance considered a match.
         const size_t first       First index of search.
         const size_t last        Last  index of search.
OUTPUTS: size_t& nearest          Index of nearest point within nearEnough.
RETURNS: bool true if found, else false.
******************************************************************************/

bool PointDataset::findNearestPoint( const double longitude,
                                     const double latitude,
                                     const double elevation,
                                     const double nearEnough,
                                     const size_t first,
                                     const size_t last,
                                     size_t& nearest ) const {

  PRE7( isLongitude( longitude ),
        isLatitude( latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ),
        nearEnough > 0.0,
        nearEnough < 1.0,
        first < data_.size() / variables_,
        IN_RANGE( last, first, data_.size() / variables_ - 1 ) );

  double smallestDistance = nearEnough + 1.0; // Use faster Manhatten distance.
  size_t index = nearest = first;

  do {
    const Point& point = coordinates_[ index ];
    const double pointLongitude = point.longitude;
    const double longitudeDistance =
      pointLongitude <= longitude ? longitude - pointLongitude
      : pointLongitude - longitude;
    double pointDistance = longitudeDistance;

    if ( pointDistance < smallestDistance ) {
      const double pointLatitude = point.latitude;
      const double latitudeDistance =
        pointLatitude <= latitude ? latitude - pointLatitude
        : pointLatitude - latitude;
      pointDistance += latitudeDistance;

      if ( pointDistance < smallestDistance ) {

        if ( AND2( hasElevation_, elevation != MISSING ) ) {
          const double pointElevation = point.elevation;
          const double elevationDistance =
            pointElevation <= elevation ? elevation - pointElevation
            : pointElevation - elevation;
          pointDistance += elevationDistance;
        }

        if ( pointDistance < smallestDistance ) {
          smallestDistance = pointDistance;
          nearest = index;
        }
      }
    }

    ++index;
  } while ( index <= last );

  const bool result = smallestDistance <= nearEnough;

  if ( result ) {

    if ( nearest < idNotes_.size() ) {
      probedNote_ = idNotes_[ nearest ];
    }
  }

  return result;
}



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

double PointDataset::vprobe( const Timestamp timestamp,
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
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  const double nearEnough = 0.1; // Probe close enough to data point.
  double result = MISSING;
  size_t first = 0;
  size_t last = 0;
  const bool foundMatchingTimestamps =
    matchingTimestamps( timestamp, timestamps_, first, last );

  if ( foundMatchingTimestamps ) {
    size_t nearestPoint = 0;
    const bool foundNearestPoint =
      findNearestPoint( longitude, latitude, elevation, nearEnough, first, last,
                        nearestPoint );

    if ( foundNearestPoint ) {
      const int probeVariable =
        variable != IMISSING ? variable
        : isVector_ ? variables_ - 2
        : dataVariable_;
      const size_t totalPoints = timestamps_.size();
      const size_t dataIndex = probeVariable * totalPoints + nearestPoint;
      CHECK( dataIndex < data_.size() );
      result = data_[ dataIndex ];

      if ( AND2( variable == IMISSING, isVector_ ) ) { // vector magnitude:
        const size_t dataIndex2 = dataIndex + totalPoints;
        CHECK( dataIndex2 < data_.size() );
        const double component2 = data_[ dataIndex2 ];
        result *= result;
        result += component2 * component2;
        result = sqrt( result );
      }
    }
  }

  POST( OR2( result == MISSING,
             IN_RANGE( result, minimum( variable ), maximum( variable ) ) ) );
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
PointDataset::vsubset( const Timestamp beginTimestamp,
                       const Timestamp endTimestamp,
                       const int variable ) const {

  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable  == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<SubsetPointers> result;
  size_t first = 0;
  size_t last = 0;
  const bool foundMatchingTimestamps =
    matchingTimestamps(beginTimestamp, endTimestamp, timestamps_, first, last);

  if ( foundMatchingTimestamps ) {
    const size_t totalPoints = timestamps_.size();
    const size_t subsetCount = last - first + 1;

    if ( OR2( ! isVector_, variable != IMISSING ) ) {
      const int variable0 = variable == IMISSING ? dataVariable_ : variable;
      const size_t firstIndex = variable0 * totalPoints + first;
      CHECK3( firstIndex < data_.size(),
              IN_RANGE( subsetCount, 1, data_.size() ),
              IN_RANGE( firstIndex + subsetCount, 1, data_.size() ) );
      const Dataset::SubsetPointers subsetPointers = {
        subsetCount,
        &coordinates_[ first ],
        subsetCount,
        &data_[ firstIndex ],
        notes_.size() ? subsetCount : 0,
        notes_.size() ? &notes_[ first ] : 0
      };
      result.push_back( subsetPointers ); //x
    } else { // Append each vector component variable:
      CHECK2( isVector_, variables_ >= 2 );

      for ( int component = variables_ - 2; component < variables_;
            ++component ) {
        const size_t firstIndex = component * totalPoints + first;
        CHECK3( firstIndex < data_.size(),
                IN_RANGE( subsetCount, 1, data_.size() ),
                IN_RANGE( firstIndex + subsetCount, 1, data_.size() ) );
        const Dataset::SubsetPointers subsetPointers = {
          subsetCount,
          &coordinates_[ first ],
          subsetCount,
          &data_[ firstIndex ],
          notes_.size() ? subsetCount : 0,
          notes_.size() ? &notes_[ firstIndex ] : 0
        };
        result.push_back( subsetPointers ); //x
      }
    }
  }

  POST( IMPLIES( result.size(),
                 AND10( result[ 0 ].dataCount,
                        result[ 0 ].coordinatesCount == result[ 0 ].dataCount,
                        result[ 0 ].coordinates == &coordinates_[ first ],
                        result[ 0 ].data,
                        isPoint( result[ 0 ].coordinates[ 0 ] ),
                        isPoint( result[ 0 ].coordinates[
                                   result[ 0 ].coordinatesCount - 1 ] ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::minimum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                         > MISSING ),
                                 IN_RANGE( emvl::minimum( result[ 0 ].data,
                                                          result[ 0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( AND2( variable != IMISSING,
                                       emvl::maximum( result[ 0 ].data,
                                                      result[ 0 ].dataCount )
                                         > MISSING ),
                                 IN_RANGE( emvl::maximum( result[ 0 ].data,
                                                          result[ 0].dataCount),
                                           minimum( variable ),
                                           maximum( variable ) ) ),
                        IMPLIES( notes_.size(),
                                 AND4( result[ 0 ].notesCount ==
                                       result[ 0 ].dataCount,
                                       result[ 0 ].notes,
                                       result[ result.size() - 1].notesCount ==
                                       result[ result.size() - 1].dataCount,
                                       result[ result.size() - 1 ].notes ) ),
                        IMPLIES( AND2( isVector(), variable == IMISSING ),
                                 result.size() >= 2 ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: allDistinctNotes - Return a reference to an array of distinct notes.
RETURNS: const std::vector<std::string>& idNotes_;
******************************************************************************/

  const std::vector<std::string>& PointDataset::allDistinctNotes() const {
  PRE( true );
  const std::vector<std::string>& result = idNotes_;
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: distinctNotePoint - Return a const pointer to the first point matching
         distictNote or 0 if none matched.
INPUTS:  const std::string& distinctNote  Distinct note to lookup.
RETURNS: const Point* if found match else 0.
******************************************************************************/

const Point*
PointDataset::distinctNotePoint( const std::string& distinctNote ) const {
  PRE( true );
  const Point* result = 0;
  const size_t count = idNotes_.size();

  if ( count == coordinates_.size() ) {

    for ( size_t index = 0; index < count; ++index ) {

      if ( distinctNote == idNotes_[ index ] ) {
        result = &coordinates_[ index ];
        index = count;
      }
    }
  }

  POST( OR2( result == 0, isPoint( *result ) ) );
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
         hasElevation_
         timestamps_
         notes_
NOTES:   Throws exception upon failure to read valid header from a stream.
******************************************************************************/

void PointDataset::readXDRHeader( FILE* stream ) {
  PRE0( stream );

  /* Read XDR header that looks like this:

  Point 1.0
  ftp://ftp.orbit.nesdis.noaa.gov/pub/smcd/xzhang/PM25/
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Dimensions: variables points
  3 72
  # Variable names:
  timestamp longitude latitude pm25
  # Variable units:
  yyyymmddhhmmss deg deg metric_tons
  # IEEE-754 64-bit reals data[variables][points]:

  or, with elevations:

  Point 1.0
  ftp://ftp.orbit.nesdis.noaa.gov/pub/smcd/xzhang/PM25/
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Dimensions: variables points
  4 72
  # Variable names:
  timestamp longitude latitude elevation pm25
  # Variable units:
  yyyymmddhhmmss deg deg m metric_tons
  # IEEE-754 64-bit reals data[variables][points]:

  or, with elevations and vector2 variable:

  Point 1.0
  ftp://ftp.orbit.nesdis.noaa.gov/pub/smcd/xzhang/PM25/
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Dimensions: variables points
  6 72
  # Variable names:
  timestamp longitude latitude elevation wind_u wind_v
  # Variable units:
  yyyymmddhhmmss deg deg m m/s m/s
  # IEEE-754 64-bit reals data[variables][points]:

  or, with elevations and id variable and notes-per-point:

  Point 1.0
   https://api.purpleair.com,PurpleAirSubset
  2020-12-20T00:00:00-0000 2020-12-20T23:59:59-0000
  # Dimensions: variables points
  6 459
  # Variable names:
  timestamp longitude latitude elevation id pm25_corrected
  # Variable units:
  yyyymmddhhmmss deg deg m - ug/m3
  # char notes[points][80] and
  # IEEE-754 64-bit reals data[variables][points]:

  or, like above but with counts:

  Point 1.0
   https://api.purpleair.com,PurpleAirSubset
  2020-12-20T00:00:00-0000 2020-12-20T23:59:59-0000
  # Dimensions: variables points
  7 459
  # Variable names:
  timestamp longitude latitude elevation id counts pm25_corrected_hourly
  # Variable units:
  yyyymmddhhmmss deg deg m - - ug/m3
  # char notes[points][80] and
  # IEEE-754 64-bit reals data[variables][points]:

  */

  readAndMatchLine( stream, "Point 1.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, true ); //x
  skipLine( stream ); //x
  size_t dimensions[ 2 ] = { 0, 0 };
  readDimensions( stream, 2, dimensions ); //x
  const size_t variables = dimensions[ 0 ];
  const size_t points    = dimensions[ 1 ];
  timestamps_.resize( points ); //x
  readVariablesAndUnits( stream, variables ); //x

  hasElevation_ =
    AND2( variables_ > 3,
          ! strcmp( variableNames_[ 3 ].c_str(), "elevation" ) );

  isVector_ =
    AND2( variables_ >= 5,
     OR2( AND2( ! strcmp( variableNames_[variables_ - 2].c_str(), "windU" ),
                ! strcmp( variableNames_[variables_ - 1].c_str(), "windV" )),
          AND2( ! strcmp( variableNames_[variables_ - 2].c_str(), "wind_u" ),
                ! strcmp( variableNames_[variables_ - 1].c_str(), "wind_v" ))));

  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_

  if ( timestepSize_ == DAYS ) {
    timesteps_ /= 24;
  }

  if ( timesteps_ == 0 ) {
    timesteps_ = 1;
  }

  const bool hasNotes =
    readAndMatchLine2( stream,
                       "# char notes[points][80] and\n",
              "# IEEE-754 64-bit reals data[variables][points]:\n" ) == 1; //x

  if ( hasNotes ) {
    readAndMatchLine( stream,
                      "# IEEE-754 64-bit reals data[variables][points]:\n");//x
    notes_.resize( points ); //x
  }

  POST010( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           IN_RANGE( variables_, 4, 8 ),
           dataVariable_ == variables_ - 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           endingTimestamp_ > startingTimestamp_,
           timesteps_ > 0,
           timestamps_.size() > 0 );
}



/******************************************************************************
PURPOSE: readXDRData - Read XDR data from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: hasElevation_
         coordinates_
         data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void PointDataset::readXDRData( FILE* stream ) {
  PRE011( stream,
           isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           IN_RANGE( variables_, 4, 8 ),
           dataVariable_ == variables_ - 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           endingTimestamp_ > startingTimestamp_,
           timesteps_ > 0,
           timestamps_.size() > 0);

  /* Data looks like this (notes is optional):

  # char notes[points][80] and
  # IEEE-754 64-bit reals data[variables][points]:

  */

  if ( notes_.size() ) {
    readStrings( stream, NOTE_LENGTH, notes_ ); //x
  }

  const size_t points = timestamps_.size();

  // Check for possible auxilliary data variables: id count:

  bool hasCount = false;
  bool hasId    = false;

  for ( int variable = 0; variable < variables_; ++variable ) {
    const char* const variableName = variableNames_[ variable ].c_str();

    if ( OR2( ! strcmp( variableName, "id" ),
              strstr( variableName, "_id" ) ) ) {
      hasId = true;
    } else if ( ! strcmp( variableName, "count" ) ) {
      hasCount = true;
    }
  }

  const int isOnlyId =
    OR2( ! strcmp( variableNames_[ variables_ - 1 ].c_str(), "id" ),
         strstr( variableNames_[ variables_ - 1 ].c_str(), "_id" ) );
  const size_t dataVariables = hasId + hasCount + 1 + isVector_ - isOnlyId;
  data_.resize( dataVariables * points ); //x
  coordinates_.resize( points ); //x

  // Reduce variables_, names, units to match only the data variables:

  int idVariable = -1;

  for ( size_t to = 0, from = variables_ - dataVariables;
        to < dataVariables; ++to, ++from ) {
    variableNames_[ to ] = variableNames_[ from ];
    variableUnits_[ to ] = variableUnits_[ from ];

    if ( ! strcmp( variableNames_[ to ].c_str(), "id" ) ) {
      idVariable = to;
    }
  }

  variableNames_.resize( dataVariables );
  variableUnits_.resize( dataVariables );
  variables_ = dataVariables;
  dataVariable_ = variables_ - 1; // Select last variable by default.

  // Read timestamps:

  bool ok = fread( &data_[ 0 ], sizeof (double), points, stream ) == points;

  if ( ok ) {
    reverse8ByteWordsIfLittleEndian( &data_[ 0 ], points );

    // Convert and store timestamps:
    // Memoize: save previous point timestamp and compare to current point.
    // If the same (very frequent) then just copy the timestamp
    // which is much faster than calling yyyymmddhhmmss().

    long long yyyymmddhhmmss0 = 0;
    bool initialized = false;

    for ( size_t point = 0; AND2( ok, point < points ); ++point ) {
      const double timestamp = data_[ point ];
      const long long yyyymmddhhmmss = (long long) timestamp;

      if ( AND2( initialized, yyyymmddhhmmss == yyyymmddhhmmss0 ) ) {
        timestamps_[ point ] = timestamps_[ point - 1 ];
      } else {
        const int yyyymmdd = yyyymmddhhmmss / 1000000;
        const int hhmmss   = yyyymmddhhmmss % 1000000;
        ok = AND2( Timestamp::isYYYYMMDD( yyyymmdd ),
                   Timestamp::isHHMMSS( hhmmss ) );

        if ( ok ) {
          timestamps_[ point ].yyyymmddhhmmss( yyyymmdd, hhmmss );
          yyyymmddhhmmss0 = yyyymmddhhmmss;
          initialized = true;
        }
      }
    }

    if ( ok ) {
      readCoordinates( stream, hasElevation_, &data_[ 0 ] ); //x
      computeBounds();
      const size_t dataSize = data_.size();
      ok = fread( &data_[ 0 ], sizeof (double), dataSize, stream ) == dataSize;

      if ( ok ) {
        reverse8ByteWordsIfLittleEndian( &data_[ 0 ], dataSize );
        updateMissingValues( &data_[ 0 ], dataSize );
        computeVariableRanges(); //x

        if ( AND2( idVariable > -1, notes_.size() ) ) {
          size_t idIndex = points * idVariable;
          idNotes_.reserve( notes_.size() ); //x

          for ( size_t point = 0; point < points; ++point, ++idIndex ) {
            CHECK2( idIndex < data_.size(), point < notes_.size() );
            const long long siteId = data_[ idIndex ];
            const double id = siteId;
            CHECK2( siteId > 0, id > 0.0 );
            char idNote[ 128 ] = "";
            memset( idNote, 0, sizeof idNote );
            snprintf( idNote, sizeof idNote / sizeof *idNote,
                      "%0.0f %s", id, notes_[ point ].c_str() );
            idNotes_.push_back( std::string( idNote ) );
          }
        }
      }
    }
  }

  if ( ! AND4( ok, isValidCoordinates(), isValidData(), isValidRange() ) ) {
    throw std::runtime_error( "Failed to read valid point data." ); //x
  }

  POST03( coordinates_.size() == timestamps_.size(),
          data_.size() == timestamps_.size() * variables_,
          isValidRange() );

}



/******************************************************************************
PURPOSE: writeXDR - Write XDR format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void PointDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

  Point 1.0
  ftp://ftp.orbit.nesdis.noaa.gov/pub/smcd/xzhang/PM25/
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Dimensions: variables points
  3 72
  # Variable names:
  timestamp longitude latitude pm25
  # Variable units:
  yyyymmddhhmmss deg deg metric_tons
  # IEEE-754 64-bit reals data[variables][points]:

  or, with elevations:

  Point 1.0
  ftp://ftp.orbit.nesdis.noaa.gov/pub/smcd/xzhang/PM25/
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Dimensions: variables points
  4 72
  # Variable names:
  timestamp longitude latitude elevation pm25
  # Variable units:
  yyyymmddhhmmss deg deg m metric_tons
  # IEEE-754 64-bit reals data[variables][points]:

  or, with elevations and vector2 variable:

  Point 1.0
  ftp://ftp.orbit.nesdis.noaa.gov/pub/smcd/xzhang/PM25/
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Dimensions: variables points
  6 72
  # Variable names:
  timestamp longitude latitude elevation wind_u wind_v
  # Variable units:
  yyyymmddhhmmss deg deg m m/s m/s
  # IEEE-754 64-bit reals data[variables][points]:

  or, with elevations and vector2 variable and notes-per-point:

  Point 1.0
  ftp://ftp.orbit.nesdis.noaa.gov/pub/smcd/xzhang/PM25/
  2008-07-03T00:00:00-0000 2008-07-03T23:59:59-0000
  # Dimensions: variables points
  6 72
  # Variable names:
  timestamp longitude latitude elevation wind_u wind_v
  # Variable units:
  yyyymmddhhmmss deg deg m m/s m/s
  # char notes[points][80] and
  # IEEE-754 64-bit reals data[variables][points]:

  or, with elevations and id variable and notes-per-point:

  Point 1.0
   https://api.purpleair.com,PurpleAirSubset
  2020-12-20T00:00:00-0000 2020-12-20T23:59:59-0000
  # Dimensions: variables points
  6 459
  # Variable names:
  timestamp longitude latitude elevation id pm25_corrected
  # Variable units:
  yyyymmddhhmmss deg deg m - ug/m3
  # char notes[points][80] and
  # IEEE-754 64-bit reals data[variables][points]:

  or, like above but with counts:

  Point 1.0
   https://api.purpleair.com,PurpleAirSubset
  2020-12-20T00:00:00-0000 2020-12-20T23:59:59-0000
  # Dimensions: variables points
  7 459
  # Variable names:
  timestamp longitude latitude elevation id counts pm25_corrected_hourly
  # Variable units:
  yyyymmddhhmmss deg deg m - - ug/m3
  # char notes[points][80] and
  # IEEE-754 64-bit reals data[variables][points]:

  */

  Timestamp::UTCString timestamp1 = "";
  Timestamp::UTCString timestamp2 = "";
  startingTimestamp_.toUTCString( timestamp1 );
  endingTimestamp_.toUTCString( timestamp2 );
  const char* const format1 =
    "Point 1.0\n"
    "%s\n"
    "%s %s\n"
    "# Dimensions: variables points:\n"
    "%d %" LU "\n"
    "# Variable names:\ntimestamp longitude latitude %s";
  const char* const elevationString = hasElevation_ ? "elevation " : "";
  bool ok =
    fprintf( stream, format1,
             description_.c_str(),
             timestamp1, timestamp2,
             3 + hasElevation_ + variables_,
             timestamps_.size(), elevationString )
    > (int) strlen( format1 );

  for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
    const char delimiter = variable + 1 == variables_ ? '\n' : ' ';
    const char* const variableName = variableNames_[ variable ].c_str();
    ok = fprintf( stream, "%s%c", variableName, delimiter )
         > (int) strlen( variableName );
  }

  if ( ok ) {
    const char* const elevationUnits  = hasElevation_ ? "m " : "";
    ok = fprintf( stream, "%s%s", "# Variable units:\nyyyymmddhhmmss deg deg ",
                 elevationUnits ) >= 40;

    for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
      const char delimiter = variable + 1 == variables_ ? '\n' : ' ';
      const char* const units = variableUnits_[ variable ].c_str();
      ok = fprintf( stream, "%s%c", units, delimiter ) > (int) strlen( units );
    }

    if ( ok ) {
      const char* const format2 =
        "%s"
        "# IEEE-754 64-bit reals data[variables][points]:\n";
      ok = fprintf( stream, format2,
                    notes_.size() ? "# char notes[points][80] and\n" : "" )
           >= (int) strlen( format2 ) - 2;

      if ( ok ) {

        if ( notes_.size() ) {
          writeStrings( stream, NOTE_LENGTH, notes_ ); //x
        }

        const size_t points = timestamps_.size();
        const size_t count = variables_ * points;
        std::vector<double> buffer( count ); //x

        // Convert timestamps_ to double and write them:

        for ( size_t point = 0; point < points; ++point ) {
          const Timestamp timestamp = timestamps_[ point ];
          const long long yyyymmdd = timestamp.yyyymmdd();
          const long long hhmmss   = timestamp.hhmmss();
          const long long yyyymmddhhmmss = yyyymmdd * 1000000 + hhmmss;
          const double dtimestamp = (double) yyyymmddhhmmss;
          buffer[ point ] = dtimestamp;
        }

        reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], points );
        ok = fwrite( &buffer[ 0 ], sizeof (double), points, stream ) == points;

        if ( ok ) {
          writeCoordinates( stream, &buffer[ 0 ] ); //x
          memcpy( &buffer[ 0 ], &data_[ 0 ], count * sizeof (double) );
          reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], count );
          ok = fwrite( &buffer[ 0 ], sizeof (double), count, stream ) == count;
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write point data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void PointDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  LONGITUDE(deg)  LATITUDE(deg)   PM25_emission(kg)
   2005-08-26T00:00:00-0000    -86.8100    33.5500    38.0000
   ...

  */

  const char* const start = "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)";
  const char* const elevationHeading = hasElevation_ ? "\tELEVATION(m)" : "";
  bool ok =
    fprintf( stream, "%s%s", start, elevationHeading )
    >= (int) strlen( start ) - 2;

  for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
    const char* const variableName  = variableNames_[ variable ].c_str();
    const char* const variableUnits = variableUnits_[ variable ].c_str();
    ok = fprintf( stream, "\t%s(%s)", variableName, variableUnits )
         > (int) strlen( variableName );
  }

  if ( ok ) {

    if ( notes_.size() ) {
      ok = fputs( "\tNOTE(-)\n", stream ) != EOF;
    } else {
      ok = fputc( '\n', stream ) != EOF;
    }

    const size_t points = timestamps_.size();

    for ( size_t point = 0; AND2( ok, point < points ); ++point ) {
      const Timestamp timestamp = timestamps_[ point ];
      Timestamp::UTCString timestampString = "";
      timestamp.toUTCString( timestampString );
      const double longitude = coordinates_[ point ].longitude;
      const double latitude  = coordinates_[ point ].latitude;

      ok = fprintf( stream, "%s\t%11.5lf\t%11.5lf",
                    timestampString, longitude, latitude ) >= 48;

      if ( ok ) {

        if ( hasElevation_ ) {
          const double elevation = coordinates_[ point ].elevation;
          ok = fprintf( stream, "\t%11.5lf", elevation ) >= 0;
        }

        if ( ok ) {

          for ( int variable = 0; AND2( ok, variable < variables_ );
                ++variable ) {
            const size_t index = variable * points + point;
            CHECK( index < data_.size() );
            const double value = data_[ index ];
            ok = fprintf( stream, "\t%11.5e", value ) > 0;
          }

          if ( ok ) {

            if ( notes_.size() ) {
              const char* const note = notes_[ point ].c_str();
              CHECK2( note, *note );
              ok = fprintf( stream, "\t%-46s\n", note ) >= 48;
            } else {
              ok = fputc( '\n', stream ) != EOF;
            }
          }
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write point data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void PointDataset::writeCOARDS( const std::string& fileName ) const {

  PRE( fileName.length() );
  const size_t points = timestamps_.size();
  const size_t outputVariables = 2 + hasElevation_ + variables_ + 3;
  const bool hasNotes = notes_.size() != 0;
  const size_t fileSizeEstimate =
    points * outputVariables * 4 + hasNotes * NOTE_LENGTH + 10000; // header.
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    enum { POINTS, LENGTH, DIMENSIONS };
    const char* const names[ DIMENSIONS ] = { "points", "length" };
    const size_t dimensions[ DIMENSIONS ] = { points, NOTE_LENGTH };
    int dimensionIds[ DIMENSIONS ] = { -1, -1 };
    const int dimensionsCount = hasNotes ? 2 : 1;
    ok = createNetCDFDimensions( file, dimensionsCount, names, dimensions,
                                 dimensionIds );

    if ( ok ) {

      if ( hasNotes ) {
        ok = createNetCDFVariable( file, "notes", "-", NC_CHAR, 0, 2,
                                   dimensionIds ) != -1;
      }

      writeCOARDSCommon( file, 0, false, false, hasElevation_, true, 1,
                         1, dimensionIds, dimensions, names, timestamps_ ); //x

      if ( hasNotes ) {
        writeCOARDSNotes( file, NOTE_LENGTH, notes_ ); //x
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to create NetCDF file for point data.");//x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void PointDataset::writeKML() const {
  PRE( outputKMLFile_ );
  const char* const variable =
    isVector_ ? "Wind" : variableNames_[ dataVariable_ ].c_str();
  std::string source( name_ ); //x
  const size_t index = source.rfind( variable );

  if ( index != std::string::npos && index > 1 ) {
    source.erase( index - 1 );
  }

  const char* const units = variableUnits_[ dataVariable_ ].c_str();
  const char* const dataType = isVector_ ? "Vector" : "Point";
  const char* const description =
    isVector_ ? "Measured vector data." : "Measured point data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;

  writeStartFolderKML( outputKMLFile_, source.c_str(), variable, units,
                       dataType, description );

  const int components = isVector_ ? 2 : 1;
  const double variableMinimum = minimum();
  const double variableMaximum = maximum();
  const size_t points = data_.size() / variables_;

  for ( size_t point = 0; point < points; ++point ) {
    const Timestamp timestamp = timestamps_[ point ];
    const int yyyymmdd = timestamp.yyyymmdd();
    const int hhmmss = timestamp.hhmmss();
    const int valueTimestamp =
      timestepSize_ == HOURS ? yyyymmdd * 100 + hhmmss / 10000
      : timestepSize_ == DAYS ? yyyymmdd
      : timestepSize_ == MONTHS ? yyyymmdd / 100
      : timestepSize_ == TRIMONTHS ? yyyymmdd / 100
      : yyyymmdd / 10000;
    const double longitude = coordinates_[ point ].longitude;
    const double latitude  = coordinates_[ point ].latitude;
    const double elevation = coordinates_[ point ].elevation;
    const double pointCoordinates[ 3 ] = { longitude, latitude, elevation };
    const double value     = data_[ point ];
    const double value2 = isVector_ ? data_[ point + point ] : MISSING;

    if ( OR2( ! isVector_, value2 > MISSING ) ) {
      const double magnitude =
        isVector_ ? sqrt( value * value + value2 * value2 ) : value;
      const Color color =
        dataColorFunction( magnitude, variableMinimum, variableMaximum );

      writeCellDataToKML( outputKMLFile_, source.c_str(), variable, units,
                          valueTimestamp, components,
                          MISSING, value, value2, MISSING,
                          1, pointCoordinates, color );
    }
  }

  writeEndFolderKML( outputKMLFile_ );

  POST( true );
}



} // namespace emvl.



