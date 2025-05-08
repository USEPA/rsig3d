
#ifndef DATAUTILITIES_H
#define DATAUTILITIES_H

/*****************************************************************************
PURPOSE: DataUtilities.h - Declare basic low-level utility routines.
HISTORY: 2012-11-20 plessel.todd@epa.gov
STATUS: unreviewed tested
*****************************************************************************/

//================================ INCLUDES ==================================

#include <stdlib.h> // For size_t.
#include <stdio.h>  // For FILE.

#include <string>  // For std::string.

#ifdef _WIN32
/* _WIN32 BUG: Non-standard long long format! */
#define LU "I64u"
#define LLD "I64d"
/* Windows BUG: broken snprintf(), but working hidden one! */
/* #define snprintf _snprintf */
#define FSEEK fseeko64
#define FTELL ftello64
#else
#define LU "lu"
#define LLD "lld"
#define FSEEK fseek
#define FTELL ftell
#endif

#ifdef _WIN32
extern "C" {
extern double drand48( void );
extern char* strtok_r( char*, const char*, char** );
}
#endif

#include <Assertions.h> // For PRE*(), POST*(), AND*(), IN_RANGE().

//=============================== FUNCTIONS ==================================

namespace emvl {

// Constants:

// Having different slashes on local and remote host is problematic
// so just use a UNIX forward slash everywhere - it works on Windows too.
#ifdef _WIN32
const char slash = '\\';
#else
const char slash = '/';
#endif

#if defined( _WIN32 )
const char* const platform = "Win64"; // Subsetter is 64-bit.
#elif defined( __APPLE__ )
#if defined( __x86_64__ )
const char* const platform = "Darwin.x86_64";
#elif defined( __arm64__ )
const char* const platform = "Darwin.arm64";
#endif
#elif defined( __linux__ )
#if defined( __x86_64__ )
const char* const platform = "Linux.x86_64";
#endif
#endif

static const double MISSING = -9.999e36;
static const int IMISSING = -99;
enum { LONGITUDE, LATITUDE, ELEVATION };
enum { MINIMUM, MAXIMUM };
enum { FIRST, LAST };
enum { COLUMN, ROW, LAYER, TIME };

// Basic numerics:

bool isSignedChar(       long long value );
bool isUnsignedChar(     long long value );
bool isSignedShort(      long long value );
bool isUnsignedShort(    long long value );
bool isSignedInt(        long long value );
bool isUnsignedInt(      long long value );
bool isSignedLong(       long long value );
bool isUnsignedLong(     long long value );
bool isSizet(            long long value );
bool isUnsignedLongLong( long long value );

long long toInteger( const char* string, long long lower, long long upper,
                     bool& ok );

double toReal( const char* string, double lower, double upper, bool& ok );


bool isInfinity(      double value );
bool isMinusInfinity( double value );
bool isFinite(        double value );
bool isNan(           double value );
template <typename T> bool isNan( const T* ) { return false; }
bool aboutEqual( double value1, double value2, double tolerance = 1e-6 );

bool isLongitude( double longitude );
bool isLatitude( double latitude );
bool isLongitudeLatitude( double longitude, double latitude );
double radians( double theDegrees );
double degrees( double theRadians );
  
double areaOfTriangle( const double x1, const double y1,
                       const double x2, const double y2,
                       const double x3, const double y3 );

bool pointInsideTriangle( const double x, const double y,
                          const double x1, const double y1,
                          const double x2, const double y2,
                          const double x3, const double y3 );

void reverse2ByteWordsIfLittleEndian( void* array, const size_t count = 1 );
void reverse4ByteWordsIfLittleEndian( void* array, const size_t count = 1 );
void reverse8ByteWordsIfLittleEndian( void* array, const size_t count = 1 );
void expand4ByteWords( double array[], const size_t count = 1 );
void expand4ByteWords( long long array[], const size_t count = 1 );
void expand4ByteWords( unsigned long long array[], const size_t count = 1 );

bool isNanFree( const double array[], const size_t count );
bool isNanFree( const float  array[], const size_t count );
template <typename T> bool isNanFree( const T*, const size_t ) { return true; }
bool allFinite( const double array[], const size_t count );
bool allFinite( const float  array[], const size_t count );
template <typename T> bool allFinite( const T*, const size_t ) { return true; }

void updateMissingValues( double array[], const size_t count );

// Are the only non-zero elements at indices of the given modulus?

template <typename T> bool onlyNonZeroModulo( const T modulus,
                                              const T array[],
                                              const size_t count ) {
  PRE03( modulus, array, count );
  bool result = AND2( count >= modulus, count % modulus == 0 );

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    const T value = array[ index ];

    if ( index % modulus == 0 ) {
      result = value != 0;
    } else {
      result = value == 0;
    }
  }

  POST0( IMPLIES( result, array[ 0 ] ) );
  return result;
}


// Result is value clamped within range [low, high]:

template <typename T> T clampedToRange( const T value,
                                        const T low, const T high ) {
  PRE04( ! isNan( value ), ! isNan( low ), ! isNan( high ), low <= high );
  const T result = value < low ? low : value > high ? high : value;
  POST02( ! isNan( result ), IN_RANGE( result, low, high ) );
  return result;
}


// Are all array elements zero?

template <typename T> bool allZero( const T array[], const size_t count ) {
  PRE02( array, count );
  bool result = true;

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    result = array[ index ] == 0;
  }

  POST02( IMPLIES( result, IS_ZERO2( array[ 0 ], array[ count - 1 ] ) ),
          IMPLIES( OR2( array[ 0 ], array[ count - 1 ] ), ! result ) );
  return result;
}


// Are all array elements non-zero?

template <typename T> bool allNonZero( const T array[], const size_t count ) {
  PRE02( array, count );
  bool result = true;

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    result = array[ index ] != 0;
  }

  POST02( IMPLIES( result, ! IS_ZERO2( array[ 0 ], array[ count - 1 ] ) ),
          IMPLIES( OR2( ! array[ 0 ], ! array[ count - 1 ] ), ! result ) );
  return result;
}


// Are array elements ordered by <=?

template <typename T> bool isSorted( const T array[], const size_t count ) {
  PRE02( array, isNanFree( array, count ) );
  bool result = true;

  for ( size_t index = 1; AND2( result, index < count ); ++index ) {
    result = array[ index - 1 ] <= array[ index ];
  } 

  POST02( IMPLIES( result, array[ 0 ] <= array[ count - 1 ] ),
          IMPLIES( array[ 0 ] > array[ count - 1 ], ! result ) );
  return result;
}


// Are array elements ordered by <?

template <typename T> bool isIncreasing(const T array[], const size_t count) {
  PRE02( array, isNanFree( array, count ) );
  bool result = true;

  for ( size_t index = 1; AND2( result, index < count ); ++index ) {
    result = array[ index - 1 ] < array[ index ];
  } 

  POST02( IMPLIES( result, array[ 0 ] < array[ count - 1 ] ),
          IMPLIES( array[ 0 ] >= array[ count - 1 ], ! result ) );
  return result;
}


// Are array elements ordered by >?

template <typename T> bool isDecreasing(const T array[], const size_t count) {
  PRE02( array, isNanFree( array, count ) );
  bool result = true;

  for ( size_t index = 1; AND2( result, index < count ); ++index ) {
    result = array[ index - 1 ] > array[ index ];
  } 

  POST02( IMPLIES( result, array[ 0 ] > array[ count - 1 ] ),
          IMPLIES( array[ 0 ] <= array[ count - 1 ], ! result ) );
  return result;
}


// Is value in array?

template <typename T> bool has( const T array[], const size_t count, T value) {
  PRE04( array, count, isNanFree( array, count ), ! isNan( value ) );
  bool result = false;

  for ( size_t index = 0; AND2( ! result, index < count ); ++index ) {
    result = array[ index ] == value;
  }

  return result;
}


// Minimum element in array:

template <typename T> T minimum( const T array[], const size_t count ) {
  PRE03( array, count, isNanFree( array, count ) );
  T result = array[ 0 ];

  for ( size_t index = 1; index < count; ++index ) {
    const T value = array[ index ];

    if ( value < result ) {
      result = value;
    }
  }

  POST04( ! isNan( result ),
          result <= array[ 0 ],
          result <= array[ count - 1 ],
          has( array, count, result ) );
  return result;
}


// Maximum element in array:

template <typename T> T maximum( const T array[], const size_t count ) {
  PRE03( array, count, isNanFree( array, count ) );
  T result = array[ 0 ];

  for ( size_t index = 1; index < count; ++index ) {
    const T value = array[ index ];

    if ( value > result ) {
      result = value;
    }
  }

  POST04( ! isNan( result ),
          result >= array[ 0 ],
          result >= array[ count - 1 ],
          has( array, count, result ) );
  return result;
}


// Minimum and maximum elements in array:

template <typename T>
void range( const T array[], const size_t count, T& minimum, T& maximum ) {
  PRE03( array, count, isNanFree( array, count ) );
  minimum = maximum = array[ 0 ];

  for ( size_t index = 1; index < count; ++index ) {
    const T value = array[ index ];

    if ( value < minimum ) {
      minimum = value;
    } else if ( value > maximum ) {
      maximum = value;
    }
  }

  POST09( ! isNan( minimum ),
          ! isNan( maximum ),
          minimum <= maximum,
          minimum <= array[ 0 ],
          minimum <= array[ count - 1 ],
          maximum >= array[ 0 ],
          maximum >= array[ count - 1 ],
          has( array, count, minimum ),
          has( array, count, maximum ) );
}


// Minimum and maximum elements in array > threshold:


template <typename T>
void range( const T array[], const size_t count, const T threshold,
            T& minimum, T& maximum ) {
  PRE04( array, count, ! isNan( threshold ), isNanFree( array, count ) );
  minimum = maximum = array[ 0 ]; // Initialize to first item.

  for ( size_t index = 1; index < count; ++index ) {
    const T value = array[ index ];

    if ( value > threshold ) { // Only consider values > threshold.

      if ( minimum <= threshold ) { // Re-initialize to 1st item > threshold.
        minimum = maximum = value;
      } else if ( value < minimum ) {
        minimum = value;
      } else if ( value > maximum ) {
        maximum = value;
      }
    }
  }

  if ( minimum < threshold ) {
    minimum = threshold;
  }

  if ( maximum < minimum ) {
    maximum = minimum;
  }

  POST07( ! isNan( minimum ),
          ! isNan( maximum ),
          minimum <= maximum,
          IMPLIES( array[ 0 ] > threshold,
                   AND2( minimum <= array[ 0 ],
                         maximum >= array[ 0 ] ) ),
          IMPLIES( array[ count - 1 ] > threshold,
                   AND2( minimum <= array[ count - 1 ],
                         maximum >= array[ count - 1 ] ) ),
          IMPLIES( minimum != threshold,
                   AND2( minimum > threshold,
                         has( array, count, minimum ) ) ),
          IMPLIES( maximum != threshold,
                   AND2( maximum > threshold,
                         has( array, count, maximum ) ) ) );
}


// Sum of array elements.

template <typename T>
T sum( const T array[], const size_t count ) {
  PRE03( array, count, allFinite( array, count ) );
  T result = array[ 0 ];

  for ( size_t index = 1; index < count; ++index ) {
    result += array[ index ];
  }

  POST0( ! isNan( result ) );
  return result;
}


// Sum product of each pair of adjacent array elements.

template <typename T>
T sumProduct2( const T array[], const size_t count ) {
  PRE04( array, count, count % 2 == 0, allFinite( array, count ) );
  T result = 0;

  for ( size_t index = 0; index < count; index += 2 ) {
    result += array[ index ] * array[ index + 1 ];
  }

  POST0( ! isNan( result ) );
  return result;
}


// Maximum of products of pairs of adjacent array elements.

template <typename T>
T maximumProduct2( const T array[], const size_t count ) {
  PRE04( array, count, count % 2 == 0, allFinite( array, count ) );
  T result = 0;

  for ( size_t index = 0; index < count; index += 2 ) {
    const T product = array[ index ] * array[ index + 1 ];

    if ( product > result ) {
      result = product;
    }
  }

  POST0( ! isNan( result ) );
  return result;
}


// output += input.

template <typename T>
void accumulate( const T input[], T output[], const size_t count ) {
  PRE05( input, output, count,
         allFinite( input, count ), isNanFree( output, count ) );

  for ( size_t index = 0; index < count; ++index ) {
    output[ index ] += input[ index ];
  }

  POST0( isNanFree( output, count ) );
}


// if array > threshold then array *= scale. Returns maximum scaled element.

template <typename T>
T scaledMaximum( T array[], const size_t count, T scale, T threshold ) {
  PRE07( array, count, isNanFree( array, count ),
         scale, isFinite( scale ), ! isNan( scale ), ! isNan( threshold ) );
  bool initialized = false;
  T result = *array;

  for ( size_t index = 0; index < count; ++index ) {

    if ( array[ index ] > threshold ) {
      array[ index ] *= scale;

      if ( ! initialized ) {
        result = array[ index ];
        initialized = true;
      } else if ( array[ index ] > result ) {
        result = array[ index ];
      }
    }
  }

  POST0( ! isNan( result ) );
  return result;
}


// Swap row order of 2D array[ rows ][ columns ]:

template <typename T> void swapRows( T array[], const size_t rows,
                                     const size_t columns ) {
  PRE03( array, rows, columns );
  size_t lowerRowIndex = 0;
  size_t upperRowIndex = rows - 1;

  for ( ; lowerRowIndex < upperRowIndex; ++lowerRowIndex, --upperRowIndex ) {
    T* const lowerRowData = array + lowerRowIndex * columns;
    T* const upperRowData = array + upperRowIndex * columns;

    for ( size_t column = 0; column < columns; ++column ) {
      T temp = lowerRowData[ column ];
      lowerRowData[ column ] = upperRowData[ column ];
      upperRowData[ column ] = temp;
    }
  }
}


// Expand each 1-byte word in array to type T:

template <typename T> void expandBytes( T array[], const size_t count ) {
  PRE02( array, count );
  const unsigned char* input = (const unsigned char*) array;
  T* output = array + count;
  size_t counter = count;
  input += count;

  while ( counter-- ) {
    *--output = *--input;
  }
}


// Copy converted/clamped array data to output array.

template <typename T>
void copyArray( const double array[], const size_t count,
                const T minimum, const T maximum, T output[] ) {
  PRE07( array, count, isNanFree( array, count ),
         isFinite( minimum ), isFinite( maximum ), minimum <= maximum, output);

  for ( size_t index = 0; index < count; ++index ) {
    const double value = array[ index ];
    const double clampedValue =
      clampedToRange( value, (double) minimum, (double) maximum );
    const T convertedValue = (const T) clampedValue;
    output[ index ] = convertedValue;
  }
}


/*
 * Read from stream into buffer and convert/copy to output.
 * return true if converted values are in [minimum, maximum] else false.
 */

template <typename T1, typename T2>
bool readConvertedArray( FILE* const stream,
                         T1 buffer[], const size_t count,
                         const T2 minimum, const T2 maximum, T2 output[] ) {
  PRE05( stream, buffer, count, minimum <= maximum, output );
  bool result = fread( buffer, sizeof buffer[ 0 ], count, stream ) == count;

  if ( result ) {

    if ( sizeof buffer[ 0 ] == 8 ) {
      reverse8ByteWordsIfLittleEndian( buffer, count );
    } else if ( sizeof buffer[ 0 ] == 4 ) {
      reverse4ByteWordsIfLittleEndian( buffer, count );
    } else if ( sizeof buffer[ 0 ] == 2 ) {
      reverse2ByteWordsIfLittleEndian( buffer, count );
    }

    for ( size_t index = 0; AND2( result, index < count ); ++index ) {
      const T1 value = buffer[ index ];
      const T2 convertedValue = (const T2) value;
      result = IN_RANGE( convertedValue, minimum, maximum );

      if ( result ) {
        output[ index ] = convertedValue;
      }
    }
  }

  return result;
}


/*
 * Write to stream a buffer converted/copied from input and
 * clamped to [minimum, maximum].
 * returns true if successful.
 */

template <typename T1, typename T2>
bool writeConvertedArray( FILE* const stream,
                          T1 buffer[], const size_t count,
                          const T1 minimum, const T1 maximum,
                          const T2 input[] ) {
  PRE05( stream, buffer, count, minimum <= maximum, input );

  for ( size_t index = 0; index < count; ++index ) {
    const T2 value = input[ index ];
    const T1 convertedValue = (const T1) value;
    const T1 clampedValue =
      clampedToRange( convertedValue, minimum, maximum );
    buffer[ index ] = clampedValue;
  }

  if ( sizeof buffer[ 0 ] == 8 ) {
    reverse8ByteWordsIfLittleEndian( buffer, count );
  } else if ( sizeof buffer[ 0 ] == 4 ) {
    reverse4ByteWordsIfLittleEndian( buffer, count );
  } else if ( sizeof buffer[ 0 ] == 2 ) {
    reverse2ByteWordsIfLittleEndian( buffer, count );
  }

  const bool result =
    fwrite( buffer, sizeof buffer[ 0 ], count, stream ) == count;

  return result;
}


// String processing:

bool isValidArgs( int argc, const char* const argv[] );

size_t indexOfString( const char* string, const char* const strings[],
                      const size_t count );

size_t wordCount( const char* string );
size_t lineCount( const char* string );
bool isText( const char* string );
void lowercase( char string[] );
void eraseDoubleQuotedCommas( char string[] );

void substituteWord( const char* input,
                     const char* oldWord,
                     const char* newWord,
                     char* output );


// File processing:

bool fileExists( const char* fileName );
size_t fileSize( const char* fileName );
bool copyFile( const char* inputFileName, const char* outputFileName );
bool copyFileBytes( FILE* inputFile, FILE* outputFile );
bool copyFileBytes( FILE* inputFile, const char* outputFileName );
bool copyFileBytes( FILE* inputFile, const char* outputFileName,
                    const size_t bytes );

bool streamFile( const char* fileName );
bool streamFile( const char* fileName, FILE* stream );
void removeAllFiles( const char* const directoryName,
                     const char* const extensions = 0 );
void removeDirectory( const char* const directoryName );

void directoryListing( const char* directoryName, const char* extensions,
                       size_t size, char buffer[] );

bool isDirectory( const char* const name );
bool isDirectory( const std::string& name );
char trimSlashes( std::string& directory );
std::string parentDirectory( const std::string& directory );
std::string appendPath( const std::string& directory,
                        const std::string& subdirectory );

std::string getHomeDirectory();
bool readCommandResult( const std::string& command, std::string& result);
bool readCommandResult( const std::string& command, FILE* result );
void trimEndingWhitespace( std::string& s );

bool isNetCDFFile( const std::string& name );

// Misc:

void elevationsAtSigmaPressures( const double g,
                                 const double R,
                                 const double A,
                                 const double T0s,
                                 const double P00,
                                 const double surfaceElevation,
                                 const int levels,
                                 const double topPressure,
                                 const double sigmaPressures[],
                                 double elevations[] );



} // namespace emvl.

#endif /* DATAUTILITIES_H */



