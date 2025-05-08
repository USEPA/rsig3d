/******************************************************************************
PURPOSE: RegriddedDataset.C - RegriddedDataset non-instantiable mixin.
HISTORY: 2013-12-18 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <string.h> // For memset().
#include <ctype.h>  // For toupper().
#include <unistd.h> // For unlink().
#include <math.h>   // For sqrt().

#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <stdexcept> // For std::runtime_error.

#include <Assertions.h> // For PRE(), POST(), AND(), IN_RANGE().
#include <KMLFile.h>    // For write*KML().

#include <NetCDFUtilities.h>    // For writeNetCDFSomeDoubleData().
#include <DataUtilities.h>      // For emvl::minimum(), emvl::maximum().
#include <ResourceHandler.h>    // For emvl::ResourceHandler.
#include <MapProjection.h>      // For MapProjection.
#include <Lambert.h>            // For Lambert.
#include <Albers.h>             // For Albers.
#include <Mercator.h>           // For Mercator.
#include <Stereographic.h>      // For Stereographic.
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <RegriddedDataset.h>   // For RegriddedDataset.


namespace emvl {

static size_t indexOfString( const char* const string,
                             const std::vector<std::string>& strings );
  
//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~RegriddedDataset - Destruct a RegriddedDataset.
******************************************************************************/

RegriddedDataset::~RegriddedDataset() {
  totalPoints_ = 0;

  if ( mapProjection_ ) {
    delete mapProjection_, mapProjection_ = 0;
  }
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool RegriddedDataset::invariant() const {
  const bool result =
    AND5( Dataset::invariant(),
          OR2( IS_ZERO2( pointsPerTimestep_.size(), totalPoints_ ),
               AND2( totalPoints_ == sum( &pointsPerTimestep_[ 0 ],
                                          pointsPerTimestep_.size() ),
                     totalPoints_ > 0 ) ),
          gridParameters_.invariant(),
          IMPLIES( mapProjection_, mapProjection_->invariant() ),
          IMPLIES_ELSE( ! mapProjection_,
                        gridParameters_.gdtyp() == CMAQGridParameters::LATGRD3,
                      gridParameters_.gdtyp() != CMAQGridParameters::LATGRD3));
  return result;
}



//=========================== PROTECTED FUNCTIONS =============================



/******************************************************************************
PURPOSE: RegriddedDataset - Construct a RegriddedDataset.
NOTES:   throws std::runtime_error if unable to create mapProjection_ matching
         the default gridParameters_.
******************************************************************************/

RegriddedDataset::RegriddedDataset() {
  totalPoints_ = 0;
  mapProjection_ = 0;
  CHECK( gridParameters_.invariant() );
  const int gdtyp = gridParameters_.gdtyp();
  const double sphereRadius = gridParameters_.sphereRadius();
  const double xcent = gridParameters_.xcent();
  const double ycent = gridParameters_.ycent();
  const double p_alp = gridParameters_.p_alp();
  const double p_bet = gridParameters_.p_bet();

  switch ( gdtyp ) {
  case CMAQGridParameters::LAMGRD3:
    mapProjection_ = new Lambert( sphereRadius, sphereRadius,
                                  xcent, ycent, p_alp, p_bet ); //x
    break;
  case CMAQGridParameters::ALBGRD3:
    mapProjection_ = new Albers( sphereRadius, sphereRadius,
                                 xcent, ycent, p_alp, p_bet ); //x
    break;
  case CMAQGridParameters::EQMGRD3:
    mapProjection_ = new Mercator( sphereRadius, sphereRadius, xcent ); //x
    break;
  case CMAQGridParameters::POLGRD3:
    mapProjection_ = new Stereographic( sphereRadius, sphereRadius,
                                        xcent, ycent, p_bet ); //x
    break;
  default:
    CHECK( gdtyp == CMAQGridParameters::LATGRD3 );
    break;
  }
}



/******************************************************************************
PURPOSE: RegriddedDataset - Copy constructor.
INPUTS:  const RegriddedDataset& other  Object to copy.
******************************************************************************/

RegriddedDataset::RegriddedDataset( const RegriddedDataset& other ) {
  copy( other );
}



/******************************************************************************
PURPOSE: operator= - Assignment.
INPUTS:  const RegriddedDataset& other  Object to copy.
******************************************************************************/

RegriddedDataset& RegriddedDataset::operator=( const RegriddedDataset& other) {
  copy( other );
  return *this;
}



/******************************************************************************
PURPOSE: copy - Copy other's attributes.
INPUTS:  const RegriddedDataset& other  Other object to copy.
NOTES:   throws std::runtime_error if unable to allocate memory to copy.
         Also calls Dataset::copy() to copy inherited attributes.
******************************************************************************/

void RegriddedDataset::copy( const RegriddedDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    Dataset::copy( other );
    pointsPerTimestep_ = other.pointsPerTimestep_; //x
    totalPoints_       = other.totalPoints_;
    gridParameters_    = other.gridParameters_;

    if ( mapProjection_ ) {
      delete mapProjection_, mapProjection_ = 0;
    }

    if ( other.mapProjection_ ) {
      mapProjection_ = other.mapProjection_->newCopy(); //x      
    }
  }

  POST0( RegriddedDataset::invariant() );
}



/******************************************************************************
PURPOSE: equal - Compare to other's attributes.
INPUTS:  const RegriddedDataset& other  Other object to compare to.
RETURNS: bool true if equal, else false.
         Also calls Dataset::equal() to compare inherited attributes.
******************************************************************************/

bool RegriddedDataset::equal( const RegriddedDataset& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND4( Dataset::equal( other ),
               totalPoints_ == other.totalPoints_,
               pointsPerTimestep_ == other.pointsPerTimestep_,
               gridParameters_ == other.gridParameters_ ) );
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: vprobe - Return nearest data point to time-space location.
INPUTS:  const Timestamp& timestamp  Timestamp of probe.
         const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double elevation      Elevation of probe.
         const int probeLayer        0-based index of variable or IMISSING.
         const int variable          0-based index of variable or IMISSING.
RETURNS: double Data value nearest probe or MISSING if not near any data.
NOTES:   Called by base class member function probe().
         Template Method Design Pattern.
******************************************************************************/

double RegriddedDataset::vprobe( const Timestamp timestamp,
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
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 ) ) );

  const double nearEnough = 1e-2; // Probe close enough to data point.
  const bool isAggregated = timesteps_ == 1;
  double result = MISSING;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {
    Timestamp firstHour( startingTimestamp_ ); // Will adjust to start of hour.

    if ( OR2( isAggregated,
              isInTimeRange( timestamp, firstHour ) ) ) {
      size_t skipPoints = 0;
      size_t searchPoints = 0;

      if ( isAggregated ) {
        searchPoints = totalPoints_;
      } else {
        // Skip points before probe timestep hour (or day if daily data):

        const int hoursPerTimestep = timestepSize_ == DAYS ? 24 : 1;
        const int timestepIncrement =
          AND3( timestepSize_ == DAYS,
                timesteps_ / 24 > 0,
                timesteps_ % 24 == 0 ) ? 24
          : 1;
        Timestamp dataTimestamp = firstHour;
        size_t timestep = 0;

        while ( IMPLIES_ELSE( timestepSize_ == DAYS,
                              ! dataTimestamp.sameDay( timestamp  ),
                              ! dataTimestamp.sameHour( timestamp ) ) ) {
          CHECK( timestep < pointsPerTimestep_.size() );
          const size_t pointsThisTimestep = pointsPerTimestep_[ timestep ];
          skipPoints += pointsThisTimestep;
          timestep += timestepIncrement;
          dataTimestamp.offsetHours( hoursPerTimestep );
        }

        CHECK2( timestep < pointsPerTimestep_.size(),
                skipPoints <= totalPoints_ );

        // Count points to search (within probe hour or day):


        if ( timestepSize_ == DAYS ) { // Sum points in probe day:

          while ( AND2( dataTimestamp.sameDay( timestamp  ),
                        timestep < timesteps_ ) ) {
            CHECK( timestep < pointsPerTimestep_.size() );
            const size_t pointsThisTimestep = pointsPerTimestep_[ timestep ];
            searchPoints += pointsThisTimestep;
            timestep += timestepIncrement;
            dataTimestamp.offsetHours( hoursPerTimestep );
          }
        } else {
          searchPoints = pointsPerTimestep_[ timestep ]; // If hourly data.
        }
      }

      if ( AND2( skipPoints < totalPoints_, searchPoints ) ) {
        const size_t verticesPerCell = cellVertexCount();
        const size_t dataLayers0 = layers();
        const size_t dataLayers = dataLayers0 > 0 ? dataLayers0 : 1;
        size_t endPoint = skipPoints + searchPoints;
        skipPoints *= dataLayers;
        endPoint   *= dataLayers;

        for ( size_t point = skipPoints; point < endPoint; ++point ) {
          const size_t cell = point * verticesPerCell;
          CHECK2( point < data_.size() / variables_,
                  cell <= coordinates_.size() - cellVertexCount() );

          if ( pointInCell( longitude, latitude, elevation, verticesPerCell,
                            &coordinates_[ cell ] ) ) {
            const int probeVariable =
              variable != IMISSING ? variable
              : isVector_ ? variables_ - 2
              : dataVariable_;
            const size_t index =
              probeVariable * totalPoints_ * dataLayers + point;
            CHECK( index < data_.size() );
            result = data_[ index ];

            if ( result < MISSING ) {
              result = MISSING;
            }

            if ( AND2( variable == IMISSING, isVector_ ) ) { // vector magnitude
              const size_t index2 =
                ( probeVariable + 1 ) * totalPoints_ * dataLayers + point;
              CHECK( index2 < data_.size() );
              const double component2 = data_[ index2 ];

              if ( component2 < MISSING ) {
                result = MISSING;
              } else {
                result *= result;
                result += component2 * component2;
                result = sqrt( result );
              }
            }

            if ( point < notes_.size() ) {
              probedNote_ = notes_[ point ];
            }

            point = endPoint; // Stop looping.
          }
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
RETURNS: std::vector<Dataset::SubsetPointers>  Pointers to subset coordinates
                                               and data.
NOTES:   Called by public member function subset().
         Template Method Design Pattern.
******************************************************************************/

std::vector<Dataset::SubsetPointers>
RegriddedDataset::vsubset( const Timestamp beginTimestamp,
                           const Timestamp endTimestamp,
                           const int variable ) const {
  PRE4( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp,
        OR2( variable == IMISSING, IN_RANGE( variable, 0, variables() - 1 )));

  std::vector<Dataset::SubsetPointers> result;
  const bool isAggregated = timesteps_ == 1;

  // overlappedTimeRange() will expand all timestamps to full hour:

  Timestamp beginHour( beginTimestamp );
  Timestamp endHour( endTimestamp );
  Timestamp firstHour( startingTimestamp_ );
  Timestamp lastHour( endingTimestamp_ );

  if ( OR2( isAggregated,
            overlappedTimeRange( beginHour, endHour, firstHour, lastHour ) ) ) {
    size_t skipPoints = 0;
    size_t subsetPoints = 0;

    if ( isAggregated ) {
      subsetPoints = totalPoints_;
    } else {

      /*
       * If daily data and [beginHour, endHour] is 1 hour then
       * expand [beginHour, endHour] to full day.
       */

      if ( AND2( timestepSize_ == DAYS, beginHour.sameDay( endHour ) ) ) {
        beginHour = beginHour.startOfDay();
        endHour   = endHour.endOfDay();
      }

      // Get first matching timestep (i.e., within the same hour) & skip points:

      const int hoursPerTimestep = timestepSize_ == DAYS ? 24 : 1;
      const int timestepIncrement =
        AND3( timestepSize_ == DAYS,
              timesteps_ / 24 > 0,
              timesteps_ % 24 == 0 ) ? 24
        : 1;
      Timestamp dataTimestamp = firstHour;
      size_t timestep = 0;

      for ( timestep = 0;
            AND2( timestep < timesteps_,
                  ! IN_RANGE( dataTimestamp, beginHour, endHour ) );
            timestep += timestepIncrement ) {
        const size_t pointsThisTimestep = pointsPerTimestep_[ timestep ];
        skipPoints += pointsThisTimestep;
        dataTimestamp.offsetHours( hoursPerTimestep );
      }

      // Get last matching timestep (i.e., within the same hour) & subset points:

      for ( ;
            AND2( timestep < timesteps_,
                  IN_RANGE( dataTimestamp, beginHour, endHour ) );
            timestep += timestepIncrement ) {
        const size_t pointsThisTimestep = pointsPerTimestep_[ timestep ];
        subsetPoints += pointsThisTimestep;
        dataTimestamp.offsetHours( hoursPerTimestep );
      }
    }

    if ( subsetPoints ) {
      const size_t dataLayers0 = layers();
      const size_t dataLayers = dataLayers0 > 0 ? dataLayers0 : 1;
      subsetPoints *= dataLayers;
      skipPoints   *= dataLayers;
      const int variable0 =
        variable != IMISSING ? variable
        : isVector_ ? variables_ - 2
        : dataVariable_;
      const size_t index = variable0 * totalPoints_ * dataLayers + skipPoints;
      const size_t cell  = skipPoints * cellVertexCount();
      CHECK5( skipPoints < totalPoints_ * dataLayers,
              IN_RANGE( subsetPoints, 1, totalPoints_ * dataLayers ),
              index < data_.size(),
              IN_RANGE( index + subsetPoints, 1, data_.size() ),
              cell <= coordinates_.size() - cellVertexCount() );
      const std::string* notes = subsetNotes( skipPoints );
      const SubsetPointers subsetPointers = {
        subsetPoints * cellVertexCount(),
        &coordinates_[ cell ],
        subsetPoints,
        &data_[ index ],
        notes ? subsetPoints : 0,
        notes
      };
      result.push_back( subsetPointers ); //x

      if ( AND2( variable == IMISSING, isVector_ ) ) {
        const size_t index2 =
          ( variable0 + 1 ) * totalPoints_ * dataLayers + skipPoints;
        CHECK5( skipPoints < totalPoints_ * dataLayers,
                IN_RANGE( subsetPoints, 1, totalPoints_ * dataLayers ),
                index2 < data_.size(),
                IN_RANGE( index2 + subsetPoints, 1, data_.size() ),
                cell <= coordinates_.size() - cellVertexCount() );
        const SubsetPointers subsetPointers2 = {
          subsetPoints * cellVertexCount(),
          &coordinates_[ cell ],
          subsetPoints,
          &data_[ index2 ],
          notes ? subsetPoints : 0,
          notes
        };
        result.push_back( subsetPointers2 ); //x
      }
    }
  }

  POST( IMPLIES( result.size(),
                 AND10( IMPLIES_ELSE( isVector(),
                                      AND2( result.size() > 1,
                                            result.size() % 2 == 0 ),
                                      result.size() == 1 ),
                        result[ 0 ].dataCount,
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
                                  IN_RANGE( emvl::minimum( result[ 0 ].data,
                                                           result[0].dataCount),
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
                        IMPLIES( AND2( isVector(), variable == IMISSING ),
                                       result.size() >= 2 ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: subsetNotes - Called by vsubset() with the number of subset points to
         allow derived classes with notes to include pointers to the
         corresponding subset.
INPUTS:  const size_t skipPoints  Number of points skipped in vsubset().
RETURNS: const std::string*  Pointer to notes, if any.
NOTES:   This default implementation returns 0.
******************************************************************************/

const std::string* RegriddedDataset::subsetNotes( const size_t ) const {
  return 0;
}



/******************************************************************************
PURPOSE: computeCMAQCellCoordinates - Compute coordinates_ from data_ columns,
         rows, gridParameters_, mapProjection_.
INPUTS:  const size_t layersPerPoint  Number of layers per point.
         const double centerElevations[ totalPoints_ ] 0 or cell centers.
******************************************************************************/

void RegriddedDataset::computeCMAQCellCoordinates( const size_t layersPerPoint,
                                            const double centerElevations[] ) {
  PRE09( coordinates_.size() ==
           totalPoints_ * ( layersPerPoint > 1 ? layersPerPoint : 1 )
           * cellVertexCount(),
         isValidData(),
         variables_ >= 3,
         ! strcmp( variableNames_[ 0 ].c_str(), "column" ),
         ! strcmp( variableNames_[ 1 ].c_str(), "row" ),
         IMPLIES( centerElevations,
                  ! strcmp( variableNames_[ 2 ].c_str(), "layer" ) ),
         gridParameters_.invariant(),
         IMPLIES( mapProjection_, mapProjection_->invariant() ),
         IMPLIES_ELSE( ! mapProjection_,
                       gridParameters_.gdtyp() == CMAQGridParameters::LATGRD3,
                      gridParameters_.gdtyp() != CMAQGridParameters::LATGRD3));

  const size_t totalPoints = coordinates_.size() / cellVertexCount();
  const double* const columns = &data_[ 0 ];
  const double* const rows    = columns + totalPoints;
  const double* const layers  = centerElevations ? rows + totalPoints : 0;
  const double xorig = gridParameters_.xorig();
  const double yorig = gridParameters_.yorig();
  const double xcell = gridParameters_.xcell();
  const double ycell = gridParameters_.ycell();
  const int ncols    = gridParameters_.ncols();
  const int nrows    = gridParameters_.nrows();
  const int nlays    = gridParameters_.nlays();
  const size_t verticesPerCell = cellVertexCount();

  for ( size_t cell = 0, vertexIndex = 0; cell < totalPoints; ++cell,
       vertexIndex += verticesPerCell ) {
    const long long column = columns[ cell ] - 1;
    const long long row    = rows[ cell ] - 1;
    const long long layer  = layers ? layers[ cell ] - 1 : -1;

    if ( ! AND3( column < ncols,
                 row    < nrows,
                 IMPLIES( layers, layer < nlays ) ) ) {
      throw std::runtime_error( "Read invalid regrided column/row/layer." );//x
    }

    const double xMinimum = xorig + column * xcell;
    const double xMaximum = xMinimum + xcell;
    const double yMinimum = yorig + row * ycell;
    const double yMaximum = yMinimum + ycell;
    const double centerElevation =
      centerElevations ? centerElevations[ cell ] : 0.0;
    const double thickness =
      centerElevations ? gridParameters_.layerThickness( layer ) : 0.0;
    const double halfThickness = thickness * 0.5;
    const double lowerElevation = centerElevation - halfThickness;

    Point sw = { xMinimum, yMinimum, lowerElevation };
    Point se = { xMaximum, yMinimum, lowerElevation };
    Point ne = { xMaximum, yMaximum, lowerElevation };
    Point nw = { xMinimum, yMaximum, lowerElevation };

    if ( mapProjection_ ) {
      mapProjection_->unproject(xMinimum, yMinimum, sw.longitude, sw.latitude);
      mapProjection_->unproject(xMaximum, yMinimum, se.longitude, se.latitude);
      mapProjection_->unproject(xMaximum, yMaximum, ne.longitude, ne.latitude);
      mapProjection_->unproject(xMinimum, yMaximum, nw.longitude, nw.latitude);
    }

    CHECK( IN_RANGE( vertexIndex + 3, 3, coordinates_.size() - 1 ) );
    coordinates_[ vertexIndex     ] = sw;
    coordinates_[ vertexIndex + 1 ] = se;
    coordinates_[ vertexIndex + 2 ] = ne;
    coordinates_[ vertexIndex + 3 ] = nw;

    if ( centerElevations ) {
      const double upperElevation = centerElevation + halfThickness;
      CHECK2( cellVertexCount() == 8,
              IN_RANGE( vertexIndex + 7, 7, coordinates_.size() - 1 ) );
      sw.elevation = upperElevation;
      se.elevation = upperElevation;
      ne.elevation = upperElevation;
      nw.elevation = upperElevation;
      coordinates_[ vertexIndex + 4 ] = sw;
      coordinates_[ vertexIndex + 5 ] = se;
      coordinates_[ vertexIndex + 6 ] = ne;
      coordinates_[ vertexIndex + 7 ] = nw;
    }
  }

  computeBounds();

  POST010( coordinates_.size() ==
             totalPoints_ * ( layersPerPoint > 1 ? layersPerPoint : 1 )
             * cellVertexCount(),
           isValidData(),
           variables_ >= 3,
           ! strcmp( variableNames_[ 0 ].c_str(), "column" ),
           ! strcmp( variableNames_[ 1 ].c_str(), "row" ),
           IMPLIES( centerElevations,
                   ! strcmp( variableNames_[ 2 ].c_str(), "layer" ) ),
           gridParameters_.invariant(),
           IMPLIES( mapProjection_, mapProjection_->invariant() ),
           IMPLIES_ELSE( ! mapProjection_,
                        gridParameters_.gdtyp() == CMAQGridParameters::LATGRD3,
                       gridParameters_.gdtyp() != CMAQGridParameters::LATGRD3),
           isValidCoordinates() );
}



/******************************************************************************
PURPOSE: computeCMAQGridElevations - Compute grid cell center elevations from
         gridParameters_.
OUTPUTS: std::vector<float> elevations  Cell center elevations.
         elevations[ hasElevation() ? gridParameters_.nlays() : 1 ]
                   [ gridParameters_.nrows() ][ gridParameters_.ncols() ]
NOTES:   Throws exception upon failure to allocate elevations.
         Computes elevations from sigma-P values assuming surface_elevation = 0
******************************************************************************/

void RegriddedDataset::computeCMAQGridElevations(
                                      std::vector<float>& elevations ) const {
  PRE( gridParameters_.invariant() );

  const size_t layers = hasElevation() ? gridParameters_.nlays() : 1;
  const size_t layerCells = gridParameters_.nrows() * gridParameters_.ncols();
  elevations.clear();
  elevations.resize( layers * layerCells ); //x

  if ( layers == 1 ) {
    const double layerThicknesses = 19.8075; // CDC grid layer 1 thickness (m).
    const double elevation = layerThicknesses * 0.5;

    for ( size_t cell = 0; cell < layerCells; ++cell ) {
      elevations[ cell ] = elevation;
    }

  } else if ( gridParameters_.gdtyp() == CMAQGridParameters::LATGRD3 ) {
    const double* elevationsAtLevels = gridParameters_.vglvls();

    for ( size_t layer = 0, index = 0; layer < layers; ++layer ) {
      const double cellCenterElevation = 0.5 *
        ( elevationsAtLevels[ layer ] + elevationsAtLevels[ layer + 1 ] );

      for ( size_t cell = 0; cell < layerCells; ++cell, ++index ) {
        elevations[ index ] = cellCenterElevation;
      }
    }

  } else {
    const double g = 9.81;
    const double R = 287.04;
    const double A = 50.0;
    const double T0s = 290.0;
    const double P00 = 100000.0;
    const double surfaceElevation = 0.0;
    const int levels = layers + 1;
    const double topPressure = gridParameters_.vgtop();
    const double* sigmaPressures = gridParameters_.vglvls();
    std::vector<double> elevationsAtSigmas( levels ); //x

    elevationsAtSigmaPressures( g, R, A, T0s, P00, surfaceElevation,
                                levels, topPressure, sigmaPressures,
                                &elevationsAtSigmas[ 0 ] );

    for ( size_t layer = 0, index = 0; layer < layers; ++layer ) {
      const double cellCenterElevation = 0.5 *
        ( elevationsAtSigmas[ layer ] + elevationsAtSigmas[ layer + 1 ] );

      for ( size_t cell = 0; cell < layerCells; ++cell, ++index ) {
        elevations[ index ] = cellCenterElevation;
      }
    }
  }

  POST3( (int) elevations.size() ==
           ( hasElevation() ? gridParameters_.nlays() : 1 ) *
           gridParameters_.nrows() * gridParameters_.ncols(),
         isElevation( emvl::minimum( &elevations[ 0 ], elevations.size() ) ),
         isElevation( emvl::maximum( &elevations[ 0 ], elevations.size() ) ) );
}



/******************************************************************************
PURPOSE: writeIOAPICoordinates - Write IOAPI-format longitude
         latitude and optionally elevation data to file.
INPUTS:  const int file  NetCDF file to write to.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedDataset::writeIOAPICoordinates( const int file ) const {
  PRE( file > 0 );

  const bool andElevation = hasElevation();
  const size_t layers  = andElevation ? gridParameters_.nlays() : 1;
  const size_t rows    = gridParameters_.nrows();
  const size_t columns = gridParameters_.ncols();
  const size_t dimensions[ 4 ] = { 1, layers, rows, columns };
  std::vector<float> longitudes;
  std::vector<float> latitudes;
  std::vector<float> elevations;

  computeCMAQGridLongitudesLatitudes( gridParameters_, 0, mapProjection_,
                                      longitudes, latitudes ); //x

  if ( andElevation ) {
    computeCMAQGridElevations( elevations ); //x
  }

  // For each timestep, write the same coordinates:

  bool ok = true;

  for ( size_t timestep = 0; AND2( ok, timestep < timesteps_ ); ++timestep ) {

    // Write longitudes:

    ok = writeNetCDFSomeFloatData( file, "LONGITUDE", timestep, dimensions,
                                   &longitudes[ 0 ] );

    if ( ok ) { // Write latitudes:
      ok = writeNetCDFSomeFloatData( file, "LATITUDE", timestep, dimensions,
                                     &latitudes[ 0 ] );

      if ( AND2( ok, andElevation ) ) { // Write elevations
        ok = writeNetCDFSomeFloatData( file, "ELEVATION", timestep, dimensions,
                                       &elevations[ 0 ] );
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write IOAPI-format coordinates.");//x
  }
}



/******************************************************************************
PURPOSE: writeIOAPIVariable - Write IOAPI-format variable data to file.
INPUTS:  const int file  NetCDF file to write to.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedDataset::writeIOAPIVariable( const int file ) const {
  PRE( file > 0 );

  const size_t layerVariable  = indexOfString( "layer", variableNames_ );
  const size_t rowVariable    = indexOfString( "row", variableNames_ );
  const size_t columnVariable = indexOfString( "column", variableNames_ );
  const std::string defaultVariableName( variableName() );
  const size_t dataVariable   =
    indexOfString( defaultVariableName.c_str(), variableNames_ );
  const bool hasLayers =
    AND2( hasElevation(), layerVariable != variableNames_.size() );
  const size_t layers = hasLayers ? gridParameters_.nlays() : 1;
  const size_t dataLayers0 = this->layers();
  const size_t dataLayers = dataLayers0 == 0 ? 1 : dataLayers0;
  const size_t totalDataPoints = totalPoints_ * dataLayers;
  const size_t rows = gridParameters_.nrows();
  const size_t columns = gridParameters_.ncols();
  const size_t layerSize = rows * columns;
  const size_t cells = layers * layerSize;
  const size_t layerOffset = hasLayers ? layerVariable * totalDataPoints : 0;
  const size_t rowOffset = rowVariable * totalDataPoints;
  const size_t columnOffset = columnVariable * totalDataPoints;
  const size_t dataOffset = dataVariable * totalDataPoints;
  const size_t dimensions[ 4 ] = { 1, layers, rows, columns };
  const float BADVAL3 = -9.999E+36f;
  std::vector<float> variableData( cells, BADVAL3 ); //x
  bool ok = true;

  // Copy variable name to IOAPI form (not more than 16 characters, uppercase):

  const std::string& variable = variableName();
  const size_t length0 = variable.length();
  enum { NAMLEN3 = 16 };
  const size_t length = length0 > NAMLEN3 ? NAMLEN3 : length0;
  char name[ NAMLEN3 + 1 ] = "";
  size_t index = 0;

  for ( index = 0 ; index < length; ++index ) {
    name[ index ] = toupper( variable[ index ] );
  }

  for ( ; index < NAMLEN3; ++index ) {
    name[ index ] = '\0';
  }

  name[ NAMLEN3 ] = '\0';

  CHECK5( (size_t) ( emvl::minimum( &data_[ columnOffset ],
                                    totalDataPoints ) + 0.5 ) >= 1,
          (size_t) ( emvl::maximum( &data_[ columnOffset ],
                                    totalDataPoints ) + 0.5 ) <= columns,
          (size_t) ( emvl::minimum( &data_[ rowOffset ],
                                    totalDataPoints ) + 0.5 ) >= 1,
          (size_t) ( emvl::maximum( &data_[ rowOffset ],
                                    totalDataPoints ) + 0.5 ) <= rows,
          IMPLIES( hasLayers,
                  AND2( (size_t) ( emvl::minimum( &data_[ layerOffset ],
                                                  totalDataPoints ) + 0.5 )
                           >= 1,
                        (size_t) ( emvl::maximum( &data_[ layerOffset ],
                                                  totalDataPoints ) + 0.5 ) <=
                          layers ) ) );

  // For each timestep:

  for ( size_t timestep = 0,
        dataIndex = dataOffset,
        layerIndex = layerOffset,
        rowIndex = rowOffset,
        columnIndex = columnOffset;
        AND2( ok, timestep < timesteps_ );
        ++timestep ) {

    variableData.assign( cells, BADVAL3 ); // Re-initialize cells to BADVAL3.

    // Store data value in cells that have data for this timestep:

    const size_t points = pointsPerTimestep_[ timestep ];

    for ( size_t point = 0; point < points; ++point ) {

      for ( size_t dataLayer = 0; dataLayer < dataLayers; ++dataLayer,
            ++dataIndex, ++layerIndex, ++rowIndex, ++columnIndex ) {
        CHECK3( layerIndex  < data_.size(),
                rowIndex    < data_.size(),
                columnIndex < data_.size() );
        const size_t layer =
          ! hasLayers ? 0 :   (size_t) ( data_[ layerIndex  ] + 0.5 ) - 1;
        const size_t row    = (size_t) ( data_[ rowIndex    ] + 0.5 ) - 1;
        const size_t column = (size_t) ( data_[ columnIndex ] + 0.5 ) - 1;
        const size_t cell = layer * layerSize + row * columns + column;
        CHECK5( layer     < layers,
                row       < rows,
                column    < columns,
                cell      < variableData.size(),
                dataIndex < data_.size() );
        variableData[ cell ] = data_[ dataIndex ];
      }
    }

    // Write data for timestep:

    ok = writeNetCDFSomeFloatData( file, name, (int) timestep, dimensions,
                                   &variableData[ 0 ] );
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write IOAPI-format variable." ); //x
  }
}



/******************************************************************************
PURPOSE: writeIOAPI - Write IOAPI-format file.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedDataset::writeIOAPI( const std::string& fileName ) const {
  PRE( fileName.length() );
  const bool andElevation = hasElevation();
  const int layers  = andElevation ? gridParameters_.nlays() : 1;
  const int rows    = gridParameters_.nrows();
  const int columns = gridParameters_.ncols();
  const int skipVariables = 2 + andElevation; // col, row, layer.
  const size_t outputVariables = skipVariables + 1 + isVector_; // Lon, Lat, Elv, O3.
  const size_t fileSizeEstimate =
    outputVariables * timesteps_ * layers * rows * columns * 4 +
    timesteps_ * outputVariables * 2 * 4 + 10000; // TFLAG + header.
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    int dimensionIds[ 6 ] = { -1, -1, -1, -1, -1, -1 };
    const bool forceUppercase = true; // Ensure variable names are all capitals
    writeIOAPIHeader( file, andElevation, forceUppercase, skipVariables,
                      layers, rows, columns, gridParameters_,
                      0, 0, dimensionIds ); //x
    writeIOAPITFLAGVariable( file, outputVariables ); //x
    writeIOAPICoordinates( file ); //x
    writeIOAPIVariable( file ); //x
    ok = true;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create NetCDF-IOAPI file for "
                              "regridded data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: timestampsPerDataPoint - Create and return a copy of timestamps per
         data point.
NOTES:   Throws exception upon failure to allocate result.
******************************************************************************/

std::vector<Timestamp> RegriddedDataset::timestampsPerDataPoint() const {
  PRE( true );
  const size_t layers0 = layers();
  const size_t pointLayers = layers0 > 0 ? layers0 : 1;
  const size_t dataPoints = totalPoints_ * pointLayers;
  const int hoursPerTimestep = timestepSize_ == DAYS ? 24 : 1;
  Timestamp timestamp( startingTimestamp_ );
  std::vector<Timestamp> result( dataPoints, timestamp ); //x

  for ( size_t timestep = 0, index = 0;
        timestep < timesteps_;
        ++timestep, timestamp.offsetHours( hoursPerTimestep ) ) {
    CHECK( timestep < pointsPerTimestep_.size() );
    const size_t pointsThisTimestep =
      pointsPerTimestep_[ timestep ] * pointLayers;

    for ( size_t point = 0; point < pointsThisTimestep; ++point, ++index ) {
      CHECK( index < result.size() );
      result[ index ] = timestamp;
    }
  }


  POST( result.size() == totalPoints_ * ( layers() > 0 ? layers() : 1 ) );

  return result;
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters
   and optional ELEVATION/LAYER):

   Timestamp(UTC) LONGITUDE(deg) LATITUDE(deg) ELEVATION(m) COLUMN(-)  ROW(-) \
     LAYER(-)  AOD(-)
   2006-08-26T00:00:00-0000   -86.81000  33.55000  60.0   11   22  3   0.03800
   ...

  */

  const bool andElevation = hasElevation();
  bool ok = false;

  if ( isVector_ ) {
    const char* const headerFormat =
      andElevation ?
        "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tELEVATION(m)"
        "\tCOLUMN(-)\tROW(-)\tLAYER(-)\t%s(%s)\t%s(%s)\n"
      : "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)"
        "\tCOLUMN(-)\tROW(-)\t%s(%s)\t%s(%s)\n";
    const int backslashesAndPercentsInFormat = 8 + ( andElevation ? 2 : 0 ) + 3;
    ok = fprintf( stream, headerFormat,
                  variableNames_[ variables_ - 2 ].c_str(),
                  variableUnits_[ variables_ - 2 ].c_str(),
                  variableNames_[ variables_ - 1 ].c_str(),
                  variableUnits_[ variables_ - 1 ].c_str() )
         >= (int) strlen( headerFormat ) - backslashesAndPercentsInFormat;
  } else {
    const char* const headerFormat =
      andElevation ?
        "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tELEVATION(m)"
        "\tCOLUMN(-)\tROW(-)\tLAYER(-)\t%s(%s)\n"
      : "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)"
        "\tCOLUMN(-)\tROW(-)\t%s(%s)\n";
    const int backslashesAndPercentsInFormat = 8 + ( andElevation ? 2 : 0 );
    ok = fprintf( stream, headerFormat,
                  variableNames_[ dataVariable_ ].c_str(),
                  variableUnits_[ dataVariable_ ].c_str() )
         >= (int) strlen( headerFormat ) - backslashesAndPercentsInFormat;
  }

  if ( ok ) {
    Timestamp timestamp( startingTimestamp_ );
    const int hoursPerTimestep = timestepSize_ == DAYS ? 24 : 1;
    const size_t layers0 = layers();
    const size_t pointLayers = layers0 > 0 ? layers0 : 1;
    const size_t dataPoints = totalPoints_ * pointLayers;
    size_t cellIndex = 0;
    size_t columnIndex = 0;
    size_t rowIndex    = columnIndex + dataPoints;
    size_t layerIndex  = andElevation ? rowIndex + dataPoints : rowIndex;
    size_t dataIndex   = layerIndex + dataPoints;

    for ( size_t timestep = 0; timestep < timesteps_;
         ++timestep, timestamp.offsetHours( hoursPerTimestep ) ) {
      const size_t pointsThisTimestep = pointsPerTimestep_[ timestep ];
      Timestamp::UTCString timestampString = "";
      timestamp.toUTCString( timestampString );

      for ( size_t point = 0; point < pointsThisTimestep; ++point ) {

        for ( size_t level = 0; level < pointLayers; ++level,
             ++columnIndex, ++rowIndex, ++layerIndex, ++dataIndex,
             ++cellIndex ) {
          CHECK5( cellIndex < coordinates_.size() / cellVertexCount(),
                  columnIndex < data_.size(),
                  rowIndex    < data_.size(),
                  layerIndex  < data_.size(),
                  dataIndex   < data_.size() );
          const Point center = cellCenter( cellIndex );
          const double longitude = center.longitude;
          const double latitude  = center.latitude;
          const int column = (int) data_[ columnIndex ];
          const int row    = (int) data_[ rowIndex ];
          const double value     = data_[ dataIndex ];
          CHECK5( isLongitude( longitude ), isLatitude( latitude ),
                  column > 0, row > 0, isFinite( value ) );

          if ( isVector_ ) {
            const size_t dataIndex2 = dataIndex + dataPoints;
            CHECK( dataIndex2 < data_.size() );
            const double value2 = data_[ dataIndex2 ];

            if ( andElevation ) {
              const char* const dataFormat =
                "%s\t%11.5f\t%11.5f\t%11.5f\t%11d\t%11d\t%11d\t%11.5e\t%11.5e\n";
              const double elevation = center.elevation;
              const int layer  = (int) data_[ layerIndex ];
              CHECK2( isElevation( elevation ), layer > 0 );
              ok = fprintf( stream, dataFormat,
                          timestampString, longitude, latitude, elevation,
                          column, row, layer, value, value2 ) > 0;
            } else {
              const char* const dataFormat =
                "%s\t%11.5f\t%11.5f\t%11d\t%11d\t%11.5e\t%11.5e\n";
              ok = fprintf( stream, dataFormat,
                            timestampString, longitude, latitude,
                            column, row, value, value2 ) > 0;
            }
          } else {

            if ( andElevation ) {
              const char* const dataFormat =
                "%s\t%11.5f\t%11.5f\t%11.5f\t%11d\t%11d\t%11d\t%11.5e\n";
              const double elevation = center.elevation;
              const int layer  = (int) data_[ layerIndex ];
              CHECK2( isElevation( elevation ), layer > 0 );
              ok = fprintf( stream, dataFormat,
                          timestampString, longitude, latitude, elevation,
                          column, row, layer, value ) > 0;
            } else {
              const char* const dataFormat =
                "%s\t%11.5f\t%11.5f\t%11d\t%11d\t%11.5e\n";
              ok = fprintf( stream, dataFormat,
                            timestampString, longitude, latitude,
                            column, row, value ) > 0;
            }
          }

          if ( ! ok ) {
            point = totalPoints_;
            timestep = timesteps_;
          }
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write ASCII regridded data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedDataset::writeCOARDS( const std::string& fileName) const {

  PRE( fileName.length() );

  const size_t timeVariables = 3; // yyyymmdd, hhmmss, time.
  const size_t coordinateVariables = 2 + (hasElevation() ? 1 : 0);//lon,lat,elv
  const size_t wordSize = 4; // IEEE-754 float.
  const size_t headerSize = 10000; // Guess.
  const size_t fileVariables =
    variables_ + coordinateVariables + timeVariables;
  const size_t layers0 = layers();
  const size_t pointLayers = layers0 > 0 ? layers0 : 1;
  const size_t dataPoints = totalPoints_ * pointLayers;
  const size_t fileSizeEstimate =
    fileVariables * dataPoints * wordSize + headerSize;
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
    CHECK( isSignedInt( dataPoints ) );
    const size_t dimension = dataPoints;

    if ( createNetCDFDimensions( file, 1, &dimensionName, &dimension,
                                 &dimensionId ) ) {
      std::vector<Timestamp> timestamps = timestampsPerDataPoint(); //x
      writeCOARDSCommon( file, 0, true, false, hasElevation_, true, 1,
                         1, &dimensionId, &dimension, &dimensionName,
                         timestamps ); //x
      ok = true;
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create NetCDF-COARDS file for "
                              "regridded data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedDataset::writeKML() const {
  PRE( outputKMLFile_ );
  const char* const variable =
    isVector_ ? "Wind" : variableNames_[ dataVariable_ ].c_str();
  std::string source( name_ ); //x
  const size_t variableIndex = source.rfind( variable );

  if ( variableIndex != std::string::npos && variableIndex > 1 ) {
    source.erase( variableIndex - 1 );
  }

  const char* const units = variableUnits_[ dataVariable_ ].c_str();
  const bool andElevation = hasElevation();
  const char* const dataType =
     isVector_ ? "Vector" : andElevation ? "Hexahedron" : "Quadrilateral";
  const char* const description =
    isVector_ ? "Regridded vector data." : "Regridded point data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;

  writeStartFolderKML( outputKMLFile_, source.c_str(), variable, units,
                       dataType, description );

  const int components = isVector_ ? 2 : 1;
  const int vertexCount = cellVertexCount();
  const double variableMinimum = minimum();
  const double variableMaximum = maximum();
  Timestamp timestamp( startingTimestamp_ );
  const int hoursPerTimestep = timestepSize_ == DAYS ? 24 : 1;
  const size_t layers0 = layers();
  const size_t pointLayers = layers0 > 0 ? layers0 : 1;
  const size_t dataPoints = totalPoints_ * pointLayers;
  size_t dataIndex = ( variables_ - components ) * dataPoints;
  size_t cellIndex = 0;

  for ( size_t timestep = 0; timestep < timesteps_;
       ++timestep, timestamp.offsetHours( hoursPerTimestep ) ) {
    const size_t pointsThisTimestep = pointsPerTimestep_[ timestep ];
    const int yyyymmdd = timestamp.yyyymmdd();
    const int hhmmss = timestamp.hhmmss();
    const int valueTimestamp =
      timestepSize_ == DAYS ? yyyymmdd : yyyymmdd * 100 + hhmmss / 10000;

    for ( size_t point = 0; point < pointsThisTimestep; ++point ) {

      for ( size_t level = 0; level < pointLayers; ++level,
           ++dataIndex, ++cellIndex ) {
        CHECK2( cellIndex < coordinates_.size(),
                dataIndex < data_.size() );
        const double value = data_[ dataIndex ];

        if ( value > MISSING ) {
          bool ok = true;
          double value2 = MISSING;

          if ( isVector_ ) {
            const size_t dataIndex2 = dataIndex + dataPoints;
            CHECK( dataIndex2 < data_.size() );
            value2 = data_[ dataIndex2 ];
            ok = value2 > MISSING;
          }

          if ( ok ) {
            double cellCoordinates[ 8 * 3 ];

            for ( int vertex = 0, vertex3 = 0; vertex < vertexCount; ++vertex) {
              const size_t index = cellIndex * vertexCount + vertex;
              CHECK2( index < coordinates_.size(),
                      (size_t) vertex3 + 2 <
                        sizeof cellCoordinates / sizeof *cellCoordinates );
              cellCoordinates[ vertex3++ ] = coordinates_[ index ].longitude;
              cellCoordinates[ vertex3++ ] = coordinates_[ index ].latitude;
              cellCoordinates[ vertex3++ ] = coordinates_[ index ].elevation;
            }

            const double magnitude =
              isVector_ ? sqrt( value * value + value2 * value2 ) : value;
            const Color color =
              dataColorFunction( magnitude, variableMinimum, variableMaximum );

            writeCellDataToKML( outputKMLFile_, source.c_str(),
                                variable, units, valueTimestamp, components,
                                MISSING, value, value2, MISSING,
                                vertexCount, cellCoordinates, color );
          }
        }
      }
    }
  }

  writeEndFolderKML( outputKMLFile_ );

  POST( true );
}



/******************************************************************************
PURPOSE: indexOfString - Index of string in strings or -1 if not present.
INPUTS:  const char* const string                 String to search for.
         const std::vector<std::string>& strings  Strings to search.
RETURNS: size_t index of string in strings, else strings.size() if not present.
******************************************************************************/

static size_t indexOfString( const char* const string,
                             const std::vector<std::string>& strings ) {

  PRE03( string, *string, strings.size() );
  const size_t count = strings.size();
  size_t result = count;
  size_t index = 0;

  do {

    if ( AND2( strings[ index ].length(),
               ! strcmp( string, strings[ index ].c_str() ) ) ) {
      result = index;
      index = count - 1;
    }

    ++index;
  } while ( index < count );

  POST0( OR2( result == strings.size(),
              AND2( result < strings.size(),
                    ! strcmp( string, strings[ result ].c_str() ) ) ) );
  return result;
}



} // namespace emvl.



