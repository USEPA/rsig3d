/*
gcc -Wall -g -o test/test_subsetMap test/test_subsetMap.c \
    -I. ../../../lib/Darwin/libUtilities.debug.a -lm
test/test_subsetMap
*/

#include <stdio.h>     /* For printf(). */
#include <string.h>    /* For memset(). */
#include <Utilities.h> /* For subsetMap(). */

static void test1( void ) {
  const int inputPolylineCount = 1;
  const int inputCounts[ 1 ] = { 5 };
  const int inputVertexCount = 5;
  const float inputVertices[] = {
    2.0, -4.0,
    4.0,  0.0,
/*  2.0,  4.0, */
    7.0,  6.0,
/* -4.0,  2.0, */
   -5.0,  4.0,
   -1.0, -4.0
  };
  const double resolution = 0.0;
  const double bounds[ 2 ][ 2 ] = { { -3.0, 3.0 }, { -3.0, 3.0 } };
  int outputPolylineCount = 0;
  int outputVertexCount = 0;
  int outputCounts[ 4 ] = { 0, 0, 0, 0 };
  float outputVertices[ 8 * 2 ] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
  };
  int polyline = 0;
  int vertex2 = 0;
  printf( "\ntest1:\n\n" );

  subsetMap( inputPolylineCount, inputVertexCount,
             inputCounts, inputVertices,
             resolution, bounds,
             &outputPolylineCount, &outputVertexCount,
             outputCounts, outputVertices );

  printf( "outputPolylineCount = %d, outputVertexCount = %d\n",
          outputPolylineCount, outputVertexCount );

  for ( polyline = 0; polyline < outputPolylineCount; ++polyline ) {
    const int polylineVertices = outputCounts[ polyline ];
    int vertex = 0;
    printf( "polyline #%d: %d vertices:\n", polyline, polylineVertices );

    for ( vertex = 0; vertex < polylineVertices; ++vertex, vertex2 += 2 ) {
      printf( "  (%g, %g)\n",
              outputVertices[ vertex2 ], outputVertices[ vertex2 + 1 ] );
    }
  }
}


static void test2( void ) {
  const int inputPolylineCount = 1;
  const int inputCounts[ 1 ] = { 22 };
  const int inputVertexCount = 22;
  const float inputVertices[] = {
    -122.413, 37.850,
    -122.411, 37.850,
    -122.408, 37.853,
    -122.407, 37.856,
    -122.409, 37.862,
    -122.411, 37.865,
    -122.414, 37.868,
    -122.417, 37.869,
    -122.418, 37.870,
    -122.422, 37.867,
    -122.424, 37.865,
    -122.429, 37.863,
    -122.431, 37.860,
    -122.433, 37.857,
    -122.432, 37.855,
    -122.431, 37.854,
    -122.429, 37.852,
    -122.426, 37.852,
    -122.424, 37.852,
    -122.421, 37.851,
    -122.416, 37.850,
    -122.414, 37.850
  };
  const double resolution = 0.0;
  const double bounds[ 2 ][ 2 ] =
    { { -122.416, -122.411 }, { 37.8479, 37.8526 } };
  int outputPolylineCount = 0;
  int outputVertexCount = 0;
  int outputCounts[ 100 ];
  float outputVertices[ 100 * 2 ];
  int polyline = 0;
  int vertex2 = 0;
  memset( &outputCounts, 0, sizeof outputCounts );
  memset( &outputVertices, 0, sizeof outputVertices );
  printf( "\ntest2:\n\n" );

  subsetMap( inputPolylineCount, inputVertexCount,
             inputCounts, inputVertices,
             resolution, bounds,
             &outputPolylineCount, &outputVertexCount,
             outputCounts, outputVertices );

  printf( "outputPolylineCount = %d, outputVertexCount = %d\n",
          outputPolylineCount, outputVertexCount );

  for ( polyline = 0; polyline < outputPolylineCount; ++polyline ) {
    const int polylineVertices = outputCounts[ polyline ];
    int vertex = 0;
    printf( "polyline #%d: %d vertices:\n", polyline, polylineVertices );

    for ( vertex = 0; vertex < polylineVertices; ++vertex, vertex2 += 2 ) {
      printf( "  (%g, %g)\n",
              outputVertices[ vertex2 ], outputVertices[ vertex2 + 1 ] );
    }
  }
}


int main( void ) {
  test1();
  test2();
  return 0;
}
