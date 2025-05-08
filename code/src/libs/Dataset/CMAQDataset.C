/******************************************************************************
PURPOSE: CMAQDataset.C - Dataset for CMAQ gridded model data.
HISTORY: 2013-04-08 plessel.todd@epa.gov
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
#include <CMAQGridParameters.h> // For CMAQGridParameters.
#include <NetCDFUtilities.h>    // For *NetCDF*() routines.
#include <MapProjection.h>      // For MapProjection.
#include <Lambert.h>            // For Lmabert.
#include <Mercator.h>           // For Mercator.
#include <Stereographic.h>      // For Stereographic.
#include <Albers.h>             // For Albers.
#include <Dataset.h>            // For Dataset.
#include <CMAQDataset.h>        // For CMAQDataset.

namespace emvl {


//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~CMAQDataset - Destruct a CMAQDataset.
******************************************************************************/

CMAQDataset::~CMAQDataset() {
  layers_         = 0;
  rows_           = 0;
  columns_        = 0;
  memset( subsetIndices_, 0, sizeof subsetIndices_ );
  delete mapProjection_, mapProjection_ = 0;
  elevationDataIndex_ = 0;
  elevation1_ = 0.0;
  is64bit_ = false;
}



/******************************************************************************
PURPOSE: CMAQDataset - Construct a CMAQDataset from reading a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

CMAQDataset::CMAQDataset( FILE* stream ) {
  PRE0( stream );

  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = HEXAHEDRON;
  hasElevation_     = true;
  hasArea_          = true;
  hasVolume_        = true;
  isStationary_     = true;
  isGroup_          = false;
  isStructuredGrid_ = true;
  isVector_         = false;
  timestepSize_     = HOURS;
  timesteps_        = 0;
  layers_           = 0;
  rows_             = 0;
  columns_          = 0;
  elevationDataIndex_ = 0;
  elevation1_       = 0.0;
  memset( subsetIndices_, 0, sizeof subsetIndices_ );
  mapProjection_    = 0;
  is64bit_          = false;
  bool readAndSkipLonLats = false;
  readXDRHeader( stream, readAndSkipLonLats ); //x
  readXDRData( stream, readAndSkipLonLats ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid CMAQ data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: CMAQDataset - Construct a CMAQDataset from reading a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

CMAQDataset::CMAQDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  timesteps_        = 0;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = HEXAHEDRON;
  hasElevation_     = true;
  hasArea_          = true;
  hasVolume_        = true;
  isStationary_     = true;
  isGroup_          = false;
  isStructuredGrid_ = true;
  isVector_         = false;
  timestepSize_     = HOURS;
  layers_           = 0;
  rows_             = 0;
  columns_          = 0;
  elevationDataIndex_ = 0;
  elevation1_       = 0.0;
  memset( subsetIndices_, 0, sizeof subsetIndices_ );
  mapProjection_    = 0;
  is64bit_          = false;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    bool readAndSkipLonLats = false;
    readXDRHeader( file, readAndSkipLonLats ); //x
    readXDRData( file, readAndSkipLonLats ); //x
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid CMAQ data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: CMAQDataset - Copy-construct a CMAQDataset.
INPUTS:  const CMAQDataset& other  Object to copy.
******************************************************************************/

CMAQDataset::CMAQDataset( const CMAQDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    layers_         = other.layers_;
    rows_           = other.rows_;
    columns_        = other.columns_;
    memcpy( subsetIndices_, other.subsetIndices_, sizeof subsetIndices_ );
    gridParameters_ = other.gridParameters_;
    mapProjection_  = other.mapProjection_->newCopy(); //x
    longitudes_     = other.longitudes_; //x
    latitudes_      = other.latitudes_; //x
    elevations_     = other.elevations_; //x
    elevationDataIndex_ = other.elevationDataIndex_; //x
    elevation1_     = other.elevation1_; //x
    is64bit_        = false;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a CMAQDataset.
INPUTS:  const CMAQDataset& other  Object to copy.
RETURNS: CMAQDataset& *this.
******************************************************************************/

CMAQDataset& CMAQDataset::operator=( const CMAQDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    copy( other ); // Copy base class attributes.
    layers_         = other.layers_;
    rows_           = other.rows_;
    columns_        = other.columns_;
    memcpy( subsetIndices_, other.subsetIndices_, sizeof subsetIndices_ );
    gridParameters_ = other.gridParameters_;
    delete mapProjection_, mapProjection_ = 0;
    mapProjection_  = other.mapProjection_->newCopy(); //x
    longitudes_     = other.longitudes_; //x
    latitudes_      = other.latitudes_; //x
    elevations_     = other.elevations_; //x
    elevationDataIndex_ = other.elevationDataIndex_; //x
    elevation1_     = other.elevation1_; //x
    is64bit_        = other.is64bit_;
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

Dataset* CMAQDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: CMAQDataset::sample()" ); //x
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

Dataset* CMAQDataset::regrid( const int regridMethod,
                              const CMAQGridParameters&
                                cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: CMAQDataset::regrid()" ); //x
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

void CMAQDataset::write( const std::string& directory,
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
  } else if ( format == QueryParameters::NETCDF_IOAPI ) {
    writeIOAPI( fileName ); //x
  } else if ( format == QueryParameters::KML ) {
    writeKML(); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool CMAQDataset::invariant() const {
  const bool result =
    AND29( Dataset::invariant(),
           layers_ > 0,
           rows_ > 0,
           columns_ > 0,
           IMPLIES_ELSE( mapProjection_,
                         longitudes_.size() == ( rows_ + 1 ) * ( columns_ + 1 ),
                         longitudes_.size() == 0 ),
           latitudes_.size() == longitudes_.size(),
           IMPLIES_ELSE( variableNames_[ 0 ] == "ELEVATION",
                         elevations_.size() ==
                           ( layers_ + 1 ) * rows_ * columns_,
                         elevations_.size() == 0 ),
           elevationDataIndex_ < data_.size(),
           variables_ >= 1,
           cellType_== HEXAHEDRON,
           hasElevation_     == true,
           hasArea_          == true,
           hasVolume_        == true,
           isStationary_     == true,
           isGroup_          == false,
           isStructuredGrid_ == true,
           subsetIndices_[ TIME   ][ LAST  ] >= subsetIndices_[ TIME  ][FIRST],
           subsetIndices_[ LAYER  ][ FIRST ] >= 1,
           subsetIndices_[ LAYER  ][ LAST  ] >= subsetIndices_[ LAYER ][FIRST],
           subsetIndices_[ ROW    ][ FIRST ] >= 1,
           subsetIndices_[ ROW    ][ LAST  ] >= subsetIndices_[ ROW   ][FIRST],
           subsetIndices_[ COLUMN ][ FIRST ] >= 1,
           subsetIndices_[ COLUMN ][ LAST  ] >= subsetIndices_[ COLUMN][FIRST],
           timesteps_ == 1 +
             subsetIndices_[ TIME ][ LAST ] - subsetIndices_[ TIME ][ FIRST ],
           layers_ == 1 +
             subsetIndices_[ LAYER ][ LAST ] - subsetIndices_[ LAYER ][ FIRST],
           rows_ == 1 +
             subsetIndices_[ ROW ][ LAST ] - subsetIndices_[ ROW ][FIRST],
           columns_ == 1 +
             subsetIndices_[ COLUMN ][ LAST ] - subsetIndices_[ COLUMN][FIRST],
           gridParameters_.invariant(),
           IMPLIES( mapProjection_, mapProjection_->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool CMAQDataset::operator==( const CMAQDataset& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND7( equal( other ),
               longitudes_ == other.longitudes_,
               latitudes_  == other.latitudes_,
               ! memcmp( subsetIndices_, other.subsetIndices_,
                         sizeof subsetIndices_ ),
               gridParameters_ == other.gridParameters_,
               IMPLIES_ELSE( mapProjection_,
                             ! strcmp( mapProjection_->name(),
                                       other.mapProjection_->name() ),
                             ! other.mapProjection_ ),
               is64bit_ == other.is64bit_  ) );

  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool CMAQDataset::operator!=( const CMAQDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



/******************************************************************************
PURPOSE: isSurfaceData - Is this dataset only on the Earth surface?
RETURNS: bool true if this is only layer-1 data.
******************************************************************************/

bool CMAQDataset::isSurfaceData() const {
  const bool result =
    AND2( layers_ == 1, subsetIndices_[ LAYER ][ FIRST ] == 1 );
  return result;
}



/******************************************************************************
PURPOSE: layers - Number of grid layers.
RETURNS: size_t Number of grid layers.
******************************************************************************/

size_t CMAQDataset::layers() const {
  const size_t result = layers_;
  POST0( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: rows - Number of grid rows.
RETURNS: size_t Number of grid rows.
******************************************************************************/

size_t CMAQDataset::rows() const {
  const size_t result = rows_;
  POST0( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: columns - Number of grid columns.
RETURNS: size_t Number of grid columns.
******************************************************************************/

size_t CMAQDataset::columns() const {
  const size_t result = columns_;
  POST0( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: cellVertices - Get a copy of a cell's vertices. This is needed when
         coordinates are not stored, but rather computed (by this routine) as
         indicated by subset() returning SubsetPointers.coordinates == 0.
INPUTS:  const size_t cell  Index of cell.
OUTPUTS: Point points[ cellVertexCount() ]  Cell's vertices.
******************************************************************************/

void CMAQDataset::cellVertices( const size_t cell, Point points[] ) const {
  PRE05( points,
         cell < layers_ * rows_ * columns_ * cellVertexCount(),
         IMPLIES_ELSE( variableNames_[ 0 ] == "ELEVATION",
                       elevations_.size() ==
                         ( layers_ + 1 ) * rows_ * columns_,
                       elevations_.size() == 0 ),
         IMPLIES_ELSE( mapProjection_,
                       longitudes_.size() == ( 1 + rows_ ) * ( 1 + columns_ ),
                       longitudes_.size() == 0 ),
         latitudes_.size() == longitudes_.size() );

  const size_t layerSize = rows_ * columns_;
  const size_t cellL = cell % layerSize; // First layer cell index
  const size_t cellRow = cellL / columns_;
  const size_t cellColumn = cellL % columns_;

  double longitudeSW = 0.0;
  double longitudeSE = 0.0;
  double longitudeNE = 0.0;
  double longitudeNW = 0.0;
  double latitudeSW  = 0.0;
  double latitudeSE  = 0.0;
  double latitudeNE  = 0.0;
  double latitudeNW  = 0.0;

  if ( mapProjection_ ) {
    const size_t rowSize = columns_ + 1;
    const size_t offset = cellRow * rowSize + cellColumn;
    const size_t offsetNextRow = offset + rowSize;
    const size_t indexSW = offset;
    const size_t indexSE = indexSW + 1;
    const size_t indexNE = offsetNextRow + 1;
    const size_t indexNW = offsetNextRow;

    CHECK4( indexSW < longitudes_.size(),
            indexSE < longitudes_.size(),
            indexNW < longitudes_.size(),
            indexNE < longitudes_.size() );

    longitudeSW = longitudes_[ indexSW ];
    longitudeSE = longitudes_[ indexSE ];
    longitudeNE = longitudes_[ indexNE ];
    longitudeNW = longitudes_[ indexNW ];
    latitudeSW  = latitudes_[  indexSW ];
    latitudeSE  = latitudes_[  indexSE ];
    latitudeNE  = latitudes_[  indexNE ];
    latitudeNW  = latitudes_[  indexNW ];
  } else {
    const double xorig = gridParameters_.xorig();
    const double yorig = gridParameters_.yorig();
    const double xcell = gridParameters_.xcell();
    const double ycell = gridParameters_.ycell();
    const double west = xorig + ( subsetIndices_[ COLUMN ][ FIRST] - 1) * xcell;
    const double south = yorig + ( subsetIndices_[ ROW ][ FIRST ] - 1 ) * ycell;
    const double cellWest = west + cellColumn * xcell;
    const double cellEast = cellWest + xcell;
    const double cellSouth = south + cellRow * ycell;
    const double cellNorth = cellSouth + ycell;

    longitudeSW = cellWest;
    longitudeSE = cellEast;
    longitudeNE = cellEast;
    longitudeNW = cellWest;
    latitudeSW  = cellSouth;
    latitudeSE  = cellSouth;
    latitudeNE  = cellNorth;
    latitudeNW  = cellNorth;
  }

  double elevationBottom = elevation1_;
  double elevationTop    = elevation1_ + 0.1;

  if ( elevations_.size() ) {
    const size_t cellLayer = cell / layerSize;
    const size_t indexBottom = cellLayer * layerSize + cellL;
    const size_t indexTop = indexBottom + layerSize;
    CHECK2( indexBottom < elevations_.size(),
            indexTop    < elevations_.size() );
    elevationBottom = elevations_[ indexBottom ];
    elevationTop    = elevations_[ indexTop ];
  }

  CHECK( elevationTop > elevationBottom );

  points[ 0 ].longitude = longitudeSW;
  points[ 0 ].latitude  = latitudeSW;
  points[ 0 ].elevation = elevationBottom;
  points[ 1 ].longitude = longitudeSE;
  points[ 1 ].latitude  = latitudeSE;
  points[ 1 ].elevation = elevationBottom;
  points[ 2 ].longitude = longitudeNE;
  points[ 2 ].latitude  = latitudeNE;
  points[ 2 ].elevation = elevationBottom;
  points[ 3 ].longitude = longitudeNW;
  points[ 3 ].latitude  = latitudeNW;
  points[ 3 ].elevation = elevationBottom;
  points[ 4 ].longitude = longitudeSW;
  points[ 4 ].latitude  = latitudeSW;
  points[ 4 ].elevation = elevationTop;
  points[ 5 ].longitude = longitudeSE;
  points[ 5 ].latitude  = latitudeSE;
  points[ 5 ].elevation = elevationTop;
  points[ 6 ].longitude = longitudeNE;
  points[ 6 ].latitude  = latitudeNE;
  points[ 6 ].elevation = elevationTop;
  points[ 7 ].longitude = longitudeNW;
  points[ 7 ].latitude  = latitudeNW;
  points[ 7 ].elevation = elevationTop;

  POST02( isPoint( points[ 0 ] ), isPoint( points[ cellVertexCount() - 1 ] ) );
}



/******************************************************************************
PURPOSE: gridName - Name of grid.
RETURNS: const std::string&  Name of grid.
******************************************************************************/

const std::string& CMAQDataset::gridName() const {
  PRE( true );
  const std::string& result = gridName_;
  POST( result.length() );
  return result;
}



/******************************************************************************
PURPOSE: subsetIndices - 0-based timestep and 1-based layer, row, column
         indices of subset of original grid.
RETURNS: const CMAQDataset::SubsetIndices& Name of grid.
******************************************************************************/

const CMAQDataset::SubsetIndices& CMAQDataset::subsetIndices() const {
  PRE( true );
  const SubsetIndices& result = subsetIndices_;
  POST12( result[ TIME   ][ FIRST ] >= 0,
          result[ TIME   ][ LAST  ] >= result[ TIME  ][FIRST],
          result[ LAYER  ][ FIRST ] >= 1,
          result[ LAYER  ][ LAST  ] >= result[ LAYER ][FIRST],
          result[ ROW    ][ FIRST ] >= 1,
          result[ ROW    ][ LAST  ] >= result[ ROW   ][FIRST],
          result[ COLUMN ][ FIRST ] >= 1,
          result[ COLUMN ][ LAST  ] >= result[ COLUMN][FIRST],
          timesteps() == 1 + result[ TIME   ][ LAST ] - result[ TIME ][ FIRST],
          layers()    == 1 + result[ LAYER  ][ LAST ] - result[ LAYER][ FIRST],
          rows()      == 1 + result[ ROW    ][ LAST ] - result[ ROW  ][ FIRST],
          columns()   == 1 + result[ COLUMN ][ LAST ] - result[COLUMN][FIRST]);
  return result;
}



/******************************************************************************
PURPOSE: gridParameters - Parameters of grid.
RETURNS: const CMAQGridParameters&  Name of grid.
******************************************************************************/

const CMAQGridParameters& CMAQDataset::gridParameters() const {
  PRE( true );
  const CMAQGridParameters& result = gridParameters_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: mapProjection - MapProjection of grid.
RETURNS: const MapProjection*  MapProjection of grid or 0 if lonlat grid.
******************************************************************************/

const MapProjection* CMAQDataset::mapProjection() const {
  PRE( true );
  const MapProjection* const result = mapProjection_;
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: computeBounds - Compute coordinate bounds.
******************************************************************************/

void CMAQDataset::computeBounds() {
  PRE02( IMPLIES_ELSE( mapProjection_,
                       longitudes_.size() == ( rows_ + 1 ) * ( columns_ + 1 ),
                       longitudes_.size() == 0 ),
         latitudes_.size() == longitudes_.size() );

  double west = 0.0;
  double east = 0.0;
  double south = 0.0;
  double north = 0.0;

  if ( longitudes_.size() ) {
    range( &longitudes_[ 0 ], longitudes_.size(), west, east );
    range( &latitudes_[ 0 ], latitudes_.size(), south, north );
  } else {
    const double xorig = gridParameters_.xorig();
    const double yorig = gridParameters_.yorig();
    const double xcell = gridParameters_.xcell();
    const double ycell = gridParameters_.ycell();
    west = xorig + ( subsetIndices_[ COLUMN ][ FIRST ] - 1 ) * xcell;
    east = xorig + subsetIndices_[ COLUMN ][ LAST ] * xcell;
    south = yorig + ( subsetIndices_[ ROW ][ FIRST ] - 1 ) * ycell;
    north = yorig + subsetIndices_[ ROW ][ LAST ] * ycell;
  }

  bounds_.west( west );
  bounds_.east( east );
  bounds_.south( south );
  bounds_.north( north );
  POST0( bounds_.invariant() );
}



/******************************************************************************
PURPOSE: timestepOfTimestamp - Determine timestep containing timestamp.
INPUTS:  const Timestamp& timestamp  Timestamp to check for.
OUTPUTS: size_t& timestep Timestep (0-based index) for timestamp, or 0.
RETURNS: true if in time range, else false.
******************************************************************************/

bool CMAQDataset::timestepOfTimestamp( const Timestamp timestamp,
                                       size_t& timestep ) const {
  PRE( timestamp.invariant() );
  timestep = 0;
  const bool result = isInTimeRange( timestamp, timestep );
  POST( IMPLIES_ELSE( result,
                      timestep < timesteps_,
                      timestep == 0 ) );
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

size_t CMAQDataset::timestepsOfTimestamps( const Timestamp beginTimestamp,
                                           const Timestamp endTimestamp,
                                           size_t& firstTimestep,
                                           size_t& lastTimestep ) const {
  PRE3( beginTimestamp.invariant(),
        endTimestamp.invariant(),
        beginTimestamp <= endTimestamp );

  size_t result = 0;
  const bool overlapped =
    overlappedTimeRange( beginTimestamp, endTimestamp, firstTimestep, result );
  lastTimestep = 0;

  if ( overlapped ) {
    lastTimestep = firstTimestep + result - 1;
  }

  POST( IMPLIES_ELSE( result,
                      AND3( IN_RANGE( result, 1, timesteps_ ),
                            firstTimestep < timesteps_,
                            IN_RANGE( lastTimestep,
                                      firstTimestep, timesteps_ - 1 ) ),
                      IS_ZERO2( firstTimestep, lastTimestep ) ) );
  return result;
}



/******************************************************************************
PURPOSE: probedCell - Compute layer, row, column of a probe.
INPUTS:  const double longitude      Longitude of probe.
         const double latitude       Latitude of probe.
         const double elevation      Elevation of probe or MISSING for 2D.
OUTPTUS: size_t& column              Column index of probed cell.
         size_t& row                 Row    index of probed cell.
         size_t& layer               Layer  index of probed cell.
RETURNS: bool true if probe location is within the grid, else false.
******************************************************************************/

bool CMAQDataset::probedCell( const double longitude,
                              const double latitude,
                              const double elevation,
                              size_t& column,
                              size_t& row,
                              size_t& layer ) const {
  PRE2( isLongitudeLatitude( longitude, latitude ),
        OR2( elevation == MISSING, isElevation( elevation ) ) );

  const double nearEnough = 1e-2; // Probe close enough to data point.
  bool result = false;
  column = row = layer = 0;

  if ( isInBounds( longitude, latitude, nearEnough ) ) {
 
    // Get probed cell by projecting:

    const double xcell = gridParameters_.xcell();
    const double ycell = gridParameters_.ycell();
    const double xMinimum =
      gridParameters_.xorig() +
      ( subsetIndices_[ COLUMN ][ FIRST ] - 1 ) * xcell;
    const double yMinimum =
      gridParameters_.yorig() +
      ( subsetIndices_[ ROW ][ FIRST ] - 1 ) * ycell;
    const double xMaximum = xMinimum + columns_ * xcell;
    const double yMaximum = yMinimum + rows_ * ycell;
    const double oneOverWidth  = 1.0 / xcell;
    const double oneOverHeight = 1.0 / ycell;
    double x = longitude;
    double y = latitude;

    if ( mapProjection_ ) {
      mapProjection_->project( longitude, latitude, x, y );
    }

    if ( AND2( IN_RANGE( x, xMinimum, xMaximum ),
               IN_RANGE( y, yMinimum, yMaximum ) ) ) {
      const double fractionalColumn = ( x - xMinimum ) * oneOverWidth  + 1.0;
      const double fractionalRow    = ( y - yMinimum ) * oneOverHeight + 1.0;
      column = (size_t) fractionalColumn; // Truncate fraction.
      row    = (size_t) fractionalRow;

      if ( column > columns_ ) {
        column = columns_;
      }

      if ( row > rows_ ) {
        row = rows_;
      }

      --column;
      --row;

      CHECK2( column < columns_, row < rows_ );

      // Compute layer index: */

      const size_t layerCells = rows_ * columns_;
      layer = 0;
      result = true;

      if ( AND3( elevation != MISSING, layers_ > 1, elevations_.size() ) ) {
        size_t index = row * columns_ + column;

        do {
          const size_t index2 = index + layerCells;
          CHECK2( index < elevations_.size(),
                  IN_RANGE( index2, index, elevations_.size() - 1 ) );
          const double cellLowerElevation = elevations_[ index ];
          const double cellUpperElevation = elevations_[ index2 ];
          result = IN_RANGE(elevation, cellLowerElevation, cellUpperElevation);

          if ( ! result ) {
            ++layer;
            index += layerCells;
          }

        } while ( AND2( ! result, layer < layers_ ) );
      }
    }
  }

  if ( ! result ) {
    column = row = layer = 0;
  }

  POST( IMPLIES_ELSE( result,
                      AND3( column < columns_,
                            row    < rows_,
                            layer  < layers_ ),
                     IS_ZERO3( column, row, layer ) ) );
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

double CMAQDataset::vprobe( const Timestamp timestamp,
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
  size_t timestep = 0;

  if ( timestepOfTimestamp( timestamp, timestep ) ) {
    const size_t layerCells = rows_ * columns_;
    const size_t cells = layers_ * layerCells;
    const size_t loadedTimesteps =
      dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
    const size_t loadedTimestep =
      dataFileOffset_ ? timestep % BUFFERED_DATA_TIMESTEPS : timestep;
    const size_t timestepOffset = loadedTimestep * cells;

    // Update elevations_ from data_ for cellVertices():

    if ( AND3( elevation != MISSING, timestepOffset != elevationDataIndex_,
               elevations_.size() ) ) {
      elevationDataIndex_ = timestepOffset;
      CHECK( elevationDataIndex_ < data_.size() );
      computeElevations( &data_[ elevationDataIndex_ ] ); //x cell bot/top.
    }

    size_t column = 0;
    size_t row = 0;
    size_t layer = 0;

    if ( probedCell( longitude, latitude, elevation, column, row, layer ) ) {

      if ( probeLayer != IMISSING ) {
        layer = probeLayer;
        CHECK( layer < layers_ );
      }

      const size_t cellOffset =
        loadedTimestep * cells + layer * layerCells + row * columns_ + column;

      if ( OR2( ! isVector_, variable != IMISSING ) ) {
        const int variable0 = variable == IMISSING ? dataVariable_ : variable;
        const size_t index = variable0 * loadedTimesteps * cells + cellOffset;
        CHECK( index < data_.size() );
        result = data_[ index ];
      } else { // Compute vector magnitude:
        const size_t loadedTimestepsCells = loadedTimesteps * cells;
        const int first = variableNames_[ 0 ] == "ELEVATION" ? 1 : 0;
        bool atLeastOneValidValue = false;
        result = 0.0;

        for ( int component = first; component < variables_; ++component ) {
          const size_t index = component * loadedTimestepsCells + cellOffset;
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

void CMAQDataset::vtimeseries( const Timestamp beginTimestamp,
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

  size_t firstTimestep = 0;
  size_t subsetTimesteps = 0;
  size_t skippedHours = 0;

  if ( overlappedTimeRange( beginTimestamp, endTimestamp,
                            firstTimestep, subsetTimesteps, skippedHours ) ) {
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
      const double value =
        probe( timestamp, longitude, latitude, elevation, probeLayer, IMISSING);

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

  POST3( IMPLIES_ELSE( IN3( timestepSize(), HOURS, DAYS ),
                       result.size() >=
                         (size_t) 1 + beginTimestamp.hoursUntil( endTimestamp ),
                       result.size() >=
                         (size_t) 1 + beginTimestamp.monthsUntil( endTimestamp ) ),
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
CMAQDataset::vsubset( const Timestamp beginTimestamp,
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
    const size_t cells = layers_ * rows_ * columns_;
    const size_t loadedTimesteps =
      dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
    const size_t loadedFirstTimestep =
      dataFileOffset_ ? firstTimestep % BUFFERED_DATA_TIMESTEPS : firstTimestep;
    const size_t loadedFirstTimestepCells = loadedFirstTimestep * cells;

    if ( elevations_.size() ) {

      // Update elevations_ from data_ for cellVertices():
      // Note this is only correct for the first timestep!

      if ( loadedFirstTimestepCells != elevationDataIndex_ ) {
        elevationDataIndex_ = loadedFirstTimestepCells;
        CHECK( elevationDataIndex_ < data_.size() );
        computeElevations( &data_[ elevationDataIndex_ ] ); //x cell bot/top.
      }
    }

    const size_t coordinatesCount =
      layers_ * rows_ * columns_ * cellVertexCount();
    const size_t subsetCount = subsetTimesteps * cells;

    if ( OR2( ! isVector_, variable != IMISSING ) ) {
      const int variable0 = variable == IMISSING ? dataVariable_ : variable;
      const size_t firstIndex =
        variable0 * loadedTimesteps * cells + loadedFirstTimestepCells;
      CHECK( firstIndex < data_.size() );
      CHECK( IN_RANGE( subsetCount, 1, data_.size() ) );
      CHECK( IN_RANGE( firstIndex + subsetCount, 1, data_.size() ) );
      const Dataset::SubsetPointers subsetPointers = {
        coordinatesCount,
        0, // Caller must use cellVertices() instead.
        subsetCount,
        &data_[ firstIndex ],
        0,
        0
      };
      result.push_back( subsetPointers ); //x
    } else { // Append each vector component variable:
      const int first = variableNames_[ 0 ] == "ELEVATION" ? 1 : 0;

      for ( int component = first; component < variables_; ++component ) {
        const size_t firstIndex =
          component * loadedTimesteps * cells + loadedFirstTimestepCells;
        CHECK( firstIndex < data_.size() );
        CHECK( IN_RANGE( subsetCount, 1, timesteps_ * cells ) );
        const Dataset::SubsetPointers subsetPointers = {
          coordinatesCount,
          0, // Caller must use cellVertices() instead.
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
                        result[ 0 ].coordinatesCount ==
                          layers() * rows() * columns() * cellVertexCount(),
                        result[ 0 ].coordinates == 0,
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
                        result[ result.size() - 1 ].coordinates == 0,
                        result[ result.size() - 1 ].data,
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
                        result[ result.size() - 1 ].notes == 0,
                        IMPLIES( AND2( isVector(), variable == IMISSING ),
                                 result.size() == (size_t) variables() - 1 ))));
  return result;
}



/******************************************************************************
PURPOSE: readXDRHeader - Read XDR header from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: bool readAndSkipLonLats
         name_
         description_
         variableNames_
         variableUnits_
         variables_
         dataVariable_
         timesteps_
         layers_
         rows_
         columns_
         startingTimestamp_
         endingTimestamp_
NOTES:   Throws exception upon failure to read valid header from a stream.
******************************************************************************/

void CMAQDataset::readXDRHeader( FILE* stream, bool& readAndSkipLonLats ) {
  PRE0( stream );

  /* Read XDR header that looks like this:

    SUBSET 9.0 CMAQ
    C_12_OAQPS
    https://www.epa.gov/ttn/scram/,EPA-RTP,CMAQSubset
    2006-08-28T00:00:00-0000
    # data dimensions: timesteps variables layers rows columns:
    24 4 24 13 12
    # subset indices (0-based time, 1-based layer/row/column): \
     first-timestep last-timestep first-layer last-layer first-row last-row \
     first-column last-column:
     0 23 1 24 43 55 139 150
    # Variable names:
    LONGITUDE LATITUDE ELEVATION O3
    # Variable units:
    deg deg m ppmV
    # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
    33 45 40 -97 6.37e+06 6.37e+06
    # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
    279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
    0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 0.4\
    0.3 0.2 0.1 0
    # IEEE-754 32-bit reals data[variables][timesteps][layers][rows][columns]:

  */

  readAndMatchLine( stream, "SUBSET 9.0 CMAQ\n" ); //x
  readDescription( stream ); //x
  gridName_ = description_; //x
  description_.clear();
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  size_t dimensions[ 5 ] = { 0, 0, 0, 0, 0 };
  readDimensions( stream, sizeof dimensions / sizeof *dimensions,
                  dimensions ); //x
  timesteps_ = dimensions[ 0 ];
  const size_t tempVariables = dimensions[ 1 ];
  layers_    = dimensions[ 2 ];
  rows_      = dimensions[ 3 ];
  columns_   = dimensions[ 4 ];
  readSubsetIndices( stream ); //x
  readVariablesAndUnits( stream, tempVariables ); //x
  CHECK( variables_ == (int) tempVariables );

  // Check then remove longitude, latitude variables:

  if ( AND4( variables_ == (int) tempVariables,
             variables_ >= 3,
             variableNames_[ 0 ] == "LONGITUDE",
             variableNames_[ 1 ] == "LATITUDE" ) ) {

    CHECK2( variableNames_.size() == tempVariables,
            variableUnits_.size() == tempVariables );

    variableNames_.erase( variableNames_.begin(), variableNames_.begin() + 2 );
    variableUnits_.erase( variableUnits_.begin(), variableUnits_.begin() + 2 );
    variables_ -= 2;
    dataVariable_ = variables_ - 1;
    readAndSkipLonLats = true;
  }

  if ( variableNames_[ 0 ] != "ELEVATION" ) {
    const char* const firstVariableName = variableNames_[ 0 ].c_str();
    elevation1_ =
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
    isVector_ = variables_ > 1;
  } else if ( variables_ > 1 ) {
    CHECK3( (size_t) variables_ == variableNames_.size(),
            variableNames_.size() == variableUnits_.size(),
            (size_t) dataVariable_ == variableNames_.size() - 1 );

    if ( AND2( variableNames_[ dataVariable_ - 1 ] == "UWIND",
               variableNames_[ dataVariable_     ] == "VWIND" ) ) {
      variableUnits_[ dataVariable_ - 1 ] = "m/s";
      variableUnits_[ dataVariable_     ] = "m/s";
      isVector_ = true;
    } else if (  AND4( variables_ > 2,
                       variableNames_[ dataVariable_ - 2 ] == "UWIND",
                       variableNames_[ dataVariable_ - 1 ] == "VWIND",
                       variableNames_[ dataVariable_     ] == "WWIND" ) ) {
      variableUnits_[ dataVariable_ - 2 ] = "m/s";
      variableUnits_[ dataVariable_ - 1 ] = "m/s";
      variableUnits_[ dataVariable_     ] = "m/s";
      isVector_ = true;
    }
  }

  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.
  readProjectionAndGrid( stream, mapProjection_, gridParameters_ ); //x
  const int match = //x
    readAndMatchLine2( stream,
                      "# IEEE-754 32-bit reals "
                        "data[variables][timesteps][layers][rows][columns]:\n",
                      "# IEEE-754 64-bit reals "
                        "data[variables][timesteps][layers][rows][columns]:\n");
  is64bit_ = match == 2;

  POST027( AND3( isValidName(),
                 isValidDescription(),
                 isValidVariableNamesAndUnits() ),
           variables_ > 1,
           dataVariable_ == variables_ - 1,
           GT_ZERO4( timesteps_, layers_ , rows_, columns_ ),
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           cellType_== HEXAHEDRON,
           hasElevation_ == true,
           hasArea_ == true,
           hasVolume_ == true,
           isStationary_ == true,
           isGroup_ == false,
           isStructuredGrid_ == true,
           subsetIndices_[ TIME   ][ LAST  ] >= subsetIndices_[ TIME  ][FIRST],
           subsetIndices_[ LAYER  ][ FIRST ] >= 1,
           subsetIndices_[ LAYER  ][ LAST  ] >= subsetIndices_[ LAYER ][FIRST],
           subsetIndices_[ ROW    ][ FIRST ] >= 1,
           subsetIndices_[ ROW    ][ LAST  ] >= subsetIndices_[ ROW   ][FIRST],
           subsetIndices_[ COLUMN ][ FIRST ] >= 1,
           subsetIndices_[ COLUMN ][ LAST  ] >= subsetIndices_[ COLUMN][FIRST],
           timesteps_ == 1 +
             subsetIndices_[ TIME ][ LAST ] - subsetIndices_[ TIME ][ FIRST ],
           layers_ == 1 +
             subsetIndices_[ LAYER ][ LAST ] - subsetIndices_[ LAYER ][ FIRST],
           rows_ == 1 +
             subsetIndices_[ ROW ][ LAST ] - subsetIndices_[ ROW ][FIRST],
           columns_ == 1 +
             subsetIndices_[ COLUMN ][ LAST ] - subsetIndices_[ COLUMN][FIRST],
           gridParameters_.invariant(),
           IMPLIES( mapProjection_, mapProjection_->invariant() ) );
}



/******************************************************************************
PURPOSE: readSubsetIndices - Read subsetIndices of XDR header from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: subsetIndices_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void CMAQDataset::readSubsetIndices( FILE* stream ) {
  PRE02( stream, NON_ZERO4( timesteps_, layers_, rows_, columns_ ) );
  memset( subsetIndices_, 0, sizeof subsetIndices_ );
  skipLine( stream ); //x
  size_t tempIndices[ 4 ][ 2 ] = {
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }
  };
  bool ok =
    fscanf( stream,
            "%" LU " %" LU " %" LU " %" LU " %" LU " %" LU " %" LU " %" LU "\n",
            &tempIndices[ TIME ][ FIRST ],
            &tempIndices[ TIME ][ LAST ],
            &tempIndices[ LAYER ][ FIRST ],
            &tempIndices[ LAYER ][ LAST ],
            &tempIndices[ ROW ][ FIRST ],
            &tempIndices[ ROW ][ LAST ],
            &tempIndices[ COLUMN ][ FIRST ],
            &tempIndices[ COLUMN ][ LAST ] ) == 8;

  if ( ok ) {
    subsetIndices_[ TIME   ][ FIRST ] = tempIndices[ TIME   ][ FIRST];
    subsetIndices_[ TIME   ][ LAST  ] = tempIndices[ TIME   ][ LAST ];
    subsetIndices_[ LAYER  ][ FIRST ] = tempIndices[ LAYER  ][ FIRST];
    subsetIndices_[ LAYER  ][ LAST  ] = tempIndices[ LAYER  ][ LAST ];
    subsetIndices_[ ROW    ][ FIRST ] = tempIndices[ ROW    ][ FIRST];
    subsetIndices_[ ROW    ][ LAST  ] = tempIndices[ ROW    ][ LAST ];
    subsetIndices_[ COLUMN ][ FIRST ] = tempIndices[ COLUMN ][ FIRST];
    subsetIndices_[ COLUMN ][ LAST  ] = tempIndices[ COLUMN ][ LAST ];
    ok =
      AND11( subsetIndices_[ TIME   ][LAST ] >= subsetIndices_[ TIME  ][FIRST],
             subsetIndices_[ LAYER  ][FIRST] >= 1,
             subsetIndices_[ LAYER  ][LAST ] >= subsetIndices_[ LAYER ][FIRST],
             subsetIndices_[ ROW    ][FIRST] >= 1,
             subsetIndices_[ ROW    ][LAST ] >= subsetIndices_[ ROW   ][FIRST],
             subsetIndices_[ COLUMN ][FIRST] >= 1,
             subsetIndices_[ COLUMN ][LAST ] >= subsetIndices_[ COLUMN][FIRST],
             timesteps_ == 1 +
               subsetIndices_[ TIME ][ LAST ] - subsetIndices_[ TIME ][ FIRST],
             layers_ == 1 +
               subsetIndices_[ LAYER ][ LAST ] - subsetIndices_[ LAYER][FIRST],
             rows_ == 1 +
               subsetIndices_[ ROW ][ LAST ] - subsetIndices_[ ROW ][ FIRST ],
             columns_ == 1 +
               subsetIndices_[ COLUMN ][LAST] - subsetIndices_[COLUMN][FIRST]);
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid subset indices."); //x
  }

  POST011( subsetIndices_[ TIME   ][LAST ] >= subsetIndices_[ TIME  ][FIRST],
           subsetIndices_[ LAYER  ][FIRST] >= 1,
           subsetIndices_[ LAYER  ][LAST ] >= subsetIndices_[ LAYER ][FIRST],
           subsetIndices_[ ROW    ][FIRST] >= 1,
           subsetIndices_[ ROW    ][LAST ] >= subsetIndices_[ ROW   ][FIRST],
           subsetIndices_[ COLUMN ][FIRST] >= 1,
           subsetIndices_[ COLUMN ][LAST ] >= subsetIndices_[ COLUMN][FIRST],
           timesteps_ == 1 +
             subsetIndices_[ TIME ][ LAST ] - subsetIndices_[ TIME ][ FIRST],
           layers_ == 1 +
             subsetIndices_[ LAYER ][ LAST ] - subsetIndices_[ LAYER][FIRST],
           rows_ == 1 +
             subsetIndices_[ ROW ][ LAST ] - subsetIndices_[ ROW ][ FIRST ],
           columns_ == 1 +
             subsetIndices_[ COLUMN ][ LAST ] - subsetIndices_[COLUMN][FIRST]);
}



/******************************************************************************
PURPOSE: loadedDataSubset - Determine if data is available for the given time
         range and, if available and not already in data_ then read it from
         fileName_ into data_.
INPUTS:  const Timestamp firstTimestamp  Timestamp of start of data needed.
         const Timestamp lastTimestamp   Timestamp of end   of data needed.
OUTPUTS: firstDataTimestep_, lastDataTimestep_, data_.
RETURNS: bool true if data is available, else false if no data is available or
         it exceeds BUFFERED_DATA_TIMESTEPS or failed to read from fileName_.
NOTES:   This is called by Dataset::probe() and Dataset::subset().
         If a file open/seek/read failure occurs then a failure message is
         printed to stdout.
         If a partial read occurs then damaged values in data_ are zeroed.
******************************************************************************/

bool CMAQDataset::loadedDataSubset( const Timestamp firstTimestamp,
                                    const Timestamp lastTimestamp ) const {
  PRE0( IMPLIES( dataFileOffset_, ! fileName_.empty() ) );
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

      DEBUG( fprintf( stderr, "CMAQDataset::loadedDataSubset(): "
                     "firstDataTimestep_ = %" LU
                     ", lastDataTimestep_ = %" LU ", "
                     "firstTimestep = %" LU ", lastTimestep = %" LU ", "
                     "outside = %d\n",
                     firstDataTimestep_, lastDataTimestep_,
                     firstTimestep, lastTimestep,
                     outside ); )

      if ( outside ) { // Must read XDR-format data from fileName_ into data_.
        result = readBufferedDataSubset( firstTimestep, false );
      }
    }
  }

  POST0( isValidData() );
  return result;
}



/******************************************************************************
PURPOSE: readBufferedDataSubset - Read variables * BUFFERED_DATA_TIMESTEPS from
         fileName_ into data_.
INPUTS:  const size_t firstTimestep  First timestep of data to read.
         const bool checkAll         Check all values?
                                     If false just check sampled values.
OUTPUTS: firstDataTimestep_, lastDataTimestep_, data_.
RETURNS: bool true if successful, else false if failed to read from fileName_.
NOTES:   This is called initially by readXDRData() and by loadedDataSubset().
         If a file open/seek/read failure occurs then a failure message is
         printed to stdout.
         If a partial read occurs then damaged values in data_ are zeroed.
******************************************************************************/

bool CMAQDataset::readBufferedDataSubset( const size_t firstTimestep,
                                          const bool checkAll ) const {
  PRE07( dataFileOffset_,
         ! fileName_.empty(),
         GT_ZERO5( variables_, timesteps_, layers_, rows_, columns_ ),
         timesteps_ > BUFFERED_DATA_TIMESTEPS,
         timesteps_ % BUFFERED_DATA_TIMESTEPS == 0,
         data_.size() ==
           variables_ * BUFFERED_DATA_TIMESTEPS * layers_ * rows_ * columns_,
         firstTimestep <= timesteps_ - BUFFERED_DATA_TIMESTEPS );

  bool result = false;

  /*
   * Read XDR-format data from fileName_ into data_.
   * XDR file data looks like this:
   * # IEEE-754 32-bit data[variables][timesteps][layers][rows][columns]
   */

  ResourceHandler<FILE*> fileCloser( fopen( fileName_.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();
  size_t damagedDoubles = 0;

  if ( file ) {
    const size_t cells = layers_ * rows_ * columns_;
    const size_t readWords = BUFFERED_DATA_TIMESTEPS * cells;
    CHECK( readWords <= data_.size() );
    const size_t bytesPerWord = is64bit_ ? 8 : 4;
    const size_t firstReadTimestep =
      ( firstTimestep / BUFFERED_DATA_TIMESTEPS ) * BUFFERED_DATA_TIMESTEPS;
    double* data = &data_[ 0 ];
    result = true;

    for ( int variable = 0; AND2( result, variable < variables_); ++variable) {
      const size_t dataOffset =
        variable * timesteps_ * cells + firstReadTimestep * cells;
      CHECK2( dataOffset <
                variables_ * ( timesteps_ - BUFFERED_DATA_TIMESTEPS ) * cells,
              dataOffset + readWords <= variables_ * timesteps_ * cells );
      const size_t seekOffset = dataFileOffset_ + dataOffset * bytesPerWord;
      result = FSEEK( file, seekOffset, SEEK_SET ) == 0;

      if ( result ) {
        CHECK( data <= &data_[ data_.size() - readWords ] );
        const size_t wordsRead = fread( data, bytesPerWord, readWords, file );
        result = wordsRead == readWords;

        /*
         * If a failed read occurs with partial float words read into
         * data then zero-out the double words damaged by the bad
         * partial read:
         */

        if ( AND3( bytesPerWord == 4, wordsRead, wordsRead < readWords ) ) {
          damagedDoubles = wordsRead / 2 + wordsRead % 2;
          memset( data, 0, damagedDoubles * sizeof (double) );
        }

        if ( result ) {

          if ( bytesPerWord == 4 ) {
            reverse4ByteWordsIfLittleEndian( data, readWords );
            expand4ByteWords( data, readWords );
          } else {
            reverse8ByteWordsIfLittleEndian( data, readWords );
          }

          if ( variableNames_[ variable ] == "ELEVATION" ) {
            elevationDataIndex_ = 0;
            computeElevations( data ); //x compute cell bot/top in elevations_.
          }

          data += readWords;
          DEBUG( fprintf( stderr,
                          "loaded %" LU " values "
                          "from file at offset = %" LU ".\n",
                          readWords, seekOffset ); )
        }
      }
    }

    if ( result ) {

      if ( checkAll ) {
        result = allFinite( &data_[ 0 ], data_.size() ); // Check all values.
      } else {
        result = isValidData(); // Check only sample values (for speed).
      }
    }
  }

  const size_t lastTimestep = firstTimestep + BUFFERED_DATA_TIMESTEPS - 1;

  if ( ! result ) {
    fprintf( stderr,
             "\nI'm sorry, Failed to open, seek and read valid CMAQ data from "
             " %s for timesteps [%" LU ", %" LU "]",
             fileName_.c_str(), firstTimestep, lastTimestep );

    if ( damagedDoubles ) {
      fprintf( stderr,
              " (and %" LU " values were zeroed due to partial file read)",
              damagedDoubles );
    }

    fputs( ".\n", stderr );
  } else {
    firstDataTimestep_ = firstTimestep;
    lastDataTimestep_  = lastTimestep;
  }

  POST0( isValidData() );
  return result;
}



/******************************************************************************
PURPOSE: readXDRData - Read XDR data from a stream.
INPUTS:  FILE* stream  Stream to read from.
         const bool readAndSkipLonLats  Read and skip longitudes, latitudes?
OUTPUTS: data_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void CMAQDataset::readXDRData( FILE* stream, const bool readAndSkipLonLats ) {
  PRE028( stream,
          AND3( isValidName(),
                isValidDescription(),
                isValidVariableNamesAndUnits() ),
          variables_ > 1,
          dataVariable_ == variables_ - 1,
          GT_ZERO4( timesteps_, layers_ , rows_, columns_ ),
          startingTimestamp_.invariant(),
          endingTimestamp_.invariant(),
          startingTimestamp_ < endingTimestamp_,
          cellType_== HEXAHEDRON,
          hasElevation_ == true,
          hasArea_ == true,
          hasVolume_ == true,
          isStationary_ == true,
          isGroup_ == false,
          isStructuredGrid_ == true,
          subsetIndices_[ TIME   ][ LAST  ] >= subsetIndices_[ TIME  ][FIRST],
          subsetIndices_[ LAYER  ][ FIRST ] >= 1,
          subsetIndices_[ LAYER  ][ LAST  ] >= subsetIndices_[ LAYER ][FIRST],
          subsetIndices_[ ROW    ][ FIRST ] >= 1,
          subsetIndices_[ ROW    ][ LAST  ] >= subsetIndices_[ ROW   ][FIRST],
          subsetIndices_[ COLUMN ][ FIRST ] >= 1,
          subsetIndices_[ COLUMN ][ LAST  ] >= subsetIndices_[ COLUMN][FIRST],
          timesteps_ == 1 +
            subsetIndices_[ TIME ][ LAST ] - subsetIndices_[ TIME ][ FIRST ],
          layers_ == 1 +
            subsetIndices_[ LAYER ][ LAST ] - subsetIndices_[ LAYER ][ FIRST],
          rows_ == 1 +
            subsetIndices_[ ROW ][ LAST ] - subsetIndices_[ ROW ][FIRST],
          columns_ == 1 +
            subsetIndices_[ COLUMN ][ LAST ] - subsetIndices_[ COLUMN][FIRST],
          gridParameters_.invariant(),
          IMPLIES( mapProjection_, mapProjection_->invariant() ) );

  /* Data looks like one of these:
  # IEEE-754 32-bit reals data[variables][timesteps][layers][rows][columns]
  # IEEE-754 64-bit reals data[variables][timesteps][layers][rows][columns]
  */

  // Create data_ and decide whether or not to use buffering:

  const size_t cells = layers_ * rows_ * columns_;
  const size_t count0 = variables_ * timesteps_ * cells;
  const size_t dataSize = count0 * sizeof (double);
  const size_t bytesPerWord = is64bit_ ? 8 : 4;
  const bool useBuffering =
    AND4( ! fileName_.empty(),
          timesteps_ > BUFFERED_DATA_TIMESTEPS,
          timesteps_ % BUFFERED_DATA_TIMESTEPS == 0,
          dataSize > PAGE_THREHOLD_GB );
  const size_t count =
    useBuffering ? variables_ * BUFFERED_DATA_TIMESTEPS * cells : count0;
  data_.resize( count ); //x
  void* vdata = (void*) &data_[ 0 ];
  bool ok = true;

  DEBUG( fprintf( stderr,
                  "CMAQDataset::readXDR(): useBuffering = %d, is64bit_ = %d, "
                  "count = %" LU ", readAndSkipLonLats = %d\n",
                  useBuffering, is64bit_, count, readAndSkipLonLats ); )

  if ( readAndSkipLonLats ) { // Read and skip longitude, latitude float data:

    for ( size_t timestep = 0; AND2( ok, timestep < timesteps_ ); ++timestep) {
      ok = fread( vdata, bytesPerWord, cells, stream ) == cells;
      ok = AND2( ok, fread( vdata, bytesPerWord, cells, stream ) == cells );
    }
  }

  if ( ok ) {

    if ( useBuffering ) {
      CHECK( ! fileName_.empty() );
      const long long offset = FTELL( stream );
      ok = offset > 0;

      if ( ok ) {
        dataFileOffset_ = offset;
        ok = readBufferedDataSubset( 0, true );
        CHECK( IMPLIES( ok,
                        AND2( firstDataTimestep_ == 0,
                              lastDataTimestep_ ==
                              BUFFERED_DATA_TIMESTEPS - 1 ) ) );
      }
    } else { // Read all variables float data and expand to double:
      ok = fread( vdata, bytesPerWord, count, stream ) == count;

      if ( ok ) {

        if ( is64bit_ ) {
          reverse8ByteWordsIfLittleEndian( vdata, count );
        } else {
          reverse4ByteWordsIfLittleEndian( vdata, count );
          expand4ByteWords( &data_[ 0 ], count );
        }

        ok = allFinite( &data_[ 0 ], count );

        if ( AND2( ok, variableNames_[ 0 ] == "ELEVATION" ) ) {

          // Compute elevations_:

          double minimum = 0.0;
          double maximum = 0.0;
          range( &data_[ 0 ], cells, minimum, maximum );
          ok = AND2( isElevation( minimum ), isElevation( maximum ) );

          if ( ok ) {
            elevationDataIndex_ = 0;
            computeElevations( &data_[ 0 ] ); //x Compute cell bot/top elev.
          }
        }
      }
    }

    if ( ok ) {

      if ( mapProjection_ ) { // Create longitudes_ and latitudes_ from grid:
        computeLongitudesLatitudes(); //x
      }

      computeBounds();
      computeVariableRanges(); //x Only computes range of loaded data.

      if ( useBuffering ) { // Compute range over rest of timesteps:
        std::vector<double> minimums( minimums_ ); //x
        std::vector<double> maximums( maximums_ ); //x

        for ( size_t timestep = 1; AND2( ok, timestep < timesteps_ );
              ++timestep ) {
          ok = readBufferedDataSubset( timestep, false );

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
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid CMAQ data." ); //x
  }

  POST012( layers_ > 0,
           rows_ > 0,
           columns_ > 0,
           coordinates_.size() == 0,
           IMPLIES_ELSE( mapProjection_,
                         longitudes_.size() == ( rows_ + 1 ) * ( columns_ + 1 ),
                         longitudes_.size() == 0 ),
           latitudes_.size() == longitudes_.size(),
           cellType_== HEXAHEDRON,
           IMPLIES_ELSE( variableNames_[ 0 ] == "ELEVATION",
                        elevations_.size() ==
                          ( layers_ + 1 ) * rows_ * columns_,
                        elevations_.size() == 0 ),
           data_.size() ==
             ( dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_ )
             * variables_ * layers_ * rows_ *columns_,
           isValidCoordinates(),
           isValidData(),
           isValidRange() );
}



/******************************************************************************
PURPOSE: computeElevations - Compute elevation of each grid cell bottom/top.
INPUTS:  const double cellCenterElevations[ layers_ * rows_ * columns_ ]
         subsetIndices_[ LAYER ][ FIRST ],
         layers_, rows_, columns_,
         data_  data_[layers_][rows_][columns_]  Cell center elevations.
OUTPUTS: elevations_  Cell bottom/top elevations.
                      elevations_[layers_+1][rows_][columns_]
NOTES:   Throws exception upon failure to allocate elevations.
******************************************************************************/

void CMAQDataset::computeElevations( const double cellCenterElevations[] )
  const {
  PRE06( cellCenterElevations,
         layers_ > 0,
         rows_ > 0,
         columns_ > 0,
         isElevation( emvl::minimum( cellCenterElevations,
                                     layers_ * rows_ * columns_ ) ),
         isElevation( emvl::maximum( cellCenterElevations,
                                     layers_ * rows_ * columns_ ) ) );

  const size_t layerCells = rows_ * columns_;
  elevations_.resize( ( layers_ + 1 ) * layerCells ); //x

  if ( layers_ == 1 ) {

    /*
     * Given only one layer of cell centers, compute approximate cell
     * thicknesses using matched CMAQ grid ZF values.
     * Use subset layer number to make cell about 20m thick if near surface to
     * about 4,000m thick if near the top - depending on the CMAQ grid.
     * The following command gets the west-south (ocean) grid cell ZF and
     * computes the cell thicknesses.
     *  /rsig/current/code/bin/Linux.x86_64/CMAQSubset_new \
     *  -files METCRO3D -time 2017010100 -row 1 -column 1 -variable ZF > junk
     * ncdump junk | \
     * awk '/ZF =/{getline; while ($1 != "}") {printf "%f\n", $1; getline}}' |\
     * awk '{if (NR == 1) {print; below = $1} else {print $1 - below; below = $1}}'
     */
    const int CONUS_LAYERS = 35; // EQUATES CONUS run layer thicknesses.
    const double conusLayerThicknesses[ CONUS_LAYERS ] = { // Meters.
      20.76379,
      20.7927,
      41.6969,
      41.8517,
      42.0095,
      84.4966,
      85.1478,
      85.8083,
      86.4816,
      87.1724,
      87.919,
      88.7949,
      89.6664,
      181.799,
      185.188,
      189.151,
      193.669,
      197.448,
      304.124,
      314.225,
      435.418,
      573.337,
      609.226,
      648.356,
      691.696,
      742.125,
      806.262,
      886.498,
      980.315,
      1089.83,
      1227.5,
      1416.31,
      1698.99,
      2283.58,
      3817.51
    };
    const int HEMI_LAYERS = 44; // EQUATES HEMI run layer thicknesses.
    const double hemiLayerThicknesses[ HEMI_LAYERS ] = { // Meters.
      21.47592,
      24.945,
      28.4448,
      32.8371,
      38.1328,
      43.4807,
      50.6369,
      57.8858,
      67.8895,
      78.0696,
      90.2396,
      104.5,
      121.878,
      141.639,
      164.867,
      192.152,
      225.757,
      265.679,
      313.981,
      374.223,
      407.433,
      424.345,
      442.69,
      463.931,
      484.636,
      509.129,
      537.115,
      569.194,
      605.649,
      646.704,
      660.447,
      673.424,
      688.459,
      703.77,
      727.25,
      747.9,
      780.65,
      815.28,
      861.88,
      928.83,
      1025.67,
      1180.32,
      1430.78,
      1792.2
    };
    const int nlays = gridParameters_.nlays();
    const int thicknessLayers =
      nlays == HEMI_LAYERS ? HEMI_LAYERS : CONUS_LAYERS;
    const double* const layerThicknesses =
      nlays == HEMI_LAYERS ? &hemiLayerThicknesses[ 0 ]
      : &conusLayerThicknesses[ 0 ];
    const int subsetLayer = subsetIndices_[ LAYER ][ FIRST ];
    const int layer = std::min<int>( subsetLayer, thicknessLayers ) - 1;
    const double layerThickness = layerThicknesses[ layer ];
    const double halfThickness = layerThickness * 0.5;

    for ( size_t cell = 0; cell < layerCells; ++cell ) {
      const size_t aboveCell = layerCells + cell;
      CHECK2( cell < elevations_.size(), aboveCell < elevations_.size() );
      const double centerElevation = cellCenterElevations[ cell ];
      const double belowElevation = centerElevation - halfThickness;
      const double aboveElevation = centerElevation + halfThickness;
      CHECK2( isElevation( belowElevation ), isElevation( aboveElevation ) );
      elevations_[ cell      ] = belowElevation;
      elevations_[ aboveCell ] = aboveElevation;
    }

  } else { // Compute from adjacent layers:

    // Compute surface elevations from 1st & 2nd layer cell center elevations:

    for ( size_t cell = 0; cell < layerCells; ++cell ) {
      const size_t aboveCell = layerCells + cell;
      CHECK2( cell      < layers_ * rows_ * columns_,
              aboveCell < layers_ * rows_ * columns_ );
      const double centerElevation      = cellCenterElevations[ cell ];
      const double aboveCenterElevation = cellCenterElevations[ aboveCell ];
      const double halfHeight = 0.5 * (aboveCenterElevation - centerElevation);
      const double surfaceElevation = centerElevation - halfHeight;
      CHECK2( isElevation( surfaceElevation ), cell < elevations_.size() );
      elevations_[ cell ] = surfaceElevation;
    }

    // Compute cell top elevations from cell bottom elevation and cell centers:

    for ( size_t layer = 0; layer < layers_; ++layer ) {
      const size_t layerOffset = layer * layerCells;
      const size_t aboveLayerOffset = layerOffset + layerCells;

      for ( size_t cell = 0; cell < layerCells; ++cell ) {
        const size_t index = layerOffset + cell;
        const size_t aboveIndex = aboveLayerOffset + cell;
        CHECK2( index < elevations_.size(),
                index < layers_ * rows_ * columns_ );
        const double bottomElevation = elevations_[ index ];
        const double centerElevation = cellCenterElevations[ index ];
        const double topElevation =
          centerElevation + centerElevation - bottomElevation;
        CHECK2( isElevation( topElevation ),
                aboveIndex < elevations_.size() );
        elevations_[ aboveIndex ] = topElevation;
      }
    }
  }

  POST03( elevations_.size() ==
            ( layers_ + 1 ) * rows_ * columns_,
          isElevation( emvl::minimum( &elevations_[ 0 ], elevations_.size())),
          isElevation( emvl::maximum( &elevations_[ 0 ], elevations_.size())));
}



/******************************************************************************
PURPOSE: computeLongitudesLatitudes - Compute longitude-latitude coordinates
         of each surface cell vertex.
INPUTS:  rows_, columns_, gridParameters_, mapProjection_
OUTPUTS: longitudes_, latitudes_
NOTES:   Throws exception upon failure to allocate longitudes_ and latitudes_.
******************************************************************************/

void CMAQDataset::computeLongitudesLatitudes() {
  PRE05( rows_ > 0,
         columns_ > 0,
         gridParameters_.invariant(),
         mapProjection_,
         mapProjection_->invariant() );

  const size_t rows1 = rows_ + 1;
  const size_t columns1 = columns_ + 1;
  const size_t points = rows1 * columns1;
  longitudes_.reserve( points ); //x
  latitudes_.reserve( points ); //x
  const double xcell = gridParameters_.xcell();
  const double ycell = gridParameters_.ycell();
  const double xorig =
    gridParameters_.xorig() + ( subsetIndices_[ COLUMN ][ FIRST] - 1 ) * xcell;
  const double yorig =
    gridParameters_.yorig() + ( subsetIndices_[ ROW ][ FIRST ] - 1 ) * ycell;

  /*
   * Compute surface vertices by unprojecting cell corners:
   */

  for ( size_t row = 0; row < rows1; ++row ) {
    const double y = yorig + row * ycell;

    for ( size_t column = 0; column < columns1; ++column ) {
      const double x = xorig + column * xcell;
      double longitude = x;
      double latitude  = y;

      if ( mapProjection_ ) {
        mapProjection_->unproject( x, y, longitude, latitude );
      }

      longitudes_.push_back( longitude );
      latitudes_.push_back( latitude );
    }
  }

  POST0( isValidCoordinates() );
}



/******************************************************************************
PURPOSE: computeCMAQGridElevations - Compute elevation of each grid cell center
INPUTS:  elevations_[layers_][rows_][columns_]  Cell center elevations.
OUTPUTS: std::vector<float>& elevations  Cell center elevations.
NOTES:   Throws exception upon failure to allocate elevations.
******************************************************************************/

void CMAQDataset::computeCMAQGridElevations(
                                      std::vector<float>& elevations ) const {
  PRE( elevations_.size() == ( layers_ + 1 ) * rows_ * columns_ );
  elevations.clear();
  elevations.resize( layers_ * rows_ * columns_ ); //x
  const size_t layerCells = rows_ * columns_;

  for ( size_t layer = 0, cell = 0; layer < layers_; ++layer ) {

    for ( size_t layerCell = 0; layerCell < layerCells; ++layerCell, ++cell ) {
      CHECK2( cell < elevations.size(),
              cell + layerCells < elevations_.size() );
      const double bottomElevation = elevations_[ cell ];
      const double topElevation = elevations_[ cell + layerCells ];
      const double centerElevation = 0.5 * ( bottomElevation + topElevation );
      elevations[ cell ] = centerElevation;
    }
  }
}



/******************************************************************************
PURPOSE: writeXDR - Write XDR format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
         This routine does not simply copy the file bytes to stream, but rather
         writes a fully-expanded XDR format including time-stepped
         LONGITUDE, LATITUDE, ELEVATION variables.
******************************************************************************/

void CMAQDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

    SUBSET 9.0 CMAQ
    C_12_OAQPS
    https://www.epa.gov/ttn/scram/,EPA-RTP,CMAQSubset
    2006-08-28T00:00:00-0000
    # data dimensions: timesteps variables layers rows columns:
    24 4 24 13 12
    # subset indices (0-based time, 1-based layer/row/column): first-timestep \
     last-timestep first-layer last-layer first-row last-row first-column \
     last-column:
     0 23 1 24 43 55 139 150
    # Variable names:
    LONGITUDE LATITUDE ELEVATION O3
    # Variable units:
    deg deg m ppmV
    # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
    33 45 40 -97 6.37e+06 6.37e+06
    # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
    279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
      0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
      0.4 0.3 0.2 0.1 0
    # IEEE-754 32-bit reals data[variables][timesteps][layers][rows][columns]:

  */

  Timestamp::UTCString utcTimestamp = "";
  startingTimestamp_.toUTCString( utcTimestamp );
  const int outputVariables = is64bit_ ? variables_ : 2 + variables_;
  const char* const format1 =
    "SUBSET 9.0 CMAQ\n"
    "%s\n"
    "%s\n"
    "%s\n"
    "# data dimensions: timesteps variables layers rows columns:\n"
    "%" LU " %d %" LU " %" LU " %" LU "\n"
    "# subset indices (0-based time, 1-based layer/row/column): "
    "first-timestep last-timestep first-layer last-layer first-row last-row "
    "first-column last-column:\n"
    "%" LU " %" LU " %" LU " %" LU " %" LU " %" LU " %" LU " %" LU "\n";
  bool ok =
    fprintf( stream, format1,
             gridName_.c_str(),
             description_.c_str(),
             utcTimestamp,
             timesteps_, outputVariables, layers_, rows_, columns_,
             subsetIndices_[ TIME ][ FIRST ],
             subsetIndices_[ TIME ][ LAST ],
             subsetIndices_[ LAYER ][ FIRST ],
             subsetIndices_[ LAYER ][ LAST ],
             subsetIndices_[ ROW ][ FIRST ],
             subsetIndices_[ ROW ][ LAST ],
             subsetIndices_[ COLUMN ][ FIRST ],
             subsetIndices_[ COLUMN ][ LAST ] ) > (int) strlen( format1 );

  if ( ok ) { // Write variable names:
    const int firstVariable = variableNames_[ 0 ] == "ELEVATION" ? 1 : 0;
    const char* const format2 =
      is64bit_ ? "# Variable names:\n"
      : "# Variable names:\nLONGITUDE LATITUDE ELEVATION ";
    ok = fprintf( stream, format2 ) == (int) strlen( format2 );

    for ( int variable = firstVariable; AND2( ok, variable < variables_ );
          ++variable ) {
      const char* const name = variableNames_[ variable ].c_str();
      const char* const blank = variable + 1 < variables_ ? " " : "";
      ok = fprintf( stream, "%s%s", name, blank )
           == (int) ( strlen( name ) + strlen( blank ) );
    }

    if ( ok ) { // Write variable units:
      const char* const format3 =
        is64bit_ ? "\n# Variable units:\n"
        : "\n# Variable units:\ndeg deg m";
      ok = fprintf( stream, format3 ) == (int) strlen( format3 );

      for ( int variable = firstVariable; AND2( ok, variable < variables_ );
            ++variable) {
        const char* const units = variableUnits_[ variable ].c_str();
        const char* const blank = variable + 1 < variables_ ? " " : "";
        ok = fprintf( stream, "%s%s", units, blank )
             == (int) ( strlen( units ) + strlen( blank ) );
      }

      ok = AND2( ok, fprintf( stream, "\n" ) == 1 );

      if ( ok ) {
        writeProjectionAndGrid( stream, gridParameters_ ); //x

        if ( is64bit_ ) {
          const char* const end =
            "# IEEE-754 64-bit reals "
            "data[variables][timesteps][layers][rows][columns]:\n";
          ok = fprintf( stream, "%s", end ) == (int) strlen( end );
        } else {
          const char* const end =
            "# IEEE-754 32-bit reals "
            "data[variables][timesteps][layers][rows][columns]:\n";
          ok = fprintf( stream, "%s", end ) == (int) strlen( end );
        }

        if ( ok ) {
          const size_t cells = layers_ * rows_ * columns_;
          const size_t bufferSize = cells;
          std::vector<float> buffer; //x

          if ( ! is64bit_ ) { // Copy and write coordinates as 32-bit reals:
            buffer.resize( bufferSize ); //x

            // Compute, copy and write grid cell center coordinates:

            for ( int coordinate = LONGITUDE;
                  AND2( ok, coordinate <= ELEVATION - firstVariable );
                  ++coordinate ) {
              copyCellCenterCoordinates( coordinate, &buffer[ 0 ] );
              reverse4ByteWordsIfLittleEndian( &buffer[ 0 ], bufferSize );

              for ( size_t timestep = 0; AND2( ok, timestep < timesteps_ );
                    ++timestep ) {
                ok =
                  fwrite( &buffer[ 0 ], sizeof buffer[ 0 ], bufferSize, stream )
                  == bufferSize;
              }
            }
          }

          // Write copy of data variables:

          const size_t loadedTimesteps =
            dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
          const size_t variableSize = loadedTimesteps * cells;

          for ( int variable = 0; AND2( ok, variable < variables_ );
                ++variable ) {
            const float minimum = this->minimum( variable );
            const float maximum = this->maximum( variable );
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
            const size_t variableOffset = variable * variableSize;

            for ( size_t timestep = 0; AND2( ok, timestep < timesteps_ );
                  ++timestep ) {
              const size_t loadedTimestep =
                dataFileOffset_ ? timestep % BUFFERED_DATA_TIMESTEPS : timestep;
              const size_t timestepOffset = loadedTimestep * cells;
              const size_t index = variableOffset + timestepOffset;
              CHECK( index < data_.size() );
              ok = loadedDataSubset( timestamp, timestamp );

              if ( is64bit_ ) {
                reverse8ByteWordsIfLittleEndian( &data_[ index ], cells );
                ok = fwrite( &data_[ index ], sizeof (double), cells, stream )
                     == cells;
                reverse8ByteWordsIfLittleEndian( &data_[ index ], cells );
              } else {
                ok = AND2( ok, writeConvertedArray( stream,
                                                    &buffer[ 0 ], bufferSize,
                                                    minimum, maximum,
                                                    &data_[ index ] ) );
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
    throw std::runtime_error( "Failed to write XDR format CMAQ data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CMAQDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  LONGITUDE(deg)  LATITUDE(deg)   ELEVATION(m)      O3(ppmV)
   2005-08-29T22:00:00-0000  -9.103617095947265625e+01  \
     3.100816154479980469e+01  9.984109497070312500e+01  \
     4.548812285065650940e-05
   2005-08-29T22:00:00-0000  -9.091098785400390625e+01  \
     3.100103569030761719e+01  1.197701492309570312e+02  \
     3.958785600843839347e-05
   ...

  */

  // Write header row:

  const char* const format =
  is64bit_ ?
    "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)"
    : "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tELEVATION(m)";
  bool ok = fprintf( stream, "%s", format ) == (int) strlen( format );
  const char* const headerFormat = "\t%s(%s)";
  const int firstVariable = variableNames_[ 0 ] == "ELEVATION" ? 1 : 0;

  for ( int variable = firstVariable; AND2( ok, variable < variables_ );
        ++variable ) {
    ok = fprintf( stream, headerFormat,
                  variableNames_[ variable ].c_str(),
                  variableUnits_[ variable ].c_str() ) > 4;
  }

  ok = AND2( ok, fprintf( stream, "\n" ) == 1 );

  if ( ok ) { // Write data[V][T][L][R][C] as data[T][L][R][C][V]:
    const char* const dataFormat = "\t%28.18le";
    const size_t wordSize = 48;
    char word[ wordSize ] = "";
    const size_t layerSize  = rows_ * columns_;
    const size_t bufferSize = layerSize * (4 + variables_) * wordSize + 1;
    std::vector<char> buffer( bufferSize + 1 ); //x
    const size_t timestepSize = layers_ * layerSize;
    const size_t bufferedTimesteps =
      dataFileOffset_ ? BUFFERED_DATA_TIMESTEPS : timesteps_;
    const size_t variableSize = bufferedTimesteps * timestepSize;
    Timestamp timestamp( startingTimestamp_ );
    const int hoursPerTimestep =
      timestepSize_ == HOURS ? 1
      : timestepSize_ == DAYS ? 24
      : 0;
    const int monthsPerTimestep =
      timestepSize_ == MONTHS ? 1
      : timestepSize_ == TRIMONTHS ? 3
      : timestepSize_ == YEARS ? 12
      : 0;
    const size_t cells = layers_ * rows_ * columns_;
    std::vector<double> longitudes( cells ); //x
    std::vector<double> latitudes(  cells ); //x
    copyOutLongitudes( longitudes );
    copyOutLatitudes(  latitudes  );

    for ( size_t timestep = 0; AND2( ok, timestep < timesteps_ ); ++timestep) {
      ok = loadedDataSubset( timestamp, timestamp );

      if ( ok ) {
        const size_t bufferedTimestep = timestep % bufferedTimesteps;
        Timestamp::UTCString utcString = "";
        timestamp.toUTCString( utcString );

        // Write one layer worth of buffered data (for performance):

        for ( size_t layer = 0; AND2( ok, layer < layers_ ); ++layer ) {
          buffer.assign( buffer.size(), '\0' );
          char* outputBuffer = &buffer[ 0 ];
          const size_t coordinateOffset = layer * layerSize;

          for ( size_t cell = 0; cell < layerSize; ++cell ) {
            strncpy( outputBuffer, utcString, Timestamp::UTC_STRING_LENGTH );
            outputBuffer += Timestamp::UTC_STRING_LENGTH;
            const size_t coordinateIndex = coordinateOffset + cell;
            CHECK2( coordinateIndex < longitudes.size(),
                    coordinateIndex < latitudes.size() );
            const double longitude = longitudes[ coordinateIndex ];
            const double latitude  = latitudes[  coordinateIndex ];
            size_t count =
              snprintf( word, sizeof word / sizeof *word, dataFormat,
                        longitude );
            CHECK2( count < sizeof word / sizeof *word, strlen(word) == count);
            strncpy( outputBuffer, word, count );
            outputBuffer += count;
            count =
              snprintf( word, sizeof word / sizeof *word, dataFormat, latitude);
            CHECK2( count < sizeof word / sizeof *word, strlen(word) == count);
            strncpy( outputBuffer, word, count );
            outputBuffer += count;

            for ( int variable = 0; variable < variables_; ++variable ) {
              const size_t dataIndex =
                variable * variableSize + bufferedTimestep * timestepSize +
                layer * layerSize + cell;
              CHECK( dataIndex < data_.size() );
              count = snprintf( word, sizeof word / sizeof *word, dataFormat,
                                data_[ dataIndex ] );
              CHECK2(count < sizeof word / sizeof *word, strlen(word) ==count);
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
      }

      if ( hoursPerTimestep ) {
        timestamp.offsetHours( hoursPerTimestep );
      } else {
        timestamp.offsetMonths( monthsPerTimestep );
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write ASCII format CMAQ data." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CMAQDataset::writeCOARDS( const std::string& fileName ) const {
  PRE( fileName.length() );
  const size_t fileSizeEstimate =
    3 * layers_ * rows_ * columns_ * 4 +
    timesteps_ * variables_ * layers_ * rows_ * columns_ * 4 +
    timesteps_ * 3 * 4 + 10000;
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    const int dimensions = layers_ > 1 ? 4 : 3;
    const char* names[ 4 ] = { "time", "elevation", "latitude", "longitude" };
    int ids[ 4 ] = { -1, -1, -1, -1 };
    size_t sizes[ 4 ] = { timesteps_, layers_, rows_, columns_ };

    // If 2D surface grid then omit elevation dimension:

    if ( dimensions == 3 ) {
      names[ 1 ] = names[ 2 ];
      names[ 2 ] = names[ 3 ];
      names[ 3 ] = 0;
      sizes[ 1 ] = sizes[ 2 ];
      sizes[ 2 ] = sizes[ 3 ];
      sizes[ 3 ] = 0;
    }

    if ( createNetCDFDimensions( file, dimensions, names, sizes, ids ) ) {

      if ( writeNetCDFTextAttribute( file, NC_GLOBAL, "grid",
                                     gridName_.c_str() ) ) {
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

        /*
         * writeCOARDSCommon() creates/writes the following variables:
         *
         * if dimensions == 4:
         *
         * double longitude(latitude, longitude) ;
         * double latitude(latitude, longitude) ;
         * double elevation(elevation, latitude, longitude) ;
         * int yyyymmdd(time) ;
         * int hhmmss(time) ;
         * double O3(time, elevation, latitude, longitude) ;
         * float time(time) ;
         *       time:units = "hours since 2008-07-03 00:00:00.0 -00:00" ;
         *
         * if dimensions == 3:
         *
         * double longitude(longitude) ;
         * double latitude(latitude) ;
         * int yyyymmdd(time) ;
         * int hhmmss(time) ;
         * double O3(time, latitude, longitude) ;
         * float time(time) ;
         *       time:units = "hours since 2008-07-03 00:00:00.0 -00:00" ;
         */

        const int coordinateRank = mapProjection_ ? 2 : 1;
        const int skipVariables = variableNames_[ 0 ] == "ELEVATION" ? 1 : 0;
        const bool withElevation = dimensions == 4;
        writeCOARDSCommon( file, skipVariables, false, false, withElevation,
                           true, coordinateRank, dimensions, ids, sizes, names,
                           timestamps ); //x
        ok = true;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create "
                              "NetCDF-COARDS header for CMAQ data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeIOAPI - Write IOAPI-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CMAQDataset::writeIOAPI( const std::string& fileName ) const {
  PRE( fileName.length() );
  const size_t outputVariables = 2 + variables_;
  // LONGITUDE, LATITUDE, ELEVATION, O3, ...
  const size_t fileSizeEstimate =
    outputVariables * timesteps_ * layers_ * rows_ * columns_ * 4 +
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
    writeIOAPIHeader( file, false, true, 0,
                      (int) layers_, (int) rows_, (int) columns_,
                      gridParameters_, subsetIndices_, 0, dimensionIds ); //x
    writeIOAPIData( file ); //x
    ok = true;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create "
                              "NetCDF-IOAPI file for CMAQ data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



/******************************************************************************
PURPOSE: writeIOAPIData - Write IOAPI-format data.
INPUTS:  const int file  NetCDF file to write to.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CMAQDataset::writeIOAPIData( const int file ) const {

  PRE( file > -1 );

  const size_t outputVariables = 2 + variables_ ;
  // LONGITUDE, LATITUDE, ELEVATION, O3, ...
  writeIOAPITFLAGVariable( file, outputVariables ); //x
  const size_t dimensions[ 4 ] = { timesteps_, layers_, rows_, columns_ };

  writeIOAPICoordinates( file );

  if ( dataFileOffset_ ) {
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

    writeBufferedIOAPIVariables( file, 0, 0, 4, dimensions, timestamps ); //x
  } else {
    writeIOAPIVariables( file, 0, 0, dimensions ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeIOAPICoordinates - Write IOAPI-format longitude, latitude to file
INPUTS:  const int file  NetCDF file to write to.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CMAQDataset::writeIOAPICoordinates( const int file ) const {
  PRE( file > 0 );

  const size_t layers =
    1 + subsetIndices_[ LAYER ][ LAST ] - subsetIndices_[ LAYER ][ FIRST ];
  const size_t rows =
    1 + subsetIndices_[ ROW ][ LAST ] - subsetIndices_[ ROW ][ FIRST ];
  const size_t columns =
    1 + subsetIndices_[ COLUMN ][ LAST ] - subsetIndices_[ COLUMN ][ FIRST ];
  const size_t sizes[ 4 ] = { 1, layers, rows, columns };

  std::vector<float> longitudes;
  std::vector<float> latitudes;

  computeCMAQGridLongitudesLatitudes( gridParameters_, subsetIndices_,
                                      mapProjection_,
                                      longitudes, latitudes ); //x

  // For each timestep, write the same coordinates:

  bool ok = true;

  for ( size_t timestep = 0; AND2( ok, timestep < timesteps_ ); ++timestep ) {

    // Write longitudes:

    ok = writeNetCDFSomeFloatData( file, "LONGITUDE", timestep, sizes,
                                   &longitudes[ 0 ] );

    if ( ok ) { // Write latitudes:
      ok = writeNetCDFSomeFloatData( file, "LATITUDE",  timestep, sizes,
                                     &latitudes[ 0 ] );
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write IOAPI-format coordinates.");//x
  }
}



/******************************************************************************
PURPOSE: writeKML - Write KML-format data.
NOTES:   Throws exception upon failure.
******************************************************************************/

void CMAQDataset::writeKML() const {
  PRE( outputKMLFile_ );
  const char* const variable =
    isVector_ ? "Wind" : variableNames_[ dataVariable_ ].c_str();
  std::string source( name_ ); //x
  const size_t variableIndex = source.rfind( variable );

  if ( variableIndex != std::string::npos && variableIndex > 1 ) {
    source.erase( variableIndex - 1 );
  }

  const char* const units = variableUnits_[ dataVariable_ ].c_str();
  const size_t vertexCount = cellVertexCount();
  const char* const dataType =
    isVector_ ? "Vector" : vertexCount == 4 ? "Quadrilateral" : "Hexahedron";
  const char* const description =
    isVector_ ? "Modeled gridded vector data."
    : "Modeled gridded scalar data.";
  DataColor dataColorFunction =
    outputKMLDataColorFunction_ ? outputKMLDataColorFunction_ : dataColor;

  writeStartFolderKML( outputKMLFile_, source.c_str(), variable, units,
                       dataType, description );

  const int components = isVector_ ? 3 : 1;
  const double variableMinimum = minimum();
  const double variableMaximum = maximum();
  const size_t cells = layers_ * rows_ * columns_;
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

  for ( size_t timestep = 0; AND2( ok, timestep < timesteps_ ); ++timestep ) {
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
      const size_t loadedTimestepsCells = loadedTimesteps * cells;
      const size_t loadedTimestepsCells2 =
        loadedTimestepsCells + loadedTimestepsCells;
      const size_t timestepOffset = loadedTimestep * cells;

      // Update elevations_ from data_ for cellVertices():

      if ( AND2( timestepOffset != elevationDataIndex_,
                 variableNames_[ 0 ] == "ELEVATION" ) ) {
        elevationDataIndex_ = timestepOffset;
        CHECK( elevationDataIndex_ < data_.size() );
        computeElevations( &data_[ elevationDataIndex_ ] ); //x cell bot/top.
      }

      for ( size_t cell = 0; cell < cells; ++cell ) {
        const size_t index = timestepOffset + cell;
        CHECK( index < data_.size() );
        double value = data_[ index ];

        if ( value > MISSING ) {
          double value2 = MISSING;
          double value3 = MISSING;
          double magnitude = MISSING;

          if ( ! isVector_ ) {
            magnitude = value;
          } else if ( components == 2 ) {
            const size_t index2 = loadedTimestepsCells + index;
            CHECK2( index2 > index, index2 < data_.size() );
            value2 = data_[ index2 ];

            if ( value2 > MISSING ) {
              magnitude = sqrt( value * value + value2 * value2 );
            }
          } else {
            CHECK ( components == 3 );
            const size_t index3 = loadedTimestepsCells2 + index;
            CHECK2( index3 > loadedTimestepsCells + index,
                    index3 < data_.size() );
            value3 = data_[ index3 ];

            if ( value3 > MISSING ) {
              magnitude =
                sqrt( value * value + value2 * value2 + value3 * value3 );
            }
          }

          if ( magnitude > MISSING ) {
            const Color color =
              dataColorFunction( magnitude, variableMinimum, variableMaximum );
            Point computedCellVertices[ 8 ];
            cellVertices( cell, computedCellVertices );
            double cellCoordinates[ 8 * 3 ];

            for (size_t vertex = 0, vertex3 = 0; vertex < vertexCount; ++vertex) {
              CHECK2( vertex <
                      sizeof computedCellVertices / sizeof *computedCellVertices,
                      vertex3 + 2 <
                      sizeof cellCoordinates / sizeof *cellCoordinates );
              cellCoordinates[vertex3++] = computedCellVertices[vertex].longitude;
              cellCoordinates[vertex3++] = computedCellVertices[vertex].latitude;
              cellCoordinates[vertex3++] = computedCellVertices[vertex].elevation;
            }

            writeCellDataToKML( outputKMLFile_, source.c_str(), variable, units,
                                valueTimestamp, components,
                                MISSING, value, value2, value3,
                                vertexCount, cellCoordinates, color );
          }
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
    throw std::runtime_error( "Failed to write CMAQ data to KML." ); //x
  } else {
    writeEndFolderKML( outputKMLFile_ );
  }

  POST( true );
}



} // namespace emvl.



