
/******************************************************************************
PURPOSE: MapFile.h - Define routines to read .bin map polyline files.
HISTORY: 2018-11-24 plessel.todd@epa.gov
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h> /* For FILE, stderr, fprintf(), fread(). */

#include <Assertions.h> /* For PRE*(), POST*(), CHECK*(), AND*(). */
#include <Utilities.h>  /* For isValidBounds(). */
#include <MapFile.h>    /* For public interface. */


/*================================ MACROS =================================*/

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

/*=========================== FORWARD DECLARATIONS ==========================*/

static int clipLine( double wxl, double wyl, double wxu, double wyu,
                     double* x1, double* y1, double* x2, double*y2 );

static int clipCoordinate( double p, double q, double* t1, double* t2 );

static int nonCoincident( double longitude1, double latitude1,
                          double longitude2, double latitude2,
                          double tolerance );

static int minimumItemi( const int array[], int count );

static int sum( const int array[], int count );

static int inBoundsFloat( const float vertices[], int count,
                          const double bounds[ 2 ][ 2 ] );

static int inBoundsDouble( const double vertices[], int count,
                           const double bounds[ 2 ][ 2 ] );

static void rotate4ByteArrayIfLittleEndian( void* array, int count );

/*============================ PUBLIC FUNCTIONS =============================*/



/******************************************************************************
PURPOSE: readMapFileHeader - Read header dimensions of map_*.bin file.
INPUTS:  FILE* file  Opened readable file.
OUTPUTS: int* polylineCount  Number of polylines in file.
         int* vertexCount    Number of lon-lat vertices in file.
RETURNS: int 1 if successful, else 0 and failureMessage is called.
******************************************************************************/

int readMapFileHeader( FILE* file, int* polylineCount, int* vertexCount ) {
  PRE03( file, polylineCount, vertexCount );
  const int result =
    fscanf( file, "%*[^\n]\n%*[^\n]\n%d %d\n%*[^\n]%*c",
            polylineCount, vertexCount ) == 2 &&
    *polylineCount > 0 && *vertexCount > 0;

  if ( ! result ) {
    fprintf( stderr, "\nInvalid map file header.\n" );
    *polylineCount = *vertexCount = 0;
  }

  POST0( IMPLIES_ELSE( result,
                      GT_ZERO2( *polylineCount, *vertexCount ),
                      IS_ZERO2( *polylineCount, *vertexCount ) ) );
  return result;
}



/******************************************************************************
PURPOSE: readMapFileData - Read data of map_*.bin file.
INPUTS:  FILE* file  Opened readable file, seeked to data.
         int polylineCount  Number of polylines in file.
         int vertexCount    Number of lon-lat vertex pairs in file.
OUTPUTS: int counts[ polylineCount ]  Array of vertex counts per polyline.
         float vertices[ vertexCount * 2 ] Array of lon-lat vertices.
RETURNS: int 1 if successful, else 0.
******************************************************************************/

int readMapFileData( FILE* file,
                     int polylineCount, int vertexCount,
                     int counts[], float vertices[] ) {
  PRE05( file, polylineCount > 0, vertexCount > 0, counts, vertices );
  assert_static( sizeof (int) == 4 );
  assert_static( sizeof (float) == 4 );
  const int vertexValues = vertexCount + vertexCount;
  int result = fread( counts, polylineCount * sizeof (int), 1, file ) == 1;

  if ( result ) {
    result = fread( vertices, vertexValues * sizeof (float), 1, file ) == 1;

    if ( result ) {
      rotate4ByteArrayIfLittleEndian( counts, polylineCount );
      rotate4ByteArrayIfLittleEndian( vertices, vertexValues );
    }
  }

  if ( result ) {
    int index = 0;

    for ( index = 0; AND2( result, index < polylineCount ); ++index ) {
      result = counts[ index ] > 0;
   }

    for ( index = 0; AND2( result, index < vertexCount ); ++index ) {
      const int index2 = index + index;
      const float longitude = vertices[ index2 ];
      const float latitude = vertices[ index2 + 1 ];
      result = AND2( IN_RANGE( longitude, -180.0, 180.0 ),
                     IN_RANGE( latitude,   -90.0,  90.0 ) );
    }
  }

  if ( ! result ) {
    fprintf( stderr, "\nInvalid map file data.\n" );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: subsetMapFloat - Count and clip polylines to bounds.
INPUTS:  int inputPolylineCount  Number of polylines in input data.
         int inputVertexCount    Number of vertices in input data.
         const int inputCounts[inputPolylines] Vertex count per input polyline.
         const float inputVertices[ inputVertexCount * 2 ] Input vertices.
OUTPUTS: double resolution  Minimum distance between included points.
         const double bounds[ 2 ][ 2 ]  Longitude-latitude bounds to include.
                           bounds[ LONGITUDE, LATITUDE ][ MINIMUM, MAXIMUM ].
         int* outputPolylineCount  Number of polylines in output data.
         int* outputVertexCount    Number of vertices in output data.
         int outputCounts[outputPolylines] Vertex count per output polyline.
         float outputVertices[ outputVertexCount * 2 ] Output vertices if non-0
******************************************************************************/

void subsetMapFloat( int inputPolylineCount,
                     int inputVertexCount,
                     const int inputCounts[],
                     const float inputVertices[],
                     double resolution,
                     const double bounds[ 2 ][ 2 ],
                     int* outputPolylineCount,
                     int* outputVertexCount,
                     int outputCounts[],
                     float outputVertices[] ) {

  PRE012( inputPolylineCount > 0,
          inputVertexCount > 0,
          inputCounts,
          inputCounts[ 0 ] > 0,
          inputCounts[ inputPolylineCount - 1 ] > 0,
          inputVertices,
          IN_RANGE( inputVertices[ 0 ], -180.0, 180.0 ),
          IN_RANGE( inputVertices[ 1 ],  -90.0,  90.0 ),
          resolution >= 0.0,
          isValidBounds( (const double (*)[2]) bounds ),
          outputPolylineCount,
          outputVertexCount );

  const double longitudeMinimum = bounds[ LONGITUDE ][ MINIMUM ];
  const double longitudeMaximum = bounds[ LONGITUDE ][ MAXIMUM ];
  const double latitudeMinimum  = bounds[ LATITUDE  ][ MINIMUM ];
  const double latitudeMaximum  = bounds[ LATITUDE  ][ MAXIMUM ];
  const float* inputLongitudes  = inputVertices;
  const float* inputLatitudes   = inputVertices + 1;
  float* outputLongitudes = outputVertices;
  float* outputLatitudes  = outputVertices ? outputVertices + 1 : 0;
  int outputPolyline = 0;
  int polyline = 0;
  double lastStoredLongitude = -999.0; /* Sentinel value. */
  double lastStoredLatitude  = -999.0;

  *outputPolylineCount = *outputVertexCount = 0; /* Initialize outputs. */

  if ( outputCounts ) {
    outputCounts[ 0 ] = 0;
  }

  if ( outputVertices ) {
    outputVertices[ 0 ] = outputVertices[ 1 ] = 0.0;
  }

  DEBUG( fprintf( stderr, "\nsubset( %d polylines %d vertices ):\n",
                  inputPolylineCount, inputVertexCount ); )

  for ( polyline = 0; polyline < inputPolylineCount; ++polyline ) {
    const int thisPolylineVertexCount = inputCounts[ polyline ];
    int vertex = 0;
    double longitude1 = *inputLongitudes;
    double latitude1  = *inputLatitudes;
    inputLongitudes += 2;
    inputLatitudes  += 2;

    DEBUG2( fprintf( stderr, "polyline # %6d, vertex count = %6d\n",
                     polyline, thisPolylineVertexCount ); )

    for ( vertex = 1; vertex < thisPolylineVertexCount; ++vertex ) {
      const double longitude2 = *inputLongitudes;
      const double latitude2  = *inputLatitudes;
      double clippedLongitude1 = longitude1;
      double clippedLatitude1  = latitude1;
      double clippedLongitude2 = longitude2;
      double clippedLatitude2  = latitude2;
      inputLongitudes += 2;
      inputLatitudes  += 2;

      DEBUG2( fprintf( stderr, "before (%g, %g)---(%g, %g)\n",
                       clippedLongitude1, clippedLatitude1,
                       clippedLongitude2, clippedLatitude2 ); )

      if ( AND2( OR2( resolution == 0.0,
                      nonCoincident( longitude1, latitude1,
                                     longitude2, latitude2,
                                     resolution ) ),
                 clipLine( longitudeMinimum, latitudeMinimum,
                           longitudeMaximum, latitudeMaximum,
                           &clippedLongitude1, &clippedLatitude1,
                           &clippedLongitude2, &clippedLatitude2 ) ) ) {

        const int discontiguous =
          clippedLongitude1 != lastStoredLongitude ||
          clippedLatitude1  != lastStoredLatitude;
        const int addedVertices = 1 + discontiguous;

        DEBUG2( fprintf( stderr, "after  (%g, %g)---(%g, %g)\n",
                         clippedLongitude1, clippedLatitude1,
                         clippedLongitude2, clippedLatitude2 ); )
        DEBUG2( fprintf( stderr, "discontiguous = %d\n", discontiguous );)
        DEBUG2( fprintf( stderr, "addedVertices = %d\n", addedVertices );)
        CHECK( IN_RANGE( clippedLongitude1,
                         bounds[ LONGITUDE ][ MINIMUM ],
                         bounds[ LONGITUDE ][ MAXIMUM ] ) );
        CHECK( IN_RANGE( clippedLongitude2,
                         bounds[ LONGITUDE ][ MINIMUM ],
                         bounds[ LONGITUDE ][ MAXIMUM ] ) );
        CHECK( IN_RANGE( clippedLatitude1,
                         bounds[ LATITUDE ][ MINIMUM ],
                         bounds[ LATITUDE ][ MAXIMUM ] ) );
        CHECK( IN_RANGE( clippedLatitude2,
                         bounds[ LATITUDE ][ MINIMUM ],
                         bounds[ LATITUDE ][ MAXIMUM ] ) );

        if ( outputVertices ) {

          if ( addedVertices == 2 ) {
            *outputLongitudes = clippedLongitude1;
            outputLongitudes += 2;
            *outputLatitudes  = clippedLatitude1;
            outputLatitudes  += 2;
            *outputLongitudes = clippedLongitude2;
            outputLongitudes += 2;
            *outputLatitudes  = clippedLatitude2;
            outputLatitudes  += 2;
          } else {
            *outputLongitudes = clippedLongitude2;
            outputLongitudes += 2;
            *outputLatitudes  = clippedLatitude2;
            outputLatitudes  += 2;
          }
        }

        *outputVertexCount += addedVertices;

        if ( discontiguous && lastStoredLongitude > -900.0 ) {
          ++outputPolyline;
          DEBUG2(fprintf(stderr,"outputPolyline index = %d\n",outputPolyline);)

          if ( outputCounts ) {
            outputCounts[ outputPolyline ] = 0;
          }
        }

        if ( outputCounts ) {
          outputCounts[ outputPolyline ] += addedVertices;
          DEBUG2( fprintf( stderr, "outputCounts[ %6d ] = %6d\n",
                           outputPolyline, outputCounts[ outputPolyline ] ); )
          CHECK( outputCounts[ outputPolyline ] >= 2 );
        }

        DEBUG2( fprintf( stderr, "total = %6d\n", *outputVertexCount ); )

        lastStoredLongitude = clippedLongitude2;
        lastStoredLatitude  = clippedLatitude2;

        DEBUG2( fprintf( stderr, "lastStored = (%g, %g)\n",
                         lastStoredLongitude, lastStoredLatitude ); )
      }

      longitude1 = longitude2;
      latitude1  = latitude2;

    } /* End loop on this polyline's vertices. */
  } /* End loop on polylines. */

  *outputPolylineCount = outputPolyline + ( *outputVertexCount != 0 );

  if ( *outputPolylineCount == 0 ) {
    *outputVertexCount = 0;

    if ( outputCounts ) {
      outputCounts[ 0 ] = 0;
    }

    if ( outputVertices ) {
      outputVertices[ 0 ] = outputVertices[ 1 ] = 0.0;
    }
  }

  DEBUG( fprintf( stderr, "*outputPolylineCount = %d\n",*outputPolylineCount);)
  DEBUG( fprintf( stderr, "*outputVertexCount = %d\n",*outputVertexCount);)
  CHECK( IMPLIES( AND2( *outputPolylineCount > 0, outputVertices ),
                  inBoundsFloat( outputVertices, *outputVertexCount,
                                 (const double (*)[2]) bounds ) ) );

  POST0( IMPLIES_ELSE( *outputPolylineCount == 0, *outputVertexCount == 0,
           AND3( *outputVertexCount >= 2,
                 IMPLIES( outputCounts,
                          AND2( minimumItemi( outputCounts,
                                              *outputPolylineCount ) >= 2,
                                sum( outputCounts, *outputPolylineCount )
                                  == *outputVertexCount ) ),
                 IMPLIES( outputVertices,
                          inBoundsFloat( outputVertices, *outputVertexCount,
                                         (const double (*)[2]) bounds ) ) ) ) );
}



/******************************************************************************
PURPOSE: subsetMapDouble - Count and clip polylines to bounds.
INPUTS:  int inputPolylineCount  Number of polylines in input data.
         int inputVertexCount    Number of vertices in input data.
         const int inputCounts[inputPolylines] Vertex count per input polyline.
         const double inputVertices[ inputVertexCount * 2 ] Input vertices.
OUTPUTS: double resolution  Minimum distance between included points.
         const double bounds[ 2 ][ 2 ]  Longitude-latitude bounds to include.
                           bounds[ LONGITUDE, LATITUDE ][ MINIMUM, MAXIMUM ].
         int* outputPolylineCount  Number of polylines in output data.
         int* outputVertexCount    Number of vertices in output data.
         int outputCounts[outputPolylines] Vertex count per output polyline.
         double outputVertices[ outputVertexCount * 2] Output vertices if non-0
******************************************************************************/

void subsetMapDouble( int inputPolylineCount,
                      int inputVertexCount,
                      const int inputCounts[],
                      const double inputVertices[],
                      double resolution,
                      const double bounds[ 2 ][ 2 ],
                      int* outputPolylineCount,
                      int* outputVertexCount,
                      int outputCounts[],
                      double outputVertices[] ) {

  PRE012( inputPolylineCount > 0,
          inputVertexCount > 0,
          inputCounts,
          inputCounts[ 0 ] > 0,
          inputCounts[ inputPolylineCount - 1 ] > 0,
          inputVertices,
          IN_RANGE( inputVertices[ 0 ], -180.0, 180.0 ),
          IN_RANGE( inputVertices[ 1 ],  -90.0,  90.0 ),
          resolution >= 0.0,
          isValidBounds( (const double (*)[2]) bounds ),
          outputPolylineCount,
          outputVertexCount );

  const double longitudeMinimum = bounds[ LONGITUDE ][ MINIMUM ];
  const double longitudeMaximum = bounds[ LONGITUDE ][ MAXIMUM ];
  const double latitudeMinimum  = bounds[ LATITUDE  ][ MINIMUM ];
  const double latitudeMaximum  = bounds[ LATITUDE  ][ MAXIMUM ];
  const double* inputLongitudes  = inputVertices;
  const double* inputLatitudes   = inputVertices + 1;
  double* outputLongitudes = outputVertices;
  double* outputLatitudes  = outputVertices ? outputVertices + 1 : 0;
  int outputPolyline = 0;
  int polyline = 0;
  double lastStoredLongitude = -999.0; /* Sentinel value. */
  double lastStoredLatitude  = -999.0;

  *outputPolylineCount = *outputVertexCount = 0; /* Initialize outputs. */

  if ( outputCounts ) {
    outputCounts[ 0 ] = 0;
  }

  DEBUG( fprintf( stderr, "\nsubset( %d polylines %d vertices ):\n",
                  inputPolylineCount, inputVertexCount ); )

  for ( polyline = 0; polyline < inputPolylineCount; ++polyline ) {
    const int thisPolylineVertexCount = inputCounts[ polyline ];
    int vertex = 0;
    double longitude1 = *inputLongitudes;
    double latitude1  = *inputLatitudes;
    inputLongitudes += 2;
    inputLatitudes  += 2;

    DEBUG2( fprintf( stderr, "polyline # %6d, vertex count = %6d\n",
                     polyline, thisPolylineVertexCount ); )

    for ( vertex = 1; vertex < thisPolylineVertexCount; ++vertex ) {
      const double longitude2 = *inputLongitudes;
      const double latitude2  = *inputLatitudes;
      double clippedLongitude1 = longitude1;
      double clippedLatitude1  = latitude1;
      double clippedLongitude2 = longitude2;
      double clippedLatitude2  = latitude2;
      inputLongitudes += 2;
      inputLatitudes  += 2;

      DEBUG2( fprintf( stderr, "before (%lg, %lg)---(%lg, %lg)\n",
                       clippedLongitude1, clippedLatitude1,
                       clippedLongitude2, clippedLatitude2 ); )

      if ( AND2( OR2( resolution == 0.0,
                      nonCoincident( longitude1, latitude1,
                                     longitude2, latitude2,
                                     resolution ) ),
                 clipLine( longitudeMinimum, latitudeMinimum,
                           longitudeMaximum, latitudeMaximum,
                           &clippedLongitude1, &clippedLatitude1,
                           &clippedLongitude2, &clippedLatitude2 ) ) ) {

        const int discontiguous =
          clippedLongitude1 != lastStoredLongitude ||
          clippedLatitude1  != lastStoredLatitude;
        const int addedVertices = 1 + discontiguous;

        DEBUG2( fprintf( stderr, "after  (%lg, %lg)---(%lg, %lg)\n",
                         clippedLongitude1, clippedLatitude1,
                         clippedLongitude2, clippedLatitude2 ); )
        DEBUG2( fprintf( stderr, "discontiguous = %d\n", discontiguous );)
        DEBUG2( fprintf( stderr, "addedVertices = %d\n", addedVertices );)
        CHECK( IN_RANGE( clippedLongitude1,
                         bounds[ LONGITUDE ][ MINIMUM ],
                         bounds[ LONGITUDE ][ MAXIMUM ] ) );
        CHECK( IN_RANGE( clippedLongitude2,
                         bounds[ LONGITUDE ][ MINIMUM ],
                         bounds[ LONGITUDE ][ MAXIMUM ] ) );
        CHECK( IN_RANGE( clippedLatitude1,
                         bounds[ LATITUDE ][ MINIMUM ],
                         bounds[ LATITUDE ][ MAXIMUM ] ) );
        CHECK( IN_RANGE( clippedLatitude2,
                         bounds[ LATITUDE ][ MINIMUM ],
                         bounds[ LATITUDE ][ MAXIMUM ] ) );

        if ( outputVertices ) {

          if ( addedVertices == 2 ) {
            *outputLongitudes = clippedLongitude1;
            outputLongitudes += 2;
            *outputLatitudes  = clippedLatitude1;
            outputLatitudes  += 2;
            *outputLongitudes = clippedLongitude2;
            outputLongitudes += 2;
            *outputLatitudes  = clippedLatitude2;
            outputLatitudes  += 2;
          } else {
            *outputLongitudes = clippedLongitude2;
            outputLongitudes += 2;
            *outputLatitudes  = clippedLatitude2;
            outputLatitudes  += 2;
          }
        }

        *outputVertexCount += addedVertices;

        if ( discontiguous && lastStoredLongitude > -900.0 ) {
          ++outputPolyline;
          DEBUG2(fprintf(stderr,"outputPolyline index = %d\n",outputPolyline);)

          if ( outputCounts ) {
            outputCounts[ outputPolyline ] = 0;
          }
        }

        if ( outputCounts ) {
          outputCounts[ outputPolyline ] += addedVertices;
          DEBUG2( fprintf( stderr, "outputCounts[ %6d ] = %6d\n",
                           outputPolyline, outputCounts[ outputPolyline ] ); )
          CHECK( outputCounts[ outputPolyline ] >= 2 );
        }

        DEBUG2( fprintf( stderr, "total = %6d\n", *outputVertexCount ); )

        lastStoredLongitude = clippedLongitude2;
        lastStoredLatitude  = clippedLatitude2;

        DEBUG2( fprintf( stderr, "lastStored = (%lg, %lg)\n",
                         lastStoredLongitude, lastStoredLatitude ); )
      }

      longitude1 = longitude2;
      latitude1  = latitude2;

    } /* End loop on this polyline's vertices. */
  } /* End loop on polylines. */

  *outputPolylineCount = outputPolyline + ( *outputVertexCount != 0 );

  if ( *outputPolylineCount == 0 ) {
    *outputVertexCount = 0;

    if ( outputCounts ) {
      outputCounts[ 0 ] = 0;
    }

    if ( outputVertices ) {
      outputVertices[ 0 ] = outputVertices[ 1 ] = 0.0;
    }
  }

  DEBUG( fprintf( stderr, "*outputPolylineCount = %d\n",*outputPolylineCount);)
  DEBUG( fprintf( stderr, "*outputVertexCount = %d\n",*outputVertexCount);)
  CHECK( IMPLIES( AND2( *outputPolylineCount > 0, outputVertices ),
                  inBoundsDouble( outputVertices, *outputVertexCount,
                                 (const double (*)[2]) bounds ) ) );

  POST0( IMPLIES_ELSE( *outputPolylineCount == 0, *outputVertexCount == 0,
           AND3( *outputVertexCount >= 2,
                 IMPLIES( outputCounts,
                          AND2( minimumItemi( outputCounts,
                                              *outputPolylineCount ) >= 2,
                                sum( outputCounts, *outputPolylineCount )
                                  == *outputVertexCount ) ),
                 IMPLIES( outputVertices,
                          inBoundsDouble( outputVertices, *outputVertexCount,
                                          (const double (*)[2]) bounds )))));
}



/*============================ PRIVATE FUNCTIONS ============================*/



/******************************************************************************
PURPOSE: clipLine - Check and clip a line segment to a rectangle window.
INPUTS:  double wxl  Window x-lower coordinate.
         double wyl  Window y-lower coordinate.
         double wxu  Window x-upper coordinate.
         double wyu  Window y-upper coordinate.
         double* x1  (x1, y1)-(x2, y2) is the line segment to clip.
         double* y1
         double* x2
         double* y2
OUTPUTS: double* x1  (x1, y1)-(x2, y2) is the clipped line segment.
         double* y1
         double* x2
         double* y2
RETURNS: int 1 if line segment intersects the rectangle.
NOTES:   Uses the Liang-Barsky Algorithm.
******************************************************************************/

static int clipLine( double wxl, double wyl, double wxu, double wyu,
                     double* x1, double* y1, double* x2, double* y2 ) {
  int result = 0;
  const double dx = *x2 - *x1;
  double t1 = 0.0; /* t holds new start point. */
  double t2 = 1.0;

  /* Check boundaries: left, right, bottom, top: */

  if ( clipCoordinate( -dx, *x1 - wxl, &t1, &t2 ) ) { /* left. */

    if ( clipCoordinate( dx, wxu - *x1, &t1, &t2 ) ) { /* right. */
      const double dy = *y2 - *y1;

      if ( clipCoordinate( -dy, *y1 - wyl, &t1, &t2 ) ) { /* bottom. */

        if ( clipCoordinate( dy, wyu - *y1, &t1, &t2 ) ) { /* top. */

          /*
           * At least some of the line is within the window so
           * calculate the new end and start points (in that order).
           */

          if ( t2 < 1.0 ) { /* Calculate new end point first. */
            *x2 = *x1 + t2 * dx;
            *y2 = *y1 + t2 * dy;
          }

          if ( t1 > 0.0 ) { /* Calculate new start point. */
            *x1 += t1 * dx;
            *y1 += t1 * dy;
          }

          result = 1; /* Successfully clipped. */
        }
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: clipCoordinate - Check and clip a line segment to a boundary.
INPUTS:  double p
         double q
         double* t1
         double* t2
OUTPUTS: double* t1
         double* t2
RETURNS: int 1 if point crosses the boundary.
NOTES:   Part of the Liang-Barsky Algorithm.
******************************************************************************/

static int clipCoordinate( double p, double q, double* t1, double* t2 ) {
  int result = 1; /* Clipped. */

  if ( p < 0.0 ) { /* Line from outside to inside of that boundary. */
    const double r = q / p; /* Intersection coordinate. */

    if ( r > *t2 ) {
      result = 0; /* Intersection past segment end point. */
    } else if ( r > *t1 ) {
      *t1 = r; /* Intersection is past start point. */
    }
  } else if ( p > 0.0 ) { /* Line from inside to outside of that boundary. */
    const double r = q / p;

    if ( r < *t1 ) {
      result = 0; /* Intersection is before start point. */
    } else if ( r < *t2 ) {
      *t2 = r; /* Intersection is before end point. */
    }
  } else if ( q < 0.0 ) { /* p == 0.0. */
    result = 0; /* Line is parallel to that boundary. */
  }

  return result;
}



/******************************************************************************
PURPOSE: nonCoincident - Are the two points more than tolerance apart?
INPUTS:  double longitude1  Longitude of 1st point to compare.
         double latitude1   Latitude  of 1st point to compare.
         double longitude2  Longitude of 2nd point to compare.
         double latitude2   Latitude  of 2nd point to compare.
         double tolerance   Minimum distance between points considered
                            non-coincident.
RETURNS: int 1 if both coordinates of points are more than tolerance apart.
******************************************************************************/

static int nonCoincident( double longitude1, double latitude1,
                          double longitude2, double latitude2,
                          double tolerance ) {
  const double deltaLongitude = longitude1 - longitude2;
  const double deltaLatitude  = latitude1  - latitude2;
  const int result =
    deltaLongitude > tolerance || deltaLongitude < -tolerance ||
    deltaLatitude  > tolerance || deltaLatitude  < -tolerance;
  return result;
}



/******************************************************************************
PURPOSE: minimumItemi - Minimum value in array.
INPUTS:  const int array[ count ]  Values to check.
         int count                 Number of values in array.
RETURNS: int minimum value in array.
******************************************************************************/

static int minimumItemi( const int array[], int count ) {
  int result = array[ 0 ];
  int index = 0;

  for ( index = 1; index < count; ++index ) {
    const int value = array[ index ];

    if ( value < result ) {
      result = value;
    }
  }

  DEBUG( fprintf( stderr, "minimumItemi = %d\n", result ); )
  return result;
}



/******************************************************************************
PURPOSE: sum - Sum of values in array.
INPUTS:  const int array[ count ]  Values to sum.
         int count                 Number of values in array.
RETURNS: int sum of array values.
******************************************************************************/

static int sum( const int array[], int count ) {
  int result = 0;
  int index = 0;

  for ( index = 0; index < count; ++index ) {
    result += array[ index ];
  }

  DEBUG( fprintf( stderr, "sum = %d\n", result ); )
  return result;
}



/******************************************************************************
PURPOSE: inBoundsFloat - Are all vertices within bounds?
INPUTS:  const float vertices[ count ][ 2 ]  Lon-lat vertices to check.
         int count                           Number of vertices.
         const double bounds[ LONGITUDE, LATITUDE ][ MINIMUM, MAXIMUM ].
RETURNS: int 1 if the vertices are all within bounds, else 0.
******************************************************************************/

static int inBoundsFloat( const float vertices[], int count,
                          const double bounds[ 2 ][ 2 ] ) {
  PRE03( vertices, count > 0, isValidBounds( bounds ) );
  const double tolerance = 1e-3;
  const double longitudeMinimum = bounds[ LONGITUDE ][ MINIMUM ];
  const double longitudeMaximum = bounds[ LONGITUDE ][ MAXIMUM ];
  const double latitudeMinimum  = bounds[ LATITUDE  ][ MINIMUM ];
  const double latitudeMaximum  = bounds[ LATITUDE  ][ MAXIMUM ];
  const float* longitudes = vertices;
  const float* latitudes  = vertices + 1;
  int vertex = 0;
  int result = 1;

  for ( vertex = 0; vertex < count && result; ++vertex,
        longitudes += 2, latitudes += 2 ) {
    result =
      IN_RANGE( *longitudes,
                longitudeMinimum - tolerance,
                longitudeMaximum + tolerance ) &&
      IN_RANGE( *latitudes,
                latitudeMinimum - tolerance,
                latitudeMaximum + tolerance );

#ifdef DEBUGGING
    if ( ! result ) {
      fprintf( stderr, "Invalid vertex: index %d (%f, %f)\n",
               vertex, *longitudes, *latitudes ) ;
    }
#endif
  }

  DEBUG( fprintf( stderr, "inBoundsFloat = %d\n", result ); )
  return result;
}



/******************************************************************************
PURPOSE: inBoundsDouble - Are all vertices within bounds?
INPUTS:  const double vertices[ count ][ 2 ]  Lon-lat vertices to check.
         int count                           Number of vertices.
         const double bounds[ LONGITUDE, LATITUDE ][ MINIMUM, MAXIMUM ].
RETURNS: int 1 if the vertices are all within bounds, else 0.
******************************************************************************/

static int inBoundsDouble( const double vertices[], int count,
                           const double bounds[ 2 ][ 2 ] ) {
  PRE03( vertices, count > 0, isValidBounds( bounds ) );
  const double tolerance = 1e-3;
  const double longitudeMinimum = bounds[ LONGITUDE ][ MINIMUM ];
  const double longitudeMaximum = bounds[ LONGITUDE ][ MAXIMUM ];
  const double latitudeMinimum  = bounds[ LATITUDE  ][ MINIMUM ];
  const double latitudeMaximum  = bounds[ LATITUDE  ][ MAXIMUM ];
  const double* longitudes = vertices;
  const double* latitudes  = vertices + 1;
  int vertex = 0;
  int result = 1;

  for ( vertex = 0; vertex < count && result; ++vertex,
        longitudes += 2, latitudes += 2 ) {
    result =
      IN_RANGE( *longitudes,
                longitudeMinimum - tolerance,
                longitudeMaximum + tolerance ) &&
      IN_RANGE( *latitudes,
                latitudeMinimum - tolerance,
                latitudeMaximum + tolerance );

#ifdef DEBUGGING
    if ( ! result ) {
      fprintf( stderr, "Invalid vertex: index %d (%f, %f)\n",
               vertex, *longitudes, *latitudes ) ;
    }
#endif
  }

  DEBUG( fprintf( stderr, "inBoundsDouble = %d\n", result ); )
  return result;
}



/******************************************************************************
PURPOSE: rotate4ByteArrayIfLittleEndian() - Rotate 4-bytes of each array item
         if on a little-endian platform.
INPUTS:  void* array      Array of 4-byte values to rotate.
         int count  Number of items in array.
OUTPUTS: void* array      Array of rotated values.
******************************************************************************/

static void rotate4ByteArrayIfLittleEndian( void* array, int count ) {

#if IS_LITTLE_ENDIAN

  PRE02( array, count > 0 );
  assert_static( sizeof (int) == 4 );
  int* const array4 = array;
  int index = 0;

#pragma omp parallel for

  for ( index = 0; index < count; ++index ) {
    const int value = array4[ index ];
    const int newValue =
      ( value & 0xff000000 ) >> 24 |
      ( value & 0x00ff0000 ) >>  8 |
      ( value & 0x0000ff00 ) <<  8 |
      ( value & 0x000000ff ) << 24;
    array4[ index ] = newValue;
  }

#endif /* IS_LITTLE_ENDIAN */

}


