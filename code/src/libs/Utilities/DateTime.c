/******************************************************************************
PURPOSE: DateTime.c - Define routines for date/time computation.
NOTES:   See DateTime.h.
HISTORY: 2004/10, Created.
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h>  /* For stderr, snprintf(). */
#include <stdlib.h> /* For abs(). */
#include <string.h> /* For strlen(), memset(), memcpy(). */
#include <time.h>   /* For gmtime_r(). */
#include <sys/stat.h> /* For stat(). */

#ifdef _WIN32
#ifdef _WIN64
/*
 * HACK non-standard gmtime_s() exists but with a different argument order
 * and return logic, but we can use it with adjustments:
 */
#define gmtime_r( clock, result ) ( gmtime_s( result, clock ) ? 0 : result )
#else
/*
 * gmtime_s() is missing in mingw32 so just use gmtime().
 * According to this reference there is no actual race-condition:
 * https://sourceforge.net/p/mingw-w64/mailman/message/21523522/
 * "Please note that plain old gmtime() and localtime() *are* thread-safe
 * in Microsoft's C library. They use a thread-local buffer."
 */
static struct tm* gmtime_r( const time_t* clock, struct tm* result ) {
  const struct tm* const result0 = gmtime( clock ); /* HACK: not thread-safe!*/
  const int ok = result0 != 0;
  if ( result0 ) memcpy( result, result0, sizeof (struct tm) );
  return ok ? result : 0;
}
#endif
#endif

#include <Assertions.h> /* For PRE0*(), POST0*(), AND?(). */
#include <DateTime.h> /* For public interface. */

/*============================= PUBLIC FUNCTIONS ============================*/


/*
 * 30 days hath September, April, June and November, all the rest have 31,
 * except February which has either 28 or 29 (on a leap year).
 */

static const int daysPerMonth[ 2 ][ 12 ] =
{
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, /* Non-leap year. */
  { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }  /* Leap year. */
};


/*============================= PUBLIC FUNCTIONS ============================*/




/******************************************************************************
PURPOSE: isValidDate - Is the given date valid YYYYDDD format?
INPUTS:  int yyyyddd  The date to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidDate( int yyyyddd ) {
  const int yyyy = yyyyddd / 1000;
  const int ddd  = yyyyddd % 1000;
  const int result =
    AND3( yyyy >= 1800,
          IN_RANGE( ddd, 1, 366 ),
          IMPLIES( ddd == 366, isLeapYear( yyyy ) ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidTime - Is the given time valid HHMMSS format?
INPUTS:  int hhmmss  The time to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidTime( int hhmmss ) {
  const int hh     = hhmmss / 10000;
  const int mm     = ( hhmmss / 100 ) % 100;
  const int ss     = hhmmss % 100;
  const int result =
    AND3( IN_RANGE( hh, 0, 23 ), IN_RANGE( mm, 0, 59 ), IN_RANGE( ss, 0, 59));
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidTimestepSize - Is the given time valid *HHMMSS format?
INPUTS:  int hhmmss  The timestep size to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidTimestepSize( int hhmmss ) {
  const int hh     = hhmmss / 10000;
  const int mm     = ( hhmmss / 100 ) % 100;
  const int ss     = hhmmss % 100;
  const int result =
    AND4( hhmmss > 0, hh >= 0, IN_RANGE( mm, 0, 59 ), IN_RANGE( ss, 0, 59 ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isLeapYear - Does the year have 366 days?
INPUTS:  int yyyy  YYYY.
RETURNS: int 1 if yyyy is a leap year, else 0.
******************************************************************************/

int isLeapYear( int yyyy ) {
  PRE0( isValidDate( yyyy * 1000 + 1 ) );
  const int result = yyyy % 4 == 0 && (yyyy % 100 != 0 || yyyy % 400 == 0);
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: timestepsInRange - Number of one-hour timesteps in range, inclusive.
INPUTS:  int firstDate  YYYYDDD
         int firstTime  HHMMSS
         int lastDate   YYYYDDD
         int lastTime   HHMMSS
RETURNS: int Number of one-hour timesteps in range, inclusive.
******************************************************************************/

int timestepsInRange( int firstDate, int firstTime,
                      int lastDate,  int lastTime ) {

  PRE06( isValidDate( firstDate ), isValidTime( firstTime ),
         isValidDate( lastDate  ), isValidTime( lastTime  ),
         firstDate <= lastDate,
         IMPLIES( firstDate == lastDate, firstTime <= lastTime ) );

  int yyyyddd = firstDate;
  int hhmmss  = firstTime;
  int result  = 1;

  while ( ! AND2( yyyyddd == lastDate, hhmmss == lastTime ) ) {
    incrementOneHour( &yyyyddd, &hhmmss );
    ++result;
  }

  POST0( result > 0 );
  return result;
}



/******************************************************************************
PURPOSE: monthAndDay - Month [1..12] and day of month [1..31] of yyyyddd.
INPUTS:  int yyyyddd      Year and day of year.
OUTPUTS: int* month       Month of year [1..12].
         int* dayOfMonth  Day of month [1..31].
******************************************************************************/

void monthAndDay( int yyyyddd, int* month, int* dayOfMonth ) {

  PRE03( isValidDate( yyyyddd ), month, dayOfMonth );

  enum { MONTHS = 12 };
  int daysPerMonth[ MONTHS ] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };

  const int yyyy = yyyyddd / 1000;
  const int ddd  = yyyyddd % 1000;
  int dddRemaining = ddd;
  int days = 0, thisMonth = 0;
  *month = *dayOfMonth = 0;
  daysPerMonth[ 1 ] += isLeapYear( yyyy );

  for ( thisMonth = 0; thisMonth < MONTHS; ++thisMonth ) {
    const int daysThisMonth = daysPerMonth[ thisMonth ];
    days += daysThisMonth;

    if ( days >= ddd ) {
      *month = thisMonth + 1;
      *dayOfMonth = dddRemaining;
      thisMonth = MONTHS;
    }

    dddRemaining -= daysThisMonth;
  }

  POST02( IN_RANGE( *month, 1, 12 ), IN_RANGE( *dayOfMonth, 1, 31 ) );
}



/******************************************************************************
PURPOSE: incrementOneHour - Increment date/time by one hour.
INPUTS:  int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
OUTPUTS: int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
******************************************************************************/

void incrementOneHour( int* yyyyddd, int* hhmmss ) {

  PRE04( yyyyddd, hhmmss, isValidDate( *yyyyddd ), isValidTime( *hhmmss ) );

  const int oneHour     =  10000; /* HHMMSS = 01:00:00. */
  const int maximumTime = 235959; /* HHMMSS = 23:59:59. */
  *hhmmss += oneHour;

  if ( *hhmmss > maximumTime ) {
    const int ss = *hhmmss % 100;
    const int mm = ( *hhmmss / 100 ) % 100;
    int ddd;
    *hhmmss = mm * 100 + ss;
    *yyyyddd += 1;
    ddd = *yyyyddd % 1000;

    if ( ddd > 365 ) {
      const int yyyy = *yyyyddd / 1000;
      const int daysInYear = 365 + isLeapYear( yyyy );

      if ( ddd > daysInYear ) {
        *yyyyddd = ( yyyy + 1 ) * 1000 + 1; /* Next year, first day. */
      }
    }
  }

  POST02( isValidDate( *yyyyddd ), isValidTime( *hhmmss ) );
}



/******************************************************************************
PURPOSE: decrementOneHour - Decrement date/time by one hour.
INPUTS:  int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
OUTPUTS: int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
******************************************************************************/

void decrementOneHour( int* yyyyddd, int* hhmmss ) {

  PRE04( yyyyddd, hhmmss, isValidDate( *yyyyddd ), isValidTime( *hhmmss ) );

  const int oneHour = 10000; /* HHMMSS = 01:00:00. */
  *hhmmss -= oneHour;

  if ( *hhmmss < 0 ) {
    const int lastHour = 230000; /* HHMMSS = 23:00:00. */
    int ss, mm, ddd;
    *hhmmss += oneHour;
    ss = *hhmmss % 100;
    mm = ( *hhmmss / 100 ) % 100;
    *hhmmss = lastHour + mm * 100 + ss;
    *yyyyddd -= 1;
    ddd = *yyyyddd % 1000;

    if ( ddd < 1 ) {
      const int yyyy = *yyyyddd / 1000;
      const int daysInYear = 365 + isLeapYear( yyyy );
      *yyyyddd = ( yyyy - 1 ) * 1000 + daysInYear; /* Previous year last day*/
    }
  }

  POST02( isValidDate( *yyyyddd ), isValidTime( *hhmmss ) );
}



/******************************************************************************
PURPOSE: incrementTime - Increment date/time by step.
INPUTS:  int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
         int  step     ...HHMMSS.
OUTPUTS: int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
******************************************************************************/

void incrementTime( int* yyyyddd, int* hhmmss, int step ) {

  PRE05( yyyyddd, hhmmss, isValidDate( *yyyyddd ), isValidTime( *hhmmss ),
         isValidTimestepSize( step ) );

  int hours = step / 10000; /* ...hh0000 */

  while ( hours-- ) {
    incrementOneHour( yyyyddd, hhmmss );
  }

  step %= 10000; /* 00mmss. */

  if ( step ) {
    const int stepSS = step % 100;
    int ss = *hhmmss % 100;
    int mm = ( *hhmmss / 100 ) % 100;
    int hh = 0;
    ss += stepSS;
    mm += ss / 60;
    ss %= 60;
    hh = mm / 60;
    mm %= 60;

    if ( hh > 0 ) {
      incrementOneHour( yyyyddd, hhmmss );
      hh = *hhmmss / 10000;
    }

    *hhmmss = hh * 10000 + mm * 100 + ss;
  }

  POST02( isValidDate( *yyyyddd ), isValidTime( *hhmmss ) );
}



/******************************************************************************
PURPOSE: decrementTime - Decrement date/time by step.
INPUTS:  int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
         int  step     HHMMSS.
OUTPUTS: int* yyyyddd  YYYYDDD.
         int* hhmmss   HHMMSS.
******************************************************************************/

void decrementTime( int* yyyyddd, int* hhmmss, int step ) {

  PRE05( yyyyddd, hhmmss, isValidDate( *yyyyddd ), isValidTime( *hhmmss ),
         isValidTimestepSize( step ) );

  int hours = 1 + step / 10000; /* ...hh0000 */

  do {
    decrementOneHour( yyyyddd, hhmmss );
    --hours;
  } while ( hours );

  step %= 10000; /* 00mmss. */

  if ( step == 0 ) {
    incrementOneHour( yyyyddd, hhmmss );
  } else {
    incrementTime( yyyyddd, hhmmss, step );
  }

  POST02( isValidDate( *yyyyddd ), isValidTime( *hhmmss ) );
}



/******************************************************************************
PURPOSE: isValidUTCTimestamp - Is string a valid UTCTimestamp?
INPUTS:  const char* string  The string to examine.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidUTCTimestamp( const char* string ) {
  int result = 0;

  if ( AND2( string, strlen( string ) == UTC_TIMESTAMP_LENGTH ) ) {
    int yyyy = 0, mo = 0, dd = 0, hh = 0, mm = 0, ss = 0, zone = 0;

    if ( sscanf( string, "%04d-%02d-%02dT%02d:%02d:%02d-%04d",
                 &yyyy, &mo, &dd, &hh, &mm, &ss, &zone ) == 7 ) {
      const int yyyymmdd = yyyy * 10000 + mo * 100 + dd;
      result = isValidYearMonthDay( yyyymmdd );
      result = AND6( result,
                     IN_RANGE( hh, 0, 23 ),
                     IN_RANGE( mm, 0, 59 ),
                     IN_RANGE( ss, 0, 59 ),
                     IN_RANGE( zone / 100, -23, 23 ),
                     IN_RANGE( abs( zone % 100 ), 0, 59 ) );
    }
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: toUTCTimestamp - Convert timestamp to ISO UTC string format.
INPUTS:  long long yyyydddhhmm  Timestamp to convert.
OUTPUTS: UTCTimestamp string  ISO UTC string format for the timestamp.
******************************************************************************/

void toUTCTimestamp( long long yyyydddhhmm, UTCTimestamp string ) {
  PRE02( isValidTimestamp( yyyydddhhmm ), string );
  const int mm      = yyyydddhhmm % 100;
  const int hh      = yyyydddhhmm / 100 % 100;
  const int yyyyddd = yyyydddhhmm / 10000;
  const int yyyy    = yyyyddd / 1000;
  int mo = 0;
  int dd = 0;
  monthAndDay( yyyyddd, &mo, &dd );
  snprintf( string, sizeof (UTCTimestamp) / sizeof (char),
            "%04d-%02d-%02dT%02d:%02d:00-0000",
            yyyy, mo, dd, hh, mm );
  POST0( strlen( string ) == UTC_TIMESTAMP_LENGTH );
}



/******************************************************************************
PURPOSE: fromUTCTimestamp - Convert ISO UTC string to integer.
INPUTS:  const UTCTimestamp string  ISO UTC string to convert.
RETURNS: long long yyyydddhhmm timestamp.
******************************************************************************/

long long fromUTCTimestamp( const UTCTimestamp string ) {

  PRE0( isValidUTCTimestamp( (const char*) string ) );

  long long result = 0;
  int yyyy = 1900, mo = 1, dd = 1, hh = 0, mm = 0, ss = 0, zone = 0;

  if ( sscanf( string, "%04d-%02d-%02dT%02d:%02d:%02d-%04d",
               &yyyy, &mo, &dd, &hh, &mm, &ss, &zone ) == 7 ) {
    result = yyyy;
    result *= 100;
    result += mo;
    result *= 100;
    result += dd;
    result = convertYearMonthDay( result );
    result *= 100;
    result += hh;
    result *= 100;
    result += mm;
  }

  POST0( isValidTimestamp( result ) );
  return result;
}



/******************************************************************************
PURPOSE: parseTimestamp - Parse string timestamp into its integer value.
INPUTS:  const char* string      String representation of timestamp.
OUTPUTS: long long*    yyyydddhh00 Value of string as yyyydddhh00.
RETURNS: int 1 if successful, else 0.
******************************************************************************/

int parseTimestamp( const char* string, long long* yyyydddhh00 ) {

  PRE02( string, yyyydddhh00 );

  const int yyyymmddhh = atoi( string );
  const int yyyymmdd       = yyyymmddhh / 100;
  const int hh             = yyyymmddhh % 100;
  const int result =
    AND2( IN_RANGE( hh, 0, 23 ), isValidYearMonthDay( yyyymmdd ) );

  *yyyydddhh00 = 0;

  if ( ! result ) {
    fprintf( stderr, "\a\n\nInvalid timestamp specified '%s'.\n", string );
  } else {
    const int yyyyddd   = convertYearMonthDay( yyyymmdd );
    const int yyyydddhh = yyyyddd * 100 + hh;
    *yyyydddhh00 = yyyydddhh * 100;
  }

  POST02( IS_BOOL( result ),
         IMPLIES_ELSE( result,
                       isValidTimestamp( *yyyydddhh00 ), *yyyydddhh00 == 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidTimestamp - Is the timestamp valid?
INPUTS:  long long yyyydddhhmm The timestamp to examine.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidTimestamp( long long yyyydddhhmm ) {
  const int yyyy = yyyydddhhmm / 10000000;
  const int ddd  = yyyydddhhmm / 10000 % 1000;
  const int hh   = yyyydddhhmm / 100 % 100;
  const int mm   = yyyydddhhmm % 100;
  const int result =
    AND4( IN_RANGE( yyyy, 1800, 9999 ),
          IN_RANGE( ddd, 1, 365 + isLeapYear( yyyy ) ),
          IN_RANGE( hh, 0, 23 ),
          IN_RANGE( mm, 0, 59 ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidYearMonthDay - Is the date valid?
INPUTS:  int yyyymmdd The date to examine.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidYearMonthDay( int yyyymmdd ) {
  const int yyyy = yyyymmdd / 10000;
  const int mm   = yyyymmdd / 100 % 100;
  const int dd   = yyyymmdd % 100;
  const int result =
    AND3( IN_RANGE( yyyy, 1800, 9999 ),
          IN_RANGE( mm, 1, 12 ),
          IN_RANGE( dd, 1, daysPerMonth[ isLeapYear( yyyy ) ][ mm - 1 ] ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: convertYearMonthDay - Convert date from YYYYMMDD to YYYYDDD.
INPUTS:  int yyyymmdd  Year, month, day to convert.
RETURNS: int yyyyddd equivalent day.
******************************************************************************/

int convertYearMonthDay( int yyyymmdd ) {

  PRE03( yyyymmdd / 10000 > 1000,
        IN_RANGE(  yyyymmdd / 100 % 100, 1, 12 ),
        IN_RANGE(  yyyymmdd % 100, 1,
                   daysPerMonth[ isLeapYear( yyyymmdd / 10000 ) ]
                   [ yyyymmdd / 100 % 100 - 1] ) );

  const int yyyy = yyyymmdd / 10000;
  const int mm0  = yyyymmdd / 100 % 100 - 1;
  const int dd   = yyyymmdd % 100;
  const int leap = isLeapYear( yyyy );
  int result = 0;
  int month = 0;

  for ( month = 0; month < mm0; ++month ) {
    result += daysPerMonth[ leap ][ month ];
  }

  result += dd;
  result += yyyy * 1000;

  POST03( result / 1000 == yyyymmdd / 10000,
          result % 1000 > 0,
          result % 1000 <= 365 + isLeapYear( yyyymmdd / 10000 ) );
  return result;
}



/******************************************************************************
PURPOSE: incrementTimestamp - Increment timestamp by one hour.
INPUTS:  long long* yyyydddhhmm  Timestamp to increment.
OUTPUTS: long long* yyyydddhhmm  Inremented timestamp.
******************************************************************************/

void incrementTimestamp( long long* yyyydddhhmm ) {
  PRE02( yyyydddhhmm, isValidTimestamp( *yyyydddhhmm ) );
  const int mm = *yyyydddhhmm % 100;
  int hh = *yyyydddhhmm / 100 % 100;
  ++hh;

  if ( hh < 24 ) { /* Just update the hour: */
    *yyyydddhhmm = *yyyydddhhmm / 10000 * 10000 + hh * 100 + mm;
  } else { /* Reset the hour and increment the day: */
    int yyyy = *yyyydddhhmm / 10000000;
    int ddd  = *yyyydddhhmm / 10000 % 1000;
    hh = 0;
    ++ddd;

    if ( AND2( ddd > 365, ddd > 365 + isLeapYear( yyyy ) ) ) {
      ddd = 1; /* First day,    */
      ++yyyy;  /* of next year. */
    }

    *yyyydddhhmm = yyyy * 10000000 + ddd * 10000 + hh * 00 + mm;
  }

  POST0( isValidTimestamp( *yyyydddhhmm ) );
}



/******************************************************************************
PURPOSE: decrementTimestamp - Decrement timestamp by one hour.
INPUTS:  long long* yyyydddhhmm  Timestamp to increment.
OUTPUTS: long long* yyyydddhhmm  Deremented timestamp.
******************************************************************************/

void decrementTimestamp( long long* yyyydddhhmm ) {
  PRE02( yyyydddhhmm, isValidTimestamp( *yyyydddhhmm ) );
  const int mm = *yyyydddhhmm % 100;
  int hh = *yyyydddhhmm / 100 % 100;
  --hh;

  if ( hh >= 0 ) { /* Just update the hour: */
    *yyyydddhhmm = *yyyydddhhmm / 10000 * 10000 + hh * 100 + mm;
  } else { /* Reset the hour and decrement the day: */
    int yyyy = *yyyydddhhmm / 10000000;
    int ddd  = *yyyydddhhmm / 10000 % 1000;
    hh = 23;
    --ddd;

    if ( ddd == 0 ) {
      --yyyy; /* Previous year. */
      ddd = 365 + isLeapYear( yyyy );
    }

    *yyyydddhhmm = yyyy * 10000000 + ddd * 10000 + hh * 100 + mm;
  }

  POST0( isValidTimestamp( *yyyydddhhmm ) );
}



/******************************************************************************
PURPOSE: offsetTimestamp - Compute timestamp + offset.
INPUTS:  long long yyyydddhhmm  Initial timestamp.
         int hours        + or - hours to offset.
RETURNS: long long yyyydddhhmm + hours.
******************************************************************************/

long long offsetTimestamp( long long yyyydddhhmm, int hours ) {
  PRE0( isValidTimestamp( yyyydddhhmm ) );
  long long result = yyyydddhhmm;
  int hour = hours;

  while ( hour < 0 ) {
    decrementTimestamp( &result );
    ++hour;
  }

  while ( hour > 0 ) {
    incrementTimestamp( &result );
    --hour;
  }

  POST02( isValidTimestamp( result ),
          OR3( AND2( hours == 0, result == yyyydddhhmm ),
               AND2( hours < 0,  result <  yyyydddhhmm ),
               AND2( hours > 0,  result >  yyyydddhhmm ) ) );
  return result;
}



/******************************************************************************
PURPOSE: nowUTC - Current timestamp in UTC.
RETURNS: long long yyyydddhhmm.
******************************************************************************/

long long nowUTC( void ) {
  const time_t clock = time( 0 );
  struct tm timeInfo;
  long long result = 19000010000LL;

  if ( gmtime_r( &clock, &timeInfo ) ) {
    result = timeInfo.tm_year + 1900;
    result = result * 1000 + timeInfo.tm_yday + 1;
    result = result *  100 + timeInfo.tm_hour;
    result = result *  100 + timeInfo.tm_min;
  }

  POST0( isValidTimestamp( result ) );
  return result;
}




/******************************************************************************
PURPOSE: fileDateUTC - UTC date when named file was last modified.
INPUTS:  const char* fileName  Name of file to check.
RETURNS: int yyyymmdd.
******************************************************************************/

int fileDateUTC( const char* fileName ) {
  int result = 19000101;
  struct stat fileInfo;

  if ( stat( fileName, &fileInfo ) == 0 ) {
    const time_t seconds = fileInfo.st_mtime;
    struct tm timestamp;

    if ( gmtime_r( &seconds, &timestamp ) ) {
      const int yyyy = timestamp.tm_year + 1900;
      const int mm   = timestamp.tm_mon + 1;
      const int dd   = timestamp.tm_mday;
      const int yyyymmdd = yyyy * 10000 + mm * 100 + dd;

      if ( isValidYearMonthDay( yyyymmdd ) ) {
        result = yyyymmdd;
      }
    }
  }

  POST0( isValidYearMonthDay( result ) );
  return result;
}



/******************************************************************************
PURPOSE: timeZoneOffset - Hour offset (subtracted) from UTC.
INPUTS:  const char* timeZoneName  E.g., "EST".
RETURNS: int offset from UTC (e.g., 4).
******************************************************************************/

int timeZoneOffset( const char* timeZoneName ) {
  PRE02( timeZoneName, *timeZoneName );
  typedef struct { const char* name; int offset; } TimeZoneEntry;
  static const TimeZoneEntry timeZones[] = {
    { "AST", 4 },
    { "EST", 5 },
    { "EDT", 4 },
    { "CST", 6 },
    { "CDT", 5 },
    { "MST", 7 },
    { "MDT", 6 },
    { "PST", 8 },
    { "PDT", 7 },
    { "AKST", 9 },
    { "AKDT", 8 },
    { "HAST", 10 },
    { "HASDT", 9 },
    { 0, 0 }
  };
  int result = 0;
  int index = 0;

  for ( index = 0; AND2( ! result, timeZones[ index ].name ); ++index ) {

    if ( ! strcmp( timeZoneName, timeZones[ index ].name ) ) {
      result = timeZones[ index ].offset;
    }
  }

  POST0( IN_RANGE( result, 0, 23 ) );
  return result;
}




int isYear( const int yyyy ) {
  const int result = IN_RANGE( yyyy, 1800, 2999 );
  return result;
}




int daysInYear( const int yyyy ) {
  PRE0( isYear( yyyy ) );
  const int isLeapYear =
    yyyy % 4 == 0 && ( yyyy % 100 != 0 || yyyy % 400 == 0 );
  const int result = 365 + isLeapYear;
  POST0( IN3( result, 365, 366 ) );
  return result;
}




int isYYYYMMDD( const int yyyy, const int mm, const int dd ) {
  int result = AND2( isYear( yyyy ), IN_RANGE( mm, 1, 12 ) );

  if ( result ) {
    const int isLeapYear = daysInYear( yyyy ) == 366;
    const int index = isLeapYear ? 1 : 0;
    const int days = daysPerMonth[ index ][ mm - 1 ];
    result = IN_RANGE( dd, 1, days );
  }

  return result;
}




int isHHMMSS( const int hhmmss ) {
  const int hh = hhmmss / 10000;
  const int mm = hhmmss / 100 % 100;
  const int ss = hhmmss % 100;
  const int result =
    AND3( IN_RANGE( hh, 0, 23 ), IN_RANGE( mm, 0, 59 ), IN_RANGE( ss, 0, 59 ));
  return result;
}




int isUTCTimestamp( const UTCTimestamp value ) {
  int result = 0;

  if ( AND2( value, strlen( value ) == UTC_TIMESTAMP_LENGTH ) ) {
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

      if ( isYYYYMMDD( yyyy, mo, dd ) ) {
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
PURPOSE: daysInMonth - The number of days in the given [year, month].
INPUTS:  int year  4-digit year.
         int month [1, 12].
RETURNS: int [1, 31].
******************************************************************************/

int daysInMonth( int year, int month ) {
  PRE02( IN_RANGE( year, 1800, 2147 ), IN_RANGE( month, 1, 12 ) );
  int result = 31;
  int isLeapYear = 0;

  switch ( month ) {
  case 2:
    isLeapYear = year % 4 == 0 && ! ( year % 100 == 0 && year % 400 != 0 );
    result = isLeapYear ? 29 : 28;
    break;
  case 4:
  case 6:
  case 9:
  case 11:
    result = 30;
    break;
  default:
    result = 31;
    break;
  }

  POST0( IN_RANGE( result, 1, 31 ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidYYYYMMDD - Is the date yyyymmdd valid?
INPUTS:  int yyyymmdd  E.g., 20090501 is May 1, 2009.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidYYYYMMDD( int yyyymmdd ) {
  int yyyy   = yyyymmdd / 10000;
  int mm     = yyyymmdd / 100 % 100;
  int dd     = yyyymmdd % 100;
  const int validYearMonth =
    AND2( IN_RANGE( yyyy, 1800, 2147 ), IN_RANGE( mm, 1, 12 ) );
  const int daysInThisMonth = validYearMonth ? daysInMonth( yyyy, mm ) : 0;
  const int result = AND2( validYearMonth, IN_RANGE( dd, 1, daysInThisMonth ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: incrementDate - The date (yyyymmdd) incremented by the given of days.
INPUTS:  int yyyymmdd  E.g., 20090501 is May 1, 2009.
         int days      Number of days to increment from yyyymmdd.
RETURNS: int incremented yyyymmdd or maximum value upon overflow.
******************************************************************************/

int incrementDate( int yyyymmdd, int days ) {
  PRE02( isValidYYYYMMDD( yyyymmdd ), days >= 0 );
  int result = yyyymmdd;
  int yyyy   = yyyymmdd / 10000;
  int mm     = yyyymmdd / 100 % 100;
  int dd     = yyyymmdd % 100;
  int daysInThisMonth = daysInMonth( yyyy, mm );
  int d = 0;

  for ( d = 0; d < days; ++d ) {
    ++dd;

    if ( dd > daysInThisMonth ) {
      dd = 1;
      ++mm;

      if ( mm > 12 ) {
        mm = 1;
        ++yyyy;

        if ( yyyy > 2147 ) {
          yyyy = 2147;
          mm = 12;
          dd = 31;
          d = days - 1;
        }
      }

      daysInThisMonth = daysInMonth( yyyy, mm );
    }
  }

  result = yyyy * 10000 + mm * 100 + dd;
  POST0( isValidYYYYMMDD( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidYYYYMMDDHH - Is the date yyyymmddhh valid?
INPUTS:  int yyyymmddhh  E.g., 2009050112 is May 1, 2009 at noon.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidYYYYMMDDHH( int yyyymmddhh ) {
  const int yyyymmdd = yyyymmddhh / 100;
  const int hh = yyyymmddhh % 100;
  const int result = AND2( isValidYYYYMMDD( yyyymmdd ), IN_RANGE( hh, 0, 23));
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: incrementDateTime - The date (yyyymmddhh) incremented by hours.
INPUTS:  int yyyymmddhh  E.g., 2009050112 is May 1, 2009 noon.
         int hours      Number of hours to increment from yyyymmddhh.
RETURNS: int incremented yyyymmddhh or maximum value upon overflow.
******************************************************************************/

int incrementDateTime( int yyyymmddhh, int hours ) {
  PRE02( isValidYYYYMMDDHH( yyyymmddhh ), hours >= 0 );
  const int days = hours / 24;
  int yyyymmdd = yyyymmddhh / 100;
  const int remainingHours = hours % 24;
  int hh = yyyymmddhh % 100 + remainingHours;
  int result = 0;
  yyyymmdd = incrementDate( yyyymmdd, days );

  if ( hh > 23 ) {
    yyyymmdd = incrementDate( yyyymmdd, 1 );
    hh -= 24;
  }

  result = yyyymmdd * 100 + hh;
  POST0( isValidYYYYMMDDHH( result ) );
  return result;
}


