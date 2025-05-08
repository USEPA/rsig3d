/*
test_kml.c - Test KML routines.
gcc -Wall -g -o test_kml -I../../../../include -L../../../../lib/Darwin -lUtilities.debug.a
*/

#include <stdio.h> /* For FILE, fscanf(). */
#include <stdlib.h> /* For malloc(), free(). */
#include <string.h> /* For memset(). */

#include <KMLFile.h> /* For writeStartKML(), writeGridToKML(), writeEndKML(). */

static int readHeader( FILE* file, int* yyyymmdd, int* timesteps, int* rows,
                       int* columns, Name name, Units units ) {

  UTCTimestamp timestamp = "";
  int variables = 0;
  int layers = 0;
  int result =
    fscanf( file,
            "%[^\n]\n"
            "%[^\n]\n"
            "%[^\n]\n"
            "%25s\n"
            "%[^\n]\n"
            "%d %d %d %d %d\n"
            "%[^\n]\n"
            "%[^\n]\n"
            "%[^\n]\n"
            "%*s %*s %*s %16s\n"
            "%[^\n]\n"
            "%*s %*s %*s %16s\n"
            "%[^\n]\n"
            "%[^\n]\n"
            "%[^\n]\n"
            "%[^\n]\n"
            "%[^\n]\n",
            timestamp,
            timesteps, &variables, &layers, rows, columns,
            name,
            units ) == 8;

  if ( result ) {
    int yyyy = 0,
    int mm = 0;
    int dd = 0;
    sscanf( UTCTimestamp, "%d-%d-%d", &yyyy, &mm, &dd );
    *yyyymmdd = yyyy * 10000 + mm * 100 + dd;
    result =
      isValidYearMonthDay( *yyyymmdd ) &&
      *timesteps == 1 && variables == 4 && layers == 1 && *rows > 0 &&
      *columns > 0 && *name && *units;
  }

  return result;
}


static int readData( FILE* file, int timesteps, int variables, int layers,
                     int rows, int columns ) {
  const size_t bytes =
    timesteps * variables * layers * rows * columns * sizeof (float);
  float* result = (float*) malloc( bytes );

  if ( result ) {

    if ( fread( result, bytes, 1, file ) != 1 ) {
      free( result ), result = 0;
    }
  }

  return result;
}


int main( void ) {
  const char* const inputFileName = "/Users/plessel/cmaq_aqmeii_no.bin";
  const LongName source = "CMAQ-12km-ExtCONUS";
  int ok = 0;
  FILE* inputFile = fopen( inputFile, "rb" );

  if ( inputFile ) {

    if ( readHeader( inputFile, &yyyymmdd, &timesteps, &rows, &columns,
                     name, units ) ) {

      float* data = readData( inputFile, 1, 4, 1, rows, columns );

      if ( data ) {
        free( data ), data = 0;
      }
    }

    fclose( inputFile ), inputFile = 0;
  }

  return ! ok;
}

