
/******************************************************************************
PURPOSE: Utilities.c - Utility routines.
HISTORY: 2009/05/26 plessel.todd@epa.gov Created.
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h>     /* For FILE, fopen(), fread(), snprintf(). fprintf(). */
#include <string.h>    /* For strcmp(), strncpy(), strncat(). */
#include <stdlib.h>    /* For malloc(), free(), strtod(). */
#include <ctype.h>     /* For isspace(), isprint(). */
#include <math.h>      /* For sqrt(). */
#include <limits.h>    /* For ULONG_MAX. */
#include <unistd.h>    /* For unlink(). */
#include <time.h>      /* For struct tm, struct timespec, localtime(). */
#include <dirent.h>    /* For DIR, struct dirent, opendir(), readdir(), etc.*/
#include <sys/types.h> /* For struct stat. */
#include <sys/stat.h>  /* For stat(). */

#include <Assertions.h> /* For PRE*(), POST*(), CHECK*(), AND*(). */
#include <Utilities.h>  /* For public interface. */

#ifdef MIN
#undef MIN
#endif
#define MIN( a, b ) ((a) < (b) ? (a) : (b))


/*=========================== FORWARD DECLARATIONS ==========================*/

static void removeFile0( const char* const name, void* unused ) {
  PRE02( name, *name );
  unlink( name );
}

static double radians( double theDegrees );


/*============================ PUBLIC FUNCTIONS =============================*/



/******************************************************************************
PURPOSE: newMemory - Allocate memory (called by NEW() macro).
INPUTS:  size_t count          Number of items.
         size_t bytesEach      Number of bytes per item.
RETURNS: void* block of memory of size count * sizeEach or 0 if failed and
         a failure message is printed to stderr.
******************************************************************************/

void* newMemory( size_t count, size_t bytesEach  ) {
  PRE02( bytesEach > 0, count > 0 );
  void* result = 0;

  if ( bytesEach > ULONG_MAX / count || count > ULONG_MAX / bytesEach ) {
    fprintf( stderr, "\a\nCannot allocate %lu x %lu bytes ", count, bytesEach);
  } else {
    const size_t bytes = bytesEach * count;
    result = malloc( bytes );

    if ( ! result ) {
      fprintf(stderr, "\a\nCannot allocate %lu x %lu bytes ", count,bytesEach);
      perror( "because" );
    } else {
      memset( result, 0, bytes );
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: isNan - Is value Not a Number?
INPUTS:  const double value  Value to check.
RETURNS: int 1 if NaN, else 0.
******************************************************************************/

int isNan( const double value ) {
  const double copy = value;
  const int result = ( copy != value );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isNanFreeF - Does array of floats contain no NaNs?
INPUTS:  const float data[ count ]  Values to check.
         const size_t count         Number of values in array.
RETURNS: int 1 if NaN, else 0.
******************************************************************************/

int isNanFreeF( const float data[], const size_t count ) {
  int result = 1;
  size_t index = 0;

  for ( index = 0; AND2( result, index < count ); ++index ) {
    result = ! isNan( data[ index ] );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isNanFree - Does array of doubles contain no NaNs?
INPUTS:  const double data[ count ]  Values to check.
         const size_t count          Number of values in array.
RETURNS: int 1 if NaN, else 0.
******************************************************************************/

int isNanFree( const double data[], const size_t count ) {
  int result = 1;
  size_t index = 0;

  for ( index = 0; AND2( result, index < count ); ++index ) {
    result = ! isNan( data[ index ] );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidLongitudeLatitude - Is longitude, latitude a valid point?
INPUTS:  const double longitude  Longitude to check.
         const double latitude   Latitude  to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidLongitudeLatitude( const double longitude,
                              const double latitude ) {
  const int result =
    AND2( IN_RANGE( longitude, -180.0, 180.0 ),
          IN_RANGE( latitude,   -90.0,  90.0 ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidBounds - Is bounds a valid lon-lat rectangle?
INPUTS:  const Bounds bounds  Bounds to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidBounds( const Bounds bounds ) {
  const int result =
    AND5( bounds,
          IN_RANGE( bounds[ LONGITUDE ][ MINIMUM ], -180.0, 180.0 ),
          IN_RANGE( bounds[ LONGITUDE ][ MAXIMUM ],
                    bounds[ LONGITUDE ][ MINIMUM ],  180.0 ),
          IN_RANGE( bounds[ LATITUDE  ][ MINIMUM ],  -90.0, 90.0 ),
          IN_RANGE( bounds[ LATITUDE  ][ MAXIMUM ],
                    bounds[ LATITUDE  ][ MINIMUM ],   90.0 ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: boundsOverlap - Do bounds overlap?
INPUTS:  const Bounds a  a[ LONGITUDE, LATITUDE ][ MINIMUM, MAXIMUM ].
         const Bounds b  b[ LONGITUDE, LATITUDE ][ MINIMUM, MAXIMUM ].
RETURNS: int 1 if they overlap, else 0.
******************************************************************************/

int boundsOverlap( const Bounds a, const Bounds b ) {
  PRE02( isValidBounds( a ), isValidBounds( b ) );
  const int outside =
    OR4( a[ LATITUDE  ][ MINIMUM ] > b[ LATITUDE  ][ MAXIMUM ],
         a[ LATITUDE  ][ MAXIMUM ] < b[ LATITUDE  ][ MINIMUM ],
         a[ LONGITUDE ][ MINIMUM ] > b[ LONGITUDE ][ MAXIMUM ],
         a[ LONGITUDE ][ MAXIMUM ] < b[ LONGITUDE ][ MINIMUM ] );
  const int result = ! outside;
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: degreesPerPixel - Degrees lon-lat per screen pixel.
INPUTS:  const Bounds bounds  Bounds of lon-lat window.
         int width            Width  of window in screen pixels.
         int height           Height of window in screen pixels.
RETURNS double degrees of lon-lat per screen pixel (maximum of lon/lat).
******************************************************************************/

double degreesPerPixel( const Bounds bounds, int width, int height ) {
  PRE03( isValidBounds( bounds ), width > 0, height > 0);
  const double longitudeRange =
    bounds[ LONGITUDE ][ MAXIMUM ] - bounds[ LONGITUDE ][ MINIMUM ];
  const double latitudeRange =
    bounds[ LATITUDE ][ MAXIMUM ] - bounds[ LATITUDE ][ MINIMUM ];
  const double longitudeDegreesPerPixel = longitudeRange / width;
  const double latitudeDegreesPerPixel  = latitudeRange / height;
  const double result =
    longitudeDegreesPerPixel > latitudeDegreesPerPixel ?
  longitudeDegreesPerPixel : latitudeDegreesPerPixel;
  DEBUG( fprintf( stderr, "degreesPerPixel = %lf\n", result ); )
  POST0( result > 0.0 );
/*POST0( IN_RANGE( result, MINIMUM_DEGREES_PER_PIXEL, 360.0 ) ); */
  return result;
}



/******************************************************************************
PURPOSE: widthInMeters - Width of lon-lat window in meters.
INPUTS:  const Bounds bounds  Lon-lat bounds of window.
RETURNS: double width of (center line of) window in meters.
******************************************************************************/

double widthInMeters( const Bounds bounds ) {
  PRE0( isValidBounds( bounds ) );
  const double longitudeMinimum = bounds[ LONGITUDE ][ MINIMUM ];
  const double longitudeMaximum = bounds[ LONGITUDE ][ MAXIMUM ];
  const double latitudeMinimum  = bounds[ LATITUDE  ][ MINIMUM ];
  const double latitudeMaximum  = bounds[ LATITUDE  ][ MAXIMUM ];
  const double latitudeMean     = 0.5 * ( latitudeMinimum + latitudeMaximum );
  const double longitudeRange   = longitudeMaximum - longitudeMinimum;
  const double meanEarthRadius = 6371009.0; /* meters. */
  const double degreesPerMeter =
    1.0 / ( 2.0 * M_PI * meanEarthRadius / 360.0 * cos(radians(latitudeMean)));
  const double result = longitudeRange / degreesPerMeter;
  POST0( result > 0.0 );
  return result;
}



/******************************************************************************
PURPOSE: computeArrowVectorCoordinates - Compute coordinates of arrow vector.
INPUTS:  const double longitude           Longitude of base of arrow.
         const double latitude            Latitude of base of arrow.
         const double x                   X-component of arrow.
         const double y                   Y-component of arrow.
         const double degreesPerPixel     Degrees longitude per viewport pixel.
         const double pixelsPerUnitLength Viewport pixels per unit vector.
OUTPUTS: double* const point0X            X-coordinate of arrow base vertex.
         double* const point0Y            Y-coordinate of arrow base vertex.
         double* const point1X            X-coordinate of arrow tip vertex.
         double* const point1Y            Y-coordinate of arrow tip vertex.
         double* const point2X            X-coordinate of arrow head top vertex
         double* const point2Y            Y-coordinate of arrow head top vertex
         double* const point3X            X-coordinate of arrow head bot vertex
         double* const point3Y            Y-coordinate of arrow head bot vertex
NOTES:
                     v2
                      \
             v0--------v1
                      /
                     v3
******************************************************************************/

void computeArrowVectorCoordinates( const double longitude,
                                    const double latitude,
                                    const double x,
                                    const double y,
                                    const double degreesPerPixel,
                                    const double pixelsPerUnitLength,
                                    double* const point0X,
                                    double* const point0Y,
                                    double* const point1X,
                                    double* const point1Y,
                                    double* const point2X,
                                    double* const point2Y,
                                    double* const point3X,
                                    double* const point3Y ) {

  PRE013( isValidLongitudeLatitude( longitude, latitude ),
          ! isNan( x ),
          ! isNan( y ),
          IN_RANGE( degreesPerPixel, 1e-8, 1.0 ),
          IN_RANGE( pixelsPerUnitLength, 1.0, 100.0 ),
          point0X, point0Y, point1X, point1Y, point2X, point2Y,
          point3X, point3Y );

  const double arrowX = 0.8; /* Normalized X-coord of barb of unit vector. */
  const double arrowY = 0.2; /* Normalized Y-coord of barb of unit vector. */
  const double angle = atan2( y, x );
  const double r = hypot( x, y );
  const double scale = r * pixelsPerUnitLength * degreesPerPixel;
  const double scaledCosineAngle = scale * cos( angle );
  const double scaledSineAngle   = scale * sin( angle );
  const double arrowXScaledCosineAngle = arrowX * scaledCosineAngle;
  const double arrowXScaledSineAngle   = arrowX * scaledSineAngle;
  const double arrowYScaledCosineAngle = arrowY * scaledCosineAngle;
  const double arrowYScaledSineAngle   = arrowY * scaledSineAngle;
  const double arrowXScaledCosineAngleMinusArrowYScaledSineAngle =
    arrowXScaledCosineAngle - arrowYScaledSineAngle;
  const double arrowYScaledCosineAnglePlusArrowXScaledSineAngle =
    arrowYScaledCosineAngle + arrowXScaledSineAngle;
  const double arrowXScaledCosineAnglePlusArrowYScaledSineAngle =
    arrowXScaledCosineAngle + arrowYScaledSineAngle;
  *point0X = longitude;
  *point0Y = latitude;
  *point1X = longitude + scaledCosineAngle;
  *point1Y = latitude  + scaledSineAngle;
  *point2X = longitude + arrowXScaledCosineAngleMinusArrowYScaledSineAngle;
  *point2Y = latitude  + arrowYScaledCosineAnglePlusArrowXScaledSineAngle;
  *point3X = longitude + arrowXScaledCosineAnglePlusArrowYScaledSineAngle;
  *point3Y = latitude  - arrowYScaledCosineAngle + arrowXScaledSineAngle;

  POST05( *point0X == longitude, *point0Y == latitude,
          isValidLongitudeLatitude( *point1X, *point1Y ),
          isValidLongitudeLatitude( *point2X, *point2Y ),
          isValidLongitudeLatitude( *point3X, *point3Y ) );
}



/* Alternate version with fixed head size */
void computeArrowVectorCoordinates2( const int headLengthPixels,
                                     const double longitude,
                                     const double latitude,
                                     const double x,
                                     const double y,
                                     const double degreesPerPixel,
                                     const double pixelsPerUnitLength,
                                     double* const point0X,
                                     double* const point0Y,
                                     double* const point1X,
                                     double* const point1Y,
                                     double* const point2X,
                                     double* const point2Y,
                                     double* const point3X,
                                     double* const point3Y ) {

  PRE013( isValidLongitudeLatitude( longitude, latitude ),
          ! isNan( x ),
          ! isNan( y ),
          IN_RANGE( degreesPerPixel, 1e-8, 1.0 ),
          IN_RANGE( pixelsPerUnitLength, 1.0, 100.0 ),
          point0X, point0Y, point1X, point1Y, point2X, point2Y,
          point3X, point3Y );

  const double theta = atan2( y, x );
  const double r = hypot( x, y );
  const double scale = r * pixelsPerUnitLength * degreesPerPixel;
  const double scaledCosineAngle = scale * cos( theta );
  const double scaledSineAngle   = scale * sin( theta );

  const double headAngle                = 20.0; // degrees
  const double deg2rad                  = 3.14 / 180.0;
  const double phi                      = headAngle * deg2rad;
  const double headLength               = headLengthPixels * degreesPerPixel;
  const double thetaMinusPhi            = theta - phi;  
  const double ninetyMinusThetaMinusPhi = (90*deg2rad) - theta - phi;  

  *point0X = longitude;
  *point0Y = latitude;
  *point1X = longitude + scaledCosineAngle;
  *point1Y = latitude  + scaledSineAngle;
  *point2X = *point1X - headLength * cos(thetaMinusPhi);
  *point2Y = *point1Y - headLength * sin(thetaMinusPhi);
  *point3X = *point1X - headLength * sin(ninetyMinusThetaMinusPhi);
  *point3Y = *point1Y - headLength * cos(ninetyMinusThetaMinusPhi);
  
  POST05( *point0X == longitude, *point0Y == latitude,
          isValidLongitudeLatitude( *point1X, *point1Y ),
          isValidLongitudeLatitude( *point2X, *point2Y ),
          isValidLongitudeLatitude( *point3X, *point3Y ) );
}






/******************************************************************************
PURPOSE: isValidColor - Is Color in the unit RGB cube?
INPUTS:  Color color  Color to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidColor( Color color ) {
  const int result =
    AND3( IN_RANGE( color.r, 0.0, 1.0 ),
          IN_RANGE( color.g, 0.0, 1.0 ),
          IN_RANGE( color.b, 0.0, 1.0 ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: makeRGB - Initialize and return an RGB.
INPUTS:  unsigned char r  Red component.
         unsigned char g  Green component.
         unsigned char b  Blue component.
         const char* s    Name component.
RETURNS: RGB initialized to the given components.
******************************************************************************/

RGB makeRGB(unsigned char r, unsigned char g, unsigned char b, const char* s) {
  const RGB result = { r, g, b, s };
  return result;
}



/******************************************************************************
PURPOSE: dataColor - Map value in range [minimum, maximum] to
         RGB color cube range: blue-cyan-green-yellow-red.
INPUTS:  double value    Data value to map.
         double minimum  Minimum value for data.
         double maximum  Maximum value for data.
RETURNS: Color mapped value.
******************************************************************************/

Color dataColor( double value, double minimum, double maximum ) {
  PRE03( ! isNan( value ), ! isNan( minimum ), ! isNan( maximum ) );
  Color result = { 0.0, 0.0, 0.0 };

  if ( AND2( value != -9999.0, value != -9.999e36 ) ) {
    const double range = maximum - minimum;
    double t = range <= 0.0 ? 0.0 : ( value - minimum ) / range;

    if ( t < 0.0 || isNan( t ) ) {
      t = 0.0;
    } else if ( t > 1.0 ) {
      t = 1.0;
    }

    if ( t <= 0.25 ) {
      result.g = t * 4.0;
      result.b = 1.0;
    } else if ( t <= 0.5 ) {
      t = ( t - 0.25 ) * 4.0;
      result.g = 1.0;
      result.b = 1.0 - t;
    } else if ( t <= 0.75 ) {
      t = ( t - 0.5 ) * 4.0;
      result.g = 1.0;
      result.r = t;
    } else {
      t = ( t - 0.75 ) * 4.0;
      result.r = 1.0;
      result.g = 1.0 - t;
    }
  }

  POST0( isValidColor( result ) );
  return result;
}



/******************************************************************************
PURPOSE: lexicographicTextColor - Map text to Color based on first letter
         compared to 'a'..'z' or '0'..'9'.
INPUTS:  const char* text  Text to map.
RETURNS: Color mapped value.
******************************************************************************/

Color lexicographicTextColor( const char* text ) {
  PRE0( text );
  const char c = tolower( *text );
  const int isNumber = isdigit( c );
  const double t = isNumber ? c - '0' : c - 'a';
  const Color result = dataColor( t, 0.0, isNumber ? 9.0 : 26.0 );
  POST0( isValidColor( result ) );
  return result;
}



/******************************************************************************
PURPOSE: wordCount - Count number of whitespace-separated printable words
         in a string.
INPUTS:  const char* string  String to count.
RETURNS: int number of words in string.
******************************************************************************/

int wordCount( const char* string ) {
  PRE0( string );
  const char* s = string;
  int result = 0;

  while ( *s ) {

    while ( isspace( *s ) ) {
      ++s;
    }

    result += isprint( *s ) != 0;

    while ( *s && ! isspace( *s ) ) {
      ++s;
    }
  }

  POST0( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: lowercase - Convert a string to lowercase.
INPUTS:  char string[]  String to convert.
OUTPUTS: char string[]  Lowercase string.
******************************************************************************/

void lowercase( char string[] ) {
  PRE0( string );
  char* s = string;

  for ( s = string; *s; ++s ) {
    *s = tolower( *s );
  }
}



/******************************************************************************
PURPOSE: matchesWord - Does word appear in space-delimited words?
INPUTS:  const char* word   String to search for.
         const char* words  String of space-delimited words to search.
RETURNS: int 1 if found, else 0.
******************************************************************************/

int matchesWord( const char* word, const char* const words ) {
  PRE06( word, *word, ! strchr( word, ' ' ),
         words, *words, strchr( words, ' ' ) );
  int result = 0;
  const char* nextWord = words;
  const size_t length = strlen( word );

  do {
    nextWord = strstr( nextWord, word );

    if ( nextWord ) {
      const int atStart = nextWord == words;
      const int nothingBefore = OR2( atStart, *( nextWord - 1 ) == ' ' );

      if ( nothingBefore ) {
        const char after = *( nextWord + length );
        result = IN3( after, '\0', ' ' );
      }

      ++nextWord;
    }
  } while ( AND2( nextWord, ! result ) );

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: indexOfString - Index of string in strings[] or -1 if not present.
INPUTS:  const char* string           String to search for.
         const char* const strings[]  Strings to search.
         int count                    Size of strings[].
RETURNS: int index of string in strings[], else -1 if not present.
******************************************************************************/

int indexOfString(const char* string, const char* const strings[], int count) {

  PRE08( string, *string, strings, strings[ 0 ], *strings[ 0 ],
         count > 0, strings[ count - 1 ], *strings[ count - 1 ] );

  int result = -1;
  int index = 0;

  do {

    if ( strcmp( string, strings[ index ] ) == 0 ) {
      result = index;
      index = count;
    }

    ++index;
  } while ( index < count );

  POST0( OR2( result == -1,
              AND2( result >= 0,
                    strcmp( string, strings[ result ] ) == 0 ) ) );

  return result;
}



/******************************************************************************
PURPOSE: copyPaddedString - Copy length-characters of input to output, then
         pad end with spaces
INPUTS:  const char* string   String  to check, e.g., "WN_85_%"".
         const char* pattern  Pattern to match, e.g., "WN_%02d_%%".
RETURNS: int 1 if matched, else 0.
******************************************************************************/

void copyPaddedString( char* output, const char* input,
                       const int length, const int uppercase ) {
  PRE05( output, input, *input, length > 0, IS_BOOL( uppercase ) );
  int index = 0;

  for ( index = 0; AND2( index < length, input[ index ] ); ++index ) {
    output[ index ] = uppercase ? toupper( input[ index ] ) : input[ index ];
  }

  for ( ; index < length; ++index ) {
    output[ index ] = ' ';
  }

  output[ length ] = '\0';
  POST0( (int) ( strlen( output ) ) == length );
}



/******************************************************************************
PURPOSE: parseValues - Parse, store and return number of parsed values from
         comma/space delimited string.
INPUTS:  const char* valuesString  Space or comma-delimited string to parse.
         const size_t size  Size of values array.
RETURNS: size_t number of parsed values.
******************************************************************************/

size_t parseValues( const char* valuesString,
                    const size_t size,
                    double values[] ) {
  PRE03( valuesString, size, values );
  size_t result = 0;
  int done = 0;

  do {
    char* end = 0;
    const double value = strtod( valuesString, &end );
    done = end == valuesString || result == size;

    if ( ! done ) {
      values[ result ] = value;
      ++result;
      valuesString = end;

      while ( *valuesString == ' ' || *valuesString == ',' ) {
        ++valuesString;
      }
    }

  } while ( ! done );

  POST0( result < size );
  return result;
}



/******************************************************************************
PURPOSE: copyStream - Copy bytes from input stream to output stream.
INPUTS:  FILE* input   Stream to read from.
         FILE* output  Stream to write to.
RETURNS: int 1 if successful, else 0 and a failure message is written to stderr
******************************************************************************/

int copyStream( FILE* input, FILE* output ) {
  PRE02( input, output );
  enum { BUFFER_SIZE = 256 * 1024 };
  char buffer[ BUFFER_SIZE ] = "";
  int done = 0;
  int result = 1;

  do {
    const size_t bytes = fread( buffer, 1, BUFFER_SIZE, input );
    result = 1;

    if ( bytes ) {
      result = fwrite( buffer, 1, bytes, output ) == bytes;
      done = ! result;
    } else {
      done = 1;
    }

  } while ( ! done );

  if ( ! result ) {
    fprintf( stderr, "\nFailed to copy stream.\n" );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: copyFile - Copy a file.
INPUTS:  const char* inputFileName   Pathed name of file to copy.
         const char* outputFileName  Created file.
RETURNS: int 1 if successful, else 0.
******************************************************************************/

int copyFile( const char* inputFileName, const char* outputFileName ) {
  PRE03( inputFileName, outputFileName, *outputFileName );
  int result = 1;

  if ( strcmp( inputFileName, outputFileName ) ) {
    FILE* inputFile = fopen( inputFileName, "rb" );
    FILE* outputFile = inputFile ? fopen( outputFileName, "wb" ) : 0;
    result = 0;

    if ( outputFile ) {
      enum { BUFFER_SIZE = 256 * 1024 };
      char buffer[ BUFFER_SIZE ] = "";
      int done = 0;

      do {
        const size_t bytes = fread( buffer, 1, BUFFER_SIZE, inputFile );
        result = 1;

        if ( bytes ) {
          result = fwrite( buffer, 1, bytes, outputFile ) == bytes;
          done = ! result;
        } else {
          done = 1;
        }

      } while ( ! done );
    }

    if ( inputFile ) {
      fclose( inputFile ), inputFile = 0;
    }

    if ( outputFile ) {
      fclose( outputFile ), outputFile = 0;
    }

    if ( ! result ) {
      fprintf( stderr, "\nFailed to copy file %s\n", inputFileName );
    }
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: copyFileBytes - Copy a file's bytes to another file.
INPUTS:  FILE* inputFile             File to copy.
         const char* outputFileName  Created file.
         size_t bytes                Bytes to read and write.
RETURNS: int 1 if successful, else 0.
******************************************************************************/

int copyFileBytes( FILE* inputFile, const char* outputFileName, size_t bytes) {

  PRE04( inputFile, outputFileName, *outputFileName, bytes > 0 );

  int result = 0;
  FILE* outputFile = fopen( outputFileName, "wb" );

  if ( outputFile ) {
    enum { BUFFER_SIZE = 256 * 1024 };
    char buffer[ BUFFER_SIZE ] = "";
    size_t bytesRemaining = bytes;
    DEBUG2(fprintf(stderr, "copyFileBytes(%s, %lu)\n", outputFileName, bytes);)

    do {
      const size_t bytesToRead = MIN( bytesRemaining, BUFFER_SIZE );
      const size_t bytesRead = fread( buffer, 1, bytesToRead, inputFile );

      if ( bytesRead ) {
        const size_t bytesWritten = fwrite( buffer, 1, bytesRead, outputFile );
        result = bytesWritten == bytesRead;
        bytesRemaining -= bytesWritten;
        DEBUG2( fprintf( stderr, "  bytesWritten = %lu\n", bytesWritten ); )
      } else {
        result = 0;
      }

    } while ( AND2( result, bytesRemaining ) );

    fclose( outputFile ), outputFile = 0;
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to copy %lu bytes to output file %s ",
             bytes, outputFileName );
    perror( "because" );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: streamBytesToFile - Copy all of a stream's bytes to a file.
INPUTS:  FILE* stream                Stream to read.
         const char* outputFileName  Created file.
RETURNS: int 1 if successful, else 0.
******************************************************************************/

int streamBytesToFile( FILE* stream, const char* outputFileName ) {
  PRE03( stream, outputFileName, *outputFileName );
  int result = 0;
  FILE* outputFile = fopen( outputFileName, "wb" );

  if ( outputFile ) {
    enum { BUFFER_SIZE = 256 * 1024 };
    char buffer[ BUFFER_SIZE ] = "";
    size_t bytesRead = 0;

    do {
      bytesRead = fread( buffer, 1, BUFFER_SIZE, stream );

      if ( bytesRead ) {
        const size_t bytesWritten = fwrite( buffer, 1, bytesRead, outputFile );
        result = bytesWritten == bytesRead;
      }

    } while ( AND2( bytesRead, result ) );

    fclose( outputFile ), outputFile = 0;
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to stream all bytes to output file %s ",
             outputFileName );
    perror( "because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: streamFile - Stream bytes of a file to stdout.
INPUTS:  const char* fileName  Name of file to stream.
RETURNS: int 1 if successful, else 0.
******************************************************************************/

int streamFile( const char* fileName ) {
  PRE02( fileName, *fileName );
  int result = 0;
  FILE* file = fopen( fileName, "rb" );

  if ( file ) {
    enum { BUFFER_SIZE = 256 * 1024 };
    char buffer[ BUFFER_SIZE ] = "";
    size_t bytesRemaining = fileSize( fileName );

    do {
      const size_t bytesToStreamNow = MIN( bytesRemaining, BUFFER_SIZE );
      result = fread( buffer, bytesToStreamNow, 1, file ) == 1;
      result = AND2( result,
                     fwrite( buffer, bytesToStreamNow, 1, stdout ) == 1 );
      bytesRemaining -= bytesToStreamNow;
    } while ( AND2( result, bytesRemaining ) );

    fclose( file ), file = 0;
  }

  DEBUG( fprintf( stderr, "streamFile() returning %d\n", result ); )
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: fileExists - Does the named file exist?
INPUTS:  const char* name  Name of file to examine.
RETURNS: int 1 if the file exists, else 0.
******************************************************************************/

int fileExists( const char* name ) {
  PRE02( name, *name );
  struct stat unused;
  const int result = stat( name, &unused ) != -1;
  return result;
}



/******************************************************************************
PURPOSE: fileSize - Determine size of named file.
INPUTS:  const char* name  Name of file to examine.
RETURNS: size_t Size, in bytes, of named file, else 0 if failed.
******************************************************************************/

size_t fileSize( const char* name ) {
  PRE02( name, *name );
  size_t result = 0;
  struct stat buf;

  if ( stat( name, &buf ) == -1 ) {
    fprintf( stderr, "\nFailed to determine size of file '%s'.\n", name );
  } else {
    result = buf.st_size;
  }

  return result;
}



/******************************************************************************
PURPOSE: readFile - Read named file into memory and return it as an allocated
         string.
INPUTS:  const char* name  Name of file to examine.
OUTPUTS: size_t* length    Length of string.
         size_t* lines     Number of lines (\n) in string.
RETURNS: char* string contents of file
         (with any '\r' characters converted to ' '),
         else 0 if failed and a message is printed to stderr.
******************************************************************************/

char* readFile( const char* name, size_t* length, size_t* lines ) {
  PRE03( name, length, lines );
  char* result = 0;
  *length = fileSize( name ) / sizeof (char);
  *lines = 0;

  if ( *length > 0 ) {
    result = (char*) malloc( sizeof (char) * ( *length + 1 ) );

    if ( ! result ) {
      fprintf( stderr, "\nFailed to allocate %lu bytes to read file %s.\n",
               *length + 1, name );
      *length = 0;
    } else {
      FILE* file = fopen( name, "rb" );

      if ( file ) {
        const size_t itemsRead =
          fread( result, *length * sizeof (char), 1, file );

        if ( itemsRead != 1 ) {
          fprintf( stderr, "\nFailed to read entire file '%s'.\n", name );
          free( result ), result = 0;
          *length = 0;
        } else {
          result[ *length ] = '\0'; /* Terminate string. */
          *lines = controlMToSpace( result );
        }

        fclose( file );
        file = 0;
      }
    }
  }

  POST0( IMPLIES_ELSE( result,
                       AND2( *length > 0, *lines > 0 ),
                       AND2( *length == 0, *lines == 0 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: controlMToSpace - Convert any '\r' characters to ' '.
INPUTS:  char* string  String to filter.
OUTPUTS: char* string  Filtered string.
RETURNS: size_t number of lines (\n) in string.
******************************************************************************/

size_t controlMToSpace( char* string ) {
  PRE0( string );
  size_t result = 0;
  char* s = 0;

  for ( s = string; *s; ++s ) {

    if ( *s == '\r' ) {
      *s = ' ';
    } else if ( *s == '\n' ) {
      ++result;
    }
  }

  POST0( ! strchr( string, '\r' ) );
  return result;
}



/******************************************************************************
PURPOSE: removeAllFiles - Remove all files in a directory.
INPUTS:  const char* name  Name of directory to remove files from.
******************************************************************************/

void removeAllFiles( const char* name ) {
  PRE02( name, *name );
  DIR* directory = opendir( name );

  if ( directory ) {
    struct dirent* entry = 0;

    do {
      entry = readdir( directory );

      if ( AND3( entry, entry->d_name, entry->d_name[ 0 ] != '.' ) ) {
        enum { LENGTH = 255 };
        char pathedName[ LENGTH + 1 ] = "";
        memset( pathedName, 0, sizeof pathedName );
        snprintf( pathedName, sizeof pathedName / sizeof *pathedName,
                  "%s/%s", name, entry->d_name );
        unlink( pathedName );
      }

    } while ( entry );

    closedir( directory ), directory = 0;
  }
}



/******************************************************************************
PURPOSE: removeMatchedFiles - Remove matched files in a directory.
INPUTS:  const char* const directoryName Name of directory to remove files from
         const char* const startsWith Optional matching file name start.
         const char* const endsWith   Optional matching file name end.
******************************************************************************/

void removeMatchedFiles( const char* const directoryName,
                         const char* const startsWith,
                         const char* const endsWith ) {

  PRE04( directoryName,
         *directoryName,
         IMPLIES( startsWith, *startsWith ),
         IMPLIES( endsWith, *endsWith ) );

  forEachFile( directoryName, startsWith, endsWith, removeFile0, 0 );
}



/******************************************************************************
PURPOSE: copyFiles - Copies matched files in a directory to another directory.
INPUTS:  const char* const fromDirectory  Name of directory to copy files from.
         const char* const toDirectory    Name of directory to copy files to.
         const char* const startsWith     Optional matching file name start.
         const char* const endsWith       Optional matching file name end.
******************************************************************************/

void copyFiles( const char* const fromDirectory,
                const char* const toDirectory,
                const char* const startsWith,
                const char* const endsWith ) {

  PRE07( fromDirectory, *fromDirectory, toDirectory, *toDirectory,
         strcmp( fromDirectory, toDirectory ),
         IMPLIES( startsWith, *startsWith ),
         IMPLIES( endsWith, *endsWith ) );

  DIR* directory = opendir( fromDirectory );

  if ( directory ) {
    struct dirent* entry = 0;

    do {
      entry = readdir( directory );

      if ( AND3( entry, entry->d_name, entry->d_name[ 0 ] != '.' ) ) {
        const char* const fileName = entry->d_name;
        int matched = 1;

        if ( startsWith ) {
          const char* const found = strstr( fileName, startsWith );
          matched = found == fileName;
        }

        if ( matched ) {

          if ( endsWith ) {
            const size_t endLength = strlen( endsWith );
            const size_t fileNameLength = strlen( fileName );

            if ( fileNameLength >= endLength ) {
              const char* const endOfFileName =
                fileName + fileNameLength - endLength;
              matched = strcmp( endOfFileName, endsWith ) == 0;
            }
          }
        }

        if ( matched ) {
          enum { LENGTH = 255 };
          char fromFileName[ LENGTH + 1 ] = "";
          char toFileName[ LENGTH + 1 ] = "";
          memset( fromFileName, 0, sizeof fromFileName );
          memset( toFileName, 0, sizeof toFileName );
          snprintf( fromFileName, sizeof fromFileName / sizeof *fromFileName,
                    "%s/%s", fromDirectory, fileName );
          snprintf( toFileName, sizeof toFileName / sizeof *toFileName,
                    "%s/%s", toDirectory, fileName );
          copyFile( fromFileName, toFileName );
        }
      }

    } while ( entry );

    closedir( directory ), directory = 0;
  }
}



/******************************************************************************
PURPOSE: forEachFile - Calls callback for each matched file in a directory.
INPUTS:  const char* const directory  Name of directory to check for files.
         const char* const startsWith     Optional matching file name start.
         const char* const endsWith       Optional matching file name end.
         ForEachFileCallback callback     Called with each matched file name.
         void* callbackData               Second argument to callback.
******************************************************************************/

void forEachFile( const char* const directory,
                  const char* const startsWith,
                  const char* const endsWith,
                  ForEachFileCallback callback,
                  void* callbackData ) {

  PRE05( directory,
         *directory,
         IMPLIES( startsWith, *startsWith ),
         IMPLIES( endsWith, *endsWith ),
         callback );

  DIR* dir = opendir( directory );

  if ( dir ) {
    struct dirent* entry = 0;

    do {
      entry = readdir( dir );

      if ( AND3( entry, entry->d_name, entry->d_name[ 0 ] != '.' ) ) {
        const char* const fileName = entry->d_name;
        int matched = 1;

        if ( startsWith ) {
          const char* const found = strstr( fileName, startsWith );
          matched = found == fileName;
        }

        if ( matched ) {

          if ( endsWith ) {
            const size_t endLength = strlen( endsWith );
            const size_t fileNameLength = strlen( fileName );

            if ( fileNameLength >= endLength ) {
              const char* const endOfFileName =
                fileName + fileNameLength - endLength;
              matched = strcmp( endOfFileName, endsWith ) == 0;
            }
          }
        }

        if ( matched ) {
          callback( fileName, callbackData );
        }
      }

    } while ( entry );

    closedir( dir ), dir = 0;
  }
}



/******************************************************************************
PURPOSE: directoryExists - Does directory exist?
INPUTS:  const char* name  Name of directory to remove files from.
******************************************************************************/

int directoryExists( const char* name ) {
  PRE02( name, *name );
  DIR* directory = opendir( name );
  const int result = directory != 0;

  if ( directory ) {
    closedir( directory ), directory = 0;
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: directoryListing - List files in a directory that end with one of
         the given extensions and are dated today.
INPUTS:  const char* directory   Name of directory.
         const char* extensions  Optional: file extensions to filter by.
                                 E.g., "shx shp txt".
         int size                Size of buffer[].
OUTPUTS: char buffer[ size ]     Result containing formatted columns:
                                 bytes hh:mm name
                                 sorted by file name.
NOTES:   Omits file names that begin with dot (.).
******************************************************************************/

void directoryListing( const char* directory, const char* extensions, int size,
                       char buffer[] ) {
  PRE04( directory, *directory, size > 0, buffer );
  int remaining = size - 1;
  enum { NAME_LENGTH = 1024 };
  char pathedFileName[ NAME_LENGTH + 1 ] = "";
  DIR* dir = opendir( directory );
  memset( pathedFileName, 0, sizeof pathedFileName );
  memset( buffer, 0, size );

  if ( dir ) {
    const time_t clock = time( 0 );
    const struct tm* const timeInfo = localtime( &clock );

    if ( timeInfo ) {
      const int todayYear  = timeInfo->tm_year + 1900;
      const int todayMonth = timeInfo->tm_mon + 1;
      const int todayDay   = timeInfo->tm_mday;
      const struct dirent* entry = 0;

      while ( ( entry = readdir( dir ) ) != 0 ) {
        const char* const fileName = entry->d_name;

        if ( AND2( fileName, *fileName != '.' ) ) {
          int matched = extensions == 0; /* If no filter then all match. */

          if ( extensions ) { /* Check that file extension matches filter: */
            const char* const dot = strrchr( fileName, '.' );

            if ( dot ) {
              const char* const extension = dot + 1;

              if ( *extension ) { /* File name did not end in '.': */
                matched = matchesWord( extension, extensions );
              }
            }
          }

          if ( matched ) {
            struct stat info;
            memset( &info, 0, sizeof info );
            snprintf( pathedFileName, NAME_LENGTH, "%s/%s",
                      directory, fileName );

            if ( ! stat( pathedFileName, &info ) ) {
              const time_t seconds = info.st_mtime;
              const struct tm* const fileTimeInfo = localtime( &seconds );
              const int year   = fileTimeInfo->tm_year + 1900;
              const int month  = fileTimeInfo->tm_mon + 1;
              const int day    = fileTimeInfo->tm_mday;

              if ( AND3( year  == todayYear,
                         month == todayMonth,
                         day   == todayDay ) ) {
                const int hh = fileTimeInfo->tm_hour;
                const int mm = fileTimeInfo->tm_min;
                const size_t fileSize = info.st_size;
                enum { LINE_LENGTH = 255 };
                char line[ LINE_LENGTH + 1 ] = "";
                int lineLength = 0;
                memset( line, 0, sizeof line );
                snprintf( line, LINE_LENGTH, "%9lu %02d:%02d %s\n",
                          fileSize, hh, mm, fileName );
                line[ LINE_LENGTH ] = '\0';
                lineLength = strlen( line );

                if ( remaining > lineLength ) {
                  strncat( buffer, line, remaining );
                  remaining -= lineLength;
                }
              }
            }
          }
        }
      }
    }

    closedir( dir ), dir = 0;
  }
}



/******************************************************************************
PURPOSE: homeDirectory - Name of user's home directory.
RETURNS: const char* Name of user's home directory. Examples:
         "/Users/plessel"
         "C:\Documents and Settings\tplessel"
         "."
******************************************************************************/

const char* homeDirectory( void ) {
  static char result[ 256 ] = "";

  if ( result[ 0 ] == '\0' ) {
#if defined( _WIN32 ) || defined( __CYGWIN__ )
    const char* const homeDrive0 = getenv( "HOMEDRIVE" );
    const char* const homePath0  = getenv( "HOMEPATH" );
    const char* const homeDrive = homeDrive0 ? homeDrive0 : "";
    const char* const homePath = homePath0 ? homePath0 : ".";
    memset( result, 0, sizeof result );
    snprintf( result, sizeof result / sizeof *result,
              "%s%s", homeDrive, homePath );
#else
    const char* const home0 = getenv( "HOME" );
    const char* const home = home0 ? home0 : ".";
    memset( result, 0, sizeof result );
    strncpy( result, home, sizeof result / sizeof *result - 1 );
#endif
  }

  DEBUG( fprintf( stderr, "homeDirectory = '%s'\n", result ); )
  POST02( *result, result[ strlen( result ) - 1 ] != '/' );
  return result;
}



/*============================ PRIVATE FUNCTIONS ============================*/



/******************************************************************************
PURPOSE: radians - Radians of degrees.
INPUTS:  double theDegrees  The degrees to convert.
RETURNS: double radians of degrees.
******************************************************************************/

static double radians( double theDegrees ) {
  PRE0( ! isNan( theDegrees ) );
  const double result = theDegrees * ( M_PI / 180.0 );
  POST03( ! isNan( result ),
          OR2( SIGN( result ) == SIGN( theDegrees ), result == 0.0 ),
          fabs( result ) <= fabs( theDegrees ) );
  return result;
}


