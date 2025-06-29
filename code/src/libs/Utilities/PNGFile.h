#ifndef PNGFILE_H
#define PNGFILE_H

#ifdef __cplusplus
extern "C" {
#endif
  
/******************************************************************************
 PURPOSE: MyPNG.h - Simple PNG image file read/write routines.
 
 NOTES:   Uses PNG Library which uses z library.
 
 HISTORY: 2011/03/09, plessel.todd@epa.gov, Created.
 STATUS:  unreviewed, untested.
 ******************************************************************************/

/*================================= INCLUDES ================================*/

#include <stdio.h> /* For FILE. */

/*================================== TYPES ==================================*/

enum { BOTTOM_UP_ROW_ORDER, TOP_DOWN_ROW_ORDER };

/*================================ FUNCTIONS ================================*/


extern int readPNGFile( FILE* input, int width, int height, int rowOrder,
                        unsigned char rgb[] );

extern int writePNGFile( FILE* output, int width, int height,
                         unsigned char rgba[] );


#ifdef __cplusplus
}
#endif

#endif /* PNGFILE_H */

