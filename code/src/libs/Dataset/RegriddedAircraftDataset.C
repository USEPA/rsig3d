/******************************************************************************
PURPOSE: RegriddedAircraftDataset.C - Dataset for regridded Aircraft data.
HISTORY: 2013-10-21 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fwrite(), fscanf().
#include <string.h> // For memset(), strstr().
#include <limits.h> // For LLONG_MAX.
#include <limits.h> // For INT_MAX, LLONG_MAX.
#include <float.h>  // For FLT_MAX.
#include <unistd.h> // For unlink().

#include <stdexcept> // For std::runtime_error.
#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <memory>    // For std::auto_ptr.

#include <Assertions.h> // For PRE(), POST(), AND(), IN_RANGE().

#include <DataUtilities.h>            // For isNan(), isLongitude(), sum().
#include <ResourceHandler.h>          // For ResourceHandler.
#include <Bounds.h>                   // For Bounds.
#include <Timestamp.h>                // For Timestamp.
#include <QueryParameters.h>          // For QueryParameters.
#include <CMAQGridParameters.h>       // For CMAQGridParameters.
#include <NetCDFUtilities.h>          // For *NetCDF*() routines.
#include <Dataset.h>                  // For Dataset.
#include <RegriddedDataset.h>         // For RegriddedDataset.
#include <RegriddedAircraftDataset.h> // For RegriddedAircraftDataset.

enum { NOTE_LENGTH = 256 };

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~RegriddedAircraftDataset - Destruct a RegriddedAircraftDataset.
******************************************************************************/

RegriddedAircraftDataset::~RegriddedAircraftDataset() {
  PRE( true );
}



/******************************************************************************
PURPOSE: RegriddedAircraftDataset - Construct a RegriddedAircraftDataset from
         a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

RegriddedAircraftDataset::RegriddedAircraftDataset( FILE* stream ) {
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
  isVector_         = false;
  timesteps_        = 0;

  readXDR( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded Aircraft data.");
    //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedAircraftDataset - Construct a RegriddedAircraftDataset from
         a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

RegriddedAircraftDataset::RegriddedAircraftDataset( const std::string&
                                                      fileName ) {
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
  isVector_         = false;
  timesteps_        = 0;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded Aircraft data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedAircraftDataset - Copy-construct a RegriddedAircraftDataset.
INPUTS:  const RegriddedAircraftDataset& other  Object to copy.
******************************************************************************/

RegriddedAircraftDataset::RegriddedAircraftDataset(
  const RegriddedAircraftDataset& other ) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    RegriddedDataset::copy( other ); // Copy all inherited attributes.
    notes_ = other.notes_;
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a RegriddedAircraftDataset.
INPUTS:  const RegriddedAircraftDataset& other  Object to copy.
RETURNS: RegriddedAircraftDataset& *this.
******************************************************************************/

RegriddedAircraftDataset& RegriddedAircraftDataset::operator=(
  const RegriddedAircraftDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    RegriddedDataset::copy( other ); // Copy all inherited attributes.
    notes_ = other.notes_;
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

Dataset* RegriddedAircraftDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: RegriddedAircraftDataset::sample");
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

Dataset* RegriddedAircraftDataset::regrid( const int regridMethod,
                                           const CMAQGridParameters&
                                             cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error( "UNIMPLEMENTED: RegriddedAircraftDataset::regrid");
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

void RegriddedAircraftDataset::write( const std::string& directory,
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

bool RegriddedAircraftDataset::invariant() const {
  const bool result =
    AND14( RegriddedDataset::invariant(),
           variables_        == 4 + isVector_,
           cellType_         == HEXAHEDRON,
           hasElevation_     == true,
           hasArea_          == true,
           hasVolume_        == true,
           isStationary_     == false,
           isGroup_          == false,
           isStructuredGrid_ == false,
           isSurfaceData()   == false,
           totalPoints_ > 0,
           notes_.size() == totalPoints_,
           notes_[ 0 ][ 0 ],
           notes_[ notes_.size() - 1 ][ 0 ] );
  return result;
}



/******************************************************************************
PURPOSE: isSurfaceData - Is this dataset only on the Earth surface?
RETURNS: bool false.
******************************************************************************/

bool RegriddedAircraftDataset::isSurfaceData() const {
  const bool result = false;
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool RegriddedAircraftDataset::operator==( const RegriddedAircraftDataset&
                                             other ) const {
  PRE( other.invariant() );
  const bool result =
    OR2( this == &other,
         AND2( RegriddedDataset::equal( other ), notes_ == other.notes_ ) );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool RegriddedAircraftDataset::operator!=(
  const RegriddedAircraftDataset& other ) const {
  PRE( other.invariant() );
  const bool result = ! ( *this == other );
  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: subsetNotes - Called by vsubset() with the number of subset points to
         allow derived classes with notes to include pointers to the
         corresponding subset.
INPUTS:  const size_t skipPoints  Number of points skipped in vsubset().
RETURNS: const std::string*  Pointer to notes, if any.
NOTES:   This default implementation returns 0.
******************************************************************************/

const std::string*
RegriddedAircraftDataset::subsetNotes( const size_t skipPoints ) const {
  PRE2( notes_.size(), IN_RANGE( skipPoints, 0, notes_.size() - 1 ) );
  const std::string* const result = &notes_[ skipPoints ];
  POST( result );
  return result;
}



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

void RegriddedAircraftDataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read ASCII header that looks like this:

  REGRIDDED-Aircraft 3.0
  http://mozaic.aero.obs-mip.fr/web/,MOZAICSubset,XDRConvert
  2006-07-03T00:00:00-0000
  # timesteps
  24
  # Variable name:
  ozone
  # Variable units:
  ppmV
  # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
  33 45 40 -97 6.37e+06 6.37e+06
  # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
  279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
    0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
    0.4 0.3 0.2 0.1 0
  # MSB 64-bit integers points[timesteps] and
  # char notes[points][256] and
  # IEEE-754 64-bit reals longitudes[points] and
  # IEEE-754 64-bit reals latitudes[points] and
  # IEEE-754 64-bit reals elevations[points] and
  # MSB 64-bit integers columns[points] and
  # MSB 64-bit integers rows[points] and
  # MSB 64-bit integers layers[points] and
  # IEEE-754 64-bit reals data[points]:

  */

  readAndMatchLine( stream, "REGRIDDED-Aircraft 3.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  readDimensions( stream, 1, &timesteps_ ); //x

  // Prepend column, row and layer variables:

  variableNames_.reserve( 5 ); //x
  variableUnits_.reserve( 5 ); //x
  variableNames_.push_back( "column" ); //x
  variableNames_.push_back( "row" ); //x
  variableNames_.push_back( "layer" ); //x
  variableUnits_.push_back( "-" ); //x
  variableUnits_.push_back( "-" ); //x
  variableUnits_.push_back( "-" ); //x
  variables_ = 3; // column, row, layer

  // Read variable:

  readVariablesAndUnits( stream, 0 ); //x
  CHECK( IN3( variables_, 4, 5 ) );
  isVector_ =
    AND3( variables_ == 5,
          ! strcmp( variableNames_[ 3 ].c_str(), "wind_u" ),
          ! strcmp( variableNames_[ 4 ].c_str(), "wind_v" ) );
  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.
  readProjectionAndGrid( stream, mapProjection_, gridParameters_ ); //x
  readAndMatchLine( stream,
                    "# MSB 64-bit integers points[timesteps] and\n" );//x
  readAndMatchLine( stream, "# char notes[points][256] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "longitudes[points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "latitudes[points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "elevations[points] and\n" ); //x
  readAndMatchLine( stream, "# MSB 64-bit integers columns[points] and\n" );//x
  readAndMatchLine( stream, "# MSB 64-bit integers rows[points] and\n" ); //x
  readAndMatchLine( stream, "# MSB 64-bit integers layers[points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals data[points]:\n" ); //x

  // Read binary data:

  pointsPerTimestep_.resize( timesteps_ ); //x
  readIntegers( stream, 0LL, LLONG_MAX, pointsPerTimestep_ ); //x
  totalPoints_ = sum( &pointsPerTimestep_[ 0 ], timesteps_ );
  notes_.resize( totalPoints_ ); //x
  readStrings( stream, NOTE_LENGTH, notes_ ); //x
  coordinates_.resize( totalPoints_ * cellVertexCount() ); //x
  skipBytes( stream, totalPoints_ * 2 * sizeof (double) ); //x Center lon-lats.

  // Read cell center elevations:

  std::vector<double> elevations( totalPoints_ ); //x
  const bool ok =
    fread( &elevations[ 0 ], sizeof elevations[ 0 ], totalPoints_, stream )
    == totalPoints_;

  if ( ! ok ) {
    throw std::runtime_error( "Failed to read valid "
                              "XDR regridded Profile elevations." ); //x
  }

  reverse8ByteWordsIfLittleEndian( &elevations[ 0 ], totalPoints_ );
  readRegridded64BitVariables( stream, 1 ); //x columns, rows, layers, ozone
  computeCMAQCellCoordinates( 1, &elevations[ 0 ] );

  POST016( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ == 4 + isVector_,
           dataVariable_ == variables_ - 1,
           timesteps_ > 0,
           totalPoints_ > 0,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           coordinates_.size() == totalPoints_ * cellVertexCount(),
           notes_.size() == totalPoints_,
           data_.size() == variables_ * totalPoints_,
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

void RegriddedAircraftDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

  REGRIDDED-Aircraft 3.0
  http://mozaic.aero.obs-mip.fr/web/,MOZAICSubset,XDRConvert
  2006-07-03T00:00:00-0000
  # timesteps
  24
  # Variable name:
  ozone
  # Variable units:
  ppmV
  # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
  33 45 40 -97 6.37e+06 6.37e+06
  # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
  279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
    0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
    0.4 0.3 0.2 0.1 0
  # MSB 64-bit integers points[timesteps] and
  # char notes[points][256] and
  # IEEE-754 64-bit reals longitudes[points] and
  # IEEE-754 64-bit reals latitudes[points] and
  # IEEE-754 64-bit reals elevations[points] and
  # MSB 64-bit integers columns[points] and
  # MSB 64-bit integers rows[points] and
  # MSB 64-bit integers layers[points] and
  # IEEE-754 64-bit reals data[points]:

  */

  assert_static( AND2( sizeof (long long) == 8, sizeof (double) == 8 ) );
  Timestamp::UTCString timestamp = "";
  startingTimestamp_.toUTCString( timestamp );
  const char* const formatScalar =
    "REGRIDDED-Aircraft 3.0\n"
    "%s\n"
    "%s\n"
    "# timesteps\n%" LU "\n"
    "# Variable name:\n%s\n"
    "# Variable units:\n%s\n";
  const char* const formatVector =
    "REGRIDDED-Aircraft 3.0\n"
    "%s\n"
    "%s\n"
    "# timesteps\n%" LU "\n"
    "# Variable name:\n%s %s\n"
    "# Variable units:\n%s %s\n";
  const char* const formatRemainder =
    "# MSB 64-bit integers points[timesteps] and\n"
    "# char notes[points][256] and\n"
    "# IEEE-754 64-bit reals longitudes[points] and\n"
    "# IEEE-754 64-bit reals latitudes[points] and\n"
    "# IEEE-754 64-bit reals elevations[points] and\n"
    "# MSB 64-bit integers columns[points] and\n"
    "# MSB 64-bit integers rows[points] and\n"
    "# MSB 64-bit integers layers[points] and\n"
    "# IEEE-754 64-bit reals data[points]:\n";
  bool ok = false;

  if ( isVector_ ) {
    ok = fprintf( stream, formatVector,
                  description_.c_str(),
                  timestamp,
                  timesteps_,
                  variableNames_[ variables_ - 2 ].c_str(),
                  variableNames_[ variables_ - 1 ].c_str(),
                  variableUnits_[ variables_ - 2 ].c_str(),
                  variableUnits_[ variables_ - 1 ].c_str() )
         > (int) strlen( formatVector );
  } else {
    ok = fprintf( stream, formatScalar,
                  description_.c_str(),
                  timestamp,
                  timesteps_,
                  variableNames_[ dataVariable_ ].c_str(),
                  variableUnits_[ dataVariable_ ].c_str() )
         > (int) strlen( formatScalar );
  }

  if ( ok ) {
    writeProjectionAndGrid( stream, gridParameters_ ); //x
    ok = fprintf( stream, "%s", formatRemainder )
         == (int) strlen( formatRemainder );

    if ( ok ) {
      writeIntegers( stream, 0LL, LLONG_MAX, pointsPerTimestep_ ); //x
      writeStrings( stream, NOTE_LENGTH, notes_ ); //x
      writeRegridded64BitCoordinatesAndVariables( stream ); //x      
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR regridded Aircraft data." );
    //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeASCII - Write ASCII format to a stream.
INPUTS:  FILE* stream  Stream to write to.
OUTPUTS: FILE* stream  Updated stream.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedAircraftDataset::writeASCII( FILE* stream ) const {
  PRE( stream );

  /* Write ASCII format that looks like this (with tabs as delimiters):

   Timestamp(UTC)  LONGITUDE(deg)  LATITUDE(deg)  ELEVATION(m)  \
     COLUMN(-)  ROW(-)  LAYER(-)  ozone(ppmV)   notes(-)
   2005-08-26T00:00:00-0000    -86.81000    33.55000    4321.12345 \
     11     22     9     0.03800    MD20060703014:FRANKFURT->ATLANTA
   ...

  */

  bool ok = false;

  if ( isVector_ ) {
    const char* const format =
      "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tELEVATION(m)\t"
      "COLUMN(-)\tROW(-)\tLAYER(-)\t%s(%s)\t%s(%s)\tnotes(-)\n";
    ok = fprintf( stream, format,
                  variableNames_[ variables_ - 2 ].c_str(),
                  variableUnits_[ variables_ - 2 ].c_str(),
                  variableUnits_[ variables_ - 1 ].c_str(),
                  variableNames_[ variables_ - 1 ].c_str() )
         >= (int) strlen( format ) - 14;
  } else {
    const char* const format =
      "Timestamp(UTC)\tLONGITUDE(deg)\tLATITUDE(deg)\tELEVATION(m)\t"
      "COLUMN(-)\tROW(-)\tLAYER(-)\t%s(%s)\tnotes(-)\n";
    ok = fprintf( stream, format,
                  variableNames_[ dataVariable_ ].c_str(),
                  variableUnits_[ dataVariable_ ].c_str() )
         >= (int) strlen( format ) - 11;
  }

  if ( ok ) {
    const char* const scalarDataFormat =
      "%s\t%11.5f\t%11.5f\t%11.5f\t%11d\t%11d\t%11d\t%11.5e\t%-256s\n";
    const char* const vectorDataFormat =
      "%s\t%11.5f\t%11.5f\t%11.5f\t%11d\t%11d\t%11d\t%11.5e\t%11.5e\t%-256s\n";
    Timestamp timestamp( startingTimestamp_ );
    size_t cellIndex = 0;
    size_t columnIndex = 0;
    size_t rowIndex = totalPoints_;
    size_t layerIndex = rowIndex + totalPoints_;
    size_t dataIndex = layerIndex + totalPoints_;
    const int hoursPerTimestep = timestepSize_ == DAYS ? 24 : 1;

    for ( size_t timestep = 0; timestep < timesteps_;
         ++timestep, timestamp.offsetHours( hoursPerTimestep ) ) {
      const size_t pointsThisTimestep = pointsPerTimestep_[ timestep ];
      Timestamp::UTCString timestampString = "";
      timestamp.toUTCString( timestampString );

      for ( size_t point = 0; point < pointsThisTimestep; ++point,
            ++columnIndex, ++rowIndex, ++layerIndex, ++dataIndex,
            ++cellIndex ) {
        CHECK6( IN_RANGE( cellIndex, 0, coordinates_.size() - 1 ),
                IN_RANGE( columnIndex, 0, data_.size() - 1 ),
                IN_RANGE( rowIndex, totalPoints_, data_.size() - 1 ),
                IN_RANGE( layerIndex, rowIndex + totalPoints_,
                          data_.size() - 1 ),
                IN_RANGE( dataIndex, layerIndex + totalPoints_,
                          data_.size() - 1 ),
                IN_RANGE( cellIndex, 0, notes_.size() - 1 ) );
        const double longitude = coordinates_[ cellIndex ].longitude;
        const double latitude  = coordinates_[ cellIndex ].latitude;
        const double elevation = coordinates_[ cellIndex ].elevation;
        const int column       = (int) data_[ columnIndex ];
        const int row          = (int) data_[ rowIndex ];
        const int layer        = (int) data_[ layerIndex ];
        const double measure   = data_[ dataIndex ];
        const char* const note = notes_[ cellIndex ].c_str();

        if ( isVector_ ) {
          const size_t dataIndex2 = dataIndex + totalPoints_;
          CHECK( dataIndex2 < data_.size() );
          const double measure2 = data_[ dataIndex2 ];
          ok = fprintf( stream, vectorDataFormat,
                        timestampString, longitude, latitude, elevation,
                        column, row, layer, measure, measure2, note ) > 0;
        } else {
          ok = fprintf( stream, scalarDataFormat,
                        timestampString, longitude, latitude, elevation,
                        column, row, layer, measure, note ) > 0;
        }

        if ( ! ok ) {
          point = totalPoints_;
          timestep = timesteps_;
        }
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error("Failed to write ASCII regridded Aircraft data.");
    //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: writeCOARDS - Write COARDS-format data.
INPUTS:  const std::string& fileName  File name to create.
NOTES:   Throws exception upon failure.
******************************************************************************/

void RegriddedAircraftDataset::writeCOARDS(const std::string& fileName) const {

  PRE( fileName.length() );
  const size_t timeVariables = 3; // yyyymmdd, hhmmss, time
  const size_t coordinateVariables = 2 + (hasElevation() ? 1 : 0);//lon,lat,elv
  const size_t wordSize = 4; // IEEE-754 float.
  const size_t headerSize = 10000; // Guess.
  const size_t fileVariables =
    variables_ + coordinateVariables + timeVariables;
  const size_t layers0 = layers();
  const size_t pointLayers = layers0 > 0 ? layers0 : 1;
  const size_t dataPoints = totalPoints_ * pointLayers;
  const size_t extraVariableSize = dataPoints * NOTE_LENGTH;
  const size_t fileSizeEstimate =
    fileVariables * dataPoints * wordSize + extraVariableSize + headerSize;
  const size_t TWO_GB = 1UL << 31;
  const int create64BitFile = fileSizeEstimate > TWO_GB;
  const char* const name = fileName.c_str();
  const int file = createNetCDFFile( name, create64BitFile );
  ResourceHandler<int> fileCloser( file, closeNetCDFFile );
  fileCloser.handler2( name, unlink ); // Remove upon failure.
  bool ok = false;

  if ( file ) {
    enum {  DIMENSIONS = 2 };
    const char* const names[ DIMENSIONS ] = { "points", "length" };
    CHECK( isSignedInt( dataPoints ) );
    const size_t sizes[ DIMENSIONS ] = { dataPoints, NOTE_LENGTH };
    int ids[ DIMENSIONS ] = { -1, -1 };

    if ( createNetCDFDimensions( file, DIMENSIONS, names, sizes, ids ) ) {

      if ( createNetCDFVariable( file, "notes", "-", NC_CHAR, 0, DIMENSIONS,
                                 ids ) != -1 ) {
        std::vector<Timestamp> timestamps = timestampsPerDataPoint(); //x
        writeCOARDSCommon( file, 0, true, false, true, true, 1,
                           DIMENSIONS - 1, ids, sizes, names, timestamps ); //x
        writeCOARDSNotes( file, NOTE_LENGTH, notes_ ); //x
        ok = true;
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to create NetCDF-COARDS file for "
                              "regridded Aircraft data." ); //x
  }

  fileCloser.handler2( 0, 0 ); // Successful so don't remove.
  POST( true );
}



} // namespace emvl.



