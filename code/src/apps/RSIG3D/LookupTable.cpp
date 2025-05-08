/******************************************************************************
PURPOSE: LookupTable.cpp -

NOTES:   LookupTable class

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#if _MSC_VER
#include <windows.h>
#endif

#include <assert.h>
#include <math.h>
#include <float.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include <QDebug>
#include <QStringList>

#include "RGBColormap.h" // For RGBColormap type and related functions.
#include "LookupTableWidget.h"
#include "VisDataList.h"
#include "LookupTable.h"

using namespace std;

#define Vec4 Vector4f

// build default colormap
static void  buildDefaultColorTable( float minScalar, float maxScalar,
                                     std::vector<Vector4f>& colors,
                                     std::vector<float>& colorValues ) {

  colors.clear();
  colorValues.clear();
  const float range = maxScalar - minScalar;
  Vec4 red   (1.0f, 0.0f, 0.0f, 1.0f);
  Vec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
  Vec4 green (0.0f, 1.0f, 0.0f, 1.0f);
  Vec4 cyan  (0.0f, 1.0f, 1.0f, 1.0f);
  Vec4 blue  (0.0f, 0.0f, 1.0f, 1.0f);
  colors.push_back(blue);
  colors.push_back(cyan);
  colors.push_back(green);
  colors.push_back(yellow);
  colors.push_back(red);

  const int n = colors.size();
  const float K = 1.0f/float(n);
  colorValues.push_back(minScalar);

  for (int i=1; i<n; i++) {
    colorValues.push_back(i*K*range + minScalar);
  }

  colorValues.push_back(maxScalar);
}



Vector4f interpolate_list(float myValue,
                          const std::vector<Vector4f>& color,
                          const std::vector<float>& values,
                          bool interpolationFlag) {

  // Value array is 1 longer than color array. Example for 5 element value array:
  // v0  v1  v2  v3  v4  v5
  //   c0  c1  c2  c3  c4
  //
  // e.g, c0 is the color assigned to values between v0 and v1
  
  const int vSize = values.size();
  const int cSize = color.size();

  // if data value is outside of the value array, return the appropriate color 

  if (myValue <= values[0]) {
    return color[0];
  } else if (myValue >= values[vSize-1]) {
    return color[cSize-1];
  }

  // loop through value array. ignoring both endpoints because they are covered above
  for (int i=1; i<vSize-1; i++) {
    // if myValue is less than the array value, return a color. Otherwise keep looping.

    if (myValue < values[i]) {

      if (interpolationFlag) {
        const float k = (myValue-values[i-1])/(values[i]-values[i-1]);
        return color[i-1]*(1.0f-k) + color[i]*k;
      } else {
        return color[i-1]; 
      }
    }
  }

  // we should never get here, but just in case, return the max color
  return color[cSize-1];
}



static void buildPalette( const std::vector<Vec4>& colors,
                          const std::vector<float>& values,
                          int paletteSize,
                          std::vector<Vector4f>& palette,
                          bool interpolation ) {

  if (paletteSize<2) {
    paletteSize=2;
  }

  palette.resize(paletteSize);
  palette[0] = colors[0];
  palette[paletteSize-1] = colors[colors.size()-1];

  const float range = values[values.size()-1] - values[0];

  for (int i=1; i<paletteSize-1; i++) {
    const float x = float(i) / paletteSize * range + values[0];
    palette[i] = interpolate_list(x, colors, values, interpolation);
  }
}



static void buildNonlinearPalette( const std::vector<Vec4>& colors,
                                   const std::vector<float>& values,
                                   int paletteSize,
                                   std::vector<Vector4f>& palette,
                                   bool interpolation ) {

  if (paletteSize<2) {
    paletteSize=2;
  }

  palette.resize(paletteSize);
  palette[0] = colors[0];
  palette[paletteSize-1] = colors[colors.size()-1];

  for (int i=1; i<paletteSize; i++) {
    const float x = values[i];
    palette[i] = interpolate_list(x, colors, values, interpolation);
  }
}



//=====================================================================

LookupTable::LookupTable() : mColors(), mValues(), mPalette(), mTexID(0) {
  colormapIndex = 0;
  nonlinearFlag = false;
}



LookupTable::~LookupTable() {

  if (mTexID != 0) {
    glDeleteTextures(1, &mTexID);
  }
}




bool LookupTable::getNonlinearFlag() const {
  return nonlinearFlag;
}



bool LookupTable::getStrictFlag() const {
  return strictFlag;
}


const std::vector<float>& LookupTable::getValues() const {
  return mValues;
}



void LookupTable::setNonlinearFlag(bool x) {
  nonlinearFlag = x;
}



void LookupTable::setStrictFlag(bool x) {
  strictFlag = x;
}



void LookupTable::rebuildPalette(bool interpolation) {

  int n = mPalette.size();

  if (n<=0) {
    std::cerr<<"Error in calling function LookupTable::rebuildPalette()\n";
    return;
  }

  buildPalette( mColors, mValues, n, mPalette, interpolation );
}



void LookupTable::setDefaultLookupTable() {
  const float minScalar=0;
  const float maxScalar=1;
  buildDefaultColorTable(minScalar, maxScalar, mColors, mValues);
  buildPalette(mColors, mValues, 1024, mPalette, false);
}



std::vector<Vector4f>& LookupTable::colorPalette()  {
  return mPalette;
}



Vector4f LookupTable::colorByValue(float x) const {
  const int vSize = mValues.size();
  const int mSize = mPalette.size();

  if (vSize<2) {
    return Vector4f(0);
  }
        
  // do not assume linearity... instead find nearest value

  if (x <= mValues[0]) {
    return mPalette[0];
  } else if (x >= mValues[vSize-1]) {
    return mPalette[mSize-1];
  } else {

    for (int ind=1; ind<vSize; ind++) {

      if (x > mValues[ind-1] && x <= mValues[ind]) {
        return mPalette[ind-1];
      }
    }

    // should never get here
    qDebug() << "problem in colorByValue()";
    return mPalette[mSize-1];
  }
}



Vector4f LookupTable::colorByParameter(float t) const {
  Vector4f r(0);
  const int n = mValues.size();
  const int m = mPalette.size();

  if (m==0 || n<2) {
    return r;
  }

  const int m1 = m - 1;

  if (t<=0) {
    r = mPalette[0];
  } else if (t>=1) {
    r = mPalette[m1];
  } else {
    //t *= m;
    int i = int(t*m);

    if (i > m1) {
      i=m1;
    }

    r = mColors[i];
  }
  return r;
}



GLuint LookupTable::genTexture() {

  if (mPalette.size()==0) {
    return 0;
  }

  if (glIsBuffer(mTexID)) {
    glDeleteBuffers(1, &mTexID);
    mTexID = 0;
  }

  const unsigned int tableSize = mPalette.size();
  const GLuint target = GL_TEXTURE_1D;
  glGenTextures(1, &mTexID);
  glBindTexture(target, mTexID);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage1D(target, 0, GL_RGBA8, tableSize, 0, GL_RGBA, GL_FLOAT, &mPalette[0]);
  glBindTexture(target, 0);
   
  return mTexID;
}



void LookupTable::updateTexture() {

  if (mTexID==0) {
    genTexture();
    return;
  }

  const unsigned int tableSize = mPalette.size();
  const GLuint target = GL_TEXTURE_1D;
  glBindTexture(target, mTexID);
  glTexSubImage1D(target, 0, 0, tableSize, GL_RGBA, GL_FLOAT, &mPalette[0]);
  glBindTexture(target, 0);
}



void LookupTable::setLinearLookupTable( const unsigned char r[],
                                        const unsigned char g[],
                                        const unsigned char b[],
                                        const float values[],
                                        int lengthColor ) {

  mColors.resize(lengthColor);
  const float K=1.0f/255.0f;

  for (int i=0; i<lengthColor; i++) {
    const Vector4f rgba(r[i]*K, g[i]*K, b[i]*K, 1);
    mColors[i] = rgba;
  }

  mValues.resize(lengthColor+1);

  for (int i=0; i<=lengthColor; i++) {
    mValues[i]=values[i];
  }
}


void LookupTable::setNonlinearLookupTable( const unsigned char r[],
                                           const unsigned char g[],
                                           const unsigned char b[],
                                           const float values[],
                                           int lengthColor ) {
  mColors.resize(lengthColor);
  const float K=1.0f/255.0f;

  for (int i=0; i<lengthColor; i++) {
    const Vector4f rgba(r[i]*K, g[i]*K, b[i]*K, 1);
    mColors[i] = rgba;
  }

  mValues.resize(lengthColor+1);

  for (int i=0; i<=lengthColor; i++) {
    mValues[i]=values[i];
  }

  mPalette.resize(lengthColor);
}



void LookupTable::adjustRangeNonlinear(float values[]) {

  if (this->getStrictFlag()) {
    return;
  }
  
  const int n = mValues.size();

  if (n<2) {
    return;
  }

  for (int i=0; i<n; i++) {
    mValues[i] = values[i];
  }

  int lengthColor = n-1;
  buildNonlinearPalette( mColors, mValues, lengthColor, mPalette, true );
  updateTexture();
}



void LookupTable::adjustRange(float minval, float maxval) {

  if (this->getStrictFlag()) {
    return;
  }

  const int n = mValues.size();

  if (n<2) {
    return;
  }
    
  // if we have a logarithmic colormap, hijack the processing and redirect to adjustRangeNonlinear()

  if (this->getNonlinearFlag()) {
    float levels[emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC+1] = { 0.0 };
    float logStart = minval > 0 ? log10(minval) : 0;
    float logEnd   = maxval > 0 ? log10(maxval) : 0;

    for (int i=0; i<emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC+1; i++) {
      levels[i] = logStart + (logEnd-logStart)*i/emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC;
      levels[i] = pow(10, levels[i]);
    }

    adjustRangeNonlinear(levels);
    int lengthColor = n-1;
    buildNonlinearPalette(mColors, mValues, lengthColor, mPalette, true);
    updateTexture();
    
  } else { // normal processing for linear colormaps
    const float inv_range = (maxval - minval);

    if (inv_range != 0) {

      for (int i=1; i<n-1; i++) {
        const float newval = minval + (i)*inv_range/(n-1);
        mValues[i] = newval;
      }

      mValues[0] = minval;
      mValues[n-1] = maxval;

      int lengthColor = n-1;
      buildPalette(mColors, mValues, lengthColor, mPalette, true);
      updateTexture();
    }
  }
}



//=========================================================================



void LookupTable::setNasaDepolarization() {
  //'nasa_depolarization': begin
  static unsigned char color_r[] = { 0, 102, 109, 251, 207, 207, 212, 213, 124, 255, 255, 255 };
  static unsigned char color_g[] = { 0, 161, 208, 255, 157,  48,   5, 174,  40, 255, 255, 255 };
  static unsigned char color_b[] = {125,251,  32,  80,  39,  28, 221, 194, 221, 255, 255, 255 };
  static float levels_calipso[] = {0.0, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1};
  const int rlen = sizeof(color_r)/sizeof(color_r[0]);
  this->setLinearLookupTable(color_r, color_g, color_b, levels_calipso, rlen);
}





// MJF - set a lookuptable that I can understand

void LookupTable::setMattLookupTable( int index, float minval, float maxval ) {
  colormapIndex = index; // Remember for use by GeoDataObject::getColormapIndex()
  const char* name = emvl::getRGBColormapName( index );
  const emvl::RGBColormap& curColormap = emvl::getRGBColormap( index );
  const unsigned char * color_r = curColormap[emvl::R];  
  const unsigned char * color_g = curColormap[emvl::G];  
  const unsigned char * color_b = curColormap[emvl::B];
  this->setStrictFlag(false); // default

  if (strncmp(name, "CALIPSO-Backscatter", 16) == 0) {
    float levels[] = {1e-4, 2e-4, 3e-4, 4e-4, 5e-4, 6e-4, 7e-4, 8e-4, 9e-4, 1e-3, 1.5e-3, 2e-3, 2.5e-3, 3e-3, 3.5e-3, 4e-3, 4.5e-3, 5e-3, 5.5e-3, 6e-3, 6.5e-3, 7e-3, 7.5e-3, 8e-3, 1e-2, 2e-2, 3e-2, 4e-2, 5e-2, 6e-2, 7e-2, 8e-2, 1e-1};
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_NASA);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);

  } else if (strncmp(name, "CALIPSO-Depolarization", 16) == 0) {
    float levels[] = {-1.0, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.0};
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_NASA_DEPOLARIZATION);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);

  } else if (strncmp(name, "AQI-PM25-24hr", 16) == 0) {
    float levels[] = {0.0, 12.0, 35.4, 55.4, 150.4, 250.5, 500}; // ug/m3
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_AQI);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);
    
  } else if (strncmp(name, "AQI-Ozone-1hr", 16) == 0) {
    float levels[] = {0.0, 125.0, 164.0, 204.0, 404.0, 504.0, 604.0}; // ppb
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_AQI);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);

  } else if (strncmp(name, "AQI-Ozone-8hr", 16) == 0) {
    float levels[] = {0.0, 54.0, 70.0, 85.0, 105.0, 200.0, 604.0}; // ppb
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_AQI);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);

  } else if (strncmp(name, "AQI-CO-8hr", 16) == 0) {
    float levels[] = {0.0, 4.4, 9.4, 12.4, 15.4, 30.4, 50.4}; // ppm
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_AQI);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);

  } else if (strncmp(name, "AQI-SO2-1hr", 16) == 0) {
    float levels[] = {0.0, 35.0, 75.0, 185.0, 304.0, 604.0, 1004.0}; // ppb
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_AQI);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);
    
  } else if (strncmp(name, "AQI-NO2-1hr", 16) == 0) {
    float levels[] = {0.0, 53.0, 100.0, 360.0, 649.0, 1249.0, 2049.0}; // ppb
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_AQI);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(true);
    
  } else if (strstr(name, "(log)") != NULL) {
    float levels[emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC+1] = { 0.0 };
    float logStart = minval > 0 ? log10(minval) : 0;
    float logEnd   = maxval > 0 ? log10(maxval) : 0;

    for (int i=0; i<emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC+1; i++) {
      levels[i] = logStart + (logEnd-logStart)*float(i)/emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC;
      levels[i] = pow(10, levels[i]);
    }
 
    this->setNonlinearFlag(true);
    this->setNonlinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_LOGARITHMIC);
    this->adjustRangeNonlinear(levels);
    this->setStrictFlag(false);

  } else if (strncmp(name, "All-White", 16) == 0) {
    float levels[emvl::RGBCOLORMAP_LENGTH_SOLID+1] = { 0.0 };

    for (int i=0; i<emvl::RGBCOLORMAP_LENGTH_SOLID+1; i++) {
      levels[i] = minval + (maxval-minval)*float(i)/emvl::RGBCOLORMAP_LENGTH_SOLID;
    }

    this->setNonlinearFlag(false);
    this->setLinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_SOLID);
    this->adjustRange(minval, maxval);
    this->setStrictFlag(false);

  } else if (strncmp(name, "All-Black", 16) == 0) {
    float levels[emvl::RGBCOLORMAP_LENGTH_SOLID+1] = { 0.0 };

    for (int i=0; i<emvl::RGBCOLORMAP_LENGTH_SOLID+1; i++) {
      levels[i] = minval + (maxval-minval)*float(i)/emvl::RGBCOLORMAP_LENGTH_SOLID;
    }

    this->setNonlinearFlag(false);
    this->setLinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_SOLID);
    this->adjustRange(minval, maxval);
    this->setStrictFlag(false);

  } else if (strncmp(name, "Difference", 16) == 0) {
    float levels[emvl::RGBCOLORMAP_LENGTH_DIFFERENCE+1] = { 0.0 };

    for (int i=0; i<emvl::RGBCOLORMAP_LENGTH_DIFFERENCE+1; i++) {
      levels[i] = minval + (maxval-minval)*float(i)/float(emvl::RGBCOLORMAP_LENGTH_DIFFERENCE);
    }

    this->setNonlinearFlag(false);
    this->setLinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_DIFFERENCE);
    this->adjustRange(minval, maxval);
    this->setStrictFlag(false);

  } else {
    float levels[emvl::RGBCOLORMAP_LENGTH_DEFAULT+1] = { 0.0 };

    for (int i=0; i<emvl::RGBCOLORMAP_LENGTH_DEFAULT+1; i++) {
      levels[i] = minval + (maxval-minval)*float(i)/emvl::RGBCOLORMAP_LENGTH_DEFAULT;
    }

    this->setNonlinearFlag(false);
    this->setLinearLookupTable(color_r, color_g, color_b, levels, emvl::RGBCOLORMAP_LENGTH_DEFAULT);
    this->adjustRange(minval, maxval);
    this->setStrictFlag(false);
  }
}




