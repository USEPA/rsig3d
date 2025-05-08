/******************************************************************************
PURPOSE: DatasetManager.C - Sets-up access to webservice and creates and owns
         a collection of Datasets.
HISTORY: 2013-03-19 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdlib.h>   // For popen(), system(), getenv(), atoi().
#include <stdio.h>    // For FILE, fgets(), fscanf(), snprintf(), gethostname()
#include <string.h>   // For strlen(), strcmp(), memset().
#include <ctype.h>    // For tolower(), isalnum().
#include <unistd.h>   // For unlink().
#include <sys/stat.h> // For mkdir().

#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <algorithm> // For std::find.
#include <stdexcept> // For std::runtime_error.

#include <Assertions.h>               // For PRE(), POST(), AND(), IN_RANGE().
#include <Bounds.h>                   // For emvl::Bounds.
#include <DataUtilities.h>            // For emvl::readCommandResult().
#include <Timestamp.h>                // For Timestamp.
#include <ResourceHandler.h>          // For ResourceHandler.
#include <QueryParameters.h>          // For QueryParameters.
#include <Dataset.h>                  // For Dataset.
#include <PointDataset.h>             // For PointDataset.
#include <RegriddedPointDataset.h>    // For RegriddedPointDataset.
#include <SiteDataset.h>              // For SiteDataset.
#include <RegriddedSiteDataset.h>     // For RegriddedSiteDataset.
#include <CMAQDataset.h>              // For CMAQDataset.
#include <SwathDataset.h>             // For SwathDataset.
#include <RegriddedSwathDataset.h>    // For RegriddedSwathDataset.
#include <CALIPSODataset.h>           // For CALIPSODataset.
#include <RegriddedCALIPSODataset.h>  // For RegriddedCALIPSODataset.
#include <AircraftDataset.h>          // For AircraftDataset.
#include <RegriddedAircraftDataset.h> // For RegriddedAircraftDataset.
#include <ProfileDataset.h>           // For ProfileDataset.
#include <RegriddedProfileDataset.h>  // For RegriddedProfileDataset.
#include <GridDataset.h>              // For GridDataset.
#include <RegriddedGridDataset.h>     // For RegriddedGridDataset.
#include <PolygonDataset.h>           // For PolygonDataset.
#include <HTTPConnection.h>           // For HTTPConnection.
#include <DatasetManager.h>           // For public interface.

namespace emvl {

//================================== PRIVATE ==================================

/*
 * URL of WCS web service.
 * INTERNAL_SERVER is fastest but only accessible from within the EPA Network.
 * EXTERNAL_SERVER is publicly accessible but slower than INTERNAL.
 */

#define INTERNAL_SERVER "https://maple.hesc.epa.gov/rsig/rsigserver?"
#define EXTERNAL_SERVER "https://ofmpub.epa.gov/rsig/rsigserver?"

/*
 * The curl program is used to query the webservice.
 * It should be in the same directory (or subdirectory) as the client program
 * (e.g., RSIG3D) executable.
 */

static const char* const curlCommand =
  "curl -k --silent --retry 0 -L --tcp-nodelay --max-time"; // Timeout last.

/*
 * The gzip program is used to decompress the bytes stream from the webservice.
 * It must be in the same directory as the curl program executable.
 */

static const char* const decompressCommand = "gzip -d";


// For running CMAQSubset on local or remote files:

static const char* const subsetter = "CMAQSubset";
static const char* const ssh = "ssh -q"; // -q thwarts MOD banner!
static const char* const scp = "scp -p"; // -p preserves permissions/timestamp.

static std::string localSubsetterDirectory;  // Local path to subsetter.
static std::string remoteSubsetterDirectory; // Remote path/subdir to subsetter

static std::vector<std::string> installedHosts; // Hosts w/ installed subsetter

/*
 * When checkAndSetServerHost() is called,
 * server will be initialized to point to one of (in order of preference):
 * INTERNAL_SERVER or EXTERNAL_SERVER.
 */

static const char* server = 0;
static int remoteYYYYMMDD = 0; // From remote file download/RSIG3D_VERSION.

static char pathWithoutSpaces[ 256 ] = ""; // To programs curl, gzip, ssh, scp.
static char pathedCurlCommand[ 1024 ] = "."; // /full/path/curlCommand.
static char pathedDecompressCommand[ 1024 ] = "."; // If used.

typedef char URLString[ 1024 ]; // HTTP OGC-WCS query string.


/*
 * Dispatch table used by retrieveDataset() and readDataset().
 * To add a new dataset type MyDataset:
 *  Add #include <MyDataset.h> and <MyRegriddedDataset.h> above
 *  Add 4 new routines below:
 *    newMyDataset(FILE* s)
 *    newRegriddedMyDataset(FILE* s)
 *    newMyDataset(const std::string& f)
 *    newRegriddedMyDataset(const std::string& f)
 *  Add entries in table below
 */

static Dataset* newPointDataset(FILE* s) {return new PointDataset(s);}
static Dataset* newSiteDataset(FILE* s) {return new SiteDataset(s);}
static Dataset* newCMAQDataset(FILE* s) {return new CMAQDataset(s);}
static Dataset* newSwathDataset(FILE* s) {return new SwathDataset(s);}
static Dataset* newCALIPSODataset(FILE* s) {return new CALIPSODataset(s);}
static Dataset* newAircraftDataset(FILE* s) {return new AircraftDataset(s);}
static Dataset* newProfileDataset(FILE* s) {return new ProfileDataset(s);}
static Dataset* newGridDataset(FILE* s) {return new GridDataset(s);}
static Dataset* newPolygonDataset(FILE* s) {return new PolygonDataset(s);}

static Dataset*
newRegriddedPointDataset(FILE* s) {return new RegriddedPointDataset(s);}
static Dataset*
newRegriddedSiteDataset(FILE* s) {return new RegriddedSiteDataset(s);}
static Dataset*
newRegriddedSwathDataset(FILE* s) {return new RegriddedSwathDataset(s);}
static Dataset*
newRegriddedCALIPSODataset(FILE* s) {return new RegriddedCALIPSODataset(s);}
static Dataset*
newRegriddedAircraftDataset(FILE* s) {return new RegriddedAircraftDataset(s);}
static Dataset*
newRegriddedProfileDataset(FILE* s) {return new RegriddedProfileDataset(s);}
static Dataset*
newRegriddedGridDataset(FILE* s) {return new RegriddedGridDataset(s);}


static Dataset*
newPointDataset(const std::string& f) {return new PointDataset(f);}
static Dataset*
newSiteDataset(const std::string& f) {return new SiteDataset(f);}
static Dataset*
newCMAQDataset(const std::string& f) {return new CMAQDataset(f);}
static Dataset*
newSwathDataset(const std::string& f) {return new SwathDataset(f);}
static Dataset*
newCALIPSODataset(const std::string& f) {return new CALIPSODataset(f);}
static Dataset*
newAircraftDataset(const std::string& f) {return new AircraftDataset(f);}
static Dataset*
newProfileDataset(const std::string& f) {return new ProfileDataset(f);}
static Dataset*
newGridDataset(const std::string& f) {return new GridDataset(f);}
static Dataset*
newPolygonDataset(const std::string& f) {return new PolygonDataset(f);}

static Dataset* newRegriddedPointDataset(const std::string& f)
{return new RegriddedPointDataset(f);}
static Dataset* newRegriddedSiteDataset(const std::string& f)
{return new RegriddedSiteDataset(f);}
static Dataset* newRegriddedSwathDataset(const std::string& f)
{return new RegriddedSwathDataset(f);}
static Dataset* newRegriddedCALIPSODataset(const std::string& f)
{return new RegriddedCALIPSODataset(f);}
static Dataset* newRegriddedAircraftDataset(const std::string& f)
{return new RegriddedAircraftDataset(f);}
static Dataset* newRegriddedProfileDataset(const std::string& f)
{return new RegriddedProfileDataset(f);}
static Dataset* newRegriddedGridDataset(const std::string& f)
{return new RegriddedGridDataset(f);}


typedef Dataset* (*CreatorFromStream)( FILE* );
typedef Dataset* (*CreatorFromFile)( const std::string& );

typedef struct {
  const char* const line;
  CreatorFromStream creatorFromStream;
  CreatorFromFile creatorFromFile;
} Entry;

static const Entry table[] = {
  { "SUBSET 9.0 CMAQ\n",        newCMAQDataset, newCMAQDataset },
  { "Point 1.0\n",              newPointDataset, newPointDataset },
  { "SITE 2.0\n",               newSiteDataset, newSiteDataset },
  { "Swath 2.0\n",              newSwathDataset, newSwathDataset },
  { "CALIPSO 1.0\n",            newCALIPSODataset, newCALIPSODataset },
  { "Aircraft 2.0\n",           newAircraftDataset, newAircraftDataset },
  { "Profile 2.0\n",            newProfileDataset, newProfileDataset },
  { "Grid 1.0\n",               newGridDataset, newGridDataset },
  { "Polygon 1.0\n",            newPolygonDataset, newPolygonDataset },
  { "REGRIDDED-Point 1.0\n", newRegriddedPointDataset, newRegriddedPointDataset },
  { "REGRIDDED-SITE 2.0\n", newRegriddedSiteDataset, newRegriddedSiteDataset },
  { "REGRIDDED-Swath 2.0\n",newRegriddedSwathDataset,newRegriddedSwathDataset},
  { "REGRIDDED-CALIPSO 2.0\n",
    newRegriddedCALIPSODataset, newRegriddedCALIPSODataset },
  { "REGRIDDED-Aircraft 3.0\n",
    newRegriddedAircraftDataset, newRegriddedAircraftDataset },
  { "REGRIDDED-Profile 2.0\n",
    newRegriddedProfileDataset, newRegriddedProfileDataset },
  { "REGRIDDED-Grid 1.0\n",
    newRegriddedGridDataset, newRegriddedGridDataset },
  { 0, 0, 0 } // End of table.
};

static const Entry& lookupCreator( const char* const line ) {
  PRE02( line, *line );
  const size_t entries = sizeof table / sizeof *table - 1;
  size_t index = 0;
  bool found = false;

  do {
    const Entry* const entry = table + index;
    
    if ( AND2( entry->line, ! strcmp( line, entry->line ) ) ) {
      found = true;
    } else {
      ++index;
    }

  } while ( AND2( ! found, index < entries ) );

  CHECK( index < sizeof table / sizeof *table );
  return table[ index ];
}

static CreatorFromStream lookupCreatorFromStream( const char* const line ) {
  const Entry& entry = lookupCreator( line );
  CreatorFromStream result = entry.creatorFromStream;
  return result;
}

static CreatorFromFile lookupCreatorFromFile( const char* const line ) {
  const Entry& entry = lookupCreator( line );
  CreatorFromFile result = entry.creatorFromFile;
  return result;
}


//=========================== FORWARD DECLARATIONS ============================

enum { PURPLE_AIR_KEY, TEMPO_KEY, OAQPS_KEY, KEYS };

static void checkKeyURLString( const bool printURL,
                               const int keySource,
                               const std::string& key,
                               URLString url );

static void toURLString( const bool printURL,
                         const std::string& remoteFilename,
                         URLString url );

static void toURLString( const bool printURL,
                         const QueryParameters& queryParameters,
                         URLString url );

static void appendGASPParameters( const QueryParameters& queryParameters,
                                  URLString url );

static void appendCALIPSOParameters( const QueryParameters& queryParameters,
                                     URLString url );

static void appendTROPOMIParameters( const QueryParameters& queryParameters,
                                     URLString url );

static void appendVIIRSNOAAParameters( const QueryParameters& queryParameters,
                                       URLString url );

static void appendPandoraParameters( const QueryParameters& queryParameters,
                                     URLString url );

static void appendPurpleAirParameters( const QueryParameters& queryParameters,
                                       const char* const coverage,
                                       URLString url );

static void appendTEMPOParameters( const QueryParameters& queryParameters,
                                   URLString url );

static void appendOMIParameters( const QueryParameters& queryParameters,
                                 URLString url );

static void appendOAQPSParameters( const QueryParameters& queryParameters,
                                   URLString url );

static void appendRegriddingParameters( const QueryParameters& queryParameters,
                                        URLString url );

static void appendLevelParameters( const QueryParameters& queryParameters,
                                   URLString url );

// For running CMAQSubset on local or remote files:

static void commandPrefix( const std::string& user,
                           const std::string& host,
                           std::string& command );

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: DOSPath - Get short DOS-encoded path name (without space) as required
         to invoke programs using popen() in a DOS shell.
INPUTS:  const char* const path  Path to convert to short/space-free format.
                        E.g., "C:\Documents and Settings\Todd Plessel\RSIG3D".
RETURNS: const char* DOS form. E.g., "C:\DOCUME~1\TODDPL~1\RSIG3D".
NOTES: Returned result is a pointer to static storage. Don't free.
       Not thread-safe.
       This routine calls custom DOS batch file DOSPATH.BAT in the current
       directory.
******************************************************************************/

const char* DatasetManager::DOSPath( const char* const path ) {
  static char result[ 256 ] = "";
  memset( result, 0, sizeof result );

#if defined( _WIN32 ) || defined( _WIN64 )

  char command[ 256 ] = "";
  memset( command, 0, sizeof command );
  const char doubleQuote = '"';
  const char* const backslash = "\\";
  snprintf( command, sizeof command / sizeof *command,
            "DOSPATH.BAT %c%s%s%c",
            doubleQuote, path, backslash, doubleQuote );
  DEBUG( fprintf( stderr, command, "DOSPath(): command = '%s'\n", command ); )
  FILE* pipe = popen( command, "r" );

  if ( pipe ) {
    fgets( result, sizeof result / sizeof *result, pipe );
    pclose( pipe ), pipe = 0;
    result[ sizeof result / sizeof *result - 1 ] = '\0';
    DEBUG( fprintf( stderr, "DOSPath(): initial result = '%s'\n", result ); )

    // Erase all characters from "\\" to the end of the string:

    const char* const doubleBackslash = "\\\\";
    char* erase = strstr( result, doubleBackslash );

    if ( erase ) {

      while ( *erase ) {
        *erase++ = '\0';
      }
    } else { // Erase all characters from last '\' to the end of the string:
      erase = strrchr( result, '\\' );

      if ( erase ) {

        while ( *erase ) {
          *erase++ = '\0';
        }
      }
    }
  }

#else
  strncpy( result, path, sizeof result / sizeof *result - 1 );
#endif

  DEBUG( fprintf( stderr, "DOSPath(): result = '%s'\n", result ); )
  return result;
}



/******************************************************************************
PURPOSE: checkAndSetServerHost - Establish url to webservice.
INPUTS:  const char* const path  Path to curlCommand used to query webservice.
NOTES:   It initializes the pathed commands for curl and gzip and queries the
         webservice to get the currently deployed version (YYYYMMDD) of RSIG3D.
         It tries to query the (faster) internal host webservice
         and if unsuccessful, it tries to query the (slower) external host.
         If unsuccessful, then calls to remoteVersion() will return 0.
         If successful then calls to remoteVersion() will return a value > 0
         and subsequent calls to retrieve methods will query the established
         host and webservice.
******************************************************************************/

void DatasetManager::checkAndSetServerHost( const char* const path ) {
  PRE03( *curlCommand, path, *path );

  server = EXTERNAL_SERVER; // Fallback default.
  remoteYYYYMMDD = 0;

  // Set pathedCurlCommand and pathedDecompressCommand:

  memset( pathWithoutSpaces, 0,
          sizeof pathWithoutSpaces / sizeof *pathWithoutSpaces );
  strncpy( pathWithoutSpaces, path,
           sizeof pathWithoutSpaces / sizeof *pathWithoutSpaces - 1 );

#ifdef _WIN32

  if ( strchr( path, ' ' ) ) { // The path contains spaces.
    const char* const dosEncodedPath = DatasetManager::DOSPath( path );
    memset( pathWithoutSpaces, 0,
            sizeof pathWithoutSpaces / sizeof *pathWithoutSpaces );
    strncpy( pathWithoutSpaces, dosEncodedPath,
             sizeof pathWithoutSpaces / sizeof *pathWithoutSpaces - 1 );
  }

  // Change UNIX '/'  to DOS '\':

  for ( char* c = pathWithoutSpaces; *c; ++c ) {

    if ( *c == '/' ) {
      *c = '\\';
    }
  }

  const char slash = '\\';
#else
  const char slash = '/';
#endif

  snprintf( pathedCurlCommand,
            sizeof pathedCurlCommand / sizeof *pathedCurlCommand,
            "%s%c%s", pathWithoutSpaces, slash, curlCommand );
  pathedCurlCommand[ sizeof  pathedCurlCommand /
                     sizeof *pathedCurlCommand - 1 ] = '\0';

  CHECK( *pathedCurlCommand );

#ifdef __APPLE__

  /*
   * HACK. 2023-11-21 testing with Langley's katherine.travis@nasa.gov,
   * new Macs (M1 processor, OS 13.6.0) kill RSIG-pathed gzip -
   * even when cp -p /usr/bin/gzip . is used to install a copy of the system
   * gzip!
   * But RSIG-pathed RSIG3D, curl, ffmeg all work fine.
   * (Also, to remove file 'attributes': xattr -cr .)
   * Instead use system path to gzip and hope that it exists and works.
   * Note, on these new Macs, the sysyem gzip contains executables for both
   * x86_64 and arm64.
   */

  snprintf( pathedDecompressCommand,
            sizeof pathedDecompressCommand / sizeof *pathedDecompressCommand,
            "/usr/bin/%s", decompressCommand );
#else
  snprintf( pathedDecompressCommand,
            sizeof pathedDecompressCommand / sizeof *pathedDecompressCommand,
            "%s%c%s", pathWithoutSpaces, slash, decompressCommand );
#endif

  pathedDecompressCommand[ sizeof  pathedDecompressCommand /
                           sizeof *pathedDecompressCommand - 1 ] = '\0';

  CHECK( *pathedDecompressCommand );

  // (Re)initialize HTTPConnection:

  HTTPConnection::initialize( pathedCurlCommand ); //x

  // Try to query the INTERNAL_SERVER for RSIG3D_VERSION:

  const int timeout = 30; // Maximum seconds to wait for webservice response.

  try {
    const char* const url = INTERNAL_SERVER "download/RSIG3D_VERSION";
    DEBUG( fprintf( stderr, "calling emvl::HTTPConnection( %s, %d )\n",
                    url, timeout ); )
    emvl::HTTPConnection connection( url, timeout ); //x

    if ( AND2( fscanf( connection.getStream(), "%d", &remoteYYYYMMDD ) == 1,
               remoteYYYYMMDD > 20100101 ) ) {
      server = INTERNAL_SERVER;
    } else {
      remoteYYYYMMDD = 0;
    }

    DEBUG( fprintf( stderr, "internal remoteYYYYMMDD = %d\n", remoteYYYYMMDD);)
  } catch ( ... ) {
    remoteYYYYMMDD = 0;
  }

  // If failed, try to query the EXTERNAL_SERVER for RSIG3D_VERSION:

  if ( remoteYYYYMMDD == 0 ) {

    try {
      const char* const url = EXTERNAL_SERVER "download/RSIG3D_VERSION";
      DEBUG( fprintf( stderr, "calling emvl::HTTPConnection( %s, %d )\n",
                      url, timeout ); )
      emvl::HTTPConnection connection( url, timeout ); //x

      if ( AND2( fscanf( connection.getStream(), "%d", &remoteYYYYMMDD ) == 1,
                 remoteYYYYMMDD > 20100101 ) ) {
        server = EXTERNAL_SERVER;
      } else {
        remoteYYYYMMDD = 0;
      }

      DEBUG(fprintf(stderr, "external remoteYYYYMMDD = %d\n", remoteYYYYMMDD);)
    } catch ( ... ) {
      remoteYYYYMMDD = 0;
    }
  }

  DEBUG( fprintf( stderr, "server = '%s', remoteYYYYMMDD = %d\n",
                  server, remoteYYYYMMDD ); )

  POST02( *server, OR2( remoteYYYYMMDD == 0, remoteYYYYMMDD > 20100101 ) );
}



/******************************************************************************
PURPOSE: getCurlPath - Path to curl program as set in checkAndSetServerHost().
RETURNS: std::string directory path (without spaces).
******************************************************************************/

std::string DatasetManager::getCurlPath() {

  if ( ! isDirectory( pathWithoutSpaces ) ) {
    pathWithoutSpaces[ 0 ] = '.';
    pathWithoutSpaces[ 1 ] = '\0';
  }

  const std::string result( pathWithoutSpaces );
  POST0( isDirectory( result ) );
  return result;
}



/******************************************************************************
PURPOSE: webservice - URL of webservice that serves data.
RETURNS: std::string  E.g., "https://ofmpub.epa.gov/rsig/rsigserver?"
******************************************************************************/

std::string DatasetManager::webservice() {
  PRE0( true );

  if ( ! server ) {
    server = EXTERNAL_SERVER; // Fallback default.
  }

  CHECK2( strlen( server ) > 9, strchr( server + 8, '.' ) );
  const std::string result( server );
  POST0( ! result.empty() );
  return result;
}



/******************************************************************************
PURPOSE: serverHost - Name of computer that serves data.
RETURNS: std::string  E.g., If server is
                      "http://ofmpub.epa.gov/rsig/rsigserver?"
                      then this routine returns "ofmpub".
******************************************************************************/

std::string DatasetManager::serverHost() {
  PRE0( true );

  if ( ! server ) {
    server = EXTERNAL_SERVER; // Fallback default.
  }

  CHECK2( strlen( server ) > 9, strchr( server + 8, '.' ) );
  const int isHttps = strstr( server, "https://" ) == server;
  const char* const start = server + 7 + isHttps; // Skip "http://".
  const char* const dot = strchr( start + 1, '.' );
  CHECK( dot );
  const size_t length = (const size_t) ( dot - start );
  CHECK( length >= 1 );
  const std::string result( start, length );
  POST0( ! result.empty() );
  return result;
}



/******************************************************************************
PURPOSE: remoteVersion - Version number of latest remote deployed RSIG3D.
RETURNS: int YYYYMMDD Date of last deployed remote version or 0 if
                      checkAndSetSeverHost() was not called successfully.
NOTES:   Set during checkAndSetServerHost().
******************************************************************************/

int DatasetManager::remoteVersion() {
  PRE0( true );
  const int result = remoteYYYYMMDD;
  POST0( OR2( result == 0, result > 20100101 ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidTEMPOKey - Is TEMPO key valid?
RETURNS: bool true if validated, else false.
NOTES:   Queries external server so checkAndSetServerHost() must have been
         called successfully already.
******************************************************************************/

bool DatasetManager::isValidTEMPOKey( const char* key ) {
  PRE02( key, *key );
  bool result = false;

  if ( remoteVersion() > 0 ) {
    URLString url = "";
    checkKeyURLString( false, TEMPO_KEY, key, url );
    HTTPConnection connection( url, 30 ); //x
    char line[ 256 ] = "";
    connection.read( sizeof line / sizeof *line, line ); //x
    result = *line == '1';
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidOAQPSKey - Is OAQPS key valid?
RETURNS: bool true if validated, else false.
NOTES:   Queries external server so checkAndSetServerHost() must have been
         called successfully already.
******************************************************************************/

bool DatasetManager::isValidOAQPSKey( const char* key ) {
  PRE02( key, *key );
  bool result = false;

  if ( remoteVersion() > 0 ) {
    URLString url = "";
    checkKeyURLString( false, OAQPS_KEY, key, url );
    HTTPConnection connection( url, 30 ); //x
    char line[ 256 ] = "";
    connection.read( sizeof line / sizeof *line, line ); //x
    result = *line == '1';
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidPurpleAirKey - Is PurpleAir key valid?
RETURNS: bool true if validated, else false.
NOTES:   Queries external server so checkAndSetServerHost() must have been
         called successfully already.
******************************************************************************/

bool DatasetManager::isValidPurpleAirKey( const char* key ) {
  PRE02( key, *key );
  bool result = false;

  if ( remoteVersion() > 0 ) {
    URLString url = "";
    checkKeyURLString( false, PURPLE_AIR_KEY, key, url );
    HTTPConnection connection( url, 30 ); //x
    char line[ 256 ] = "";
    connection.read( sizeof line / sizeof *line, line ); //x
    result = *line == '1';
  }

  return result;
}



/******************************************************************************
PURPOSE: ~DatasetManager - Destruct a DatasetManager.
******************************************************************************/

DatasetManager::~DatasetManager() {
  PRE( true );
  printURL = false;
  urlCallback = 0;
  removeAll();
}



/******************************************************************************
PURPOSE: DatasetManager - Default constructor.
INPUTS:  const bool printURL  Print URL string to stdout during retrievals?
         void (*urlCallback)( const char* const url ) Optional function called
                                                      with generated WCS URL
                                                      before retrieval.
******************************************************************************/

DatasetManager::DatasetManager( const bool printURL,
                                void (*urlCallback)( const char* const url ) )
  : printURL( printURL ), urlCallback( urlCallback ) {

  datasets_.reserve( 100 ); //x
  POST( count() == 0 );
}



/******************************************************************************
PURPOSE: retrieveFile - Retrieve a remote file from a web service and write it
         to a local file.
INPUTS:  const std::string& remoteFileName  E.g., download/RSIG_Win64.zip.
         const std::string& localFileName   E.g., /Users/plessel/RSIG.zip.
         const int timeoutSeconds  Seconds to wait for response before
                                   giving-up in failure.
                                   Default is 3600 (1 hour). 0 is 'forever'.
NOTES:   Queries external server so checkAndSetServerHost() must have been
         called successfully already.
         Throws an exception upon failure.
******************************************************************************/

void DatasetManager::retrieveFile( const std::string& remoteFileName,
                                   const std::string& localFileName,
                                   const int timeoutSeconds ) const {

  PRE4( remoteVersion() > 0,
        remoteFileName.length(),
        localFileName.length(),
        timeoutSeconds >= 0 );

  URLString url = "";
  toURLString( printURL, remoteFileName, url );

  if ( urlCallback ) {
    urlCallback( url );
  }

  HTTPConnection connection( url, timeoutSeconds ); //x
  FILE* const stream = connection.getStream();

  const bool ok = copyFileBytes( stream, localFileName.c_str() );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to stream all bytes to file." ); //x
  }

  POST( fileSize( localFileName.c_str() ) );
}



/******************************************************************************
PURPOSE: retrieveDatasetToFile - Retrieve a Dataset from a web service using
         QueryParameters and write it to a (generated-named) file in directory
         queryParameters.directory().
INPUTS:  const QueryParameters& queryParameters  Parameters for retrieving.
         const int timeoutSeconds  Seconds to wait for response before
                                   giving-up in failure.
                                   Default is 3600 (1 hour). 0 is 'forever'.
NOTES:   Queries external server so checkAndSetServerHost() must have been
         called successfully already.
         Throws an exception upon failure.
******************************************************************************/

void DatasetManager::retrieveDatasetToFile( const QueryParameters&
                                              queryParameters,
                                            const int timeoutSeconds ) const {

  PRE4( remoteVersion() > 0,
        queryParameters.invariant(),
        queryParameters.directory().length() >= 2,
        timeoutSeconds >= 0 );

  URLString url = "";
  toURLString( printURL, queryParameters, url );

  if ( urlCallback ) {
    urlCallback( url );
  }

  const bool useDecompression = strstr( url, "COMPRESS=1" );

  if ( useDecompression ) {
    HTTPConnection::setDecompressProgram( pathedDecompressCommand );
  }

  HTTPConnection connection( url, timeoutSeconds, useDecompression ); //x
  FILE* const stream = connection.getStream();

  const std::string pathedFileName( queryParameters.pathedFileName() ); //x
  const bool ok = copyFileBytes( stream, pathedFileName.c_str() );

  if ( ! ok ) {
    throw std::runtime_error( "Failed to stream all bytes to file." ); //x
  }

  POST( fileSize( pathedFileName.c_str() ) );
}



/******************************************************************************
PURPOSE: appendRetrievedDataset - Retrieve a Dataset from a web service using
         QueryParameters and append it to the collection.
INPUTS:  const QueryParameters& queryParameters  Parameters for creating.
         const int timeoutSeconds  Seconds to wait for response before
                                   giving-up in failure.
                                   Default is 3600 (1 hour). 0 is 'forever'.
NOTES:   Queries external server so checkAndSetServerHost() must have been
         called successfully already.
         Throws an exception upon failure.
******************************************************************************/

void DatasetManager::appendRetrievedDataset( const QueryParameters&
                                               queryParameters,
                                             const int timeoutSeconds ) {
  PRE4( remoteVersion() > 0,
        queryParameters.invariant(),
        queryParameters.format() == QueryParameters::XDR,
        timeoutSeconds >= 0 );
  REMEMBER_F( const size_t, count );
  Dataset* dataset = retrieveDataset( queryParameters, timeoutSeconds ); //x
  CHECK2( dataset, dataset->invariant() );
  datasets_.push_back( dataset ); //x
  dataset = 0;
  POST( count() == OLD( count ) + 1 );
}



/******************************************************************************
PURPOSE: appendDatasetFromFile - Read a Dataset from a file and append it.
INPUTS:  const std::string& fileName  File to read.
NOTES:   Throws an exception upon failure.
******************************************************************************/

void DatasetManager::appendDatasetFromFile( const std::string& fileName ) {
  PRE( fileName.length() );
  REMEMBER_F( const size_t, count );
  Dataset* dataset = readDataset( fileName ); //x
  CHECK2( dataset, dataset->invariant() );
  datasets_.push_back( dataset ); //x
  dataset = 0;
  POST( count() == OLD( count ) + 1 );
}



/******************************************************************************
PURPOSE: retrieveDataset - Retrieve a Dataset from a web service using
         QueryParameters.
INPUTS:  const QueryParameters& queryParameters  Parameters for web service.
         const int timeoutSeconds  Seconds to wait for response before
                                   giving-up in failure.
                                   Default is 3600 (1 hour). 0 is 'forever'.
RETURNS: Dataset* Dataset.
NOTES:   Queries external server so checkAndSetServerHost() must have been
         called successfully already.
         Caller must deallocate returned result when no longer needed
         or else pass it to append() to pass ownership (and deallocation).
         Abstract Factory Pattern.
         Throws an exception upon failure.
******************************************************************************/

Dataset* DatasetManager::retrieveDataset( const QueryParameters&
                                            queryParameters,
                                          const int timeoutSeconds ) const {

  PRE3( remoteVersion() > 0, queryParameters.invariant(), timeoutSeconds >= 0 );

  Dataset* result = 0;
  URLString url = "";
  toURLString( printURL, queryParameters, url );

  if ( urlCallback ) {
    urlCallback( url );
  }

  HTTPConnection connection( url, timeoutSeconds ); //x
  FILE* const stream = connection.getStream();
  char line[ 256 ] = "";
  memset( line, 0, sizeof line );

  if ( ! stream ) {
    throw std::runtime_error( "Failed to get readable stream." ); //x
  } else if ( ! fgets( line, sizeof line / sizeof *line, stream ) ) {
    throw std::runtime_error( "Failed to read first line of stream." ); //x
  } else {
    CreatorFromStream creator = lookupCreatorFromStream( line );

    if ( ! creator ) {
      throw std::runtime_error( "Unsupported type of data." ); //x
    }

    // Unread the first line:

    bool ok = true;

    for ( size_t index = strlen( line ); AND2( index > 0, ok ); --index ) {
      const int c = line[ index - 1 ];
      ok = ungetc( c, stream ) == c;
    }

    if ( ok ) { // Read from stream and return Dataset:
      result = creator( stream ); //x
    }
  }

  POST2( result, result->invariant() );
  return result;
}



/******************************************************************************
PURPOSE: readDataset - Read a Dataset from a file.
INPUTS:  const std::string& fileName  Name of file to read.
RETURNS: Dataset* Dataset.
NOTES:   Throws an exception upon failure.
         Caller must deallocate returned result when no longer needed
         or else pass it to append() to pass ownership (and deallocation).
         Abstract Factory Pattern.
******************************************************************************/

Dataset* DatasetManager::readDataset( const std::string& fileName ) const {

  PRE( fileName.length() );
  Dataset* result = 0;
  const char* const name = fileName.c_str();

  if ( ! strstr( name, ".xdr" ) ) {
    throw std::runtime_error( "Unsupported file type: " + fileName ); //x
  } else { // Determine dataset type from 1st line of file:
    ResourceHandler<FILE*> fileCloser( fopen( name, "rb" ), fclose );
    FILE* const file = fileCloser.get();
    char line[ 256 ] = "";
    memset( line, 0, sizeof line );

    if ( ! file ) {
      throw std::runtime_error("Failed to open readable file: " + fileName);//x
    } else if ( ! fgets( line, sizeof line / sizeof *line, file ) ) {
      throw std::runtime_error( "Failed to read first line of file: "
                                + fileName ); //x
    } else {
      CreatorFromFile creator = lookupCreatorFromFile( line );

      if ( ! creator ) {
        const std::runtime_error
          exception( "Invalid/Unsupported file content: " + fileName );
        throw exception; //x
      } else { // Read file and return Dataset:
        result = creator( fileName ); //x
      }
    }
  }

  POST2( result, result->invariant() );
  return result;
}



/******************************************************************************
PURPOSE: append - Append a Dataset to the collection, passing ownership and
         eventual deallocation responsibility to this DatasetManager.
INPUTS:  Dataset* dataset  Dataset to append.
NOTES:   Throws an exception upon failure.
         Caller should zero argument after the call to avoid a dangling
         reference.
******************************************************************************/

void DatasetManager::append( Dataset* dataset ) {
  PRE2( dataset, dataset->invariant() );
  REMEMBER_F( const size_t, count );
  datasets_.push_back( dataset ); //x
  POST( count() == OLD( count ) + 1 );
}



/******************************************************************************
PURPOSE: remove - Remove a Dataset from the collection and deallocate it.
INPUTS:  const size_t index  Index of Dataset to remove.
******************************************************************************/

void DatasetManager::remove( const size_t index ) {
  PRE( index < count() );
  REMEMBER_F( const size_t, count );
  delete datasets_[ index ];
  datasets_[ index ] = 0;
  datasets_.erase( datasets_.begin() + index );
  POST( count() == OLD( count ) - 1 );
}



/******************************************************************************
PURPOSE: removeAll - Remove and deallocate all Datasets in the collection.
******************************************************************************/

void DatasetManager::removeAll() {
  PRE( true );

  const size_t datasetCount = datasets_.size();

  for ( size_t index = 0; index < datasetCount; ++index ) {
    delete datasets_[ index ];
    datasets_[ index ] = 0;
  }

  datasets_.clear();
  POST( count() == 0 );
}



/******************************************************************************
PURPOSE: writeAll - Write all Datasets to files.
INPUTS:  const std::string& directory  Directory to write files to.
         const int format              File format to write.
NOTES:   Throws an exception upon failure.
******************************************************************************/

void DatasetManager::writeAll( const std::string& directory,
                               const int format ) const {

  PRE3( directory.length() >= 2,
        IN_RANGE( format, 0, QueryParameters::FILE_FORMATS - 1 ),
        format != QueryParameters::NETCDF_IOAPI );

  const size_t count = datasets_.size();

  if ( count ) {

    for ( size_t index = 0; index < count; ++index ) {
      datasets_[ index ]->write( directory, format ); //x
    }
  }

  POST( true );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true if valid, else false and the code is defective.
******************************************************************************/

bool DatasetManager::invariant() const {
  bool result = true;
  const size_t count = datasets_.size();

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    const Dataset* const dataset = datasets_[ index ];
    result = AND2( dataset != 0, dataset->invariant() );
  }

  return result;
}



/******************************************************************************
PURPOSE: count - Number of datasets.
RETURNS: size_t Number of datasets.
******************************************************************************/

size_t DatasetManager::count() const {
  PRE( true );
  const size_t result = datasets_.size();
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: operator[] - Access a dataset.
INPUTS:  const size_t index  Index of dataset to access.
RETURNS: Dataset& Dataset at index.
******************************************************************************/

Dataset& DatasetManager::operator[]( const size_t index ) const {
  PRE( index < count() );
  Dataset* const dataset = datasets_[ index ];
  Dataset& result = *dataset;
  POST( result.invariant() );
  return result;
}



/******************************************************************************
PURPOSE: setLocalSubsetterDirectory - Local directory containing subsetter
         program.
INPUTS: std::string directory  Name of local directory containing subsetter.
******************************************************************************/

void DatasetManager::setLocalSubsetterDirectory( const std::string& directory) {
  PRE0( isDirectory( directory ) );
  localSubsetterDirectory = directory;
}



/******************************************************************************
PURPOSE: setRemoteSubsetterDirectory - Remote host path or subdirectory (from
         HOME) containing subsetter program.
INPUTS: std::string directory  Name of subdirectory containing subsetter.
******************************************************************************/

void DatasetManager::setRemoteSubsetterDirectory(const std::string& directory) {
  PRE0( ! directory.empty() );
  remoteSubsetterDirectory = directory;
}



/******************************************************************************
PURPOSE: getWorkingDirectory - Current directory name on local or remote host.
INPUTS:  const std::string& user  (empty if local)
         const std::string& host  (empty if local)
RETURNS: const std::string  directory name.
NOTES: Runs subsetter process and reads result from it.
******************************************************************************/

std::string DatasetManager::getWorkingDirectory( const std::string& user,
                                                 const std::string& host ) {
  std::string command; //x
  command.reserve( 256 ); //x
  commandPrefix( user, host, command );
  command += " -pwd";
  std::string result;
  result.reserve( 256 ); //x
  readCommandResult( command, result );
  return result;
}



/******************************************************************************
PURPOSE: getPlatform - Patform name on local or remote host.
INPUTS:  const std::string& user  (empty if local)
         const std::string& host  (empty if local)
RETURNS: const std::string  platform name or empty if failed.
         Darwin.x86_64
         Darwin.arm64
         Linux.x86_64
         Win64
NOTES:   If remote host, it runs ssh and uname and/or echo process and
         parses result from it.
******************************************************************************/

std::string DatasetManager::getPlatform( const std::string& user,
                                         const std::string& host ) {
  std::string result;
  result.reserve( 64 ); //x

  if ( host.empty() ) { // Get localhost platform statically from compiler:
    result = emvl::platform;
  } else { // Get remote host platform using ssh uname or ssh echo %cd%:
    std::string command; //x
    command.reserve( 256 ); //x

    if ( *pathWithoutSpaces ) {
      command += pathWithoutSpaces;
      command += slash;
    }

    command += ssh;
    command += ' ';

    if ( ! user.empty() ) {
      command += user;
      command += '@';
    }

    command += host;

    // First try uname in hopes that the remote host is UNIX:

    command += " uname -sm";
    std::string unameResult;
    unameResult.reserve( 256 ); //x
    bool ok = readCommandResult( command, unameResult );

    if ( ok ) {
      const size_t indexOfSpace = unameResult.find( ' ' );

      if ( indexOfSpace != std::string::npos ) {
        unameResult[ indexOfSpace ] = '.';
        result = unameResult;
      }
    } else { // ssh uname failed, try Windows: ssh echo %cd%
      const size_t indexOfUname = command.find( "uname" );
      CHECK( indexOfUname < command.length() );
      command.erase( indexOfUname );
      command += "echo %cd%";
      unameResult.clear();
      ok = readCommandResult( command, unameResult );

      if ( ok ) {
        const size_t indexOfDriveBackslash = unameResult.find( ":\\" );

        if ( indexOfDriveBackslash != std::string::npos ) {
          result = "Win64";
        }
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: installRemoteSubsetter - Copy subsetter to remote host.
INPUTS:  const std::string& user  (or empty)
         const std::string& host
RETURNS: const bool true if successful, else false and a failure message
         is printed to stderr.
NOTES:   Copies platform-specific subsetter executable to .rsig
         subdirectory under user's home directory on host.
******************************************************************************/

bool DatasetManager::installRemoteSubsetter( const std::string& user,
                                             const std::string& host ) {
  PRE02( ! host.empty(), ! localSubsetterDirectory.empty() );
  bool result = true;

  // Memoize: check host name to avoid repeated installations:

  if ( std::find( installedHosts.begin(), installedHosts.end(), host )
       == installedHosts.end() ) { // The host name was not found.

    if ( ! remoteSubsetterDirectory.empty() ) {
      result = ensureDirectoryExists( user, host, remoteSubsetterDirectory );
    }

    if ( result ) {
      std::string path( localSubsetterDirectory ); // On localhost.
      const size_t indexOfSlash = path.rfind( emvl::slash );
      result = indexOfSlash != std::string::npos;

      if ( result ) {
        const std::string remotePlatform = getPlatform( user, host );
        result = ! remotePlatform.empty();

        if ( result ) {
          const bool remotePlatformIsWindows =
            ! strcmp( remotePlatform.c_str(), "Win64" );
          path.erase( indexOfSlash );
          path += emvl::slash;
          path += remotePlatform;
          path += emvl::slash;
          path += subsetter;

          if ( remotePlatformIsWindows ) {
            path.append( ".exe" );
          }

          const int localSubsetterVersion =
            Timestamp::fileDateUTC( path.c_str() );
          printf( "Current subsetter version = %d\n", localSubsetterVersion );

          // Assume subsetter exists on the remote host and get its version.
          // This will be 0 if it did not exist.

          std::string command; //x
          command.reserve( 256 ); //x

          commandPrefix( user, host, command ); // ssh user@host subsetter.
          command += " -version";
          std::string version;
          version.reserve( 32 ); //x
          readCommandResult( command, version );
          const int remoteSubsetterVersion = atoi( version.c_str() );
          printf( "Remote subsetter version = %d\n", remoteSubsetterVersion );

          if ( remoteSubsetterVersion < localSubsetterVersion ) {

            // Proceed with installation (scp, chmod):

            command.clear();

            if ( *pathWithoutSpaces ) {
              command += pathWithoutSpaces;
              command += slash;
            }

            command += scp;
            command += ' ';
            command += path;
            command += ' ';

            if ( ! user.empty() ) {
              command += user;
              command += '@';
            }

            command += host;
            command += ':';

            if ( ! remoteSubsetterDirectory.empty() ) {
              command += remoteSubsetterDirectory;

              if ( remotePlatformIsWindows ) {
                command += '\\';
              } else {
                command += '/';
              }
            }

            int status = system( command.c_str() );
            result = status == 0;

            if ( result ) {

              /*
               * On Windows, scp -p does not preserve x (executable permission)!
               * So if the local platform is Windows and the remote platform is
               * non-Windows, ssh chmod +x subsetter to restore executable bit.
               * If both local and remote platforms are Windows then BUG
               * UNIMPLEMENTED solution! Perhaps with the version-check logic
               * the user could manually install/chmod the subsetter and this
               * routine will then succeed.
               */

              const bool localPlatformIsWindows =
                ! strcmp( emvl::platform, "Win64" );

              if ( AND2( localPlatformIsWindows, ! remotePlatformIsWindows ) ) {
                command.clear();

                if ( *pathWithoutSpaces ) {
                  command += pathWithoutSpaces;
                  command += slash;
                }

                command += ssh;
                command += ' ';

                if ( ! user.empty() ) {
                  command += user;
                  command += '@';
                }

                command += host;
                command += " chmod +x ";

                if ( ! remoteSubsetterDirectory.empty() ) {
                  command += remoteSubsetterDirectory;
                  command += '/';
                }

                command += subsetter;
                status = system( command.c_str() );
                result = status == 0;
              }
            }

            if ( status ) {
              fprintf( stderr, "\nCommand failed: '%s'\n", command.c_str() );
            }
          }
        }
      }
    }

    if ( result ) { // If successful, memoize host name for next time.
      installedHosts.push_back( host ); //x Memoize for next time.
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: ensureDirectoryExists - Check/create directory on local/remote
         host.
INPUTS:  const std::string& user  (or empty)
         const std::string& host  (or empty if local)
         const std::string& name  Name of directory to check/create.
RETURNS: const bool true if successful, else false and a failure message
         is printed to stderr.
******************************************************************************/

bool DatasetManager::ensureDirectoryExists( const std::string& user,
                                            const std::string& host,
                                            const std::string& name ) {
  PRE0( ! name.empty() );
  bool result = false;

  if ( host.empty() ) {
    result = isDirectory( name );

    if ( ! result ) {
#ifdef _WIN32
      result = mkdir( name.c_str() ) == 0;
#else
      result = mkdir( name.c_str(), 0755 ) == 0; // rwxr-xr-x.
#endif
    }
  } else {
    std::string command; //x
    command.reserve( 256 ); //x

    if ( *pathWithoutSpaces ) {
      command += pathWithoutSpaces;
      command += slash;
    }

    command += ssh;
    command += ' ';

    if ( ! user.empty() ) {
      command += user;
      command += '@';
    }

    command += host;

    const std::string remotePlatform = getPlatform( user, host );

    if ( remotePlatform == "Win64" ) {
      command += " if exist " + name + "\\ ( echo 1 ) else ( echo 0 )";
      std::string response; //x
      result = readCommandResult( command, response );

      if ( result ) {
        result = response[ 0 ] == '1';

        if ( ! result ) { // Directory did not exist so create it:
          const size_t indexOfif = command.find( " if " );

          if ( indexOfif != std::string::npos ) {
            command.erase( indexOfif );
            command += " mkdir " + name;
            const int status = system( command.c_str() );
            result = status == 0; // No error implies directory created.
          }
        }
      }
    } else if ( ! remotePlatform.empty() ) { // Try ls name to see if dir exists
      command += " ls " + name; // + " >& /dev/null";
      int status = system( command.c_str() );
      result = status == 0; // No error implies directory exists.

      if ( ! result ) { // Failed so try to create it:
        const size_t indexOfls = command.find( " ls ");

        if ( indexOfls != std::string::npos ) {
          command.erase( indexOfls );
          command += " mkdir " + name; // + " >& /dev/null";
          status = system( command.c_str() );
          result = status == 0; // No error implies directory created.
        }
      }
    }
  }

  if ( ! result ) {
    fprintf( stderr, "\nFailed to create directory '%s'\n", name.c_str() );
  }

  return result;
}



/******************************************************************************
PURPOSE: getDirectoryListing - Get sorted list of names of subdirectories
         and CMAQ files in directory.
RETURNS:  const std::string  sorted newline-delimited names or empty if failed.
NOTES: Runs subsetter process and reads result from it.
******************************************************************************/

std::string DatasetManager::getDirectoryListing( const std::string& user,
                                                 const std::string& host,
                                                 const std::string& directory) {
  PRE0( ! directory.empty() );

  std::string command; //x
  command.reserve( 256 ); //x
  commandPrefix( user, host, command );
  command += " -ls ";
  command += directory;

  std::string result; //x
  result.reserve( 4096 ); //x
  readCommandResult( command, result );

  return result;
}



/******************************************************************************
PURPOSE: getVariableListing - Get sorted list of names of variables in file.
INPUTS:  const std::string& user  (empty if local)
         const std::string& host  (empty if local)
         const std::string& file  Pathed file name to read variables from.
RETURNS: const std::string  sorted newline-delimited names or empty if failed.
NOTES: Runs remote subsetter process and reads result from it.
******************************************************************************/

std::string DatasetManager::getVariableListing( const std::string& user,
                                                const std::string& host,
                                                const std::string& file ) {

  PRE0( ! file.empty() );

  std::string command; //x
  command.reserve( 256 ); //x
  commandPrefix( user, host, command );
  command += " -list -files ";
  command += file;

  std::string result; //x
  result.reserve( 4096 ); //x
  readCommandResult( command, result );

  return result;
}



/******************************************************************************
PURPOSE: subsetCMAQFile - Get subset of a local or remote CMAQ file to a local
         xdr file.
INPUTS:  const std::string& user
         const std::string& host  (empty if local)
         const std::string& pathedDataFileName
         const std::string& dataVariable
         const std::string& pathedHTFileName
         const std::string& pathedZFFileName
         const std::string& pathedWWINDFileName
         const std::string& outputDirectoryName
         const Bounds& bounds
         const Timestamp& firstTimestamp
         const Timestamp& lastTimestamp
         const int layer      -1 = integrate layers, 0 = all layers, > 0 single
         const int aggregate  If 1 aggregate daily mean.
RETURNS: const std::string  Name of local xdr file.
NOTES:   Runs subsetter process.
         Creates xdr file outputDirectoryName/
         cmaq_dataVariable_yyyy-mm-dd_to_yyyy-mm-dd.xdr.
******************************************************************************/

std::string DatasetManager::subsetCMAQFile(
  const std::string& user,
  const std::string& host,
  const std::string& pathedDataFileName,
  const std::string& dataVariable,
  const std::string& pathedHTFileName,
  const std::string& pathedZFFileName,
  const std::string& pathedWWINDFileName,
  const std::string& outputDirectoryName,
  const Bounds& bounds,
  const Timestamp& firstTimestamp,
  const Timestamp& lastTimestamp,
  const int layer,
  const int aggregate ) {

  PRE09( ! pathedDataFileName.empty(),
         ! dataVariable.empty(),
         ! outputDirectoryName.empty(),
         bounds.invariant(),
         firstTimestamp.invariant(),
         lastTimestamp.invariant(),
         firstTimestamp <= lastTimestamp,
         layer > -1,
         IS_BOOL( aggregate ) );

  std::string command; //x
  command.reserve( 4096 ); //x
  commandPrefix( user, host, command );

  command += " -files ";
  command += pathedDataFileName;

  if ( ! pathedHTFileName.empty() ) {
    command += " -ht ";
    command += pathedHTFileName;
  }

  if ( ! pathedZFFileName.empty() ) {
    command += " -zf ";
    command += pathedZFFileName;
  }

  bool windVariable = false;

  if ( AND2( IN3( dataVariable, "UWIND", "VWIND" ),
             ! pathedWWINDFileName.empty() ) ) {
    command += " -wwind ";
    command += pathedWWINDFileName;
    command += " -variable UWIND VWIND"; // Change to get 3D wind.
    windVariable = true;
  } else {
    command += " -variable ";
    command += dataVariable;
  }

  char buffer[ 256 ] = "";
  memset( buffer, 0, sizeof buffer );
  const int hh1 = firstTimestamp.hhmmss() / 10000;
  const int yyyymmddhh1 = firstTimestamp.yyyymmdd() * 100 + hh1;
  const int hh2 = lastTimestamp.hhmmss() / 10000;
  const int yyyymmddhh2 = lastTimestamp.yyyymmdd() * 100 + hh2;
  snprintf( buffer, sizeof buffer / sizeof *buffer, " -time %d %d",
            yyyymmddhh1, yyyymmddhh2 );
  command += buffer;
  snprintf( buffer, sizeof buffer / sizeof *buffer, " -bounds %f %f %f %f",
            bounds.west(), bounds.south(), bounds.east(), bounds.north() );
  command += buffer;

  if ( layer == -1 ) {

    if ( ! pathedZFFileName.empty() ) {
      command += " -integrate_layers";
    }
  } else if ( layer != 0 ) {
    snprintf( buffer, sizeof buffer / sizeof *buffer, " -layer %d", layer );
    command += buffer;
  }

  if ( aggregate ) {
    command += " -aggregate daily_mean";
  }

  command += " -elevation -format xdr ";


  // Set result to the pathed output file name (cmaq_variable_timestamp.xdr):

  std::string result; //x
  result.reserve( 256 ); //x

  result += outputDirectoryName;
  result += slash;
  result += "cmaq_";

  if ( windVariable ) {
    result += "WIND";
  } else {
    result += dataVariable;
  }

  if ( firstTimestamp.sameDay( lastTimestamp ) ) {
    snprintf( buffer, sizeof buffer / sizeof *buffer, "_%d-%02d-%02d.xdr",
              yyyymmddhh1 / 1000000,
              yyyymmddhh1 / 10000 % 100,
              yyyymmddhh1 / 100 % 100 );
  } else {
    snprintf( buffer, sizeof buffer / sizeof *buffer,
              "_%d-%02d-%02d_to_%d-%02d-%02d.xdr",
              yyyymmddhh1 / 1000000,
              yyyymmddhh1 / 10000 % 100,
              yyyymmddhh1 / 100 % 100,
              yyyymmddhh2 / 1000000,
              yyyymmddhh2 / 10000 % 100,
              yyyymmddhh2 / 100 % 100 );
  }

  result += buffer;

  if ( host.empty() ) {
    command += " -output ";
    command += result;
  }

  puts( command.c_str() );

  if ( host.empty() ) { // Run subsetter on local files:
    const int status = system( command.c_str() );

    if ( status != 0 ) {
      result.clear(); // Clear result file name since subsetter failed.
    }
  } else { // Run subsetter on remote host:
    FILE* output = fopen( result.c_str(), "wb" ); // Write to local file.

    if ( output ) {
      const bool ok = readCommandResult( command, output );

      if ( ! ok ) {
        result.clear(); // Clear result file name since subsetter failed.
      }

      fclose( output ), output = 0;
    }
  }

  return result;
}



//============================= PRIVATE FUNCTIONS =============================



/******************************************************************************
PURPOSE: commandPrefix - Start command with a call to local or remote
         subsetter.
INPUTS:  const std::string& user  User login name if needed for remote host.
         const std::string& host  Host name - empty if local.
OUTPUTS: std::string& command     Initialized first part of command.
NOTES:   Uses subsetter command possibly prefixed with ssh command.
******************************************************************************/

static void commandPrefix( const std::string& user,
                           const std::string& host,
                           std::string& command ) {

  command.clear();

  if ( ! host.empty() ) { // Remote access via ssh program.

    if ( *pathWithoutSpaces ) {
      command += pathWithoutSpaces;
      command += slash;
    }

    command += ssh;
    command += ' ';

    if ( ! user.empty() ) {
      command += user;
      command += '@';
    }

    command += host;
    command += ' ';

    if ( ! remoteSubsetterDirectory.empty() ) {
      command += remoteSubsetterDirectory + '/';
      // Using '/' here only works if the remote host is non-Windows!
    }
  } else {

    if ( ! localSubsetterDirectory.empty() ) {
      command += localSubsetterDirectory + slash;
    }
  }

  command += subsetter;
}



/******************************************************************************
PURPOSE: checkKeyURLString - Construct URL to validate key.
INPUTS:  const bool printURL     Print constructed URL string to stdout?
         const int keySource     PURPLE_AIR_KEY, TEMPO_KEY, OAQPS_KEY.
         const std::string& key  Key to validate.
OUTPUTS: URLString url           Validation query string to webservice.
******************************************************************************/

static void checkKeyURLString( const bool printURL,
                               const int keySource,
                               const std::string& key,
                               URLString url ) {

  PRE05( IN_RANGE( keySource, 0, KEYS - 1 ),
         key.length(), url, server, *server );

  memset( url, 0, sizeof (URLString) );

  const char* const urlFormat =
    ! strcmp( server, INTERNAL_SERVER ) ?
      INTERNAL_SERVER "CHECK_%sKEY=%s"
    : EXTERNAL_SERVER "CHECK_%sKEY=%s";

  snprintf( url,
            sizeof (URLString) / sizeof (char),
            urlFormat,
            keySource == TEMPO_KEY ? "TEMPO_"
            : keySource == OAQPS_KEY ? "OAQPS_"
            : "", key.c_str() );

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )

  if ( printURL ) {
    puts( url );
  }

  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: toURLString - Convert remotFileName to an http URL (OGC-WCS query).
INPUTS:  const bool printURL  Print constructed URL string to stdout?
         const std::string& remotFileName  E.g., download/RSIG_Darwin.x86-64.zip
OUTPUTS: URLString url                     Query string to webservice.
******************************************************************************/

static void toURLString( const bool printURL,
                         const std::string& remoteFileName,
                         URLString url ) {

  PRE04( remoteFileName.length(), url, server, *server );

  memset( url, 0, sizeof (URLString) );

  const char* const urlFormat =
    ! strcmp( server, INTERNAL_SERVER ) ? INTERNAL_SERVER "%s"
    : EXTERNAL_SERVER "%s";

  snprintf( url, sizeof (URLString) / sizeof (char), urlFormat,
            remoteFileName.c_str() );

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )

  if ( printURL ) {
    puts( url );
  }

  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: toURLString - Convert QueryParameters to an http URL (OGC-WCS query).
INPUTS:  const bool printURL  Print constructed URL string to stdout?
         const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           Query string to webservice.
******************************************************************************/

static void toURLString( const bool printURL,
                         const QueryParameters& queryParameters,
                         URLString url ) {

  PRE04( queryParameters.invariant(), url, server, *server );

  memset( url, 0, sizeof (URLString) );

  CHECKING( const std::string& directory = queryParameters.directory(); )
  const int format = queryParameters.format();
  CHECK2( IMPLIES( directory.length() == 0, format == QueryParameters::XDR ),
          IN_RANGE( format, 0, QueryParameters::FILE_FORMATS ) );
  const char* const formatName = QueryParameters::fileFormatNames[ format ];

  // Convert timestamps from YYYY-MM-DDTHH:MM:SS-0000 to YYYY-MM-DDTHH:MM:SSZ:

  const Timestamp startingTimestamp = queryParameters.startingTimestamp();
  const Timestamp endingTimestamp   = queryParameters.endingTimestamp();
  Timestamp::UTCString startTime = "";
  Timestamp::UTCString endTime = "";
  startingTimestamp.toUTCString( startTime );
  endingTimestamp.toUTCString( endTime );
  startTime[ 19 ] = 'Z';
  startTime[ 20 ] = '\0';
  endTime[ 19 ] = 'Z';
  endTime[ 20 ] = '\0';

 // Get COVERAGE:

  const std::string& variables = queryParameters.variables();
  const char* const coverage0 = variables.c_str();
  const char* const regridded = strstr( coverage0, "regridded." );
  const char* const coverage =
    // Handle MOZAIC wind vector pseudo-variable:
    ! strcmp( coverage0, "mozaic.wind" ) ? "mozaic.wind_u,mozaic.wind_v"
    : ! strcmp( coverage0, "mozaic.profile.wind" ) ?
      "mozaic.profile.wind_u,mozaic.profile.wind_v"
    // Handle regridded:
    : regridded ? regridded // Use regriddedserver.
    // Default case:
    : coverage0;

  // Get BBOX:

  const Bounds& bounds = queryParameters.bounds();
  char boundsZ[ 128 ] = "";
  memset( boundsZ, 0, sizeof boundsZ );

  double boundsZMinimum = queryParameters.boundsZMinimum();
  double boundsZMaximum = queryParameters.boundsZMaximum();

  if ( boundsZMinimum > MISSING ) {
    snprintf( boundsZ, sizeof boundsZ / sizeof *boundsZ, ",%lg,%lg",
              boundsZMinimum, boundsZMaximum );
  }

  const bool isGridded =
    OR5( regridded,
         strstr( coverage, "omibehrioapi." ) == coverage,
         strstr( coverage, "cmaq." ) == coverage,
         strstr( coverage, "tempo.l3." ) == coverage,
         strstr( coverage, "tempo2.l3." ) == coverage );
  const bool regridding =
    AND3( queryParameters.regridMethod() != QueryParameters::REGRID_NONE,
          ! isGridded,
          strstr( coverage, "hms.smoke" ) != coverage );
  const int aggregateMethod = queryParameters.aggregateMethod();
  const bool aggregating =
    AND2( aggregateMethod != QueryParameters::AGGREGATE_NONE,
          OR4( strstr( coverage, "pandora." ) == coverage,
               strstr( coverage, "omibehrioapi." ) == coverage,
               strstr( coverage, "cmaq." ) == coverage,
               AND3( strstr( coverage, "aqs." ) == coverage,
                     ! strstr( coverage, "_daily_" ),
                     ! strstr( coverage, "_average" ) ) ) );
  const bool regridAggregating =
    AND2( regridding,
          queryParameters.regridAggregateMethod() !=
            QueryParameters::REGRID_AGGREGATE_NONE );
  const char* const extraOptions =
    strstr( coverage, "hrrr." ) == coverage ? "&CMAQ=1" :
    strstr( coverage, "cmaq." ) == coverage ?
      ( queryParameters.integrateCMAQLayers() ? "&NOLONLATS=1&INTEGRATE=1" :
      "&NOLONLATS=1" ) :
    OR2( strstr( coverage, "omibehrioapi." ) == coverage,
         strstr( coverage, "regridded."    ) == coverage ) ? "&NOLONLATS=1" :
    AND4( ! isGridded,
          ! regridded,
          queryParameters.regridMethod() != QueryParameters::REGRID_NEAREST,
          OR8( strstr( coverage, "gasp." ),
               strstr( coverage, "goes." ),
               strstr( coverage, "modis" ),
               strstr( coverage, "viirs" ),
               strstr( coverage, "tempo" ),
               strstr( coverage, "tropomi" ),
               strstr( coverage, "behr." ),
               strstr( coverage, "omi." ) ) ) ? "&CORNERS=1"
    : "";
  const char* const request =
    queryParameters.getMetadata() ? "GetMetadata" : "GetCoverage";
  const bool requestIsGetCoverage = ! strcmp( request, "GetCoverage" );
  const bool compress =
    AND3( queryParameters.compress(),
          requestIsGetCoverage,
          format != QueryParameters::ORIGINAL );

  /* Final format for OGC-WCS query: */

  const char* const urlFormat =
    ! strcmp( server, INTERNAL_SERVER ) ?
      INTERNAL_SERVER
      "SERVICE=wcs&VERSION=1.0.0&REQUEST=%s&FORMAT=%s"
      "&TIME=%20s/%20s"
      "&BBOX=%lf,%lf,%lf,%lf%s"
      "&COVERAGE=%s"
      "&COMPRESS=%d"
      "%s"
    : EXTERNAL_SERVER
      "SERVICE=wcs&VERSION=1.0.0&REQUEST=%s&FORMAT=%s"
      "&TIME=%20s/%20s"
      "&BBOX=%lf,%lf,%lf,%lf%s"
      "&COVERAGE=%s"
      "&COMPRESS=%d"
      "%s";

  snprintf( url, sizeof (URLString) / sizeof (char), urlFormat,
            request,
            formatName,
            startTime, endTime,
            bounds.west(), bounds.south(),
            bounds.east(), bounds.north(),
            boundsZ, coverage, compress, extraOptions );

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );

  if ( ! regridded ) { // Only apply these options for non-regridded data:

    if ( strstr( coverage, "gasp." ) ) {
      appendGASPParameters( queryParameters, url );
    } else if ( strstr( coverage, "calipso." ) ) {
      appendCALIPSOParameters( queryParameters, url );
    } else if ( strstr( coverage, "tropomi." ) ) {
      appendTROPOMIParameters( queryParameters, url );
    } else if ( strstr( coverage, "omi." ) ) {
      appendOMIParameters( queryParameters, url );
    } else if ( strstr( coverage, "viirsnoaa." ) ) {
      appendVIIRSNOAAParameters( queryParameters, url );
    } else if ( strstr( coverage, "pandora." ) ) {
      appendPandoraParameters( queryParameters, url );
    } else if ( strstr( coverage, "purpleair" ) ) {
      appendPurpleAirParameters( queryParameters, coverage, url );
    } else if ( strstr( coverage, "tempo" ) == coverage ) {
      appendTEMPOParameters( queryParameters, url );
    } else if ( strstr( coverage, "cmaq.oaqps." ) == coverage ) {
      appendOAQPSParameters( queryParameters, url );
    }

    if ( aggregating ) {
      const int method = queryParameters.aggregateMethod();
      const char* const methodName =
        QueryParameters::aggregateMethodNames[ method ];
      char option[ 256 ] = "";
      memset( option, 0, sizeof option );
      snprintf( option, sizeof option / sizeof *option,
                "&AGGREGATE=%s", methodName );

      const size_t urlLength = strlen( url );
      const size_t urlLengthAvailable =
        sizeof (URLString) / sizeof (char) - 1 - urlLength;
      const size_t optionLength = strlen( option );

      if ( optionLength <= urlLengthAvailable ) {
        strncat( url, option, optionLength );
      }

      CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
      DEBUG( fprintf( stderr, "url = %s\n", url ); )
    }

    if ( regridding ) {
      appendRegriddingParameters( queryParameters, url );
      const bool hasElevation =
        OR9( strstr( coverage, "calipso." ),
             strstr( coverage, "acam." ),
             strstr( coverage, "hsrl." ),
             strstr( coverage, "mozaic." ),
             strstr( coverage, "tad." ),
             strstr( coverage, "ceilometer." ),
             strstr( coverage, "pandora." ),
             strstr( coverage, "neubrew." ),
             strstr( coverage, "purpleair" ) );

      if ( hasElevation ) {
        appendLevelParameters( queryParameters, url );
      }
    }

    if ( regridAggregating ) {
      const int method = queryParameters.regridAggregateMethod();
      const char* const methodName =
        QueryParameters::regridAggregateMethodNames[ method ];
      char option[ 256 ] = "";
      memset( option, 0, sizeof option );
      snprintf( option, sizeof option / sizeof *option,
                "&REGRID_AGGREGATE=%s", methodName );

      const size_t urlLength = strlen( url );
      const size_t urlLengthAvailable =
        sizeof (URLString) / sizeof (char) - 1 - urlLength;
      const size_t optionLength = strlen( option );

      if ( optionLength <= urlLengthAvailable ) {
        strncat( url, option, optionLength );
      }

      CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
      DEBUG( fprintf( stderr, "url = %s\n", url ); )
    }
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )

  if ( printURL ) {
    puts( url );
  }

  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendGASPParameters - Append GASP-specific parameters to a URL
         (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendGASPParameters( const QueryParameters& queryParameters,
                                  URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();
  char options[ 256 ] = "";
  memset( options, 0, sizeof options );
  snprintf( options, sizeof options / sizeof *options,
            "&aod_range=%lg,%lg"
            "&std_range=%lg,%lg"
            "&cls_range=%lg,%lg"
            "&sfc_range=%lg,%lg"
            "&ch1_range=%lg,%lg"
            "&sig_range=%lg,%lg"
            "&mos_range=%lg,%lg"
            "&sca_range=%lg,%lg",
            dataFilterParameters.GASPAODMinimum(),
            dataFilterParameters.GASPAODMaximum(),
            dataFilterParameters.GASPSTDMinimum(),
            dataFilterParameters.GASPSTDMaximum(),
            dataFilterParameters.GASPCLSMinimum(),
            dataFilterParameters.GASPCLSMaximum(),
            dataFilterParameters.GASPSFCMinimum(),
            dataFilterParameters.GASPSFCMaximum(),
            dataFilterParameters.GASPCH1Minimum(),
            dataFilterParameters.GASPCH1Maximum(),
            dataFilterParameters.GASPSIGMinimum(),
            dataFilterParameters.GASPSIGMaximum(),
            dataFilterParameters.GASPMOSMinimum(),
            dataFilterParameters.GASPMOSMaximum(),
            dataFilterParameters.GASPSCAMinimum(),
            dataFilterParameters.GASPSCAMaximum() );
  CHECK( options[ sizeof options / sizeof *options - 1 ] == '\0' );
  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
    sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = strlen( options );

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options, optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendTROPOMIParameters - Append TROPOMI-specific parameters to a URL
         (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendTROPOMIParameters( const QueryParameters& queryParameters,
                                     URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();
  char options[ 256 ] = "";
  memset( options, 0, sizeof options );
  char extraOptions[ 64 ] = "";
  memset( extraOptions, 0, sizeof extraOptions );

  if ( dataFilterParameters.TROPOMIGroundPixelMinimum() != IMISSING ) {
    snprintf( extraOptions, sizeof extraOptions / sizeof *extraOptions,
              "&GROUND_PIXEL_RANGE=%d,%d",
              dataFilterParameters.TROPOMIGroundPixelMinimum(),
              dataFilterParameters.TROPOMIGroundPixelMaximum() );
  }

  const char* const extraOptions2 =
    dataFilterParameters.TROPOMIAllowNegativeCounts() ?
      "&ALLOW_NEGATIVE_COUNTS=1"
    : "";

  snprintf( options, sizeof options / sizeof *options,
            "&MINIMUM_QUALITY=%d&MAXIMUM_CLOUD_FRACTION=%f%s%s",
            dataFilterParameters.TROPOMIMinimumAcceptedQuality(),
            dataFilterParameters.TROPOMIMaximumCloudFraction(),
            extraOptions, extraOptions2 );
  CHECK( options[ sizeof options / sizeof *options - 1 ] == '\0' );
  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
    sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = strlen( options );

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options, optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendVIIRSNOAAParameters - Append VIIRSNOAA-specific parameters to a
         URL (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendVIIRSNOAAParameters( const QueryParameters& queryParameters,
                                       URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();
  char options[ 256 ] = "";
  memset( options, 0, sizeof options );
  snprintf( options, sizeof options / sizeof *options,
            "&MINIMUM_QUALITY=%s",
            dataFilterParameters.VIIRSNOAAMinimumAcceptedQuality() );
  CHECK( options[ sizeof options / sizeof *options - 1 ] == '\0' );
  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
    sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = strlen( options );

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options, optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendPandoraParameters - Append Pandora-specific parameters to a
         URL (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendPandoraParameters( const QueryParameters& queryParameters,
                                     URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();
  char options[ 256 ] = "";
  memset( options, 0, sizeof options );
  snprintf( options, sizeof options / sizeof *options,
            "&MINIMUM_QUALITY=%s",
            dataFilterParameters.PandoraMinimumAcceptedQuality() );
  CHECK( options[ sizeof options / sizeof *options - 1 ] == '\0' );
  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
    sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = strlen( options );

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options, optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendPurpleAirParameters - Append PurpleAir-specific parameters to a
         URL (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
         const char* const coverage              Coverage string.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendPurpleAirParameters( const QueryParameters& queryParameters,
                                       const char* const coverage,
                                       URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();

  std::string options; //x
  char number[ 512 ] = "";

  // OUT_IN_FLAG:

  const int outInFlag = dataFilterParameters.PurpleAirOutInFlag();

  if ( outInFlag == 0 ) {
    options += "&OUT_IN_FLAG=0";
  } else if ( outInFlag == 1 ) {
    options += "&OUT_IN_FLAG=1";
  }

  // MAXIMUM_DIFFERENCE:

  const double maximumDifference =
    dataFilterParameters.PurpleAirMaximumDifference();

  options += "&MAXIMUM_DIFFERENCE=";
  snprintf( number, sizeof number / sizeof *number, "%f", maximumDifference );
  options += number;

  // MAXIMUM_RATIO:

  const double maximumRatio =
    dataFilterParameters.PurpleAirMaximumRatio();

  options += "&MAXIMUM_RATIO=";
  snprintf( number, sizeof number / sizeof *number, "%f", maximumRatio );
  options += number;

  // DEFAULT_HUMIDITY:

  const double defaultHumidity =
    dataFilterParameters.PurpleAirDefaultHumidity();

  if ( defaultHumidity != MISSING ) {
    options += "&DEFAULT_HUMIDITY=";
    snprintf( number, sizeof number / sizeof *number, "%f", defaultHumidity );
    options += number;
  }

  // Daily and monthly variables are already aggregated so no options needed.

  if ( ! OR2( strstr( coverage, "daily" ), strstr( coverage, "monthly" ) ) ) {

    // AGGREGATE:

    const int aggregateMethod = queryParameters.aggregateMethod();

    if ( aggregateMethod == QueryParameters::AGGREGATE_DAILY_MEAN ) {
      options += "&AGGREGATE=daily";
    } else {
      options += "&AGGREGATE=hourly";
    }

    // MINIMUM_AGGREGATION_COUNT_PERCENTAGE:

    const double minimumAggregationCountPercentage =
      dataFilterParameters.PurpleAirMinimumAggregationCountPercentage();

    options += "&MINIMUM_AGGREGATION_COUNT_PERCENTAGE=";
    snprintf( number, sizeof number / sizeof *number, "%f",
              minimumAggregationCountPercentage );
    options += number;
  }

  // KEY:

  const std::string& key( dataFilterParameters.PurpleAirAPIKey() );

  if ( AND2( key.length(), isalnum( key[ 0 ] ) ) ) {
    options += "&KEY=";
    options += key.c_str();
  }

  // Append options to url:

  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
  sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = options.length();

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options.c_str(), optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendTEMPOParameters - Append TEMPO-specific parameters to a
         URL (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
=OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendTEMPOParameters( const QueryParameters& queryParameters,
                                   URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  std::string options; //x
  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();

  // MINIMUM_QUALITY:

  options += "&MINIMUM_QUALITY=";
  options += dataFilterParameters.TEMPOMinimumAcceptedQuality();

  // MAXIMUM_CLOUD_FRACTION:

  char number[ 512 ] = "";
  const double maximumCloudFraction =
    dataFilterParameters.TEMPOMaximumCloudFraction();
  options += "&MAXIMUM_CLOUD_FRACTION=";
  snprintf( number, sizeof number / sizeof *number, "%f", maximumCloudFraction );
  options += number;

  // MAXIMUM_SOLAR_ZENITH_ANGLE:

  const double maximumSolarZenithAngle =
    dataFilterParameters.TEMPOMaximumSolarZenithAngle();
  options += "&MAXIMUM_SOLAR_ZENITH_ANGLE=";
  snprintf( number, sizeof number / sizeof *number,
            "%f", maximumSolarZenithAngle );
  options += number;

  // ALLOW_NEGATIVE_COUNTS:

  if ( dataFilterParameters.TEMPOAllowNegativeCounts() ) {
    options += "&ALLOW_NEGATIVE_COUNTS=1";
  }

  // KEY:

  const std::string& key( dataFilterParameters.TEMPOKey() );

  if ( AND2( key.length(), isalnum( key[ 0 ] ) ) ) {
    options += "&KEY=";
    options += key.c_str();
  }

  // Append options to url:

  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
  sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = options.length();

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options.c_str(), optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendOMIParameters - Append OMI-specific parameters to a
         URL (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendOMIParameters( const QueryParameters& queryParameters,
                                 URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  std::string options; //x
  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();

  // MAXIMUM_CLOUD_FRACTION:

  char number[ 512 ] = "";
  const double maximumCloudFraction =
    dataFilterParameters.OMIMaximumCloudFraction();
  options += "&MAXIMUM_CLOUD_FRACTION=";
  snprintf( number, sizeof number / sizeof *number, "%f", maximumCloudFraction );
  options += number;

  // MAXIMUM_SOLAR_ZENITH_ANGLE:

  const double maximumSolarZenithAngle =
    dataFilterParameters.OMIMaximumSolarZenithAngle();
  options += "&MAXIMUM_SOLAR_ZENITH_ANGLE=";
  snprintf( number, sizeof number / sizeof *number,
            "%f", maximumSolarZenithAngle );
  options += number;

  // ALLOW_NEGATIVE_COUNTS:

  if ( dataFilterParameters.OMIAllowNegativeCounts() ) {
    options += "&ALLOW_NEGATIVE_COUNTS=1";
  }

  // Append options to url:

  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
  sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = options.length();

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options.c_str(), optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendOAQPSParameters - Append CMAQ-OAQPS-specific parameters to a
         URL (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendOAQPSParameters( const QueryParameters& queryParameters,
                                   URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  std::string options; //x
  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();

  // KEY:

  const std::string& key( dataFilterParameters.OAQPSKey() );

  if ( AND2( key.length(), isalnum( key[ 0 ] ) ) ) {
    options += "&KEY=";
    options += key.c_str();
  }

  // Append options to url:

  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
  sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = options.length();

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options.c_str(), optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendCALIPSOParameters - Append CALIPSO-specific parameters to a URL
         (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendCALIPSOParameters( const QueryParameters& queryParameters,
                                     URLString url ) {

  PRE03( queryParameters.invariant(), url, *url );

  const DataFilterParameters& dataFilterParameters =
    queryParameters.dataFilterParameters();
  const std::string dataVersionCopy( dataFilterParameters.dataVersion() );
  const char* const dataVersion =
    dataVersionCopy.length() ? dataVersionCopy.c_str() : 0;
  char options[ 256 ] = "";
  memset( options, 0, sizeof options );
  snprintf( options, sizeof options / sizeof *options,
            "&MINIMUM_CAD=%lg"
            "&MAXIMUM_UNCERTAINTY=%lg%s%s",
            dataFilterParameters.CALIPSOMinimumAcceptedCADScore(),
            dataFilterParameters.CALIPSOMaximumAcceptedUncertainty(),
            dataVersion ? "&DATA_VERSION=" : "",
            dataVersion ? dataVersion : "" );
  CHECK( options[ sizeof options / sizeof *options - 1 ] == '\0' );

  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
    sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t optionsLength = strlen( options );

  if ( optionsLength <= urlLengthAvailable ) {
    strncat( url, options, optionsLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendRegriddingParameters - Append regridding parameters to a URL
         (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendRegriddingParameters( const QueryParameters& queryParameters,
                                        URLString url ) {

  PRE04( queryParameters.invariant(),
         queryParameters.regridMethod() != QueryParameters::REGRID_NONE,
         url, *url );

  const CMAQGridParameters& cmaqGridParameters =
    queryParameters.cmaqGridParameters();
  const char* const regridMethodName =
    QueryParameters::regridMethodNames[ queryParameters.regridMethod() ];
  const int gdtyp = cmaqGridParameters.gdtyp();
  const double sphereRadius = cmaqGridParameters.sphereRadius();
  const int ncols = cmaqGridParameters.ncols();
  const int nrows = cmaqGridParameters.nrows();
  const double xorig = cmaqGridParameters.xorig();
  const double yorig = cmaqGridParameters.yorig();
  const double xcell = cmaqGridParameters.xcell();
  const double ycell = cmaqGridParameters.ycell();
  char regridParameters[ 1024 ] = "";
  memset( regridParameters, 0, sizeof regridParameters );

  switch ( gdtyp ) {
  case CMAQGridParameters::LATGRD3:
    snprintf( regridParameters,
              sizeof regridParameters / sizeof *regridParameters,
              "&REGRID=%s&LONLAT=1"
              "&ELLIPSOID=%lf,%lf&GRID=%d,%d,%lf,%lf,%lf,%lf",
              regridMethodName,
              sphereRadius, sphereRadius,
              ncols, nrows, xorig, yorig, xcell, ycell );
    CHECK( regridParameters[ sizeof regridParameters /
                             sizeof *regridParameters - 1 ] == '\0' );
    break;
  case CMAQGridParameters::LAMGRD3:
    {
      const double p_alp = cmaqGridParameters.p_alp();
      const double p_bet = cmaqGridParameters.p_bet();
      const double xcent = cmaqGridParameters.xcent();
      const double ycent = cmaqGridParameters.ycent();
      snprintf( regridParameters,
                sizeof regridParameters / sizeof *regridParameters,
                "&REGRID=%s&LAMBERT=%lf,%lf,%lf,%lf"
                "&ELLIPSOID=%lf,%lf&GRID=%d,%d,%lf,%lf,%lf,%lf",
                regridMethodName, p_alp, p_bet, xcent, ycent,
                sphereRadius, sphereRadius,
                ncols, nrows, xorig, yorig, xcell, ycell );
      CHECK( regridParameters[ sizeof regridParameters /
                               sizeof *regridParameters - 1 ] == '\0' );
    }
    break;
  case CMAQGridParameters::POLGRD3:
    {
      const double xcent = cmaqGridParameters.xcent();
      const double ycent = cmaqGridParameters.ycent();
      const double p_bet = cmaqGridParameters.p_bet();
      snprintf( regridParameters,
                sizeof regridParameters / sizeof *regridParameters,
                "&REGRID=%s&STEREOGRAPHIC=%lf,%lf,%lf"
                "&ELLIPSOID=%lf,%lf&GRID=%d,%d,%0.1lf,%0.1lf,%0.1lf,%0.1lf",
                regridMethodName, xcent, ycent, p_bet,
                sphereRadius, sphereRadius,
                ncols, nrows, xorig, yorig, xcell, ycell );
      CHECK( regridParameters[ sizeof regridParameters /
                               sizeof *regridParameters - 1 ] == '\0' );
    }
    break;
  default:
    CHECK( gdtyp == CMAQGridParameters::EQMGRD3 );
    {
      const double p_gam = cmaqGridParameters.p_gam();
      snprintf( regridParameters,
                sizeof regridParameters / sizeof *regridParameters,
                "&REGRID=%s&MERCATOR=%lf"
                "&ELLIPSOID=%lf,%lf&GRID=%d,%d,%0.1lf,%0.1lf,%0.1lf,%0.1lf",
                regridMethodName, p_gam,
                sphereRadius, sphereRadius,
                ncols, nrows, xorig, yorig, xcell, ycell );
      CHECK( regridParameters[ sizeof regridParameters /
                               sizeof *regridParameters - 1 ] == '\0' );
    }
    break;
  }

  const size_t urlLength = strlen( url );
  const size_t urlLengthAvailable =
    sizeof (URLString) / sizeof (char) - 1 - urlLength;
  const size_t regridParametersLength = strlen( regridParameters );

  if ( regridParametersLength <= urlLengthAvailable ) {
    strncat( url, regridParameters, regridParametersLength );
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



/******************************************************************************
PURPOSE: appendLevelParameters - Append regridding level parameters to a URL
         (OGC-WCS query).
INPUTS:  const QueryParameters& queryParameters  Query parameters to convert.
OUTPUTS: URLString url                           HTTP OGC-WCS query string.
******************************************************************************/

static void appendLevelParameters( const QueryParameters& queryParameters,
                                   URLString url ) {

  PRE04( queryParameters.invariant(),
         queryParameters.regridMethod() != QueryParameters::REGRID_NONE,
         url, *url );

  // Get level parameters:

  const CMAQGridParameters& cmaqGridParameters =
    queryParameters.cmaqGridParameters();
  const int nlays = cmaqGridParameters.nlays();
  const int vgtyp = cmaqGridParameters.vgtyp();
  const double vgtop = cmaqGridParameters.vgtop();
  const double g = cmaqGridParameters.g();
  const double R = cmaqGridParameters.R();
  const double A = cmaqGridParameters.A();
  const double T0 = cmaqGridParameters.T0();
  const double P0 = cmaqGridParameters.P0();

  // Append fixed-size level parameters:

  char levelParameters[ 1024 ] = "";
  memset( levelParameters, 0, sizeof levelParameters );

  snprintf( levelParameters,
            sizeof levelParameters / sizeof *levelParameters,
            "&LEVELS=%d,%d,%0.1lf",
            nlays, vgtyp, vgtop );
  CHECK( levelParameters[ sizeof levelParameters /
                          sizeof *levelParameters - 1 ] == '\0' );

  size_t urlLength = strlen( url );
  size_t urlLengthAvailable =
    sizeof (URLString) / sizeof (char) - 1 - urlLength;
  size_t levelParametersLength = strlen( levelParameters );

  if ( levelParametersLength <= urlLengthAvailable ) {
    strncat( url, levelParameters, levelParametersLength );

    // Append level values:

    const double* vglvls = cmaqGridParameters.vglvls();
    memset( levelParameters, 0, sizeof levelParameters );
    int level = 0;

    for ( level = 0; level <= nlays; ++level ) {
      snprintf( levelParameters,
                sizeof levelParameters / sizeof *levelParameters,
                ",%0.4lf", vglvls[ level ] );
      CHECK( levelParameters[ sizeof levelParameters /
                              sizeof *levelParameters - 1 ] == '\0' );

      urlLength = strlen( url );
      urlLengthAvailable =
        sizeof (URLString) / sizeof (char) - 1 - urlLength;
      levelParametersLength = strlen( levelParameters );

      if ( levelParametersLength <= urlLengthAvailable ) {
        strncat( url, levelParameters, levelParametersLength );
      } else {
        level = nlays + 1;
      }
    }

    // Append parameters used to compute elevation:

    if ( level == nlays + 1 ) {
      snprintf( levelParameters,
                sizeof levelParameters / sizeof *levelParameters,
                ",%lf,%lf,%lf,%lf,%lf",
                g, R, A, T0, P0 );
      CHECK( levelParameters[ sizeof levelParameters /
                              sizeof *levelParameters - 1 ] == '\0' );

      urlLength = strlen( url );
      urlLengthAvailable =
        sizeof (URLString) / sizeof (char) - 1 - urlLength;
      levelParametersLength = strlen( levelParameters );

      if ( levelParametersLength <= urlLengthAvailable ) {
        strncat( url, levelParameters, levelParametersLength );
      }
    }
  }

  CHECK( url[ sizeof (URLString) / sizeof (char) - 1 ] == '\0' );
  DEBUG( fprintf( stderr, "url = %s\n", url ); )
  POST02( *url, strlen( url ) < sizeof (URLString) / sizeof (char) );
}



} // namespace emvl.



