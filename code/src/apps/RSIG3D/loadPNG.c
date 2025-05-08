/******************************************************************************
PURPOSE: loadpng.c - Load a global [-180, 180] [-90, 90] PNG image as a OpenGL
         texture.
HISTORY: 2014-07-03 plessel.todd@epa.gov
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h> /* For FILE, stderr, fprintf(). */
#include <stdlib.h> /* For malloc(), free(). */
#include <string.h> /* For memset(). */
#include <assert.h> /* For assert(). */

#include <png.h> /* For png_*. */
/* HACK: pretend we have a compatible version so it just works! */
#undef  PNG_LIBPNG_VER_STRING
#define PNG_LIBPNG_VER_STRING "1.5.10"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h> /* For gl*. */
#include <GLUT/glut.h> /* For glut*. */
#else
#include <GL/glut.h> /* For gl*. */
#endif

#include "loadPNG.h" // For public interface.

/*================================= MACROS ==================================*/

#define FREE( p ) ( ( (p) ? free(p) : (void) 0 ), (p) = 0 )
#define NEW( type, count ) (type*) allocate( sizeof (type) * (count) )

/*=========================== FORWARD DECLARATIONS ==========================*/

static void* allocate( const size_t bytes );

static unsigned char* readPNGFile( FILE* file, int* width, int* height );

static GLuint makeTexture( const int width, const int height,
                           const unsigned char image[] );

/*============================ PUBLIC FUNCTIONS =============================*/



/******************************************************************************
PURPOSE: loadPNG - Read a PNG file and create an OpenGL texture of it.
INPUTS:  const char* fileName  Name of PNG file to read.
OUTPUTS: int* width            Width  of image in pixels.
         int* height           Height of image in pixels.
RETURNS: GLuint  Id of texture created or 0 if unsuccessful.
******************************************************************************/

GLuint loadPNG( const char* fileName, int* width, int* height ) {
  GLuint result = 0;
  FILE* file = 0;
  assert( fileName ); assert( *fileName ); assert( width ); assert( height );
  file = fopen( fileName, "rb" );
  *width = *height = 0;

  if ( ! file ) {
    fprintf( stderr, "\nFailed to open PNG file '%s' for reading.\n",
             fileName );
  } else {
    unsigned char* rgb = readPNGFile( file, width, height );

    if ( rgb ) {
      result = makeTexture( *width, *height, rgb );
      FREE( rgb );
    }

    fclose( file ), file = 0;
  }

  assert( ( result == 0 && *width == 0 && *height == 0 ) ||
          ( result != 0 && *width >  0 && *height >  0 ) );
  return result;
}



/*============================ PRIVATE FUNCTIONS ============================*/



/******************************************************************************
PURPOSE: allocate - Allocate memory on the heap.
INPUTS:  const size_t bytes  Number of bytes to allocate.
RETURNS: void*  Non-0 if successful, else 0 and a message is printed to stderr.
******************************************************************************/

static void* allocate( const size_t bytes ) {
  void* result = 0;
  assert( bytes > 0 );
  result = malloc( bytes );

  if ( result ) {
    memset( result, 0, bytes );
  } else {
    fprintf( stderr, "\nFailed to allocate %lu bytes "
             "to complete the requested action.\n", (unsigned long) bytes );
  }

  return result;
}



/******************************************************************************
PURPOSE: readPNGFile - Read a PNG image file.
INPUTS:  FILE* input    File to read from.
OUTPUTS: int* width     Image width in pixels.
         int* height    Image height in pixels.
RETURNS: unsigned char* rgb[ height ][ width ][ 3 ]  RGB pixel values.
******************************************************************************/

static unsigned char* readPNGFile( FILE* file, int* width, int* height ) {
  unsigned char* result = 0;
  int ok = 0;
  png_byte header[ 8 ];
  assert( file ); assert( width ); assert( height );

  if ( fread( header, 8, 1, file ) != 1 || png_sig_cmp( header, 0, 8 ) != 0 ) {
    fprintf( stderr, "\nInvalid PNG header.\n" );
  } else {
    png_structp png = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0);

    if ( ! png ) {
      fprintf( stderr, "\nFailed to create PNG read structure.\n" );
    } else {
      png_infop info = png_create_info_struct( png );

      if ( ! info ) {
        fprintf( stderr, "\nFailed to create PNG info structure.\n" );
        png_destroy_read_struct( &png, 0, 0 );
        png = 0;
      } else {
        png_uint_32 imageWidth = 0;
        png_uint_32 imageHeight = 0;
        int bitDepth = 0;
        int colorType = 0;
        int interlacing = 0;
        int unused_ = 0;

        png_init_io( png, file );
        png_set_sig_bytes( png, 8 );
        png_read_info( png, info );

        png_get_IHDR( png, info, &imageWidth, &imageHeight,
                      &bitDepth, &colorType, &interlacing,
                      &unused_, &unused_ );

        if ( ! ( imageWidth > 0 &&
                 imageHeight > 0 &&
                 bitDepth == 8 &&
                 ( colorType == PNG_COLOR_TYPE_PALETTE ||
                   colorType == PNG_COLOR_TYPE_RGB  ||
                   colorType == PNG_COLOR_TYPE_RGBA ) &&
                 interlacing == PNG_INTERLACE_NONE ) ) {
          fprintf( stderr, "\nUnsupported PNG file type.\n" );
        } else {
          const int components = colorType == PNG_COLOR_TYPE_RGBA ? 4 : 3;
          *width = (int) imageWidth;
          *height = (int) imageHeight;
          result = NEW( unsigned char, imageHeight * imageWidth * 3 );

          if ( result ) {
            png_bytep* rowPointers = NEW( png_bytep, *height );

            if ( rowPointers ) {
              unsigned char* output = result;
              png_colorp palette = 0;
              int row = 0;
              ok = 1;

              for ( row = 0; row < *height && ok; ++row ) {
                rowPointers[ row ] = NEW( png_byte, *width * components );
                ok = rowPointers[ row ] != 0;
              }

              if ( ok ) {
                png_read_image( png, rowPointers ); /* Read image. */

                if ( colorType == PNG_COLOR_TYPE_PALETTE ) {
                  png_get_PLTE( png, info, &palette, &unused_ );
                }

                /* Decode PNG: */

                for ( row = 0; row < *height; ++row ) {
                  const unsigned char* const rowPixels = rowPointers[ row ];
                  int column = 0;

                  for ( column = 0; column < *width; ++column, output += 3 ) {

                    if ( colorType == PNG_COLOR_TYPE_PALETTE ) {
                       const int index = rowPixels[ column ];
                       output[ 0 ] = palette[ index ].red;
                       output[ 1 ] = palette[ index ].green;
                       output[ 2 ] = palette[ index ].blue;
                    } else {
                      const png_byte* const pixel =
                        rowPixels + column * components;
                      output[ 0 ] = pixel[ 0 ];
                      output[ 1 ] = pixel[ 1 ];
                      output[ 2 ] = pixel[ 2 ];
                    }
                  }
                }
              }

              /* Deallocate row data: */

              for ( row = 0; row < *height; ++row ) {
                FREE( rowPointers[ row ] );
              }

              FREE( rowPointers );
            }
          }
        }
      }

      png_destroy_info_struct( png, &info );
    }

    png_destroy_read_struct( &png, 0, 0 );
  }

  if ( ! ok ) {
    FREE( result );
    *width = *height = 0;
  }

  assert( ( result == 0 && *width == 0 && *height == 0 ) ||
          ( result != 0 && *width > 0 && *height > 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: makeTexture - Create an OpenGL texture from image RGB array.
INPUTS:  const int width    Image width in pixels.
         const int height   Image height in pixels.
         const unsigned char* image[ height ][ width ][ 3 ]  RGB pixel values.
RETURNS: GLuint  Id of texture created or 0 if unsuccessful.
******************************************************************************/

static GLuint makeTexture( const int width, const int height,
                           const unsigned char image[] ) {
  
  GLuint result = 0;
  assert( width > 0 ); assert( height > 0 ); assert( image );

  glGenTextures( 1, &result );
  glBindTexture( GL_TEXTURE_2D, result );
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ); /*Allow image w/ odd bytes per row*/
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
#if 0
  /* Why doesn't this work? Because the image dimensions must be power-of-2. */
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                GL_UNSIGNED_BYTE, image );
#else
  /* SLOW: */
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE,
                     image );
#endif

  /* Restore default GL state: */

  glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
  /* glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); */
  /* glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ); */
  /* glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); */
  /* glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ); */
  /* glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ); */

  return result;
}


