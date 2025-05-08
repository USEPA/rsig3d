/******************************************************************************
PURPOSE: RenderingContext.C - Implements the base RenderingContext class.
         A base class for setting up rendering context for
         Multiple-Rendering-Target (MRT)
         based rendering algorithms.
         The MRT technique means we can write multiple output in
         a single rendering pass, which is supported since OpenGL 3.0.

NOTES: Implemented using OpenGL.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "RenderingContext.h"

const GLenum RenderingContext::mDrawBuffers[16]={
  GL_COLOR_ATTACHMENT0_EXT,
  GL_COLOR_ATTACHMENT1_EXT,
  GL_COLOR_ATTACHMENT2_EXT,
  GL_COLOR_ATTACHMENT3_EXT,
  GL_COLOR_ATTACHMENT4_EXT,
  GL_COLOR_ATTACHMENT5_EXT,
  GL_COLOR_ATTACHMENT6_EXT,
  GL_COLOR_ATTACHMENT7_EXT
#ifdef GL_COLOR_ATTACHMENT8_EXT
    ,
    GL_COLOR_ATTACHMENT8_EXT,
  GL_COLOR_ATTACHMENT9_EXT,
  GL_COLOR_ATTACHMENT10_EXT,
  GL_COLOR_ATTACHMENT11_EXT,
  GL_COLOR_ATTACHMENT12_EXT,
  GL_COLOR_ATTACHMENT13_EXT,
  GL_COLOR_ATTACHMENT14_EXT,
  GL_COLOR_ATTACHMENT15_EXT
#endif
};



/*!
 Constructor. It defines 3 macros: MRT, CHECK_FIELD, MAPPING_FUNC. 
 */
RenderingContext::RenderingContext(const int MRTBufferNum) :
  mMrtBufferNum(MRTBufferNum),
  mFboID(0),
  mTextureID(0),
  mProgramObject() {

  const int n = sizeof(mTextureIDs)/sizeof(mTextureIDs[0]);

  for (int i=0; i<n; i++) {
    mTextureIDs[i]=0;
  }

  {
    const char *macro = "MRT";
    char value[256]="2";
    sprintf(value, "%d", mMrtBufferNum);
    mProgramObject.setMacro(macro, value);
  }

  {
    const char *macro = "CHECK_FIELD";
    char value[256]="1";
    mProgramObject.setMacro(macro, value);
  }

  {
    const char *macro = "MAPPING_FUNC";
    char value[256]="0";
    mProgramObject.setMacro(macro, value);
  }
}



/*!
 Destructor. 
 */
RenderingContext::~RenderingContext() {

  try {
    deleteRenderTarget();
  } catch(...) {
    std::cerr<<"Destructor error--"<<__FILE__<<": "<<__LINE__<<"\n";
  }
    
  try {
    destroyShaderProgram();
  } catch(...) {
    std::cerr<<"Destructor error--"<<__FILE__<<": "<<__LINE__<<"\n";
  }
}



void RenderingContext::check_GL_errors(const int lineno)  {
  const GLenum err = glGetError();

  if (err) {
    printf( "Error %x at line %d\n", err, lineno);
    exit(1);
  }
}



void RenderingContext::destroyShaderProgram() {
    mProgramObject.destroy();
}



void RenderingContext::deleteRenderTarget() {

  if (mFboID) {
      glDeleteFramebuffersEXT(1, &mFboID);
      mFboID=0;
  }

  if (mTextureID) {
    glDeleteTextures(1, &mTextureID);
    mTextureID=0;
  }

  int n = sizeof(mTextureIDs)/sizeof(mTextureIDs[0]);

  for (int i=0; i<n; i++) {

    if (mTextureIDs[i]) {
      glDeleteTextures(1, &mTextureIDs[i]);
      mTextureIDs[i] = 0;
    }
  }
}



void RenderingContext::setMacro(const char *macro, const char *value) {
  mProgramObject.setMacro(macro, value);
}



