
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <QDebug>
#include <QString>
#include <QObject>
#include <QThread>
#include <QImage>
#include <QFile>

#include "loadPNG.h"
#include "ControlDockWidget.h"
#include "MainWin.h"
#include "RSIGGlobal.h"
#include "ModisTrueColor.h"

static const char* const command_prefix =
  "curl -k --silent --retry 0 -L --tcp-nodelay --max-time 120 "; // Timeout last

ModisTrueColor::ModisTrueColor() {
  urlString = "";
  pathedFilename = "";
  name = "";
}



ModisTrueColor::~ModisTrueColor() {
}
  


void ModisTrueColor::setUrlString( const QString& mystring ) {
  urlString = mystring;
}



void ModisTrueColor::setPathedFilename( const QString& mystring ) {
  pathedFilename = mystring;
}



void ModisTrueColor::setName( const QString& mystring ) {
  name = mystring;
}



double lon_to_EPSG3857(double lon) {
  // convert longitude in degrees to Web Mercator (EPSG:3857) X coordinate

  const double R       = 6378137.0; // WGS84 ellipsoid semi-major axis in meters
  const double deg2rad = M_PI / 180.0;
  const double refLon  = 0.0;
  double x =  R * deg2rad * (lon - refLon);
  return x;
}



double lat_to_EPSG3857(double lat) {
  // convert longitude in degrees to Web Mercator (EPSG:3857) Y coordinate

  const double R       = 6378137.0; // WGS84 ellipsoid semi-major axis in meters
  const double deg2rad = M_PI / 180.0;
  const double operand = (deg2rad * lat / 2.0) + (M_PI / 4.0);
  double y =  R * log(tan(operand));
  return y;
}




// Download to temp file. Then once it is done and verified, rename
// it to the desired filename. This is to prevent other threads from
// attempting to read the file while it is still downloading.

bool ModisTrueColor::downloadImage() {
  bool result = false;

  // set up input from curl

  QString command = command_prefix;

#ifdef _WIN32
  const char quote = '"';
  const char* const mode = "rb";
#else
  const char quote = '\'';
  const char* const mode = "r";
#endif
  
  command.append( quote ).append(urlString).append( quote );
  const std::string popenCommand( command.toStdString() );
  FILE* stream = popen( popenCommand.c_str(), mode );

  
  // set up output file on disk

  const QString tempFilename = pathedFilename + ".TMP";
  const std::string stdTempFileName( tempFilename.toStdString() );
  std::ofstream myfile( stdTempFileName.c_str(), std::ofstream::binary );

  // buffered read/write

  enum { BUFSIZE = 1024 * 1024 };
  std::vector<char> buffer( BUFSIZE ); //x
  char* const buf = &buffer[ 0 ];
  size_t totBytes = 0;

  while ( ! feof( stream ) ) {
    const size_t thisBytes = fread( buf, 1, BUFSIZE, stream );

    if ( thisBytes ) {
      myfile.write( buf, thisBytes );
      myfile.flush();
      totBytes += thisBytes;
    }
  }
    
  myfile.flush();
  myfile.close();
  pclose( stream );
    
  // make sure png is valid

  try {

    if ( totBytes > 1024 ) {
      QImage* testImage = new QImage(tempFilename);

      if (testImage->isNull()) {
        result = false;
        QFile thisFile(pathedFilename);
        thisFile.remove();
        emit error("True Color image not valid: " + tempFilename);
      } else {
        delete testImage;
        result = true;

        // rename the temp file to the desired filename

        int successFlag = 1; // default. 0 = it worked (unix)
        int counter = 0;
        int maxTries = 600;

        // Loop because Windows can only write one file at a time in a directory.
        // Needed in case other data is being requested.

        while ( counter < maxTries ) { // give it a certian time to complete

          if ( successFlag == 0 ) {
            break;
          }

          const std::string stdPathedFileName( pathedFilename.toStdString() );
          successFlag = rename( stdTempFileName.c_str(),
                                stdPathedFileName.c_str() );

          counter += 1;
          usleep(1 * 1000000); // in microseconds
        }

        if ( counter < maxTries ) {
          Q_EMIT requestRedraw();
        } else {
          emit message(name + " True Color not loaded.");
        }
      }

      // copy file instead of renaming, because rename often fails on Windows
      // this will ensure that the png is always available

    } else {
    result = false;
        emit error("True Color image not available.");
    }
  } catch (...) {
    result = false;
    emit error("True Color image error.");
  }
    
  emit finished();
  return result;
}



void ModisTrueColor::deleteLater() {
}



void ModisTrueColor::done() {
  MainWin& mainWin = MainWin::getInstance();  
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
  controlDockWidget->postMessage("Loaded " + pathedFilename, RSIGState::NORMAL_TEXT);
}



