/******************************************************************************
PURPOSE: ProfileDataset.C - Dataset for Profile (e.g., spectrophotometer)
         ground station data.
HISTORY: 2013-11-27 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fprofilef().
#include <string.h> // For memset(), strstr().
#include <limits.h> // For INT_MAX.
#include <float.h>  // For FLT_MAX.
#include <math.h>   // For sqrt().
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
#include <ProfileDataset.h>     // For ProfileDataset.

enum { NOTE_LENGTH = 80 };

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~ProfileDataset - Destruct a ProfileDataset.
******************************************************************************/

ProfileDataset::~ProfileDataset() {
}



/******************************************************************************
PURPOSE: ProfileDataset - Construct a ProfileDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

ProfileDataset::ProfileDataset( FILE* stream ) {
  PRE0( stream );

  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = POINT;
  hasElevation_     = true;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;
  readXDR( stream ); //x

  if ( ! Dataset::invariant() ) {
    throw std::runtime_error( "Failed to read valid Profile data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: ProfileDataset - Construct a ProfileDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

ProfileDataset::ProfileDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = POINT;
  hasElevation_     = true;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x
  }

  if ( ! Dataset::invariant() ) {
    throw std::runtime_error( "Failed to read valid Profile data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: ProfileDataset - Copy-construct a ProfileDataset.
INPUTS:  const ProfileDataset& other  Object to copy.
******************************************************************************/

ProfileDataset::ProfileDataset( const ProfileDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    points_ = other.points_;
    notes_ = other.notes_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a ProfileDataset.
INPUTS:  const ProfileDataset& other  Object to copy.
RETURNS: ProfileDataset& *this.
******************************************************************************/

ProfileDataset& ProfileDataset::operator=( const ProfileDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    points_ = other.points_;
    notes_ = other.notes_;
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

Dataset* ProfileDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: ProfileDataset::sample()" ); //x
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

Dataset* ProfileDataset::regrid( const int regridMethod,
                                 const CMAQGridParameters&
                                 cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: ProfileDataset::regrid()" ); //x
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

void ProfileDataset::write( const std::string& directory,
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

bool ProfileDataset::invariant() const {
  const bool result =
    AND16( Dataset::invariant(),
           variables_ >= 1,
           cellType_         == POINT,
           hasElevation_     == true,
           hasArea_          == false,
           hasVolume_        == false,
           isStationary_     == false,
           isGroup_          == true,
           isStructuredGrid_ == false,
           isSurfaceData()   == false,
           timestamps_.size() == data_.size() /  variables_,
           points_.size()     >= 1,
           emvl::minimum( &points_[ 0 ], points_.size() ) >= 1,
           notes_.size()      == points_.size(),
           notes_[ 0 ][ 0 ],
           notes_[ notes_.size() - 1 ][ 0 ] );
  return result;
}



/******************************************************************************
PURPOSE: isSurfaceData - Is this dataset only on the Earth surface?
RETURNS: bool false.
******************************************************************************/

bool ProfileDataset::isSurfaceData() const {
  const bool result = false;
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool ProfileDataset::operator==( const ProfileDataset& other ) const {
  PRE( other.invariant() );
  bool result =
    OR2( this == &other,
         AND3( equal( other ),
               timestamps_.size() == other.timestamps_.size(),
               points_.size() == other.points_.size() ) );
  const size_t profiles = points_.size();
  const size_t timestamps = timestamps_.size();

  for ( size_t profile = 0; AND2( result, profile < profiles ); ++profile ) {
    result = points_[ profile ] == other.points_[ profile ];
    result = AND2( result, notes_[ profile ] == other.notes_[ profile ] );
  }

  for ( size_t timestamp = 0; AND2( result, timestamp < timestamps );
        ++timestamp ) {
    result = timestamps_[ timestamp ] == other.timestamps_[ timestamp ];
  }

  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool ProfileDataset::operator!=( const ProfileDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: profiles - Number of profiles.
RETURNS: size_t Number of profiles.
******************************************************************************/

size_t ProfileDataset::profiles() const {
  PRE( true );
  const size_t result = points_.size();
  POST( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: timestamp - Timestamp of a point.
INPUTS:  const size_t point  Index of point.
RETURNS: Timestamp of a profile.
******************************************************************************/

Timestamp ProfileDataset::timestamp( const size_t point ) const {
  PRE( point < timestamps_.size() );
  const Timestamp result = timestamps_[ point ];
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: points - Points in a profile.
INPUTS:  const size_t profile  Index of profile.
RETURNS: size_t number of points in a profile.
******************************************************************************/

size_t ProfileDataset::points( const size_t profile ) const {
  PRE( profile < points_.size() );
  const size_t result = points_[ profile ];
  POST( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: note - Note of a profile.
INPUTS:  const size_t profile  Index of profile.
RETURNS: const std::string& note of a profile.
******************************************************************************/

const std::string& ProfileDataset::note( const size_t profile ) const {
  PRE( profile < notes_.size() );
  const std::string& result = notes_[ profile ];
  POST( result.length() );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: vprobe - Return nearest data point to time-space location.
INPUTS:  const Timestamp& timestamp  Timestamp of probe.
         const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double elevation      Elevation of probe or MISSING for 2D.
         const int probeLayer        0-based index of variable or IMISSING.
         const int variable          0-based index of variable or IMISSING.
RETURNS: double Data value nearest probe or MISSING if not near any data.
NOTES:   Called by base class member function probe().
         Template Method Design Pattern.
******************************************************************************/

double ProfileDataset::vprobe( const Timestamp timestamp,
                               const double longitude,
                               const double latitude,
                               const double elevation,
                               const int probeLayer,
                               const int variable ) const {
  PRE6( timestamp.invariant(),
        isLongitude( longitude ),
        isLatitude( latitude ),
        OR2( elevation == MISSING, isFinite( elevation ) ),
        OR2( probeLayer == IMISSING,
             AND2( probeLayer >= 0, (size_t) probeLayer < layers() ) ),
        IN_RANGE( variable, 0, variables() - 1 ) );

  const double nearEnough = 0.1; // Probe close enough to data point.
  double result = MISSING;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {

    if ( isInTimeRange( timestamp ) ) {
      const size_t pointCount = timestamps_.size();
      double distance = nearEnough + 1.0; // Use Manhatten distance for speed.
      size_t point = 0;

      // Find first point in probe hour and near enough to probe location:

      for ( ; AND2( distance > nearEnough, point < pointCount ); ++point ) {
        const Timestamp& pointTimestamp = timestamps_[ point ];

        if ( pointTimestamp.sameHour( timestamp ) ) {
          CHECK( point < coordinates_.size() );
          const Point& dataPoint = coordinates_[ point ];
          const double pointLongitude = dataPoint.longitude;
          const double longitudeDistance =
            pointLongitude <= longitude ? longitude - pointLongitude
            : pointLongitude - longitude;

          if ( longitudeDistance < distance ) {
            const double pointLatitude = dataPoint.latitude;
            const double latitudeDistance =
              pointLatitude <= latitude ? latitude - pointLatitude
              : pointLatitude - latitude;

            if ( latitudeDistance < distance ) {
              double elevationDistance = elevation == MISSING ? 0.0 : 1.0;

              if ( elevationDistance ) {
                const double pointElevation = dataPoint.elevation;
                elevationDistance =
                  pointElevation <= elevation ? elevation - pointElevation
                  : pointElevation - elevation;
              }

              distance =
                longitudeDistance + latitudeDistance + elevationDistance;
            }
          }
        }
      }

      if ( distance <= nearEnough ) {
        CHECK( point > 0 );
        --point;
        const size_t nearestPoint = point;
        const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
        const size_t dataIndex = variable * totalPoints + nearestPoint;
        CHECK( dataIndex < data_.size() );
        result = data_[ dataIndex ];

        // If there are notes then set the profile note of probed point:

        if ( notes_.size() ) {
          // Determine profile of nearestPoint:

          size_t profile = 0;
          size_t profilePoints = points_[ profile ];

          while ( point >= profilePoints ) {
            point -= profilePoints;
            ++profile;
            CHECK( profile < points_.size() );
            profilePoints = points_[ profile ];
          }

          if ( profile < notes_.size() ) {
            probedNote_ = notes_[ profile ];
          }
        }
      }
    }
  }

  POST( OR2( result == MISSING,
             IN_RANGE( result,  minimum( variable ), maximum( variable ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: vsubset - Extract a reference to the subset of data for the given
         time-range for data with multiple sets of coordinates.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of subset.
         const Timestamp& endTimestamp    End timestamp of subset.
         const int variable               Index of data variable.
RETURNS: std::vector<SubsetPointers>  Pointers to coordinate and data
                                          subset and count
                                          or empty if there is no data within
                                          the specified time range.
NOTES:   Called by public member function subsetData().
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
  ProfileDataset::vsubset( const Timestamp beginTimestamp,
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
    const int variable0 = variable == IMISSING ? dataVariable_ : variable;
    const size_t totalPoints = timestamps_.size();
    size_t firstSubsetPoint = 0;
    size_t subsetPointCount = 0;
    long long currentId = 0;
    size_t profile = 0;
    size_t sumPoints = points_[ 0 ];

    // Find and copy reference to each set of points within subset hour range:

    for ( size_t point = 0; point < totalPoints; ++point ) {
      CHECK( point < timestamps_.size() );
      const Timestamp& pointTimestamp = timestamps_[ point ];

      if ( pointTimestamp >= beginHour ) {

        if ( pointTimestamp <= endHour ) {
          const long long id = (long long) data_[ point ];

          if ( subsetPointCount == 0 ) {
            subsetPointCount = 1;
            firstSubsetPoint = point;
            currentId = id;
          } else if ( id == currentId ) {
            ++subsetPointCount;
          } else {

            // Store previous set:

            storeSubsetPointers( variable0, firstSubsetPoint, subsetPointCount,
                                 profile, result );

            // Reset for next set with new id:

            firstSubsetPoint = point;
            subsetPointCount = 1;
            currentId = id;
          }
        } else if ( subsetPointCount ) {

          // Store previous set:

          storeSubsetPointers( variable0, firstSubsetPoint, subsetPointCount,
                               profile, result );

          // Clear for no more points in time range:

          firstSubsetPoint = 0;
          subsetPointCount = 0;
          currentId = 0;
        }
      }

      if ( point == sumPoints ) {
        ++profile;
        CHECK2( profile > 0, profile < points_.size() );
        sumPoints += points_[ profile ];
      }
    }

    if ( subsetPointCount ) { // Store last set:
      storeSubsetPointers( variable0, firstSubsetPoint, subsetPointCount,
                           profile, result );
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
                        result[ 0 ].notesCount,
                        result[ 0 ].notes,
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
                        result[ result.size() - 1 ].notesCount,
                        result[ result.size() - 1 ].notes ) ) );
  return result;
}



/******************************************************************************
PURPOSE: storeSubsetPointers - Helper for vsubset() to store subset pointers
         for each set of data within the hour range and from the same id.
INPUTS:  const int variable              Index of data variable.
         const size_t firstSubsetPoint   Index of first data point in subset.
         const size_t subsetPointCount   Number of data points in subset.
         const size_t profile            Profile index.
OUTPUTS: std::vector<SubsetPointers>& output  Updated with pointers to
                                              coordinate and data
                                              subset and count.
NOTES:   Called by private member function vsubset().
******************************************************************************/

void ProfileDataset::storeSubsetPointers( const int variable,
                                          const size_t firstSubsetPoint,
                                          const size_t subsetPointCount,
                                          const size_t profile,
                        std::vector<Dataset::SubsetPointers>& output ) const {

  const size_t totalPoints = timestamps_.size();
  const size_t dataOffset = variable * totalPoints + firstSubsetPoint;
  CHECK4( dataOffset < data_.size(),
          IN_RANGE( dataOffset + subsetPointCount, 1, data_.size() ),
          firstSubsetPoint < coordinates_.size(),
          IN_RANGE( firstSubsetPoint + subsetPointCount, 1,
                    coordinates_.size() ) );
  CHECK( profile < notes_.size() );

  const Dataset::SubsetPointers subsetPointers = {
    subsetPointCount,
    &coordinates_[ firstSubsetPoint ],
    subsetPointCount,
    &data_[ dataOffset ],
    1,
    &notes_[ profile ]
  };

  output.push_back( subsetPointers ); //x
}



/******************************************************************************
PURPOSE: allDistinctNotes - Return a reference to an array of distinct notes.
RETURNS: const std::vector<std::string>& notes_;
******************************************************************************/

const std::vector<std::string>& ProfileDataset::allDistinctNotes() const {
  PRE( true );
  const std::vector<std::string>& result = notes_;
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
ProfileDataset::distinctNotePoint( const std::string& distinctNote ) const {
  PRE( true );
  const Point* result = 0;
  const size_t count = notes_.size();
  size_t pointIndex = 0;
  CHECK( notes_.size() == points_.size() );

  for ( size_t index = 0; index < count; ++index ) {

    if ( distinctNote == notes_[ index ] ) {
      CHECK( pointIndex < coordinates_.size() );
      result = &coordinates_[ pointIndex ];
      index = count; // Stop looping.
    } else {
      pointIndex += points_[ index ]; // Skip this profile's points.
    }
  }

  POST( OR2( result == 0, isPoint( *result ) ) );
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

void ProfileDataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read XDR ASCII header that looks like this:

  Profile 2.0
  http://www.esrl.noaa.gov/gmd/grad/neubrew/,NEUBrewSubset
  2006-07-03T00:00:00-0000 2006-07-03T23:59:59-0000
  # Subset domain: <min_lon> <min_lat> <max_lon> <max_lat>:
  -180 -90 180 90
  # Dimensions: variables profiles:
  6 10
  # Variable names:
  timestamp id longitude latitude elevation ozone
  # Variable units:
  yyyymmddhhmmss - deg deg m molecules/cm3
  # char notes[profiles][80] and
  # MSB 64-bit integers points[profiles] and
  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   data_P[variables][points_P]:

  */

  readAndMatchLine( stream, "Profile 2.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, true ); //x
  timesteps_ = 1 + startingTimestamp_.hoursUntil( endingTimestamp_ );
  skipLine( stream ); //x
  skipLine( stream ); //x
  skipLine( stream ); //x
  size_t dimensions[ 2 ] = { 0, 0 };
  readDimensions( stream, sizeof dimensions / sizeof *dimensions,
                  dimensions ); //x
  const size_t tempVariables = dimensions[ 0 ];
  const size_t profiles = dimensions[ 1 ];
  points_.resize( profiles ); //x
  notes_.resize( profiles ); //x

  readVariablesAndUnits( stream, tempVariables ); //x

  // Remove timestamp, longitude, latitude and elevation variables:

  if ( AND7( variables_ == (int) tempVariables,
             variables_ >= 6,
             variableNames_[ 0 ] == "timestamp",
             variableNames_[ 1 ] == "id",
             variableNames_[ 2 ] == "longitude",
             variableNames_[ 3 ] == "latitude",
             variableNames_[ 4 ] == "elevation" ) ) {

    CHECK2( variableNames_.size() == tempVariables,
            variableUnits_.size() == tempVariables );

    variableNames_.erase( variableNames_.begin(), variableNames_.begin() + 4 );
    variableUnits_.erase( variableUnits_.begin(), variableUnits_.begin() + 4 );

    variableNames_[ 0 ] = "id";
    variableUnits_[ 0 ] = "-";

    variables_ -= 4;
    dataVariable_ = variables_ - 1;
  } else {
    throw std::runtime_error( "Read invalid Profile variables/units." ); //x
  }

  setDatasetNameFromDescriptionAndVariable();
  readAndMatchLine( stream,
                    "# char notes[profiles][80] and\n" );//x
  readAndMatchLine( stream,
                    "# MSB 64-bit integers points[profiles] and\n" );//x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "data_1[variables][points_1] ... "
                    "data_P[variables][points_P]:\n" ); //x

  // Read binary data:

  readStrings( stream, NOTE_LENGTH, notes_ ); //x
  readIntegers( stream, 1LL, LLONG_MAX, points_ ); //x
  CHECK( sum( &points_[ 0 ], points_.size() ) > 0 );
  readProfileData( stream ); //x

  POST020( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ >= 1,
           dataVariable_ == variables_ - 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           (int) timesteps_ ==
              1 + startingTimestamp_.hoursUntil( endingTimestamp_ ),
           cellType_== POINT,
           hasElevation_ == true,
           hasArea_ == false,
           hasVolume_ == false,
           isStationary_ == false,
           isGroup_ == true,
           isStructuredGrid_ == false,
           points_.size() >= 1,
           timestamps_.size() == sum( &points_[ 0 ], points_.size() ),
           isValidCoordinates(),
           isValidData(),
           isValidRange() );
}



/******************************************************************************
PURPOSE: readProfileData - Read and convert profile data.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: timestamps_
         coordinates_
         data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void ProfileDataset::readProfileData( FILE* stream ) {
  PRE08( stream,
         variables_ >= 2,
         coordinates_.size() == 0,
         data_.size() == 0,
         emvl::minimum( &points_[ 0 ], points_.size() ) >= 1,
         timestamps_.size() == data_.size() / variables_,
         minimums_.size() == 0,
         maximums_.size() == 0 );

  /* Read XDR ASCII header that looks like this:
  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   data_P[variables][points_P]:
  */

  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  const size_t profileVariables = 4 + variables_; // time,lon,lat,ele,id,ozone.
  const size_t count = profileVariables * totalPoints;
  std::vector<double> data( count ); //x
  data_.resize( variables_ * totalPoints ); //x
  coordinates_.resize( totalPoints ); //x
  timestamps_.reserve( totalPoints ); //x
  enum {  RANGE_COUNT = 5 };
  const double ranges[ RANGE_COUNT ][ 2 ] = {
    { 2.0e13, 2.9e13 }, // yyyymmddhhmmss.
    { 1.0, DBL_MAX },   // id.
    { -180.0, 180.0 },  // longitude.
    { -90.0, 90.0 },    // latitude.
    { QueryParameters::MINIMUM_ELEVATION, QueryParameters::MAXIMUM_ELEVATION }
  };
  bool ok = false;

  if ( fread( &data[ 0 ], sizeof data[ 0 ], count, stream ) == count ) {
    reverse8ByteWordsIfLittleEndian( &data[ 0 ], count );
    updateMissingValues( &data[ 0 ], count );
    const size_t profiles = points_.size();
    long long yyyymmddhhmmss0 = 0;
    Timestamp timestamp0;
    ok = true;

    for ( size_t profile = 0, dataIndex = 0, pointOffset = 0;
          AND2( ok, profile < profiles );
          pointOffset += points_[ profile ], ++profile ) {
      const size_t points = points_[ profile ];

      for ( size_t profileVariable = 0;
            AND2( ok, profileVariable < profileVariables );
            ++profileVariable ) {
        const double minimum =
          profileVariable < RANGE_COUNT ? ranges[ profileVariable ][ MINIMUM ]
          : -DBL_MAX;
        const double maximum =
          profileVariable < RANGE_COUNT ? ranges[ profileVariable ][ MAXIMUM ]
          : DBL_MAX;

        for ( size_t point = 0; AND2( ok, point < points ); ++point,
              ++dataIndex ) {
          CHECK( dataIndex < data.size() );
          const double value = data[ dataIndex ];
          ok = IN_RANGE( value, minimum, maximum );

          if ( ok ) {

            if ( profileVariable == 0 ) { // timestamp:
              const long long yyyymmddhhmmss = (long long) value;

              if ( yyyymmddhhmmss == yyyymmddhhmmss0 ) {
                timestamps_.push_back( timestamp0 );
              } else {
                const int yyyymmdd = yyyymmddhhmmss / 1000000;
                const int hhmmss   = yyyymmddhhmmss % 1000000;
                ok = AND2( Timestamp::isYYYYMMDD( yyyymmdd ),
                           Timestamp::isHHMMSS( hhmmss ) );

                if ( ok ) {
                  const Timestamp timestamp = Timestamp( yyyymmdd, hhmmss );
                  timestamps_.push_back( timestamp );
                  yyyymmddhhmmss0 = yyyymmddhhmmss;
                  timestamp0 = timestamp;
                }
              }
            } else if ( OR2( profileVariable == 1, profileVariable > 4 ) ) {
              const size_t dataVariable =
                profileVariable == 1 ? 0 : profileVariable - 4;
              const size_t dataSaveIndex =
                dataVariable * totalPoints + pointOffset + point;
              CHECK2( dataSaveIndex < data_.size(),
                      data_[ dataSaveIndex ] == 0.0 );
              data_[ dataSaveIndex ] = value;
            } else { // longitude, latitude or elevation:
              const size_t coordinateIndex = pointOffset + point;
              CHECK( coordinateIndex < coordinates_.size() );
              Point& p = coordinates_[ coordinateIndex ];

              if ( profileVariable == 2 ) {
                p.longitude = value;
              } else if ( profileVariable == 3 ) {
                p.latitude = value;
              } else {
                p.elevation = value;
              }
            }
          }
        }
      }
    }
  }

  if ( ! AND5( ok,
               data_.size() == variables_ * totalPoints,
               timestamps_.size() == totalPoints,
               isValidCoordinates(),
               isValidData() ) ) {
    throw std::runtime_error( "Failed to read valid Profile data." ); //x
  }

  computeBounds();
  computeVariableRanges();
}



/******************************************************************************
PURPOSE: writeXDR - Write XDR format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void ProfileDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

  Profile 2.0
  http://www.esrl.noaa.gov/gmd/grad/neubrew/,NEUBrewSubset
  2006-07-03T00:00:00-0000 2006-07-03T23:59:59-0000
  # Subset domain: <min_lon> <min_lat> <max_lon> <max_lat>:
  -180 -90 180 90
  # Dimensions: variables profiles:
  6 10
  # Variable names:
  timestamp id longitude latitude elevation ozone
  # Variable units:
  yyyymmddhhmmss - deg deg m molecules/cm3
  # char notes[profiles][80] and
  # MSB 64-bit integers points[profiles] and
  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   data_P[variables][points_P]:

  */

  Timestamp::UTCString beginningTimestamp = "";
  Timestamp::UTCString endingTimestamp = "";
  startingTimestamp_.toUTCString( beginningTimestamp );
  endingTimestamp_.toUTCString( endingTimestamp );
  const char* const format1 =
    "Profile 2.0\n"
    "%s\n"
    "%s %s\n"
    "# Subset domain: <min_lon> <min_lat> <max_lon> <max_lat>:\n"
    "%lg %lg %lg %lg\n"
    "# Dimensions: variables profiles:\n"
    "%d %" LU "\n";
  bool ok =
    fprintf( stream, format1,
             description_.c_str(),
             beginningTimestamp, endingTimestamp,
             bounds_.west(), bounds_.south(), bounds_.east(), bounds_.north(),
             4 + variables_, points_.size() )
    > (int) strlen( format1 );

  if ( ok ) { // Write variable names:
    const char* const format2 =
      "# Variable names:\n"
      "timestamp id longitude latitude elevation";
    ok = fprintf( stream, format2 ) == (int) strlen( format2 );

    for ( int variable = 1; AND2( ok, variable < variables_); ++variable ) {
      const char* const name = variableNames_[ variable ].c_str();
      ok = fprintf( stream, " %s", name ) == (int) ( 1 + strlen( name ) );
    }

    if ( ok ) { // Write variable units:
      const char* const format3 =
        "\n# Variable units:\n"
        "yyyymmddhhmmss - deg deg m";
      ok = fprintf( stream, format3 ) == (int) strlen( format3 );

      for ( int variable = 1; AND2( ok, variable < variables_ ); ++variable ) {
        const char* const units = variableUnits_[ variable ].c_str();
        ok = fprintf( stream, " %s", units ) == (int) ( 1 + strlen( units ) );
      }

      if ( ok ) {
        const char* const end =
          "\n# char notes[profiles][80] and\n"
          "# MSB 64-bit integers points[profiles] and\n"
          "# IEEE-754 64-bit reals data_1[variables][points_1] ... "
          "data_P[variables][points_P]:\n";
        ok = fprintf( stream, "%s", end ) == (int) strlen( end );

        if ( ok ) {
          writeStrings( stream, NOTE_LENGTH, notes_ ); //x
          writeIntegers( stream, 1LL, LLONG_MAX, points_ ); //x
          writeProfileData( stream ); //x
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Profile data." ); //x
  }

  POST( true );
}




/******************************************************************************
PURPOSE: writeProfileData - Write XDR format profile data to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void ProfileDataset::writeProfileData( FILE* stream ) const {
  PRE( stream );

  /* Write XDR data that looks like this:

  # IEEE-754 64-bit reals data_1[variables][points_1] ... \
   data_P[variables][points_P]:

  */

  const size_t profiles = points_.size();
  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  const size_t profileVariables = 4 + variables_; // time,lon,lat,ele,id,ozone.
  const size_t count = profileVariables * totalPoints;
  size_t coordinateIndex = 0;
  size_t timestampIndex = 0;
  size_t bufferIndex = 0;
  std::vector<double> buffer( count ); //x

  for ( size_t profile = 0, pointOffset = 0; profile < profiles;
        pointOffset += points_[ profile ], ++profile ) {
    const size_t points = points_[ profile ];
    size_t coordinateIndex2 = coordinateIndex;
    size_t coordinateIndex3 = coordinateIndex;

    // Copy timestamps:

    for ( size_t point = 0; point < points; ++point,
          ++timestampIndex, ++bufferIndex ) {
      CHECK2( timestampIndex < timestamps_.size(),
              bufferIndex < buffer.size() );
      const Timestamp& timestamp = timestamps_[ timestampIndex ];
      const long long yyyymmdd = timestamp.yyyymmdd();
      const long long hhmmss   = timestamp.hhmmss();
      const long long yyyymmddhhmmss = yyyymmdd * 1000000LL + hhmmss;
      const double value = yyyymmddhhmmss;
      buffer[ bufferIndex ] = value;
    }

    // Copy ids:

    for ( size_t point = 0; point < points; ++point, ++bufferIndex ) {
      const size_t dataIndex = pointOffset + point;
      CHECK2( dataIndex < data_.size(),
              bufferIndex < buffer.size() );
      const double value = data_[ dataIndex ];
      buffer[ bufferIndex ] = value;
    }

    // Copy longitudes:

    for ( size_t point = 0; point < points; ++point,
          ++coordinateIndex, ++bufferIndex ) {
      CHECK2( coordinateIndex < coordinates_.size(),
              bufferIndex < buffer.size() );
      const double value = coordinates_[ coordinateIndex ].longitude;
      buffer[ bufferIndex ] = value;
    }

    // Copy latitudes:

    for ( size_t point = 0; point < points; ++point,
          ++coordinateIndex2, ++bufferIndex ) {
      CHECK2( coordinateIndex2 < coordinates_.size(),
              bufferIndex < buffer.size() );
      const double value = coordinates_[ coordinateIndex2 ].latitude;
      buffer[ bufferIndex ] = value;
    }

    // Copy elevations:

    for ( size_t point = 0; point < points; ++point,
          ++coordinateIndex3, ++bufferIndex ) {
      CHECK2( coordinateIndex3 < coordinates_.size(), bufferIndex < buffer.size() );
      const double value = coordinates_[ coordinateIndex3 ].elevation;
      buffer[ bufferIndex ] = value;
    }

    // Copy other variables:

    for ( size_t variable = 5; variable < profileVariables; ++variable ) {
      const size_t dataVariable = variable - 4;
      const size_t variableOffset = dataVariable * totalPoints;

      for ( size_t point = 0; point < points; ++point, ++bufferIndex ) {
        const size_t dataIndex = variableOffset + pointOffset + point;
        CHECK2( dataIndex < data_.size(), bufferIndex < buffer.size() );
        const double value = data_[ dataIndex ];
        buffer[ bufferIndex ] = value;
      }
    }
  }

  reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], count );
  const bool ok =
    fwrite( &buffer[0], sizeof buffer[0], count, stream ) == count;

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Profile data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void ProfileDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

  timestamp(UTC) id(-) longitude(deg) latitude(deg) elevation(m) \
   ozone(molecules/cm3)
  2010-07-10T07:00:00-0000  154  -95.341000  29.718000  1000.000000  \
   766000000000.000000
   ...

  */

  // Write header row:

  const char* const header =
    "timestamp(UTC)\tid(-)\tlongitude(deg)\tlatitude(deg)\televation(deg)";
  bool ok = fprintf( stream, "%s", header ) == 64;
  const char* const headerFormat = "\t%s(%s)";

  for ( int variable = 1; AND2( ok, variable < variables_ ); ++variable ) {
    ok = fprintf( stream, headerFormat,
                  variableNames_[ variable ].c_str(),
                  variableUnits_[ variable ].c_str() ) >= 5;
  }

  ok = AND2( ok, fprintf( stream, "\tnote(-)\n" ) == 9 );

  // Write data rows:

  if ( ok ) {
    const char* const dataFormatF = "\t%11.5f";
    const char* const dataFormatE = "\t%11.5e";
    const size_t wordSize = 48;
    char word[ wordSize ] = "";
    const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
    const size_t maximumPoints = emvl::maximum( &points_[ 0 ], points_.size());
    const size_t profiles = points_.size();
    const size_t bufferSize =
      ( 4 + variables_ ) * maximumPoints * wordSize + 1;
    std::vector<char> buffer( bufferSize + 1 ); //x
    size_t coordinateIndex = 0;
    size_t dataOffset = 0;

    for ( size_t profile = 0, pointOffset = 0;
          AND2( ok, profile < profiles );
          pointOffset += points_[ profile ], ++profile ) {
      const size_t points = points_[ profile ];
      buffer.assign( buffer.size(), '\0' );
      char* outputBuffer = &buffer[ 0 ];

      // Write one profile worth of buffered data (for performance):

      for ( size_t point = 0; point < points; ++point, ++coordinateIndex ) {

        // Write timestamp:

        CHECK( coordinateIndex < timestamps_.size() );
        const Timestamp& timestamp = timestamps_[ coordinateIndex ];
        Timestamp::UTCString utcString = "";
        timestamp.toUTCString( utcString );
        strncpy( outputBuffer, utcString, Timestamp::UTC_STRING_LENGTH );
        outputBuffer += Timestamp::UTC_STRING_LENGTH;

        // Write id:

        size_t dataIndex = dataOffset;
        CHECK( dataIndex < data_.size() );
        size_t count = snprintf( word, sizeof word / sizeof *word, "\t%11.0f",
                                 data_[ dataIndex ] );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count);
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

        CHECK( coordinateIndex < coordinates_.size() );
        const Point p = coordinates_[ coordinateIndex ];
        const double longitude = p.longitude;
        const double latitude  = p.latitude;
        const double elevation = p.elevation;

        // Write longitude:

        count =
          snprintf( word, sizeof word / sizeof *word, dataFormatF, longitude );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

        // Write latitude:

        count =
          snprintf( word, sizeof word / sizeof *word, dataFormatF, latitude );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

        // Write elevation:

        count =
          snprintf( word, sizeof word / sizeof *word, dataFormatF, elevation );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

        // Write variables:

        for ( int variable = 1; variable < variables_; ++variable ) {
          const size_t variableOffset = variable * totalPoints;
          dataIndex = variableOffset + pointOffset + point;
          CHECK( dataIndex < data_.size() );
          const double value = data_[ dataIndex ];
          CHECK( IN_RANGE( value, minimums_[ variable ], maximums_[variable]));
          count =
            snprintf( word, sizeof word / sizeof *word, dataFormatE, value );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count);
          strncpy( outputBuffer, word, count );
          outputBuffer += count;
        }

        // End row:

        count = snprintf( outputBuffer,
                          bufferSize - (size_t) ( outputBuffer - &buffer[ 0 ]),
                          "\t%-80s\n", notes_[ profile ].c_str() );
        outputBuffer += count;
      }

      // Write buffered output to stream:

      const size_t bufferLength = (size_t) ( outputBuffer - &buffer[ 0 ] );
      CHECK2( bufferLength <= bufferSize,
              strlen( &buffer[ 0 ] ) <= bufferSize );
      ok = fwrite( &buffer[ 0 ], sizeof (char), bufferLength, stream )
             == bufferLength;
      dataOffset += points;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to write ASCII format Profile data."); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void ProfileDataset::writeCOARDS( const std::string& fileName ) const {
  PRE( fileName.length() );

  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  const size_t outputVariables = 5 + variables_; // date,time,id,lon,lat,elv,o3
  const size_t fileSizeEstimate = outputVariables * totalPoints * 4 + 5000;
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    CHECK( totalPoints <= INT_MAX );
    enum { DIMENSIONS = 2 };
    const char* const names[ DIMENSIONS ] = { "points", "length" };
    const size_t sizes[ DIMENSIONS ] = { totalPoints, NOTE_LENGTH };
    int ids[ DIMENSIONS ] = { -1, -1 };

    if ( createNetCDFDimensions( file, 2, names, sizes, ids ) ) {

      if ( createNetCDFVariable( file, "notes", "-", NC_CHAR, 0, DIMENSIONS,
                                 ids ) != -1 ) {

        writeCOARDSCommon( file, 0, false, false, true, true, 1,
                           DIMENSIONS - 1, ids, sizes, names, timestamps_); //x

        // Create expanded copy of notes for each point:

        std::vector<std::string> notesPerPoint( totalPoints ); //x
        const size_t profiles = points_.size();

        for ( size_t profile = 0, index = 0; profile < profiles; ++profile ) {
          const size_t profilePoints = points_[ profile ];
          const std::string& profileNote = notes_[ profile ];

          for ( size_t point = 0; point < profilePoints; ++point, ++index ) {
            notesPerPoint[ index ] = profileNote; //x
          }
        }

        writeCOARDSNotes( file, NOTE_LENGTH, notesPerPoint ); //x
        ok = true;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create NetCDF-COARDS file for "
                              "Profile data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void ProfileDataset::writeKML() const {
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
    isVector_ ? "Surface profile instrument-measured vector data."
    : "Surface profile instrument-measured scalar data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;
  const int components = isVector_ ? 2 : 1;
  const double variableMinimum = minimum();
  const double variableMaximum = maximum();
  const size_t profiles = points_.size();
  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  size_t dataIndex = dataVariable_ * totalPoints;
  size_t dataIndex2 = isVector_ ? totalPoints + dataIndex : 0;
  char expandedSource[ 256 ] = "";

  for ( size_t profile = 0, pointIndex = 0;
        profile < profiles; ++profile ) {
    const size_t profilePoints = points_[ profile ];
    const char* const note = notes_[ profile ].c_str();
    memset( expandedSource, 0, sizeof expandedSource );
    snprintf( expandedSource,
             sizeof expandedSource / sizeof *expandedSource - 1,
             "%s profile %s", source.c_str(), note );

    writeStartFolderKML( outputKMLFile_, expandedSource, variable, units,
                         dataType, description );

    for ( size_t point = 0; point < profilePoints;
          ++point, ++dataIndex, ++dataIndex2, ++pointIndex ) {
      CHECK( pointIndex < timestamps_.size() );
      const Timestamp timestamp = timestamps_[ pointIndex ];
      const long long yyyymmdd = timestamp.yyyymmdd();
      const long long hhmmss = timestamp.hhmmss();
      const long long valueTimestamp = yyyymmdd * 10000 + hhmmss / 100;
      CHECK( dataIndex < data_.size() );
      const double value = data_[ dataIndex ];

      if ( value > MISSING ) {
        CHECK2( pointIndex < coordinates_.size(),
                IMPLIES( isVector_,
                         IN_RANGE( dataIndex2,
                                   totalPoints, data_.size() - 1 ) ) );
        const double value2 = isVector_ ? data_[ dataIndex2 ] : MISSING;

        if ( OR2( ! isVector_, value2 > MISSING ) ) {
          const double longitude = coordinates_[ pointIndex ].longitude;
          const double latitude  = coordinates_[ pointIndex ].latitude;
          const double elevation = coordinates_[ pointIndex ].elevation;
          const double pointCoordinates[ 3 ] =
            { longitude, latitude, elevation };
          const double magnitude =
            isVector_ ? sqrt( value * value + value2 * value2 ) : value;
          const Color color =
            dataColorFunction( magnitude, variableMinimum, variableMaximum );

          writeCellDataToKML( outputKMLFile_, source.c_str(), variable, units,
                              valueTimestamp, components, MISSING,
                              value, value2, MISSING,
                              1, pointCoordinates, color );
        }
      }
    }

    writeEndFolderKML( outputKMLFile_ );
  }

  POST( true );
}

} // namespace emvl.



