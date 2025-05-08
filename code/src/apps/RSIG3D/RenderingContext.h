/******************************************************************************
 PURPOSE: RenderingContext.h - defines RenderingContext class
 
 NOTES:   depends on OpenGL, GLEW
 
 HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
 ******************************************************************************/


#ifndef INC_RENDERING_CONTEXT
#define INC_RENDERING_CONTEXT

#if _MSC_VER
#include <windows.h>
#endif

#include <GL/glew.h>

#include "GLSLProgramObject.h"
#include "base_object.h"


class RenderingContext: public CObjectBase {

public:
  RenderingContext(const int MRTBufferNum);
  virtual ~RenderingContext();

  virtual void bind( const int pass,
                     const GLfloat* floatShaderParms,
                     const GLuint* intShaderParms ) = 0;

  virtual void unbind() = 0;
  virtual void initShaderProgram() = 0;
  virtual void destroyShaderProgram();
  virtual void initRenderTarget(const int w, const int h) = 0;
  virtual void deleteRenderTarget();

  static const GLenum mDrawBuffers[16];
  static void check_GL_errors(const int lineno);

  GLuint getTextureID() {
    return mTextureID;
  }

  const GLuint* getTextureIDs() {
    return mTextureIDs;
  }

  GLuint getFboID() {
    return mFboID;
  }

  int getMRTBufferNum() const {
    return mMrtBufferNum;
  }

  GLSLProgramObject& getProgramObject() {
    return mProgramObject;
  }

  void setMacro(const char *macro, const char *value);

protected:
  int mMrtBufferNum;      //number of MRT color buffers used
  GLuint mFboID;          //framebuffer object name
  GLuint mTextureID;      //2D or 3D texture name, if only one texture is used
  GLuint mTextureIDs[16]; //more texture names, if needed
  GLSLProgramObject mProgramObject;
};


#if 1
#define CHECK_GL_ERRORS  RenderingContext::check_GL_errors(__LINE__);
#define CHECK_GL_ERRORS2 \
if ( const GLenum code = glGetError() ) \
std::cerr << "OpenGL error " << code << " at "<<__FILE__ << ", line: " << __LINE__ << std::endl
#else
#define CHECK_GL_ERRORS
#define CHECK_GL_ERRORS2
#endif


#endif //INC_RENDERING_CONTEXT
