
#ifndef LOADPNG_H
#define LOADPNG_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
PURPOSE: loadPNG.h - Declare routine for reading a PNG file into a GL texture.

NOTES:   Uses libraries for OpenGL, PNG and Z.

         gcc -Wall -g -o example example.c loadPNG.c \
             -framework GLUT -framework OpenGL
 
HISTORY: 2014-07-02 plessel.todd@epa.gov

STATUS:  unreviewed, untested.
******************************************************************************/

/*================================ INCLUDES =================================*/

#ifdef __APPLE__
#include <OpenGL/gl.h> /* For GLuint. */
#else
#include <GL/gl.h> /* For GLuint. */
#endif

/*================================ FUNCTIONS ================================*/

extern GLuint loadPNG( const char* fileName, int* width, int* height );

#ifdef __cplusplus
}
#endif

#endif /* LOADPNG_H */


