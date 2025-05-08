
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

/******************************************************************************
PURPOSE: Timestamp.h - Calendar UTC date-timestamps.
HISTORY: 2012-11-20 plessel.todd@epa.gov
STATUS: unreviewed tested
******************************************************************************/

//================================== TYPES ====================================

namespace emvl {

class Timestamp {
private:

  // Attributes:

  long long seconds_; // Before (-) or after (+) 1970-01-01T00:00:00Z.

public:

  // Types:

  enum { UTC_STRING_LENGTH = 24 }; // YYYY-MM-DDTHH:MM:SS-HHMM.
  typedef char UTCString[ UTC_STRING_LENGTH + 1 ];

  // Commands:

  ~Timestamp();
  Timestamp(); // Current time UTC.
  Timestamp( const Timestamp& other );
  Timestamp( const long long seconds );
  Timestamp( const int yyyymmdd, const int hhmmss );
  Timestamp( const UTCString value );
  Timestamp& operator=( const Timestamp& other );
  void fromScanStartTime( const double value );
  void fromUTCString( const UTCString value );
  void yyyydddhhmmss( const int yyyyddd, const int hhmmss );
  void yyyymmddhhmmss( const int yyyymmdd, const int hhmmss );
  void offsetHours( const int hours );
  void offsetMonths( const int months );

  // Queries:

  static bool isYear( const int yyyy );
  static int dayOfYear( const int yyyy, const int mm, const int dd );
  static int dayOfWeek( const int yyyy, const int mm, const int dd );
  static int dayOfMonth( const int yyyy, const int ddd );
  static int monthOfYear( const int yyyy, const int ddd );
  static bool isUTCString( const UTCString value );
  static bool isYYYYDDD( const int yyyyddd );
  static bool isYYYYMMDD( const int yyyymmdd );
  static bool isHHMMSS( const int hhmmss );
  static int daysInYear( const int yyyy );
  static int daysInMonth( const int yyyy, const int mm );
  static int fileDateUTC( const char* fileName );
  static int timeZoneOffset( const char* timezoneName );

  void toUTCString( UTCString result ) const;
  long long seconds() const;
  int yyyyddd() const;
  int yyyymmdd() const;
  int hhmmss() const;
  Timestamp startOfHour() const;
  Timestamp endOfHour() const;
  Timestamp startOfDay() const;
  Timestamp endOfDay() const;
  Timestamp startOfMonth() const;
  Timestamp endOfMonth() const;
  Timestamp startOfYear() const;
  Timestamp endOfYear() const;
  int hoursUntil( const Timestamp& other ) const;
  int monthsUntil( const Timestamp& other ) const;
  bool operator==( const Timestamp& other ) const;
  bool operator!=( const Timestamp& other ) const;
  bool operator<=( const Timestamp& other ) const;
  bool operator>=( const Timestamp& other ) const;
  bool operator<( const Timestamp& other ) const;
  bool operator>( const Timestamp& other ) const;
  bool sameHour( const Timestamp& other ) const;
  bool sameDay( const Timestamp& other ) const;
  bool sameMonth( const Timestamp& other ) const;
  bool sameYear( const Timestamp& other ) const;
  bool invariant() const;
};


} // namespace emvl.

#endif /* TIMESTAMP_H */


