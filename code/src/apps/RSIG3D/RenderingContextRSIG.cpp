/******************************************************************************
PURPOSE: RenderingContextRSIG.C - Implements RSIG's RenderingContext class.

NOTES: Implementation based on the RenderingContext base class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#include "RenderingContextRSIG.h"

#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <QDebug>
#include <QString>


/*
 * The macro will wrap its argument as a double-quoted string and
 * prepend the version specification and append a "\n" so the string will end
 * with a newline.
 * Using it below will allow for more readable GLSL programs.
 *
 * NOTE: MISSING_VALUE must match value of emvl::MISSING_VALUE from
 * Dataset/DataUtilities.h. UGLY.
 */

#define GLSL_PROGRAM(s) \
"#version 120\n" \
"#pragma optimize(on)\n" \
"#pragma debug(on)\n" \
"#define Quaternion vec4\n" \
"#define MISSING_VALUE (-9.999e36)\n" \
#s "\n"



/******************************************************************************
PURPOSE: fragmentShader - Sets the color/alpha of the pixel.
INPUTS:  uniform float texWeight        Used for alpha blending?
         uniform int isMaplines         Is this a map lines pixel?
         uniform int maplineColorIndex  Map lines color index. Must match menu!
         uniform int isPoint            Is this a point data pixel?
         uniform sampler2D texSprite    if isPoint is 1 then use else ignore.
         //uniform sampler1D texPalette   Colormap lookup table.
         uniform float paletteR[32]     Maximum palette size in RGBColormap.h
         uniform float paletteG[32]     Maximum palette size in RGBColormap.h
         uniform float paletteB[32]     Mximum palette size in RGBColormap.h
         varying vec4 facecolor         Default background color if alpha = 0.
         uniform int numValues          Number of actual colormap threshholds.
         uniform float values[64]       Colormap threshhold lutValues.
         varying float dataValue        Data value from vertex shader.
         gl_TexCoord[0].x               Normalized colormap index of dataValue.
         gl_TexCoord[0].y               0.0 = ok, -1.0 = missing data value.
OUTPUTS: gl_FragColor  The pixel color/alpha.
NOTES:   This fragment shader (pixel processor) program is executed on each
         pixel after the vertexShader program. It needs to be very efficient!
******************************************************************************/

static const char* const fragmentShader =
GLSL_PROGRAM(
uniform float texWeight;
uniform int isMaplines;
uniform int maplineColorIndex;
uniform int isPoint;
uniform sampler2D texSprite;
//uniform sampler1D texPalette;
varying vec4 facecolor;
uniform int numValues;
uniform float values[64];
uniform float paletteR[32]; // maximum palette size in RGBColormap.h
uniform float paletteG[32]; // maximum palette size in RGBColormap.h
uniform float paletteB[32]; // maximum palette size in RGBColormap.h
varying float dataValue;

void main() {

  if ( isMaplines == 1 ) {
    float grayLevel = float( maplineColorIndex );

    if ( maplineColorIndex > 1 ) {
      grayLevel = float( maplineColorIndex - 1 ) * 0.1;
    }

    gl_FragColor = vec4( grayLevel, grayLevel, grayLevel, 1.0 );

    
    //} else if ( gl_PointCoord.x <= 0.1 && gl_PointCoord.y <= 0.1 ) {
    //// HACK to debug gl_PointCoord problem on systems that don't
    //// support point sprites correctly
    //gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );


  } else if ( ! ( gl_TexCoord[0].y > MISSING_VALUE ) ) { // Missing data value.
    gl_FragColor = facecolor;
  } else {
    float r = 1.0;
    
    if ( isPoint == 1 ) { // Only do texture2D() lookup for point data.
      r = texture2D( texSprite, gl_PointCoord ).r; // Pixel color in white glyph
    }

    if ( r < 0.5 ) { // Non-white means outside white glyph.
      discard;
    } else {
      vec3 texcolor = vec3( 0.0, 0.0, 0.0 ); // Colormap RGB color of pixel.

      // HACK: Apparently on newer Macs (2023) 1D texture does not work!
      // SLOW. Replace it with explict code to set the color.

      //if ( numValues == 7 ) { // HACK: Length 7 means use AQI colors.
      //
      //  if ( dataValue <= values[1] ) {
      //    //texcolor.g = 228.0 / 255.0;
      //    texcolor.r = paletteR[0];
      //    texcolor.g = paletteG[0];
      //    texcolor.b = paletteB[0];
      //  } else if ( dataValue <= values[2] ) {
      //    //texcolor.r = 1.0;
      //    //texcolor.g = 1.0;
      //    texcolor.r = paletteR[1];
      //    texcolor.g = paletteG[1];
      //   texcolor.b = paletteB[1];
      //  } else if ( dataValue <= values[3] ) {
      //    //texcolor.r = 1.0;
      //    //texcolor.g = 126.0 / 255.0;
      //    texcolor.r = paletteR[2];
      //    texcolor.g = paletteG[2];
      //    texcolor.b = paletteB[2];
      //  } else if ( dataValue <= values[4] ) {
      //    //texcolor.r = 1.0;
      //    texcolor.r = paletteR[3];
      //    texcolor.g = paletteG[3];
      //    texcolor.b = paletteB[3];
      //  } else if ( dataValue <= values[5] ) {
      //    //texcolor.r = 143.0 / 255.0;
      //    //texcolor.g =  63.0 / 255.0;
      //    //texcolor.b = 151.0 / 255.0;
      //    texcolor.r = paletteR[4];
      //    texcolor.g = paletteG[4];
      //    texcolor.b = paletteB[4];
      //  } else {
      //    //texcolor.r = 126.0 / 255.0;
      //    //texcolor.b =  35.0 / 255.0;
      //    texcolor.r = paletteR[5];
      //    texcolor.g = paletteG[5];
      //    texcolor.b = paletteB[5];
      //  }

      //} else { // Index into colormap to get color:
        //float texcoord = gl_TexCoord[0].x; // Colormap index in [0.0, 1.0].
        //texcolor = texture1D( texPalette, texcoord ).rgb;
      //}

      if ( dataValue > MISSING_VALUE ) {
        /*const*/ int count = numValues <= 32 ? numValues : 32;
        /*const*/ int lastIndex = count - 1;
        /*const*/ float lastValue = values[ lastIndex ];

        if ( dataValue >= lastValue ) {
          /*const*/ int lastIndex_1 = lastIndex - 1;
          texcolor.r = paletteR[ lastIndex_1 ];
          texcolor.g = paletteG[ lastIndex_1 ];
          texcolor.b = paletteB[ lastIndex_1 ];
        } else {

          // The values may not be equally-spaced so must search:

          for ( int index = 1; index < count; ++index ) {

            if ( dataValue <= values[ index ] ) {

              // paletteRGB[] has one less element than values[]:

              /*const*/ int index_1 = index - 1;
              texcolor.r = paletteR[ index_1 ];
              texcolor.g = paletteG[ index_1 ];
              texcolor.b = paletteB[ index_1 ];
              break;
            }
          }
        }
      }

      vec3 color = facecolor.rgb * ( 1.0 - texWeight ) + texcolor * texWeight;
      gl_FragColor = vec4( color, facecolor.w );
    }
  }
}
);



/* mapline stuff */
GLint* getMaplineColorIndex(QString color) {
  static GLint indexBlack  = 0;
  static GLint indexWhite  = 1;
  static GLint indexGray10 = 2;
  static GLint indexGray20 = 3;
  static GLint indexGray30 = 4;
  static GLint indexGray40 = 5;
  static GLint indexGray50 = 6;
  static GLint indexGray60 = 7;
  static GLint indexGray70 = 8;
  static GLint indexGray80 = 9;
  static GLint indexGray90 = 10;

  if (color.compare("Black")  == 0) { return &indexBlack; }
  if (color.compare("White")  == 0) { return &indexWhite; }
  if (color.compare("Gray10") == 0) { return &indexGray10; }
  if (color.compare("Gray20") == 0) { return &indexGray20; }
  if (color.compare("Gray30") == 0) { return &indexGray30; }
  if (color.compare("Gray40") == 0) { return &indexGray40; }
  if (color.compare("Gray50") == 0) { return &indexGray50; }
  if (color.compare("Gray60") == 0) { return &indexGray60; }
  if (color.compare("Gray70") == 0) { return &indexGray70; }
  if (color.compare("Gray80") == 0) { return &indexGray80; }
  if (color.compare("Gray90") == 0) { return &indexGray90; }

  return 0;
}



// mapline flag

void MarbleUVProjectionRC::setMaplineFlag(GLuint, int isMaplines, QString maplineColor) {
  mProgramObject.setUniform("isMaplines", &isMaplines, 1);
  mProgramObject.setUniform("maplineColorIndex", getMaplineColorIndex(maplineColor), 1);
}



void MarbleSphericalProjectionRC::setMaplineFlag(GLuint, int isMaplines, QString maplineColor) {
  mProgramObject.setUniform("isMaplines", &isMaplines, 1);
  mProgramObject.setUniform("maplineColorIndex", getMaplineColorIndex(maplineColor), 1);
}



void MarbleMercatorProjectionRC::setMaplineFlag(GLuint, int isMaplines, QString maplineColor) {
  mProgramObject.setUniform("isMaplines", &isMaplines, 1);
  mProgramObject.setUniform("maplineColorIndex", getMaplineColorIndex(maplineColor), 1);
}



void QMapMercatorProjectionRC::setMaplineFlag(GLuint, int isMaplines, QString maplineColor) {
  mProgramObject.setUniform("isMaplines", &isMaplines, 1);
  mProgramObject.setUniform("maplineColorIndex", getMaplineColorIndex(maplineColor), 1);
}



void LambertConformalConicProjectionRC::setMaplineFlag(GLuint, int isMaplines, QString maplineColor) {
  mProgramObject.setUniform("isMaplines", &isMaplines, 1);
  mProgramObject.setUniform("maplineColorIndex", getMaplineColorIndex(maplineColor), 1);
}



void StereographicProjectionRC::setMaplineFlag(GLuint, int isMaplines, QString maplineColor) {
  mProgramObject.setUniform("isMaplines", &isMaplines, 1);
  mProgramObject.setUniform("maplineColorIndex", getMaplineColorIndex(maplineColor), 1);
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*!
 \class MarbleUVProjectionRC
 \brief This class implements the Longitude-Latitue projection algorithm in 
 shader program as the rendering context. 
 
 The paramters in projection are accquired from the MarbleWidget. 
 */

MarbleUVProjectionRC::MarbleUVProjectionRC() : RenderingContextRSIGBase(0) {
}



MarbleUVProjectionRC::~MarbleUVProjectionRC() {
}



void MarbleUVProjectionRC::bind() {
  CHECK_GL_ERRORS2;
  mProgramObject.bind();
  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_LOGIC_OP);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  CHECK_GL_ERRORS2;
}



void MarbleUVProjectionRC::setParameters(
  GLuint /* paletteID */,
  const GLfloat uvCenter[2],
  const GLfloat viewSize[2],
  float rad2Pixel,
  const std::vector<float>& lutValues,
  const std::vector<float>& paletteR,
  const std::vector<float>& paletteG,
  const std::vector<float>& paletteB,
  float texWeight ) {

  const GLfloat* glLutValues = (const GLfloat*) &lutValues[ 0 ];
  const GLint numValues = lutValues.size();

  const GLfloat* glPaletteValuesR = (const GLfloat*) &paletteR[ 0 ];
  const GLfloat* glPaletteValuesG = (const GLfloat*) &paletteG[ 0 ];
  const GLfloat* glPaletteValuesB = (const GLfloat*) &paletteB[ 0 ];
  const GLint    paletteSize = paletteR.size();
  
  //bind texture
  //mProgramObject.bindTexture1D("texPalette", paletteID, 0);

  //bind uniforms
  mProgramObject.setUniform("uvcenter", uvCenter, 2);
  mProgramObject.setUniform("viewsize", viewSize, 2);
  mProgramObject.setUniform("rad2pixel", &rad2Pixel, 1);
  mProgramObject.setUniform("texWeight", &texWeight, 1);
  mProgramObject.setUniform("values", glLutValues, numValues );
  mProgramObject.setUniform("paletteR", glPaletteValuesR, paletteSize );
  mProgramObject.setUniform("paletteG", glPaletteValuesG, paletteSize );
  mProgramObject.setUniform("paletteB", glPaletteValuesB, paletteSize );
  mProgramObject.setUniform("numValues", &numValues, 1);
  setIsPointUniformParameter( 0 );
  CHECK_GL_ERRORS2;
}



void MarbleUVProjectionRC::unbind() {
  glDisable(GL_BLEND);
  mProgramObject.unbind();
}


void MarbleUVProjectionRC::initShaderProgram() {

/******************************************************************************
PURPOSE: vertexShader - Sets the color/alpha/texture of a vertex.
INPUTS:  uniform vec2 uvcenter.xy       Radians of vertex.
         uniform vec2 viewsize.xy       Viewport width,height in pixels.
         uniform float rad2pixel        Radians per pixel.
         uniform int numValues          Number of actual colormap threshholds.
         uniform float values[64]       Colormap threshhold lutValues.
         gl_Vertex.xy                   Longitude, Latitude  of vertex.
         gl_Vertex.w                    Data value of vertex.
         gl_ModelViewProjectionMatrix
OUTPUTS: varying float dataValue        Data value of vertex.
         varying vec4 facecolor         Color of vertex.
         gl_TexCoord[0].x               Normalized colormap index of dataValue.
         gl_TexCoord[0].y               0.0 = ok, -1.0 = missing data value.
         gl_FrontColor                  The vertex color/alpha.
         gl_BackColor                   The vertex color/alpha.
         gl_Position                    The vertex xyzw position.
NOTES:   This vertex shader (vertex processor) program is executed on each
         vertex before the fragmentShader program.
         It needs to be very efficient!
******************************************************************************/

  const char* const vertexShader =
  GLSL_PROGRAM(
    uniform vec2 uvcenter;
    uniform vec2 viewsize;
    uniform float rad2pixel;
    uniform int numValues;
    uniform float values[64];
    varying vec4 facecolor;
    varying float dataValue;

    // Get screen pixel x,y of lon,lat vertex:

    vec2 worldCoordinateToScreen( in vec2 lonlat ) {
      vec2 uv = radians( lonlat );
      float centerLon = uvcenter.x;
      float centerLat = uvcenter.y;
      float x = ( viewsize.x * 0.5 + ( uv.x - centerLon ) * rad2pixel );
      float y = ( viewsize.y * 0.5 - ( uv.y - centerLat ) * rad2pixel );
      return vec2(x, y);
    }

    void main() {
      float imageHeight = viewsize.y;
      vec4 wpos;
      wpos.xy = worldCoordinateToScreen( gl_Vertex.xy );
      wpos.y = imageHeight - wpos.y;
      wpos.z = 0.5;
      wpos.w = 1.0;
      gl_Position = gl_ModelViewProjectionMatrix * wpos;

      // Compute t = normalized colormap index:

      dataValue = gl_Vertex.w;
      int index = 0;

      while ( index < numValues && values[ index ] < dataValue ) {
        ++index;
      }

      float t = index <= 1 ? 0.0 : float(index - 1) / numValues;

      gl_TexCoord[0].x = t + 1e-20; // Index into colormap in range [1e-20, 1.0]
      gl_TexCoord[0].y = 0.0;

      if ( ! ( dataValue > MISSING_VALUE ) ) {
        gl_TexCoord[0].y = -1.0; // Flag indicating missing in fragmentShader.
      }

      gl_FrontColor = gl_Color;
      gl_BackColor  = gl_Color;
      facecolor     = gl_Color;
    }
  );

  mProgramObject.attachVertexShaderString(vertexShader);
  mProgramObject.attachFragmentShaderString(fragmentShader);
  mProgramObject.link();
  CHECK_GL_ERRORS2;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/*!
 \class MarbleMercatorProjectionRC
 \brief This class implements the Mercator projection algorithm in 
 shader program as the rendering context. 
 
 The paramters in projection are accquired from the MarbleWidget. 
 */

MarbleMercatorProjectionRC::MarbleMercatorProjectionRC()
  : RenderingContextRSIGBase(0) {
}



MarbleMercatorProjectionRC::~MarbleMercatorProjectionRC() {
}



void MarbleMercatorProjectionRC::bind() {
  mProgramObject.bind();
  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_LOGIC_OP);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  CHECK_GL_ERRORS2;
}



void MarbleMercatorProjectionRC::setParameters( GLuint /* paletteID */,
                                                const GLfloat uvCenter[2],
                                                const GLfloat viewSize[2],
                                                float rad2Pixel,
                                                const std::vector<float>& lutValues,
                                                const std::vector<float>& paletteR,
                                                const std::vector<float>& paletteG,
                                                const std::vector<float>& paletteB,
                                                float texWeight ) {

  const GLfloat* glLutValues = (const GLfloat*) &lutValues[ 0 ];
  const GLint numValues = lutValues.size();

  const GLfloat* glPaletteValuesR = (const GLfloat*) &paletteR[ 0 ];
  const GLfloat* glPaletteValuesG = (const GLfloat*) &paletteG[ 0 ];
  const GLfloat* glPaletteValuesB = (const GLfloat*) &paletteB[ 0 ];
  const GLint    paletteSize = paletteR.size();

  //bind texture
  //mProgramObject.bindTexture1D("texPalette", paletteID, 0);

  //bind uniforms
  mProgramObject.setUniform("uvcenter", uvCenter, 2);
  mProgramObject.setUniform("viewsize", viewSize, 2);
  mProgramObject.setUniform("rad2pixel", &rad2Pixel, 1);
  mProgramObject.setUniform("texWeight", &texWeight, 1);
  mProgramObject.setUniform("values", glLutValues, numValues );
  mProgramObject.setUniform("paletteR", glPaletteValuesR, paletteSize );
  mProgramObject.setUniform("paletteG", glPaletteValuesG, paletteSize );
  mProgramObject.setUniform("paletteB", glPaletteValuesB, paletteSize );
  mProgramObject.setUniform("numValues", &numValues, 1);
  setIsPointUniformParameter( 0 );
  CHECK_GL_ERRORS2;
}



void MarbleMercatorProjectionRC::unbind() {
  glDisable(GL_BLEND);
  mProgramObject.unbind();
}


void MarbleMercatorProjectionRC::initShaderProgram() {

/******************************************************************************
PURPOSE: vertexShader - Sets the color/alpha/texture of a vertex.
INPUTS:  uniform vec2 uvcenter.xy       Radians of vertex.
         uniform vec2 viewsize.xy       Viewport width,height in pixels.
         uniform float rad2pixel        Radians per pixel.
         uniform int numValues          Number of actual colormap threshholds.
         uniform float values[64]       Colormap threshhold lutValues.
         gl_Vertex.xy                   Longitude, Latitude  of vertex.
         gl_Vertex.w                    Data value of vertex.
         gl_ModelViewProjectionMatrix
OUTPUTS: varying float dataValue        Data value of vertex.
         varying vec4 facecolor         Color of vertex.
         gl_TexCoord[0].x               Normalized colormap index of dataValue.
         gl_TexCoord[0].y               0.0 = ok, -1.0 = missing data value.
         gl_FrontColor                  The vertex color/alpha.
         gl_BackColor                   The vertex color/alpha.
NOTES:   This vertex shader (vertex processor) program is executed on each
         vertex before the fragmentShader program.
         It needs to be very efficient!
******************************************************************************/

  const char* const vertexShader =
  GLSL_PROGRAM(
    uniform vec2 uvcenter;
    uniform vec2 viewsize;
    uniform float rad2pixel;
    uniform int numValues;
    uniform float values[64];
    varying float dataValue;
    varying vec4 facecolor;

    // GLSL BUG: Workaround missing built-in math function:
    float atanh( float x ) { return 0.5 * log((1.0 + x) / (1.0 - x)); }

    // Get screen pixel x,y of lon,lat vertex:

    vec2 worldCoordinateToScreen( in vec2 lonlat ) {
      const float MAXLAT = 85.05113;
      lonlat.y = clamp( lonlat.y, -MAXLAT, MAXLAT );
      vec2 uv = radians( lonlat );
      float centerLon = uvcenter.x;
      float centerLat = uvcenter.y;
      float x = viewsize.x * 0.5 + ( uv.x - centerLon ) * rad2pixel;
      float y = viewsize.y * 0.5 -
                  ( atanh( sin( uv.y ) ) - atanh( sin( centerLat ) ) )
                  * rad2pixel;
      return vec2(x, y);
    }

    void main() {
      float imageHeight = viewsize.y;
      vec4 wpos;
      wpos.xy = worldCoordinateToScreen( gl_Vertex.xy );
      wpos.y = imageHeight - wpos.y;
      wpos.z = 0.5;
      wpos.w = 1.0;
      gl_Position = gl_ModelViewProjectionMatrix * wpos;

      // Compute t = normalized colormap index:

      dataValue = gl_Vertex.w;
      int index = 0;

      while ( index < numValues && values[ index ] < dataValue ) {
        ++index;
      }

      float t = index <= 1 ? 0.0 : float(index - 1) / numValues;

      gl_TexCoord[0].x = t + 1e-20; // Index into colormap in range [1e-20, 1.0]
      gl_TexCoord[0].y = 0.0;

      if ( ! ( dataValue > MISSING_VALUE ) ) {
        gl_TexCoord[0].y = -1.0; // Flag indicating missing in fragmentShader.
      }

      gl_FrontColor = gl_Color;
      gl_BackColor  = gl_Color;
      facecolor     = gl_Color;
    }
  );

  mProgramObject.attachVertexShaderString(vertexShader);
  mProgramObject.attachFragmentShaderString(fragmentShader);
  mProgramObject.link();
  CHECK_GL_ERRORS2;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*!
 \class MarbleSphericalProjectionRC
 \brief This class implements the Spherical projection algorithm in 
 shader program as the rendering context. 
 
 The paramters in projection are accquired from the MarbleWidget. 
 */

MarbleSphericalProjectionRC::MarbleSphericalProjectionRC()
  : RenderingContextRSIGBase(0) {
}



MarbleSphericalProjectionRC::~MarbleSphericalProjectionRC() {
}



void MarbleSphericalProjectionRC::bind() {
  mProgramObject.bind();
  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_LOGIC_OP);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void MarbleSphericalProjectionRC::setParameters( GLuint /* paletteID */,
                                                 const GLfloat axis[4],
                                                 const GLfloat viewSize[2],
                                                 float radius,
                                                 const std::vector<float>& lutValues,
                                                 const std::vector<float>& paletteR,
                                                 const std::vector<float>& paletteG,
                                                 const std::vector<float>& paletteB,
                                                 float texWeight ) {

  const GLfloat* glLutValues = (const GLfloat*) &lutValues[ 0 ];
  const GLint numValues = lutValues.size();

  const GLfloat* glPaletteValuesR = (const GLfloat*) &paletteR[ 0 ];
  const GLfloat* glPaletteValuesG = (const GLfloat*) &paletteG[ 0 ];
  const GLfloat* glPaletteValuesB = (const GLfloat*) &paletteB[ 0 ];
  const GLint    paletteSize = paletteR.size();

  //bind texture
  //mProgramObject.bindTexture1D("texPalette", paletteID, 0);

  //bind uniforms
  mProgramObject.setUniform("axis", axis, 4);
  mProgramObject.setUniform("viewsize", viewSize, 2);
  mProgramObject.setUniform("radius", &radius, 1);
  mProgramObject.setUniform("texWeight", &texWeight, 1);
  mProgramObject.setUniform("values", glLutValues, numValues);
  mProgramObject.setUniform("paletteR", glPaletteValuesR, paletteSize );
  mProgramObject.setUniform("paletteG", glPaletteValuesG, paletteSize );
  mProgramObject.setUniform("paletteB", glPaletteValuesB, paletteSize );
  mProgramObject.setUniform("numValues", &numValues, 1);
  setIsPointUniformParameter( 0 );
  CHECK_GL_ERRORS2;
}



void MarbleSphericalProjectionRC::unbind() {
  glDisable(GL_BLEND);
  mProgramObject.unbind();
}



void MarbleSphericalProjectionRC::initShaderProgram() {

/******************************************************************************
PURPOSE: vertexShader - Sets the color/alpha/texture of a vertex.
INPUTS:  uniform vec2 uvcenter.xy       Radians of vertex.
         uniform vec2 viewsize.xy       Viewport width,height in pixels.
         uniform float rad2pixel        Radians per pixel.
         uniform int numValues          Number of actual colormap threshholds.
         uniform float values[64]       Colormap threshhold lutValues.
         gl_Vertex.xy                   Longitude, Latitude  of vertex.
         gl_Vertex.w                    Data value of vertex.
         gl_ModelViewProjectionMatrix
OUTPUTS: varying float dataValue        Data value of vertex.
         varying vec4 facecolor         Color of vertex.
         gl_TexCoord[0].x               Normalized colormap index of dataValue.
         gl_TexCoord[0].y               0.0 = ok, -1.0 = missing data value.
         gl_FrontColor                  The vertex color/alpha.
         gl_BackColor                   The vertex color/alpha.
NOTES:   This vertex shader (vertex processor) program is executed on each
         vertex before the fragmentShader program.
         It needs to be very efficient!
******************************************************************************/

  const char* const vertexShader =
  GLSL_PROGRAM(
    uniform vec4 axis;
    uniform vec2 viewsize;
    uniform float radius;
    uniform int numValues;
    uniform float values[64];
    varying float dataValue;
    varying vec4 facecolor;

    const int Q_X = 0;
    const int Q_Y = 1;
    const int Q_Z = 2;
    const int Q_W = 3;

    Quaternion quat_fromSpherical(in float lon, in float lat) {
      float w = 0.0;
      float x = cos(lat) * sin(lon);
      float y = sin(lat);
      float z = cos(lat) * cos(lon);
      return Quaternion( x, y, z, w );
    }

    Quaternion quat_mult(in Quaternion p, in Quaternion q) {
      float w = p[Q_W] * q[Q_W] - p[Q_X] * q[Q_X] - p[Q_Y] * q[Q_Y] - p[Q_Z] * q[Q_Z];
      float x = p[Q_W] * q[Q_X] + p[Q_X] * q[Q_W] + p[Q_Y] * q[Q_Z] - p[Q_Z] * q[Q_Y];
      float y = p[Q_W] * q[Q_Y] - p[Q_X] * q[Q_Z] + p[Q_Y] * q[Q_W] + p[Q_Z] * q[Q_X];
      float z = p[Q_W] * q[Q_Z] + p[Q_X] * q[Q_Y] - p[Q_Y] * q[Q_X] + p[Q_Z] * q[Q_W];
      return Quaternion( x, y, z, w);
    }

    Quaternion quat_rotateAroundAxis(in Quaternion p, in Quaternion q) {
      float w =  p[Q_X] * q[Q_X] + p[Q_Y] * q[Q_Y] + p[Q_Z] * q[Q_Z];
      float x =  p[Q_X] * q[Q_W] - p[Q_Y] * q[Q_Z] + p[Q_Z] * q[Q_Y];
      float y =  p[Q_X] * q[Q_Z] + p[Q_Y] * q[Q_W] - p[Q_Z] * q[Q_X];
      float z = -p[Q_X] * q[Q_Y] + p[Q_Y] * q[Q_X] + p[Q_Z] * q[Q_W];
      Quaternion r = quat_mult(q , Quaternion( x, y, z, w));
      return r;
    }

    vec2 worldCoordinateToScreen(in vec2 uv, out float visible) {
      float lon = radians(uv.x);
      float lat = radians(uv.y);
      Quaternion p = quat_fromSpherical( lon, lat );
      p = quat_rotateAroundAxis( p, axis );
      float x = viewsize.x * 0.5 + radius * p[Q_X] ;
      float y = viewsize.y * 0.5 - radius * p[Q_Y];
      visible = 1.0;

      if (p.z <= 0.0) {
        visible = 0.01;
      }

      return vec2(x, y);
    }

    void main() {
      float imageHeight = viewsize.y;
      vec4 wpos;
      float visible;
      wpos.xy = worldCoordinateToScreen(gl_Vertex.xy, visible);
      wpos.y = imageHeight - wpos.y;
      wpos.z = 0.5;
      wpos.w = 1.0;
      gl_Position = gl_ModelViewProjectionMatrix * wpos;

      // Compute t = normalized colormap index:

      dataValue = gl_Vertex.w;
      int index = 0;

      while ( index < numValues && values[ index ] < dataValue ) {
        ++index;
      }

      float t = index <= 1 ? 0.0 : float(index - 1) / numValues;

      gl_TexCoord[0].x = t + 1e-20; // Index into colormap in range [1e-20, 1.0]
      gl_TexCoord[0].y = 0.0;

      if ( ! ( dataValue > MISSING_VALUE ) ) {
        gl_TexCoord[0].y = -1.0; // Flag indicating missing in fragmentShader.
      }

      gl_FrontColor = gl_BackColor = gl_Color;
      facecolor = vec4(gl_Color.xyz, visible * gl_Color.w);
    }
  );

  mProgramObject.attachVertexShaderString(vertexShader);
  mProgramObject.attachFragmentShaderString(fragmentShader);
  mProgramObject.link();
  CHECK_GL_ERRORS2;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*!
 \class QMapMercatorProjectionRC
 \brief This class implements the Mercator projection algorithm in 
 shader program as the rendering context for online maps. 
 
 The paramters in projection are accquired from the QMapControl. 
 */

QMapMercatorProjectionRC::QMapMercatorProjectionRC()
  : RenderingContextRSIGBase(0) {
}



QMapMercatorProjectionRC::~QMapMercatorProjectionRC() {
}



void QMapMercatorProjectionRC::bind() {
  mProgramObject.bind();
  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_LOGIC_OP);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}



void QMapMercatorProjectionRC::setParameters( GLuint /* paletteid */,
                                              GLfloat imageHeight,
                                              GLfloat numberOfTiles,
                                              GLfloat mytilesize,
                                              const GLfloat midp[2],
                                              const GLfloat screenMiddle[2],
                                              const std::vector<float>& lutValues,
                                              const std::vector<float>& paletteR,
                                              const std::vector<float>& paletteG,
                                              const std::vector<float>& paletteB,
                                              float texWeight ) {

  const GLfloat* glLutValues = (const GLfloat*) &lutValues[ 0 ];
  const GLint numValues = lutValues.size();

  const GLfloat* glPaletteValuesR = (const GLfloat*) &paletteR[ 0 ];
  const GLfloat* glPaletteValuesG = (const GLfloat*) &paletteG[ 0 ];
  const GLfloat* glPaletteValuesB = (const GLfloat*) &paletteB[ 0 ];
  const GLint    paletteSize = paletteR.size();
  
  //bind texture
  //mProgramObject.bindTexture1D("texPalette", paletteid, 0);

  //bind uniforms
  //mProgramObject.setUniform("minmaxVal", minmaxVal, 2);
  mProgramObject.setUniform("imageHeight", &imageHeight, 1);
  mProgramObject.setUniform("numberOfTiles", &numberOfTiles, 1);
  mProgramObject.setUniform("mytilesize", &mytilesize, 1);
  mProgramObject.setUniform("midp", midp, 2);
  mProgramObject.setUniform("screenMiddle", screenMiddle, 2);
  mProgramObject.setUniform("texWeight", &texWeight, 1);
  mProgramObject.setUniform("values", glLutValues, numValues);
  mProgramObject.setUniform("paletteR", glPaletteValuesR, paletteSize );
  mProgramObject.setUniform("paletteG", glPaletteValuesG, paletteSize );
  mProgramObject.setUniform("paletteB", glPaletteValuesB, paletteSize );
  mProgramObject.setUniform("numValues", &numValues, 1);
  setIsPointUniformParameter( 0 );
  CHECK_GL_ERRORS2;
}



void QMapMercatorProjectionRC::unbind() {
  mProgramObject.unbind();
}



void QMapMercatorProjectionRC::initShaderProgram() {

/******************************************************************************
PURPOSE: vertexShader - Sets the color/alpha/texture of a vertex.
INPUTS:  uniform float imageHeight      Viewport height in pixels.
         uniform float numberOfTiles
         uniform float mytilesize
         uniform vec2 midp
         uniform vec2 screenMiddle
         uniform int numValues          Number of actual colormap threshholds.
         uniform float values[64]       Colormap threshhold lutValues.
         gl_Vertex.xy                   Longitude, Latitude  of vertex.
         gl_Vertex.w                    Data value of vertex.
         gl_ModelViewProjectionMatrix
OUTPUTS: varying float dataValue        Data value of vertex.
         varying vec4 facecolor         Color of vertex.
         gl_TexCoord[0].x               Normalized colormap index of dataValue.
         gl_TexCoord[0].y               0.0 = ok, -1.0 = missing data value.
         gl_FrontColor                  The vertex color/alpha.
         gl_BackColor                   The vertex color/alpha.
NOTES:   This vertex shader (vertex processor) program is executed on each
         vertex before the fragmentShader program.
         It needs to be very efficient!
******************************************************************************/

  const char* const vertexShader =
  GLSL_PROGRAM(
    uniform float imageHeight;
    uniform float numberOfTiles;
    uniform float mytilesize;
    uniform vec2 midp;
    uniform vec2 screenMiddle;
    uniform int numValues;
    uniform float values[64];
    varying float dataValue;
    varying vec4 facecolor;

    const float M_PI = 3.14159265359;

    vec2 coordinateToDisplay(in vec2 coordinate) {
      float imageLength = numberOfTiles * mytilesize;
      float x = (coordinate.x + 180.0) * (1.0 / 360.0) * imageLength;
      float y = (1.0 - (log(tan(M_PI * 0.25 + radians(coordinate.y) * 0.5)) / M_PI))
                * 0.5 * imageLength;
      return vec2(x, y);
    }

    vec2 worldCoordinateToScreen(in vec2 uv) {
      vec2 sp = coordinateToDisplay(uv);
      return sp + screenMiddle - midp;
    }

    void main() {
      vec4 wpos;
      wpos.xy = worldCoordinateToScreen(gl_Vertex.xy);
      wpos.y = imageHeight - wpos.y;
      wpos.z = 0.5;
      wpos.w = 1.0;
      gl_Position = gl_ModelViewProjectionMatrix * wpos;

      // Compute t = normalized colormap index:

      dataValue = gl_Vertex.w;
      int index = 0;

      while ( index < numValues && values[ index ] < dataValue ) {
        ++index;
      }

      float t = index <= 1 ? 0.0 : float(index - 1) / numValues;

      gl_TexCoord[0].x = t + 1e-20; // Index into colormap in range [1e-20, 1.0]
      gl_TexCoord[0].y = 0.0;

      if ( ! ( dataValue > MISSING_VALUE ) ) {
        gl_TexCoord[0].y = -1.0; // Flag indicating missing in fragmentShader.
      }

      gl_FrontColor = gl_Color;
      gl_BackColor  = gl_Color;
      facecolor     = gl_Color;
    }
  );

  mProgramObject.attachVertexShaderString(vertexShader);
  mProgramObject.attachFragmentShaderString(fragmentShader);
  mProgramObject.link();
  CHECK_GL_ERRORS2;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*!
 \class LambertConformalConicProjectionRC
 \brief This class implements the Lambert Conformal Conic projection algorithm in 
 shader program as the rendering context for online maps. 
 
 The paramters in projection are accquired from the MarbleWidget. 
 */
LambertConformalConicProjectionRC::LambertConformalConicProjectionRC()
  : RenderingContextRSIGBase(0) {
}



LambertConformalConicProjectionRC::~LambertConformalConicProjectionRC() {
}



void LambertConformalConicProjectionRC::bind() {
  mProgramObject.bind();
  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_LOGIC_OP);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  CHECK_GL_ERRORS2;
}



void LambertConformalConicProjectionRC::setParameters( GLuint /* paletteid */,
                                                       GLfloat phi1,
                                                       GLfloat phi2,
                                                       const GLfloat centerLonLat[2],
                                                       GLfloat rad2Pixel,
                                                       const GLfloat screenSize[2],
                                                       const std::vector<float>& lutValues,
                                                       const std::vector<float>& paletteR,
                                                       const std::vector<float>& paletteG,
                                                       const std::vector<float>& paletteB,
                                                       float texWeight ) {

  const GLfloat* glLutValues = (const GLfloat*) &lutValues[ 0 ];
  const GLint numValues = lutValues.size();

  const GLfloat* glPaletteValuesR = (const GLfloat*) &paletteR[ 0 ];
  const GLfloat* glPaletteValuesG = (const GLfloat*) &paletteG[ 0 ];
  const GLfloat* glPaletteValuesB = (const GLfloat*) &paletteB[ 0 ];
  const GLint    paletteSize = paletteR.size();

  //bind texture
  //mProgramObject.bindTexture1D("texPalette", paletteid, 0);

  //bind uniforms
  //mProgramObject.setUniform("minmaxVal", minmaxVal, 2);
  mProgramObject.setUniform("phi1", &phi1, 1);
  mProgramObject.setUniform("phi2", &phi2, 1);
  mProgramObject.setUniform("centerLonLat", centerLonLat, 2);
  mProgramObject.setUniform("rad2Pixel", &rad2Pixel, 1);
  mProgramObject.setUniform("screenSize", screenSize, 2);
  mProgramObject.setUniform("texWeight", &texWeight, 1);
  mProgramObject.setUniform("values", glLutValues, numValues);
  mProgramObject.setUniform("paletteR", glPaletteValuesR, paletteSize );
  mProgramObject.setUniform("paletteG", glPaletteValuesG, paletteSize );
  mProgramObject.setUniform("paletteB", glPaletteValuesB, paletteSize );
  mProgramObject.setUniform("numValues", &numValues, 1);
  setIsPointUniformParameter( 0 );
  CHECK_GL_ERRORS2;
}



void LambertConformalConicProjectionRC::unbind() {
  mProgramObject.unbind();
}



void LambertConformalConicProjectionRC::initShaderProgram() {

/******************************************************************************
PURPOSE: vertexShader - Sets the color/alpha/texture of a vertex.
INPUTS:  uniform float phi1             Radians of lower secant latitude.
         uniform float phi2             Radians of upper secant latitude.
         uniform float rad2pixel        Radians per pixel.
         uniform vec2 centerLonLat      Longitude,latitude that projects to (0,0)
         uniform vec2 screenSize        Viewport width,height in pixels.
         uniform int numValues          Number of actual colormap threshholds.
         uniform float values[64]       Colormap threshhold lutValues.
         gl_Vertex.xy                   Longitude, Latitude of vertex.
         gl_Vertex.w                    Data value of vertex.
         gl_ModelViewProjectionMatrix
OUTPUTS: varying float dataValue        Data value of vertex.
         varying vec4 facecolor         Color of vertex.
         gl_TexCoord[0].x               Normalized colormap index of dataValue.
         gl_TexCoord[0].y               0.0 = ok, -1.0 = missing data value.
         gl_FrontColor                  The vertex color/alpha.
         gl_BackColor                   The vertex color/alpha.
NOTES:   This vertex shader (vertex processor) program is executed on each
         vertex before the fragmentShader program.
         It needs to be very efficient!
         http://en.wikipedia.org/wiki/Lambert_conformal_conic_projection
******************************************************************************/

  const char* const vertexShader =
  GLSL_PROGRAM(
    const float M_PI = 3.14159265359;
    const float M_PI_2 = M_PI * 0.5;
    const float M_PI_4 = M_PI * 0.25;

    // GLSL BUG: Workaround missing built-in math functions:
    float cot(float x) { return  tan(M_PI_2 - x); }
    float sec(float x) { return 1.0 / cos(x); }

    void toScreenCoordinate( in  float lambda,  // Longitude (radians).
                             in  float phi,     // Latitude (radians).
                             in  float lambda0, // Center longitude (radians).
                             in  float phi0,    // Center latitude (radians).
                             in  float phi1,    // Lower secant latitude (radians).
                             in  float phi2,    // Upper secant latitude (radians).
                             out float x,       // Projected x (radians).
                             out float y ) {    // Projected y (radians).

      // Formula from code/src/libs/marble/src/lib/Projections/LambertConformalConicProjection.cpp

      float temp_phi  = M_PI_4 + 0.5 * phi;
      float temp_phi0 = M_PI_4 + 0.5 * phi0;
      float temp_phi1 = M_PI_4 + 0.5 * phi1;
      float temp_phi2 = M_PI_4 + 0.5 * phi2;
      float tan_temp_phi1 = tan(temp_phi1);
      float cos_phi1 = cos(phi1);
      float n = log(cos_phi1 * sec(phi2)) / log(tan(temp_phi2) / tan_temp_phi1);
      float F = cos_phi1 * pow(tan_temp_phi1, n) / n;
      float rho0 = F * pow(cot(temp_phi0), n);
      float rho  = F * pow(cot(temp_phi), n);
      float t = n * (lambda - lambda0);
      x = rho * sin(t);
      y = rho0 - rho * cos(t);
    }

    uniform float phi1;
    uniform float phi2;
    uniform float rad2Pixel;
    uniform vec2 centerLonLat;
    uniform vec2 screenSize;
    uniform int numValues;
    uniform float values[64];
    varying float dataValue;
    varying vec4 facecolor;

    vec2 worldCoordinateToScreen(in vec2 uv) {
      vec2 sp;
      toScreenCoordinate(uv.x, uv.y, centerLonLat.x, centerLonLat.y, phi1, phi2,
                         sp.x, sp.y);
      return screenSize * 0.5 + rad2Pixel * sp;
    }

    void main() {
      vec4 wpos;
      wpos.xy = worldCoordinateToScreen( radians(gl_Vertex.xy) );
      wpos.z = 0.5;
      wpos.w = 1.0;
      gl_Position = gl_ModelViewProjectionMatrix * wpos;

      // Compute t = normalized colormap index:

      dataValue = gl_Vertex.w;
      int index = 0;

      while ( index < numValues && values[ index ] < dataValue ) {
        ++index;
      }

      float t = index <= 1 ? 0.0 : float(index - 1) / numValues;

      gl_TexCoord[0].x = t + 1e-20; // Index into colormap in range [1e-20, 1.0]
      gl_TexCoord[0].y = 0.0;

      if ( ! ( dataValue > MISSING_VALUE ) ) {
        gl_TexCoord[0].y = -1.0; // Flag indicating missing in fragmentShader.
      }

      gl_FrontColor = gl_Color;
      gl_BackColor  = gl_Color;
      facecolor     = gl_Color;
    }
  );

  mProgramObject.attachVertexShaderString(vertexShader);
  mProgramObject.attachFragmentShaderString(fragmentShader);
  mProgramObject.link();
  CHECK_GL_ERRORS2;
}




//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
StereographicProjectionRC::StereographicProjectionRC()
  : RenderingContextRSIGBase(0) {
}



StereographicProjectionRC::~StereographicProjectionRC() {
}



void StereographicProjectionRC::bind() {
  mProgramObject.bind();
  glDisable(GL_CULL_FACE);
  glDisable(GL_COLOR_LOGIC_OP);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}



void StereographicProjectionRC::setParameters( GLuint /* paletteid */,
                                               const GLfloat centerLonLat[2],
                                               GLfloat rad2Pixel,
                                               const GLfloat screenSize[2],
                                               const std::vector<float>& lutValues,
                                               const std::vector<float>& paletteR,
                                               const std::vector<float>& paletteG,
                                               const std::vector<float>& paletteB,
                                               float texWeight ) {
  const GLfloat* glLutValues = (const GLfloat*) &lutValues[ 0 ];
  const GLint numValues = lutValues.size();

  const GLfloat* glPaletteValuesR = (const GLfloat*) &paletteR[ 0 ];
  const GLfloat* glPaletteValuesG = (const GLfloat*) &paletteG[ 0 ];
  const GLfloat* glPaletteValuesB = (const GLfloat*) &paletteB[ 0 ];
  const GLint    paletteSize = paletteR.size();
  
  //bind texture
  //mProgramObject.bindTexture1D("texPalette", paletteid, 0);

  //bind uniforms
  mProgramObject.setUniform("centerLonLat", centerLonLat, 2);
  mProgramObject.setUniform("rad2Pixel", &rad2Pixel, 1);
  mProgramObject.setUniform("screenSize", screenSize, 2);
  mProgramObject.setUniform("texWeight", &texWeight, 1);
  mProgramObject.setUniform("values", glLutValues, numValues);
  mProgramObject.setUniform("paletteR", glPaletteValuesR, paletteSize );
  mProgramObject.setUniform("paletteG", glPaletteValuesG, paletteSize );
  mProgramObject.setUniform("paletteB", glPaletteValuesB, paletteSize );
  mProgramObject.setUniform("numValues", &numValues, 1);
  setIsPointUniformParameter( 0 );
  CHECK_GL_ERRORS2;
}



void StereographicProjectionRC::unbind() {
  mProgramObject.unbind();
}


void StereographicProjectionRC::initShaderProgram() {

/******************************************************************************
PURPOSE: vertexShader - Sets the color/alpha/texture of a vertex.
INPUTS:  uniform float rad2pixel        Radians per pixel.
         uniform vec2 centerLonLat      Longitude,latitude that projects to (0,0)
         uniform vec2 screenSize        Viewport width,height in pixels.
         uniform int numValues          Number of actual colormap threshholds.
         uniform float values[64]       Colormap threshhold lutValues.
         gl_Vertex.xy                   Longitude, Latitude  of vertex.
         gl_Vertex.w                    Data value of vertex.
         gl_ModelViewProjectionMatrix
OUTPUTS: varying float dataValue        Data value of vertex.
         varying vec4 facecolor         Color of vertex.
         gl_TexCoord[0].x               Normalized colormap index of dataValue.
         gl_TexCoord[0].y               0.0 = ok, -1.0 = missing data value.
         gl_FrontColor                  The vertex color/alpha.
         gl_BackColor                   The vertex color/alpha.
NOTES:   This vertex shader (vertex processor) program is executed on each
         vertex before the fragmentShader program.
         It needs to be very efficient!
         http://en.wikipedia.org/wiki/Stereographic_projection
******************************************************************************/

  const char* const vertexShader =
  GLSL_PROGRAM(

    void toScreenCoordinate( in float lambda,  //longitude, input
                             in float phi,     //latitude, input
                             in float lambda0, //reference longitude
                             in float phi1,    //reference latitudes
                             out float x, out float y ) { //output screen x, y (unit not known yet)
      float cos_phi1 = cos(phi1);
      float sin_phi1 = sin(phi1);
      float R = 1.0;
      float t = lambda - lambda0;
      float cos_t = cos(t);
      float cos_phi = cos(phi);
      float sin_phi = sin(phi);
      float a = (1.0 + sin_phi1 * sin_phi + cos_phi1 * cos_phi * cos_t);
      const float epsilon = 1e-10;

      // If a is too close to zero make it epsilon to avoid division by zero:

      if ( -epsilon < a && a < epsilon ) {

        if ( a > 0.0 ) {
          a = epsilon;
        } else {
          a = -epsilon;
        }
      }

      float k = 2.0 * R / a;
      x = k * cos_phi * sin(t);
      y = k * (cos_phi1 * sin_phi - sin_phi1 * cos_phi * cos_t);
    }

    uniform float rad2Pixel;
    uniform vec2 centerLonLat;
    uniform vec2 screenSize;
    uniform int numValues;
    uniform float values[64];
    varying float dataValue;
    varying vec4 facecolor;

    vec2 worldCoordinateToScreen(in vec2 uv) {
      vec2 sp;
      toScreenCoordinate(uv.x, uv.y, centerLonLat.x, centerLonLat.y, sp.x, sp.y);
      return screenSize * 0.5 + rad2Pixel * sp;
    }

    void main() {
      vec4 wpos;
      wpos.xy = worldCoordinateToScreen(radians(gl_Vertex.xy));
      wpos.z = 0.5;
      wpos.w = 1.0;
      gl_Position = gl_ModelViewProjectionMatrix * wpos;

      // Compute t = normalized colormap index:

      dataValue = gl_Vertex.w;
      int index = 0;

      while ( index < numValues && values[ index ] < dataValue ) {
        ++index;
      }

      float t = index <= 1 ? 0.0 : float(index - 1) / numValues;

      gl_TexCoord[0].x = t + 1e-20; // Index into colormap in range [1e-20, 1.0]
      gl_TexCoord[0].y = 0.0;

      if ( ! ( dataValue > MISSING_VALUE ) ) {
        gl_TexCoord[0].y = -1.0; // Flag indicating missing in fragmentShader.
      }

      gl_FrontColor = gl_Color;
      gl_BackColor  = gl_Color;
      facecolor     = gl_Color;
    }
  );

  mProgramObject.attachVertexShaderString(vertexShader);
  mProgramObject.attachFragmentShaderString(fragmentShader);
  mProgramObject.link();
  CHECK_GL_ERRORS2;
}



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// Shared global variables, used in RSIGScene.cpp
// NOTE: these global objects's constructors are called before main().

MarbleUVProjectionRC marbleUVProjectionRC;
MarbleSphericalProjectionRC marbleSphericalProjectionRC;
MarbleMercatorProjectionRC marbleMercatorProjectionRC;
QMapMercatorProjectionRC qmapMercatorProjectionRC;
LambertConformalConicProjectionRC lambertConformalProjectionRC;
StereographicProjectionRC stereographicProjectionRC;

void initRenderingContexts() {

  try {
    marbleUVProjectionRC.initShaderProgram();
    CHECK_GL_ERRORS2;

    marbleMercatorProjectionRC.initShaderProgram();
    CHECK_GL_ERRORS2;

    marbleSphericalProjectionRC.initShaderProgram();
    CHECK_GL_ERRORS2;

    qmapMercatorProjectionRC.initShaderProgram();
    CHECK_GL_ERRORS2;

    lambertConformalProjectionRC.initShaderProgram();
    CHECK_GL_ERRORS2;

    stereographicProjectionRC.initShaderProgram();
    CHECK_GL_ERRORS2;

  } catch ( ... ) {
    std::cerr<<"Run time error in initializing Rendering Context!\n";
  }
}



