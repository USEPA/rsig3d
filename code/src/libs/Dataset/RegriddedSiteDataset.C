/******************************************************************************
PURPOSE: RegriddedSiteDataset.C - Dataset for regridded site (ground site) data
HISTORY: 2013-02-20 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, fgets(), fread(), fwrite(), fscanf().
#include <string.h> // For memset(), strstr().
#include <limits.h> // For INT_MAX, LLONG_MAX.
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
#include <RegriddedSiteDataset.h> // For RegriddedSiteDataset.

namespace emvl {

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: ~RegriddedSiteDataset - Destruct a RegriddedSiteDataset.
******************************************************************************/

RegriddedSiteDataset::~RegriddedSiteDataset() {
  PRE( true );
}



/******************************************************************************
PURPOSE: RegriddedSiteDataset - Construct a RegriddedSiteDataset from reading a
         stream.
INPUTS:  const FILE* stream  Stream to read from.
NOTES:   Throws exception upon failure to read/store valid regridded site data
         from file.
******************************************************************************/

RegriddedSiteDataset::RegriddedSiteDataset( FILE* stream ) {
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
  isVector_         = false;
  timesteps_        = 0;

  totalPoints_      = 0;

  readXDR( stream ); //x

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded site data."); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedSiteDataset - Construct a RegriddedSiteDataset from reading a
         file.
INPUTS:  const std::string& fileName  Name of file to read from.
NOTES:   Throws exception upon failure to read/store valid regridded site data
         from file.
******************************************************************************/

RegriddedSiteDataset::RegriddedSiteDataset( const std::string& fileName ) {
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
  isVector_         = false;
  timesteps_        = 0;

  totalPoints_      = 0;

  ResourceHandler<FILE*> fileCloser( fopen( fileName.c_str(), "rb" ), fclose );
  FILE* const file = fileCloser.get();

  if ( file ) {
    readXDR( file ); //x    
  }

  if ( ! invariant() ) {
    throw std::runtime_error( "Failed to read valid regridded site data "
                              "from file " + fileName + '.' ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: RegriddedSiteDataset - Copy-construct a RegriddedSiteDataset.
INPUTS:  const RegriddedSiteDataset& other  Object to copy.
******************************************************************************/

RegriddedSiteDataset::RegriddedSiteDataset(const RegriddedSiteDataset& other) {
  PRE0( other.invariant() );

  if ( this != &other ) {
    RegriddedDataset::copy( other ); // Copy all inherited attributes.
  }

  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Assign a RegriddedSiteDataset.
INPUTS:  const RegriddedSiteDataset& other  Object to copy.
RETURNS: RegriddedSiteDataset& *this.
******************************************************************************/

RegriddedSiteDataset& RegriddedSiteDataset::operator=(
  const RegriddedSiteDataset& other ) {
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

Dataset* RegriddedSiteDataset::sample( const Dataset& other ) const {
  PRE( other.invariant() );
  Dataset* result = 0;
  throw std::runtime_error("UNIMPLEMENTED: RegriddedSiteDataset::sample()");//x
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

Dataset* RegriddedSiteDataset::regrid( const int regridMethod,
                                       const CMAQGridParameters&
                                         cmaqGridParameters ) const {
  PRE3( IN_RANGE( regridMethod, 0, QueryParameters::REGRID_METHODS - 1 ),
        regridMethod != QueryParameters::REGRID_NONE,
        cmaqGridParameters.invariant() );
  Dataset* result = 0;
  throw std::runtime_error("UNIMPLEMENTED: RegriddedSiteDataset::regrid()");//x
  POST( IMPLIES( result, result->invariant() ) );
  return result;
}



/******************************************************************************
PURPOSE: write - Write dataset to a directory file in the specified format.
INPUTS:  const std::string& directory  Name of directory to write to.
         const int format              Format of file to write.
NOTES:   The name of the file is determined by the dataset name, variable, etc.
******************************************************************************/

void RegriddedSiteDataset::write( const std::string& directory,
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

bool RegriddedSiteDataset::invariant() const {
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

bool RegriddedSiteDataset::operator==( const RegriddedSiteDataset& other )
  const {
  PRE( other.invariant() );
  const bool result = OR2( this == &other, RegriddedDataset::equal( other ) );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are object unequal?
RETURNS: bool true if equal, else false.
******************************************************************************/

bool RegriddedSiteDataset::operator!=( const RegriddedSiteDataset& other )
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

void RegriddedSiteDataset::readXDR( FILE* stream ) {
  PRE0( stream );

  /* Read ASCII header that looks like this:

    REGRIDDED-SITE 2.0
    http://airnow.gov/,reformat_airnow_obs,SiteSubset,XDRConvert
    2005-08-26T00:00:00-0000
    # timesteps
    3
    # Variable name:
    OZONE
    # Variable units:
    ppb
    # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
    33 45 40 -97 6.37e+06 6.37e+06
    # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
    279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
      0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
      0.4 0.3 0.2 0.1 0
    # MSB 32-bit integers points[timesteps] and
    # IEEE-754 32-bit reals longitudes[timesteps][points] and
    # IEEE-754 32-bit reals latitudes[timesteps][points] and
    # MSB 32-bit integers columns[timesteps][points] and
    # MSB 32-bit integers rows[timesteps][points] and
    # IEEE-754 32-bit reals data[timesteps][points]:

  */

  readAndMatchLine( stream, "REGRIDDED-SITE 2.0\n"); //x
  readDescription( stream ); //x
  readTimestamp( stream, false ); //x
  skipLine( stream ); //x
  readDimensions( stream, 1, &timesteps_ ); //x

  // Prepend row and column variables:

  variableNames_.reserve( variables_ ); //x
  variableUnits_.reserve( variables_ ); //x
  variableNames_.push_back( "column" ); //x
  variableNames_.push_back( "row" ); //x
  variableUnits_.push_back( "-" ); //x
  variableUnits_.push_back( "-" ); //x
  variables_ = 2; // row, column.

  // Read variable:

  readVariablesAndUnits( stream, 0 ); //x

  isVector_ =
    AND2( variables_ == 4,
          OR2( AND2( ! strcmp( variableNames_[ 2 ].c_str(), "windU" ),
                     ! strcmp( variableNames_[ 3 ].c_str(), "windV" ) ),
               AND2( ! strcmp( variableNames_[ 2 ].c_str(), "wind_u" ),
                     ! strcmp( variableNames_[ 3 ].c_str(), "wind_v" ) ) ) );

  setDatasetNameFromDescriptionAndVariable(); // in:isVector_,out:timestepSize_
  setEndingTimestamp(); // Reads timestepSize_.
  readProjectionAndGrid( stream, mapProjection_, gridParameters_ ); //x
  readAndMatchLine( stream,
                    "# MSB 32-bit integers points[timesteps] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 32-bit reals "
                    "longitudes[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 32-bit reals "
                    "latitudes[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# MSB 32-bit integers "
                    "columns[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# MSB 32-bit integers "
                    "rows[timesteps][points] and\n" ); //x
  readAndMatchLine( stream, "# IEEE-754 32-bit reals "
                    "data[timesteps][points]:\n" ); //x

  // Read binary data:
  
  pointsPerTimestep_.resize( timesteps_ ); //x
  readIntegers( stream, 0, INT_MAX, pointsPerTimestep_ ); //x
  totalPoints_ = sum( &pointsPerTimestep_[ 0 ], timesteps_ );
  coordinates_.resize( totalPoints_ * cellVertexCount() ); //x
  skipBytes( stream, totalPoints_ * 2 * sizeof (float) ); //x Center lon-lats.
  readRegridded32BitVariables( stream ); //x columns, rows, ozone.
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

void RegriddedSiteDataset::writeXDR( FILE* stream ) const {
  PRE( stream );

  /* Write XDR header that looks like this:

    REGRIDDED-SITE 2.0
    http://airnow.gov/,reformat_airnow_obs,SiteSubset,XDRConvert
    2005-08-26T00:00:00-0000
    # timesteps
    3
    # Variable name:
    OZONE
    # Variable units:
    ppb
    # lcc projection: lat_1 lat_2 lat_0 lon_0 major_semiaxis minor_semiaxis
    33 45 40 -97 6.37e+06 6.37e+06
    # Grid: ncols nrows xorig yorig xcell ycell vgtyp vgtop vglvls[25]:
    279 240 -1.008e+06 -1.62e+06 12000 12000 2 10000 1 0.995 0.99 0.98 0.97 \
      0.96 0.94 0.92 0.9 0.88 0.86 0.84 0.82 0.8 0.77 0.74 0.7 0.65 0.6 0.5 \
      0.4 0.3 0.2 0.1 0
    # MSB 32-bit integers points[timesteps] and
    # IEEE-754 32-bit reals longitudes[timesteps][points] and
    # IEEE-754 32-bit reals latitudes[timesteps][points] and
    # MSB 32-bit integers columns[timesteps][points] and
    # MSB 32-bit integers rows[timesteps][points] and
    # IEEE-754 32-bit reals data[timesteps][points]:

  */

  Timestamp::UTCString timestamp = "";
  startingTimestamp_.toUTCString( timestamp );

  const char* const formatScalar =
    "REGRIDDED-SITE 2.0\n"
    "%s\n"
    "%s\n"
    "# timesteps\n%" LU "\n"
    "# Variable name:\n%s\n"
    "# Variable units:\n%s\n";
  const char* const formatVector =
    "REGRIDDED-SITE 2.0\n"
    "%s\n"
    "%s\n"
    "# timesteps\n%" LU "\n"
    "# Variable name:\n%s %s\n"
    "# Variable units:\n%s %s\n";
  const char* const formatRemainder =
    "# MSB 32-bit integers points[timesteps] and\n"
    "# IEEE-754 32-bit reals longitudes[timesteps][points] and\n"
    "# IEEE-754 32-bit reals latitudes[timesteps][points] and\n"
    "# MSB 32-bit integers columns[timesteps][points] and\n"
    "# MSB 32-bit integers rows[timesteps][points] and\n"
    "# IEEE-754 32-bit reals data[timesteps][points]:\n";
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
      writeIntegers(stream, 0, INT_MAX, pointsPerTimestep_ ); //x
      writeRegridded32BitCoordinatesAndVariables( stream ); //x      
    }
  }

  if ( ! ok ) {
    throw std::runtime_error( "Failed to write regridded site data." ); //x
  }

  POST( true );
}



} // namespace emvl.



