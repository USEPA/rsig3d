/******************************************************************************
PURPOSE: RegriddedGridDataset.C - Dataset for regridded Grid point data.
HISTORY: 2020-04-01 plessel.todd@epa.gov
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

#include <DataUtilities.h>        // For isNan(), isLongitude(), sum().
#include <ResourceHandler.h>      // For ResourceHandler.
#include <Bounds.h>               // For Bounds.
#include <Timestamp.h>            // For Timestamp.
#include <QueryParameters.h>      // For QueryParameters.
#include <CMAQGridParameters.h>   // For CMAQGridParameters.
#include <NetCDFUtilities.h>      // For *NetCDF*() routines.
#include <Dataset.h>              // For Dataset.
#include <RegriddedDataset.h>     // For RegriddedDataset.
#include <RegriddedGridDataset.h> // For RegriddedGridDataset.

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~RegriddedGridDataset - Destruct a RegriddedGridDataset.
******************************************************************************/

RegriddedGridDataset::~RegriddedGridDataset() {
  PRE( true );
  totalPoints_ = 0;
}



/******************************************************************************
PURPOSE: RegriddedGridDataset - Construct a RegriddedGridDataset from a stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

RegriddedGridDataset::RegriddedGridDataset( FILE* stream ) {
  PRE0( stream );

  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = QUADRILATERAL;
  hasElevation_     = false;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = false;
  isStructuredGrid_ = false;
  timesteps_        = 0;

  totalPoints_      = 0;

  readXDR( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded Grid data."); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedGridDataset - Construct a RegriddedGridDataset from a file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid data.
******************************************************************************/

RegriddedGridDataset::RegriddedGridDataset( const std::string& fileName ) {
  PRE0( fileName.length() );

  fileName_         = fileName;
  variables_        = 0;
  dataVariable_     = 0;
  cellType_         = QUADRILATERAL;
  hasElevation_     = false;
  hasArea_          = true;
  hasVolume_        = false;
  isStationary_     = false;
  isGroup_          = false;
  isStructuredGrid_ = false;
  timesteps_        = 0;

  totalPoints_      = 0;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded Grid data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedGridDataset - Copy-construct a RegriddedGridDataset.
INPUTS:  const RegriddedGridDataset& other  Object to copy.
******************************************************************************/

RegriddedGridDataset::RegriddedGridDataset(const RegriddedGridDataset& other) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    RegriddedDataset::copy( other ); // Copy all inherited attributes.
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a RegriddedGridDataset.
INPUTS:  const RegriddedGridDataset& other  Object to copy.
RETURNS: RegriddedGridDataset& *this.
******************************************************************************/

RegriddedGridDataset& RegriddedGridDataset::operator=(
  const RegriddedGridDataset& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    RegriddedDataset::copy( other ); // Copy all inherited attributes.
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

Dataset* RegriddedGridDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error("UNIMPLEMENTED: RegriddedGridDataset::sample()");//x
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

Dataset* RegriddedGridDataset::regrid( const int regridMethod,
                                       const CMAQGridParameters&
                                         cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error("UNIMPLEMENTED: RegriddedGridDataset::regrid()");//x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: write - Write dataset to a directory file in the specified format.
INPUTS:  const std::string& directory  Name of directory to write to.
         const int format              Format of file to write.
NOTES:   The name of the file is determined by the dataset name, variable, etc.
******************************************************************************/

void RegriddedGridDataset::write( const std::string& directory,
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

bool RegriddedGridDataset::invariant() const {
  const bool result =
    AND10( RegriddedDataset::invariant(),
           variables_        == 3 + isVector_,
           cellType_         == QUADRILATERAL,
           hasElevation_     == false,
           hasArea_          == true,
           hasVolume_        == false,
           isStationary_     == false,
           isGroup_          == false,
           isStructuredGrid_ == false,
           totalPoints_      > 0 );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are object equal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool RegriddedGridDataset::operator==( const RegriddedGridDataset& other )
  const {
  PRE( other.invariant() );
  const bool result = OR2( this == &other, RegriddedDataset::equal( other ) );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool RegriddedGridDataset::operator!=( const RegriddedGridDataset& other )
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

void RegriddedGridDataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read ASCII header that looks like this:

REGRIDDED-Grid 1.0
http://home.chpc.utah.edu/~u0553130/Brian_Blaylock/,XDRConvert
2008-07-03T00:00:00-0000
# timesteps
24
# Variable name:
wind_u wind_v
# Variable units:
m/s m/s
# lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
33 45 40 -97 6370000.000000 6370000.000000
# Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[2]:
459 299 -2556000.000000 -1728000.000000 12000.000000 12000.000000 2 10000.000000 1 0.995
# MSB 64-bit integers points[timesteps] and
# IEEE-754 64-bit reals longitudes[timesteps][points] and
# IEEE-754 64-bit reals latitudes[timesteps][points] and
# MSB 64-bit integers columns[timesteps][points] and
# MSB 64-bit integers rows[timesteps][points] and
# IEEE-754 64-bit reals data[timesteps][points]:

  */

  readAndMatchLine( stream, "REGRIDDED-Grid 1.0\n" ); //x
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  readDimensions( stream, 1, &timesteps_ ); //x

  // Prepend row and column variables:

  variableNames_.reserve( 3 ); //x
  variableUnits_.reserve( 3 ); //x
  variableNames_.push_back( "column" ); //x
  variableNames_.push_back( "row" ); //x
  variableUnits_.push_back( "-" ); //x
  variableUnits_.push_back( "-" ); //x
  variables_ = 2; // row, column.

  // Read variable:

  readVariablesAndUnits( stream, 0 ); //x
  CHECK2( IN3( variables_, 3, 4 ), dataVariable_ == variables_ - 1 );

  isVector_ =
    AND2( variables_ == 4,
          OR2( AND2( strcmp( variableNames_[ 2 ].c_str(), "windU" ) == 0,
                     strcmp( variableNames_[ 3 ].c_str(), "windV" ) == 0 ),
               AND2( strcmp( variableNames_[ 2 ].c_str(), "wind_u" ) == 0,
                     strcmp( variableNames_[ 3 ].c_str(), "wind_v" ) == 0 ) ) );

  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.
  readProjectionAndGrid( stream, mapProjection_, gridParameters_ ); //x

  readAndMatchLine( stream,
                    "# MSB 64-bit integers points[timesteps] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "longitudes[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "latitudes[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# MSB 64-bit integers "
                    "columns[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# MSB 64-bit integers "
                    "rows[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 64-bit reals "
                    "data[timesteps][points]:\n" ); //x

  // Read binary data:

  pointsPerTimestep_.resize( timesteps_ ); //x
  readIntegers( stream, 0LL, LLONG_MAX, pointsPerTimestep_ ); //x
  totalPoints_ = sum( &pointsPerTimestep_[ 0 ], timesteps_ );
  coordinates_.resize( totalPoints_ * cellVertexCount() ); //x
  skipBytes( stream, totalPoints_ * 2 * sizeof (double) ); //x Center lon-lats.
  readRegridded64BitVariables( stream, 0 ); //x
  computeCMAQCellCoordinates( 0, 0 );

  POST015( isValidName(),
           isValidDescription(),
           isValidVariableNamesAndUnits(),
           variables_ == 3 + isVector_,
           dataVariable_ == variables_ - 1,
           timesteps_ > 0,
           totalPoints_ > 0,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ < endingTimestamp_,
           coordinates_.size() == totalPoints_ * cellVertexCount(),
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

void RegriddedGridDataset::writeXDR( FILE* stream ) const {
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

    REGRIDDED-Grid 1.0
    http://home.chpc.utah.edu/~u0553130/Brian_Blaylock/,XDRConvert
    2008-07-03T00:00:00-0000
    # timesteps
    24
    # Variable name:
    wind_u wind_v
    # Variable units:
    m/s m/s
    # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
    33 45 40 -97 6370000.000000 6370000.000000
    # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[2]:
    459 299 -2556000.000000 -1728000.000000 12000.000000 12000.000000 2 10000.000000 1 0.995
    # MSB 64-bit integers points[timesteps] and
    # IEEE-754 64-bit reals longitudes[timesteps][points] and
    # IEEE-754 64-bit reals latitudes[timesteps][points] and
    # MSB 64-bit integers columns[timesteps][points] and
    # MSB 64-bit integers rows[timesteps][points] and
    # IEEE-754 64-bit reals data[timesteps][points]:

    */

    assert_static( AND2( sizeof (long long) == 8, sizeof (double) == 8 ) );
    Timestamp::UTCString timestamp = "";
    startingTimestamp_.toUTCString( timestamp );
    const char* const format1 =
      "REGRIDDED-Grid 1.0\n"
      "%s\n"
      "%s\n"
      "# timesteps\n%" LU "\n"
      "# Variable name:\n%s\n"
      "# Variable units:\n%s\n";
    const char* const format2 =
      "REGRIDDED-Grid 1.0\n"
      "%s\n"
      "%s\n"
      "# timesteps\n%" LU "\n"
      "# Variable name:\n%s %s\n"
      "# Variable units:\n%s %s\n";
    const char* const format3 =
      "# MSB 64-bit integers points[timesteps] and\n"
      "# IEEE-754 64-bit reals longitudes[timesteps][points] and\n"
      "# IEEE-754 64-bit reals latitudes[timesteps][points] and\n"
      "# MSB 64-bit integers columns[timesteps][points] and\n"
      "# MSB 64-bit integers rows[timesteps][points] and\n"
      "# IEEE-754 64-bit reals data[timesteps][points]:\n";
    ok =
      isVector_ ?
        fprintf( stream, format2,
                 description_.c_str(),
                 timestamp,
                 timesteps_,
                 variableNames_[ variables_ - 2 ].c_str(),
                 variableNames_[ variables_ - 1 ].c_str(),
                 variableUnits_[ variables_ - 2 ].c_str(),
                 variableUnits_[ variables_ - 1 ].c_str() )
        > (int) strlen( format2 )
      :
        fprintf( stream, format1,
                 description_.c_str(),
                 timestamp,
                 timesteps_,
                 variableNames_[ dataVariable_ ].c_str(),
                 variableUnits_[ dataVariable_ ].c_str() )
        > (int) strlen( format1 );

    if ( ok ) {
      writeProjectionAndGrid( stream, gridParameters_ ); //x
      ok = fprintf( stream, "%s", format3 ) == (int) strlen( format3 );

      if ( ok ) {
        writeIntegers( stream, 0LL, LLONG_MAX, pointsPerTimestep_ ); //x
        writeRegridded64BitCoordinatesAndVariables( stream ); //x
      }
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write XDR regridded Grid data."); //x
  }

  POST( true );
}



} // namespace emvl.



