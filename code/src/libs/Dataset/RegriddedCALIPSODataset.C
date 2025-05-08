/******************************************************************************
PURPOSE: RegriddedCALIPSODataset.C - Dataset for regridded CALIPSO satellite
         data.
HISTORY: 2013-12-14 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fwrite(), fscanf().
#include <string.h> // For memset(), strstr().
#include <limits.h> // For LLONG_MAX.
#include <float.h>  // For FLT_MAX.
#include <unistd.h> // For unlink().

#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <memory>    // For std::auto_ptr.

#include <Assertions.h> // For PRE(), POST(), AND(), IN_RANGE().

#include <DataUtilities.h>           // For isNan(), isLongitude(), sum().
#include <ResourceHandler.h>         // For ResourceHandler.
#include <Bounds.h>                  // For Bounds.
#include <Timestamp.h>               // For Timestamp.
#include <QueryParameters.h>         // For QueryParameters.
#include <CMAQGridParameters.h>      // For CMAQGridParameters.
#include <NetCDFUtilities.h>         // For *NetCDF*() routines.
#include <Dataset.h>                 // For Dataset.
#include <RegriddedDataset.h>        // For RegriddedDataset.
#include <RegriddedCALIPSODataset.h> // For RegriddedCALIPSODataset.

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~RegriddedCALIPSODataset - Destruct a RegriddedCALIPSODataset.
******************************************************************************/

RegriddedCALIPSODataset::~RegriddedCALIPSODataset() {
  PRE( true );
  layers_ = 0;
}



/******************************************************************************
PURPOSE: RegriddedCALIPSODataset - Construct a RegriddedCALIPSODataset from a
         stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

RegriddedCALIPSODataset::RegriddedCALIPSODataset( FILE* stream ) {
  PRE0( stream );

  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = HEXAHEDRON;
  hasElevation_     = true;
  hasArea_          = true;
  hasVolume_        = true;
  isStationary_     = false;
  isGroup_          = false;
  isStructuredGrid_ = false;
  timesteps_        = 0;

  totalPoints_      = 0;

  layers_           = 0;

  readXDR( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded CALIPSO data." );
    //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedCALIPSODataset - Construct a RegriddedCALIPSODataset from a
         file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

RegriddedCALIPSODataset::RegriddedCALIPSODataset(const std::string& fileName) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = HEXAHEDRON;
  hasElevation_     = true;
  hasArea_          = true;
  hasVolume_        = true;
  isStationary_     = false;
  isGroup_          = false;
  isStructuredGrid_ = false;
  timesteps_        = 0;

  totalPoints_      = 0;

  layers_           = 0;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded CALIPSO data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedCALIPSODataset - Copy-construct a RegriddedCALIPSODataset.
INPUTS:  const RegriddedCALIPSODataset& other  Object to copy.
******************************************************************************/

RegriddedCALIPSODataset::RegriddedCALIPSODataset(
                                        const RegriddedCALIPSODataset& other) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    RegriddedDataset::copy( other ); // Copy all inherited attributes.
    layers_ = other.layers_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a RegriddedCALIPSODataset.
INPUTS:  const RegriddedCALIPSODataset& other  Object to copy.
RETURNS: RegriddedCALIPSODataset& *this.
******************************************************************************/

RegriddedCALIPSODataset& RegriddedCALIPSODataset::operator=(
  const RegriddedCALIPSODataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    RegriddedDataset::copy( other ); // Copy all inherited attributes.
    layers_ = other.layers_;
  }

  POST( *this == other );
  return *this;
}



/******************************************************************************
PURPOSE: sample - Sample other dataset at this dataset's coordinates.
INPUTS:  const Dataset& other  Dataset to sample.
RETURNS: Dataset* sampled dataset.
         Throws exception upon failure.
******************************************************************************/

Dataset* RegriddedCALIPSODataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error("UNIMPLEMENTED: RegriddedCALIPSODataset::sample()");
  //x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: regrid - Regrid this dataset's data onto given grid.
INPUTS:  const int regridMethod  REGRID_NEAREST...REGRID_WEIGHTED.
         const CMAQGridParameters& cmaqGridParameters  Grid parameters.
RETURNS: Dataset* regridded dataset.
         Throws exception upon failure.
******************************************************************************/

Dataset* RegriddedCALIPSODataset::regrid( const int regridMethod,
                                          const CMAQGridParameters&
                                          cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error("UNIMPLEMENTED: RegriddedCALIPSODataset::regrid()");
  //x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: write - Write dataset to a directory file in the specified format.
INPUTS:  const std::string& directory  Name of directory to write to.
         const int format              Format of file to write.
NOTES:   The name of the file is determined by the dataset name, variable, etc.
******************************************************************************/

void RegriddedCALIPSODataset::write( const std::string& directory,
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

bool RegriddedCALIPSODataset::invariant() const {
  const bool result =
    AND12( RegriddedDataset::invariant(),
           variables_        == 4,
           cellType_         == HEXAHEDRON,
           hasElevation_     == true,
           hasArea_          == true,
           hasVolume_        == true,
           isStationary_     == false,
           isGroup_          == false,
           isStructuredGrid_ == false,
           isSurfaceData()   == ( layers_ == 1 ),
           totalPoints_      > 0,
           layers_           > 0 );
  return result;
}



/******************************************************************************
PURPOSE: layers - Number of layers.
RETURNS: size_t Number of layers.
******************************************************************************/

size_t RegriddedCALIPSODataset::layers() const {
  const size_t result = layers_;
  POST0( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: isSurfaceData - Is this dataset only on the Earth surface?
RETURNS: bool layers_ == 1.
******************************************************************************/

bool RegriddedCALIPSODataset::isSurfaceData() const {
  const bool result = layers_ == 1;
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool RegriddedCALIPSODataset::operator==(
  const RegriddedCALIPSODataset& other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND2( RegriddedDataset::equal( other ), layers_ == other.layers_ ) );


  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool RegriddedCALIPSODataset::operator!=( const RegriddedCALIPSODataset& other)
  const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: readXDR - Read XDR regridded data from a stream.
INPUTS:  FILE* stream  Stream to read from.
OUTPUTS: name_
         description_
         variableNames_
         startingTimestamp
         variables_
         dataVariable_
         timesteps_
         startingTimestamp_
         endingTimestamp_
         totalPoints_
         pointsPerTimestep_
NOTES:   Throws exception upon failure to read valid data from a stream.
******************************************************************************/

void RegriddedCALIPSODataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read ASCII header that looks like this:

  REGRIDDED-CALIPSO 2.0
  http://eosweb.larc.nasa.gov/PRODOCS/calipso/table_calipso.html,\
  CALIPSOSubset,XDRConvert
  2006-07-05T00:00:00-0000
  # timesteps layers
  24 34
  # Variable name:
  Feature_Optical_Depth_532
  # Variable units:
  -
  # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
  33 45 40 -97 6.37e+06 6.37e+06
  # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
  279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
    0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
    0.4 0.3 0.2 0.1 0
  # MSB 64-bit integers points[timesteps] and
  # IEEE-754 64-bit reals longitudes[timesteps][points] and
  # IEEE-754 64-bit reals latitudes[timesteps][points] and
  # IEEE-754 64-bit reals elevations[timesteps][points][layers] and
  # MSB 64-bit integers columns[timesteps][points] and
  # MSB 64-bit integers rows[timesteps][points] and
  # MSB 64-bit integers layers[timesteps][points][layers] and
  # IEEE-754 64-bit reals data[timesteps][points][layers]:

  */

  readAndMatchLine( stream, "REGRIDDED-CALIPSO 2.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  size_t dimensions[ 2 ] = { 0, 0 };
  readDimensions( stream, sizeof dimensions / sizeof *dimensions,
                  dimensions ); //x
  timesteps_ = dimensions[ 0 ];
  layers_ = dimensions[ 1 ];

  // Prepend row, column and layer variables:

  variableNames_.reserve( 4 ); //x
  variableUnits_.reserve( 4 ); //x
  variableNames_.push_back( "column" ); //x
  variableNames_.push_back( "row" ); //x
  variableNames_.push_back( "layer" ); //x
  variableUnits_.push_back( "-" ); //x
  variableUnits_.push_back( "-" ); //x
  variableUnits_.push_back( "-" ); //x
  variables_ = 3; // column, row, layer.

  // Read variable:

  readVariablesAndUnits( stream, 1 ); //x
  CHECK2( variables_ == 4, dataVariable_ == variables_ - 1 );
  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.
  readProjectionAndGrid( stream, mapProjection_, gridParameters_ ); //x

  readAndMatchLine( stream,
                    "# MSB 64-bit integers points[timesteps] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "longitudes[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "latitudes[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                      "elevations[timesteps][points][layers] and\n" ); //x    
  readAndMatchLine( stream, "# MSB 64-bit integers "
                    "columns[timesteps][points] and\n" );//x
  readAndMatchLine( stream, "# MSB 64-bit integers "
                    "rows[timesteps][points] and\n" );//x
  readAndMatchLine( stream, "# MSB 64-bit integers "
                    "layers[timesteps][points][layers] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "data[timesteps][points][layers]:\n" ); //x

  // Read binary data:

  pointsPerTimestep_.resize( timesteps_ ); //x
  readIntegers( stream, 0LL, LLONG_MAX, pointsPerTimestep_ ); //x
  totalPoints_ = sum( &pointsPerTimestep_[ 0 ], timesteps_ );
  const size_t cellCount = totalPoints_ * layers_;
  coordinates_.resize( cellCount * cellVertexCount() ); //x
  skipBytes( stream, totalPoints_ * 2 * sizeof (double) ); //x Center lon-lats.

  // Read cell center elevations:

  std::vector<double> elevations( cellCount ); //x
  const bool ok =
    fread( &elevations[ 0 ], sizeof elevations[ 0 ], cellCount, stream )
      == cellCount;

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid "
                              "XDR regridded CALIPSO elevations." ); //x
  }

  reverse8ByteWordsIfLittleEndian( &elevations[ 0 ], cellCount );
  readRegridded64BitVariables( stream, layers_ ); //x columns, rows, layers,tab
  computeCMAQCellCoordinates( layers_, &elevations[ 0 ] );


  POST015( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ == 4,
           dataVariable_ == variables_ - 1,
           timesteps_ > 0,
           totalPoints_ > 0,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           coordinates_.size() == totalPoints_ * layers_ * cellVertexCount(),
           data_.size() == variables_ * totalPoints_ * layers_,
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

void RegriddedCALIPSODataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

  REGRIDDED-CALIPSO 2.0
  http://eosweb.larc.nasa.gov/PRODOCS/calipso/table_calipso.html,\
    CALIPSOSubset,XDRConvert
  2006-07-05T00:00:00-0000
  # timesteps layers
  24 34
  # Variable name:
  Feature_Optical_Depth_532
  # Variable units:
  -
  # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
  33 45 40 -97 6.37e+06 6.37e+06
  # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
  279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
    0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
    0.4 0.3 0.2 0.1 0
  # MSB 64-bit integers points[timesteps] and
  # IEEE-754 64-bit reals longitudes[timesteps][points] and
  # IEEE-754 64-bit reals latitudes[timesteps][points] and
  # IEEE-754 64-bit reals elevations[timesteps][points][layers] and
  # MSB 64-bit integers columns[timesteps][points] and
  # MSB 64-bit integers rows[timesteps][points] and
  # MSB 64-bit integers layers[timesteps][points][layers] and
  # IEEE-754 64-bit reals data[timesteps][points][layers]:

  */

  assert_static( AND2( sizeof (long long) == 8, sizeof (double) == 8 ) );
  Timestamp::UTCString timestamp = "";
  startingTimestamp_.toUTCString( timestamp );
  const char* const format1 =
    "REGRIDDED-CALIPSO 2.0\n"
    "%s\n"
    "%s\n"
    "# timesteps layers\n%" LU " %" LU "\n"
    "# Variable name:\n%s\n"
    "# Variable units:\n%s\n";
  const char* const format2 =
    "# MSB 64-bit integers points[timesteps] and\n"
    "# IEEE-754 64-bit reals longitudes[timesteps][points] and\n"
    "# IEEE-754 64-bit reals latitudes[timesteps][points] and\n"
    "# IEEE-754 64-bit reals elevations[timesteps][points][layers] and\n"
    "# MSB 64-bit integers columns[timesteps][points] and\n"
    "# MSB 64-bit integers rows[timesteps][points] and\n"
    "# MSB 64-bit integers layers[timesteps][points][layers] and\n"
    "# IEEE-754 64-bit reals data[timesteps][points][layers]:\n";
  bool ok =
    fprintf( stream, format1,
             description_.c_str(),
             timestamp,
             timesteps_, layers_,
             variableNames_[ dataVariable_ ].c_str(),
             variableUnits_[ dataVariable_ ].c_str() ) > (int) strlen(format1);

  if ( ok ) {
    writeProjectionAndGrid( stream, gridParameters_ ); //x
    ok = fprintf( stream, "%s", format2 ) == (int) strlen( format2 );

    if ( ok ) {
      writeIntegers( stream, 0LL, LLONG_MAX, pointsPerTimestep_ ); //x
      writeCoordinates( stream ); //x
      writeData( stream ); //x
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to write XDR regridded CALIPSO data.");//x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCoordinates - Write XDR regridded 64-bit longitudes, latitudes
         and elevations to a stream.
INPUTS:  FILE* stream  Stream to write from.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure to write coordinates to a stream.
******************************************************************************/

void RegriddedCALIPSODataset::writeCoordinates( FILE* stream ) const {
  PRE( stream );

  /* Data looks like this:
  # IEEE-754 64-bit reals longitudes[timesteps][points] and
  # IEEE-754 64-bit reals latitudes[timesteps][points] and
  # IEEE-754 64-bit reals elevations[timesteps][points][layers] and
  */

  const size_t longitudesLatitudeSize = totalPoints_ * 2;
  const size_t elevationsSize = totalPoints_ * layers_;
  const size_t bufferSize = longitudesLatitudeSize + elevationsSize;
  std::vector<double> buffer( bufferSize ); //x
  double* const longitudes = &buffer[ 0 ];
  double* const latitudes  = longitudes + totalPoints_;
  double* const elevations = latitudes + totalPoints_;

  for ( size_t point = 0, index = 0; point < totalPoints_; ++point ) {
    CHECK( point < coordinates_.size() );
    const Point& p = coordinates_[ point ];
    const double longitude = p.longitude;
    const double latitude  = p.latitude;
    longitudes[ point ] = longitude;
    latitudes[  point ] = latitude;
    CHECK( isLongitudeLatitude( longitude, latitude ) );

    for ( size_t layer = 0; layer < layers_; ++layer, ++index ) {
      CHECK( index < coordinates_.size() );
      const Point& centerPoint = cellCenter( index );
      const double elevation = centerPoint.elevation;
      CHECK( isElevation( elevation ) );
      elevations[ index ] = elevation;
    }
  }

  // Write longitudes, latitudes, elevations from buffer:

  reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], bufferSize );
  const bool ok =
    fwrite( &buffer[0], sizeof buffer[ 0 ], bufferSize, stream ) == bufferSize;

  if ( ! ok ) {
    throw std::runtime_error("Failed to write regridded CALIPSO coordinates.");
    //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeData - Write XDR regridded variable 64-bit columns, rows, layers,
         data to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure to write data to a stream.
******************************************************************************/

void RegriddedCALIPSODataset::writeData( FILE* stream ) const {
  PRE( stream );

  /* Data looks like this:
  # MSB 64-bit integers columns[timesteps][points] and
  # MSB 64-bit integers rows[timesteps][points] and
  # MSB 64-bit integers layers[timesteps][points][layers] and
  # IEEE-754 64-bit reals data[timesteps][points][layers]:
  */

  assert_static( AND2( sizeof (long long) == 8, sizeof (double) == 8 ) );
  const size_t variableSize = totalPoints_ * layers_;
  std::vector<double> buffer( variableSize ); //x
  long long* const ibuffer = (long long*) &buffer[ 0 ];
  const int integerVariables = 3;
  size_t dataIndex = 0;
  bool ok = true;

  for ( int variable = 0; AND2( ok, variable < variables_ ); ++variable ) {
    const size_t writeSize = variable < 2 ? totalPoints_ : variableSize;
    const size_t stride = variable < 2 ? layers_ : 1;

    for ( size_t point = 0; point < writeSize; ++point, dataIndex += stride ) {
      CHECK2( dataIndex < data_.size(), point < buffer.size() );

      if ( variable < integerVariables ) { // Convert integer column, row:
        const double dvalue = data_[ dataIndex ];
        const long long ivalue = (long long) dvalue;
        CHECK( ivalue > 0 );
        ibuffer[ point ] = ivalue;
      } else {
        const double value = data_[ dataIndex ];
        CHECK( isFinite( value ) );
        buffer[ point ] = value;
      }
    }

    reverse8ByteWordsIfLittleEndian( &buffer[ 0 ], writeSize );
    ok = fwrite( &buffer[0], sizeof buffer[0], writeSize, stream) == writeSize;
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write regridded CALIPSO data." ); //x
  }

  POST( true );
}



} // namespace emvl.



