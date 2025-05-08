/******************************************************************************
PURPOSE: RenderingContextRSIGBase.h - Base class for 2D shader rendering.

NOTES:  depends on OpenGL, GLEW

HISTORY: 2013/04/17, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef INC_RENDERCONTEXTRSIGBASE_H
#define INC_RENDERCONTEXTRSIGBASE_H

#include "RenderingContext.h"
#include <vector4f.h>

class RenderingContextRSIGBase: public RenderingContext {
public:
  RenderingContextRSIGBase(int numMRT = 0);
  ~RenderingContextRSIGBase();
  void setLutToTextureChannel0(GLuint paletteID);
  void setLutToTextureChannel(int textype, GLuint paletteID,
                              const char* glslTexName, int channelID);
  void setLutMinMax(float minval, float maxval);
  void setLutValues(const std::vector<float>&);
  void setColorPaletteR(const std::vector<float>&);
  void setColorPaletteG(const std::vector<float>&);
  void setColorPaletteB(const std::vector<float>&);
  void setTextureWeight(float _texWeight);
  void setIsPointUniformParameter( int value );
};

#endif //INC_RENDERCONTEXTRSIGBASE_H
