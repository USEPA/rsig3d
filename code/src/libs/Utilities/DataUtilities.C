
/******************************************************************************
PURPOSE: DataUtilities.C - Data utility routines.
HISTORY: 2012-11-20 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdlib.h>   // For popen().
#include <string.h>   // For strcmp().
#include <ctype.h>    // For isspace(), isprint().
#include <errno.h>    // For errno.
#include <math.h>     // For M_PI, fabs(), sqrt().
#include <limits.h>   // For INT_MAX.
#include <float.h>    // For DBL_MAX.
#include <stdlib.h>   // For popen(), getenv(), drand48() or rand().
#include <unistd.h>   // For unlink().
#include <time.h>     // For struct tm, localtime().
#include <dirent.h>   // For DIR, struct dirent, opendir(), readdir(), etc.
#include <sys/stat.h> // For stat().

#include <string> // For std::string.

/*
 * Is the platform big-endian (MSB: most significant byte first) or
 * little-endian (LSB: least significant byte first)?
 */

#ifndef IS_LITTLE_ENDIAN
#if ( \
    ( defined(__BYTE_ORDER__) && \
      defined(__ORDER_LITTLE_ENDIAN__) && \
      __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ ) || \
    defined(__x86_64__) || \
    defined(__i386__)   || \
    defined(__i486__)   || \
    defined(__i586__)   || \
    defined(__i686__)   || \
    defined(__alpha)    || \
    defined(__ia64__)   || \
    defined(__ARMEL__)  || \
    defined(__MIPSEL)   || \
    defined(_WIN32)     || \
    defined(_WIN64)     || \
    defined(_M_IX86)    || \
    defined(_M_AMD64)      \
  )
#define IS_LITTLE_ENDIAN 1
#else
#define IS_LITTLE_ENDIAN 0
#endif
#endif


#include <Assertions.h>    // For PRE*(), POST*(), CHECK*(), AND*().
#include <DataUtilities.h> // For public interface.

//============================= PUBLIC FUNCTIONS ==============================

namespace emvl {



/******************************************************************************
PURPOSE: isSignedChar - Is value representable without loss as a signed char?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isSignedChar( long long value ) {
  const bool result = IN_RANGE( value, SCHAR_MIN, SCHAR_MAX );
  POST0( result == IN_RANGE( value, SCHAR_MIN, SCHAR_MAX ) );
  return result;
}



/******************************************************************************
PURPOSE: isUnsignedChar - Is value representable without loss as unsigned char?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isUnsignedChar( long long value ) {
  const bool result = IN_RANGE( value, 0, UCHAR_MAX );
  POST0( result == IN_RANGE( value, 0, UCHAR_MAX ) );
  return result;
}



/******************************************************************************
PURPOSE: isSignedShort - Is value representable without loss as a signed short?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isSignedShort( long long value ) {
  const bool result = IN_RANGE( value, SHRT_MIN, SHRT_MAX );
  POST0( result == IN_RANGE( value, SHRT_MIN, SHRT_MAX ) );
  return result;
}



/******************************************************************************
PURPOSE: isUnsignedShort - Is value representable without loss as an unsigned
         short?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isUnsignedShort( long long value ) {
  const bool result =
    AND2( value >= 0,
          OR2( sizeof (unsigned short) >= sizeof (long long),
               value <= USHRT_MAX ) );
  POST0( result == AND2( value >= 0,
                         OR2( sizeof (unsigned short) >= sizeof (long long),
                              value <= USHRT_MAX ) ) );
  return result;
}



/******************************************************************************
PURPOSE: isSignedInt - Is value representable without loss as a signed int?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isSignedInt( long long value ) {
  const bool result = IN_RANGE( value, INT_MIN, INT_MAX );
  POST0( result == IN_RANGE( value, INT_MIN, INT_MAX ) );
  return result;
}



/******************************************************************************
PURPOSE: isUnsignedInt - Is value representable without loss as unsigned int?
INPUTS:  long long value The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isUnsignedInt( long long value ) {
  const bool result = AND2( value >= 0,
                            OR2( sizeof (unsigned int) >= sizeof (long long),
                                 value <= UINT_MAX ) );
  POST0( result == AND2( value >= 0,
                         OR2( sizeof (unsigned int) >= sizeof (long long),
                              value <= UINT_MAX ) ) );
  return result;
}



/******************************************************************************
PURPOSE: isSignedLong - Is value representable without loss as a signed long?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isSignedLong( long long value ) {
  const bool result = IN_RANGE( value, LONG_MIN, LONG_MAX );
  POST0( result == IN_RANGE( value, LONG_MIN, LONG_MAX ) );
  return result;
}



/******************************************************************************
PURPOSE: isUnsignedLong - Is value representable without loss as an unsigned
         long?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isUnsignedLong( long long value ) {
  const bool result =
    AND2( value >= 0,
          OR2( sizeof (unsigned long) >= sizeof (long long),
               value <= (long long) ULONG_MAX ) );
  POST0( result == AND2( value >= 0,
                         OR2( sizeof (unsigned long) >= sizeof (long long),
                              value <= (long long) ULONG_MAX ) ) );
  return result;
}



/******************************************************************************
PURPOSE: isSizet - Is value representable without loss as a size_t?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isSizet( long long value ) {
  const bool result =
    AND2( value >= 0,
          OR2( sizeof (size_t) >= sizeof (long long),
               value <= (long long) ULONG_MAX ) );
  POST0( result == AND2( value >= 0,
                         OR2( sizeof (size_t) >= sizeof (long long),
                              value <= (long long) ULONG_MAX ) ) );
  return result;
}



/******************************************************************************
PURPOSE: isUnsignedLongLong - Is value representable without loss as an
         unsigned long long?
INPUTS:  long long value  The value to test.
RETURNS: bool true if representable, else false.
******************************************************************************/

bool isUnsignedLongLong( long long value ) {
  const bool result =
    AND2( value >= 0,
          OR2( sizeof (unsigned long long) >= sizeof (long long),
               (unsigned long long) value <= ULLONG_MAX ) );
  POST0( result == AND2( value >= 0,
                         OR2(sizeof (unsigned long long) >= sizeof (long long),
                              (unsigned long long) value <= ULLONG_MAX ) ) );
  return result;
}



/******************************************************************************
PURPOSE: toInteger - Integer value of string if within range [lower, upper].
INPUTS:  const char* string  - The string to convert.
         long long lower  The lower limit of valid range.
         long long upper  The upper limit of valid range.
OUTPUTS: bool& ok         Does string represent an integer in [lower,upper]?
RETURNS: long long value of string within range [lower, upper], else 0.
NOTES:   Unlike atoI(), strtoI() and scanf(), this routine rejects strings
         that would overflow or that contain non-digit characters (except
         an optional leading sign) or lack digit characters, or contain
         multiple whitespace-separated tokens.
******************************************************************************/

long long toInteger( const char* string, long long lower, long long upper,
                     bool& ok ) {
  PRE02( string, lower <= upper );
  long long result = 0;
  ok = false;
  errno = 0; // strtoll sets errno upon failure but won't clear it on success!

  if ( *string ) {
    char* terminator = 0;
    const long long convertionResult = strtoll( string, &terminator, 10 );

    if ( AND2( terminator, terminator != string ) ) {

      while ( isspace( *terminator ) ) { // Skip trailing whitespace.
        ++terminator;
      }

      ok = AND3( errno == 0, // No overflow.
                 *terminator == '\0', // No remaining characters.
                 IN_RANGE( convertionResult, lower, upper ) );

      if ( ok ) {
        result = convertionResult;
      }
    }
  }

  POST0( IMPLIES_ELSE( ok, IN_RANGE( result, lower, upper ), result == 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: toReal - Real value of string if within range [lower, upper].
INPUTS:  const char* string   The string to convert.
         double lower         The lower limit of valid range.
         double upper         The upper limit of valid range.
OUTPUTS: bool& ok             Is string an integer in [lower, upper]?
RETURNS: double value of string within range [lower, upper], else 0.0.
******************************************************************************/

double toReal( const char* string, double lower, double upper, bool& ok ) {
  PRE04( string, ! isNan( lower ), ! isNan( upper ), lower <= upper );
  double result = 0;
  ok = false;
  errno = 0; // strtod sets errno upon failure but won't clear it on success!

  if ( *string ) {
    char* terminator = 0;
    const double convertionResult = strtod( string, &terminator );

    if ( AND2( terminator, terminator != string ) ) {

      while ( isspace( *terminator ) ) { // Skip trailing whitespace.
        ++terminator;
      }

      ok = AND2( *terminator == '\0', // No remaining characters.
                 IN_RANGE( convertionResult, lower, upper ) );

      if ( ok ) {
        result = convertionResult;
      }
    }
  }

  POST0( IMPLIES_ELSE( ok, IN_RANGE( result, lower, upper ), result == 0.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: isInfinity - Is the value so large that its recipricol is zero?
INPUTS:  double x - The value to test.
RETURNS: bool true if recipricol is zero, else false.
******************************************************************************/

bool isInfinity( double x ) {
  const bool result = AND2( x > 0.0, OR2( x > DBL_MAX, 1.0 / x == 0.0 ) );
  POST0( result == AND2( x > 0.0, OR2( x > DBL_MAX, 1.0 / x == 0.0 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: isMinusInfinity - Is the value so small that its recipricol is zero?
INPUTS:  double x - The value to test.
RETURNS: bool true if recipricol is zero, else false.
******************************************************************************/

bool isMinusInfinity( double x ) {
  const bool result = AND2( x < 0.0, OR2( x < -DBL_MAX, 1.0 / x == 0.0 ) );
  POST0( result == AND2( x < 0.0, OR2( x < -DBL_MAX, 1.0 / x == 0.0 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: isFinite - Is the value and its recipricol
         non-NaN/+Infinity/-Infinity?
INPUTS:  double x - The value to test.
RETURNS: bool true if finite, else false.
******************************************************************************/

bool isFinite( double x ) {
  const bool result =
    AND3( ! isNan( x ), ! isInfinity( x ), ! isMinusInfinity( x ) );
  POST0( result == AND3( ! isNan(x), ! isInfinity(x), ! isMinusInfinity(x) ));
  return result;
}



/******************************************************************************
PURPOSE: isNan - Is x a NaN (Not a Number)?
INPUTS:  double x - The 64-bit value to test.
RETURNS: bool true if x is a NaN, else false.
******************************************************************************/

bool isNan( double x ) {
  const double copy = x;
  const bool result = ( copy != x );
  return result;
}



/******************************************************************************
PURPOSE: aboutEqual - Do x and y differ by less than (non-negative,
         finite) tolerance (i.e., fabs( x - y ) <= tolerance) or, for large
         values, differ only in digits beyond number of significant digits in
         tolerance? (E.g., tolerance = 1e-6 and 1.0000000001e30 and
         1.0000000002e30 are considered equal.)
INPUTS:  double x - first value to compare.
         double y - second value to compare.
         double tolerance - tolerance threshold (e.g., 1-e6).
RETURNS: bool reue if x and y differ (in significant digits) by less than
         tolerance, else false.
NOTES:   This function is commutative:
           withinTolerance( x, y, tolerance ) ==
           withinTolerance( y, x, tolerance )
         but not transitive:
           (withinTolerance( x, y, tolerance ) &&
            withinTolerance( x, z, tolerance ))
         does not imply
            withinTolerance( y, z, tolerance )
         (due to pivoting, e.g., about zero:
         if x == 0.0 and y = -tolerance and z == +tolerance,
         x ~ y and x ~ z but y !~ z)
         See: Squassabia, Alberto, "Comparing Floats", C++ Report, Vol 12,
         No 2, February 2000, pp 30-32, 39. SIGS Publications.
******************************************************************************/

bool aboutEqual( double x, double y, double tolerance ) {
  PRE02( ! isNan( tolerance ), IN_RANGE( tolerance, 0.0, 0.1 ) );

  // First try bitwise comparison (handles nans):

  const double* const xp = &x;
  const double* const yp = &y;
  const long long* const ixp = (const long long*) xp;
  const long long* const iyp = (const long long*) yp;
  const long long ix = *ixp;
  const long long iy = *iyp;
  bool result = ix == iy;

  if ( result == 0 ) {

    if ( x == 0.0 ) {
      result = IN_RANGE( y, -tolerance, tolerance ); // Close enough to 0?
    } else if ( y == 0.0 ) {
      result = IN_RANGE( x, -tolerance, tolerance ); // Close enough to 0?
    } else if ( IN_RANGE( x, y - tolerance, y + tolerance)) { // Or each other?
      result = 1;
    } else if ( IN_RANGE( y, x - tolerance, x + tolerance)) { // Or each other?
      result = 1;
    } else { // Ratio handles cases of large values differing in last digits.
      const double ax = fabs( x );
      const double ay = fabs( y );

      if ( AND2( ay < 1.0, ax > ay * DBL_MAX ) ) { // Avoid overflow.
        result = 0;
      } else if ( AND2( ay > 1.0, ax < ay * DBL_MIN ) ) { // Avoid underflow.
        result = 0;
      } else {
        const double ratio = x / y;
        result = IN_RANGE( ratio, 1.0 - tolerance, 1.0 + tolerance );
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: isLongitude - Is longitude in range [-180, 180]?
INPUTS:  double longitude Longitude to check.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool isLongitude( double longitude ) {
  const bool result = IN_RANGE( longitude, -180.0, 180.0 );
  POST0( result == IN_RANGE( longitude, -180.0, 180.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: isLatitude - Is latitude in range [-90, 90]?
INPUTS:  double latitude Latitude to check.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool isLatitude( double latitude ) {
  const bool result = IN_RANGE( latitude, -90.0, 90.0 );
  POST0( result == IN_RANGE( latitude, -90.0, 90.0 ) );
  return result;
}





/******************************************************************************
PURPOSE: isLongitudeLatitude - Is (longitude, latitude) a valid point?
INPUTS:  double longitude Longitude to check.
         double latitude  Latitude  to check.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool isLongitudeLatitude( double longitude, double latitude ) {
  const bool result =
    AND2( isLongitude( longitude ), isLatitude( latitude ) );
  POST0( result == AND2( isLongitude( longitude ), isLatitude( latitude ) ) );
  return result;
}



/******************************************************************************
PURPOSE: radians - Radians of degrees.
INPUTS:  double theDegrees  The degrees to convert.
RETURNS: double radians of degrees.
******************************************************************************/

double radians( double theDegrees ) {
  PRE0( ! isNan( theDegrees ) );
  const double result = theDegrees * ( M_PI / 180.0 );
  POST03( ! isNan( result ),
          OR2( SIGN( result ) == SIGN( theDegrees ), result == 0.0 ),
          fabs( result ) <= fabs( theDegrees ) );
  return result;
}



/******************************************************************************
PURPOSE: degrees - Degrees of radians.
INPUTS:  double theDegrees  The degrees to convert.
RETURNS: double radians of degrees.
******************************************************************************/

double degrees( double theRadians ) {
  PRE0( ! isNan( theRadians ) );
  const double result = theRadians * ( 180.0 / M_PI );
  POST03( ! isNan( result ),
          OR2( SIGN( result ) == SIGN( theRadians ), result == 0.0 ),
          fabs( result ) >= fabs( theRadians ) );
  return result;
}



/******************************************************************************
PURPOSE: areaOfTriangle - Area of triangle with vertices
         (x1, y1), (x2, y2), (x3, y3).
INPUTS:  const double x1   X-Coordinate of 1st vertex of triangle.
         const double y1   Y-Coordinate of 1st vertex of triangle.
         const double x2   X-Coordinate of 2nd vertex of triangle.
         const double y2   Y-Coordinate of 2nd vertex of triangle.
         const double x3   X-Coordinate of 3rd vertex of triangle.
         const double y3   Y-Coordinate of 3rd vertex of triangle.
RETURNS: double area.
******************************************************************************/

double areaOfTriangle( const double x1, const double y1,
                       const double x2, const double y2,
                       const double x3, const double y3 ) {
  const double a = x1 - x3;
  const double b = y1 - y3;
  const double c = x2 - x3;
  const double d = y2 - y3;
  const double triangleArea = 0.5 * ( a * d - b * c );
  const double result = triangleArea < 0.0 ? -triangleArea : triangleArea;
  return result;
}



/******************************************************************************
PURPOSE: pointInsideTriangle - Determine if point (x, y) is inside triangle
         with vertices (x1, y1), (x2, y2), (x3, y3).
INPUTS:  const double x    X-Coordinate of point to test.
         const double y    Y-Coordinate of point to test.
         const double x1   X-Coordinate of 1st vertex of triangle.
         const double y1   Y-Coordinate of 1st vertex of triangle.
         const double x2   X-Coordinate of 2nd vertex of triangle.
         const double y2   Y-Coordinate of 2nd vertex of triangle.
         const double x3   X-Coordinate of 3rd vertex of triangle.
         const double y3   Y-Coordinate of 3rd vertex of triangle.
RETURNS: bool true if inside, else false.
******************************************************************************/

bool pointInsideTriangle( const double x, const double y,
                          const double x1, const double y1,
                          const double x2, const double y2,
                          const double x3, const double y3 ) {
  const double scale = 1.01; // Allow within 1% larger for round-off error.
  const double triangleArea = scale * areaOfTriangle( x1, y1, x2, y2, x3, y3 );
  bool result = triangleArea > 0.0;

  if ( result ) {
    double area = areaOfTriangle( x, y, x2, y2, x3, y3 );
    result = area <= triangleArea;

    if ( result ) {
      area += areaOfTriangle( x, y, x1, y1, x2, y2 );
      result = area <= triangleArea;

      if ( result ) {
        area += areaOfTriangle( x, y, x1, y1, x3, y3 );
        result = area <= triangleArea;
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: reverse2ByteWordsIfLittleEndian - Reverse 2-byte words of each array
         element if on a little-endian platform.
INPUTS:  void* array         Array of 2-byte values to reverse.
         const size_t count  Number of words in array.
OUTPUTS: void* array   Array of reversed values.
******************************************************************************/

void reverse2ByteWordsIfLittleEndian( void* array, const size_t count ) {

#if IS_LITTLE_ENDIAN

  PRE02( array, IN_RANGE( count, 1, LLONG_MAX ) );
  assert_static( sizeof (unsigned short) == 2 );
  unsigned short* const array2 = (unsigned short*) array;

/*
RSIG3D uses pthreads then calls this routine which crashes when using
OpenMP threads due to a g++ BUG:
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=42616
#pragma omp parallel for
*/

  for ( long long index = 0; index < (long long) count; ++index ) {
    const unsigned short value = array2[ index ];
    const unsigned short newValue =
      ( value & 0xff00 ) >> 8 |
      ( value & 0x00ff ) << 8;
    CHECK( IMPLIES( OR2( value == 0, value == 65535 ), newValue == value ) );
    array2[ index ] = newValue;
  }

#endif

}



/******************************************************************************
PURPOSE: reverse4ByteWordsIfLittleEndian - Reverse 4-byte words of each array
         element if on a little-endian platform.
INPUTS:  void* array         Array of 4-byte values to reverse.
         const size_t count  Number of words in array.
OUTPUTS: void* array   Array of reversed values.
******************************************************************************/

void reverse4ByteWordsIfLittleEndian( void* array, const size_t count ) {

#if IS_LITTLE_ENDIAN

  PRE02( array, IN_RANGE( count, 1, LLONG_MAX ) );
  assert_static( sizeof (unsigned int) == 4 );
  unsigned int* const array4 = (unsigned int*) array;

/*
RSIG3D uses pthreads then calls this routine which crashes when using
OpenMP threads due to a g++ BUG:
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=42616
#pragma omp parallel for
*/

  for ( long long index = 0; index < (long long) count; ++index ) {
    const unsigned int value = array4[ index ];
    const unsigned int newValue =
      ( value & 0xff000000 ) >> 24 |
      ( value & 0x00ff0000 ) >>  8 |
      ( value & 0x0000ff00 ) <<  8 |
      ( value & 0x000000ff ) << 24;
    CHECK( IMPLIES( OR2(value == 0, value == 4294967295U), newValue == value));
    array4[ index ] = newValue;
  }

#endif

}



/******************************************************************************
PURPOSE: reverse8ByteWordsIfLittleEndian - Reverse 8-byte words of each array
         element if on a little-endian platform.
INPUTS:  void* array         Array of 8-byte values to reverse.
         const size_t count  Number of words in array.
OUTPUTS: void* array   Array of reversed values.
******************************************************************************/

void reverse8ByteWordsIfLittleEndian( void* array, const size_t count ) {

#if IS_LITTLE_ENDIAN

  PRE02( array, IN_RANGE( count, 1, LLONG_MAX ) );
  assert_static( sizeof (unsigned long long) == 8 );
  unsigned long long* const array8 = (unsigned long long*) array;

/*
RSIG3D uses pthreads then calls this routine which crashes when using
OpenMP threads due to a g++ BUG:
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=42616
#pragma omp parallel for
*/

  for ( long long index = 0; index < (long long) count; ++index ) {
    const unsigned long long value = array8[ index ];
    const unsigned long long newValue =
      ( value & 0xff00000000000000LL ) >> 56 |
      ( value & 0x00ff000000000000LL ) >> 40 |
      ( value & 0x0000ff0000000000LL ) >> 24 |
      ( value & 0x000000ff00000000LL ) >>  8 |
      ( value & 0x00000000ff000000LL ) <<  8 |
      ( value & 0x0000000000ff0000LL ) << 24 |
      ( value & 0x000000000000ff00LL ) << 40 |
      ( value & 0x00000000000000ffLL ) << 56;
    CHECK( IMPLIES( OR2( value == 0, value == 18446744073709551615ULL ),
                    newValue == value ) );
    array8[ index ] = newValue;
  }

#endif

}



/******************************************************************************
PURPOSE: expand4ByteWords - Expand 4-byte words of each array element stored
         at the first half of the array.
INPUTS:  double array[ count ]   Array of 4-byte floats to expand to double.
         const size_t count      Number of words in array.
OUTPUTS: double array[ count ]   Array of 8-byte doubles.
******************************************************************************/

void expand4ByteWords( double array[], const size_t count ) {
  PRE0( array );
  assert_static( sizeof (double) == 8 );
  assert_static( sizeof (float) == 4 );
  double* output = array + count;
  float* input = ( (float*) array ) + count;
  size_t counter = count;

  while ( counter-- ) {
    *--output = *--input;
  }
}



/******************************************************************************
PURPOSE: expand4ByteWords - Expand 4-byte words of each array element stored
         at the first half of the array.
INPUTS:  long long array[ count ]  Array of 4-byte ints to expand to long long.
         const size_t count        Number of words in array.
OUTPUTS: long long array[ count ]  Array of 8-byte long longs
******************************************************************************/

void expand4ByteWords( long long array[], const size_t count ) {
  PRE0( array );
  assert_static( sizeof (long long) == 8 );
  assert_static( sizeof (int) == 4 );
  long long* output = array + count;
  int* input = ( (int*) array ) + count;
  size_t counter = count;

  while ( counter-- ) {
    *--output = *--input;
  }
}



/******************************************************************************
PURPOSE: expand4ByteWords - Expand 4-byte words of each array element stored
         at the first half of the array.
INPUTS:  unsigned long long array[ count ]  Array of 4-byte unsigned ints to
                                            expand to unsigned long long.
         const size_t count        Number of words in array.
OUTPUTS: unsigned long long array[ count ]  Array of 8-byte unsigned long longs
******************************************************************************/

void expand4ByteWords( unsigned long long array[], const size_t count ) {
  PRE0( array );
  assert_static( sizeof (unsigned long long) == 8 );
  assert_static( sizeof (unsigned int) == 4 );
  unsigned long long* output = array + count;
  unsigned int* input = ( (unsigned int*) array ) + count;
  size_t counter = count;

  while ( counter-- ) {
    *--output = *--input;
  }
}



/******************************************************************************
PURPOSE: isNanFree - Verify that an array contains no NaNs (Not A Number).
INPUTS:  const double* array  The array of values to check.
         size_t        count  The number of values in the array.
RETURNS: bool true if no NaNs found, else false.
******************************************************************************/

bool isNanFree( const double array[], const size_t count ) {
  PRE02( array, count );
  bool result = true;

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    result = ! isNan( array[ index ] );
  }

  POST0( IMPLIES( result,
                  AND2( ! isNan( array[ 0 ] ), ! isNan( array[ count - 1 ]))));
  return result;
}



/******************************************************************************
PURPOSE: isNanFree - Verify that an array contains no NaNs (Not A Number).
INPUTS:  const float* array  The array of values to check.
         size_t       count  The number of values in the array.
RETURNS: bool true if no NaNs found, else false.
******************************************************************************/

bool isNanFree( const float array[], const size_t count ) {
  PRE02( array, count );
  bool result = true;

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    result = ! isNan( array[ index ] );
  }

  POST0( IMPLIES( result,
                  AND2( ! isNan( array[ 0 ] ), ! isNan( array[ count - 1 ]))));
  return result;
}



/******************************************************************************
PURPOSE: allFinite - Are all array values finite?
INPUTS:  const double* array  The array of values to check.
         size_t count         The number of values in the array.
RETURNS: bool true if all values are finite, else false.
******************************************************************************/

bool allFinite( const double array[], const size_t count ) {
  PRE02( array, count );
  bool result = true;

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    result = isFinite( array[ index ] );
  }

  POST0( IMPLIES( result,
                  AND2( isFinite( array[ 0 ] ),
                        isFinite( array[ count - 1 ] ) ) ) );
  return result;
}




/******************************************************************************
PURPOSE: allFinite - Are all array values finite?
INPUTS:  const float* array  The array of values to check.
         size_t count        The number of values in the array.
RETURNS: bool true if all values are finite, else false.
******************************************************************************/

bool allFinite( const float array[], const size_t count ) {
  PRE02( array, count );
  bool result = true;

  for ( size_t index = 0; AND2( result, index < count ); ++index ) {
    result = isFinite( array[ index ] );
  }

  POST0( IMPLIES( result,
                  AND2( isFinite( array[ 0 ] ),
                        isFinite( array[ count - 1 ] ) ) ) );
  return result;
}




/******************************************************************************
PURPOSE: isValidArgs() - Are each of the strings printable and blank-free?
INPUTS:  int argc                        Number of strings to check.
         const char* const argv[ argc ]  Strings to check.
RETURNS: bool true if non-zero, else 0.
******************************************************************************/

bool isValidArgs( int argc, const char* const argv[] ) {
  bool result = 0;

  if ( AND2( IN_RANGE( argc, 1, INT_MAX - 1 ), argv != 0 ) ) {
    int index = 0;

    do {

      if ( OR2( argv[ index ] == 0, argv[ index ][ 0 ] == '\0' ) ) {
        index = argc;
      } else {
        const char* s = argv[ index ];

        do {

          if ( OR2( ! isprint( *s ), isspace( *s ) ) ) {
            index = argc;
          }

          ++s;
        } while ( AND2( *s, index < argc ) );
      }

      ++index;
    } while ( index < argc );

    result = index == argc;
  }

  return result;
}



/******************************************************************************
PURPOSE: indexOfString - Index of string in strings[] or -1 if not present.
INPUTS:  const char* string           String to search for.
         const char* const strings[]  Strings to search.
         size_t count                 Size of strings[].
RETURNS: size_t index of string in strings[], else count if not present.
******************************************************************************/

size_t indexOfString( const char* string, const char* const strings[],
                      size_t count) {

  PRE09( string, *string, strings, strings[ 0 ], *strings[ 0 ],
         count, strings[ count - 1 ], *strings[ count - 1 ],
         allNonZero( strings, count ) );

  size_t result = count;
  size_t index = 0;

  do {

    if ( AND2( strings[ index ], ! strcmp( string, strings[ index ] ) ) ) {
      result = index;
      index = count - 1;
    }

    ++index;
  } while ( index < count );

  POST0( OR2( result == count,
              AND2( result < count,
                    ! strcmp( string, strings[ result ] ) ) ) );
  return result;
}



/******************************************************************************
PURPOSE: wordCount - Count number of whitespace-separated printable words
         in a string.
INPUTS:  const char* string  String to count.
RETURNS: size_t number of words in string.
******************************************************************************/

size_t wordCount( const char* string ) {
  PRE0( string );
  const char* s = string;
  size_t result = 0;

  while ( *s ) {

    while ( isspace( *s ) ) {
      ++s;
    }

    result += isprint( *s ) != 0;

    while ( *s && ! isspace( *s ) ) {
      ++s;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: wordCount - Count lines in a string.
INPUTS:  const char* string  String to count.
RETURNS: size_t number of lines ('\n' characters) in string.
******************************************************************************/

size_t lineCount( const char* string ) {
  PRE0( string );
  size_t result = 0;

  for ( const char* s = string; *s; ++s ) {
    result += *s == '\n';
  }

  return result;
}



/******************************************************************************
PURPOSE: isText - Is string non-empty and only contains isprint() characters?
INPUTS:  const char* string  String to check.
RETURNS: bool true if each character in string isprint(), else 0.
******************************************************************************/

bool isText( const char* string ) {
  PRE0( string );
  bool result = *string != '\0';

  for ( ; AND2( *string, result ); ++string ) {
    result = OR2( isprint( *string ), isspace( *string ) );
  }

  return result;
}



/******************************************************************************
PURPOSE: lowercase - Convert a string to lowercase.
INPUTS:  char string[]  String to convert.
OUTPUTS: char string[]  Lowercase string.
******************************************************************************/

void lowercase( char string[] ) {
  PRE0( string );

  for ( char* s = string; *s; ++s ) {
    *s = tolower( *s );
  }

  POST0( IMPLIES( *string, ! isupper( *string ) ) );
}



/******************************************************************************
PURPOSE: eraseDoubleQuotedCommas - Change commas to space in each double-quoted
         portion of string.
INPUTS:  char string[]  String to convert.
OUTPUTS: char string[]  Converted string.
******************************************************************************/

void eraseDoubleQuotedCommas( char string[] ) {
  PRE0( string );
  bool quoted = false;

  for ( char* s = string; *s; ++s ) {

    if ( *s == '"' ) {
      quoted = ! quoted;
    } else if ( *s == ',' ) {

      if ( quoted ) {
        *s = ' ';
      }
    }
  }
}



/******************************************************************************
PURPOSE: substituteWord - Change occurrences of oldWord to newWord in input.
INPUTS:  const char* input    String to read.
         const char* oldWord  Word to search for.
         const char* newWord  Word to substitute.
OUTPUTS: char* output         Resulting string.
******************************************************************************/

void substituteWord( const char* input,
                     const char* oldWord,
                     const char* newWord,
                     char* output ) {
  PRE06( input, oldWord, *oldWord, newWord, output, input != output );
  const size_t length = strlen( oldWord );
  const char* i = input;
  char* o = output;

  do {
    const char* const p = strstr( i, oldWord ); /* Point to oldWord in input:*/

    if ( p ) {
      const char* n = newWord;

      while ( i < p ) { /* Copy input up to oldWord: */
        *o++ = *i++;
      }

      i += length; /* Skip oldWord. */

      while ( *n ) { /* Copy newWord: */
        *o++ = *n++;
      }
    } else { /* Copy remainder of input: */

      while ( *i ) {
        *o++ = *i++;
      }
    }
  } while ( *i );

  *o = '\0';
  POST0( IMPLIES( strcmp( oldWord, newWord ), ! strstr( output, oldWord ) ) );
}



/******************************************************************************
PURPOSE: fileExists - Determine named file exists.
INPUTS:  const char* name  Name of file to examine.
RETURNS: bool truw if the named file exists, else false.
******************************************************************************/

bool fileExists( const char* name ) {
  PRE02( name, *name );
  struct stat unused;
  const bool result = stat( name, &unused ) != -1;
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
PURPOSE: copyFileBytes - Copy all of a file's bytes to another file.
INPUTS:  FILE* inputFile   File to copy.
         FILE* outputFile  Created file.
RETURNS: bool true if successful, else false.
******************************************************************************/

bool copyFileBytes( FILE* inputFile, FILE* outputFile ) {

  PRE03( inputFile, outputFile, inputFile != outputFile );

  bool result = false;
  enum { BUFFER_SIZE = 16384 };
  char buffer[ BUFFER_SIZE ] = "";
  size_t bytesRead = 0;

  do {
    bytesRead = fread( buffer, 1, BUFFER_SIZE, inputFile );

    if ( bytesRead ) {
      const size_t bytesWritten = fwrite( buffer, 1, bytesRead, outputFile );
      result = bytesWritten == bytesRead;
    }

  } while ( AND2( bytesRead, result ) );

  if ( ! result ) {
    fprintf( stderr, "Failed to copy all bytes to output file." );
    perror( "because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: copyFile - Copy a file.
INPUTS:  const char* inputFileName   Pathed name of file to copy.
         const char* outputFileName  Created file.
RETURNS: bool true if successful, else false.
******************************************************************************/

bool copyFile( const char* inputFileName, const char* outputFileName ) {
  PRE04( inputFileName, *inputFileName, outputFileName, *outputFileName );
  bool result = true;

  if ( strcmp( inputFileName, outputFileName ) ) {
    FILE* inputFile = fopen( inputFileName, "rb" );
    FILE* outputFile = inputFile ? fopen( outputFileName, "wb" ) : 0;
    result = false;

    if ( outputFile ) {
      result = copyFileBytes( inputFile, outputFile );
    }

    if ( inputFile ) {
      fclose( inputFile ), inputFile = 0;
    }

    if ( outputFile ) {
      fclose( outputFile ), outputFile = 0;
    }

    if ( ! result ) {
      fprintf( stderr, "\nFailed to copy file '%s' to '%s'.\n",
               inputFileName , outputFileName );
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: copyFileBytes - Copy all of a file's bytes to another file.
INPUTS:  FILE* inputFile             File to copy.
         const char* outputFileName  Created file.
RETURNS: bool true if successful, else false.
******************************************************************************/

bool copyFileBytes( FILE* inputFile, const char* outputFileName ) {

  PRE03( inputFile, outputFileName, *outputFileName );

  bool result = false;
  FILE* outputFile = fopen( outputFileName, "wb" );

  if ( outputFile ) {
    result = copyFileBytes( inputFile, outputFile );
    fclose( outputFile ), outputFile = 0;
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to copy all bytes to output file %s ",
             outputFileName );
    perror( "because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: copyFileBytes - Copy some of file's bytes to another file.
INPUTS:  FILE* inputFile             File to copy.
         const char* outputFileName  Created file.
         size_t bytes                Bytes to read and write.
RETURNS: bool true if successful, else false.
******************************************************************************/

bool copyFileBytes( FILE* inputFile, const char* outputFileName,
                    const size_t bytes) {

  PRE04( inputFile, outputFileName, *outputFileName, bytes );

  bool result = false;
  FILE* outputFile = fopen( outputFileName, "wb" );

  if ( outputFile ) {
    enum { BUFFER_SIZE = 16384 };
    char buffer[ BUFFER_SIZE ] = "";
    size_t bytesRemaining = bytes;

    do {
      const size_t bytesToRead =
        bytesRemaining < BUFFER_SIZE ? bytesRemaining : BUFFER_SIZE;
      const size_t bytesRead = fread( buffer, 1, bytesToRead, inputFile );

      if ( bytesRead ) {
        const size_t bytesWritten = fwrite( buffer, 1, bytesRead, outputFile );
        result = bytesWritten == bytesRead;
        bytesRemaining -= bytesWritten;
      } else {
        result = false;
      }

    } while ( AND2( result, bytesRemaining ) );

    fclose( outputFile ), outputFile = 0;
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to copy %lu bytes to output file %s ",
             bytes, outputFileName );
    perror( "because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: streamFile - Stream bytes of a file to stdout.
INPUTS:  const char* fileName  Name of file to stream.
RETURNS: bool true if successful, else false.
******************************************************************************/

bool streamFile( const char* fileName ) {
  PRE02( fileName, *fileName );
  bool result = false;
  FILE* file = fopen( fileName, "rb" );

  if ( file ) {
    result = copyFileBytes( file, stdout );
    fclose( file ), file = 0;
  }

  return result;
}



/******************************************************************************
PURPOSE: streamFile - Stream bytes of named file to given stream.
INPUTS:  const char* fileName  Name of file to stream.
OUTPUTS: FILE* stream          Stream to write to.
RETURNS: bool true if successful, else false.
******************************************************************************/

bool streamFile( const char* fileName, FILE* stream ) {
  PRE03( fileName, *fileName, stream );
  bool result = false;
  FILE* file = fopen( fileName, "rb" );

  if ( file ) {
    result = copyFileBytes( file, stream );
    fclose( file );
    file = 0;
  }

  return result;
}



/******************************************************************************
PURPOSE: removeAllFiles - Remove all files (but not sub-directories) in a
         directory.
INPUTS:  const char* directoryName  Name of directory to remove files from.
         const char* extensions  Optional: file extensions to remove.
                                 E.g., "xdr nc" only removes .xdr and .nc files
                                 If extensions is 0 then all files are removed.
******************************************************************************/

void removeAllFiles( const char* const directoryName,
                     const char* const extensions ) {
  PRE0( isDirectory( directoryName ) );
  DIR* directory = opendir( directoryName );

  if ( directory ) {
    struct dirent* entry = 0;

    do {
      entry = readdir( directory );

      if ( AND3( entry, entry->d_name, entry->d_name[ 0 ] != '.' ) ) {
        const char* const dot = strrchr( entry->d_name, '.' );
        const char* const extension = dot ? dot + 1 : "";
        enum { LENGTH = 255 };
        char pathedName[ LENGTH + 1 ] = "";
        memset( pathedName, 0, sizeof pathedName );
        snprintf( pathedName, sizeof pathedName / sizeof *pathedName,
                  "%s/%s", directoryName, entry->d_name );

        if ( OR2( extensions == 0, strstr( extensions, extension ) ) ) {
          unlink( pathedName );
        }
      }

    } while ( entry );

    closedir( directory ), directory = 0;
  }
}



/******************************************************************************
PURPOSE: removeDirectory - Remove a directory and its contents.
INPUTS:  const char* directoryName  Name of directory to remove.
******************************************************************************/

void removeDirectory( const char* const directoryName ) {
  PRE0( isDirectory( directoryName ) );
  removeAllFiles( directoryName, 0 ); // But not sub-directories or . or ..
  DIR* directory = opendir( directoryName );

  if ( directory ) {
    struct dirent* entry = 0;

    do {
      entry = readdir( directory );

      if ( AND3( entry, entry->d_name, entry->d_name[ 0 ] != '.' ) ) {
        enum { LENGTH = 1023 };
        char subDirectoryName[ LENGTH + 1 ] = "";
        memset( subDirectoryName, 0, sizeof subDirectoryName );
        snprintf( subDirectoryName,
                  sizeof subDirectoryName / sizeof *subDirectoryName,
                  "%s/%s", directoryName, entry->d_name );

        if ( isDirectory( subDirectoryName ) ) {
          removeDirectory( subDirectoryName ); // Recurse on sub-directory.
        }
      }

    } while ( entry );

    closedir( directory ), directory = 0;
    rmdir( directoryName ); // Remove directory not containing sub-directories.
  }
}



/******************************************************************************
PURPOSE: isDirectory - Is the given name a directory?
INPUTS:  const char* const name  Name of directory.
RETURNS: bool true if name is a directory, else false.
******************************************************************************/

bool isDirectory( const char* const name ) {
  bool result = AND3( name, *name, isprint( *name ) );

  if ( result ) {
    DIR* dir = opendir( name );
    result = dir != 0;

    if ( dir ) {
      closedir( dir ), dir = 0;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: isDirectory - Is the name a valid directory?
INPUTS:  const std::string& name  Name to check.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool isDirectory( const std::string& name ) {
  return isDirectory( name.c_str() );
}



/******************************************************************************
PURPOSE: trimSlashes - Trim extraneous slashes from directory path.
INPUTS:  std::string& directory  Directory to trim.
OUTPUTS: std::string& directory  Trimmed dirtectory.
RETURNS: char slash character of directory: '/' or '\\.
******************************************************************************/

char trimSlashes( std::string& directory ) {
  PRE0( ! directory.empty() );
  const bool isWindows =
    AND5( directory.length() >= 3,
          directory[ 0 ] >= 'A',
          directory[ 0 ] <= 'Z',
          directory[ 1 ] == ':',
          directory[ 2 ] == '\\' );
  const char directorySlash = isWindows ? '\\' : '/';
  const size_t start = isWindows ? 2 : 0;
  size_t index = directory.length() - 1;

  // Trim any ending slashes:

  while ( AND2( index > start, directory[ index ] == directorySlash ) ) {
    directory.erase( index );
    --index;
  }

  return directorySlash;
}



/******************************************************************************
PURPOSE: parentDirectory - Get path to parent directory.
INPUTS:  const std::string& directory  Directory to traverse up.
RETURNS: std::string parent  Path of parent directory.
******************************************************************************/

std::string parentDirectory( const std::string& directory ) {
  PRE0( ! directory.empty() );
  std::string result( directory );
  const char directorySlash = trimSlashes( result );
  const size_t start = directorySlash == '\\' ? 2 : 0;
  size_t index = directory.length() - 1;

  // Trim ending non-slash characters:

  while ( AND2( index > start, result[ index ] != directorySlash ) ) {
    result.erase( index );
    --index;
  }

  trimSlashes( result ); // Trim final slash.
  return result;
}



/******************************************************************************
PURPOSE: appendPath - Append subdirectory to directory path.
INPUTS:  const std::string& directory     Directory to append to.
         const std::string& subdirectory  Subdirectory to append.
RETURNS: std::string parent  Path of appended subdirectory.
******************************************************************************/

std::string appendPath( const std::string& directory,
                        const std::string& subdirectory ) {
  PRE04( ! directory.empty(),
         ! subdirectory.empty(),
         subdirectory.find( '/'  ) == std::string::npos,
         subdirectory.find( '\\' ) == std::string::npos );
  std::string result( directory );
  const char directorySlash = trimSlashes( result );

  // Append slash (unless at top) then subdirectory name:

  if ( IMPLIES_ELSE( directorySlash == '/',
                     directory.length() > 1,
                     directory.length() > 3 )  ) {
    result += directorySlash;
  }

  result += subdirectory;

  POST0( result.rfind( subdirectory ) ==
         result.length() - subdirectory.length() );
  return result;
}



/******************************************************************************
PURPOSE: getHomeDirectory - Home directory.
RETURNS: std::string Name of user's home directory. Examples:
         "/Users/plessel"
         "C:\Documents and Settings\tplessel"
         "."
******************************************************************************/

std::string getHomeDirectory() {
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
    const size_t length = strlen( result );

    if ( result[ length - 1 ] == '\\' ) {
      result[ length - 1 ] = '\0';
    }

#else
    const char* const home0 = getenv( "HOME" );
    const char* const home = home0 ? home0 : ".";
    memset( result, 0, sizeof result );
    strncpy( result, home, sizeof result / sizeof *result - 1 );
#endif
  }

  DEBUG( fprintf( stderr, "homeDirectory = '%s'\n", result ); )
  POST02( *result, result[ strlen( result ) - 1 ] != slash );
  return std::string( result );
}



/******************************************************************************
PURPOSE: isNetCDFFile - Is file large enough to be a CMAQ file, not on tape
         and starts with 'CDF1', 'CDF2' or 211HDF'?
INPUTS:  const std::string& name  Name to check.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool isNetCDFFile( const std::string& name ) {
  bool result = false;

  if ( ! name.empty() ) {
    struct stat buffer;
    memset( &buffer, 0, sizeof buffer );

    if ( stat( name.c_str(), &buffer ) == 0 ) {

      if ( buffer.st_size > 10000 ) { /* Minimal CMAQ file header size. */
#ifdef _WIN32
        const bool onTape = false;
#else
        const bool onTape = buffer.st_blocks == 0;
#endif

        if ( onTape ) { /* 0 blocks means on tape */
          result = true; /* which is too slow to demigrate/read/check so allow.*/
        } else { /* Read and check first 4 bytes of file: */
          FILE* file = fopen( name.c_str(), "rb" );

          if ( file ) {
            unsigned char bytes[ 5 ] = "";
            const bool ok = fread( &bytes, 4, 1, file ) == 1;

            if ( ok ) {
              result =
                OR2( AND4( bytes[ 0 ] == 'C',
                           bytes[ 1 ] == 'D',
                           bytes[ 2 ] == 'F',
                           IN3( bytes[ 3 ], 1, 2 ) ),
                    AND4( bytes[ 0 ] == 0211,
                          bytes[ 1 ] == 'H',
                          bytes[ 2 ] == 'D',
                          bytes[ 3 ] == 'F' ) );
            }

            fclose( file ), file = 0;
          }
        }
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: readCommandResult - Execute command and read result into string.
INPUTS:   const std::string command  Command to execute.
OUTPUTS:  std::string& result        Result read from command.
RETURNS:  bool true if successful, else false.
******************************************************************************/

bool readCommandResult( const std::string& command, std::string& result ) {
  PRE0( ! command.empty() );
  const char* const readBinary =
#ifdef _WIN32
    "rb";
#else
    "r";
#endif

  FILE* pipe = popen( command.c_str(), readBinary );

  result.clear();

  if ( pipe ) {
    enum { BUFFER_SIZE = 256 * 1024 };
    char buffer[ BUFFER_SIZE ] = "";

    do {
      const size_t bytes_read = fread( buffer, 1, BUFFER_SIZE, pipe );

      if ( bytes_read ) {

        // Filter-out any '\r' characters:

        for ( size_t index = 0; index < bytes_read; ++index ) {
          const char ch = buffer[ index ];

          if ( ch != '\r' ) {
            result += ch;
          }
        }
      }

    } while ( ! feof( pipe ) );

    trimEndingWhitespace( result );
    pclose( pipe ), pipe = 0;
  }

  const bool ok = result.length() > 0;
  DEBUG( fprintf( stderr, "readCommandResult( %s ), result = '%s', ok = %d\n",
                 command.c_str(), result.length() ? result.c_str() : "", ok );)
  return ok;
}




/******************************************************************************
PURPOSE: readCommandResult - Execute command and copy output to stream.
INPUTS:   const std::string& command  Command to execute.
OUTPUTS:  FILE* result                Stream to write command output.
RETURNS:  bool true if successful, else false.
******************************************************************************/

bool readCommandResult( const std::string& command, FILE* result ) {
  PRE0( ! command.empty() );
  bool ok = false;
  const char* const readBinary =
#ifdef _WIN32
    "rb";
#else
    "r";
#endif

  FILE* pipe = popen( command.c_str(), readBinary );

  if ( pipe ) {
    enum { BUFFER_SIZE = 256 * 1024 };
    char buffer[ BUFFER_SIZE ] = "";

    do {
      const size_t bytes_read = fread( buffer, 1, BUFFER_SIZE, pipe );

      if ( bytes_read ) {
        ok = fwrite( buffer, bytes_read, 1, result ) == 1;
      }

    } while ( AND2( ok, ! feof( pipe ) ) );

    pclose( pipe ), pipe = 0;
  }

  return ok;
}



/******************************************************************************
PURPOSE: trimEndingWhitespace - Trim whitespace from the end of a string.
INPUTS:   std::string& s  String to trim.
OUTPUTS:  std::string& s  Trimmed string.
******************************************************************************/

void trimEndingWhitespace( std::string& s ) {
  const size_t length = s.length();
  size_t index = length;
  size_t erase = 0;

  while ( index ) {
    --index;

    if ( isspace( s[ index ] ) ) {
      s[ index ] = '\0'; // Must overwrite characters!
    } else {
      erase = index + 1;
      index = 0; // Stop looping.
    }
  }

  if ( erase < length ) {
    s.erase( erase );
  }
}



/******************************************************************************
PURPOSE: directoryListing - List files in a directory that end with one of
         the given extensions and are dated today.
INPUTS:  const char* directory   Name of directory.
         const char* extensions  Optional: file extensions to filter by.
                                 E.g., "shx shp txt".
         size_t size             Size of buffer[].
OUTPUTS: char buffer[ size ]     Result containing formatted columns:
                                 bytes hh:mm name
                                 sorted by file name.
******************************************************************************/

void directoryListing( const char* directory, const char* extensions,
                       size_t size, char buffer[] ) {
  PRE03( isDirectory( directory ), size, buffer );
  size_t remaining = size - 1;
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

        if ( AND3( fileName,
                   strcmp( fileName, "." ), strcmp( fileName, ".." ) ) ) {
          const char* const extension = strrchr( fileName, '.' );

          if ( IMPLIES( extensions, AND2( extension, extension[ 1 ] ) ) ) {

            if ( IMPLIES( extensions, strstr( extensions, extension + 1 ) ) ) {
              struct stat info;
              memset( &info, 0, sizeof info );
              snprintf( pathedFileName, NAME_LENGTH, "%s/%s",
                        directory, fileName );

              if ( ! stat( pathedFileName, &info ) ) {
                const time_t seconds = info.st_mtime;
                const struct tm* const fileTimeInfo = localtime( &seconds );
                const int year  = fileTimeInfo->tm_year + 1900;
                const int month = fileTimeInfo->tm_mon + 1;
                const int day   = fileTimeInfo->tm_mday;

                if ( AND3( year == todayYear, month == todayMonth,
                           day == todayDay ) ) {
                  const int hh = fileTimeInfo->tm_hour;
                  const int mm = fileTimeInfo->tm_min;
                  const size_t fileSize = info.st_size;
                  enum { LINE_LENGTH = 255 };
                  char line[ LINE_LENGTH + 1 ] = "";
                  size_t lineLength = 0;
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
    }

    closedir( dir ), dir = 0;
  }
}



/******************************************************************************
PURPOSE: elevationsAtSigmaPressures - Compute elevations in meters above mean
         sea-level at sigma-pressures.
INPUTS:  const double g                Gravitational force, e.g., 9.81 m/s^2.
         const double R                Gas constant. 287.04 J/kg/K = m^3/s/K.
         const double A                Atmospheric lapse rate, e.g., 50.0 K/kg.
         const double T0s              Reference surface temperature, 2900 K.
         const double P00              Reference surface pressure, 100000 Pa.
         const double surfaceElevation Elevation of surface in meters AMSL.
         const int levels              Number of levels of sigmaPressures.
         const double topPressure      Pressure in Pa at the top of the model.
         const double sigmaPressures[ levels ]  Sigma-pressures at levels.
OUTPUTS: double elevations[ levels ]  Elevation in meters above MSL at sigmas.
NOTES:   Based on formula used in MM5.
******************************************************************************/

void elevationsAtSigmaPressures( const double g,
                                 const double R,
                                 const double A,
                                 const double T0s,
                                 const double P00,
                                 const double surfaceElevation,
                                 const int levels,
                                 const double topPressure,
                                 const double sigmaPressures[],
                                 double elevations[] ) {

  PRE014( ! isNan( g ),
          ! isNan( R ),
          ! isNan( A ),
          ! isNan( T0s ),
          ! isNan( P00 ),
          ! isNan( surfaceElevation ),
          surfaceElevation > -1000.0,
          levels > 0,
          ! isNan( topPressure ),
          GT_ZERO6( topPressure, g, R, A, T0s, P00 ),
          isNanFree( sigmaPressures, levels ),
          minimum( sigmaPressures, levels ) >= 0.0,
          maximum( sigmaPressures, levels ) <= 1.0,
          elevations );

  /* Derived constants: */

  const double H0s            = R * T0s / g;
  const double one_over_H0s   = 1.0 / H0s;
  const double A_over_T0s     = A / T0s;
  const double A_over_two_T0s = A / ( T0s + T0s );
  const double Pt             = topPressure;
  const double Zs             = surfaceElevation;
  const double two_Zs         = Zs + Zs;
  const double sqrt_factor    = sqrt(1.0 - A_over_T0s * one_over_H0s * two_Zs);
  const double q_factor =
   ( Pt / P00 ) * exp( two_Zs * one_over_H0s / sqrt_factor );
  int level = 0;

  /* Compute elevations at sigma-pressures: */

  for ( level = 0; level < levels; ++level ) {
    const double sigma_p0   = sigmaPressures[ level ];
    const double q0_star    = sigma_p0 + ( 1.0 - sigma_p0 ) * q_factor;
    const double ln_q0_star = log( q0_star );
    const double z_level    =
      Zs - H0s * ln_q0_star * ( A_over_two_T0s * ln_q0_star + sqrt_factor );
    elevations[ level ]     = z_level;
  }

  POST03( isNanFree( elevations, levels ),
          minimum( elevations, levels ) >= -1000.0,
          maximum( elevations, levels ) <= 1e6 );

}



} // Namespace emvl.




