/******************************************************************************
PURPOSE: GLSLProgramObject.h - defines GLSLProgramObject class

NOTES:   adapted from NVIDIA's sample code

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef GLSL_PROGRAM_OBJECT_H
#define GLSL_PROGRAM_OBJECT_H

#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h> // For GL*, gl*().
#else
#include <GL/gl.h> // For GL*, gl*().
#endif

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class GLSLProgramObject {
public:
  GLSLProgramObject();

  virtual ~GLSLProgramObject();

  void destroy();

  void bind();

  void unbind();

  void setUniform(const char* name, const GLfloat* val, int count);

  void setUniform(const char* name, const GLint* val, int count);

  void setTextureUnit(const char* texname, int texunit);

  void bindTexture(GLenum target, const char* texname, GLuint texid, int texunit);

  void bindTexture1D(const char* texname, GLuint texid, int texunit) {
    bindTexture(GL_TEXTURE_1D, texname, texid, texunit);
  }

  void bindTexture2D(const char* texname, GLuint texid, int texunit) {
    bindTexture(GL_TEXTURE_2D, texname, texid, texunit);
  }

  void bindTexture3D(const char* texname, GLuint texid, int texunit) {
    bindTexture(GL_TEXTURE_3D, texname, texid, texunit);
  }

  void bindTextureRECT(const char* texname, GLuint texid, int texunit) {
    bindTexture(GL_TEXTURE_RECTANGLE_ARB, texname, texid, texunit);
  }

  void bindTexture2DArray(const char* texname, GLuint texid, int texunit) {
#ifdef GL_TEXTURE_2D_ARRAY
        const unsigned int textarget = GL_TEXTURE_2D_ARRAY;
#else
#ifdef GL_TEXTURE_2D_ARRAY_EXT
        const unsigned int textarget = GL_TEXTURE_2D_ARRAY_EXT;
#endif
#endif
    bindTexture(textarget, texname, texid, texunit);
  }


  void bindFragDataLocation(GLuint  colorNumber,  const char*  name);

  GLint getFragDataLocation(const char* name);

  void attachVertexShaderString(const char* shaderprogram);

  void attachVertexShader(const char* filename);

  void attachFragmentShaderString(const char* shaderprogram);

  void attachFragmentShader(const char* filename);

  void attachGeometryShaderString(const char* shaderprogram);

  void attachGeometryShader(const char* filename);

  void link();

  inline GLuint getProgId() { return _progId; }

    void setMacro(const char *macro, const char *value);

protected:
  GLuint _progId;
  std::vector<GLuint>  _vertexShaders;
  std::vector<GLuint>  _fragmentShaders;
  std::vector<GLuint>  _geometryShaders;

  struct ShaderMacroStruct{
    std::string macro;
    std::string value;
  };

  std::vector<ShaderMacroStruct> mMacroList;
};

#endif
