/******************************************************************************
PURPOSE: GLSLProgramObject.C - OpenGL's GLSL programObject

NOTES:   converted and modified from NVIDIA's sample code.

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#define NV_REPORT_COMPILE_ERRORS

#include <nvShaderUtils.h>
#include <nvSDKPath.h>
#include <assert.h>
#include "GLSLProgramObject.h"
#include "ShadersManagement.h"

static nv::SDKPath sdkPath;


GLSLProgramObject::GLSLProgramObject()
  : _progId(0) {
}


GLSLProgramObject::~GLSLProgramObject() {
  destroy();
}


/*!
 \fn void GLSLProgramObject::destroy()
  
 Destroy all the resources used in the class. It is called by the destructor.
 */

void GLSLProgramObject::destroy() {

  for (unsigned i = 0; i < _vertexShaders.size(); i++) {
    glDeleteShader(_vertexShaders[i]);
  }

  for (unsigned i = 0; i < _fragmentShaders.size(); i++) {
    glDeleteShader(_fragmentShaders[i]);
  }

  if (_progId != 0) {
    glDeleteProgram(_progId);
  }

  _progId = 0;
  _vertexShaders.clear();
  _fragmentShaders.clear();
}



void GLSLProgramObject::attachVertexShaderString(const char* shaderprogram) {
  GLuint shaderId = nv::CompileGLSLShader(GL_VERTEX_SHADER, shaderprogram);

  if (shaderId == 0) {
    const char *errmsg = "Error: Vertex shader failed to compile";
    std::cerr << errmsg << std::endl;
    return;
  }

  _vertexShaders.push_back(shaderId);
}



void GLSLProgramObject::attachVertexShader(const char* filename) {
  std::string resolved_path;

  if (sdkPath.getFilePath(filename, resolved_path)) {
    GLuint shaderId =
      nv::CompileGLSLShaderFromFile(GL_VERTEX_SHADER, resolved_path.c_str());

    if (shaderId == 0) {
      const char *errmsg = "Error: Vertex shader failed to compile";
      std::cerr << errmsg << std::endl;
      return;
    }

    _vertexShaders.push_back(shaderId);
  } else {
    std::cerr << "Error: Failed to find vertex shader" << std::endl;
    exit(1);
  }
}



void GLSLProgramObject::attachFragmentShaderString(const char* shaderprogram) {
  GLuint shaderId = createShaderFromString(shaderprogram, NULL, GL_FRAGMENT_SHADER);

  if (shaderId == 0) {
    std::cerr << "Error: Fragment shader failed to compile\n";
    return;
  }

  _fragmentShaders.push_back(shaderId);
}



void GLSLProgramObject::attachFragmentShader(const char* filename) {
  std::string resolved_path;

  if (sdkPath.getFilePath( filename, resolved_path)) {
    resetShadersGlobalMacros();

    for (size_t i=0; i<mMacroList.size(); i++) {
      setShadersGlobalMacro(mMacroList[i].macro.c_str(),
                            mMacroList[i].value.c_str());
    }

    GLuint shaderId = createShader(resolved_path.c_str(), GL_FRAGMENT_SHADER);

    if (shaderId == 0) {
      const char *errmsg = "Error: Fragment shader failed to compile\n";
      std::cerr << errmsg;
      return;
    }

    _fragmentShaders.push_back(shaderId);
    resetShadersGlobalMacros();
  } else {
    std::cerr << "Error: Failed to find fragment shader\n";
  }
}



void GLSLProgramObject::attachGeometryShaderString(const char* shaderprogram) {
  GLuint shaderId = createShaderFromString(shaderprogram, NULL, GL_GEOMETRY_SHADER);

  if (shaderId == 0) {
    const char *errmsg = "Error: Geometry shader failed to compile\n";
    std::cerr << errmsg;
    return;
  }

  _geometryShaders.push_back(shaderId);
}



void GLSLProgramObject::attachGeometryShader(const char* filename) {
  std::string resolved_path;
  if (sdkPath.getFilePath( filename, resolved_path)) {
    resetShadersGlobalMacros();

    for (size_t i=0; i<mMacroList.size(); i++) {
      setShadersGlobalMacro(mMacroList[i].macro.c_str(),
                            mMacroList[i].value.c_str());
    }

    GLuint shaderId = createShader(resolved_path.c_str(), GL_GEOMETRY_SHADER);

    if (shaderId == 0) {
      std::cerr << "Error: Geometry shader failed to compile\n";
      return;
    }

    _geometryShaders.push_back(shaderId);
    resetShadersGlobalMacros();
  } else {
    std::cerr << "Error: Failed to find geometry shader\n";
  }
}



void GLSLProgramObject::link() {

  if (!_progId) {
    _progId = glCreateProgram();
  }

  for (unsigned i = 0; i < _vertexShaders.size(); i++) {
    glAttachShader(_progId, _vertexShaders[i]);
  }

  for (unsigned i = 0; i < _fragmentShaders.size(); i++) {
    glAttachShader(_progId, _fragmentShaders[i]);
  }

  for (unsigned i = 0; i < _geometryShaders.size(); i++) {
    glAttachShader(_progId, _geometryShaders[i]);
  }

  glLinkProgram(_progId);
  GLint success = 0;
  glGetProgramiv(_progId, GL_LINK_STATUS, &success);

  if (!success) {
    char temp[2048];
    glGetProgramInfoLog(_progId, 1024, NULL, temp);
    std::cerr << "Failed to link program: " << temp << std::endl;
  }
}



void GLSLProgramObject::bind() {
  glUseProgram(_progId);
}



void GLSLProgramObject::unbind() {
  glUseProgram(0);
}



void GLSLProgramObject::setUniform(const char* name, const GLfloat* val, int count) {
  const GLint id = glGetUniformLocation(_progId, name);

  if (id == -1) {
    std::cerr << "Name: " << name << " not found in shader program!\n";
    return;
  }

  switch (count) {
  case 1:
    glUniform1fv(id, 1, val);
    break;
  case 2:
    glUniform2fv(id, 1, val);
    break;
  case 3:
    glUniform3fv(id, 1, val);
    break;
  case 4:
    glUniform4fv(id, 1, val);
    break;
  default:
    glUniform1fv(id, count, val); // values
    break;
  }
}



void GLSLProgramObject::setUniform(const char* name, const GLint* val, int count)  {
  const GLint id = glGetUniformLocation(_progId, name);

  if (id == -1) {
    std::cerr<<"Name: " << name << " not found in shader program!\n";
    return;
  }

  switch (count) {
  case 1:
    glUniform1iv(id, 1, val);
    break;
  case 2:
    glUniform2iv(id, 1, val);
    break;
  case 3:
    glUniform3iv(id, 1, val);
    break;
  case 4:
    glUniform4iv(id, 1, val);
    break;
  default:
    std::cerr<<"Invalid parameter in file "<<__FILE__<<", line "<<__LINE__<<std::endl;
    break;
  }
}



void GLSLProgramObject::setTextureUnit(const char* texname, int texunit) {
  GLint linked;
  glGetProgramiv(_progId, GL_LINK_STATUS, &linked);

  if (linked != GL_TRUE) {
    std::cerr << "Error: setTextureUnit needs program to be linked." << std::endl;
    return;
  }

  GLint id = glGetUniformLocation(_progId, texname);

  if (id == -1) {
    std::cerr << "Warning: Invalid texture " << texname << std::endl;
    return;
  }

  glUniform1i(id, texunit);
}



void GLSLProgramObject::bindTexture(GLenum target, const char* texname, GLuint texid, int texunit) {
  glActiveTexture(GL_TEXTURE0 + texunit);
  glBindTexture(target, texid);
  glActiveTexture(GL_TEXTURE0);
  setTextureUnit(texname, texunit);
}



void GLSLProgramObject::bindFragDataLocation(GLuint colorNumber,  const char* name) {

  if (!_progId) {
    _progId = glCreateProgram();
  }

  glBindFragDataLocationEXT(_progId,  colorNumber,  name);
}



GLint GLSLProgramObject::getFragDataLocation(const char* name)
{
  if (!_progId) return -1;
    return glGetFragDataLocationEXT(_progId,  name);
}



void GLSLProgramObject::setMacro(const char *macro, const char *value) {
  bool hit = false;

  for (size_t i=0; i<mMacroList.size(); i++) {
    ShaderMacroStruct& m = mMacroList[i];

    if (m.macro == macro) {
      m.value = value;
      hit = true;
      break;
    }
  }

  if (!hit) {
    ShaderMacroStruct m;
    m.macro = macro;
    m.value = value;
    mMacroList.push_back(m);
  }
}



