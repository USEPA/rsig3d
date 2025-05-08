/******************************************************************************
PURPOSE: ShadersManagement.C - utility class for loading OpenGL shaders

NOTES:

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#include "ShadersManagement.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

//Size of the string, the shorter is better
const int STRING_BUFFER_SIZE = 4096;

///////////////////////////////////////////

void defineMacro(std::string &shaderSource, const char *macro, const char *value);
void defineMacro(std::string &shaderSource, const char *macro, int val);
std::string manageIncludes(const std::string& src, const std::string& sourceFileName);

///////////////////////////////////////////

struct ShaderMacroStruct {
  std::string macro;
  std::string value;
};

static std::vector<ShaderMacroStruct> shadersMacroList;



void resetShadersGlobalMacros() {
  shadersMacroList.clear();
}



void setShadersGlobalMacro(const char *macro, int val) {
  ShaderMacroStruct ms;
  ms.macro=std::string(macro);
  char buff[128];
  sprintf(buff, "%d", val);
  ms.value=std::string(buff);
  shadersMacroList.push_back(ms);
}



void setShadersGlobalMacro(const char *macro, float val) {
  ShaderMacroStruct ms;
  ms.macro=std::string(macro);
  char buff[128];
  sprintf(buff, "%ff", val);
  ms.value=std::string(buff);
  shadersMacroList.push_back(ms);
}



void setShadersGlobalMacro(const char *macro, const char* val) {
  ShaderMacroStruct ms;
  ms.macro=std::string(macro);
  ms.value=std::string(val);
  shadersMacroList.push_back(ms);
}



//GLSL shader program creation

GLuint createShaderProgram(const char* fileNameVS, const char* fileNameGS,
                           const char* fileNameFS, GLuint programID ) {
  bool reload=programID!=0;
  GLuint vertexShaderID=0;
  GLuint geometryShaderID=0;
  GLuint fragmentShaderID=0;

  if ( ! reload ) {
    // Create GLSL program
    programID=glCreateProgram();
  } else {
    GLsizei count;
    GLuint shaders[3];
    glGetAttachedShaders(programID, 3, &count, shaders);

    for (int i=0; i<count; i++) {
      GLint shadertype;
      glGetShaderiv(  shaders[i], GL_SHADER_TYPE, &shadertype);

      if (shadertype == GL_VERTEX_SHADER) {
        vertexShaderID=shaders[i];
      } else if (shadertype == GL_GEOMETRY_SHADER) {
        geometryShaderID=shaders[i];
      } else if (shadertype == GL_FRAGMENT_SHADER) {
        fragmentShaderID=shaders[i];
      }
    }
  }

  if ( fileNameVS ) {
    // Create vertex shader
    vertexShaderID=createShader(fileNameVS, GL_VERTEX_SHADER, vertexShaderID);

    if ( ! reload ) {
      // Attach vertex shader to program object
      glAttachShader(programID, vertexShaderID);
    }
  }

  if ( fileNameGS ) {
    // Create geometry shader
    geometryShaderID=createShader(fileNameGS, GL_GEOMETRY_SHADER, geometryShaderID);

    if ( ! reload ) {
      // Attach vertex shader to program object
      glAttachShader(programID, geometryShaderID);
    }
  }

  if ( fileNameFS ) {
    // Create fragment shader
    fragmentShaderID=createShader(fileNameFS, GL_FRAGMENT_SHADER, fragmentShaderID);

    if ( ! reload ) {
      // Attach fragment shader to program object
      glAttachShader(programID, fragmentShaderID);
    }
  }
  return programID;
}



void linkShaderProgram(GLuint programID) {
  int linkStatus;
  glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

  // Link all shaders togethers into the GLSL program
  glLinkProgram(programID);
  checkProgramInfos(programID, GL_LINK_STATUS);

  // Validate program executability giving current OpenGL states
  glValidateProgram(programID);
  checkProgramInfos(programID, GL_VALIDATE_STATUS);
}



//GLSL shader creation (of a certain type, vertex shader, fragment shader oe geometry shader)

GLuint createShaderFromString( const char* shaderString,
                               const char* filepath,
                               GLuint shaderType,
                               GLuint shaderID ) {

  std::string shaderSource( shaderString );

  if ( shaderID == 0 ) {
    shaderID=glCreateShader(shaderType);
  }

  //Manage #includes
  std::string fileName;

  if ( filepath != NULL && filepath[0] != 0 ) {
    fileName = filepath;
  } else {
    fileName = "./default.glsl";
  }

  unsigned int shaderSize = shaderSource.size();

  for (int i=0; i<8; i++) {
    shaderSource = manageIncludes(shaderSource, fileName);

    if ( shaderSize == shaderSource.size() ) {
      break;
    }

    shaderSize = shaderSource.size();
  }

  //Define global macros

  for (unsigned int i=0; i<shadersMacroList.size(); i++) {
    const char *macro = shadersMacroList[i].macro.c_str();
    const char *value = shadersMacroList[i].value.c_str();
    defineMacro(shaderSource, macro, value);
  }

  //Passing shader source code to GL
  //Source used for "shaderID" shader, there is only "1" source code and
  //the string is NULL terminated (no sizes passed)
  const char* src = shaderSource.c_str();
  glShaderSource( shaderID, 1, &src, NULL );

  //Compile shader object
  glCompileShader( shaderID );

  //Check compilation status
  GLint ok;
  glGetShaderiv( shaderID, GL_COMPILE_STATUS, &ok );

  if ( ! ok ) {
    int ilength;
    char stringBuffer[STRING_BUFFER_SIZE];
    glGetShaderInfoLog(shaderID, STRING_BUFFER_SIZE, &ilength, stringBuffer);
    std::cout << "Compilation error (" << fileName << ") : " << stringBuffer;
  }

  return shaderID;
}



//GLSL shader creation (of a certain type, vertex shader, fragment shader oe geometry shader)

GLuint createShader( const char* fileName, GLuint shaderType, GLuint shaderID ) {
  GLuint newShaderID = 0;
  std::string shaderSource=loadTextFile(fileName);

  if (!shaderSource.empty()) {
    newShaderID =
      createShaderFromString(shaderSource.c_str(), fileName, shaderType, shaderID);
  }

  return newShaderID;
}


//Text file loading for shaders sources

std::string loadTextFile( const char* name ) {
  std::string buffer;
  std::ifstream file;
  file.open(name);

  if ( file.fail() ) {
    std::cerr << "loadFile: unable to open file: " << name;
  } else {
    buffer.reserve( 1024 * 1024 );
    std::string line;
    line.reserve( 80 );

    while ( std::getline( file, line ) ) {
      buffer += line;
      buffer += '\n';
    }
  }

  return buffer;
}



void checkProgramInfos( GLuint programID, GLuint stat ) {
  GLint ok = 0;
  glGetProgramiv(programID, stat, &ok);

  if ( ! ok ) {
    int ilength;
    char stringBuffer[STRING_BUFFER_SIZE];
    glGetProgramInfoLog(programID, STRING_BUFFER_SIZE, &ilength, stringBuffer);
    std::cout<<"Program error :\n"<<stringBuffer<<"\n";
  }
}



void defineMacro(std::string& shaderSource, const char* macro, const char* value ) {
  char buff[512];
  snprintf( buff, sizeof buff / sizeof *buff, "#define %s", macro );
  int mstart = (int)shaderSource.find(buff);
  snprintf( buff, sizeof buff / sizeof *buff, "#define %s %s\n", macro, value);

  if ( mstart >= 0 ) {
    int mlen = (int)shaderSource.find("\n", mstart)-mstart+1 ;
    std::string prevval( shaderSource.substr( mstart, mlen ) );

    if ( strcmp( prevval.c_str(), buff ) ) {
      shaderSource.replace(mstart, mlen, buff);
    }
  } else {
    shaderSource.insert(0, buff);
  }
}



std::string manageIncludes( const std::string& source, const std::string& sourceFileName ) {
  std::string src( source );
  std::string res;
  res.reserve(100000);

  char buff[512];
  sprintf(buff, "#include");
  size_t includepos = src.find(buff, 0);

  while ( includepos != std::string::npos ) {
    bool comment=src.substr(includepos-2, 2)==std::string("//");

    if ( ! comment ) {
      size_t fnamestartLoc = src.find("\"", includepos);
      size_t fnameendLoc = src.find("\"", fnamestartLoc+1);
      size_t fnamestartLib = src.find("<", includepos);
      size_t fnameendLib = src.find(">", fnamestartLib+1);
      size_t fnameEndOfLine = src.find("\n", includepos);
      size_t fnamestart;
      size_t fnameend;

      bool uselibpath=false;

      if ( ( fnamestartLoc == std::string::npos ||
             fnamestartLib < fnamestartLoc ) &&
            fnamestartLib < fnameEndOfLine ) {
        fnamestart=fnamestartLib;
        fnameend=fnameendLib;
        uselibpath=true;
      } else if ( fnamestartLoc != std::string::npos &&
                  fnamestartLoc < fnameEndOfLine ) {
        fnamestart=fnamestartLoc;
        fnameend=fnameendLoc;
        uselibpath=false;
      } else {
        return src;
      }

      std::string incfilename=src.substr(fnamestart+1, fnameend-fnamestart-1);
      std::string incsource;

      if ( uselibpath ) {
        std::string usedPath;

        //TODO: Add paths types into the manager -> search only onto shaders paths.
        std::vector<std::string> pathsList;
        pathsList.push_back("./");

        for ( std::vector<std::string>::iterator it = pathsList.begin();
              it != pathsList.end(); ++it ) {
          std::string fullpathtmp=(*it)+incfilename;
          FILE* file = fopen( fullpathtmp.c_str(), "r" );

          if ( file ) {
            usedPath=(*it);
            fclose(file);
            break;
          } else {
            usedPath="";
          }
        }

        if ( usedPath != "" ) {
          const std::string name( usedPath + incfilename );
          incsource = loadTextFile( name.c_str() );
        } else {
          return src;
        }
      } else {
        const size_t slash = sourceFileName.find_last_of( '/' );

        if ( slash != std::string::npos ) {
          const std::string path( sourceFileName.substr( 0, slash + 1 ) );
          const std::string name( path + incfilename );
          incsource = loadTextFile( name.c_str() );
        }
      }

      incsource = manageIncludes( incsource, sourceFileName );
      incsource = incsource.substr( 0, incsource.size() - 1 );
      std::string preIncludePart=src.substr( 0, includepos );
      std::string postIncludePart=src.substr( fnameend + 1, src.size() - fnameend );
      int numline = 0;
      size_t newlinepos = 0;

      do {
        newlinepos = preIncludePart.find( "\n", newlinepos + 1 );
        numline++;
      } while ( newlinepos != std::string::npos );

      numline--;
      char buff2[512];
      sprintf(buff2, "\n#line 0\n");
      std::string linePragmaPre(buff2);
      sprintf(buff2, "\n#line %d\n", numline);
      std::string linePragmaPost(buff2);
      res = preIncludePart+ linePragmaPre+incsource + linePragmaPost + postIncludePart;
      src = res;
    }

    includepos = src.find( buff, includepos + 1 );
  }

  return src;
}



