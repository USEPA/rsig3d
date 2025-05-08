/*
test_image.c - Test image routines.
gcc -Wall -g -o test_image test_image.c \
    -I../../../../include -L../../../../lib/Darwin -lUtilities.debug
valgrind --tool=memcheck --leak-check=yes test_image
*/

#include <stdio.h> /* For printf(). */
#include <stdlib.h> /* For free(). */

#include <ImageFile.h> /* For readImageFile(). */

int main( void ) {
  const char* const inputFileName = "input.bin";
  const Bounds clip = { { -90.0, 29.0 }, { -89.0, 30.0 } };
  Bounds domain     = { { 0.0, 0.0 }, { 0.0, 0.0 } };
  Bounds corners    = { { 0.0, 0.0 }, { 0.0, 0.0 } };
  int width = 0;
  int height = 0;
  unsigned char* rgb =
    readImageFile( inputFileName, clip, &width, &height, domain, corners );
  const int ok = rgb != 0;

  if ( rgb ) {
    enum { R, G, B };
    const int count = width * height * 3;
    printf( "domain = [ %lf %lf ][ %lf %lf ]\n",
            domain[ LONGITUDE ][ MINIMUM ],
            domain[ LONGITUDE ][ MAXIMUM ],
            domain[ LATITUDE  ][ MINIMUM ],
            domain[ LATITUDE  ][ MAXIMUM ] );
    printf( "corners = [ %lf %lf ][ %lf %lf ]\n",
            corners[ LONGITUDE ][ MINIMUM ],
            corners[ LONGITUDE ][ MAXIMUM ],
            corners[ LATITUDE  ][ MINIMUM ],
            corners[ LATITUDE  ][ MAXIMUM ] );
    printf( "rgb = [ %d %d %d ... %d %d %d ]\n",
            rgb[ R ], rgb[ G ], rgb[ B ],
            rgb[ count - 3 + R ],
            rgb[ count - 3 + G ],
            rgb[ count - 3 + B ] );
    free( rgb ), rgb = 0;
  }

  return ! ok;
}

