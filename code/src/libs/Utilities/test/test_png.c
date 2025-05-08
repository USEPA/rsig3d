/*
test_png.c - Test PNG routines.
gcc -Wall -g -o test_png test_png.c \
    -I../../../../include -L../../../../lib/Darwin \
    -lUtilities.debug -lPNG -lZ
valgrind --tool=memcheck --leak-check=yes test_png
open input.png output.png
*/

#include <stdio.h> /* For FILE, fopen(). */
#include <string.h> /* For memset(). */

#include <PNGFile.h> /* For readPNGFile(), writePNGFile(). */

int main( void ) {
  enum { R, G, B, A, WIDTH = 128, HEIGHT = 128 };
  const char* const inputFileName = "input.png";
  const char* const outputFileName = "output.png";
  int ok = 0;
  FILE* file = fopen( inputFileName, "rb" );
  unsigned char rgb[ HEIGHT * WIDTH * 3 ] = "";
  unsigned char rgba[ HEIGHT * WIDTH * 4 ] = "";
  memset( rgb, 0, sizeof rgb );
  memset( rgba, 0, sizeof rgba );

  if ( file ) {
    ok = readPNGFile( file, WIDTH, HEIGHT, TOP_DOWN_ROW_ORDER, rgb );

    if ( ok ) {
      const int size = sizeof rgb / sizeof *rgb;
      int in = 0;
      int out = 0;

      for ( in = 0; in < size; in += 3, out += 4 ) {
        rgba[ out     ] = rgb[ in     ];
        rgba[ out + 1 ] = rgb[ in + 1 ];
        rgba[ out + 2 ] = rgb[ in + 2 ];
        rgba[ out + 3 ] = 0xff;
      }
    }

    fclose( file ), file = 0;
  }

  if ( ok ) {
    ok = 0;
    file = fopen( outputFileName, "wb" );

    if ( file ) {
      ok = writePNGFile( file, WIDTH, HEIGHT, rgba );
      fclose( file ), file = 0;
    }
  }

  return ! ok;
}

