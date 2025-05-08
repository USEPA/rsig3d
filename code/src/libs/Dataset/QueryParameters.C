/******************************************************************************
PURPOSE: QueryParameters.C - Web service query parameters.
NOTES:
HISTORY: 2012-11-20 plessel.todd@epa.gov
******************************************************************************/


//================================ INCLUDES ===================================

#include <string.h> // For memset(), strcmp().

#include <string> // For std::string.

#include <Assertions.h>           // For PRE0*(), POST0*(), AND?().
#include <Bounds.h>               // For Bounds.
#include <Timestamp.h>            // For Timestamp.
#include <DataUtilities.h>        // For MISSING, IMISSING, MINIMUM, MAXIMUM.
#include <DataFilterParameters.h> // For DataFilterParameters.
#include <CMAQGridParameters.h>   // For CMAQGridParameters.
#include <QueryParameters.h>      // For public interface.

namespace emvl {


//============================= GLOBAL VARIABLES ==============================

const double QueryParameters::MINIMUM_ELEVATION = -500.0; // Dead Sea (m).
const double QueryParameters::MAXIMUM_ELEVATION = 1e5; // Top of atmosphere (m)

const char* const QueryParameters::fileFormatNames[ FILE_FORMATS ] = {
  "xdr", "ascii", "netcdf-coards", "netcdf-ioapi", "shapefile", "kml","original"
};

const char* const QueryParameters::fileFormatExtensions[ FILE_FORMATS ] = {
  ".xdr", ".txt", ".nc", ".ncf", ".shp", ".kml", ".tgz"
};

const char* const QueryParameters::aggregateMethodNames[ AGGREGATE_METHODS] = {
  "none", "daily_mean", "daily_max", "daily_max8", "hourly", "daily"
};

const char* const QueryParameters::regridMethodNames[ REGRID_METHODS ] = {
  "none", "nearest", "mean", "weighted"
};

const char* const
QueryParameters::regridAggregateMethodNames[ REGRID_AGGREGATE_METHODS ] = {
  "none", "all", "daily"
};

//============================= PUBLIC FUNCTIONS ==============================



bool isElevation( const double elevation ) {
  return IN_RANGE( elevation,
                   QueryParameters::MINIMUM_ELEVATION,
                   QueryParameters::MAXIMUM_ELEVATION );
}



/******************************************************************************
PURPOSE: ~QueryParameters - Destructor.
******************************************************************************/

QueryParameters::~QueryParameters() {
  PRE( true );
  boundsZ_[ MINIMUM ] = boundsZ_[ MAXIMUM ] = 0.0;
  format_ = 0;
  getMetadata_ = false;
  compress_ = false;
  integrateCMAQLayers_ = false;
  aggregateMethod_ = 0;
  regridMethod_ = 0;
  regridAggregateMethod_ = 0;
}



/******************************************************************************
PURPOSE: QueryParameters - Default constructor.
******************************************************************************/

QueryParameters::QueryParameters() {

  startingTimestamp_.yyyymmddhhmmss( 20060703, 0 );
  endingTimestamp_.yyyymmddhhmmss( 20060703, 10000 );
  boundsZ_[ MINIMUM ] = boundsZ_[ MAXIMUM ] = 0.0;
  variables_ = "aqs.pm25";
  format_ = 0;
  directory_ = ".";
  getMetadata_ = false;
  compress_ = false;
  integrateCMAQLayers_ = false;
  aggregateMethod_ = 0;
  regridMethod_ = 0;
  regridAggregateMethod_ = 0;

  POST19( startingTimestamp().yyyymmdd() == 20060703,
          startingTimestamp().hhmmss() == 0,
          endingTimestamp().yyyymmdd() == 20060703,
          endingTimestamp().hhmmss() == 10000,
          bounds().west() == -180.0,
          bounds().east() ==  180.0,
          bounds().south() == -90.0,
          bounds().north() ==   90.0,
          boundsZMinimum() == 0.0,
          boundsZMaximum() == 0.0,
          variables() == "aqs.pm25",
          format() == XDR,
          directory() == ".",
          getMetadata() == false,
          compress() == false,
          integrateCMAQLayers() == false,
          aggregateMethod() == AGGREGATE_NONE,
          regridMethod() == REGRID_NONE,
          regridAggregateMethod() == REGRID_AGGREGATE_NONE );
}



/******************************************************************************
PURPOSE: QueryParameters - Contructor.
******************************************************************************/

QueryParameters::QueryParameters(
  const Timestamp startingTimestamp,
  const Timestamp endingTimestamp,
  const Bounds& bounds,
  const double boundsZ[ 2 ],
  const std::string& variables,
  const int format,
  const std::string& directory,
  const bool getMetadata,
  const bool compress,
  const bool integrateCMAQLayers,
  const DataFilterParameters& dataFilterParameters,
  const int aggregateMethod,
  const int regridMethod,
  const int regridAggregateMethod,
  const CMAQGridParameters& cmaqGridParameters ) {

  PRE015( startingTimestamp.invariant(),
          endingTimestamp.invariant(),
          startingTimestamp <= endingTimestamp,
          bounds.invariant(),
          boundsZ,
          OR2( AND2( boundsZ[ MINIMUM ] == MISSING,
                     boundsZ[ MAXIMUM ] == MISSING ),
               AND2( IN_RANGE( boundsZ[ MINIMUM ],
                               MINIMUM_ELEVATION, MAXIMUM_ELEVATION ),
                     IN_RANGE( boundsZ[ MAXIMUM ],
                               boundsZ[ MINIMUM ], MAXIMUM_ELEVATION ) ) ),
          variables.length() >= 3,
          IN_RANGE( format, 0, FILE_FORMATS - 1 ),
          directory.length() > 0,
          isDirectory( directory.c_str() ),
          dataFilterParameters.invariant(),
          IN_RANGE( aggregateMethod, 0, AGGREGATE_METHODS - 1 ),
          IN_RANGE( regridMethod, 0, REGRID_METHODS - 1 ),
          IN_RANGE( regridAggregateMethod, 0, REGRID_AGGREGATE_METHODS - 1 ),
          cmaqGridParameters.invariant() );

  startingTimestamp_    = startingTimestamp;
  endingTimestamp_      = endingTimestamp;
  bounds_               = bounds;
  boundsZ_[ MINIMUM ]   = boundsZ[ MINIMUM ];
  boundsZ_[ MAXIMUM ]   = boundsZ[ MAXIMUM ];
  variables_            = variables;
  format_               = format;
  directory_            = directory;
  trimSlashes( directory_ );
  getMetadata_          = getMetadata;
  compress_             = compress;
  integrateCMAQLayers_  = integrateCMAQLayers;
  dataFilterParameters_ = dataFilterParameters;
  aggregateMethod_      = aggregateMethod;
  regridMethod_         = regridMethod;
  regridAggregateMethod_ = regridAggregateMethod;
  cmaqGridParameters_   = cmaqGridParameters;

  POST16( this->startingTimestamp() == startingTimestamp,
          this->endingTimestamp() == endingTimestamp,
          this->bounds() == bounds,
          this->boundsZMinimum() == boundsZ[ MINIMUM ],
          this->boundsZMaximum() == boundsZ[ MAXIMUM ],
          this->variables() == variables,
          this->format() == format,
          isDirectory( this->directory() ),
          this->getMetadata() == getMetadata,
          this->compress() == compress,
          this->integrateCMAQLayers() == integrateCMAQLayers,
          this->dataFilterParameters() == dataFilterParameters,
          this->aggregateMethod() == aggregateMethod,
          this->regridMethod() == regridMethod,
          this->regridAggregateMethod() == regridAggregateMethod,
          this->cmaqGridParameters() == cmaqGridParameters );
}



/******************************************************************************
PURPOSE: QueryParameters - Copy constructor.
INPUTS:  const QueryParameters& other  Object to copy.
******************************************************************************/

QueryParameters::QueryParameters( const QueryParameters& other ) {
  PRE0( other.invariant() );
  startingTimestamp_    = other.startingTimestamp_;
  endingTimestamp_      = other.endingTimestamp_;
  bounds_               = other.bounds_;
  boundsZ_[ MINIMUM ]   = other.boundsZ_[ MINIMUM ];
  boundsZ_[ MAXIMUM ]   = other.boundsZ_[ MAXIMUM ];
  variables_            = other.variables_;
  format_               = other.format_;
  directory_            = other.directory_;
  getMetadata_          = other.getMetadata_;
  compress_             = other.compress_;
  integrateCMAQLayers_  = other.integrateCMAQLayers_;
  dataFilterParameters_ = other.dataFilterParameters_;
  aggregateMethod_      = other.aggregateMethod_;
  regridMethod_         = other.regridMethod_;
  regridAggregateMethod_ = other.regridAggregateMethod_;
  cmaqGridParameters_   = other.cmaqGridParameters_;
  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Copy object.
INPUTS:  const QueryParameters& other  Object to copy.
******************************************************************************/

QueryParameters& QueryParameters::operator=( const QueryParameters& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    startingTimestamp_    = other.startingTimestamp_;
    endingTimestamp_      = other.endingTimestamp_;
    bounds_               = other.bounds_;
    boundsZ_[ MINIMUM ]   = other.boundsZ_[ MINIMUM ];
    boundsZ_[ MAXIMUM ]   = other.boundsZ_[ MAXIMUM ];
    variables_            = other.variables_;
    format_               = other.format_;
    directory_            = other.directory_;
    getMetadata_          = other.getMetadata_;
    compress_             = other.compress_;
    integrateCMAQLayers_  = other.integrateCMAQLayers_;
    dataFilterParameters_ = other.dataFilterParameters_;
    aggregateMethod_      = other.aggregateMethod_;
    regridMethod_         = other.regridMethod_;
    regridAggregateMethod_ = other.regridAggregateMethod_;
    cmaqGridParameters_   = other.cmaqGridParameters_;
  }

  POST( *this == other );
  return *this;
}



/******************************************************************************
PURPOSE: timestamps - Set starting and ending timestamps.
INPUTS:  const Timestamp startingTimestamp  Starting timestamp to copy.
         const Timestamp endingTimestamp    Ending   timestamp to copy.
******************************************************************************/

void QueryParameters::timestamps( const Timestamp startingTimestamp,
                                  const Timestamp endingTimestamp ) {
  PRE3( startingTimestamp.invariant(),
        endingTimestamp.invariant(),
        startingTimestamp <= endingTimestamp );

  startingTimestamp_ = startingTimestamp;
  endingTimestamp_   = endingTimestamp;

  POST2( this->startingTimestamp() == startingTimestamp,
         this->endingTimestamp() == endingTimestamp );
}



/******************************************************************************
PURPOSE: bounds - Set bounds.
INPUTS:  const Bounds& bounds  Bounds to copy.
******************************************************************************/

void QueryParameters::bounds( const Bounds& bounds ) {
  PRE( bounds.invariant() );
  bounds_ = bounds;
  POST( this->bounds() == bounds );
}



/******************************************************************************
PURPOSE: boundsZ - Set boundsZ.
INPUTS:  const double boundsZ[ 2 ]  BoundsZ to copy.
******************************************************************************/

void QueryParameters::boundsZ( const double boundsZ[ 2 ] ) {
  PRE2( boundsZ,
        OR2( AND2( boundsZ[ 0 ] == MISSING,
                   boundsZ[ 1 ] == MISSING ),
             AND2( IN_RANGE( boundsZ[ 0 ],
                             MINIMUM_ELEVATION, MAXIMUM_ELEVATION ),
                   IN_RANGE( boundsZ[ 1 ],
                             boundsZ[ 0 ], MAXIMUM_ELEVATION ) ) ) );
  boundsZ_[ MINIMUM ] = boundsZ[ 0 ];
  boundsZ_[ MAXIMUM ] = boundsZ[ 1 ];
  POST2( boundsZMinimum() == boundsZ[ 0 ],
         boundsZMaximum() == boundsZ[ 1 ] );
}



/******************************************************************************
PURPOSE: variables - Set variables.
INPUTS:  const std::string& variables  Variables to copy.
******************************************************************************/

void QueryParameters::variables( const std::string& variables ) {
  PRE( variables.length() >= 3 );
  variables_ = variables;
  POST( this->variables() == variables );
}



/******************************************************************************
PURPOSE: format - Set format.
INPUTS:  const int format  Variables to copy.
******************************************************************************/

void QueryParameters::format( const int format ) {
  PRE( IN_RANGE( format, 0, FILE_FORMATS - 1 ) );
  format_ = format;
  POST( this->format() == format );
}



/******************************************************************************
PURPOSE: directory - Set directory.
INPUTS:  const std::string& directory  Directory to copy.
******************************************************************************/

void QueryParameters::directory( const std::string& directory ) {
  PRE( isDirectory( directory ) );
  directory_ = directory;
  trimSlashes( directory_ );
}



/******************************************************************************
PURPOSE: getMetadata - Set getMetadata.
INPUTS:  const bool getMetadata  Get metadata flag.
******************************************************************************/

void QueryParameters::getMetadata( const bool getMetadata ) {
  PRE( true );
  getMetadata_ = getMetadata;
  POST( this->getMetadata() == getMetadata );
}



/******************************************************************************
PURPOSE: compress - Set compress.
INPUTS:  const bool compress  Compress flag.
******************************************************************************/

void QueryParameters::compress( const bool compress ) {
  PRE( true );
  compress_ = compress;
  POST( this->compress() == compress );
}



/******************************************************************************
PURPOSE: integrateCMAQLayers - Set integrate.
INPUTS:  const bool integrate  Integrate flag.
******************************************************************************/

void QueryParameters::integrateCMAQLayers( const bool integrate ) {
  PRE( true );
  integrateCMAQLayers_ = integrate;
  POST( this->integrateCMAQLayers() == integrate );
}



/******************************************************************************
PURPOSE: dataFilterParameters - Set dataFilterParameters.
INPUTS:  const DataFilterParameters& dataFilterParameters  Argument to copy.
******************************************************************************/

void QueryParameters::dataFilterParameters( const DataFilterParameters&
                                              dataFilterParameters ) {
  PRE( dataFilterParameters.invariant() );
  dataFilterParameters_ = dataFilterParameters;
  POST( this->dataFilterParameters() == dataFilterParameters );
}



/******************************************************************************
PURPOSE: aggregateMethod - Set aggregateMethod.
INPUTS:  const int aggregateMethod  aggregateMethod to copy.
******************************************************************************/

void QueryParameters::aggregateMethod( const int aggregateMethod ) {
  PRE( IN_RANGE( aggregateMethod, 0, AGGREGATE_METHODS - 1 ) );
  aggregateMethod_ = aggregateMethod;
  POST( this->aggregateMethod() == aggregateMethod );
}



/******************************************************************************
PURPOSE: regridMethod - Set regridMethod.
INPUTS:  const int regridMethod  regridMethod to copy.
******************************************************************************/

void QueryParameters::regridMethod( const int regridMethod ) {
  PRE( IN_RANGE( regridMethod, 0, REGRID_METHODS - 1 ) );
  regridMethod_ = regridMethod;
  POST( this->regridMethod() == regridMethod );
}



/******************************************************************************
PURPOSE: regridAggregateMethod - Set regridAggregateMethod.
INPUTS:  const int regridAggregateMethod  regridAggregateMethod to copy.
******************************************************************************/

void QueryParameters::regridAggregateMethod( const int regridAggregateMethod) {
  PRE( IN_RANGE( regridAggregateMethod, 0, REGRID_AGGREGATE_METHODS - 1 ) );
  regridAggregateMethod_ = regridAggregateMethod;
  POST( this->regridAggregateMethod() == regridAggregateMethod );
}



/******************************************************************************
PURPOSE: cmaqGridParameters - Set cmaqGridParameters.
INPUTS:  const CMAQGridParameters& cmaqGridParameters  Argument to copy.
******************************************************************************/

void QueryParameters::cmaqGridParameters( const CMAQGridParameters&
                                            cmaqGridParameters ) {
  PRE( cmaqGridParameters.invariant() );
  cmaqGridParameters_ = cmaqGridParameters;
  POST( this->cmaqGridParameters() == cmaqGridParameters );
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const QueryParameters& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool QueryParameters::operator==( const QueryParameters& other ) const {
  PRE( other.invariant() );
  const bool result =
    AND16( startingTimestamp_    == other.startingTimestamp_,
           endingTimestamp_      == other.endingTimestamp_,
           bounds_               == other.bounds_,
           boundsZ_[ MINIMUM ]   == other.boundsZ_[ MINIMUM ],
           boundsZ_[ MAXIMUM ]   == other.boundsZ_[ MAXIMUM ],
           variables_            == other.variables_,
           format_               == other.format_,
           directory_            == other.directory_,
           getMetadata_          == other.getMetadata_,
           compress_             == other.compress_,
           integrateCMAQLayers_  == other.integrateCMAQLayers_,
           dataFilterParameters_ == other.dataFilterParameters_,
           aggregateMethod_      == other.aggregateMethod_,
           regridMethod_         == other.regridMethod_,
           regridAggregateMethod_ == other.regridAggregateMethod_,
           cmaqGridParameters_   == other.cmaqGridParameters_ );
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are objects unequal?
INPUTS:  const QueryParameters& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool QueryParameters::operator!=( const QueryParameters& other ) const {
  PRE( other.invariant() );
  const bool result = ! this->operator==( other );
  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true or else there is a bug.
******************************************************************************/

bool QueryParameters::invariant() const {
  bool result = true;

  for ( size_t index = 0; AND2( result, index < FILE_FORMATS ); ++index ) {
    result = fileFormatNames[ index ] != 0;
    result = AND2( result, fileFormatExtensions[ index ] != 0 );
  }

  for ( size_t index = 0; AND2( result, index < AGGREGATE_METHODS ); ++index) {
    result = aggregateMethodNames[ index ] != 0;
  }

  for ( size_t index = 0; AND2( result, index < REGRID_METHODS ); ++index ) {
    result = regridMethodNames[ index ] != 0;
  }

  for ( size_t index = 0; AND2( result, index < REGRID_AGGREGATE_METHODS );
        ++index ) {
    result = regridAggregateMethodNames[ index ] != 0;
  }

  result =
    AND16( result,
           startingTimestamp_.invariant(),
           endingTimestamp_.invariant(),
           startingTimestamp_ <= endingTimestamp_,
           bounds_.invariant(),
           boundsZ_,
           OR2( AND2( boundsZ_[ MINIMUM ] == MISSING,
                      boundsZ_[ MAXIMUM ] == MISSING ),
                AND2( IN_RANGE( boundsZ_[ MINIMUM ],
                                MINIMUM_ELEVATION, MAXIMUM_ELEVATION ),
                      IN_RANGE( boundsZ_[ MAXIMUM ],
                                boundsZ_[ MINIMUM ], MAXIMUM_ELEVATION ) ) ),
           variables_.length() >= 3,
           IN_RANGE( format_, 0, FILE_FORMATS ),
           directory_.length() > 0,
           isDirectory( directory_.c_str() ),
           dataFilterParameters_.invariant(),
           IN_RANGE( aggregateMethod_, 0, AGGREGATE_METHODS - 1 ),
           IN_RANGE( regridMethod_, 0, REGRID_METHODS - 1 ),
           IN_RANGE( regridAggregateMethod_, 0, REGRID_AGGREGATE_METHODS - 1 ),
           cmaqGridParameters_.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: startingTimestamp - Starting timestamp.
RETURNS: Timestamp starting timestamp.
******************************************************************************/

Timestamp QueryParameters::startingTimestamp() const {
  PRE( true );
  const Timestamp result = startingTimestamp_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: endingTimestamp - Ending timestamp.
RETURNS: Timestamp ending timestamp.
******************************************************************************/

Timestamp QueryParameters::endingTimestamp() const {
  PRE( true );
  const Timestamp result = endingTimestamp_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: bounds - Bounds.
RETURNS: const Bounds& bounds.
******************************************************************************/

const Bounds& QueryParameters::bounds() const {
  PRE( true );
  const Bounds& result = bounds_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: boundsZMinimum - BoundsZ Minimum.
RETURNS: double boundsZ Minimum.
******************************************************************************/

double QueryParameters::boundsZMinimum() const {
  PRE( true );
  const double result = boundsZ_[ MINIMUM ];
  POST( OR2( result == MISSING,
             IN_RANGE( result, MINIMUM_ELEVATION, MAXIMUM_ELEVATION ) ) );
  return result;
}



/******************************************************************************
PURPOSE: boundsZMaximum - BoundsZ Maximum.
RETURNS: double boundsZ Maximum.
******************************************************************************/

double QueryParameters::boundsZMaximum() const {
  PRE( true );
  const double result = boundsZ_[ MAXIMUM ];
  POST( OR2( result == MISSING,
             IN_RANGE( result, MINIMUM_ELEVATION, MAXIMUM_ELEVATION ) ) );
  return result;
}



/******************************************************************************
PURPOSE: variables - Variables.
RETURNS: const std::string& variables.
******************************************************************************/

const std::string& QueryParameters::variables() const {
  PRE( true );
  const std::string& result = variables_;
  POST( result.length() >= 3 );
  return result;
}



/******************************************************************************
PURPOSE: format - Format.
RETURNS: int format.
******************************************************************************/

int QueryParameters::format() const {
  PRE( true );
  const int result = format_;
  POST( IN_RANGE( result, 0, FILE_FORMATS - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: directory - Directory to write files.
RETURNS: const std::string& directory.
******************************************************************************/

const std::string& QueryParameters::directory() const {
  PRE( true );
  const std::string& result = directory_;
  POST( isDirectory( result ) );
  return result;
}



/******************************************************************************
PURPOSE: getMetadata - Get metadata?
RETURNS: bool getMetadata.
******************************************************************************/

bool QueryParameters::getMetadata() const {
  PRE( true );
  const bool result = getMetadata_;
  return result;
}



/******************************************************************************
PURPOSE: compress - Compress data over the network?
RETURNS: bool compress.
******************************************************************************/

bool QueryParameters::compress() const {
  PRE( true );
  const bool result = compress_;
  return result;
}



/******************************************************************************
PURPOSE: integrateCMAQLayers - Integrate CMAQ data over layers?
RETURNS: bool integrate.
******************************************************************************/

bool QueryParameters::integrateCMAQLayers() const {
  PRE( true );
  const bool result = integrateCMAQLayers_;
  return result;
}



/******************************************************************************
PURPOSE: dataFilterParameters - DataFilterParameters.
RETURNS: const DataFilterParameters&  dataFilterParameters.
******************************************************************************/

const DataFilterParameters& QueryParameters::dataFilterParameters() const {
  PRE( true );
  const DataFilterParameters& result = dataFilterParameters_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: aggregateMethod - Aggregate method.
RETURNS: int aggregate method.
******************************************************************************/

int QueryParameters::aggregateMethod() const {
  PRE( true );
  const int result = aggregateMethod_;
  POST( IN_RANGE( result, 0, AGGREGATE_METHODS - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: regridMethod - Regrid method.
RETURNS: int regrid method.
******************************************************************************/

int QueryParameters::regridMethod() const {
  PRE( true );
  const int result = regridMethod_;
  POST( IN_RANGE( result, 0, REGRID_METHODS - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: regridAggregateMethod - Regrid aggregate method.
RETURNS: int regrid aggreagte method.
******************************************************************************/

int QueryParameters::regridAggregateMethod() const {
  PRE( true );
  const int result = regridAggregateMethod_;
  POST( IN_RANGE( result, 0, REGRID_AGGREGATE_METHODS - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: cmaqGridParameters - CMAQGridParameters.
RETURNS: const CMAQGridParameters&  cmaqGridParameters.
******************************************************************************/

const CMAQGridParameters& QueryParameters::cmaqGridParameters() const {
  PRE( true );
  const CMAQGridParameters& result = cmaqGridParameters_;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: pathedFileName - Create pathed file name used for saving data.
RETURNS: std::string result  Pathed file name.
NOTES:   Throws an exception if there is not enough memory for result.
******************************************************************************/

std::string QueryParameters::pathedFileName() const {
  PRE( directory().length() >= 1 );
  std::string result;
  result.reserve( directory_.length() + variables_.length() + 64 ); //x
  result = directory_ + slash; // '/';

  if ( regridMethod_ != REGRID_NONE ) {
    result += "regridded_";

    if ( AND2( regridAggregateMethod_ == REGRID_AGGREGATE_DAILY,
               ! strstr( variables_.c_str(), "daily_" ) ) ) {
      result += "daily_";
    } else if ( regridAggregateMethod_ == REGRID_AGGREGATE_ALL ) {
      result += "mean_";
    }
  } else if ( aggregateMethod_ == AGGREGATE_DAILY_MEAN ) {
    result += "daily_mean_";
  } else if ( aggregateMethod_ == AGGREGATE_DAILY_MAX ) {
    result += "daily_max_";
  } else if ( aggregateMethod_ == AGGREGATE_DAILY_MAX8 ) {
    result += "daily_max8_";
  } else if ( aggregateMethod_ == AGGREGATE_HOURLY ) {
    result += "hourly_";
  } else if ( aggregateMethod_ == AGGREGATE_DAILY ) {
    result += "daily_";
  }

  // File name starts with the variables, but changes each [.,] to [_+]:

  const size_t length = variables_.length();

  for ( size_t index = 0; index < length; ++index ) {
    const char c = variables_[ index ];

    if ( c == '.' ) {
      result += '_';
    } else if (  c == ',' ) {
      result += '+';
    } else {
      result += c;
    }
  }

  // Also include date range:

  Timestamp::UTCString startTime = "";
  Timestamp::UTCString endTime = "";
  startingTimestamp_.toUTCString( startTime );
  endingTimestamp_.toUTCString( endTime );
  startTime[ 10 ] = '\0'; // Truncate to YYYY-MM-DD.
  endTime[   10 ] = '\0'; // Truncate to YYYY-MM-DD.
  result += '_';
  result += startTime;

  if ( strcmp( startTime, endTime ) ) {
    result += "_to_";
    result += endTime;    
  }

  // Add extension(s):

  if ( getMetadata_ ) {
    result += '_'; // Use underscore instead of dot.
    result += fileFormatExtensions[ format_ ] + 1; // Skip dot.
    result += "_metadata.txt";
  } else {
    result += fileFormatExtensions[ format_ ];
  }

  //  HACK for already regridded data:

  if ( result.find( "regridded_regridded_" ) != std::string::npos ) {
    result.erase( 0, strlen( "regridded_" ) );
  }

  POST( result.length() >= 18 ); // E.g., "./s_v_2006-07-03.e".

  return result;
}



} // End namespace emvl.


