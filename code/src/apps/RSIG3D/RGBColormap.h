#ifndef RGBCOLORMAP_H
#define RGBCOLORMAP_H


/******************************************************************************
PURPOSE: RGBColormap.h - Declare RGBColormap type and routines.
HISTORY: 2018-12-24 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdlib.h> // For size_t.

#include <Utilities.h> // Color, DataColor, isNan().

#include "RGBColormap.h" // For public interface.

#include <QString> // for stuff that needs QString


namespace emvl {

//================================ CONSTANTS ==================================

enum { R, G, B };

// colormap lengths
// NOTES: - COLORMAPS LONGER THAN 32 MAY CAUSE WEIRD BEHAVIOR IN THE SHADER
//        - ONLY THE AQI COLORMAP CAN HAVE A LENGTH OF 6, AS THIS IS TESTED FOR
//          IN THE SHADER.  
enum {
  RGBCOLORMAP_LENGTH_DEFAULT = 20,
  RGBCOLORMAP_LENGTH_NASA = 32, 
  RGBCOLORMAP_LENGTH_NASA_DEPOLARIZATION = 12,
  RGBCOLORMAP_LENGTH_LOGARITHMIC = 20,
  RGBCOLORMAP_LENGTH_AQI = 6,
  RGBCOLORMAP_LENGTH_SOLID = 20,
  RGBCOLORMAP_LENGTH_DIFFERENCE = 20
};

//================================== TYPES ====================================

typedef const unsigned char RGBColormap[3][RGBCOLORMAP_LENGTH_NASA]; // set to longest colormap

//================================ FUNCTIONS ==================================

extern size_t getRGBColormapCount(); // Number of colormaps.

extern const char* getRGBColormapName( const size_t index ); // Name of colormap

extern const RGBColormap& getRGBColormap( const size_t index ); // Get colormap

extern DataColor getRGBColormapFunction( const size_t index ); // Get function

extern int getRGBColormapIndex( QString name ); // Get index of named colormap

};

#endif

