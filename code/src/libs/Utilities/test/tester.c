
/******************************************************************************
PURPOSE: tester.c - Tester for this library.
gcc -Wall -g -o tester tester.c -I..include -L../../../../lib/$platform \
    -lUtilities.debug.a -lnetcdf -lshp -lGPC -lPNG -lZ
HISTORY: 2018-11-26 plessel.todd@epa.gov
STATUS:  inchoate
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h>  /* For FILE, stderr, fprintf(), fopen(), fscanf(). */
#include <string.h> /* For strncpy(), memset(). */
#include <limits.h> /* For INT_MAX. */
#include <unistd.h> /* For unlink(). */

#include <Assertions.h>      /* For PRE*(), POST*(), DEBUG(), IMPLIES_ELSE().*/
#include <PNGFile.h>         /* For readPNGFile(). */
#include <DateTime.h>        /* For isValidYearMonthDay(). */
#include <Utilities.h>       /* For isValidLongitudeLatitude, copyFile(). */
#include <ImageFile.h>       /* For readImageFile(), etc. */
#include <MapFile.h>         /* For readMapFileHeader(), etc. */
#include <NetCDFUtilities.h> /* For createNetCDFFile(), etc. */
#include <Shapefile.h>       /* For readAndTriangulateShapes(), etc. */
#include <KMLFile.h>         /* For writePolygonsToKML(), etc. */


int main( void ) {
  int ok = 0;
  /* UNIMPLEMENTED */
  return ! ok;
}

