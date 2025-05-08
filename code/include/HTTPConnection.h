
#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

/******************************************************************************
PURPOSE: HTTPConenction.h - For opening/closing/reading from an HTTP/S GET URL.
NOTES:   Example usage

#include <stdio.h>
#include <string.h>

#include <HTTPConnection.h>

int main( void ) {

  try {
    const char* const command =
      "/usr/bin/curl -k --silent --retry 0 -L --tcp-nodelay --max-time";
    emvl::HTTPConnection::initialize( command );
    const char* const url =
      "https://ofmpub.epa.gov/rsig/rsigserver?"
      "SERVICE=wcs&VERSION=1.0.0&REQUEST=GetCoverage&"
      "COVERAGE=uvnet.irradiance&"
      "TIME=1996-01-01T00:00:00Z/1996-01-02T23:59:59Z&BBOX=-118,33,-117,35&"
      "FORMAT=ascii";
    const int timeoutSeconds = 300;
    emvl::HTTPConnection connection( url, timeoutSeconds );
    enum { LINE_SIZE = 256 };
    char line[ LINE_SIZE ] = "";

    while ( connection.read( LINE_SIZE, line ) ) {
      printf( "%s", line );
    }

  } catch ( ... ) {
    fprintf( stderr, "\nFailed!\n" );
  }

  return 0;
}

HISTORY: 2012-06-06 plessel.todd@epa.gov
STATUS:  unreviewed tested

******************************************************************************/

//================================= INCLUDES ==================================

#include <stdio.h> // For FILE.

//================================== TYPES ====================================

namespace emvl {

class HTTPConnection {
private:
  static char HTTPGetProgram[ 1024 ]; // Set with initialize().
  static char decompressProgram[ 1024 ]; // Set with initialize().
  FILE* stream;
  HTTPConnection( const HTTPConnection& ); // No copying allowed.
  void operator=( const HTTPConnection& ); // No assignment allowed.
public:
  static void initialize( const char* HTTPGetUtility,
                          const char* decompressUtility = 0 );
  static const char* getHTTPGetProgram() { return HTTPGetProgram; }
  static const char* getDecompressProgram() { return decompressProgram; }
  static void setDecompressProgram( const char* decompressUtility );
  static bool isInitialized() { return *HTTPGetProgram != '\0'; }
  HTTPConnection( const char* url,
                  const int timeoutSeconds = 0,
                  const bool useDecompression = false ); //x
  ~HTTPConnection();
  bool invariant() const;
  FILE* getStream() const { return stream; };
  bool read( const size_t size, char line[] ); // Read a line of ASCII text.
  bool read( const size_t count, const size_t wordSize, void* array ); //binary
};

} // Namespace emvl.

#endif /* HTTPCONNECTION_H */


