
/******************************************************************************
PURPOSE: CMAQFileReaderModel.cpp - Implements Model (state) to be controlled.

NOTES:

HISTORY: 2022-01-10 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <string>    // For std::string.
#include <vector>    // For std::vector.
#include <algorithm> // For std::find.

#ifdef DEBUGGING
#include <iostream> // For std::cerr, std::endl.
#endif

#include <Assertions.h>     // For PRE(), POST(), AND2().
#include <Bounds.h>         // For class emvl::Bounds.
#include <DataUtilities.h>  // For emvl::getHomeDirectory(), emvl::subsetCMAQFile().
#include <DatasetManager.h> // For emvl::DatasetManager::getWorkingDirectory().

#include "CMAQFileReaderModel.h" // For emvl::CMAQFileReaderModel.

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: CMAQFileReaderModel - Construct with initial state:
         bounds = global
         first/lastTimestamp = now
         layer = 0
         aggregate = 0
         mode = LOCAL
         outputDirectory = home directory
         data/ht/zf/wwindDirectory = cwd
         data/ht/zf/wwindFile = empty()
         dataVariable = empty().
******************************************************************************/

CMAQFileReaderModel::CMAQFileReaderModel() {
  layer = 0;
  aggregate = 0;
  outputDirectory = emvl::getHomeDirectory();
  mode = LOCAL;

  const std::string cwd( emvl::DatasetManager::getWorkingDirectory(user, host));
  dataDirectory[  mode ] = cwd;
  htDirectory[    mode ] = cwd;
  zfDirectory[    mode ] = cwd;
  wwindDirectory[ mode ] = cwd;

  // listing is SLOW: it reads first 3 bytes of files in cwd for "CDF":

  const std::string listing(
    emvl::DatasetManager::getDirectoryListing( user, host, cwd ) ); // SLOW.

  dataDirectoryListing[  mode ] = listing;
  htDirectoryListing[    mode ] = listing;
  zfDirectoryListing[    mode ] = listing;
  wwindDirectoryListing[ mode ] = listing;

  POST18( bounds.west() == -180.0,
          bounds.east() ==  180.0,
          bounds.south() == -90.0,
          bounds.north() ==  90.0,
          layer == 0,
          aggregate == 0,
          mode == LOCAL,
          ! outputDirectory.empty(),
          ! dataDirectory[ mode ].empty(),
          htDirectory[    mode ] == dataDirectory[ mode ],
          zfDirectory[    mode ] == dataDirectory[ mode ],
          wwindDirectory[ mode ] == dataDirectory[ mode ],
          dataFile[     mode ].empty(),
          dataVariableListing[ mode ].empty(),
          dataVariable[ mode ].empty(),
          htFile[       mode ].empty(),
          zfFile[       mode ].empty(),
          wwindFile[    mode ].empty() );
}



/******************************************************************************
PURPOSE: ~CMAQFileReaderModel - Deallocate resources.
******************************************************************************/

CMAQFileReaderModel::~CMAQFileReaderModel() {
  layer = 0;
  aggregate = 0;
  mode = LOCAL;
}



/******************************************************************************
PURPOSE: setOutputDirectory - Set local directory to write subset file.
INPUTS:  const std::string& outputDirectory  Local directory to write subset.
******************************************************************************/

void CMAQFileReaderModel::setOutputDirectory(
  const std::string& outputDirectory ) {
  PRE( emvl::isDirectory( outputDirectory ) );
  this->outputDirectory = outputDirectory;
  POST( getOutputDirectory() == outputDirectory );
}



/******************************************************************************
PURPOSE: setSubset - Set subset options.
INPUTS:  const emvl::Bounds& bounds  Longitude-Latitude bounds.
         const emvl::Timestamp& firstTimestamp  First timestamp of subset.
         const emvl::Timestamp& lastTimestamp   Last  timestamp of subset.
         const int layer                  -1 = integrate, 0 = all, > 0 = single
         const int aggregate              0 = none, 1 = every 24 hours.
******************************************************************************/

void CMAQFileReaderModel::setSubset( const emvl::Bounds& bounds,
                                     const emvl::Timestamp& firstTimestamp,
                                     const emvl::Timestamp& lastTimestamp,
                                     const int layer,
                                     const int aggregate ) {

  PRE6( bounds.invariant(),
        firstTimestamp.invariant(),
        lastTimestamp.invariant(),
        firstTimestamp <= lastTimestamp,
        layer >= -1,
        IS_BOOL( aggregate ) );

  this->bounds         = bounds;
  this->firstTimestamp = firstTimestamp;
  this->lastTimestamp  = lastTimestamp;
  this->layer          = layer;
  this->aggregate      = aggregate;

  POST5( this->bounds == bounds,
         this->firstTimestamp == firstTimestamp,
         this->lastTimestamp  == lastTimestamp,
         this->layer          == layer,
         this->aggregate      == aggregate );
}



/******************************************************************************
PURPOSE: setMode - Set host mode: LOCAL or REMOTE.
RETURNS: const int mode  LOCAL or REMOTE.
******************************************************************************/

void CMAQFileReaderModel::setMode( const int mode ) {
  PRE( IN3( mode, LOCAL, REMOTE ) );

  if ( mode != this->mode ) {
    this->mode = mode;
  }

  POST( getMode() == mode );
}



/******************************************************************************
PURPOSE: setUser - Set login name of user.
INPUTS: const std::string& name  New name of user.
******************************************************************************/

void CMAQFileReaderModel::setUser( const std::string& name ) {
  PRE( true );
  user = name;
  POST( IMPLIES_ELSE( name.empty(), getUser().empty(), getUser() == name ) );
}



/******************************************************************************
PURPOSE: setHost - Set name of remote host computer and connect to it and
         update directories and listings.
INPUTS:  const std::string& name  New name of host.
NOTES:   If host is non-empty then an attempt will be made to install
         CMAQSubset on the remote host. This routine is SLOW so it is Memoized.
******************************************************************************/

void CMAQFileReaderModel::setHost( const std::string& name ) {
  PRE( true );

  if ( name.empty() ) {
    host.clear();
    dataDirectory[         REMOTE ].clear();
    htDirectory[           REMOTE ].clear();
    zfDirectory[           REMOTE ].clear();
    wwindDirectory[        REMOTE ].clear();
    dataDirectoryListing[  REMOTE ].clear();
    htDirectoryListing[    REMOTE ].clear();
    zfDirectoryListing[    REMOTE ].clear();
    wwindDirectoryListing[ REMOTE ].clear();
    dataFile[              REMOTE ].clear();
    dataVariableListing[   REMOTE ].clear();
    dataVariable[          REMOTE ].clear();
    htFile[                REMOTE ].clear();
    zfFile[                REMOTE ].clear();
    wwindFile[             REMOTE ].clear();
  } else if ( AND2( mode == REMOTE, name != host ) ) { // If host name differs.

    // Check host name to avoid repeated install.

    bool ok = std::find( installedHosts.begin(), installedHosts.end(), name )
              != installedHosts.end(); // Found?

    if ( ! ok ) { // Not found.
      ok = emvl::DatasetManager::installRemoteSubsetter( user, name ); // SLOW

      if ( ok ) {
        try {
          installedHosts.push_back( name ); //x Memoize for next time.
        } catch( ... ) {
        }
      }
    }

    if ( ok ) { // Update state based on new remote host:
      host = name;

      const std::string cwd(
        emvl::DatasetManager::getWorkingDirectory( user, host ) ); // SLOW.

      dataDirectory[  REMOTE ] = cwd;
      htDirectory[    REMOTE ] = cwd;
      zfDirectory[    REMOTE ] = cwd;
      wwindDirectory[ REMOTE ] = cwd;

      const std::string listing(
        emvl::DatasetManager::getDirectoryListing( user, host, cwd ) ); // SLOW

      dataDirectoryListing[  REMOTE ] = listing;
      htDirectoryListing[    REMOTE ] = listing;
      zfDirectoryListing[    REMOTE ] = listing;
      wwindDirectoryListing[ REMOTE ] = listing;

      dataFile[     REMOTE ].clear();
      dataVariableListing[ REMOTE ].clear();
      dataVariable[ REMOTE ].clear();
      htFile[       REMOTE ].clear();
      zfFile[       REMOTE ].clear();
      wwindFile[    REMOTE ].clear();
    }
  }

  POST( true );
}



/******************************************************************************
PURPOSE: setDataDirectory - Set name of data directory.
INPUTS: const std::string& name  New name of data directory.
******************************************************************************/

void CMAQFileReaderModel::setDataDirectory( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, emvl::isDirectory( name ) ) );

  if ( name != dataDirectory[ mode ] ) {
    const std::string listing(
      emvl::DatasetManager::getDirectoryListing( user,
                                                 mode == REMOTE ? host : "",
                                                 name ) ); // SLOW

    if ( ! listing.empty() ) {
      dataDirectory[ mode ] = name;
      dataDirectoryListing[ mode ] = listing;
      dataFile[ mode ].clear();
      dataVariableListing[ mode ].clear();
      dataVariable[ mode ].clear();

      if ( htDirectory[ mode ].empty() ) {
        htDirectory[ mode ] = name;
        htDirectoryListing[ mode ] = listing;
        htFile[ mode ].clear();
      }

      if ( zfDirectory[ mode ].empty() ) {
        zfDirectory[ mode ] = name;
        zfDirectoryListing[ mode ] = listing;
        zfFile[ mode ].clear();
      }

      if ( wwindDirectory[ mode ].empty() ) {
        wwindDirectory[ mode ] = name;
        wwindDirectoryListing[ mode ] = listing;
        wwindFile[ mode ].clear();
      }
    }
  }

  POST( IMPLIES( dataDirectory[ mode ] == name,
                 ! dataDirectoryListing[ mode ].empty() ) );
}



/******************************************************************************
PURPOSE: setDataFile - Set name of data file.
INPUTS: const std::string& name  New name of data file.
******************************************************************************/

void CMAQFileReaderModel::setDataFile( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, isDataFile( name ) ) );

  if ( name != dataFile[ mode ] ) {
    dataFile[ mode ] = name;
    dataVariableListing[ mode ].clear();
    dataVariable[ mode ].clear();

    const char s = host.empty() ? emvl::slash : '/'; // Use '/' for remote hosts.
    const std::string listing(
      emvl::DatasetManager::getVariableListing( user,
                                                mode == REMOTE ? host : "",
                                                dataDirectory[ mode ] + s +
                                                dataFile[ mode ] ) );

    if ( ! listing.empty() ) {
      dataVariableListing[ mode ] = listing;
    }
  }

  POST2( dataFile[ mode ] == name, dataVariable[ mode ].empty() );
}



/******************************************************************************
PURPOSE: setDataVariable - Set name of data variable.
INPUTS: const std::string& name  New name of data file.
******************************************************************************/

void CMAQFileReaderModel::setDataVariable( const std::string& name ) {
  PRE( ! name.empty() );
  dataVariable[ mode ] = name;
  POST( dataVariable[ mode ] == name );
}



/******************************************************************************
PURPOSE: setHTDirectory - Set name of HT file directory.
 INPUTS: const std::string& name  New name of HT file directory.
******************************************************************************/

void CMAQFileReaderModel::setHTDirectory( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, emvl::isDirectory( name ) ) );

  if ( name != htDirectory[ mode ] ) {
    const std::string listing( // Check if the same as another then just copy.
      name == dataDirectory[ mode ] ? dataDirectoryListing[ mode ] :
      emvl::DatasetManager::getDirectoryListing( user,
                                                 mode == REMOTE ? host : "",
                                                 name ) ); // SLOW

    if ( ! listing.empty() ) {
      htDirectory[ mode ] = name;
      htDirectoryListing[ mode ] = listing;
      htFile[ mode ].clear();
    }
  }

  POST( IMPLIES( htDirectory[ mode ] == name,
                 ! htDirectoryListing[ mode ].empty() ) );
}



/******************************************************************************
PURPOSE: setHTFile - Set name of HT file.
INPUTS: const std::string& name  New name of HT file.
******************************************************************************/

void CMAQFileReaderModel::setHTFile( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, isHTFile( name ) ) );

  if ( name != htFile[ mode ] ) {
    htFile[ mode ] = name;
  }

  POST( htFile[ mode ] == name );
}



/******************************************************************************
PURPOSE: setZFDirectory - Set name of ZF file directory.
INPUTS: const std::string& name  New name of ZF file directory.
******************************************************************************/

void CMAQFileReaderModel::setZFDirectory( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, emvl::isDirectory( name ) ) );

  if ( name != zfDirectory[ mode ] ) {
    const std::string listing( // Check if the same as another then just copy.
      name == dataDirectory[ mode ] ? dataDirectoryListing[ mode ] :
      name == htDirectory[ mode ] ? htDirectoryListing[ mode ] :
      emvl::DatasetManager::getDirectoryListing( user,
                                                mode == REMOTE ? host : "",
                                                 name ) ); // SLOW

    if ( ! listing.empty() ) {
      zfDirectory[ mode ] = name;
      zfDirectoryListing[ mode ] = listing;
      zfFile[ mode ].clear();
    }
  }

  POST( IMPLIES( zfDirectory[ mode ] == name,
                 ! zfDirectoryListing[ mode ].empty() ) );
}



/******************************************************************************
PURPOSE: setZFFile - Set name of ZF file.
INPUTS: const std::string& name  New name of ZF file.
******************************************************************************/

void CMAQFileReaderModel::setZFFile( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, isZFFile( name ) ) );

  if ( name != zfFile[ mode ] ) {
    zfFile[ mode ] = name;
  }

  POST( zfFile[ mode ] == name );
}



/******************************************************************************
PURPOSE: setWWINDDirectory - Set name of WWIND file directory.
INPUTS: const std::string& name  New name of WWIND file directory.
******************************************************************************/

void CMAQFileReaderModel::setWWINDDirectory( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, emvl::isDirectory( name ) ) );

  if ( name != wwindDirectory[ mode ] ) {
    const std::string listing( // Check if the same as another then just copy.
      name == dataDirectory[ mode ] ? dataDirectoryListing[ mode ] :
      name == htDirectory[ mode ] ? htDirectoryListing[ mode ] :
      name == zfDirectory[ mode ] ? zfDirectoryListing[ mode ] :
      emvl::DatasetManager::getDirectoryListing( user,
                                                 mode == REMOTE ? host : "",
                                                 name ) ); // SLOW

    if ( ! listing.empty() ) {
      wwindDirectory[ mode ] = name;
      wwindDirectoryListing[ mode ] = listing;
      wwindFile[ mode ].clear();
    }
  }

  POST( IMPLIES( wwindDirectory[ mode ] == name,
                 ! wwindDirectoryListing[ mode ].empty() ) );
}



/******************************************************************************
PURPOSE: setWWINDFile - Set name of WWIND file.
INPUTS: const std::string& name  New name of WWIND file.
******************************************************************************/

void CMAQFileReaderModel::setWWINDFile( const std::string& name ) {
  PRE2( ! name.empty(), OR2( mode == REMOTE, isWWINDFile( name ) ) );

  if ( name != wwindFile[ mode ] ) {
    wwindFile[ mode ] = name;
  }

  POST( wwindFile[ mode ] == name );
}



/******************************************************************************
PURPOSE: readCMAQFile - Read CMAQ file subsetted by bounds, time and  variable.
NOTES: Sets outputFile to name of local xdr file if successful,
       else empty string and a failure message is printed to stderr.
******************************************************************************/

void CMAQFileReaderModel::readCMAQFile() {
  PRE3( ! dataDirectory[ mode ].empty(),
        ! dataFile[ mode ].empty(),
        ! dataVariable[ mode ].empty() );

  outputFile.clear();

  std::string pathedDataFileName( dataDirectory[ mode ] + '/' + dataFile[mode]); //x
  std::string pathedHTFileName; //x
  std::string pathedZFFileName; //x
  std::string pathedWWINDFileName; //x

  if ( AND2( ! htDirectory[ mode ].empty(), ! htFile[ mode ].empty() ) ) {
    pathedHTFileName = htDirectory[ mode ] + '/' + htFile[ mode ]; //x
  }

  if ( AND2( ! zfDirectory[ mode ].empty(), ! zfFile[ mode ].empty() ) ) {
    pathedZFFileName = zfDirectory[ mode ] + '/' + zfFile[ mode ]; //x
  }

  if ( AND2( ! wwindDirectory[ mode ].empty(), ! wwindFile[ mode ].empty() ) ) {
    pathedWWINDFileName = wwindDirectory[ mode ] + '/' + wwindFile[ mode ]; //x
  }

  outputFile =
    emvl::DatasetManager::subsetCMAQFile( user,
                                          mode == REMOTE ? host : "",
                                          pathedDataFileName,
                                          dataVariable[ mode ],
                                          pathedHTFileName,
                                          pathedZFFileName,
                                          pathedWWINDFileName,
                                          outputDirectory,
                                          bounds,
                                          firstTimestamp,
                                          lastTimestamp,
                                          layer,
                                          aggregate );

  DEBUG( std::cerr << outputFile << std::endl; )
  POST( true );
}



/******************************************************************************
PURPOSE: clearInputFiles - Clear all input data files and variables of mode.
******************************************************************************/

void CMAQFileReaderModel::clearInputFiles() {
  PRE( true );

  dataFile[     mode ].clear();
  dataVariable[ mode ].clear();
  dataVariableListing[ mode ].clear();
  htFile[       mode ].clear();
  zfFile[       mode ].clear();
  wwindFile[    mode ].clear();

  POST6( dataFile[     mode ].empty(),
         dataVariable[ mode ].empty(),
         dataVariableListing[ mode ].empty(),
         htFile[       mode ].empty(),
         zfFile[       mode ].empty(),
         wwindFile[    mode ].empty() );
}



/******************************************************************************
PURPOSE: clearOutputFile - Clear output data file.
******************************************************************************/

void CMAQFileReaderModel::clearOutputFile() {
  PRE( true );
  outputFile.clear();
  POST( outputFile.empty() );
}



/******************************************************************************
PURPOSE: getOutputFile - Get name of last successful output subset file.
RETURNS: const std::string&  Name - may be empty() if readCMAQFile() failed.
******************************************************************************/

const std::string& CMAQFileReaderModel::getOutputFile() const {
  PRE( true );
  const std::string& result = outputFile;
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: getOutputDirectory - Get name of directory to write output file.
RETURNS: const std::string&  Name.
******************************************************************************/

const std::string& CMAQFileReaderModel::getOutputDirectory() const {
  PRE( true );
  const std::string& result = outputDirectory;
  POST( emvl::isDirectory( result ) );
  return result;
}



/******************************************************************************
PURPOSE: getMode - Get host mode: LOCAL or REMOTE.
RETURNS: const std::string&  Name of remote user - may be empty().
******************************************************************************/

int CMAQFileReaderModel::getMode() const {
  PRE( true );
  const int result = mode;
  POST( IN3( result, LOCAL, REMOTE ) );
  return result;
}



/******************************************************************************
PURPOSE: getUser - Get login name of user or blank if on local host.
RETURNS: const std::string&  Name of remote user - may be empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getUser() const {
  PRE( true );
  return user;
}



/******************************************************************************
PURPOSE: getHost - Get name of host computer or blank if local host.
RETURNS: const std::string&  Name of remote host - may be empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getHost() const {
  PRE( true );
  return host;
}



/******************************************************************************
PURPOSE: getDataDirectory - Get name of data directory.
RETURNS: const std::string&  Name.
******************************************************************************/

const std::string& CMAQFileReaderModel::getDataDirectory() const {
  PRE( true );
  const std::string& result = dataDirectory[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      emvl::isDirectory( result ),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getDataDirectoryListing - Get host data directory listing.
RETURNS: std::string  Sorted list of newline-delimited names of subdirectories
                      and CMAQ files.
******************************************************************************/

const std::string& CMAQFileReaderModel::getDataDirectoryListing() const {
  PRE( true );
  const std::string& result = dataDirectoryListing[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      ! result.empty(),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getDataFile - Get name of selected data file.
RETURNS: const std::string&  Name of file - may be empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getDataFile() const {
  PRE( true );
  const std::string& result = dataFile[ mode ];
  POST( OR3( result.empty(),
             mode == REMOTE,
             isDataFile( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getDataVariableListing - Get listing of CMAQ variables in dataFile.
RETURNS: std::string  Sorted list of newline-delimited names of variables
                      in dataFile[ mode ] or empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getDataVariableListing() const {
  PRE( ! dataFile[ mode ].empty() );
  const std::string& result = dataVariableListing[ mode ];
  return result;
}



/******************************************************************************
PURPOSE: getDataVariable - Get name of selected data variable.
RETURNS: const std::string&  Name of file - may be empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getDataVariable() const {
  PRE( true );
  const std::string& result = dataVariable[ mode ];
  return result;
}



/******************************************************************************
PURPOSE: getHTDirectory - Get name of HT directory.
RETURNS: const std::string&  Name.
******************************************************************************/

const std::string& CMAQFileReaderModel::getHTDirectory() const {
  PRE( true );
  const std::string& result = htDirectory[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      emvl::isDirectory( result ),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getHTDirectoryListing - Get host HT directory listing.
RETURNS: std::string  Sorted list of newline-delimited names of subdirectories
                      and CMAQ files.
******************************************************************************/

const std::string& CMAQFileReaderModel::getHTDirectoryListing() const {
  PRE( true );
  const std::string& result = htDirectoryListing[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      ! result.empty(),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getHTFile - Get name of HT file.
RETURNS: const std::string&  Name of file - may be empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getHTFile() const {
  PRE( true );
  const std::string& result = htFile[ mode ];
  POST( OR3( result.empty(),
             mode == REMOTE,
             isHTFile( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getZFDirectory - Get name of ZF directory.
RETURNS: const std::string&  Name.
******************************************************************************/

const std::string& CMAQFileReaderModel::getZFDirectory() const {
  PRE( true );
  const std::string& result = zfDirectory[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      emvl::isDirectory( result ),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getZFDirectoryListing - Get host ZF directory listing.
RETURNS: std::string  Sorted list of newline-delimited names of subdirectories
                      and CMAQ files.
******************************************************************************/

const std::string& CMAQFileReaderModel::getZFDirectoryListing() const {
  PRE( true );
  const std::string& result = zfDirectoryListing[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      ! result.empty(),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getZFFile - Get name of ZF file.
RETURNS: const std::string&  Name of file - may be empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getZFFile() const {
  PRE( true );
  const std::string& result = zfFile[ mode ];
  POST( OR3( result.empty(),
             mode == REMOTE,
             isZFFile( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getWWINDDirectory - Get name of WWIND directory.
RETURNS: const std::string&  Name.
******************************************************************************/

const std::string& CMAQFileReaderModel::getWWINDDirectory() const {
  PRE( true );
  const std::string& result = wwindDirectory[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      emvl::isDirectory( result ),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getWWINDDirectoryListing - Get host WWIND directory listing.
RETURNS: std::string  Sorted list of newline-delimited names of subdirectories
                      and CMAQ files.
******************************************************************************/

const std::string& CMAQFileReaderModel::getWWINDDirectoryListing() const {
  PRE( true );
  const std::string& result = wwindDirectoryListing[ mode ];
  POST( IMPLIES_ELSE( mode == LOCAL,
                      ! result.empty(),
                      OR2( host.empty(), ! result.empty() ) ) );
  return result;
}



/******************************************************************************
PURPOSE: getZFFile - Get name of WWIND file.
RETURNS: const std::string&  Name of file - may be empty().
******************************************************************************/

const std::string& CMAQFileReaderModel::getWWINDFile() const {
  PRE( true );
  const std::string& result = wwindFile[ mode ];
  POST( OR3( result.empty(),
             mode == REMOTE,
             isWWINDFile( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: isDataFile - Is this a valid data file?
INPUTS:  const std::string& name  Name to check.
RETURNS: bool true if valid (or mode == REMOTE) else false.
NOTES:   When mode == REMOTE no file check is done since it would be too slow.
******************************************************************************/

bool CMAQFileReaderModel::isDataFile( const std::string& name ) const {
  return AND2( ! name.empty(),
               OR2( mode == REMOTE,
                    emvl::isNetCDFFile( dataDirectory[ LOCAL ] +
                                        emvl::slash + name ) ) );
}



/******************************************************************************
PURPOSE: isHTFile - Is this a valid HT file?
INPUTS:  const std::string& name  Name to check.
RETURNS: bool true if valid (or mode == REMOTE) else false.
NOTES:   When mode == REMOTE no file check is done since it would be too slow.
******************************************************************************/

bool CMAQFileReaderModel::isHTFile( const std::string& name ) const {
  return AND2( ! name.empty(),
               OR2( mode == REMOTE,
                    emvl::isNetCDFFile( htDirectory[ LOCAL ] +
                                        emvl::slash + name ) ) );
}



/******************************************************************************
PURPOSE: isZFFile - Is this a valid ZF file?
INPUTS:  const std::string& name  Name to check.
RETURNS: bool true if valid (or mode == REMOTE) else false.
NOTES:   When mode == REMOTE no file check is done since it would be too slow.
******************************************************************************/

bool CMAQFileReaderModel::isZFFile( const std::string& name ) const {
  return AND2( ! name.empty(),
               OR2( mode == REMOTE,
                    emvl::isNetCDFFile( zfDirectory[ LOCAL ] +
                                        emvl::slash + name ) ) );
}



/******************************************************************************
PURPOSE: isWWINDFile - Is this a valid WWIND file?
INPUTS:  const std::string& name  Name to check.
RETURNS: bool true if valid (or mode == REMOTE) else false.
NOTES:   When mode == REMOTE no file check is done since it would be too slow.
******************************************************************************/

bool CMAQFileReaderModel::isWWINDFile( const std::string& name ) const {
  return AND2( ! name.empty(),
               OR2( mode == REMOTE,
                    emvl::isNetCDFFile( wwindDirectory[ LOCAL ] +
                                        emvl::slash + name ) ) );
}



/******************************************************************************
PURPOSE: invariant - Is object in a valid state?
RETURNS: bool true or else there is a defect in the program.
NOTES: Called implicitly by PRE() and POST() unless compiled with
       -DNO_ASSERTIONS.
******************************************************************************/

bool CMAQFileReaderModel::invariant() const {
  const bool result =
    AND20( bounds.invariant(),
           firstTimestamp.invariant(),
           lastTimestamp.invariant(),
           firstTimestamp <= lastTimestamp,
           layer >= -1,
           IS_BOOL( aggregate ),
           IN3( mode, LOCAL, REMOTE ),
           ! dataDirectory[ LOCAL ].empty(),
           ! htDirectory[ LOCAL ].empty(),
           ! zfDirectory[ LOCAL ].empty(),
           ! wwindDirectory[ LOCAL ].empty(),
           ! dataDirectoryListing[ LOCAL ].empty(),
           ! htDirectoryListing[ LOCAL ].empty(),
           ! zfDirectoryListing[ LOCAL ].empty(),
           ! wwindDirectoryListing[ LOCAL ].empty(),
           OR2( dataFile[ LOCAL ].empty(), isDataFile( dataFile[ LOCAL ] ) ),
           OR2( htFile[ LOCAL ].empty(), isHTFile( htFile[ LOCAL ] ) ),
           OR2( zfFile[ LOCAL ].empty(), isZFFile( zfFile[ LOCAL ] ) ),
           OR2( wwindFile[ LOCAL ].empty(), isWWINDFile( wwindFile[ LOCAL ] ) ),
           IMPLIES( AND2( mode == REMOTE, ! host.empty() ),
                    AND8( ! dataDirectory[ REMOTE ].empty(),
                          ! htDirectory[ REMOTE ].empty(),
                          ! zfDirectory[ REMOTE ].empty(),
                          ! wwindDirectory[ REMOTE ].empty(),
                          ! dataDirectoryListing[ REMOTE ].empty(),
                          ! htDirectoryListing[ REMOTE ].empty(),
                          ! zfDirectoryListing[ REMOTE ].empty(),
                          ! wwindDirectoryListing[ REMOTE ].empty() ) ) );
  return result;
}



