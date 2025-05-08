/******************************************************************************
PURPOSE: RenderingContextRSIGBase.cpp - Implements RSIG's RenderingContextBase class.

NOTES: Implementation based on the RenderingContext class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <QDebug>

#include "RenderingContextRSIGBase.h"

#define assert_static(a) extern int unused_assert_static_[ (a) ? 1 : -1 ]
assert_static( sizeof (GLfloat) == sizeof (float) );

RenderingContextRSIGBase::RenderingContextRSIGBase(int mrtNum)
  : RenderingContext(mrtNum) {
}



RenderingContextRSIGBase::~RenderingContextRSIGBase() {
}



void RenderingContextRSIGBase::setLutToTextureChannel(int textype,
                                                      GLuint paletteID,
                                                      const char* glslTexName,
                                                      int channelID ) {
  //bind texture to the program

  if (textype==1) {
    mProgramObject.bindTexture1D(glslTexName, paletteID, channelID);
  } else if (textype==2) {
    mProgramObject.bindTexture2D(glslTexName, paletteID, channelID);
  } else {
    std::cerr << "Error: not supported texture dimmension, must be 1D or 2D!\n";
  }
}



void RenderingContextRSIGBase::setLutToTextureChannel0(GLuint /* paletteID */) {
  //setLutToTextureChannel(1, paletteID, "texPalette", 0);
}



void RenderingContextRSIGBase::setLutMinMax(float minval, float maxval) {
  const GLfloat minmaxVal[2] = {minval, maxval};
  mProgramObject.setUniform("minmaxVal", minmaxVal, 2);
}



void RenderingContextRSIGBase::setLutValues(const std::vector<float>& values) {
  const GLfloat* const glValues = (const GLfloat*) &values[ 0 ];
  const GLint numValues = (GLint) values.size();
  mProgramObject.setUniform("values", glValues, values.size());
  mProgramObject.setUniform("numValues", &numValues, 1);
}

void RenderingContextRSIGBase::setColorPaletteR(const std::vector<float>& values) {
  const GLfloat* const glValues = (const GLfloat*) &values[ 0 ];
  mProgramObject.setUniform("paletteR", glValues, values.size());
}

void RenderingContextRSIGBase::setColorPaletteG(const std::vector<float>& values) {
  const GLfloat* const glValues = (const GLfloat*) &values[ 0 ];
  mProgramObject.setUniform("paletteG", glValues, values.size());
}

void RenderingContextRSIGBase::setColorPaletteB(const std::vector<float>& values) {
  const GLfloat* const glValues = (const GLfloat*) &values[ 0 ];
  mProgramObject.setUniform("paletteB", glValues, values.size());
}


void RenderingContextRSIGBase::setTextureWeight(float _texWeight) {
  GLfloat texWeight = _texWeight;
  mProgramObject.setUniform("texWeight", &texWeight, 1);
}



void RenderingContextRSIGBase::setIsPointUniformParameter( int value ) {
  GLint isPoint = value;
  mProgramObject.setUniform( "isPoint", &isPoint, 1 );
}



