/******************************************************************************
PURPOSE: RSIGGlobal.h - RSIG3D global vars and functions.

NOTES:

HISTORY: 2012/11/20, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef RSIGGLOBAL_H_
#define RSIGGLOBAL_H_

#include <Timestamp.h> // For emvl::Timestamp.

#include "Singleton.h"

#include <QObject> // For macro Q_ENUMS().
class QString;

//This application's name
#define APP_NAME "RSIG3D"

struct RSIGState {

  static const int INIT_WINWIDTH = 600;
  static const int INIT_WINHEIGHT = 600;

  /*
   * Store path (full or relative) to this executable.
   * Note: MacOSX 10.12 AppTranslocation feature causes
   * QCoreApplication::applicationDir() to return garbage! APPLE BUG.
   */

  static const char* program;
  static char directory[ 256 ];
  static const char* getProgram();
  static const char* getProgramDirectory();
  static void setLocation( const char* argv0 );

  Q_ENUMS(MapProjectionType);  
  Q_ENUMS(RsigMessageType);
  Q_ENUMS(CheckUpdateType);
  Q_ENUMS(TimebaseType);
  
  enum MapProjectionType
    {
      PROJECTION_LONGLAT               = 0,
      PROJECTION_SPHERICAL             = 1,
      PROJECTION_MERCATOR              = 2,
      PROJECTION_LAMBERTCONFORMALCONIC = 3,  
      PROJECTION_STEREOGRAPHIC         = 4  
    };
  
  enum RsigMessageType
    {
      NORMAL_TEXT      = 0,
      WARNING_TEXT     = 1,
      DEBUG_TEXT       = 2,
      ERROR_TEXT       = 3,
      FATAL_ERROR_TEXT = 4
    };
  
  enum CheckUpdateType
    {
     CHECK_NEVER = 0,
     CHECK_START = 1,
     CHECK_QUIT  = 2
    };

  enum TimebaseType
    {
     TIMEBASE_HOURLY    = 0,
     TIMEBASE_DAILY     = 1,
     TIMEBASE_DAILY_LST = 2,
     TIMEBASE_MONTHLY   = 3
    };
  
  int mDuration; // was nDays  
  emvl::Timestamp startingTimestamp;
  emvl::Timestamp endingTimestamp;

  int mTimebaseType;

  bool mShowLonlatLines;
  bool mShowScale;
  bool mShowLUTs;
  bool mShowNavigator;
  bool mShowDataProbe;
  enum CheckUpdateType mCheckUpdate;
  unsigned int mCounter;
    
  // Default constructor:

  RSIGState():
    mDuration(1)
    ,mTimebaseType(0)
    ,mShowLonlatLines(true)
    ,mShowScale(true)
    ,mShowLUTs(true)
    ,mShowNavigator(true)
    ,mShowDataProbe(false)
    ,mCheckUpdate(CHECK_QUIT)
    ,mCounter(0)
  {
  }

  void setTimebaseType( int timebaseType ) {
    mTimebaseType = timebaseType;
    updateEndingTimestamp();
  }
  
  void setDate( QString startingDate ) {
    const QString myStartTime( startingDate.replace("Z", "-0000") );
    const std::string copy( myStartTime.toStdString() );
    startingTimestamp.fromUTCString( copy.c_str() );
    updateEndingTimestamp();
   }

  void setDuration( const int duration ) {
    mDuration = duration;
    updateEndingTimestamp();
  }

  void updateEndingTimestamp();

  emvl::Timestamp getStartingTimestamp() const { return startingTimestamp; }
  
  emvl::Timestamp getEndingTimestamp() const { return endingTimestamp; }

  emvl::Timestamp getEndRenderTimestamp() const;

  emvl::Timestamp getCurrentTimestamp() const;
  

  // Types:

  enum { UTC_STRING_LENGTH = 20 }; // "YYYY-MM-DD HH:MM UTC"
  typedef char UTCString[ UTC_STRING_LENGTH + 1 ];

  // Outputs current timestamp as:
  // Hourly:  "YYYY-MM-DD HH:MM UTC"
  // Daily:   "YYYY-MM-DD UTC"
  // Monthly: "YYYY-MM UTC"
  
  void getCurrentUTCString( UTCString s ) const;
};


typedef Singleton<RSIGState> RSIGStateSingleton;
 

QString formatLonLat(double lon, double lat);
void displayProbe(QString probeMsg);
void displayProbe2(QString probeMsg2);


//================================================

int executableVersion(); // Get UTC yyyymmdd of the running program executable.

/*
 * Get UTC yyyymmdd of latest version of RSIG3D available from the remote EPA
 * server and return true if the remote version is newer than this running
 * program executable, else false.
*/

bool checkRsig3dUpdate(int& remoteVersion);

bool downloadUpdate(const QString& savefilename);


#endif //RSIGGLOBAL_H_
