/******************************************************************************
PURPOSE: Timestamp.C - Calendar UTC date-timestamps.
NOTES:
HISTORY: 2012-11-20 plessel.todd@epa.gov
******************************************************************************/


//================================ INCLUDES ===================================

#include <stdio.h>    // For snprintf().
#include <stdlib.h>   // For abs()
#include <string.h>   // For strlen(), memset(), memcpy().
#include <limits.h>   // For LLONG_MAX.
#include <time.h>     // For time(), gmtime_r(), timegm().
#include <sys/stat.h> // For stat().

#ifdef _WIN32
/*
 * gmtime_r() is missing, just take a chance on using gmtime().
 * According to this reference there is no actual race-condition:
 * https://sourceforge.net/p/mingw-w64/mailman/message/21523522/
 * "Please note that plain old gmtime() and localtime() *are* thread-safe
 * in Microsoft's C library. They use a thread-local buffer."
 */
static struct tm* GMTIME_R( const time_t* clock, struct tm* result ) {
  const struct tm* const copy = gmtime( clock ); /* HACK: not thread-safe! */
  memcpy( result, copy, sizeof (struct tm) );
  return result;
}
#else
#define GMTIME_R gmtime_r /* Use available gmtime_r(). */
#endif


/*
 * The non-standard routine timegm(), when available, is either not thread-safe
 * or is thread-safe but employs mutex-locking that is too slow or even hangs!
 */

static time_t my_timegm( struct tm *t ) {
  time_t result = mktime( t );

  /* Compute seconds from 1970-01-01T00:00:00-0000 to get timezone offset: */

  time_t timezone_offset_seconds = 0;
  struct tm t0;
  memset( &t0, 0, sizeof t0 );
  t0.tm_mday = 1;
  t0.tm_year = 70;
  t0.tm_wday = 4; /* January 1, 1970 was Thu. Sun = 0, Mon = 1, ... Thu = 4. */
  timezone_offset_seconds = mktime( &t0 );

  result -= timezone_offset_seconds;
  return result;
}


#include <Assertions.h>    // For PRE0*(), POST0*(), AND?().
#include <DataUtilities.h> // For isNan().
#include <Timestamp.h>     // For public interface.

//============================= PRIVATE VARIABLES =============================


// 30 days hath September, April, June and November, all the rest have 31,
// except February which has either 28 or 29 (on a leap year).

static const int daysPerMonth[ 2 ][ 12 ] = {
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, // Non-leap year.
  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }  // Leap year.
};


//============================= PUBLIC FUNCTIONS ==============================



namespace emvl {



/******************************************************************************
PURPOSE: ~Timestamp - Destructor.
******************************************************************************/

Timestamp::~Timestamp() {
  PRE( true );
  seconds_ = 0;
}



/******************************************************************************
PURPOSE: Timestamp - Contruct with current UTC date-time.
******************************************************************************/

Timestamp::Timestamp() {
  seconds_ = (long long) time( 0 );
  POST( true );
}



/******************************************************************************
PURPOSE: yyyymmddhhmmss - Construct with seconds.
INPUTS:  const long long seconds
******************************************************************************/

Timestamp::Timestamp( const long long seconds ) {
  seconds_ = seconds;
  POST( this->seconds() == seconds );
}



/******************************************************************************
PURPOSE: yyyymmddhhmmss - Construct with yyyymmdd and hhmmss.
INPUTS:  const int yyyymmdd  Date.
         const int hhmmss    Time.
******************************************************************************/

Timestamp::Timestamp( const int yyyymmdd, const int hhmmss ) {
  PRE02( isYYYYMMDD( yyyymmdd ), isHHMMSS( hhmmss ) );
  seconds_ = (long long) time( 0 );
  yyyymmddhhmmss( yyyymmdd, hhmmss );
  POST2( this->yyyymmdd() == yyyymmdd, this->hhmmss() == hhmmss );
}



/******************************************************************************
PURPOSE: Timestamp - Construct a timestamp from a UTCString.
INPUTS:  const UTCString value  Timestamp to initialize to.
******************************************************************************/

Timestamp::Timestamp( const UTCString value ) {
  PRE0( isUTCString( value ) );
  seconds_ = (long long) time( 0 );
  fromUTCString( value );
  DEBUG( fprintf( stderr, "Timestamp::Timestamp( %s ) = %" LLD "\n",
                  value, seconds_ ); )
  POST( true );
}



/******************************************************************************
PURPOSE: Timestamp - Copy construct a timestamp.
INPUTS:  const Timestamp& other  Object to copy.
******************************************************************************/

Timestamp::Timestamp( const Timestamp& other ) {
  PRE( other.invariant() );
  seconds_ = other.seconds_;
  POST( *this == other );
}



/******************************************************************************
PURPOSE: operator= - Copy a timestamp.
INPUTS:  const Timestamp& other  Object to copy.
******************************************************************************/

Timestamp& Timestamp::operator=( const Timestamp& other ) {
  PRE( other.invariant() );

  if ( this != &other ) {
    seconds_ = other.seconds_;
  }

  POST( *this == other );
  return *this;
}



/******************************************************************************
PURPOSE: fromScanStartTime - Contruct from a NASA MODIS/CALIPSO scan start time
         TAI seconds since 1993-01-01T00:00:00Z).
INPUTS:  const double value  Scan start time value.
******************************************************************************/

void Timestamp::fromScanStartTime( const double value ) {
  PRE2( ! isNan( value ), IN_RANGE( value, 0.0, LLONG_MAX ) );

  // Compute approximate seconds from UNIX time: 1970-01-01T00:00:00Z
  // to TAI Scan Start Time base 1993-01-01T00:00:00Z
  // including 6 leap years: 1972, 1976, 1980, 1984, 1988, 1992 and
  // 17 leap seconds. See http://en.wikipedia.org/wiki/Leap_second

  const int daysFrom1970To1993 = 8401; // Includes 6 leap years.
  const int leapSecondsFrom1970To1993 = 17;
  const int secondsDifferenceUTC2TAI = -10;
  const int offset = -13; // HACK to match MODIS/CALIPSO UTC timestamp.
  const int hoursPerDay = 24;
  const int minutesPerHour = 60;
  const int secondsPerMinute = 60;
  seconds_ =
    daysFrom1970To1993 * hoursPerDay * minutesPerHour * secondsPerMinute +
    leapSecondsFrom1970To1993 + secondsDifferenceUTC2TAI + offset;

  // Add TAI scan start time seconds after 1993-01-01T00:00:00Z
  // which includes leap seconds:

  seconds_ += (long long) ( value + 0.5 );
  POST( true );
}



/******************************************************************************
PURPOSE: fromUTCString - Contruct from a UTC string.
INPUTS:  const UTCString value  UTC string value.
******************************************************************************/

void Timestamp::fromUTCString( const UTCString value ) {
  PRE( isUTCString( value ) );
  int yyyy = 1900;
  int mo = 1;
  int dd = 1;
  int hh = 0;
  int mm = 0;
  int ss = 0;
  char s = 0;
  int zone = 0;
  const int parts =
    sscanf( value, "%04d-%02d-%02dT%02d:%02d:%02d%c%04d",
            &yyyy, &mo, &dd, &hh, &mm, &ss, &s, &zone );

  if ( parts == 8 ) {
    const int ddd = dayOfYear( yyyy, mo, dd );
    const int dw  = dayOfWeek( yyyy, mo, dd );
    struct tm t;
    memset( &t, 0, sizeof t );
    t.tm_sec = ss;
    t.tm_min = mm;
    t.tm_hour = hh;
    t.tm_mday = dd;
    t.tm_mon = mo - 1;
    t.tm_year = yyyy - 1900;
    t.tm_wday = dw;
    t.tm_yday = ddd - 1;
    t.tm_isdst = 0;
#if ! defined( _WIN32 ) && ! defined( __CYGWIN__ )
    char utc[ 4 ] = "UTC";
    t.tm_zone = utc;
    t.tm_gmtoff = 0;
#endif
    seconds_ = (long long) my_timegm( &t );

    if ( zone ) {
      const int sign = s == '-' ? -1 : 1;
      const int hours   = zone / 100;
      const int minutes = zone % 100;
      const int secondsPerMinute = 60;
      const int minutesPerHour = 60;
      const int secondsPerHour = secondsPerMinute * minutesPerHour;
      const int seconds = hours * secondsPerHour + minutes * secondsPerMinute;
      seconds_ += sign * seconds;
    }
  }

  POST( true );
}



/******************************************************************************
PURPOSE: yyyydddhhmmss - Set date to year and day of year plus time.
INPUTS:  const int yyyyddd  Year and day of year.
         const int hhmmss   Time.
******************************************************************************/

void Timestamp::yyyydddhhmmss( const int yyyyddd, const int hhmmss ) {
  PRE2( isYYYYDDD( yyyyddd ), isHHMMSS( hhmmss ) );
  const int yyyy = yyyyddd / 1000;
  const int ddd  = yyyyddd % 1000;
  const int hh   = hhmmss  / 10000;
  const int mm   = hhmmss  / 100 % 100;
  const int ss   = hhmmss  % 100;
  const int mo   = monthOfYear( yyyy, ddd );
  const int dd   = dayOfMonth( yyyy, ddd );
  const int dw   = dayOfWeek( yyyy, mo, dd );
  struct tm t;
  memset( &t, 0, sizeof t );
  t.tm_sec = ss;
  t.tm_min = mm;
  t.tm_hour = hh;
  t.tm_mday = dd;
  t.tm_mon = mo - 1;
  t.tm_year = yyyy - 1900;
  t.tm_wday = dw;
  t.tm_yday = ddd - 1;
  t.tm_isdst = 0;
#if ! defined( _WIN32 ) && ! defined( __CYGWIN__ )
  char utc[ 4 ] = "UTC";
  t.tm_zone = utc;
  t.tm_gmtoff = 0;
#endif
  seconds_ = (long long) my_timegm( &t );
  POST2( this->yyyyddd() == yyyyddd, this->hhmmss() == hhmmss );
}



/******************************************************************************
PURPOSE: yyyymmddhhmmss - Set to date and time.
INPUTS:  const int yyyymmdd  Date.
         const int hhmmss    Time.
******************************************************************************/

void Timestamp::yyyymmddhhmmss( const int yyyymmdd, const int hhmmss ) {
  PRE2( isYYYYMMDD( yyyymmdd ), isHHMMSS( hhmmss ) );
  const int yyyy = yyyymmdd / 10000;
  const int mo   = yyyymmdd / 100 % 100;
  const int dd   = yyyymmdd % 100;
  const int ddd  = dayOfYear( yyyy, mo, dd );
  const int yyyyddd = yyyy * 1000 + ddd;
  yyyydddhhmmss( yyyyddd, hhmmss );
  POST2( this->yyyymmdd() == yyyymmdd, this->hhmmss() == hhmmss );
}



/******************************************************************************
PURPOSE: offsetHours - Offset by hours.
INPUTS:  const int hours  Hours to offset.
******************************************************************************/

void Timestamp::offsetHours( const int hours ) {
  PRE( true );
  const int secondsPerMinute = 60;
  const int minutesPerHour = 60;
  const long long secondsPerHour = secondsPerMinute * minutesPerHour;
  const long long seconds = hours * secondsPerHour;
  seconds_ += seconds;
  POST( true );
}



/******************************************************************************
PURPOSE: offsetMonths - Offset by months.
INPUTS:  const int months  Months to offset.
******************************************************************************/

void Timestamp::offsetMonths( const int months ) {
  PRE( true );
  const int hhmmss = this->hhmmss();
  int yyyymmdd = this->yyyymmdd();
  int dd       = yyyymmdd % 100;
  int yyyymm   = yyyymmdd / 100;
  int yyyy     = yyyymm / 100;
  int mm       = yyyymm % 100;

  mm += months;

  if ( mm > 12 ) {
    yyyy += mm / 12;
    mm %= 12;
  } else if ( mm < 1 ) {
    yyyy += mm / 12 - 1;
    mm = 12 - ( -mm % 12 );
  }

  const int monthDays =
    daysPerMonth[ daysInYear( yyyy ) == 366 ? 1 : 0 ][ mm - 1 ];

  if ( dd > monthDays ) {
    dd = monthDays;
  }

  yyyymmdd = yyyy * 10000 + mm * 100 + dd;
  yyyymmddhhmmss( yyyymmdd, hhmmss );
  POST( true );
}



/******************************************************************************
PURPOSE: isYear - Is yyyy valid?
INPUTS:  const int yyyy  Year.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool Timestamp::isYear( const int yyyy ) {
  const bool result = yyyy >= 1800;
  return result;
}



/******************************************************************************
PURPOSE: isYYYYMMDD - Is yyyymmdd valid?
INPUTS:  const int yyyymmdd   Year, month, day.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool Timestamp::isYYYYMMDD( const int yyyymmdd ) {
  const int yyyy = yyyymmdd / 10000;
  const int mm = yyyymmdd / 100 % 100;
  const int dd = yyyymmdd % 100;
  int result = AND2( isYear( yyyy ), IN_RANGE( mm, 1, 12 ) );

  if ( result ) {
    const bool isLeapYear = daysInYear( yyyy ) == 366;
    const int index = isLeapYear ? 1 : 0;
    const int days = daysPerMonth[ index ][ mm - 1 ];
    result = IN_RANGE( dd, 1, days );
  }

  return result;
}



/******************************************************************************
PURPOSE: daysInYear - Days in year 365 or 366.
INPUTS:  const int yyyy  Year.
RETURNS: int days in year.
******************************************************************************/

int Timestamp::daysInYear( const int yyyy ) {
  PRE0( isYear( yyyy ) );
  const bool isLeapYear =
    yyyy % 4 == 0 && ( yyyy % 100 != 0 || yyyy % 400 == 0 );
  const int result = 365 + ( isLeapYear ? 1 : 0 );
  POST0( IN3( result, 365, 366 ) );
  return result;
}



/******************************************************************************
PURPOSE: dayOfYear - Day of year [1, 366].
INPUTS:  const int yyyy  Year.
         const int mm    Month.
         const int dd    Day of month.
RETURNS: int ddd day of year.
******************************************************************************/

int Timestamp::dayOfYear( const int yyyy, const int mm, const int dd ) {
  PRE0( isYYYYMMDD( ( yyyy * 100 + mm ) * 100 + dd ) );
  const int ddd = daysInYear( yyyy );
  const int index = ddd == 366 ? 1 : 0;
  const int mm1 = mm - 1;
  int result = 0;

  for ( int month = 0; month < mm1; ++month ) {
    result += daysPerMonth[ index ][ month ];
  }

  result += dd;
  POST0( IN_RANGE( result, 1, daysInYear( yyyy ) ) );
  return result;
}



/******************************************************************************
PURPOSE: dayOfMonth - Day of month [1, 12].
INPUTS:  const int yyyy  Year.
         const int ddd   Day of year.
RETURNS: int day of month.
******************************************************************************/

int Timestamp::dayOfMonth( const int yyyy, const int ddd ) {
  PRE02( isYear( yyyy ), IN_RANGE( ddd, 1, daysInYear( yyyy ) ) );
  const int index = daysInYear( yyyy ) == 366 ? 1 : 0;
  int dd = ddd;

  for ( int month = 0;
        AND2( month < 12, dd > daysPerMonth[ index ][ month ] );
        ++month ) {
    dd -= daysPerMonth[ index ][ month ];
  }

  const int result = dd;
  POST0( IN_RANGE( result, 1, 31 ) );
  return result;
}



/******************************************************************************
PURPOSE: dayOfWeek - Day of week [Sunday = 1, Saturday = 7].
INPUTS:  const int yyyy  Year.
         const int mm    Month.
         const int dd    Day of month.
RETURNS: int day of week.
NOTES:   int 0 = Sunday, ..., 6 = Saturday.
         See http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
******************************************************************************/

int Timestamp::dayOfWeek( const int yyyy, const int mm, const int dd ) {
  PRE0( isYYYYMMDD( ( yyyy * 100 + mm ) * 100 + dd ) );
  const int m[ 12 ] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
  const int y = mm < 3 ? yyyy - 1 : yyyy;
  const int result = ( y + y / 4 - y / 100 + y / 400 + m[ mm - 1 ] + dd ) % 7;
  POST0( IN_RANGE( result, 0, 6 ) );
  return result;
}



/******************************************************************************
PURPOSE: monthOfYear - Month of year.
INPUTS:  const int yyyy  Year.
         const int ddd   Day of year.
RETURNS: int month of year.
******************************************************************************/

int Timestamp::monthOfYear( const int yyyy, const int ddd ) {
  PRE02( isYear( yyyy ), IN_RANGE( ddd, 1, daysInYear( yyyy ) ) );
  const int index = daysInYear( yyyy ) == 366 ? 1 : 0;
  int days = 0;
  int month = 0;

  for ( month = 0; AND2( month < 12, days < ddd ); ++month ) {
    days += daysPerMonth[ index ][ month ];
  }

  const int result = month;
  POST0( IN_RANGE( result, 1, 12 ) );
  return result;
}



/******************************************************************************
PURPOSE: isUTCString - Is value a valid UTC string?
INPUTS:  const UTCString value  E.g., "2006-07-31T23:59:59-0000".
RETURNS: bool true if valid, else false.
******************************************************************************/

bool Timestamp::isUTCString( const UTCString value ) {
  bool result = false;

  if ( AND2( value, strlen( value ) == UTC_STRING_LENGTH ) ) {
    int yyyy = 0;
    int mo = 0;
    int dd = 0;
    int hh = 0;
    int mm = 0;
    int ss = 0;
    char s = 0;
    int zone = 0;
    const int parts =
      sscanf( value, "%04d-%02d-%02dT%02d:%02d:%02d%c%04d",
              &yyyy, &mo, &dd, &hh, &mm, &ss, &s, &zone );

    if ( parts == 8 ) {

      if ( isYYYYMMDD( ( yyyy * 100 + mo ) * 100 + dd ) ) {
        const int hhmmss = hh * 10000 + mm * 100 + ss;

        if ( isHHMMSS( hhmmss ) ) {

          if ( IN3( s, '-', '+' ) ) {
            const int hours   = zone / 100;
            const int minutes = zone % 100;
            result =
              AND2( IN_RANGE( hours, 0, 23 ), IN_RANGE( minutes, 0, 59 ) );
          }
        }
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: isYYYYDDD - Is argument a valid year and day of year?
INPUTS:  const int yyyyddd  Value to check.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool Timestamp::isYYYYDDD( const int yyyyddd ) {
  const int yyyy = yyyyddd / 1000;
  const int ddd  = yyyyddd % 1000;
  const int days = isYear( yyyy ) ? daysInYear( yyyy ) : 0;
  const bool result = AND2( isYear( yyyy ), IN_RANGE( ddd, 1, days ) );
  return result;
}



/******************************************************************************
PURPOSE: isHHMMSS - Is argument a valid hour minute second?
INPUTS:  const int hhmmss  Value to check.
RETURNS: bool true if valid, else false.
******************************************************************************/

bool Timestamp::isHHMMSS( const int hhmmss ) {
  const int hh = hhmmss / 10000;
  const int mm = hhmmss / 100 % 100;
  const int ss = hhmmss % 100;
  const bool result =
    AND3( IN_RANGE( hh, 0, 23 ), IN_RANGE( mm, 0, 59 ), IN_RANGE( ss, 0, 59 ));
  return result;
}



/******************************************************************************
PURPOSE: daysInMonth - Number of days in given year, month.
INPUTS:  const int yyyy  4-digit year.
         const int mm    1-based 2-digit month.
RETURNS: int number of days.
******************************************************************************/

int Timestamp::daysInMonth( const int yyyy, const int mm ) {
  PRE02( isYear( yyyy ), IN_RANGE( mm, 1, 12 ) );
  const int result =
    daysPerMonth[ daysInYear( yyyy ) == 366 ? 1 : 0 ][ mm - 1 ];
  POST02( IN_RANGE( result, 1, 31 ),
          isYYYYMMDD( ( yyyy * 100 + mm ) * 100 + result ) );
  return result;
}



/******************************************************************************
PURPOSE: fileDateUTC - UTC date of a file.
INPUTS:  const char* fileName  File to check.
RETURNS: int yyyymmdd of file.
******************************************************************************/

int Timestamp::fileDateUTC( const char* fileName ) {
  PRE02( fileName, *fileName );
  int result = 19000101;
  struct stat fileInfo;

  if ( stat( fileName, &fileInfo ) == 0 ) {
    const time_t seconds = fileInfo.st_mtime;
    struct tm timestamp;

    if ( GMTIME_R( &seconds, &timestamp ) ) {
      const int yyyy = timestamp.tm_year + 1900;
      const int mm   = timestamp.tm_mon + 1;
      const int dd   = timestamp.tm_mday;
      const int yyyymmdd = yyyy * 10000 + mm * 100 + dd;

      if ( isYYYYMMDD( yyyymmdd ) ) {
        result = yyyymmdd;
      }
    }
  }

  POST0( isYYYYMMDD( result ) );
  return result;
}



/******************************************************************************
PURPOSE: timeZoneOffset - Hour offset from UTC.
INPUTS:  const char* timeZoneName  E.g., "EST".
RETURNS: int offset from UTC (e.g., -4).
NOTES:   Only implemented for US time zones.
******************************************************************************/

int Timestamp::timeZoneOffset( const char* timeZoneName ) {
  PRE02( timeZoneName, *timeZoneName );
  typedef struct { const char* name; int offset; } TimeZoneEntry;
  static const TimeZoneEntry timeZones[] = {
    { "AST", -4 },
    { "EST", -5 },
    { "EDT", -4 },
    { "CST", -6 },
    { "CDT", -5 },
    { "MST", -7 },
    { "MDT", -6 },
    { "PST", -8 },
    { "PDT", -7 },
    { "AKST", -9 },
    { "AKDT", -8 },
    { "HAST", -10 },
    { "HASDT", -9 },
    { 0, 0 }
  };
  int result = 0;

  for ( int index = 0; AND2( ! result, timeZones[ index ].name ); ++index ) {

    if ( ! strcmp( timeZoneName, timeZones[ index ].name ) ) {
      result = timeZones[ index ].offset;
    }
  }

  POST0( IN_RANGE( result, -10, 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: toUTCString - Convert to UTCString.
OUTPUTS: UTCString result  UTC string value.
******************************************************************************/

void Timestamp::toUTCString( UTCString result ) const {
  PRE0( result );
  strncpy( result, "1900-01-01T00:00:00-0000", UTC_STRING_LENGTH + 1 );
  struct tm t;
  const time_t seconds = (time_t) seconds_;

  if ( GMTIME_R( &seconds, &t ) ) {
    const int yyyy = t.tm_year + 1900;
    const int mo   = t.tm_mon + 1;
    const int dd   = t.tm_mday;
    const int hh   = t.tm_hour;
    const int mm   = t.tm_min;
    const int ss   = t.tm_sec;
    snprintf( result, sizeof (UTCString) / sizeof *result,
              "%04d-%02d-%02dT%02d:%02d:%02d-0000",
              yyyy, mo, dd, hh, mm, ss );
  }

  POST0( isUTCString( result ) );
}



/******************************************************************************
PURPOSE: seconds - Seconds since 1970-01-01T00:00:00Z.
RETURNS: long long seconds since 1970-01-01T00:00:00Z.
******************************************************************************/

long long Timestamp::seconds() const {
  PRE( true );
  const long long result = seconds_;
  POST( true );
  return result;
}



/******************************************************************************
PURPOSE: yyyyddd - Year and day of year.
RETURNS: int year and day of year.
******************************************************************************/

int Timestamp::yyyyddd() const {
  PRE( true );
  int result = 1900001;
  struct tm t;
  const time_t seconds = (time_t) seconds_;

  if ( GMTIME_R( &seconds, &t ) ) {
    const int yyyy = t.tm_year + 1900;
    const int ddd  = t.tm_yday + 1;
    result = yyyy * 1000 + ddd;
  }

  POST( isYYYYDDD( result ) );
  return result;
}



/******************************************************************************
PURPOSE: yyyymmdd - Year, month and day.
RETURNS: int year, month and day.
******************************************************************************/

int Timestamp::yyyymmdd() const {
  PRE( true );
  int result = 19000101;
  struct tm t;
  const time_t seconds = (time_t) seconds_;

  if ( GMTIME_R( &seconds, &t ) ) {
    const int yyyy = t.tm_year + 1900;
    const int mm   = t.tm_mon + 1;
    const int dd   = t.tm_mday;
    result = yyyy * 10000 + mm * 100 + dd;
  }

  POST( isYYYYMMDD( result ) );
  return result;
}



/******************************************************************************
PURPOSE: hhmmss - Hour, minute and second.
RETURNS: int hour, minute and second.
******************************************************************************/

int Timestamp::hhmmss() const {
  PRE( true );
  int result = 0;
  struct tm t;
  const time_t seconds = (time_t) seconds_;

  if ( GMTIME_R( &seconds, &t ) ) {
    const int hh = t.tm_hour;
    const int mm = t.tm_min;
    const int ss = t.tm_sec;
    result = hh * 10000 + mm * 100 + ss;
  }

  POST( isHHMMSS( result ) );
  return result;
}



/******************************************************************************
PURPOSE: startOfHour - Start of hour.
RETURNS: Timestamp at start of hour.
******************************************************************************/

Timestamp Timestamp::startOfHour() const {
  PRE( true );
  const int hh0000 = hhmmss() / 10000 * 10000;
  const Timestamp result( yyyymmdd(), hh0000 );
  POST2( result.sameHour( *this ), result.hhmmss() % 10000 == 0 );
  return result;
}



/******************************************************************************
PURPOSE: endOfHour - End of hour.
RETURNS: Timestamp at end of hour.
******************************************************************************/

Timestamp Timestamp::endOfHour() const {
  PRE( true );
  const int hh5959 = hhmmss() / 10000 * 10000 + 5959;
  const Timestamp result( yyyymmdd(), hh5959 );
  POST3( result.sameHour( *this ),
         result.hhmmss() / 100 % 100 == 59,
         result.hhmmss() % 100 == 59 );
  return result;
}



/******************************************************************************
PURPOSE: startOfDay - Start of day.
RETURNS: Timestamp at start of day.
******************************************************************************/

Timestamp Timestamp::startOfDay() const {
  PRE( true );
  const Timestamp result( yyyymmdd(), 0 );
  POST2( result.sameDay( *this ), result.hhmmss() == 0 );
  return result;
}



/******************************************************************************
PURPOSE: endOfDay - End of day.
RETURNS: Timestamp at end of day.
******************************************************************************/

Timestamp Timestamp::endOfDay() const {
  PRE( true );
  const Timestamp result( yyyymmdd(), 235959 );
  POST2( result.sameDay( *this ), result.hhmmss() == 235959 );
  return result;
}



/******************************************************************************
PURPOSE: startOfMonth - Start of month.
RETURNS: Timestamp at start of month.
******************************************************************************/

Timestamp Timestamp::startOfMonth() const {
  PRE( true );
  const int yyyymm01 = yyyymmdd() / 100 * 100 + 1;
  const Timestamp result( yyyymm01, 0 );
  POST3( result.sameMonth( *this ),
         result.yyyymmdd() % 100 == 1,
         result.hhmmss() == 0 );
  return result;
}



/******************************************************************************
PURPOSE: endOfMonth - End of month.
RETURNS: Timestamp at end of month.
******************************************************************************/

Timestamp Timestamp::endOfMonth() const {
  PRE( true );
  const int yyyymm = yyyymmdd() / 100;
  const int yyyy = yyyymm / 100;
  const int mm = yyyymm % 100;
  const int dd =
    daysPerMonth[ daysInYear( yyyy ) == 366 ? 1 : 0 ][ mm - 1 ];
  const Timestamp result( yyyymm * 100 + dd, 235959 );
  POST3( result.sameMonth( *this ),
         result.yyyymmdd() % 100 ==
           daysPerMonth[ daysInYear( result.yyyymmdd() / 10000 ) == 366 ? 1 : 0]
                       [ result.yyyymmdd() / 100 % 100 - 1 ],
         result.hhmmss() == 235959 );
  return result;
}



/******************************************************************************
PURPOSE: startOfYear - Start of year.
RETURNS: Timestamp at start of year.
******************************************************************************/

Timestamp Timestamp::startOfYear() const {
  PRE( true );
  const int yyyy0101 = ( yyyymmdd() / 10000 * 100 + 1 ) * 100 + 1;
  const Timestamp result( yyyy0101, 0 );
  POST4( result.sameYear( *this ),
         result.yyyymmdd() / 100 % 100 == 1,
         result.yyyymmdd() % 100 == 1,
         result.hhmmss() == 0 );
  return result;
}



/******************************************************************************
PURPOSE: endOfYear - End of year.
RETURNS: Timestamp at end of year.
******************************************************************************/

Timestamp Timestamp::endOfYear() const {
  PRE( true );
  const int yyyy1231 = ( yyyymmdd() / 10000 * 100 + 12 ) * 100 + 31;
  const Timestamp result( yyyy1231, 235959 );
  POST4( result.sameYear( *this ),
         result.yyyymmdd() / 100 % 100 == 12,
         result.yyyymmdd() % 100 == 31,
         result.hhmmss() == 235959 );
  return result;
}



/******************************************************************************
PURPOSE: hoursUntil - Hours until sameHour( other ).
INPUTS:  const Timestamp& other  Other timestamp.
RETURNS: int hours.
******************************************************************************/

int Timestamp::hoursUntil( const Timestamp& other ) const {
  PRE2( other.invariant(), *this <= other );
  const long long secondsUntil = other.seconds_ - seconds_;
  const int secondsPerMinute = 60;
  const int minutesPerHour = 60;
  const int secondsPerHour = secondsPerMinute * minutesPerHour;
  const long long hours = secondsUntil / secondsPerHour;
  const int result = (int) hours;
  POST( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: monthsUntil - Months until sameMonth( other ).
INPUTS:  const Timestamp& other  Other timestamp.
RETURNS: int months.
******************************************************************************/

int Timestamp::monthsUntil( const Timestamp& other ) const {
  PRE2( other.invariant(), *this <= other );
  int result = 0;
  Timestamp yyyymm( yyyymmdd() / 100 * 100 + 1, 0 );

  while ( AND2( yyyymm < other, ! yyyymm.sameMonth( other ) ) ) {
    yyyymm.offsetMonths( 1 );
    ++result;
  }

  POST( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: operator== - Are objects equal?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if equal, else false.
******************************************************************************/

bool Timestamp::operator==( const Timestamp& other ) const {
  PRE( other.invariant() );
  const bool result = seconds_ == other.seconds_;
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Are objects unequal?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if unequal, else false.
******************************************************************************/

bool Timestamp::operator!=( const Timestamp& other ) const {
  PRE( other.invariant() );
  const bool result = seconds_ != other.seconds_;
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Is this <= other?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if <=, else false.
******************************************************************************/

bool Timestamp::operator<=( const Timestamp& other ) const {
  PRE( other.invariant() );
  const bool result = seconds_ <= other.seconds_;
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Is this >= other?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if <=, else false.
******************************************************************************/

bool Timestamp::operator>=( const Timestamp& other ) const {
  PRE( other.invariant() );
  const bool result = seconds_ >= other.seconds_;
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Is this < other?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if <, else false.
******************************************************************************/

bool Timestamp::operator<( const Timestamp& other ) const {
  PRE( other.invariant() );
  const bool result = seconds_ < other.seconds_;
  return result;
}



/******************************************************************************
PURPOSE: operator!= - Is this > other?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if <, else false.
******************************************************************************/

bool Timestamp::operator>( const Timestamp& other ) const {
  PRE( other.invariant() );
  const bool result = seconds_ > other.seconds_;
  return result;
}



/******************************************************************************
PURPOSE: sameHour - Are objects the same date and within the same hour?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if same date and within same hour, else false.
******************************************************************************/

bool Timestamp::sameHour( const Timestamp& other ) const {
  PRE( other.invariant() );
  bool result = OR2( this == &other, *this == other );

  if ( ! result ) {
    struct tm t1;
    struct tm t2;
    const time_t seconds1 = (time_t) seconds_;
    const time_t seconds2 = (time_t) other.seconds_;

    if ( AND2( GMTIME_R( &seconds1, &t1 ), GMTIME_R( &seconds2, &t2 ) ) ) {
      result =
        AND3( t1.tm_hour == t2.tm_hour,
              t1.tm_yday == t2.tm_yday,
              t1.tm_year == t2.tm_year );
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: sameDay - Are objects the same date?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if same date and within same hour, else false.
******************************************************************************/

bool Timestamp::sameDay( const Timestamp& other ) const {
  PRE( other.invariant() );
  bool result = OR2( this == &other, *this == other );

  if ( ! result ) {
    struct tm t1;
    struct tm t2;
    const time_t seconds1 = (time_t) seconds_;
    const time_t seconds2 = (time_t) other.seconds_;

    if ( AND2( GMTIME_R( &seconds1, &t1 ), GMTIME_R( &seconds2, &t2 ) ) ) {
      result = AND2( t1.tm_yday == t2.tm_yday, t1.tm_year == t2.tm_year );
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: sameMonth - Are objects the same year and month?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if same year and month, else false.
******************************************************************************/

bool Timestamp::sameMonth( const Timestamp& other ) const {
  PRE( other.invariant() );
  bool result = OR2( this == &other, *this == other );

  if ( ! result ) {
    struct tm t1;
    struct tm t2;
    const time_t seconds1 = (time_t) seconds_;
    const time_t seconds2 = (time_t) other.seconds_;

    if ( AND2( GMTIME_R( &seconds1, &t1 ), GMTIME_R( &seconds2, &t2 ) ) ) {
      result = AND2( t1.tm_mon == t2.tm_mon, t1.tm_year == t2.tm_year );
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: sameYear - Are objects the same year?
INPUTS:  const Timestamp& other  Object to compare to.
RETURNS: bool true if same same year, else false.
******************************************************************************/

bool Timestamp::sameYear( const Timestamp& other ) const {
  PRE( other.invariant() );
  bool result = OR2( this == &other, *this == other );

  if ( ! result ) {
    struct tm t1;
    struct tm t2;
    const time_t seconds1 = (time_t) seconds_;
    const time_t seconds2 = (time_t) other.seconds_;

    if ( AND2( GMTIME_R( &seconds1, &t1 ), GMTIME_R( &seconds2, &t2 ) ) ) {
      result = t1.tm_year == t2.tm_year;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: invariant - Class invariant.
RETURNS: bool true.
******************************************************************************/

bool Timestamp::invariant() const {
  const bool result = true;
  return result;
}



} // End namespace emvl.


