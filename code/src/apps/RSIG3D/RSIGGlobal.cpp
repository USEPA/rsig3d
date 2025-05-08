/******************************************************************************
PURPOSE: RSIGGlobal.C - RSIG3D global vars.

NOTES:

HISTORY: 2013/3/25, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#include <stdio.h>  // For snprintf().
#include <string.h> // For strrchr().

#include <string> // For std::string.

#include <QString>

#include <Timestamp.h>      // For emvl::Timestamp::fileDateUTC().
#include <DatasetManager.h> // For emvl::DatasetManager::webservice().

#include "GetData.h"          // For downloadFile().
#include "RSIGGlobal.h"

/*
 * Store path (full or relative) to this executable.
 * Note: MacOSX 10.12 AppTranslocation feature causes
 * QCoreApplication::applicationDir() to return garbage! APPLE BUG.
 * Instead use RSIGState::getProgramDirectory().
 */

const char* RSIGState::program = "RSIG3D";
char RSIGState::directory[ 256 ] = ".";
const char* RSIGState::getProgram( void ) { return program; }
const char* RSIGState::getProgramDirectory( void ) { return directory; }

void RSIGState::setLocation( const char* argv0 ) {

  if ( argv0 && *argv0 ) {
    const char* end = strrchr( argv0, 'R' ); /* Point to last occurrence of RSIG3D. */
    program = argv0; /* Remember fully/partally/unpathed executable. */
    directory[ 0 ] = '.'; /* Default  to '.'. */
    directory[ 1 ] = '\0';

    if ( end != 0 ) {

      if ( end != argv0 ) {
        --end; /* Before "RSIG3D". */

        if ( end != argv0 ) {
          const char* input = argv0;
          char* output = directory;
          int count = sizeof directory / sizeof *directory - 1;

          while ( count-- && input < end ) {
            *output++ = *input++;
          }

          *output = '\0';
        }
      }
    }
  }
}


// Name of platform per compilation defined symbols:

static const char* platform() {
#if defined( __APPLE__ ) && defined( __x86_64__ )
  const char* const result = "Darwin.x86_64";
#elif defined( __APPLE__ ) && defined( __arm64__ )
  const char* const result = "Darwin.x86_64";
#elif defined( __linux__ ) && defined( __x86_64__ )
  const char* const result = "Linux_x86_64";
#elif defined( __linux__ ) && defined( __ia64__ )
  const char* const result = "Linux_ia64";
#elif defined( __linux__ ) && defined( __i386__ )
  const char* const result = "Linux_i686";
#elif defined( __MINGW64__ ) 
  const char* const result = "Win64"; // "Mingw64";
#elif defined( __CYGWIN__ )
  const char* const result = "Win32"; // "Cygwin";
#elif defined( __MINGW32__ ) 
  const char* const result = "Win32"; // "Mingw";
#elif defined( _MSC_VER ) 
  const char* const result = "Win32";
#else
#error "Unsupported platform"
#endif
  return result;
}


// Download newest RSIG3D_platform.zip file via rsigserver to $HOME/RSIG3D.zip

bool downloadUpdate( const QString& savefilename ) {
  std::string remoteFileName( "download/RSIG3D_" );
  remoteFileName += platform();
  remoteFileName += ".zip";
  const bool status =
    downloadFile( remoteFileName, savefilename.toStdString() );
  return status;
}



// Get UTC yyyymmdd of the running program executable:

int executableVersion() {
  const int result = emvl::Timestamp::fileDateUTC( RSIGState::getProgram() );
  return result;
}



// Is a newer version of this application available?

bool checkRsig3dUpdate( int& remoteVersion ) {
  remoteVersion = emvl::DatasetManager::remoteVersion() ;
  const int currentVersion = executableVersion();
  const bool result = remoteVersion > currentVersion;
  return result;
}



void RSIGState::getCurrentUTCString( UTCString utcString ) const {
  const emvl::Timestamp currentTimestamp( getCurrentTimestamp() );
  const int yyyymmdd = currentTimestamp.yyyymmdd();
  const int yyyy     = yyyymmdd / 10000;
  const int mm       = yyyymmdd / 100 % 100;
  const int dd       = yyyymmdd % 100;

  switch ( mTimebaseType ) {
  case TIMEBASE_HOURLY:
    {
      const int hhmmss = currentTimestamp.hhmmss();
      const int hh     = hhmmss / 10000;
      const int min    = hhmmss / 100 % 100;
      snprintf( utcString, sizeof (UTCString) / sizeof (char),
                "%04d-%02d-%02d %02d:%02d UTC", yyyy, mm, dd, hh, min );
    }
    break;
  case TIMEBASE_DAILY:
      snprintf( utcString, sizeof (UTCString) / sizeof (char),
                "      %04d-%02d-%02d UTC", yyyy, mm, dd );
    break;
  case TIMEBASE_DAILY_LST:
      snprintf( utcString, sizeof (UTCString) / sizeof (char),
                "      %04d-%02d-%02d", yyyy, mm, dd );
    break;
  case TIMEBASE_MONTHLY:
      snprintf( utcString, sizeof (UTCString) / sizeof (char),
                "         %04d-%02d UTC", yyyy, mm );
    break;
  default:
    utcString[ 0 ] = '\0';
    break;
  }
}



emvl::Timestamp RSIGState::getCurrentTimestamp() const {
  emvl::Timestamp result( startingTimestamp );

  switch ( mTimebaseType ) {
  case RSIGState::TIMEBASE_HOURLY:
    result.offsetHours( mCounter );
    break;
  case RSIGState::TIMEBASE_DAILY:
    result.offsetHours( mCounter * 24 );
    break;
  case RSIGState::TIMEBASE_DAILY_LST:
    result.offsetHours( mCounter * 24 );
    break;
  case RSIGState::TIMEBASE_MONTHLY:
    result.offsetMonths( mCounter );
    break;
  default:
    break;
  }
  
  return result;
}



void RSIGState::updateEndingTimestamp() {
  endingTimestamp = startingTimestamp;

  switch ( mTimebaseType ) {
  case TIMEBASE_HOURLY:
    endingTimestamp.offsetHours( mDuration * 24 - 1 ); // Retrieve at least 24 hours.
    break;
  case TIMEBASE_DAILY:
    endingTimestamp.offsetHours( mDuration * 24 - 1 );
    break;
  case TIMEBASE_DAILY_LST:
    endingTimestamp.offsetHours( mDuration * 24 - 1 );
    break;
  case TIMEBASE_MONTHLY:
    endingTimestamp.offsetMonths( mDuration - 1 );
    endingTimestamp = endingTimestamp.endOfMonth();
    break;
  default:
    break;
  }

  endingTimestamp = endingTimestamp.endOfHour();
}



emvl::Timestamp RSIGState::getEndRenderTimestamp() const {
  emvl::Timestamp endTime( getCurrentTimestamp() );

  if ( mTimebaseType == RSIGState::TIMEBASE_DAILY ) {
    endTime = endTime.endOfDay();
  } else if ( mTimebaseType == RSIGState::TIMEBASE_DAILY_LST ) {
    endTime = endTime.endOfDay();
  } else if ( mTimebaseType == RSIGState::TIMEBASE_MONTHLY ) {
    endTime = endTime.endOfMonth();
  }

  endTime = endTime.endOfHour();
  return endTime;
}

