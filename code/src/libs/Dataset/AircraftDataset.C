/******************************************************************************
PURPOSE: AircraftDataset.C - Dataset for Aircraft measured data.
HISTORY: 2013-10-21 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), ftrackf().
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
#include <AircraftDataset.h>    // For AircraftDataset.

enum { NOTE_LENGTH = 80 };

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~AircraftDataset - Destruct a AircraftDataset.
******************************************************************************/

AircraftDataset::~AircraftDataset() {
}



/******************************************************************************
PURPOSE: AircraftDataset - Construct a AircraftDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

AircraftDataset::AircraftDataset( FILE* stream ) {
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
  isVector_         = false;
  readXDR( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid Aircraft data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: AircraftDataset - Construct a AircraftDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

AircraftDataset::AircraftDataset( const std::string& fileName ) {
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
  isVector_         = false;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid Aircraft data from file " +
                              fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: AircraftDataset - Copy-construct a AircraftDataset.
INPUTS:  const AircraftDataset& other  Object to copy.
******************************************************************************/

AircraftDataset::AircraftDataset( const AircraftDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    points_ = other.points_;
    trackBounds_ = other.trackBounds_;
    notes_ = other.notes_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a AircraftDataset.
INPUTS:  const AircraftDataset& other  Object to copy.
RETURNS: AircraftDataset& *this.
******************************************************************************/

AircraftDataset& AircraftDataset::operator=( const AircraftDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    points_ = other.points_;
    trackBounds_ = other.trackBounds_;
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

Dataset* AircraftDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: AircraftDataset::sample()" ); //x
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

Dataset* AircraftDataset::regrid( const int regridMethod,
                                  const CMAQGridParameters& cmaqGridParameters
                                ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: AircraftDataset::regrid()" ); //x
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

void AircraftDataset::write( const std::string& directory,
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

bool AircraftDataset::invariant() const {
  const bool result =
    AND23( Dataset::invariant(),
           variables_ >= 1,
           cellType_== POINT,
           hasElevation_ == true,
           hasArea_ == false,
           hasVolume_ == false,
           isStationary_ == false,
           isGroup_ == true,
           isStructuredGrid_ == false,
           isSurfaceData() == false,
           timestamps_.size() >= 1,
           timestamps_.size() == coordinates_.size(),
           timestamps_[ 0 ].invariant(),
           timestamps_[ timestamps_.size() - 1 ].invariant(),
           points_.size() >= 1,
           emvl::minimum( &points_[ 0 ], points_.size() ) >= 1,
           emvl::sum( &points_[ 0 ], points_.size() ) == coordinates_.size(),
           notes_.size() == points_.size(),
           notes_[ 0 ][ 0 ],
           notes_[ notes_.size() - 1 ][ 0 ],
           trackBounds_.size() == points_.size(),
           trackBounds_[ 0 ].invariant(),
           trackBounds_[ trackBounds_.size() - 1 ].invariant() );
  return result;
}



/******************************************************************************
PURPOSE: isSurfaceData - Is this dataset only on the Earth surface?
RETURNS: bool false.
******************************************************************************/

bool AircraftDataset::isSurfaceData() const {
  const bool result = false;
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool AircraftDataset::operator==( const AircraftDataset& other ) const {
  PRE( other.invariant() );
  bool result =
    OR2( this == &other,
         AND3( equal( other ),
               timestamps_.size() == other.timestamps_.size(),
               points_.size() == other.points_.size() ) );
  const size_t tracks = points_.size();

  for ( size_t track = 0; AND2( result, track < tracks ); ++track ) {
    result =
      AND2( points_[ track ] == other.points_[ track ],
            notes_[ track ] == other.notes_[ track ] );
    const size_t points = points_[ track ];

    for ( size_t point = 0; AND2( result, point < points ); ++point ) {
      result = timestamps_[ point ] == other.timestamps_[ point ];
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool AircraftDataset::operator!=( const AircraftDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: tracks - Number of satellite tracks.
RETURNS: size_t Number of satellite tracks.
******************************************************************************/

size_t AircraftDataset::tracks() const {
  PRE( true );
  const size_t result = points_.size();
  POST( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: points - Points in a satellite track.
INPUTS:  const size_t track  Index of track.
RETURNS: size_t number of points in a track.
******************************************************************************/

size_t AircraftDataset::points( const size_t track ) const {
  PRE( track < points_.size() );
  const size_t result = points_[ track ];
  POST( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: trackBounds - Bounds of a satellite track.
INPUTS:  const size_t track  Index of track.
RETURNS: const Bounds& Bounds of a track.
******************************************************************************/

const Bounds& AircraftDataset::trackBounds( const size_t track ) const {
  PRE( track < trackBounds_.size() );
  const Bounds& result = trackBounds_[ track ];
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: note - Note of a satellite track.
INPUTS:  const size_t track  Index of track.
RETURNS: const std::string& note of a track.
******************************************************************************/

const std::string& AircraftDataset::note( const size_t track ) const {
  PRE( track < notes_.size() );
  const std::string& result = notes_[ track ];
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

double AircraftDataset::vprobe( const Timestamp timestamp,
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
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  const double nearEnough = 0.1; // Probe close enough to data point.
  double result = MISSING;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {

    if ( isInTimeRange( timestamp ) ) {

      // Search for nearest point to timestamp-longitude-latitude-elevation:

      const size_t tracks = points_.size();
      size_t nearestPointIndex = 0;
      size_t nearestTrackIndex = 0;
      double distance = 1e6; // Use Manhatten distance for speed.

      for ( size_t track = 0, pointIndex = 0; track < tracks; ++track ) {
        CHECK( track < points_.size() );
        const size_t trackPoints = points_[ track ];
        const size_t lastTrackPointIndex = pointIndex + trackPoints - 1;
        CHECK3( pointIndex < timestamps_.size(),
                IN_RANGE( lastTrackPointIndex, pointIndex,
                          timestamps_.size() - 1 ),
                track < trackBounds_.size() );
        const Bounds& trackBounds = trackBounds_[ track ];
        const Timestamp firstTrackTimestamp =
          timestamps_[ pointIndex ].startOfHour();
        const Timestamp lastTrackTimestamp =
          timestamps_[ lastTrackPointIndex ].endOfHour();

        if ( AND3( IN_RANGE( timestamp, firstTrackTimestamp,
                             lastTrackTimestamp ),
                   IN_RANGE( longitude,
                             trackBounds.west(), trackBounds.east() ),
                   IN_RANGE( latitude,
                             trackBounds.south(), trackBounds.north() ) ) ) {

          for ( size_t point = 0; point < trackPoints; ++point, ++pointIndex) {
            CHECK( pointIndex < timestamps_.size() );
            const Timestamp pointTimestamp = timestamps_[ pointIndex ];

            if ( pointTimestamp.sameHour( timestamp ) ) {
              CHECK( pointIndex < coordinates_.size() );
              const Point& p = coordinates_[ pointIndex ];
              const double pointLongitude = p.longitude;
              const double longitudeDistance =
                pointLongitude <= longitude ? longitude - pointLongitude
                : pointLongitude - longitude;

              if ( longitudeDistance < distance ) {
                const double pointLatitude  = p.latitude;
                const double latitudeDistance =
                  pointLatitude <= latitude ? latitude - pointLatitude
                  : pointLatitude - latitude;

                if ( latitudeDistance < distance ) {
                  double elevationDistance = elevation == MISSING ? 0.0 : 1.0;

                  if ( elevationDistance ) {
                    const double pointElevation = p.elevation;
                    elevationDistance =
                      pointElevation <= elevation ? elevation - pointElevation
                      : pointElevation - elevation;
                  }

                  if ( elevationDistance < distance ) {
                    distance =
                      longitudeDistance + latitudeDistance + elevationDistance;
                    nearestPointIndex = pointIndex;
                    nearestTrackIndex = track;
                  }
                }
              }
            }
          }
        } else {
          pointIndex += trackPoints;
        }
      }

      if ( distance <= nearEnough ) {
        const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
        const int probeVariable =
          variable != IMISSING ? variable
          : isVector_ ? variables_ - 2
          : dataVariable_;
        const size_t dataIndex =
          probeVariable * totalPoints + nearestPointIndex;
        CHECK( dataIndex < data_.size() );
        result = data_[ dataIndex ];

        if ( nearestTrackIndex < notes_.size() ) {
          probedNote_ = notes_[ nearestTrackIndex ];
        }

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
         const int variable               Index of data variable.
RETURNS: std::vector<SubsetPointers>  Pointers to coordinate and data
                                          subset and count
                                          or empty if there is no data within
                                          the specified time range.
NOTES:   Called by public member function subsetData().
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
  AircraftDataset::vsubset( const Timestamp beginTimestamp,
                            const Timestamp endTimestamp,
                            const int variable ) const {
  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable  == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<Dataset::SubsetPointers> result;
  Timestamp beginHour( beginTimestamp );
  Timestamp endHour( endTimestamp );

  if ( overlappedTimeRange( beginHour, endHour ) ) {

    // Search for points within the subset time range:

    const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
    const size_t tracks = points_.size();

    for ( size_t track = 0, pointIndex = 0; track < tracks; ++track ) {
      CHECK( track < points_.size() );
      const size_t trackPoints = points_[ track ];
      const size_t lastTrackPointIndex = pointIndex + trackPoints - 1;
      CHECK3( pointIndex < timestamps_.size(),
              IN_RANGE( lastTrackPointIndex, pointIndex,
                        timestamps_.size() - 1 ),
              track < trackBounds_.size() );
      const Timestamp firstTrackTimestamp = timestamps_[ pointIndex ];
      const Timestamp lastTrackTimestamp  = timestamps_[ lastTrackPointIndex ];
      const bool outsideTrackTimeRange =
        OR2( endTimestamp < firstTrackTimestamp,
             beginTimestamp > lastTrackTimestamp );

      if ( outsideTrackTimeRange ) {
        pointIndex += trackPoints;
      } else {
        size_t subsetPointOffset = 0;
        size_t subsetCount = 0;

        for ( size_t point = 0; point < trackPoints; ++point, ++pointIndex ) {
          CHECK( pointIndex < timestamps_.size() );
          const Timestamp pointTimestamp = timestamps_[ pointIndex ];
          const bool pointIsInTimeRange =
            IN_RANGE( pointTimestamp, beginHour, endHour );

          if ( pointIsInTimeRange ) {

            if ( subsetCount == 0 ) {
              subsetCount = 1;
              subsetPointOffset = pointIndex;
            } else {
              ++subsetCount;
            }
          }

          /*
           * If there are subset points
           * and we've reached the end of the contiguous sequence of points
           * within the subset or the end of the track, then store pointers
           * to this subset:
           */

          if ( AND2( subsetCount,
                     OR2( ! pointIsInTimeRange, point == trackPoints - 1 ))) {

            if ( OR2( ! isVector_, variable != IMISSING ) ) {
              const int variable0 =
                variable == IMISSING ? dataVariable_ : variable;
              const size_t dataOffset =
                variable0 * totalPoints + subsetPointOffset;
              CHECK5( dataOffset <  data_.size(),
                      IN_RANGE( dataOffset + subsetCount, 1, data_.size() ),
                      subsetPointOffset < coordinates_.size(),
                      IN_RANGE( subsetPointOffset + subsetCount, 1,
                                coordinates_.size() ),
                      track < notes_.size() );
              const Dataset::SubsetPointers subsetPointers = {
                subsetCount,
                &coordinates_[ subsetPointOffset ],
                subsetCount,
                &data_[ dataOffset ],
                1,
                &notes_[ track ]
              };
              result.push_back( subsetPointers ); //x
            } else { // Append each vector component variable:
              CHECK2( isVector_, variables_ >= 2 );

              for ( int component = variables_ - 2; component < variables_;
                    ++component ) {
                const size_t dataOffset =
                  component * totalPoints + subsetPointOffset;
                CHECK5( dataOffset < data_.size(),
                        IN_RANGE( dataOffset + subsetCount, 1, data_.size() ),
                        subsetPointOffset < coordinates_.size(),
                        IN_RANGE( subsetPointOffset + subsetCount, 1,
                                  coordinates_.size() ),
                        track < notes_.size() );
                const Dataset::SubsetPointers subsetPointers = {
                  subsetCount,
                  &coordinates_[ subsetPointOffset ],
                  subsetCount,
                  &data_[ dataOffset ],
                  1,
                  &notes_[ track ]
                };
                result.push_back( subsetPointers ); //x
              }
            }

            subsetCount = 0; // Reset count for next subset.
          }
        }
      }
    }
  }

  POST( IMPLIES( result.size(),
                 AND21( result[ 0 ].dataCount,
                        result[ 0 ].coordinatesCount == result[ 0 ].dataCount,
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
                        result[ 0 ].notesCount == 1,
                        result[ 0 ].notes,
                        result[ result.size() - 1 ].dataCount,
                        result[ result.size() - 1 ].coordinatesCount ==
                          result[ result.size() - 1 ].dataCount,
                        result[ result.size() - 1 ].coordinates,
                        result[ result.size() - 1 ].data,
                        isPoint( result[ result.size() - 1 ].coordinates[ 0 ]),
                        isPoint( result[ result.size() - 1 ].coordinates[
                          result[ result.size() - 1 ].coordinatesCount - 1 ] ),
                        IMPLIES( AND2( variable != IMISSING,
                                  emvl::minimum( result[result.size() - 1].data,
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
                        result[ result.size() - 1 ].notesCount == 1,
                        result[ result.size() - 1 ].notes,
                        IMPLIES( AND2( isVector(), variable == IMISSING ),
                                 result.size() >= 2 ) ) ) );

  return result;
}



/******************************************************************************
PURPOSE: allDistinctNotes - Return a reference to an array of distinct notes.
RETURNS: const std::vector<std::string>& notes_;
******************************************************************************/

const std::vector<std::string>& AircraftDataset::allDistinctNotes() const {
  PRE( true );
  const std::vector<std::string>& result = notes_;
  POST( true );
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

void AircraftDataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read XDR ASCII header that looks like this:

  Aircraft 2.0
  http://mozaic.aero.obs-mip.fr/web/,MOZAICSubset
  2006-07-03T00:00:00-0000 2006-07-03T23:59:59-0000
  # Subset domain: <min_lon> <min_lat> <max_lon> <max_lat>:
  -180 -90 180 90
  # Dimensions: variables points tracks:
  16 578 3
  # Variable names:
   timestamp longitude latitude elevation radio_altitude pressure temperature \
   air_speed ground_speed wind_direction wind_speed ozone \
   h2o_static_temperature relative_humidity h2o co
  # Variable units:
   yyyymmddhhmmss deg deg m m Pa C m/s m/s deg m/s ppmV C % g/kg ppmV
  # char notes[tracks][80] and
  # IEEE-754 64-bit reals bounds[tracks][2=lon,lat][2=min,max] and
  # MSB 64-bit integers points[tracks] and
  # IEEE-754 64-bit reals data_1[points_1][variables] ... \
   data_T[points_T][variables]:

  */

  readAndMatchLine( stream, "Aircraft 2.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, true ); //x

  // Set timesteps_ to # of hours in [startingTimestamp_, endingTimestamp_].

  timesteps_ = 1 + startingTimestamp_.hoursUntil( endingTimestamp_ );

  skipLine( stream ); //x
  skipLine( stream ); //x
  skipLine( stream ); //x
  size_t dimensions[ 3 ] = { 0, 0, 0 };
  readDimensions( stream, sizeof dimensions / sizeof *dimensions,
                  dimensions ); //x
  const size_t tempVariables = dimensions[ 0 ];
  const size_t tracks = dimensions[ 2 ];
  points_.resize( tracks ); //x
  notes_.resize( tracks ); //x
  trackBounds_.resize( tracks ); //x
  readVariablesAndUnits( stream, tempVariables ); //x

  // Check then remove timestamp, longitude, latitude, elevation variables:

  if ( AND6( variables_ == (int) tempVariables,
             variables_ >= 5,
             variableNames_[ 0 ] == "timestamp",
             variableNames_[ 1 ] == "longitude",
             variableNames_[ 2 ] == "latitude",
             variableNames_[ 3 ] == "elevation" ) ) {
    CHECK2( variableNames_.size() == tempVariables,
            variableUnits_.size() == tempVariables );
    variableNames_.erase( variableNames_.begin(), variableNames_.begin() + 4 );
    variableUnits_.erase( variableUnits_.begin(), variableUnits_.begin() + 4 );
    variables_ -= 4;
    dataVariable_ = variables_ - 1;
  } else {
    throw std::runtime_error( "Read invalid Aircraft variables/units." ); //x
  }

  isVector_ =
    AND3( variables_ == 2,
          strcmp( variableNames_[ 0 ].c_str(), "wind_u" ) == 0,
          strcmp( variableNames_[ 1 ].c_str(), "wind_v" ) == 0 );

  setDatasetNameFromDescriptionAndVariable();
  readAndMatchLine( stream, "# char notes[tracks][80] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "bounds[tracks][2=lon,lat][2=min,max] and\n");//x
  readAndMatchLine( stream, "# MSB 64-bit integers "
                    "points[tracks] and\n");//x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "data_1[points_1][variables] ... "
                    "data_T[points_T][variables]:\n" ); //x

  // Read binary data:

  readStrings( stream, NOTE_LENGTH, notes_ ); //x
  readBounds( stream, trackBounds_.size(), &trackBounds_[ 0 ] ); //x
  readIntegers( stream, 1LL, LLONG_MAX, points_ ); //x
  readTrackData( stream ); //x

  POST022( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ >= 1,
           dataVariable_ == variables_ - 1,
           timesteps_ >= 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           cellType_== POINT,
           hasElevation_ == true,
           hasArea_ == false,
           hasVolume_ == false,
           isStationary_ == false,
           isGroup_ == true,
           isStructuredGrid_ == false,
           points_.size() >= 1,
           timestamps_.size() >= 1,
           timestamps_.size() == sum( &points_[ 0 ], points_.size() ),
           isValidCoordinates(),
           isValidData(),
           isValidRange() );
}



/******************************************************************************
PURPOSE: readTrackData - Read and convert track data.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: coordinates_
         data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void AircraftDataset::readTrackData( FILE* stream ) {
  PRE08( stream,
         variables_ >= 1,
         timestamps_.size() == 0,
         coordinates_.size() == 0,
         data_.size() == 0,
         emvl::minimum( &points_[ 0 ], points_.size() ) >= 1,
         minimums_.size() == 0,
         maximums_.size() == 0 );

  /* Read XDR data that looks like this:
  # IEEE-754 64-bit reals data_1[points_1][variables] ... \
                          data_T[points_T][variables]:
  */

  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  timestamps_.reserve( totalPoints ); //x
  coordinates_.reserve( totalPoints ); //x
  data_.resize( variables_ * totalPoints ); //x
  const size_t maximumPoints = emvl::maximum( &points_[ 0 ], points_.size() );
  const size_t trackVariables = 4 + variables_; // time, lon, lat, elv + co,...
  std::vector<double> buffer( maximumPoints * trackVariables ); //x
  const size_t tracks = points_.size();
  bool ok = true;

  for ( size_t track = 0, dataIndex = 0; AND2( ok, track < tracks ); ++track ) {
    const size_t points = points_[ track ];
    const size_t count = points * trackVariables;
    ok = fread( &buffer[ 0 ], sizeof buffer[ 0 ], count, stream ) == count;

    if ( ok ) {
      reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], count );
      updateMissingValues( &buffer[ 0 ], count );

      // Copy and store timestamps, longitudes, latitudes, elevations, co, ...:

      for ( size_t point = 0, bufferIndex = 0;
            AND2( ok, point < points );
            ++point, ++dataIndex ) {
        CHECK(  bufferIndex + trackVariables <= buffer.size() );
        const double dtimestamp = buffer[ bufferIndex ];
        ++bufferIndex;
        const double longitude = buffer[ bufferIndex ];
        ++bufferIndex;
        const double latitude = buffer[ bufferIndex ];
        ++bufferIndex;
        const double elevation = buffer[ bufferIndex ];
        ++bufferIndex;
        const long long yyyymmddhhmmss = (long long) dtimestamp;
        const long long yyyymmdd = yyyymmddhhmmss / 1000000;
        const long long hhmmss   = yyyymmddhhmmss % 1000000;

        ok = AND5( Timestamp::isYYYYMMDD( yyyymmdd ),
                   Timestamp::isHHMMSS( hhmmss ),
                   isLongitude( longitude ),
                   isLatitude( latitude ),
                   isElevation( elevation ) );

        if ( ok ) {
          const Timestamp timestamp( yyyymmdd, hhmmss );
          timestamps_.push_back( timestamp );
          Point p = { 0.0, 0.0, 0.0 };
          p.longitude = longitude;
          p.latitude  = latitude;
          p.elevation = elevation;
          coordinates_.push_back( p );

          for ( int variable = 0;
                AND2( ok, variable < variables_ );
                ++variable, ++bufferIndex ) {
            CHECK( bufferIndex < buffer.size() );
            const double value = buffer[ bufferIndex ];
            ok = isFinite( value );

            if ( ok ) {
              const size_t variableIndex = variable * totalPoints + dataIndex;
              CHECK( variableIndex < data_.size() );
              data_[ variableIndex ] = value;
            }
          }
        }
      }
    }
  }

  if ( ! AND6( ok,
               coordinates_.size() == totalPoints,
               timestamps_.size() == totalPoints,
               data_.size() == variables_ * totalPoints,
               isValidCoordinates(),
               isValidData() ) ) {
    throw std::runtime_error( "Failed to read valid Aircraft data." ); //x
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

void AircraftDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

  Aircraft 2.0
  http://mozaic.aero.obs-mip.fr/web/,MOZAICSubset
  2006-07-03T00:00:00-0000 2006-07-03T23:59:59-0000
  # Subset domain: <min_lon> <min_lat> <max_lon> <max_lat>:
  -180 -90 180 90
  # Dimensions: variables points tracks:
  16 578 3
  # Variable names:
   timestamp longitude latitude elevation radio_altitude pressure temperature \
   air_speed ground_speed wind_direction wind_speed ozone \
   h2o_static_temperature relative_humidity h2o co
  # Variable units:
   yyyymmddhhmmss deg deg m m Pa C m/s m/s deg m/s ppmV C % g/kg ppmV
  # char notes[tracks][80] and
  # IEEE-754 64-bit reals bounds[tracks][2=lon,lat][2=min,max] and
  # MSB 64-bit integers points[tracks] and
  # IEEE-754 64-bit reals data_1[points_1][variables] ... \
   data_T[points_T][variables]:

  */

  Timestamp::UTCString firstTimestamp = "";
  Timestamp::UTCString lastTimestamp = "";
  startingTimestamp_.toUTCString( firstTimestamp );
  endingTimestamp_.toUTCString( lastTimestamp );
  const char* const format1 =
    "Aircraft 2.0\n"
    "%s\n"
    "%s %s\n"
    "# Subset domain: <min_lon> <min_lat> <max_lon> <max_lat>:\n"
    "%lg %lg %lg %lg\n"
    "# Dimensions: variables points tracks:\n"
    "%d %" LU "%" LU "\n";
  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  bool ok =
    fprintf( stream, format1,
             description_.c_str(),
             firstTimestamp, lastTimestamp,
             bounds_.west(), bounds_.east(), bounds_.south(), bounds_.north(),
             4 + variables_, totalPoints, points_.size() )
    > (int) strlen( format1 );

  if ( ok ) { // Write variable names:
    const char* const format2 =
      "# Variable names:\ntimestamp longitude latitude elevation";
    ok = fprintf( stream, format2 ) == (int) strlen( format2 );

    for ( int variable = 0; AND2( ok, variable < variables_); ++variable ) {
      const char* const name = variableNames_[ variable ].c_str();
      ok = fprintf( stream, " %s", name ) == (int) ( 1 + strlen( name ) );
    }

    if ( ok ) { // Write variable units:
      const char* const format3 =
        "\n# Variable units:\nyyyymmddhhmmss deg deg m";
      ok = fprintf( stream, format3 ) == (int) strlen( format3 );

      for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
        const char* const units = variableUnits_[ variable ].c_str();
        ok = fprintf( stream, " %s", units ) == (int) ( 1 + strlen( units ) );
      }

      if ( ok ) {
        const char* const end =
          "\n# char notes[tracks][80] and\n"
          "# IEEE-754 64-bit reals bounds[tracks][2=lon,lat][2=min,max] and\n"
          "# MSB 64-bit integers points[tracks] and\n"
          "# IEEE-754 64-bit reals data_1[points_1][variables] ... "
          "data_T[points_T][variables]:\n";
        ok = fprintf( stream, "%s", end ) == (int) strlen( end );

        if ( ok ) {
          writeStrings( stream, NOTE_LENGTH, notes_ ); //x
          writeBounds( stream, trackBounds_.size(), &trackBounds_[ 0 ] ); //x
          const long long minimum = 1;
          const long long maximum = LLONG_MAX;
          writeIntegers( stream, minimum, maximum, points_ ); //x
          writeTrackData( stream ); //x
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Aircraft data."); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeTrackData - Write XDR format track data to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void AircraftDataset::writeTrackData( FILE* stream ) const {
  PRE( stream );

  /* Write XDR data that looks like this:
  # IEEE-754 64-bit reals data_1[points_1][variables] ... \
  data_T[points_T][variables]:
  */

  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  const size_t maximumPoints = emvl::maximum( &points_[ 0 ], points_.size() );
  const size_t trackVariables = 4 + variables_; // time, lon, lat, elv + co,...
  std::vector<double> buffer( maximumPoints * trackVariables ); //x
  const size_t tracks = points_.size();
  size_t timestampIndex = 0;
  size_t coordinateIndex = 0;
  size_t dataOffset = 0;
  bool ok = true;

  for ( size_t track = 0; AND2( ok, track < tracks ); ++track ) {
    const size_t points = points_[ track ];

    // Copy and write timestamps, longitudes, latitudes, elevations, co, ...:

    for ( size_t point = 0, bufferIndex = 0;
          point < points;
          ++point, ++dataOffset,
          ++timestampIndex, ++coordinateIndex ) {
      CHECK3( timestampIndex < timestamps_.size(),
              coordinateIndex < coordinates_.size(),
              bufferIndex + trackVariables <= buffer.size() );
      const Timestamp timestamp = timestamps_[ timestampIndex ];
      const long long yyyymmdd  = timestamp.yyyymmdd();
      const long long hhmmss    = timestamp.hhmmss();
      const long long yyyymmddhhmmss = yyyymmdd * 1000000 + hhmmss;
      const double dtimestamp = (double) yyyymmddhhmmss;
      const Point& p = coordinates_[ coordinateIndex ];
      const double longitude = p.longitude;
      const double latitude  = p.latitude;
      const double elevation = p.elevation;
      buffer[ bufferIndex ] = dtimestamp;
      ++bufferIndex;
      buffer[ bufferIndex ] = longitude;
      ++bufferIndex;
      buffer[ bufferIndex ] = latitude;
      ++bufferIndex;
      buffer[ bufferIndex ] = elevation;
      ++bufferIndex;

      for ( int variable = 0; variable < variables_; ++variable,
            ++bufferIndex ) {
        const size_t dataIndex = variable * totalPoints + dataOffset;
        CHECK2( dataIndex < data_.size(),
                bufferIndex < buffer.size() );
        const double value = data_[ dataIndex ];
        buffer[ bufferIndex ] = value;
      }
    }

    const size_t count = points * trackVariables;
    reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], count );
    ok = fwrite( &buffer[ 0 ], sizeof buffer[ 0 ], count, stream ) == count;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Aircraft data."); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void AircraftDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  Longitude(deg)  Latitude(deg)   ELEVATION(m)  ozone(ppmV)\
      notes(-)
   2011-08-29T22:00:00-0000  -91.03617  31.00816   4321.12345    0.05632\
      MD20060703014:FRANKFURT->ATLANTA
   ...

  */

  // Write header row:

  const char* const format =
    "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tELEVATION(m)";
  bool ok = fprintf( stream, "%s", format ) == (int) strlen( format );
  const char* const headerFormat = "\t%s(%s)";

  for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
    ok = fprintf( stream, headerFormat,
                  variableNames_[ variable ].c_str(),
                  variableUnits_[ variable ].c_str() ) > 4;
  }

  ok = AND2( ok, fprintf( stream, "\tnotes(-)\n" ) == 10 );

  // Write data rows:

  if ( ok ) {
    const char* const dataFormatF = "\t%11.5f";
    const char* const dataFormatE = "\t%11.5e";
    const size_t wordSize = 48;
    char word[ wordSize ] = "";
    const size_t lineLength =
      Timestamp::UTC_STRING_LENGTH + 1 +
      ( 3 + variables_ ) * ( wordSize + 1 ) + NOTE_LENGTH + 1;
    const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
    const size_t maximumPoints = emvl::maximum( &points_[ 0 ], points_.size());
    const size_t tracks = points_.size();
    const size_t bufferSize = maximumPoints * lineLength + 1;
    std::vector<char> buffer( bufferSize + 1 ); //x
    size_t coordinateIndex = 0;
    size_t dataOffset = 0;

    for ( size_t track = 0; AND2( ok, track < tracks ); ++track ) {
      const size_t points = points_[ track ];
      buffer.assign( buffer.size(), '\0' );
      char* outputBuffer = &buffer[ 0 ];

      // Write one track worth of buffered data (for performance):

      for ( size_t point = 0;
            point < points;
            ++point, ++coordinateIndex, ++dataOffset ) {
        CHECK( dataOffset < timestamps_.size() );
        const Timestamp timestamp = timestamps_[ dataOffset ];
        Timestamp::UTCString utcString = "";
        timestamp.toUTCString( utcString );
        strncpy( outputBuffer, utcString, Timestamp::UTC_STRING_LENGTH );
        outputBuffer += Timestamp::UTC_STRING_LENGTH;

        CHECK( coordinateIndex < coordinates_.size() );
        const Point& p = coordinates_[ coordinateIndex ];
        const double longitude = p.longitude;
        const double latitude  = p.latitude;
        const double elevation = p.elevation;

        size_t count =
          snprintf( word, sizeof word / sizeof *word, dataFormatF, longitude );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

        count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                          latitude );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

        count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                          elevation );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

        for ( int variable = 0; variable < variables_; ++variable ) {
          const size_t dataIndex = variable * totalPoints + dataOffset;
          CHECK( dataIndex < data_.size() );
          const double value = data_[ dataIndex ];
          count = snprintf(word, sizeof word / sizeof *word, dataFormatE,value);
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count);
          strncpy( outputBuffer, word, count );
          outputBuffer += count;
        }

        // End row:

        count = snprintf( outputBuffer,
                          bufferSize - (size_t) ( outputBuffer - &buffer[ 0 ]),
                          "\t%-80s\n", notes_[ track ].c_str() );
        outputBuffer += count;
      }

      // Write buffered output to stream:

      const size_t bufferLength = (size_t) ( outputBuffer - &buffer[ 0 ] );
      CHECK2( bufferLength <= bufferSize,
              strlen( &buffer[ 0 ] ) <= bufferSize );
      ok = fwrite( &buffer[ 0 ], sizeof (char), bufferLength, stream )
             == bufferLength;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to write ASCII format Aircraft data.");//x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void AircraftDataset::writeCOARDS( const std::string& fileName ) const {
  PRE( fileName.length() );

  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  const size_t outputVariables = 4 + variables_; // date,time,lon,lat,aod,...
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
        const size_t tracks = points_.size();

        for ( size_t track = 0, index = 0; track < tracks; ++track ) {
          const size_t trackPoints = points_[ track ];
          const std::string& trackNote = notes_[ track ];

          for ( size_t point = 0; point < trackPoints; ++point, ++index ) {
            notesPerPoint[ index ] = trackNote; //x
          }
        }

        writeCOARDSNotes( file, NOTE_LENGTH, notesPerPoint ); //x
        ok = true;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create NetCDF-COARDS file for "
                              "Aircraft data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void AircraftDataset::writeKML() const {
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
    isVector_ ? "Aircraft-measured vector data."
    : "Aircraft-measured scalar data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;
  const int components = isVector_ ? 2 : 1;
  const double variableMinimum = minimum();
  const double variableMaximum = maximum();
  const size_t tracks = points_.size();
  const size_t totalPoints = sum( &points_[ 0 ], points_.size() );
  size_t dataIndex = 0;
  size_t dataIndex2 = isVector_ ? totalPoints : 0;
  char expandedSource[ 256 ] = "";

  for ( size_t track = 0; track < tracks; ++track ) {
    const size_t trackPoints = points_[ track ];
    const char* const note = notes_[ track ].c_str();
    memset( expandedSource, 0, sizeof expandedSource );
    snprintf( expandedSource,
             sizeof expandedSource / sizeof *expandedSource - 1,
             "%s flight %s", source.c_str(), note );

    writeStartFolderKML( outputKMLFile_, expandedSource, variable, units,
                         dataType, description );

    for ( size_t point = 0; point < trackPoints;
          ++point, ++dataIndex, ++dataIndex2 ) {
      const Timestamp timestamp = timestamps_[ dataIndex ];
      const long long yyyymmdd = timestamp.yyyymmdd();
      const long long hhmmss = timestamp.hhmmss();
      const long long valueTimestamp = yyyymmdd * 10000 + hhmmss / 100;
      CHECK( dataIndex < data_.size() );
      const double value = data_[ dataIndex ];

      if ( value > MISSING ) {
        CHECK( IMPLIES( isVector_,
                        IN_RANGE( dataIndex2,
                                  totalPoints, data_.size() - 1 ) ) );
        const double value2 = isVector_ ? data_[ dataIndex2 ] : MISSING;

        if ( OR2( ! isVector_, value2 > MISSING ) ) {
          const double longitude = coordinates_[ dataIndex ].longitude;
          const double latitude  = coordinates_[ dataIndex ].latitude;
          const double elevation = coordinates_[ dataIndex ].elevation;
          const double pointCoordinates[ 3 ] =
            { longitude, latitude, elevation };
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
    }

    writeEndFolderKML( outputKMLFile_ );
  }

  POST( true );
}




} // namespace emvl.



