/******************************************************************************
PURPOSE: GridDataset.C - Dataset of gridded model surface point scalar/vector
         data.
HISTORY: 2020-03-27 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fscanf(), fseek(), ftell().
#include <string.h> // For memset(), strstr().
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
#include <NetCDFUtilities.h>    // For *NetCDF*() routines.
#include <Dataset.h>            // For Dataset.
#include <GridDataset.h>        // For GridDataset.

namespace emvl {


//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~GridDataset - Destruct a GridDataset.
******************************************************************************/

GridDataset::~GridDataset() {
  rows_    = 0;
  columns_ = 0;
}



/******************************************************************************
PURPOSE: GridDataset - Construct a GridDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

GridDataset::GridDataset( FILE* stream ) {
  PRE0( stream );

  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = POINT;
  hasElevation_     = false;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = true;
  isGroup_          = false;
  isStructuredGrid_ = true;
  isVector_         = false;
  timestepSize_     = HOURS;
  timesteps_        = 0;
  rows_             = 0;
  columns_          = 0;
  readXDRHeader( stream ); //x
  readXDRData( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid Grid data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: GridDataset - Construct a GridDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

GridDataset::GridDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = POINT;
  hasElevation_     = false;
  hasArea_          = false;
  hasVolume_        = false;
  isStationary_     = true;
  isGroup_          = false;
  isStructuredGrid_ = true;
  isVector_         = false;
  timestepSize_     = HOURS;
  rows_             = 0;
  columns_          = 0;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDRHeader( file ); //x
    readXDRData( file ); //x
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid Grid data from file " +
                              fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: GridDataset - Copy-construct a GridDataset.
INPUTS:  const GridDataset& other  Object to copy.
******************************************************************************/

GridDataset::GridDataset( const GridDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    rows_       = other.rows_;
    columns_    = other.columns_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a GridDataset.
INPUTS:  const GridDataset& other  Object to copy.
RETURNS: GridDataset& *this.
******************************************************************************/

GridDataset& GridDataset::operator=( const GridDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    rows_       = other.rows_;
    columns_    = other.columns_;
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

Dataset* GridDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: GridDataset::sample()" ); //x
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

Dataset* GridDataset::regrid( const int regridMethod,
                              const CMAQGridParameters&
                                cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: GridDataset::regrid()" ); //x
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

void GridDataset::write( const std::string& directory,
                         const int format ) const {
  PRE2( directory.length(),
        IN_RANGE( format, 0, QueryParameters::FILE_FORMATS - 1 ) );

  const std::string fileName( pathedFileName( directory, format ) ); //x
  DEBUG( fprintf( stderr, "Writing file '%s'...\n", fileName.c_str() ); )

  if ( IN3( format, QueryParameters::XDR, QueryParameters::ASCII ) ) {
    const char* const name = fileName.c_str();
    ResourceHandler<FILE*> fileCloser( fopen( name, "wb" ), fclose );
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

bool GridDataset::invariant() const {
  const bool result =
    AND10( Dataset::invariant(),
           rows_ > 0,
           columns_ > 0,
           cellType_== POINT,
           hasElevation_     == false,
           hasArea_          == false,
           hasVolume_        == false,
           isStationary_     == true,
           isGroup_          == false,
           isStructuredGrid_ == true );

  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool GridDataset::operator==( const GridDataset& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND3( equal( other ),
               rows_    == other.rows_,
               columns_ == other.columns_  ) );

  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool GridDataset::operator!=( const GridDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: rows - Number of grid rows.
RETURNS: size_t Number of grid rows.
******************************************************************************/

size_t GridDataset::rows() const {
  const size_t result = rows_;
  POST0( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: columns - Number of grid columns.
RETURNS: size_t Number of grid columns.
******************************************************************************/

size_t GridDataset::columns() const {
  const size_t result = columns_;
  POST0( result >= 1 );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: timestepOfTimestamp - Determine timestep containing timestamp.
INPUTS:  const Timestamp& timestamp  Timestamp to check for.
OUTPUTS: size_t& timestep Timestep (0-based index) for timestamp, or 0.
RETURNS: true if in time range, else 0.
******************************************************************************/

bool GridDataset::timestepOfTimestamp( const Timestamp timestamp,
                                       size_t& timestep ) const {
  PRE( timestamp.invariant() );
  timestep = 0;
  const bool result = isInTimeRange( timestamp, timestep );
  POST( IMPLIES_ELSE( result, timestep < timesteps_, timestep == 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: timestepsOfTimestamps - Compute timestep range of timestamp range.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of subset.
         const Timestamp& endTimestamp    End timestamp of subset.
OUTPUTS: size_t& firstTimestep            First timestep of data for range.
         size_t& lastTimestep             Last  timestep of data for range.
RETURNS: size_t Number of subset timesteps or 0 if data is outside range.
******************************************************************************/

size_t GridDataset::timestepsOfTimestamps( const Timestamp beginTimestamp,
                                           const Timestamp endTimestamp,
                                           size_t& firstTimestep,
                                           size_t& lastTimestep ) const {
  PRE3( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp );

  size_t result = 0;
  const bool overlapped =
    overlappedTimeRange( beginTimestamp, endTimestamp,
                         firstTimestep, result );
  lastTimestep = overlapped ? firstTimestep + result - 1 : 0;

  POST( IMPLIES_ELSE( result,
                      AND3( IN_RANGE( result, 1, timesteps_ ),
                            firstTimestep < timesteps_,
                            IN_RANGE( lastTimestep,
                                      firstTimestep, timesteps_ - 1 ) ),
                      IS_ZERO2( firstTimestep, lastTimestep ) ) );
  return result;
}



/******************************************************************************
PURPOSE: probedPoint - Compute row, column of a probe.
INPUTS:  const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
OUTPTUS: size_t& column              Column index of probed cell.
         size_t& row                 Row    index of probed cell.
RETURNS: bool true if probe location is within the grid, else false.
******************************************************************************/

bool GridDataset::probedPoint( const double longitude,
                               const double latitude,
                               size_t& column,
                               size_t& row ) const {
  PRE( isLongitudeLatitude( longitude, latitude ) );

  const double nearEnough = 1e-2; // Probe close enough to data point.
  bool result = false;
  column = row = 0;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {
    const size_t points = coordinates_.size();
    size_t nearestPoint = 0;
    double nearestDistance = 1.0;

    for ( size_t point = 0; point < points; ++point ) {
      const double pointLongitude = coordinates_[ point ].longitude;
      const double longitudeDistance =
        pointLongitude <= longitude ?
          longitude - pointLongitude
        : pointLongitude - longitude;

      if ( longitudeDistance < nearestDistance ) {
        const double pointLatitude = coordinates_[ point ].latitude;
        const double latitudeDistance =
          pointLatitude <= latitude ?
            latitude - pointLatitude
          : pointLatitude - latitude;

        if ( latitudeDistance < nearestDistance ) {
          nearestDistance = longitudeDistance + latitudeDistance; // Manhatten.
          nearestPoint = point;
        }
      }
    }

    if ( nearestDistance <= nearEnough ) {
      const size_t nearestCell = nearestPoint / cellVertexCount();
      row    = nearestCell / columns_;
      column = nearestCell % columns_;
      CHECK2( column < columns_, row < rows_ );
      result = true;
    }
  }

  POST( IMPLIES_ELSE( result,
                      AND2( column < columns_, row < rows_ ),
                      IS_ZERO2( column, row ) ) );
  return result;
}



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

double GridDataset::vprobe( const Timestamp timestamp,
                            const double longitude,
                            const double latitude,
                            const double elevation,
                            const int probeLayer,
                            const int variable ) const {
  PRE5( timestamp.invariant(),
        isLongitudeLatitude( longitude, latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ),
        OR2( probeLayer == IMISSING,
             AND2( probeLayer >= 0, (size_t) probeLayer < layers() ) ),
        OR2( variable  == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  double result = MISSING;

  if ( OR2( elevation == MISSING, IN_RANGE( elevation, 0.0, 20.0 ) ) ) {
    size_t column = 0;
    size_t row = 0;

    if ( probedPoint( longitude, latitude, column, row ) ) {
      size_t timestep = 0;

      if ( timestepOfTimestamp( timestamp, timestep ) ) {
        const size_t points = rows_ * columns_;
        const size_t loadedTimesteps =
          dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
        const size_t loadedTimestep =
          dataFileOffset_ ? timestep % BUFFERED_DATA_TIMESTEPS : timestep;
        const size_t pointOffset =
          loadedTimestep * points + row * columns_ + column;

        if ( OR2( ! isVector_, variable != IMISSING ) ) {
          const int variable0 = variable == IMISSING ? dataVariable_ : variable;
          const size_t index =
            variable0 * loadedTimesteps * points + pointOffset;
          CHECK( index < data_.size() );
          result = data_[ index ];
        } else { // Compute vector magnitude:
          const size_t loadedTimestepsPoints = loadedTimesteps * points;
          bool atLeastOneValidValue = false;
          result = 0.0;

          for ( int component = 0; component < variables_; ++component ) {
            const size_t index =
              component * loadedTimestepsPoints + pointOffset;
            CHECK( index < data_.size() );
            const double value = data_[ index ];

            if ( value > MISSING ) {
              result += value * value;
              atLeastOneValidValue = true;
            }
          }

          if ( atLeastOneValidValue ) {
            result = sqrt( result );
          }
        }
      }
    }
  }

  POST( OR2( result == MISSING,
             IMPLIES( variable != IMISSING,
                      IN_RANGE( result,
                                minimum( variable ),
                                maximum( variable ) ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: vtimeseries - Probe dataset over time range at a specified location.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of sample.
         const Timestamp& endTimestamp    Ending timestamp of sample.
         const double longitude           Longitude of probe.
         const double latitude            Latitude of probe.
         const double elevation           Elevation of probe or MISSING.
         const int probeLayer             0-based index of variable or IMISSING.
OUTPUTS: std::vector<double>& result      Hourly values (or MISSING) over time
                                          range at location.
NOTES:   Called by base class member function timeseries().
         Template Method Design Pattern.
******************************************************************************/

void GridDataset::vtimeseries( const Timestamp beginTimestamp,
                               const Timestamp endTimestamp,
                               const double longitude,
                               const double latitude,
                               const double elevation,
                               const int probeLayer,
                               std::vector<double>& result ) const {

  PRE6( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        isLongitudeLatitude( longitude, latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ),
        OR2( probeLayer == IMISSING,
             AND2( probeLayer >= 0, (size_t) probeLayer < layers() ) ) );

  if ( OR2( elevation == MISSING, IN_RANGE( elevation, 0.0, 20.0 ) ) ) {
    size_t column = 0;
    size_t row = 0;

    if ( probedPoint( longitude, latitude, column, row ) ) {
      size_t firstTimestep = 0;
      size_t subsetTimesteps = 0;
      size_t skippedHours = 0;

      if ( overlappedTimeRange( beginTimestamp, endTimestamp,
                                firstTimestep, subsetTimesteps, skippedHours)) {
        const size_t points = rows_ * columns_;
        const size_t sampleHours = result.size();
        const int hoursPerTimestep =
          timestepSize_ == HOURS ? 1
          : timestepSize_ == DAYS ? 24
          : 0;
        const int monthsOffset =
          timestepSize_ == MONTHS ? 1
          : timestepSize_ == TRIMONTHS ? 3
          : timestepSize_ == YEARS ? 12
          : 0;
        Timestamp timestamp( startingTimestamp_ );

        if ( hoursPerTimestep ) {
          timestamp.offsetHours( firstTimestep * hoursPerTimestep );
        } else {
          timestamp.offsetMonths( firstTimestep * monthsOffset );
        }

        for ( size_t subsetTimestep = 0, hour = skippedHours;
              subsetTimestep < subsetTimesteps;
              ++subsetTimestep ) {

          if ( loadedDataSubset( timestamp, timestamp ) ) {
            const size_t timestep = subsetTimestep + firstTimestep;
            const size_t loadedTimesteps =
              dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
            const size_t loadedTimestep =
              dataFileOffset_ ? timestep % BUFFERED_DATA_TIMESTEPS : timestep;
            const size_t loadedTimestepsTimesPoints = loadedTimesteps * points;
            const size_t pointOffset =
              loadedTimestep * points + row * columns_ + column;
            double value = MISSING;

            if ( ! isVector_ ) {
              const size_t index =
                dataVariable_ * loadedTimestepsTimesPoints + pointOffset;
              CHECK( index < data_.size() );
              value = data_[ index ];
            } else { // Compute vector magnitude:

              for ( int component = 0; component < variables_; ++component ) {
                const size_t index =
                  component * loadedTimestepsTimesPoints + pointOffset;
                CHECK( index < data_.size() );
                const double componentValue = data_[ index ];

                if ( componentValue > MISSING ) {

                  if ( value < 0.0 ) {
                    value = 0.0;
                  }

                  value += componentValue * componentValue;
                }
              }

              if ( value > 0.0 ) {
                value = sqrt( value );
              }
            }

            // Replicate daily data for each hour:

            for ( size_t counter = hoursPerTimestep;
                  AND2( hour < sampleHours, counter );
                  --counter, ++hour ) {
              CHECK( hour < result.size() );
              result[ hour ] = value;
            }

            if ( hoursPerTimestep ) {
              timestamp.offsetHours( hoursPerTimestep );
            } else {
              timestamp.offsetMonths( monthsOffset );
            }
          }
        }
      }
    }
  }

  POST3( IMPLIES_ELSE( IN3( timestepSize(), HOURS, DAYS ),
                       result.size() >=
                         (size_t) 1 + beginTimestamp.hoursUntil( endTimestamp ),
                       result.size() >=
                         (size_t) 1 + beginTimestamp.monthsUntil(endTimestamp)),
         OR2( ::emvl::minimum( &result[ 0 ], result.size() ) == MISSING,
              IMPLIES( ! isVector(),
                       IN_RANGE( ::emvl::minimum( &result[ 0 ], result.size() ),
                                 minimum(), maximum() ) ) ),
         OR2( ::emvl::maximum( &result[ 0 ], result.size() ) == MISSING,
              IMPLIES( ! isVector(),
                       IN_RANGE( ::emvl::maximum( &result[ 0 ], result.size() ),
                                 ::emvl::minimum( &result[ 0 ], result.size() ),
                                 maximum() ) ) ) );
}



/******************************************************************************
PURPOSE: vsubset - Extract a reference to the subset of data for the given
         time-range for data with time-varying coordinates.
INPUTS:  const Timestamp& beginTimestamp  Beginning timestamp of subset.
         const Timestamp& endTimestamp    End timestamp of subset.
         const int variable               Index of data variable or
                                          IMISSING for vector.
RETURNS: std::vector<SubsetPointers>      Pointers to coordinate and data
                                          subset and count
                                          or empty if there is no data within
                                          the specified time range.
NOTES:   Called by public member function subsetData().
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
GridDataset::vsubset( const Timestamp beginTimestamp,
                      const Timestamp endTimestamp,
                      const int variable ) const {
  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<SubsetPointers> result;

  size_t firstTimestep = 0;
  size_t lastTimestep  = 0;
  const size_t subsetTimesteps =
    timestepsOfTimestamps( beginTimestamp, endTimestamp,
                           firstTimestep, lastTimestep );

  if ( subsetTimesteps ) {
    const size_t points = rows_ * columns_;
    const size_t loadedTimesteps =
      dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
    const size_t loadedFirstTimestep =
      dataFileOffset_ ? firstTimestep % BUFFERED_DATA_TIMESTEPS :firstTimestep;
    const size_t subsetCount = subsetTimesteps * points;
    const size_t loadedFirstTimestepPoints = loadedFirstTimestep * points;

    if ( OR2( ! isVector_, variable != IMISSING ) ) {
      const int variable0 = variable == IMISSING ? dataVariable_ : variable;
      const size_t firstIndex =
        variable0 * loadedTimesteps * points + loadedFirstTimestepPoints;
      CHECK( firstIndex < data_.size() );
      CHECK( IN_RANGE( subsetCount, 1, timesteps_ * points ) );
      const Dataset::SubsetPointers subsetPointers = {
        coordinates_.size(),
        &coordinates_[ 0 ],
        subsetCount,
        &data_[ firstIndex ],
        0,
        0
      };
      result.push_back( subsetPointers ); //x
    } else { // Append each vector component variable:

      for ( int component = 0; component < variables_; ++component ) {
        const size_t firstIndex =
          component * loadedTimesteps * points + loadedFirstTimestepPoints;
        CHECK( firstIndex < data_.size() );
        CHECK( IN_RANGE( subsetCount, 1, timesteps_ * points ) );
        const Dataset::SubsetPointers subsetPointers = {
          coordinates_.size(),
          &coordinates_[ 0 ],
          subsetCount,
          &data_[ firstIndex ],
          0,
          0
        };
        result.push_back( subsetPointers ); //x
      }
    }
  }

  POST( IMPLIES( result.size(),
                 AND17( result[ 0 ].dataCount,
                        result[ 0 ].coordinatesCount == coordinates_.size(),
                        result[ 0 ].coordinates == &coordinates_[ 0 ],
                        result[ 0 ].data,
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
                          result[ 0 ].coordinatesCount,
                        result[ result.size() - 1 ].coordinates ==
                          &coordinates_[ 0 ],
                        result[ result.size() - 1 ].data,
                        IMPLIES( AND2( variable != IMISSING,
                                  emvl::minimum(result[result.size() - 1].data,
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
                        result[ result.size() - 1 ].notes == 0,
                        IMPLIES( AND2( isVector(), variable == IMISSING ),
                                 result.size() == (size_t) variables() ) ) ) );
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
         rows_
         columns_
         startingTimestamp_
         endingTimestamp_
NOTES:   Throws exception upon failure to read valid header from a stream.
******************************************************************************/

void GridDataset::readXDRHeader( FILE* stream ) {
  PRE0( stream );

  /* Read XDR header that looks like this:

    Grid 1.0
    http://home.chpc.utah.edu/~u0553130/Brian_Blaylock/,HRRRSubset
    2020-02-17T00:00:00-0000
    # Dimensions: timesteps variables rows columns:
    24 2 1059 1799
    # Variable names:
    wind_u wind_v
    # Variable units:
    m/s m/s
    # IEEE-754 64-bit reals longitudes[rows][columns] and
    # IEEE-754 64-bit reals latitudes[rows][columns] and
    # IEEE-754 64-bit reals data[timesteps][variables][rows][columns]:

  */

  readAndMatchLine( stream, "Grid 1.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  size_t dimensions[ 4 ] = { 0, 0, 0, 0 };
  readDimensions( stream, sizeof dimensions / sizeof *dimensions,
                  dimensions ); //x
  timesteps_ = dimensions[ 0 ];
  const size_t tempVariables = dimensions[ 1 ];
  rows_      = dimensions[ 2 ];
  columns_   = dimensions[ 3 ];
  readVariablesAndUnits( stream, tempVariables ); //x
  CHECK( variables_ == (int) tempVariables );
  isVector_ = variables_ == 2;
  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.

  const char* const lines[] = {
    "# IEEE-754 64-bit reals longitudes[rows][columns] and\n",
    "# IEEE-754 64-bit reals latitudes[rows][columns] and\n",
    "# IEEE-754 64-bit reals data[timesteps][variables][rows][columns]:\n"
  };

  readAndMatchLine( stream, lines[ 0 ] );//x
  readAndMatchLine( stream, lines[ 1 ] );//x
  readAndMatchLine( stream, lines[ 2 ] );//x

  POST015( AND3( isValidName(),
                 isValidDescription(),
                 isValidVariableNamesAndUnits() ),
           variables_ >= 1,
           dataVariable_ == variables_ - 1,
           GT_ZERO3( timesteps_, rows_, columns_ ),
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           cellType_== POINT,
           hasElevation_ == false,
           hasArea_ == false,
           hasVolume_ == false,
           isStationary_ == true,
           isGroup_ == false,
           isStructuredGrid_ == true,
           timesteps_ > 0 );
}



/******************************************************************************
PURPOSE: loadedDataSubset - Determine if data is available for the given time
         range and, if available and not already in data_ then read it from
         fileName_ into data_.
INPUTS:  const Timestamp firstTimestamp  Timestamp of start of data needed.
         const Timestamp lastTimestamp   Timestamp of end   of data needed.
OUTPUTS: mutable firstDataTimestep_, lastDataTimestep_, data_.
RETURNS: bool true if data is available, else false if no data is available or
         it exceeds BUFFERED_DATA_TIMESTEPS or failed to read from fileName_.
NOTES:   This is called by Dataset::probe() and Dataset::subset().
         If a file open/seek/read failure occurs then a failure message is
         printed to stderr.
         If a partial read occurs then damaged values in data_ are zeroed.
******************************************************************************/

bool GridDataset::loadedDataSubset( const Timestamp firstTimestamp,
                                    const Timestamp lastTimestamp ) const {
  PRE( IMPLIES( dataFileOffset_, ! fileName_.empty() ) );
  bool result = dataFileOffset_ == 0;

  if ( ! result ) {
    size_t firstTimestep = 0;
    size_t lastTimestep = 0;
    const size_t subsetTimesteps =
      timestepsOfTimestamps( firstTimestamp, lastTimestamp,
                             firstTimestep, lastTimestep );

    if ( IN_RANGE( subsetTimesteps, 1, BUFFERED_DATA_TIMESTEPS ) ) {
      const bool outside = // Is loaded data outside the requested time-range?
        OR2( lastDataTimestep_  < firstTimestep,
             firstDataTimestep_ > lastTimestep );
      result = ! outside; // Nothing to do if requested data is already loaded.

      DEBUG( fprintf( stderr, "GridDataset::loadedDataSubset(): "
                     "firstDataTimestep_ = %" LU ", lastDataTimestep_ = %" LU ", "
                     "firstTimestep = %" LU ", lastTimestep = %" LU ", "
                     "outside = %d\n",
                     firstDataTimestep_, lastDataTimestep_,
                     firstTimestep, lastTimestep,
                     outside ); )

      if ( outside ) { // Must read XDR-format data from fileName_ into data_.

        /*
         * Read XDR file data that looks like this:
         * # IEEE-754 64-bit reals data[timesteps][variables][rows][columns]
         * into attribute:
         * std::vector<double> double data_ //[variable * time * rows * columns]
         */

        CHECK2( dataFileOffset_, ! fileName_.empty() );
        ResourceHandler<FILE*>
          fileCloser( fopen( fileName_.c_str(),"rb" ), fclose );
        FILE* const file = fileCloser.get();
        result = file != 0;

        if ( file ) {
          result = readData( file, firstTimestep, lastTimestep );
        }
      }
    }
  }

  POST0( isValidData() );
  return result;
}




/******************************************************************************
PURPOSE: readData - Read from file into data_ enough data to include the
         specified timestep range.
INPUTS:  FILE* file                  File to read from.
         const size_t firstTimestep  First timestep of data to read.
         const size_t lastTimestep   Last  timestep of data to read.
OUTPUTS: mutable firstDataTimestep_, lastDataTimestep_, data_.
RETURNS: bool true if successful, else false and a failure message is printed.
NOTES:   This is called by readXDRData() to load either all timesteps_
         or, if using buffering (dataFileOffst_ > 0), the first
         BUFFERED_DATA_TIMESTEPS.
         If using buffering, it is called by loadedDataSubset() to read
         not more than BUFFERED_DATA_TIMESTEPS.
         If a file open/seek/read failure occurs then a failure message is
         printed to stderr.
         If a partial read occurs then damaged values in data_ are zeroed.
******************************************************************************/

bool GridDataset::readData( FILE* file,
                            size_t firstTimestep,
                            size_t lastTimestep ) const {

  PRE04( file,
         firstTimestep <= lastTimestep,
         IMPLIES_ELSE( dataFileOffset_,
                       ( 1 + lastTimestep - firstTimestep )
                         <= BUFFERED_DATA_TIMESTEPS,
                       ( 1 + lastTimestep - firstTimestep ) <= timesteps_ ),
         data_.size() >=
           variables_ * (1 + lastTimestep - firstTimestep) * rows_ * columns_);

  const size_t points = rows_ * columns_;
  const size_t variablesTimesPoints = variables_ * points;
  bool ok = true;

  DEBUG( fprintf( stderr, "readData( %" LU ", %" LU " ) dataFileOffset_ = %" LU "\n",
                  firstTimestep, lastTimestep, dataFileOffset_ ); )

  if ( dataFileOffset_ ) {

    // Adjust first/lastTimestep to BUFFERED_DATA_TIMESTEPS covering range:

    size_t firstTimestep0 =
      ( firstTimestep / BUFFERED_DATA_TIMESTEPS ) * BUFFERED_DATA_TIMESTEPS;
    CHECK( firstTimestep0 % BUFFERED_DATA_TIMESTEPS == 0 );
    size_t lastTimestep0 = firstTimestep0 + BUFFERED_DATA_TIMESTEPS - 1;

    if ( AND2( IN_RANGE( firstTimestep, firstTimestep0, lastTimestep0 ),
               IN_RANGE( lastTimestep,  firstTimestep0, lastTimestep0 ) ) ) {
      firstTimestep = firstTimestep0;
      lastTimestep  = lastTimestep0;
    } else {
      const size_t count = 1 + lastTimestep0 - firstTimestep0;
      CHECK( count <= BUFFERED_DATA_TIMESTEPS );
      const size_t extra = BUFFERED_DATA_TIMESTEPS - count;
      lastTimestep = lastTimestep0 + extra;
    }

    CHECK3( firstTimestep <= lastTimestep,
            1 + lastTimestep - firstTimestep == BUFFERED_DATA_TIMESTEPS,
            lastTimestep < data_.size() );

    const size_t timestepOffset = firstTimestep * variablesTimesPoints;
    const size_t seekOffset =
      dataFileOffset_ + timestepOffset * sizeof (double);
    ok = FSEEK( file, seekOffset, SEEK_SET ) == 0;
    DEBUG( fprintf( stderr, "reading from file at offset = %" LU ".\n",
                    seekOffset ); )
  }

  const size_t readTimesteps = 1 + lastTimestep - firstTimestep;
  size_t damagedDoubles = 0;
  bool result = false;

  DEBUG( fprintf( stderr, "adjusted first/LastTimestep = %" LU " %" LU "\n",
                  firstTimestep, lastTimestep ); )

  /*
   * Read XDR file data that looks like this:
   * # IEEE-754 64-bit reals data[timesteps][variables][rows][columns]
   * into attribute:
   * std::vector<double> double data_ //[variable * time * rows * columns]
   */

  if ( ok ) {
    const size_t readTimestepsTimesPoints = readTimesteps * points;

    for ( size_t timestep = firstTimestep;
          AND2( ok, timestep <= lastTimestep );
          ++timestep ) {
      const size_t timestepOffset = ( timestep - firstTimestep ) * points;

      for ( int variable = 0; AND2( ok, variable < variables_ );
            ++variable ) {
        const size_t dataOffset =
          variable * readTimestepsTimesPoints + timestepOffset;
        CHECK( dataOffset <= data_.size() - points );
        double* const data = &data_[ dataOffset ];
        const size_t readWords = points;
        CHECK( data <= &data_[ data_.size() - readWords ] );
        const size_t wordsRead =
          fread( data, sizeof (double), readWords, file );
        ok = wordsRead == readWords;

        /*
         * If a failed read occurs with partial double words read into
         * data then zero-out the double words damaged by the bad
         * partial read:
         */

        if ( AND2( wordsRead, wordsRead < readWords ) ) { // Partial
          damagedDoubles = wordsRead;
          memset( data, 0, damagedDoubles * sizeof (double) );
        }

        if ( ok ) {
          reverse8ByteWordsIfLittleEndian( data, readWords );
          DEBUG( fprintf( stderr, "read %" LU " values.\n", readWords ); )
          updateMissingValues( data, readWords );
        }
      } // For variable.
    } // For timestep.

    ok = AND2( ok, isValidData() );
  } // If file.

  if ( ! ok ) {
    fprintf( stderr,
             "\nI'm sorry, Failed to open, seek and "
             "read valid Grid data from file %s "
             "for timesteps [%" LU ", %" LU "]",
             fileName_.c_str(), firstTimestep, lastTimestep );

    if ( damagedDoubles ) {
      fprintf( stderr,
               " (and %" LU " values were zeroed due to partial file read)",
               damagedDoubles );
    }

    fprintf( stderr, ".\n" );
  } else {

    if ( dataFileOffset_ ) {
      firstDataTimestep_ = firstTimestep;
      lastDataTimestep_  = firstDataTimestep_ + readTimesteps - 1;
    }

    result = true;
  }

  POST0( isValidData() );
  return result;
}



/******************************************************************************
PURPOSE: readXDRData - Read XDR data from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: coordinates_, data_,
         dataFileOffset_, firstDataTimestep_, lastDataTimestep_.
NOTES:   Throws exception upon failure to read valid data from stream.
******************************************************************************/

void GridDataset::readXDRData( FILE* stream ) {

  PRE016( stream,
          AND3( isValidName(),
                isValidDescription(),
                isValidVariableNamesAndUnits() ),
          variables_ >= 1,
          dataVariable_ == variables_ - 1,
          GT_ZERO3( timesteps_, rows_, columns_ ),
          startingTimestamp_.invariant(),
          endingTimestamp_.invariant(),
          startingTimestamp_ < endingTimestamp_,
          cellType_== POINT,
          hasElevation_ == false,
          hasArea_ == false,
          hasVolume_ == false,
          isStationary_ == true,
          isGroup_ == false,
          isStructuredGrid_ == true ,
          IS_ZERO3( dataFileOffset_, firstDataTimestep_, lastDataTimestep_ ) );

  /*
   * Read file data that looks like this:
   * # IEEE-754 64-bit reals longitudes[rows][columns] and
   * # IEEE-754 64-bit reals latitudes[rows][columns] and
   * # IEEE-754 64-bit reals data[timesteps][variables][rows][columns]:
   * into attributes:
   * std::vector<Point> coordinates_; // [rows * columns].
   * std::vector<double> double data_ // [variable * time * rows * columns]
   */

  // Read coordinates:

  const size_t points = rows_ * columns_;
  const size_t points2 = points + points;
  coordinates_.resize( points ); //x
  data_.resize( points2 ); //x. Use data_ as a temporary buffer for lons,lats.
  double* const data = &data_[ 0 ];
  bool ok = fread( data, sizeof (double), points2, stream ) == points2;

  if ( ok ) {
    reverse8ByteWordsIfLittleEndian( data, points2 );
    const double* const longitudes = data;
    const double* const latitudes  = longitudes + points;

    // Set elevation (in meters above mean sea level) based on variable name:
    // https://www.weather.gov/epz/wxcalc_pressurealtitude

    const char* const firstVariableName = variableNames_[ 0 ].c_str();
    const double elevation =
        strstr( firstVariableName, "_1000mb" ) ? 110.8
      : strstr( firstVariableName, "_925mb" ) ? 761.7
      : strstr( firstVariableName, "_850mb" ) ? 1456.7
      : strstr( firstVariableName, "_700mb" ) ? 3010.9
      : strstr( firstVariableName, "_500mb" ) ? 5572.1
      : strstr( firstVariableName, "_300mb" ) ? 9160.1
      : strstr( firstVariableName, "_255mb" ) ? 10358.5
      : strstr( firstVariableName, "_250mb" ) ? 10230.4
      : strstr( firstVariableName, "_180mb" ) ? 12415.7
      : strstr( firstVariableName, "_2m"    ) ? 2.0
      : strstr( firstVariableName, "_10m" ) ? 10.0
      : strstr( firstVariableName, "_80m" ) ? 80.0
      : strstr( firstVariableName, "_1000m" ) ? 1000.0
      : strstr( firstVariableName, "_6000m" ) ? 6000.0
      : strstr( firstVariableName, "_4000m" ) ? 6000.0
      : strstr( firstVariableName, "_1000_0m" ) ? 1000.0
      : strstr( firstVariableName, "_3000_0m" ) ? 3000.0
      : 0.0;

    for ( size_t point = 0; AND2( ok, point < points ); ++point ) {
      const double longitude = longitudes[ point ];
      const double latitude  = latitudes[  point ];
      ok = isLongitudeLatitude( longitude, latitude );

      if ( ! ok ) {
        fprintf( stderr, "\nRead invalid coordinate: (%g, %g).\n",
                 longitude, latitude );
        point = points;
      } else {
        coordinates_[ point ].longitude = longitude;
        coordinates_[ point ].latitude  = latitude;
        coordinates_[ point ].elevation = elevation;
      }
    }

    if ( ok ) {

      // Reallocate data_ and decide whether or not to use buffering:

      const size_t count0 = variables_ * timesteps_ * points;
      const size_t dataSize = count0 * sizeof (double);
      const bool useBuffering =
        AND4( ! fileName_.empty(),
              timesteps_ > BUFFERED_DATA_TIMESTEPS,
              timesteps_ % BUFFERED_DATA_TIMESTEPS == 0,
              dataSize > PAGE_THREHOLD_GB );
      const size_t count =
        useBuffering ? variables_ * BUFFERED_DATA_TIMESTEPS * points : count0;
      data_.resize( count ); //x

      DEBUG( fprintf( stderr,
                      "GridDataset::readXDR(): useBuffering = %d, "
                      "variables_ = %d, rows_ = %" LU ", columns_ = %" LU ", "
                      "points = %" LU ", count = %" LU "\n",
                      useBuffering, variables_, rows_, columns_, points, count);)

      size_t lastTimestep = timesteps_ - 1;

      if ( useBuffering ) {
        CHECK( ! fileName_.empty() );
        lastTimestep = BUFFERED_DATA_TIMESTEPS - 1;
        dataFileOffset_ = FTELL( stream );
        ok = dataFileOffset_ > 0;
      }

      if ( ok ) {
        ok = readData( stream, 0, lastTimestep );

        if ( ok ) {

          if ( allFinite( &data_[ 0 ], data_.size() ) ) {
            computeVariableRanges(); //x Only computes range of loaded data.

            if ( dataFileOffset_ ) { // Compute range over rest of timesteps:
              std::vector<double> minimums( minimums_ ); //x
              std::vector<double> maximums( maximums_ ); //x
              const int hoursPerTimestep =
                timestepSize_ == HOURS ? 1
                : timestepSize_ == DAYS ? 24
                : 0;
              const int monthsPerTimestep =
                timestepSize_ == MONTHS ? 1
                : timestepSize_ == TRIMONTHS ? 3
                : timestepSize_ == YEARS ? 12
                : 0;
              Timestamp timestamp( startingTimestamp_ );

              for ( size_t timestep = 0;
                   AND2( ok, timestep < timesteps_ );
                   ++timestep ) {
                ok = loadedDataSubset( timestamp, timestamp );

                if ( ok ) {
                  computeVariableRanges(); //x Computes range of loaded data.

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

                if ( hoursPerTimestep ) {
                  timestamp.offsetHours( hoursPerTimestep );
                } else {
                  timestamp.offsetMonths( monthsPerTimestep );
                }
              }

              for ( int variable = 0; variable < variables_; ++variable ) {
                minimums_[ variable ] = minimums[ variable ];
                maximums_[ variable ] = maximums[ variable ];
              }

              ok = loadedDataSubset( startingTimestamp_, startingTimestamp_ );
            }
          }
        }
      }
    }
  }

  if ( ! AND3( ok, isValidCoordinates(), isValidData() ) ) {
    throw std::runtime_error( "Failed to read valid Grid data." ); //x
  }

  computeBounds();

  POST08( rows_ > 0,
          columns_ > 0,
          cellType_ == POINT,
          coordinates_.size() == rows_ * columns_,
          data_.size() ==
            ( dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_ )
            * variables_ * rows_ *columns_,
          isValidCoordinates(),
          isValidData(),
          isValidRange() );
}



/******************************************************************************
PURPOSE: writeXDR - Write XDR format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void GridDataset::writeXDR( FILE* stream ) const {
  PRE( stream );
  bool ok = false;

  if ( ! fileName_.empty() ) { // If XDR file exists then simply stream it:
    ResourceHandler<FILE*>
      fileCloser( fopen( fileName_.c_str(),"rb" ), fclose );
    FILE* const file = fileCloser.get();
    ok = file != 0;

    if ( file ) {
      ok = copyStream( file, stream );
    }
  } else {

    /* Write XDR header that looks like this:

      Grid 1.0
      http://home.chpc.utah.edu/~u0553130/Brian_Blaylock/,HRRRSubset
      2020-02-17T00:00:00-0000
      # Dimensions: timesteps variables rows columns:
      24 2 1059 1799
      # Variable names:
      wind_u wind_v
      # Variable units:
      m/s m/s
      # IEEE-754 64-bit reals longitudes[rows][columns] and
      # IEEE-754 64-bit reals latitudes[rows][columns] and
      # IEEE-754 64-bit reals data[timesteps][variables][rows][columns]:

    */

    Timestamp::UTCString timestampString = "";
    startingTimestamp_.toUTCString( timestampString );
    const char* const format1 =
      "Grid 1.0\n"
      "%s\n"
      "%s\n"
      "# Dimensions: timesteps variables rows columns:\n"
      "%" LU " %d %" LU " %" LU "\n";
    ok = fprintf( stream, format1,
                  description_.c_str(),
                  timestampString,
                  timesteps_, variables_, rows_, columns_ )
         > (int) strlen( format1 );

    if ( ok ) { // Write variable names:
      const char* const format2 =
        "# Variable names:\n";
      ok = fprintf( stream, format2 ) == (int) strlen( format2 );

      for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
        const char delimiter = variable + 1 < variables_ ? ' ' : '\n';
        const char* const name = variableNames_[ variable ].c_str();
        ok = fprintf( stream, "%s%c", name, delimiter ) ==
             (int) ( strlen( name ) + 1 );
      }

      if ( ok ) { // Write variable units:
        const char* const format3 = "# Variable units:\n";
        ok = fprintf( stream, format3 ) == (int) strlen( format3 );

        for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable) {
          const char delimiter = variable + 1 < variables_ ? ' ' : '\n';
          const char* const units = variableUnits_[ variable ].c_str();
          ok = fprintf( stream, "%s%c", units, delimiter ) ==
               (int) ( strlen( units ) + 1 );
        }

        if ( ok ) {
          const char* const end =
            "# IEEE-754 64-bit reals longitudes[rows][columns] and\n"
            "# IEEE-754 64-bit reals latitudes[rows][columns] and\n"
            "# IEEE-754 64-bit reals "
            "data[timesteps][variables][rows][columns]:\n";
          ok = fprintf( stream, "%s", end ) == (int) strlen( end );

          if ( ok ) {
            const size_t points = rows_ * columns_;
            std::vector<double> buffer( points ); //x
            writeCoordinates( stream, &buffer[ 0 ] ); //x

            // Copy & write variables from data_[variable * time * points]:

            const size_t dataTimesteps =
              1 + lastDataTimestep_ - firstDataTimestep_;
            const size_t variableSize = dataTimesteps * points;
            const int hoursPerTimestep =
              timestepSize_ == HOURS ? 1
              : timestepSize_ == DAYS ? 24
              : 0;
            const int monthsPerTimestep =
              timestepSize_ == MONTHS ? 1
              : timestepSize_ == TRIMONTHS ? 3
              : timestepSize_ == YEARS ? 12
              : 0;
            Timestamp timestamp( startingTimestamp_ );

            for ( size_t timestep = 0;
                  AND2( ok, timestep < timesteps_ );
                  ++timestep ) {
              ok = loadedDataSubset( timestamp, timestamp );

              for ( int variable = 0;
                    AND2( ok, variable < variables_ );
                    ++variable ) {
                const size_t variableOffset = variable * variableSize;

                for ( size_t point = 0; point < points; ++point ) {
                  CHECK( variableOffset + point < data_.size() );
                  buffer[ point ] = data_[ variableOffset + point ];
                }

                reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], points );
                ok =
                  fwrite( &buffer[ 0 ], sizeof buffer[ 0 ], points, stream )
                  == points;
              }

              if ( hoursPerTimestep ) {
                timestamp.offsetHours( hoursPerTimestep );
              } else {
                timestamp.offsetMonths( monthsPerTimestep );
              }
            }
          }
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR format Grid data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void GridDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  Longitude(deg)  Latitude(deg)   wind_u(m/s)   wind_v(m/s)
   2020-02-17T00:00:00-0000  -9.103617095947265625e+01  \
   3.100816154479980469e+01  \
     4.548812285065650940e-05  4.548812285065650940e-05
   ...

  */

  // Write header row:

  const char* const format =
    "Timestamp(UTC)\tLongitude(deg)\tLatitude(deg)";
  bool ok = fprintf( stream, "%s", format ) == (int) strlen( format );
  const char* const headerFormat = "\t%s(%s)";

  for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
    ok = fprintf( stream, headerFormat,
                  variableNames_[ variable ].c_str(),
                  variableUnits_[ variable ].c_str() ) > 4;
  }

  ok = AND2( ok, fprintf( stream, "\n" ) == 1 );

  if ( ok ) { // Write data[V][T][R][C] as data[T][R][C][V]:
    const char* const dataFormat = "\t%28.18le";
    const size_t wordSize = 48;
    char word[ wordSize ] = "";
    const size_t points  = rows_ * columns_;
    const size_t bufferSize = points * (3 + variables_) * wordSize + 1;
    std::vector<char> buffer( bufferSize + 1 ); //x
    const size_t dataTimesteps =
      dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
    const size_t variableSize = dataTimesteps * points;
    const int hoursPerTimestep =
      timestepSize_ == HOURS ? 1
      : timestepSize_ == DAYS ? 24
      : 0;
    const int monthsPerTimestep =
      timestepSize_ == MONTHS ? 1
      : timestepSize_ == TRIMONTHS ? 3
      : timestepSize_ == YEARS ? 12
      : 0;
    Timestamp timestamp( startingTimestamp_ );

    for ( size_t timestep = 0;
          AND2( ok, timestep < timesteps_ );
          ++timestep ) {
      ok = loadedDataSubset( timestamp, timestamp );

      if ( ok ) {
        const size_t timestepOffset =
          ( timestep - firstDataTimestep_ ) * points;
        Timestamp::UTCString utcString = "";
        timestamp.toUTCString( utcString );

        // Write buffer worth of data for performance:

        buffer.assign( buffer.size(), '\0' );
        char* outputBuffer = &buffer[ 0 ];

        for ( size_t point = 0; point < points; ++point ) {

          /// Write timestamp to buffer:

          strncpy( outputBuffer, utcString, Timestamp::UTC_STRING_LENGTH );
          outputBuffer += Timestamp::UTC_STRING_LENGTH;

          // Write longitude to buffer:

          const double longitude = coordinates_[ point ].longitude;
          size_t count =
            snprintf( word, sizeof word / sizeof *word, dataFormat, longitude);
          CHECK2( count < sizeof word / sizeof *word, strlen(word) == count);
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          // Write latitude to buffer:

          const double latitude = coordinates_[ point ].latitude;
          count =
            snprintf( word, sizeof word / sizeof *word, dataFormat, latitude );
          CHECK2( count < sizeof word / sizeof *word, strlen(word) == count);
          strncpy( outputBuffer, word, count );
          outputBuffer += count;

          const size_t pointOffset = timestepOffset + point;

          // Write each variable to buffer:

          for ( int variable = 0; variable < variables_; ++variable ) {
            const size_t dataIndex = variable * variableSize + pointOffset;
            CHECK( dataIndex < data_.size() );
            count = snprintf( word, sizeof word / sizeof *word, dataFormat,
                              data_[ dataIndex ]);
            CHECK2( count < sizeof word / sizeof *word, strlen(word) == count );
            strncpy( outputBuffer, word, count );
            outputBuffer += count;
          }

          strcpy( outputBuffer, "\n" ); // End of spreadsheet row.
          ++outputBuffer;
        }

        // Write buffered output to stream:

        CHECK( strlen( &buffer[ 0 ] ) < bufferSize );
        const size_t bufferLength = (size_t) ( outputBuffer - &buffer[ 0 ] );
        ok = fwrite( &buffer[ 0 ], sizeof (char), bufferLength, stream ) ==
             bufferLength;
      }

      if ( hoursPerTimestep ) {
        timestamp.offsetHours( hoursPerTimestep );
      } else {
        timestamp.offsetMonths( monthsPerTimestep );
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write ASCII format Grid data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void GridDataset::writeCOARDS( const std::string& fileName ) const {
  PRE( fileName.length() );
  const size_t fileSizeEstimate =
    3 * rows_ * columns_ * 4 +
    timesteps_ * variables_ * rows_ * columns_ * 4 +
    timesteps_ * 3 * 4 + 10000;
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    enum { TIME, Y, X, DIMENSIONS };
    const char* const names[ DIMENSIONS ] = { "time", "latitude", "longitude" };
    int ids[ DIMENSIONS ] = { -1, -1, -1 };
    const size_t sizes[ DIMENSIONS ] = { timesteps_, rows_, columns_ };

    if ( createNetCDFDimensions( file, DIMENSIONS, names, sizes, ids ) ) {
      std::vector<Timestamp> timestamps( timesteps_ ); //x
      Timestamp timestamp = startingTimestamp_;
      const int hoursPerTimestep =
        timestepSize_ == HOURS ? 1 : timestepSize_ == DAYS ? 24 : 0;
      const int monthsPerTimestep =
        timestepSize_ == MONTHS ? 1
        : timestepSize_ == TRIMONTHS ? 3
        : timestepSize_ == YEARS ? 12
        : 0;

      for ( size_t timestep = 0; timestep < timesteps_; ++timestep ) {
        timestamps[ timestep ] = timestamp;

        if ( hoursPerTimestep ) {
          timestamp.offsetHours( hoursPerTimestep );
        } else {
          timestamp.offsetMonths( monthsPerTimestep );
        }
      }

      writeCOARDSCommon( file, 0, false, false, false, true, 2,
                         DIMENSIONS, ids, sizes, names, timestamps ); //x
      ok = true;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create "
                              "NetCDF-COARDS header for Grid data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void GridDataset::writeKML() const {
  PRE( outputKMLFile_ );
  const char* const variable =
    isVector_ ? "Wind" : variableNames_[ dataVariable_ ].c_str();
  std::string source( name_ ); //x
  const size_t variableIndex = source.rfind( variable );

  if ( variableIndex != std::string::npos && variableIndex > 1 ) {
    source.erase( variableIndex - 1 );
  }

  const char* const units = variableUnits_[ dataVariable_ ].c_str();
  const char* const dataType = isVector_ ? "Vector" : "Point";
  const char* const description =
    isVector_ ? "Modeled gridded vector data."
    : "Modeled gridded scalar data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;

  writeStartFolderKML( outputKMLFile_, source.c_str(), variable, units,
                       dataType, description );

  const double variableMinimum = minimum();
  const double variableMaximum = maximum();
  const size_t points = rows_ * columns_;
  Timestamp timestamp( startingTimestamp_ );
  const size_t loadedTimesteps =
    dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
  const int hoursPerTimestep =
    timestepSize_ == HOURS ? 1 : timestepSize_ == DAYS ? 24 : 0;
  const int monthsPerTimestep =
    timestepSize_ == MONTHS ? 1
    : timestepSize_ == TRIMONTHS ? 3
    : timestepSize_ == YEARS ? 12
    : 0;
  bool ok = true;

  for ( size_t timestep = 0;
        AND2( ok, timestep < timesteps_ );
        ++timestep ) {
    ok = loadedDataSubset( timestamp, timestamp );

    if ( ok ) {
      const int yyyymmdd = timestamp.yyyymmdd();
      const int hhmmss = timestamp.hhmmss();
      const int valueTimestamp =
        timestepSize_ == HOURS ? yyyymmdd * 100 + hhmmss / 10000
        : timestepSize_ == DAYS ? yyyymmdd
        : timestepSize_ == MONTHS ? yyyymmdd / 100
        : timestepSize_ == TRIMONTHS ? yyyymmdd / 100
        : yyyymmdd / 10000;
      const size_t loadedTimestep =
        dataFileOffset_ ? timestep % BUFFERED_DATA_TIMESTEPS : timestep;
      const size_t loadedTimestepsTimesPoints = loadedTimesteps * points;
      const size_t timestepOffset = loadedTimestep * points;

      for ( size_t point = 0; point < points; ++point ) {
        double value  = MISSING;
        double value2 = MISSING;
        double magnitude = MISSING;

        if ( isVector_ ) {
          const size_t index = timestepOffset + point;
          const size_t index2 = loadedTimestepsTimesPoints + index;
          CHECK3( index < data_.size(),
                  index2 > index,
                  index2 < data_.size() );
          value  = data_[ index  ];
          value2 = data_[ index2 ];

          if ( AND2( value > MISSING, value2 > MISSING ) ) {
            magnitude = sqrt( value * value + value2 * value2 );
          }
        } else {
          const size_t index = timestepOffset + point;
          CHECK( index < data_.size() );
          value = data_[ index ];

          if ( value > MISSING ) {
            magnitude = value;
          }
        }

        if ( magnitude > MISSING ) {
          const Color color =
            dataColorFunction( magnitude, variableMinimum, variableMaximum );
          const size_t vertexCount = 1;
          const double longitude = coordinates_[ point ].longitude;
          const double latitude  = coordinates_[ point ].latitude;
          const double cellCoordinates[ 3 ] = { longitude, latitude, 10.0 };
          const int components = isVector_ ? variables_ : 1;
          const double value3 = MISSING;
          writeCellDataToKML( outputKMLFile_, source.c_str(), variable, units,
                              valueTimestamp, components,
                              MISSING, value, value2, value3,
                              vertexCount, cellCoordinates, color );
        }
      }
    }

    if ( hoursPerTimestep ) {
      timestamp.offsetHours( hoursPerTimestep );
    } else {
      timestamp.offsetMonths( monthsPerTimestep );
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write Grid data to KML." ); //x
  } else {
    writeEndFolderKML( outputKMLFile_ );
  }

  POST( true );
}



} // namespace emvl.



