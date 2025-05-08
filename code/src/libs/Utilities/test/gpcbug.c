/*
gpcbug.c - gpc_polygon_to_tristrip() crashes.
gcc -Wall -g -o gpcbug gpcbug.c \
    -I../../../include -L../../../lib/$platform -lUtilities.debug -lShapefile.debug -lGPC -lm
gpcbug /Users/plessel/projects/estuary/data/tmp/hspf_blackstone10_huc12_polygon_hydrology
*/

#include <Shapefile.h>

int main( int argc, char* argv[] ) {

  if ( argc == 2 ) {
    const char* const inputFile = argv[ 1 ];
    int count = 0;
    PolygonShape* polygonShape = readAndTriangulateShapes( inputFile, &count );
    printf( "polygonShape = %p, count = %d\n", polygonShape, count );
  
    if ( polygonShape ) {
      deallocatePolygons( count, polygonShape );
      polygonShape = 0;
    }
  }

  return 0;
}

