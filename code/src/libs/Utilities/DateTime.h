
#ifndef DATETIME_H
#define DATETIME_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
PURPOSE: DateTime.h - Declare routines for date/time computation.

HISTORY: 2004/10 Todd Plessel EPA/LM Created.

STATUS:  unreviewed, untested.
******************************************************************************/

/*================================ FUNCTIONS ================================*/

extern int isValidDate( int yyyyddd );

extern int isValidTime( int hhmmss );

extern int isValidTimestepSize( int hhmmss );

extern int isLeapYear( int yyyy );

extern int timestepsInRange( int firstDate, int firstTime,
                             int lastDate,  int lastTime );

extern void monthAndDay( int yyyyddd, int* month, int* dayOfMonth );

extern void incrementOneHour( int* yyyyddd, int* hhmmss );

extern void decrementOneHour( int* yyyyddd, int* hhmmss );

extern void incrementTime( int* yyyyddd, int* hhmmss, int step );

extern void decrementTime( int* yyyyddd, int* hhmmss, int step );



/* New date/time routines: */

enum { UTC_TIMESTAMP_LENGTH = 24 }; /* YYYY-MM-DDTHH:MM:SS-ZZZZ */
typedef char UTCTimestamp[ UTC_TIMESTAMP_LENGTH + 1 ];

extern int isValidUTCTimestamp( const char* string );
extern void toUTCTimestamp( long long yyyydddhhmm, UTCTimestamp string );
extern long long fromUTCTimestamp( const UTCTimestamp string );
extern void incrementTimestamp( long long* yyyydddhhmm );
extern void decrementTimestamp( long long* yyyydddhhmm );
extern long long offsetTimestamp( long long yyyydddhhmm, int hours );
extern int parseTimestamp( const char* string, long long* yyyydddhh00 );
extern int isValidTimestamp( long long yyyydddhhmm );
extern int isValidYearMonthDay( int yyyymmdd );
extern int convertYearMonthDay( int yyyymmdd );
extern long long nowUTC( void ); /* Returns YYYYDDDHHMM. */
extern int fileDateUTC( const char* fileName );
extern int timeZoneOffset( const char* timeZoneName );

extern int isYear( const int yyyy );
extern int daysInYear( const int yyyy );
extern int isYYYYMMDD( const int yyyy, const int mm, const int dd );
extern int isHHMMSS( const int hhmmss );
extern int isUTCTimestamp( const UTCTimestamp value );

extern int daysInMonth( int year, int month );
extern int isValidYYYYMMDD( int yyyymmdd );
extern int incrementDate( int yyyymmdd, int days );
extern int isValidYYYYMMDDHH( int yyyymmddhh );
extern int incrementDateTime( int yyyymmddhh, int hours );

#ifdef __cplusplus
}
#endif

#endif /* DATETIME_H */


