/******************************************************************************
PURPOSE: CALIPSODataset.C - Dataset for CALIPSO satellite data.
HISTORY: 2013-12-14 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread().
#include <string.h> // For memset(), strstr().
#include <limits.h> // For INT_MAX.
#include <float.h>  // For FLT_MAX.
#include <unistd.h> // For unlink().

#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <memory>    // For std::auto_ptr.

#include <Assertions.h> // For PRE(), POST(), AND(), IN_RANGE().
#include <KMLFile.h>    // write*KML().

#include <DataUtilities.h>      // For isNan(), isLongitude(), clampedToRange()
#include <ResourceHandler.h>    // For ResourceHandler.
#include <Bounds.h>             // For Bounds.
#include <Timestamp.h>          // For Timestamp.
#include <QueryParameters.h>    // For QueryParameters.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <NetCDFUtilities.h>    // For *NetCDF*() routines.
#include <CALIPSODataset.h>     // For CALIPSODataset.

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~CALIPSODataset - Destruct a CALIPSODataset.
******************************************************************************/

CALIPSODataset::~CALIPSODataset() {
}



/******************************************************************************
PURPOSE: CALIPSODataset - Construct a CALIPSODataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

CALIPSODataset::CALIPSODataset( FILE* stream ) {
  PRE0( stream );

  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = QUADRILATERAL;
  hasElevation_     = true;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;
  readXDR( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid CALIPSO data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: CALIPSODataset - Construct a CALIPSODataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

CALIPSODataset::CALIPSODataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = QUADRILATERAL;
  hasElevation_     = true;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = true;
  isStructuredGrid_ = false;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid CALIPSO data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: CALIPSODataset - Copy-construct a CALIPSODataset.
INPUTS:  const CALIPSODataset& other  Object to copy.
******************************************************************************/

CALIPSODataset::CALIPSODataset( const CALIPSODataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    scanBounds_ = other.scanBounds_;
    scanDimensions_ = other.scanDimensions_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a CALIPSODataset.
INPUTS:  const CALIPSODataset& other  Object to copy.
RETURNS: CALIPSODataset& *this.
******************************************************************************/

CALIPSODataset& CALIPSODataset::operator=( const CALIPSODataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    timestamps_ = other.timestamps_;
    scanBounds_ = other.scanBounds_;
    scanDimensions_ = other.scanDimensions_;
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

Dataset* CALIPSODataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: CALIPSODataset::sample()" ); //x
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

Dataset* CALIPSODataset::regrid( const int regridMethod,
                                 const CMAQGridParameters&
                                 cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: CALIPSODataset::regrid()" ); //x
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

void CALIPSODataset::write( const std::string& directory,
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

bool CALIPSODataset::invariant() const {
  const bool result =
    AND17( Dataset::invariant(),
           variables_ >= 1,
           cellType_== QUADRILATERAL,
           hasElevation_ == true,
           hasArea_ == true,
           hasVolume_ == false,
           isStationary_ == false,
           isGroup_ == true,
           isStructuredGrid_ == false,
           timestamps_.size() >= 1,
           scanBounds_.size() == timestamps_.size(),
           scanDimensions_.size() == timestamps_.size() * 2,
           timestamps_[ 0 ].invariant(),
           timestamps_[ timestamps_.size() - 1 ].invariant(),
           scanBounds_[ 0 ].invariant(),
           scanBounds_[ scanBounds_.size() - 1 ].invariant(),
           emvl::minimum( &scanDimensions_[0], scanDimensions_.size() ) >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: isSurfaceData - Is this dataset only on the Earth surface?
RETURNS: bool true for data such as Column_Optical_Depth,
         false for Extinction_Coefficient_532.
******************************************************************************/

bool CALIPSODataset::isSurfaceData() const {
  bool result = true;
  const size_t scans = timestamps_.size();

  for ( size_t scan = 0; AND2( result, scan < scans ); ++scan ) {
    const size_t verticalPointsInScan = scanDimensions_[ scan + scan + 1 ];
    result = verticalPointsInScan == 1;
  }

  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool CALIPSODataset::operator==( const CALIPSODataset& other ) const {
  PRE( other.invariant() );
  bool result =
    OR2( this == &other,
         AND4( equal( other ),
               timestamps_.size() == other.timestamps_.size(),
               scanBounds_.size() == other.scanBounds_.size(),
               scanDimensions_.size() == other.scanDimensions_.size() ) );
  const size_t scans = timestamps_.size();

  for ( size_t scan = 0; AND2( result, scan < scans ); ++scan ) {
    const size_t scan2 = scan + scan;
    result =
      AND4( timestamps_[ scan ] == other.timestamps_[ scan ],
            scanBounds_[ scan ] == other.scanBounds_[ scan ],
            scanDimensions_[ scan2 ] == other.scanDimensions_[ scan2 ],
            scanDimensions_[ scan2 + 1 ] == other.scanDimensions_[ scan2 + 1]);
  }

  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool CALIPSODataset::operator!=( const CALIPSODataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: scans - Number of satellite scans.
RETURNS: size_t Number of satellite scans.
******************************************************************************/

size_t CALIPSODataset::scans() const {
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

Timestamp CALIPSODataset::timestamp( const size_t scan ) const {
  PRE( scan < scans() );
  const Timestamp result = timestamps_[ scan ];
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: scanGroundPoints - Ground points in a satellite scan.
INPUTS:  const size_t scan  Index of scan.
RETURNS: size_t number of ground points in the scan.
******************************************************************************/

size_t CALIPSODataset::scanGroundPoints( const size_t scan ) const {
  PRE( scan < scans() );
  const size_t result = scanDimensions_[ scan + scan ];
  POST( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: scanVerticalPoints - Vertical points in a satellite scan.
INPUTS:  const size_t scan  Index of scan.
RETURNS: size_t number of vertical points in the scan.
******************************************************************************/

size_t CALIPSODataset::scanVerticalPoints( const size_t scan ) const {
  PRE( scan < scans() );
  const size_t result = scanDimensions_[ scan + scan + 1 ];
  POST( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: scanBounds - Bounds of a satellite scan.
INPUTS:  const size_t scan  Index of scan.
RETURNS: const Bounds& bounds of a scan.
******************************************************************************/

const Bounds& CALIPSODataset::scanBounds( const size_t scan ) const {
  PRE( scan < scans() );
  const Bounds& result = scanBounds_[ scan ];
  POST( result.invariant() );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: vprobe - Return nearest data point to time-space location.
INPUTS:  const Timestamp& timestamp  Timestamp of probe.
         const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double elevation      Elevation of probe (or MISSING).
         const int probeLayer        0-based index of variable or IMISSING.
         const int variable          0-based index of variable or IMISSING.
RETURNS: double Data value nearest probe or MISSING if not near any data.
NOTES:   Called by base class member function probe().
         Template Method Design Pattern.
******************************************************************************/

double CALIPSODataset::vprobe( const Timestamp timestamp,
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

  const double nearEnough = 1e-2; // Probe close enough to data point.
  double result = MISSING;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {
    size_t first = 0;
    size_t last = 0;
    const bool hasMatchingTimestamps =
      matchingTimestamps( timestamp, timestamps_, first, last );

    if ( hasMatchingTimestamps ) {

      for ( size_t scan = first; scan <= last; ++scan ) {
        const size_t scan2 = scan + scan;
        const size_t skipPoints =
          scan == 0 ? 0 : sumProduct2( &scanDimensions_[ 0 ], scan2 );
        const size_t scanGroundPoints = scanDimensions_[ scan2 ];
        const size_t scanVerticalPoints = scanDimensions_[ scan2 + 1 ];
        const size_t scanPoints = scanGroundPoints * scanVerticalPoints;
        const size_t pointsPerCell = 4;
        size_t nearestPoint = scanPoints;

        for ( size_t point = 0;
              AND2( nearestPoint == scanPoints, point < scanPoints );
              point += scanVerticalPoints ) {
          const size_t pointIndex = skipPoints + point;
          const size_t cellIndex = pointIndex * pointsPerCell;
          CHECK( cellIndex <= coordinates_.size() - pointsPerCell );
          const Point& point0 = coordinates_[ cellIndex ];
          const Point& point1 = coordinates_[ cellIndex + 1 ];

          // Is point within the cell ground rectangle?

          const double longitudeMinimum =
            point0.longitude < point1.longitude ? point0.longitude
            : point1.longitude;

          if ( longitude >= longitudeMinimum ) {
            const double longitudeMaximum =
              point0.longitude > point1.longitude ? point0.longitude
              : point1.longitude;

            if ( longitude <= longitudeMaximum ) {
              const double latitudeMinimum =
                point0.latitude < point1.latitude ? point0.latitude
                : point1.latitude;

              if ( latitude >= latitudeMinimum ) {
                const double latitudeMaximum =
                  point0.latitude > point1.latitude ? point0.latitude
                  : point1.latitude;

                if ( latitude <= latitudeMaximum ) {
                  nearestPoint = point; // Ground point.
                }
              }
            }
          }
        }

        // If found a matching ground point then find vertical point:

        if ( AND3( nearestPoint < scanPoints,
                   elevation != MISSING,
                   scanVerticalPoints > 1 ) ) {
          const size_t lastVerticalPoint = nearestPoint + scanVerticalPoints;

          for ( size_t point = nearestPoint;
                point < lastVerticalPoint;
                ++point ) {
            const size_t pointIndex = skipPoints + point;
            const size_t cellIndex = pointIndex * pointsPerCell;
            CHECK( cellIndex <= coordinates_.size() - pointsPerCell );
            const double elevationMinimum = coordinates_[ cellIndex].elevation;

            if ( elevation >= elevationMinimum ) {
              const double elevationMaximum =
                coordinates_[ cellIndex + 2 ].elevation;

              if ( elevation <= elevationMaximum ) {
                nearestPoint = point;
              }
            }
          }
        }

        if ( nearestPoint < scanPoints ) {
          const size_t totalPoints =
            sumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );
          const size_t dataIndex =
            variable * totalPoints + skipPoints + nearestPoint;
          CHECK( dataIndex < data_.size() );
          result = data_[ dataIndex ];
          scan = last; // Stop looping.
        }
      } // Loop on scans within time range.
    }  // has matching timestamps.
  } // In bounds.

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
CALIPSODataset::vsubset( const Timestamp beginTimestamp,
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

    // Search for scans within the subset time range:

    const size_t totalPoints =
      sumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );
    const size_t scans = timestamps_.size();
    const size_t pointsPerCell = 4; // QUADRILATERAL.
    size_t pointOffset = 0;

    for ( size_t scan = 0; scan < scans; ++scan ) {
      const size_t scan2 = scan + scan;
      CHECK( scan < timestamps_.size() );
      CHECK( scan2 + 1 < scanDimensions_.size() );
      const Timestamp scanTimestamp = timestamps_[ scan ];
      const size_t scanPoints =
        scanDimensions_[ scan2 ] * scanDimensions_[ scan2 + 1 ];

      if ( scanTimestamp < beginHour ) {
        pointOffset += scanPoints;
      } else if ( IN_RANGE( scanTimestamp, beginHour, endHour ) ) {
        const int variable0 = variable == IMISSING ? dataVariable_ : variable;
        const size_t dataOffset = variable0 * totalPoints + pointOffset;
        const size_t coordinateOffset = pointOffset * pointsPerCell;
        CHECK4( dataOffset < data_.size(),
                IN_RANGE( dataOffset + scanPoints, 1, data_.size() ),
                coordinateOffset < coordinates_.size(),
                IN_RANGE( coordinateOffset + scanPoints * pointsPerCell, 1,
                          coordinates_.size() ) );
        const Dataset::SubsetPointers subsetPointers = {
          scanPoints * pointsPerCell,
          &coordinates_[ coordinateOffset ],
          scanPoints,
          &data_[ dataOffset ],
          0,
          0
        };
        result.push_back( subsetPointers ); //x
        pointOffset += scanPoints;
      } else {
        scan = scans; // Stop looping since beyond endHour.
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
         scanDimensions_
         coordinates_
         data_
NOTES:   Throws exception upon failure to read valid header/data from a stream.
******************************************************************************/

void CALIPSODataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read XDR ASCII header that looks like this:

  CALIPSO 1.0
  http://eosweb.larc.nasa.gov/PRODOCS/calipso/table_calipso.html,CALIPSOSubset
  2006-07-05T00:00:00-0000
  # Dimensions: variables timesteps profiles:
  6 24 4
  # Variable names:
  Profile_UTC_Time Longitude Latitude Elevation Feature_Optical_Depth_532 \
   Thickness
  # Variable units:
  yyyymmdd.f deg deg m - m
  # Domain: <min_lon> <min_lat> <max_lon> <max_lat>
  -110 35 -75 36
  # MSB 64-bit integers (yyyydddhhmm) profile_timestamps[profiles] and
  # IEEE-754 64-bit reals profile_bounds[profiles][2=<lon,lat>][2=<min,max>] \
   and
  # MSB 64-bit integers profile_dimensions[profiles][2=<points,levels>] and
  # IEEE-754 64-bit reals profile_data_1[variables][points_1][levels] ... \
   profile_data_S[variables][points_S][levels]:

   Note: Profile_UTC_Time Longitude Latitude are only for ground points -
   i.e., implicitly dimensioned with levels = 1. UGLY.
  */

  readAndMatchLine( stream, "CALIPSO 1.0\n" ); //x
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
  scanBounds_.resize( scans ); //x
  scanDimensions_.resize( scans * 2 ); //x
  readVariablesAndUnits( stream, tempVariables ); //x
  const bool hasThickness =
    ! strcmp( variableNames_[ variableNames_.size() - 1 ].c_str(),
              "Thickness" );

  // Remove time, longitude, latitude, elevation and thickness variables:

  if ( AND6( variables_ == (int) tempVariables,
            variables_ >= 5,
            variableNames_[ 0 ] == "Profile_UTC_Time",
            variableNames_[ 1 ] == "Longitude",
            variableNames_[ 2 ] == "Latitude",
            variableNames_[ 3 ] == "Elevation" ) ) {
    CHECK2( variableNames_.size() == tempVariables,
            variableUnits_.size() == tempVariables );

    // Remove Longitude, Latitude, Elevation and last one if Thickness:

    if ( hasThickness ) {
      variableNames_.erase( variableNames_.begin() + variableNames_.size() -1);
      variableUnits_.erase( variableUnits_.begin() + variableUnits_.size() -1);
     --variables_;
    }

    variableNames_.erase( variableNames_.begin() + 1,
                          variableNames_.begin() + 4 );
    variableUnits_.erase( variableUnits_.begin() + 1,
                          variableUnits_.begin() + 4 );

    variables_ -= 3;

    CHECK2( variables_ == (int) tempVariables - 3 - hasThickness,
            variables_ >= 2 );
    dataVariable_ = variables_ - 1;
  } else {
    throw std::runtime_error( "Read invalid CALIPSO variables/units." ); //x
  }

  setEndingTimestamp();
  setDatasetNameFromDescriptionAndVariable();
  skipLine( stream ); //x
  skipLine( stream ); //x
  readAndMatchLine( stream,
                    "# MSB 64-bit integers (yyyydddhhmm) "
                    "profile_timestamps[profiles] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "profile_bounds[profiles][2=<lon,lat>][2=<min,max>] "
                    "and\n");//x
  readAndMatchLine( stream, "# MSB 64-bit integers "
                    "profile_dimensions[profiles][2=<points,levels>] "
                    "and\n");//x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "profile_data_1[variables][points_1][levels] ... "
                    "profile_data_S[variables][points_S][levels]:\n" ); //x

  // Read binary data:

  readScanTimestamps( stream ); //x
  readBounds( stream, scanBounds_.size(), &scanBounds_[ 0 ] ); //x
  readIntegers( stream, 1LL, LLONG_MAX, scanDimensions_ ); //x
  readScanData( stream, hasThickness ); //x

  POST022( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ >= 1,
           dataVariable_ == variables_ - 1,
           timesteps_ >= 1,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           cellType_== QUADRILATERAL,
           hasElevation_ == true,
           hasArea_ == true,
           hasVolume_ == false,
           isStationary_ == false,
           isGroup_ == true,
           isStructuredGrid_ == false,
           timestamps_.size() >= 1,
           scanBounds_.size() == timestamps_.size(),
           scanDimensions_.size() == timestamps_.size() * 2,
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

void CALIPSODataset::readScanTimestamps( FILE* stream ) {
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
        throw std::runtime_error(
          "Failed to read valid CALIPSO scan timestamp." );  //x
      }
    }
  }

  POST02( timestamps_[ 0 ].invariant(),
          timestamps_[ timestamps_.size() - 1 ].invariant() );
}



/******************************************************************************
PURPOSE: readScanData - Read and convert scan data.
INPUTS:  FILE* stream  Stream to read from.
         const bool hasThickness  Is the last variable thickness?
OUTPUTS: coordinates_
         data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void CALIPSODataset::readScanData( FILE* stream, const bool hasThickness ) {
  PRE08( stream,
         variables_ >= 2,
         timestamps_.size() >= 1,
         coordinates_.size() == 0,
         data_.size() == 0,
         emvl::minimum( &scanDimensions_[ 0 ], scanDimensions_.size() ) >= 1,
         minimums_.size() == 0,
         maximums_.size() == 0 );

  /* Read XDR ASCII header that looks like this:
  # IEEE-754 64-bit reals profile_data_1[variables][points_1][levels] ... \
     profile_data_S[variables][points_S][levels]:

   Note: Profile_UTC_Time Longitude Latitude are only for ground points -
   i.e., implicitly dimensioned with levels = 1. UGLY.
  */

  const size_t scans = timestamps_.size();
  const size_t totalPoints =
    sumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );
  const size_t maximumPoints =
    maximumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );
  const size_t oneDVariables = 3; // Profile_UTC_Time Longitude Latitude.
  const size_t twoDVariables = 1 + ( variables_ - 1 ) + hasThickness;
  const size_t scanVariables = oneDVariables + twoDVariables;
  const size_t bufferSize = scanVariables * maximumPoints;
  std::vector<double> buffer( bufferSize ); //x
  data_.resize( variables_ * totalPoints ); //x
  const size_t pointsPerCell = 4; // QUADRILATERAL.
  coordinates_.reserve( totalPoints * pointsPerCell ); //x
  const double* const buffer0 = &buffer[ 0 ];
  bool ok = true;

  for ( size_t scan = 0, scanOffset = 0; AND2( ok, scan < scans ); ++scan ) {
    const size_t scan2 = scan + scan;
    const size_t scanGroundPoints = scanDimensions_[ scan2 ];
    const size_t scanVerticalPoints = scanDimensions_[ scan2 + 1 ];
    const size_t scanPoints = scanGroundPoints * scanVerticalPoints;
    const size_t readSize =
      ( oneDVariables * scanGroundPoints ) + ( twoDVariables * scanPoints );

    ok = fread( &buffer[ 0 ], sizeof buffer[0], readSize, stream ) == readSize;

    if ( ok ) {
      reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], readSize );
      updateMissingValues( &buffer[ 0 ], readSize );

      // Store Profile_UTC_Time as first data variable:

      for ( size_t groundPoint = 0;
            AND2( ok, groundPoint < scanGroundPoints );
            ++groundPoint ) {
        CHECK( groundPoint < buffer.size() );
        const double value = buffer0[ groundPoint ];
        const int yyyymmdd = (int) value;
        ok = Timestamp::isYYYYMMDD( yyyymmdd );

        if ( ok ) { // Replicate value for all vertical points (as a 2D var):
          const size_t offset = scanOffset + groundPoint * scanVerticalPoints;

          for ( size_t verticalPoint = 0;
                verticalPoint < scanVerticalPoints;
                ++verticalPoint ) {
            const size_t dataIndex = offset + verticalPoint;
            CHECK( dataIndex < data_.size() );
            data_[ dataIndex ] = value;
          }
        }
      }

      /*
       * Compute coordinates of vertically-oriented rectangle cells
       * from Longitude, Latitude, Elevation, Thickness:
       */

      if ( ok ) {
        const double* const longitudeCenters = buffer0 + scanGroundPoints;
        const double* const latitudeCenters =
          longitudeCenters + scanGroundPoints;
        const double* const elevationCenters =
          latitudeCenters + scanGroundPoints;
        const double* const thicknessCenters =
          hasThickness ? buffer0 + readSize - scanPoints : 0;

        computeVerticalRectangleCoordinates( scanGroundPoints,
                                             scanVerticalPoints,
                                             longitudeCenters,
                                             latitudeCenters,
                                             elevationCenters,
                                             thicknessCenters,
                                             coordinates_ ); //x

        // Store remaining data variables (except Thickness):

        const size_t scanVariables_1 = scanVariables - hasThickness;

        for ( size_t scanVariable = 4; // Skip Profile_UTC_Time, Lon, Lat, Elv
              AND2( ok, scanVariable < scanVariables_1 );
              ++scanVariable ) {
          const size_t offset =
            oneDVariables * scanGroundPoints + // Skip Time Longitude Latitude
            scanPoints +                       // Skip Elevation
           ( scanVariable - 4 ) * scanPoints;

          for ( size_t point = 0; AND2( ok, point < scanPoints ); ++point ) {
            const size_t index = offset + point;
            CHECK( index < buffer.size() );
            const double value = buffer0[ index ];
            ok = isFinite( value );

            if ( ok ) {
              const size_t dataIndex =
                ( scanVariable - 3 ) * totalPoints + scanOffset + point;
              CHECK( dataIndex < data_.size() );
              data_[ dataIndex ] = value;
            }
          }
        }
      }
    }

    scanOffset += scanPoints;
  }

  if ( ! AND5( ok,
               coordinates_.size() == totalPoints * pointsPerCell,
               data_.size() == variables_ * totalPoints,
               isValidCoordinates(),
               isValidData() ) ) {
    throw std::runtime_error( "Failed to read valid CALIPSO data." ); //x
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

void CALIPSODataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

  CALIPSO 1.0
  http://eosweb.larc.nasa.gov/PRODOCS/calipso/table_calipso.html,CALIPSOSubset
  2006-07-05T00:00:00-0000
  # Dimensions: variables timesteps profiles:
  6 24 4
  # Variable names:
  Profile_UTC_Time Longitude Latitude Elevation Feature_Optical_Depth_532 \
   Thickness
  # Variable units:
  yyyymmdd.f deg deg m - m
  # Domain: <min_lon> <min_lat> <max_lon> <max_lat>
  -110 35 -75 36
  # MSB 64-bit integers (yyyydddhhmm) profile_timestamps[profiles] and
  # IEEE-754 64-bit reals profile_bounds[profiles][2=<lon,lat>][2=<min,max>] \
   and
  # MSB 64-bit integers profile_dimensions[profiles][2=<points,levels>] and
  # IEEE-754 64-bit reals profile_data_1[variables][points_1][levels] ... \
   profile_data_S[variables][points_S][levels]:

   Note: Profile_UTC_Time Longitude Latitude are only for ground points -
   i.e., implicitly dimensioned with levels = 1. UGLY.
  */

  Timestamp::UTCString timestamp = "";
  startingTimestamp_.toUTCString( timestamp );
  const char* const format1 =
    "CALIPSO 1.0\n"
    "%s\n"
    "%s\n"
    "# Dimensions: variables timesteps profiles:\n"
    "%d %" LU "%" LU "\n";
  bool ok =
    fprintf( stream, format1,
             description_.c_str(),
             timestamp,
             3 + variables_ + 1, timesteps_, timestamps_.size() )
    > (int) strlen( format1 );

  if ( ok ) { // Write variable names:
    const char* const format2 =
      "# Variable names:\nProfile_UTC_Time Longitude Latitude Elevation";
    ok = fprintf( stream, format2 ) == (int) strlen( format2 );

    for ( int variable = 1; AND2( ok, variable < variables_ ); ++variable ) {
      const char* const name = variableNames_[ variable ].c_str();
      ok = fprintf( stream, " %s", name ) == (int) ( 1 + strlen( name ) );
    }

    const char* const extraVariableNames = " Thickness";

    ok = AND2( ok, fprintf( stream, "%s\n", extraVariableNames ) ==
                   (int) ( 1 + strlen( extraVariableNames ) ) );

    if ( ok ) { // Write variable units:
      const char* const format3 = "# Variable units:\nyyyymmdd.f deg deg m";
      ok = fprintf( stream, format3 ) == (int) strlen( format3 );

      for ( int variable = 1; AND2( ok, variable < variables_ ); ++variable ) {
        const char* const units = variableUnits_[ variable ].c_str();
        ok = fprintf( stream, " %s", units ) == (int) ( 1 + strlen( units ) );
      }

      const char* const extraVariableUnits = " m";

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
            "# MSB 64-bit integers "
            "(yyyydddhhmm) profile_timestamps[profiles] and\n"
            "# IEEE-754 64-bit reals "
            "profile_bounds[profiles][2=<lon,lat>][2=<min,max>] and\n"
            "# MSB 64-bit integers "
            "profile_dimensions[profiles][2=<points,levels>] and\n"
            "# IEEE-754 64-bit reals "
            "profile_data_1[variables][points_1][levels] ... "
            "profile_data_S[variables][points_S][levels]:\n";
          ok = fprintf( stream, "%s", end ) == (int) strlen( end );

          if ( ok ) {
            writeScanTimestamps( stream ); //x
            writeBounds( stream, scanBounds_.size(), &scanBounds_[ 0 ] ); //x
            writeIntegers( stream, 1LL, LLONG_MAX, scanDimensions_ ); //x
            writeScanData( stream ); //x
          }
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format CALIPSO data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeScanTimestamps - Write XDR format scan timestamps to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CALIPSODataset::writeScanTimestamps( FILE* stream ) const {
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
    throw std::runtime_error("Failed to write XDR format CALIPSO timestamps.");
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

void CALIPSODataset::writeScanData( FILE* stream ) const {
  PRE( stream );

  /* Write XDR data that looks like this:

  # IEEE-754 64-bit reals profile_data_1[variables][points_1][levels] ... \
   profile_data_S[variables][points_S][levels]:

   Note: Profile_UTC_Time Longitude Latitude are only for ground points -
   i.e., implicitly dimensioned with levels = 1. UGLY.
  */

  const size_t scans = timestamps_.size();
  const size_t maximumPoints =
    maximumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );
  const size_t scanVariables = 3 + variables_ + 1; // t,lon,lat, elv,ext, thick
  const size_t pointsPerCell = 4; // QUADRILATERAL.
  const size_t variableSize = data_.size() / variables_;
  std::vector<double> buffer( maximumPoints ); //x
  bool ok = true;

  for ( size_t scan = 0,
        timestampIndex = 0,
        longitudeIndex = 0, latitudeIndex = 0, elevationIndex = 0,
        thicknessIndex = 0;
        AND2( ok, scan < scans );
        ++scan ) {
    const size_t scan2 = scan + scan;
    const size_t scanGroundPoints = scanDimensions_[ scan2 ];
    const size_t scanVerticalPoints = scanDimensions_[ scan2 + 1 ];

    for ( size_t scanVariable = 0; AND2( ok, scanVariable < scanVariables );
          ++scanVariable ) {
      const size_t scanPoints =
        scanVariable < 3 ? scanGroundPoints
        : scanGroundPoints * scanVerticalPoints;

      for ( size_t point = 0; AND2( ok, point < scanPoints ); ++point ) {
        double value = 0.0;

        if ( scanVariable == 0 ) { // Profile_UTC_Time:
          CHECK( timestampIndex < data_.size() );
          value = data_[ timestampIndex ];
          CHECK( Timestamp::isYYYYMMDD( (int) value ) );
          timestampIndex += scanVerticalPoints;
        } else if ( scanVariable == 1 ) { // longitude of cell center:
          CHECK( longitudeIndex + 3 < coordinates_.size() );
          value = 0.25 * (
            coordinates_[ longitudeIndex     ].longitude +
            coordinates_[ longitudeIndex + 1 ].longitude +
            coordinates_[ longitudeIndex + 2 ].longitude +
            coordinates_[ longitudeIndex + 3 ].longitude );
          CHECK( isLongitude( value ) );
          longitudeIndex += pointsPerCell;
        } else if ( scanVariable == 2 ) { // latitude of cell center:
          CHECK( latitudeIndex + 3 < coordinates_.size() );
          value = 0.25 * (
            coordinates_[ latitudeIndex     ].latitude +
            coordinates_[ latitudeIndex + 1 ].latitude +
            coordinates_[ latitudeIndex + 2 ].latitude +
            coordinates_[ latitudeIndex + 3 ].latitude );
          CHECK( isLatitude( value ) );
          latitudeIndex += pointsPerCell;
        } else if ( scanVariable == 3 ) { // elevation of cell center:
          CHECK( elevationIndex + 3 < coordinates_.size() );
          value = 0.25 * (
            coordinates_[ elevationIndex     ].elevation +
            coordinates_[ elevationIndex + 1 ].elevation +
            coordinates_[ elevationIndex + 2 ].elevation +
            coordinates_[ elevationIndex + 3 ].elevation );
          CHECK( isElevation( value ) );
          elevationIndex += pointsPerCell;
        } else if ( scanVariable == scanVariables - 1 ) { // thickness of cell:
          CHECK( thicknessIndex + 3 < coordinates_.size() );
          value =
            coordinates_[ thicknessIndex + 3 ].elevation -
            coordinates_[ thicknessIndex     ].elevation;

          if ( value < 0.0 ) {
            value = 0.0;
          }

          CHECK( isElevation( value ) );
          thicknessIndex += pointsPerCell;
        } else { // Data:
          const size_t dataVariable = scanVariable - 4 + 1;
          const size_t dataIndex =
            timestampIndex - scanPoints + dataVariable * variableSize + point;
          CHECK( IN_RANGE( dataIndex, variableSize, data_.size() - 1 ) );
          value = data_[ dataIndex ];
          CHECK2( IN_RANGE( dataVariable, 1, (size_t) variables_ - 1 ),
                  IMPLIES( value != MISSING,
                           IN_RANGE( value,
                                     minimums_[ dataVariable ],
                                     maximums_[ dataVariable ] ) ) );
        }

        CHECK2( point < buffer.size(), point < scanPoints );
        buffer[ point ] = value;
      }

      reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], scanPoints );
      ok = fwrite( &buffer[ 0 ], sizeof buffer[ 0 ], scanPoints, stream )
           == scanPoints;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format CALIPSO data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CALIPSODataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  Longitude(deg)  Latitude(deg)   \
     Profile_UTC_Time(yyyymmdd.f)   Feature_Optical_Depth_532(-)  Thickness(m)
   2011-08-29T22:00:00-0000  -91.03617  31.00816   20060703.75   0.05632  60.0
   ...

  */

  // Write header row:

  const char* const format =
    "Timestamp(UTC)\tLongitude(deg)\tLatitude(deg)\tElevation(m)";
  bool ok = fprintf( stream, "%s", format ) == (int) strlen( format );
  const char* const headerFormat = "\t%s(%s)";

  for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
    ok = fprintf( stream, headerFormat,
                  variableNames_[ variable ].c_str(),
                  variableUnits_[ variable ].c_str() ) >= 5;
  }

  ok = AND2( ok, fprintf( stream, "\tThickness(m)" ) == 13 );

  if ( ok ) { // Also write corner coordinates header line:
    const char* const format_corners =
      "\tLongitude_SW(deg)\tLongitude_SE(deg)"
      "\tLongitude_NW(deg)\tLongitude_NE(deg)"
      "\tLatitude_SW(deg)\tLatitude_SE(deg)"
      "\tLatitude_NW(deg)\tLatitude_NE(deg)\n";
    ok = fprintf( stream, "%s", format_corners ) ==
         (int) strlen( format_corners );
  }

  // Write data rows:

  if ( ok ) {
    const char* const dataFormatF = "\t%11.5f";
    const char* const dataFormatE = "\t%11.5e";
    const size_t wordSize = 48;
    const size_t pointsPerCell = 4; // QUADRILATERAL.
    const size_t totalPoints =
      sumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );
    const size_t scans = timestamps_.size();
    const size_t lineSize = // Time,lon,lat,elv + vars + thick + corner-lon,lat
      ( 4 + variables_ + 1 + 4 + 4 ) * wordSize + 1;
    enum { BUFFER_LINES = 1000 };
    const size_t bufferSize = BUFFER_LINES * lineSize;
    std::vector<char> buffer( bufferSize ); //x
    size_t outputLines = 0;
    size_t cellIndex = 0;

    for ( size_t scan = 0; AND2( ok, scan < scans ); ++scan ) {
      const size_t scan2 = scan + scan;
      const size_t points =
        scanDimensions_[ scan2 ] * scanDimensions_[ scan2 + 1 ];
      const Timestamp timestamp = timestamps_[ scan ];
      Timestamp::UTCString utcString = "";
      timestamp.toUTCString( utcString );
      buffer.assign( buffer.size(), '\0' );
      char* outputBuffer = &buffer[ 0 ];

      for ( size_t point = 0; point < points; ++point, ++cellIndex ) {
        strncpy( outputBuffer, utcString, Timestamp::UTC_STRING_LENGTH );
        outputBuffer += Timestamp::UTC_STRING_LENGTH;

        const size_t coordinateIndex = cellIndex * pointsPerCell;
        CHECK( coordinateIndex < coordinates_.size() );
        const Point& pointSW = coordinates_[ coordinateIndex + SW ];
        const Point& pointSE = coordinates_[ coordinateIndex + SE ];
        const Point& pointNW = coordinates_[ coordinateIndex + NW ];
        const Point& pointNE = coordinates_[ coordinateIndex + NE ];
        const double longitude = 0.25 * (
          pointSW.longitude + pointSE.longitude +
          pointNW.longitude + pointNE.longitude );
        const double latitude = 0.25 * (
          pointSW.latitude + pointSE.latitude +
          pointNW.latitude + pointNE.latitude );
        const double elevation = 0.25 * (
          pointSW.elevation + pointSE.elevation +
          pointNW.elevation + pointNE.elevation );
        const double thickness = pointNW.elevation - pointSE.elevation;

        char word[ wordSize ] = "";
        size_t count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                                 longitude );
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
          const size_t dataIndex = variable * totalPoints + cellIndex;
          CHECK( dataIndex < data_.size() );
          count = snprintf( word, sizeof word / sizeof *word, dataFormatE,
                            data_[ dataIndex ] );
          CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count);
          strncpy( outputBuffer, word, count );
          outputBuffer += count;
        }

        count = snprintf( word, sizeof word / sizeof *word, dataFormatF,
                          thickness );
        CHECK2( count < sizeof word / sizeof *word, strlen( word ) == count );
        strncpy( outputBuffer, word, count );
        outputBuffer += count;

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
        ++outputLines;

        if ( outputLines == BUFFER_LINES ) { // Write buffer to stream:
          const size_t bufferLength = (size_t) ( outputBuffer - &buffer[ 0 ] );
          CHECK2( bufferLength <= bufferSize,
                  strlen( &buffer[ 0 ] ) < bufferSize );
          ok = fwrite( &buffer[ 0 ], sizeof (char), bufferLength, stream )
               == bufferLength;
          outputLines = 0;
          buffer.assign( buffer.size(), '\0' );
          outputBuffer = &buffer[ 0 ];
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to write ASCII format CALIPSO data."); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CALIPSODataset::writeCOARDS( const std::string& fileName ) const {
  PRE( fileName.length() );

  const size_t totalPoints =
    sumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );
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
    const char* const dimensionName = "points";
    int dimensionId = -1;
    const size_t dimension = totalPoints;

    if ( createNetCDFDimensions( file, 1, &dimensionName, &dimension,
                                 &dimensionId ) ) {

      // Create temporary copy of timestamps per point:

      std::vector<Timestamp> timestamps( totalPoints ); //x
      const size_t scans = timestamps_.size();

      for ( size_t scan = 0, index = 0; scan < scans; ++scan ) {
        const size_t scan2 = scan + scan;
        const size_t scanPoints =
          scanDimensions_[ scan2 ] * scanDimensions_[ scan2 + 1 ];
        const Timestamp scanTimestamp = timestamps_[ scan ];

        for ( size_t point = 0; point < scanPoints; ++point, ++index ) {
          timestamps[ index ] = scanTimestamp;
        }
      }

      writeCOARDSCommon( file, 0, false, true, true, true, 1,
                         1, &dimensionId, &dimension, &dimensionName,
                         timestamps ); //x
      ok = true;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create NetCDF-COARDS file for "
                              "CALIPSO data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CALIPSODataset::writeKML() const {
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
  const size_t vertexCount = 4; // QUADRILATERAL.
  const size_t totalPoints =
    sumProduct2( &scanDimensions_[ 0 ], scanDimensions_.size() );

  for ( size_t scan = 0, cellIndex = 0; scan < scans; ++scan ) {
    const size_t scan2 = scan + scan;
    const size_t points =
      scanDimensions_[ scan2 ] * scanDimensions_[ scan2 + 1 ];
    const Timestamp timestamp = timestamps_[ scan ];
    const long long yyyymmdd = timestamp.yyyymmdd();
    const long long hhmmss = timestamp.hhmmss();
    const long long valueTimestamp = yyyymmdd * 10000 + hhmmss / 100;

    for ( size_t point = 0; point < points; ++point, ++cellIndex ) {
      const size_t dataIndex = dataVariable_ * totalPoints + cellIndex;
      CHECK( dataIndex < data_.size() );
      const double value = data_[ dataIndex ];

      if ( value > MISSING ) {
        const size_t coordinateOffset = cellIndex * vertexCount;
        CHECK( coordinateOffset < coordinates_.size() );
        double quadCoordinates[ 4 * 3 ];

        for ( size_t vertex = 0, vertex3 = 0; vertex < vertexCount; ++vertex ) {
          const size_t coordinateIndex = coordinateOffset + vertex;
          CHECK2( coordinateIndex < coordinates_.size(),
                  vertex3 + 2 <
                    sizeof quadCoordinates / sizeof *quadCoordinates );
          quadCoordinates[ vertex3++ ] = coordinates_[ coordinateIndex ].longitude;
          quadCoordinates[ vertex3++ ] = coordinates_[ coordinateIndex ].latitude;
          quadCoordinates[ vertex3++ ] = coordinates_[ coordinateIndex ].elevation;
        }

        const Color color =
          dataColorFunction( value, variableMinimum, variableMaximum );
        writeCellDataToKML( outputKMLFile_, source.c_str(), variable, units,
                            valueTimestamp, 1,
                            MISSING, value, MISSING, MISSING,
                            4, quadCoordinates, color );
      }
    }
  }

  writeEndFolderKML( outputKMLFile_ );

  POST( true );
}



} // namespace emvl.



