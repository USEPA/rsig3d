/******************************************************************************
PURPOSE: ShadersManagement.h - defines ShadersManagement class

NOTES:   depends on OpenGL, GLEW

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef SHADERSMANAGMENT_H
#define SHADERSMANAGMENT_H

//Windows header
#if _MSC_VER
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>

#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h> // For GL*, gl*().
#include <OpenGL/glu.h>
#else
#include <GL/gl.h> // For GL*, gl*().
#include <GL/glu.h>
#endif


//===============================================================

std::string loadTextFile(const char *name);
GLuint createShader(const char *fileName, GLuint shaderType, GLuint shaderID=0);
GLuint createShaderFromString(const char *shaderString, const char *filepath, GLuint shaderType, GLuint shaderID=0);

void checkProgramInfos(GLuint programID, GLuint stat);
GLuint createShaderProgram(const char *fileNameVS, const char *fileNameGS, const char *fileNameFS, GLuint programID=0);
void linkShaderProgram(GLuint programID);

void setShadersGlobalMacro(const char *macro, int val);
void setShadersGlobalMacro(const char *macro, float val);
void setShadersGlobalMacro(const char *macro, const char* val);
void resetShadersGlobalMacros();

#ifdef DEBUGGING
static void checkGLError(const char *functionName)  {
  GLenum error;

  while (( error = glGetError() ) != GL_NO_ERROR) {
    fprintf (stderr, "[%s] GL error %s \n", functionName, gluErrorString(error));
  }
}
#endif

#endif
