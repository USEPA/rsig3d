
/******************************************************************************
PURPOSE: HTTPConnection.C - For opening/closing/reading from an HTTP/S GET URL.
HISTORY: 2012-06-06 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, stderr, popen(), fgetc(), uncgetc(), snprintf()
#include <string.h> // For memset(), strstr().

#include <stdexcept> // For std::runtime_error.

#include <Assertions.h>     // For PRE*(), POST*(), CHECK*(), DEBUG(), AND*().
#include <DataUtilities.h>  // For reverse8ByteWordsIfLittleEndian(), isText().
#include <HTTPConnection.h> // For public interface.

//============================= PUBLIC FUNCTIONS ==============================

namespace emvl {

char HTTPConnection::HTTPGetProgram[ 1024 ] = ""; // E.g., "/usr/bin/curl ..."
char HTTPConnection::decompressProgram[ 1024 ] = ""; // E.g., "/usr/bin/gzip -d"



/******************************************************************************
PURPOSE: initialize - Initialize with program used to send HTTP queries.
INPUTS:  const char* const HTTPGetUtility  Pathed program command to use to get
                                           HTTP/HTTPS queries, for example,
         "/usr/bin/curl -k --silent --retry 0 -L --tcp-nodelay --max-time "
                                           Note that the HTTPGetUtility will
                                           have timeout (seconds) appended
                                           after it when used in the constructor
                                           below.
         const char* const  decompressUtility  Pathed program command to use to
                                               decompress a byte stream.
                                               Optional.
******************************************************************************/

void HTTPConnection::initialize( const char* const HTTPGetUtility,
                                 const char* const decompressUtility ) {
  PRE03( HTTPGetUtility, *HTTPGetUtility,
         IMPLIES( decompressUtility, *decompressUtility ) );

  memset( HTTPGetProgram, 0, sizeof HTTPGetProgram );
  strncpy( HTTPGetProgram, HTTPGetUtility,
           sizeof HTTPGetProgram / sizeof *HTTPGetProgram - 1 );
  HTTPGetProgram[ sizeof HTTPGetProgram / sizeof *HTTPGetProgram - 1 ] = '\0';

  setDecompressProgram( decompressUtility );

  POST0( isInitialized() );
}



/******************************************************************************
PURPOSE: setDecompressProgram - Initialize with program used to decompress a
                                byte stream.
INPUTS:  const char* const decompressUtility  0 or pathed program command to
                                              decompress a bytes stream.
******************************************************************************/

void HTTPConnection::setDecompressProgram( const char* decompressUtility ) {
  PRE0( IMPLIES( decompressUtility, *decompressUtility ) );

  memset( decompressProgram, 0, sizeof decompressProgram );

  if ( decompressUtility ) {
    strncpy( decompressProgram, decompressUtility,
             sizeof decompressProgram / sizeof *decompressProgram - 1 );
    decompressProgram[ sizeof  decompressProgram /
                       sizeof *decompressProgram - 1 ] = '\0';
  }

  POST0( IMPLIES_ELSE( decompressUtility,
                       ! strcmp( HTTPConnection::getDecompressProgram(),
                                 decompressUtility ),
                       HTTPConnection::getDecompressProgram()[ 0 ] == '\0' ) );
}



/******************************************************************************
PURPOSE: HTTPConnection - Construct an HTTPConnection.
INPUTS:  const char* url           Full URL of HTTP query.
         const int timeoutSeconds  Maximum number of seconds to wait to receive
                                   data before giving up in failure or
                                   0 to wait indefinitely. Default is 0.
         const bool useDecompression  Use decompression program on stream?
                                      Default is false.
                                      If true then decompressProgram
                                      must be set already using either
                                      initialize() or setDecompressProgram().
******************************************************************************/

HTTPConnection::HTTPConnection( const char* url,
                                const int timeoutSeconds,
                                const bool useDecompression ) {
  PRE06( isInitialized(), url, *url,
         OR2( strstr( url, "http://" ) == url,
              strstr( url, "https://" ) == url ),
         timeoutSeconds >= 0,
         IMPLIES( useDecompression, *decompressProgram ) );

#if defined( _WIN32 )
  const char* const mode = "rb"; // DOS popen needs binary mode.
  const char quote = '"';        // DOS shell needs double quotes around url.
#else
  const char* const mode = "r";
  const char quote = '\'';
#endif
  const char* const pipeOption = useDecompression ? " | " : "";
  const char* const decompressOption = useDecompression ? decompressProgram :"";
  char command[ 2048 ] = "";
  memset( command, 0, sizeof command );
  snprintf( command, sizeof command / sizeof *command - 30, "%s %d %c%s%c%s%s",
            HTTPGetProgram, timeoutSeconds, quote, url, quote,
            pipeOption, decompressOption );
  DEBUG( fprintf( stderr, "HTTPConnection command = %s\n", command ); )
  stream = popen( command, mode );

  if ( ! stream ) {
    throw std::runtime_error( "Failed to make HTTP/GET connection." ); //x
  }

  POST( true );
}



/******************************************************************************
PURPOSE: ~HTTPConnection - Destruct a HTTPConnection.
******************************************************************************/

HTTPConnection::~HTTPConnection() {
  PRE( true );

  if ( stream ) {
    pclose( stream );
    stream = 0;
  }

  POST0( stream == 0 );
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true or else there is a bug.
******************************************************************************/

bool HTTPConnection::invariant() const {
  const bool result = AND3( isInitialized(), *HTTPGetProgram, stream );
  return result;
}



/******************************************************************************
PURPOSE: read - Read a line of text from a connection.
INPUTS:  const size_t size  Size of line.
OUTPUTS: char line[ size ]  Line of text read.
RETURNS: bool true if a line was read, else false.
******************************************************************************/

bool HTTPConnection::read( const size_t size, char line[] ) {
  PRE2( size, line );
  bool result = false;
  size_t bytesRead = 0;
  memset( line, 0, size * sizeof *line );

  do {
    fgets( line + bytesRead, size - bytesRead, stream );
    // DEBUG( fprintf( stderr, "line = '%s'\n", line ); )
    bytesRead = strlen( line );
    CHECK( bytesRead < size );
  } while ( AND3( ! feof( stream ),
                  bytesRead < size - 1,
                  IMPLIES( bytesRead, line[ bytesRead - 1 ] != '\n' ) ) );

  if ( bytesRead > 0 ) {

    // Replace '\r' (DOS control-M) character with ' ':

    char* const c = strchr( line, '\r' );

    if ( c ) {
      *c = ' ';
    }

    result = isText( line ); // Check that line has only ASCII text characters.
  }

  POST0( result == AND3( *line != '\0', ! strchr( line, '\r' ), isText( line)));
  return result;
}



/******************************************************************************
PURPOSE: read - Read an array of big-endian binary words from a connection.
INPUTS:  const size_t count     Number of words to read.
         const size_t wordSize  Size, in bytes, per word.
OUTPUTS: void* array            Data words read from stream.
RETURNS: bool true if the expected words of data were read, else false.
NOTES:   if wordSize is 2, 4 or 8 then word bytes will have been rotated on a
         little-endian platform.
******************************************************************************/

bool HTTPConnection::read( const size_t count, const size_t wordSize,
                           void* array ) {
  PRE3( count, IN5( wordSize, 1, 2, 4, 8 ), array );
  bool result = false;
  char* const data = (char* const) array;
  const size_t totalBytesToRead = count * wordSize;
  size_t totalBytesRead = 0;
  memset( data, 0, totalBytesToRead );
  DEBUG( fprintf( stderr, "totalBytesToRead = %" LU "\n", totalBytesToRead);)

  do {
    const size_t bytesRemaining = totalBytesToRead - totalBytesRead;
    const size_t bytesRead =
      fread( data + totalBytesRead, 1, bytesRemaining, stream );
    totalBytesRead += bytesRead;
    DEBUG( fprintf( stderr, "bytesRead = %" LU "\n", bytesRead ); )
    CHECK( totalBytesRead <= totalBytesToRead );
  } while ( AND2( ! feof( stream ), totalBytesRead < totalBytesToRead ) );

  result = totalBytesRead == totalBytesToRead;

  if ( wordSize == 2 ) {
    reverse2ByteWordsIfLittleEndian( array, count );
  } else if ( wordSize == 4 ) {
    reverse4ByteWordsIfLittleEndian( array, count );
  } else if ( wordSize == 8 ) {
    reverse8ByteWordsIfLittleEndian( array, count );
  }

  POST( true );
  return result;
}



} // Namespace emvl.

