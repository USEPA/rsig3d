
/******************************************************************************
PURPOSE: RGBColormap.cpp - Implement RGBColormap routines.
HISTORY: 2018-12-24 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <assert.h> // For assert().
#include <stdlib.h> // For size_t.
#include <math.h>   // For log10().

#include <Utilities.h> // Color, DataColor, isNan().
#include <DataUtilities.h> // emvl::MISSING.

#include "RGBColormap.h" // For public interface.

#include <QString>
#include <QDebug>

namespace emvl {

//============================ PRIVATE VARIABLES ==============================

/*
 * Define colormaps.
 * NOTE: There should be no entries that are black (0,0,0) since this is
 * reserved for 'missing data' which is not rendered.
 */

static const RGBColormap stdGamma = {
  { 0,   0,   0,   0,  28,  81,  84, 149, 222, 255, 255, 255, 255, 163, 229, 255, 255, 255, 255, 255 }, // red
  { 0,   0,   0,   0,   0,   0,   0,   0,   0,  37, 109, 163, 163, 163, 230, 255, 255, 255, 255, 255 }, // green
  { 10,  67, 135, 208, 234, 158,  87,  17,   0,   0,  28,  71,   0,   0,  35,  80, 122, 167, 209, 255 }  // blue
};

static const RGBColormap purpleRed = {
  { 139, 118, 100,  86,  77,  72,  73,  79,  90, 105, 123, 145, 169, 195, 220, 245, 255, 255, 255, 255}, //red
  {  60,  82, 107, 134, 164, 192, 217, 237, 251, 255, 251, 237, 217, 192, 164, 134, 107,  82,  60,  43}, //green
  { 145, 188, 226, 249, 254, 240, 208, 167, 124,  85,  53,  31,  17,   8,   4,   2,   1,   0,   0,   0} //blue
};

static const RGBColormap smokePlume = {
  { 191, 191, 191, 191, 191, 191, 191, 128, 128, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 64 }, // red
  { 191, 191, 191, 191, 191, 191, 191, 128, 128, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 64 }, // green
  { 191, 191, 191, 191, 191, 191, 191, 128, 128, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 64 }, // blue
};

static const RGBColormap grayscale = {
  { 10, 13, 26, 40, 53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }, // red
  { 10, 13, 26, 40, 53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }, // green
  { 10, 13, 26, 40, 53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }  // blue
};

static const RGBColormap allWhite = {
  { 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253 }, // red
  { 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253 }, // green
  { 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253 }  // blue
};

static const RGBColormap allBlack = {
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }, // red
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }, // green
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }  // blue
};

static const RGBColormap blueRamp = {
  {   0,   13,   26,   40,  53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }, // red
  {   0,   13,   26,   40,  53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }, // green
  { 128,  139,  151,  162, 174, 186, 197,209, 221, 232, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255 }  // blue
};

static const RGBColormap redRamp = {
  { 128,  139,  151,  162, 174, 186, 197,209, 221, 232, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255 }, // red
  {   0,   13,   26,   40,  53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }, // green
  {   0,   13,   26,   40,  53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }  // blue
};

static const RGBColormap greenRamp = {
  {   0,   13,   26,   40,  53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }, // red
  { 128,  139,  151,  162, 174, 186, 197,209, 221, 232, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255 }, // green
  {   0,   13,   26,   40,  53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }  // blue
};

static const RGBColormap yellowRamp = {
  { 128,  139,  151,  162, 174, 186, 197,209, 221, 232, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255 }, // red
  { 128,  139,  151,  162, 174, 186, 197,209, 221, 232, 244, 255, 255, 255, 255, 255, 255, 255, 255, 255 }, // green
  {   0,   13,   26,   40,  53,  67,  80, 94, 107, 120, 134, 147, 161, 174, 187, 201, 214, 228, 241, 255 }  // blue
};

static const RGBColormap blueRampInverted = {
  { 255, 241, 228, 214, 201, 187, 174, 161, 147, 134, 120, 107,  94,  80,  67,  53,  40,  26,  13,   0  }, // red
  { 255, 241, 228, 214, 201, 187, 174, 161, 147, 134, 120, 107,  94,  80,  67,  53,  40,  26,  13,   0  }, // green
  { 255, 255, 255, 255, 255, 255, 255, 255, 255, 244, 232, 221, 209, 197, 186, 174, 162, 151, 139, 128  }  // blue
};

static const RGBColormap redRampInverted = {
  { 255, 255, 255, 255, 255, 255, 255, 255, 255, 244, 232, 221, 209, 197, 186, 174, 162, 151, 139, 128  }, // red
  { 255, 241, 228, 214, 201, 187, 174, 161, 147, 134, 120, 107,  94,  80,  67,  53,  40,  26,  13,   0  }, // green
  { 255, 241, 228, 214, 201, 187, 174, 161, 147, 134, 120, 107,  94,  80,  67,  53,  40,  26,  13,   0  }  // blue
};

static const RGBColormap greenRampInverted = {
  { 255, 241, 228, 214, 201, 187, 174, 161, 147, 134, 120, 107,  94,  80,  67,  53,  40,  26,  13,   0  }, // red
  { 255, 255, 255, 255, 255, 255, 255, 255, 255, 244, 232, 221, 209, 197, 186, 174, 162, 151, 139, 128  }, // green
  { 255, 241, 228, 214, 201, 187, 174, 161, 147, 134, 120, 107,  94,  80,  67,  53,  40,  26,  13,   0  }  // blue
};

static const RGBColormap yellowRampInverted = {
  { 255, 255, 255, 255, 255, 255, 255, 255, 255, 244, 232, 221, 209, 197, 186, 174, 162, 151, 139, 128  }, // red
  { 255, 255, 255, 255, 255, 255, 255, 255, 255, 244, 232, 221, 209, 197, 186, 174, 162, 151, 139, 128  }, // green
  { 255, 241, 228, 214, 201, 187, 174, 161, 147, 134, 120, 107,  94,  80,  67,  53,  40,  26,  13,   0  }  // blue
};
  
static const RGBColormap CALIPSOBackscatter = {
  {  10,  0,   0,   0,   0,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 70, 100, 130, 155, 180, 200, 225, 235, 240, 242, 245, 255}, // red
  {  10, 42, 127, 127, 127, 127, 127, 255, 127, 170, 255, 255, 212, 170, 127,  85,   0,  42,  85, 127, 70, 100, 130, 155, 180, 200, 225, 235, 240, 242, 245, 255}, // green
  {  10,170, 255, 255, 255, 255, 255, 170, 127,  85,   0,   0,   0,   0,   0,   0,   0,  85, 127, 170, 70, 100, 130, 155, 180, 200, 225, 235, 240, 242, 245, 255}  // blue
};

static const RGBColormap CALIPSODepolarization = {
  {10,   0,   0, 255, 255, 255, 255, 255, 169, 255, 255, 255}, //red
  {10, 173, 207, 251, 169,  35,  66, 215, 101, 255, 255, 255}, //green
  {10, 255,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255}  //blue
};

static const RGBColormap blueRed = { // formerly AOD, now called blueRed in the menu
  {    0,   0,   0,   0,   0,   0,   0,   0,  35,  87, 143, 195, 251, 255, 255, 255, 255, 246, 189, 131 }, // red
  {    0,   0,   0,  27,  79, 135, 187, 239, 255, 255, 255, 255, 255, 207, 155,  99,  47,   0,   0,   0 }, // green
  {  131, 179, 231, 255, 255, 255, 255, 255, 219, 167, 111,  59,   3,   0,   0,   0,   0,   0,   0,   0 },  // blue
};

static const RGBColormap viridis = {
  {   68,  72,  72,  69,  63,  57,  50,  45,  39,  35,  31,  32,  41,  60,  86, 116, 148, 184, 220, 253 }, // red
  {   13,  21,  38,  55,  71,  85, 100, 112, 125, 138, 150, 163, 175, 188, 198, 208, 216, 222, 227, 231 }, // green
  {   84, 104, 119, 129, 136, 140, 142, 142, 142, 141, 139, 134, 127, 117, 103,  85,  64,  41,  23,  37 },  // blue
};

// The AQI colors below should be replicated in the fragment shader (RenderingContextRSIG.cpp frag_prog)
// Those who do not like it can jump through the hoops of passing them in!  
static const RGBColormap AQI = {
  {    0, 255, 255, 255, 143, 126 },  // red
  {  228, 255, 126,   0,  63,   0 },  // green
  {    0,   0,   0,   0, 151,  35 },  // blue
};

static const RGBColormap Diff = {
  {    0,   0,   0,   0,   0,  13,  67, 121, 175, 229, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, // red
  {    0,  35,  71, 106, 141, 172, 190, 208, 226, 244, 244, 228, 211, 195, 178, 146, 110,  73,  37,   0}, // green
  {  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 229, 176, 123,  70,  17,   3,   2,   2,   1,   0},  // blue
};


  
//============================ PRIVATE FUNCTIONS ==============================



/******************************************************************************
PURPOSE: Map data value in range [minimum, maximum] -> RGBColor -> Color.
INPUTS:  const double value           Data value to map.
         const double minimum         Lower range of values to map to.
         const double maximum         Upper range of values to map to.
         const bool isLog             Take the log base 10 of value to map?
         const RGBColormp& colormap   The colormap to use.
         const size_t length          The number of colormap entries to use.
RETURNS: Color the color of the value.
******************************************************************************/

static Color mapToColor( const double value,
                         const double minimum, const double maximum,
                         const bool isLog, const RGBColormap& colormap,
                         const size_t length ) {

  assert( ! isNan( value ) ); assert( minimum <= maximum );
  assert( length * 3 <= sizeof colormap );
  Color result = { 0.0, 0.0, 0.0 };

  if ( value != emvl::MISSING ) {
    const double value0 = isLog ? log10( value ) : value;
    const double range = maximum - minimum;
    double t = range <= 0.0 ? 0.0 : ( value0 - minimum ) / range;

    if ( t < 0.0 || isNan( t ) ) {
      t = 0.0;
    } else if ( t > 1.0 ) {
      t = 1.0;
    }

    const double findex = t * ( length - 1 );
    const int index = (int) findex;
    assert( index >= 0 ); assert( (size_t) index < length );
    const unsigned char ucr = colormap[ R ][ index ];
    const unsigned char ucg = colormap[ G ][ index ];
    const unsigned char ucb = colormap[ B ][ index ];
    const double factor = 1.0 / 255.0;
    const float r = ucr * factor;
    const float g = ucg * factor;
    const float b = ucb * factor;
    result.r = r;
    result.g = g;
    result.b = b;
  }

  assert( isValidColor( result ) );
  return result;
}


// DataColor functions for each colormap:

static
Color stdGammaFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, stdGamma, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color purpleRedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, purpleRed, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color smokePlumeFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, smokePlume, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color grayscaleFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, grayscale, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color allWhiteFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, allWhite, emvl::RGBCOLORMAP_LENGTH_SOLID );
}

static
Color allBlackFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, allBlack, emvl::RGBCOLORMAP_LENGTH_SOLID );
}

static
Color blueRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, blueRamp, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color redRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, redRamp, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color greenRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, greenRamp, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color yellowRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, yellowRamp, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}
  
static
Color blueRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, blueRampInverted, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}
  
static
Color redRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, redRampInverted, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color greenRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, greenRampInverted, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color yellowRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, yellowRampInverted, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}
  
static
Color calipsoBackscatterFunction(double value, double minimum, double maximum) {
  return mapToColor( value, minimum, maximum, false, CALIPSOBackscatter, emvl::RGBCOLORMAP_LENGTH_NASA );
}

static
Color calipsoDepolarizationFunction( double value,
                                     double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, CALIPSODepolarization, emvl::RGBCOLORMAP_LENGTH_NASA_DEPOLARIZATION );
}

static
Color blueRedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, blueRed, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

static
Color viridisFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, viridis, emvl::RGBCOLORMAP_LENGTH_DEFAULT );
}

  static
Color diffFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, Diff, emvl::RGBCOLORMAP_LENGTH_DIFFERENCE );
}
  
static
Color aqiFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, false, AQI, emvl::RGBCOLORMAP_LENGTH_AQI );
}

static
Color logStdGammaFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, stdGamma, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logPurpleRedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, purpleRed, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logGrayscaleFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, grayscale, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logBlueRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, blueRamp, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logRedRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, redRamp, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logGreenRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, greenRamp, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logYellowRampFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, yellowRamp, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}
  
static
Color logBlueRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, blueRampInverted, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logRedRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, redRampInverted, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logGreenRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, greenRampInverted, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}
  
static
Color logBlueRedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, blueRed, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}

static
Color logYellowRampInvertedFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, yellowRampInverted, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}
  
static
Color logViridisFunction( double value, double minimum, double maximum ) {
  return mapToColor( value, minimum, maximum, true, viridis, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC );
}


// Info about RGBColormaps and table of instances:

typedef struct {
  const char* const name;
  const RGBColormap& colormap;
  DataColor function;
} Entry;

static const Entry table[] = {
  { "Blue-red", blueRed, blueRedFunction },
  { "Purple-red", purpleRed, purpleRedFunction },
  { "StdGamma", stdGamma, stdGammaFunction },
  { "Blue-ramp", blueRamp, blueRampFunction },
  { "Red-ramp", redRamp, redRampFunction },
  { "Green-ramp", greenRamp, greenRampFunction },
  { "Yellow-ramp", yellowRamp, yellowRampFunction },
  { "Blue-ramp-inverted", blueRampInverted, blueRampInvertedFunction },
  { "Red-ramp-inverted", redRampInverted, redRampInvertedFunction },
  { "Green-ramp-inverted", greenRampInverted, greenRampInvertedFunction },
  { "Yellow-ramp-inverted", yellowRampInverted, yellowRampInvertedFunction },
  { "Viridis", viridis, viridisFunction },
  { "Grayscale", grayscale, grayscaleFunction },
  { "Smoke Plume", smokePlume, smokePlumeFunction },
  { "All-White", allWhite, allWhiteFunction },
  { "All-Black", allBlack, allBlackFunction },
  { "Difference", Diff, diffFunction },
  { "CALIPSO-Backscatter", CALIPSOBackscatter, calipsoBackscatterFunction },
  { "CALIPSO-Depolarization", CALIPSODepolarization, calipsoDepolarizationFunction },
  { "AQI-PM25-24hr", AQI, aqiFunction },
  { "AQI-Ozone-8hr", AQI, aqiFunction },
  { "AQI-CO-8hr", AQI, aqiFunction },
  { "AQI-Ozone-1hr", AQI, aqiFunction },
  { "AQI-NO2-1hr", AQI, aqiFunction },
  { "AQI-SO2-1hr", AQI, aqiFunction },
  { "Blue-red (log)", blueRed, logBlueRedFunction },
  { "Purple-red (log)", purpleRed, logPurpleRedFunction },
  { "StdGamma (log)", stdGamma, logStdGammaFunction },
  { "Blue-ramp (log)", blueRamp, logBlueRampFunction },
  { "Red-ramp (log)", redRamp, logRedRampFunction },
  { "Green-ramp (log)", greenRamp, logGreenRampFunction },
  { "Yellow-ramp (log)", yellowRamp, logYellowRampFunction },
  { "Blue-ramp-inverted (log)", blueRampInverted, logBlueRampInvertedFunction },
  { "Red-ramp-inverted (log)", redRampInverted, logRedRampInvertedFunction },
  { "Green-ramp-inverted (log)", greenRampInverted, logGreenRampInvertedFunction },
  { "Yellow-ramp-inverted (log)", yellowRampInverted, logYellowRampInvertedFunction },
  { "Viridis (log)",viridis, logViridisFunction },
  { "Grayscale (log)", grayscale, logGrayscaleFunction }
};

// Get indexed entry:

static const Entry* getEntry( const size_t index ) {
  assert( index < sizeof table / sizeof *table );
  return table + index;
}



//============================= PUBLIC FUNCTIONS ==============================



size_t getRGBColormapCount() {
  return sizeof table / sizeof *table;
}

const char* getRGBColormapName( const size_t index ) {
  return getEntry( index )->name;
}

const RGBColormap& getRGBColormap( const size_t index ) {
  return getEntry( index )->colormap;
}

DataColor getRGBColormapFunction( const size_t index ) {
  return getEntry( index )->function;
}

int getRGBColormapIndex( QString name ) {
  const size_t tableSize = getRGBColormapCount();
  QString thisName;

  for (size_t thisInd=0; thisInd<tableSize; thisInd++) {
    thisName = QString(getEntry( thisInd )->name);

    if (thisName.compare(name) == 0) {
      return thisInd;
    }
  }
    
  // if we made it this far, something is wrong, but
  // return a valid index to keep downstream stuff from breaking
  qDebug() << "Colormap not found: " << thisName;
  return 0;
}

  
}; // namespace 
