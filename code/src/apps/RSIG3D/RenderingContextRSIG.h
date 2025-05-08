/******************************************************************************
PURPOSE: RenderingContextRSIG.h - defines serveral RenderingContext classes for
         RSIG3D's rendering algorithms

NOTES:   depends on OpenGL, GLEW

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef INC_RENDERCONTEXTRSIG_H
#define INC_RENDERCONTEXTRSIG_H

#include "RenderingContextRSIGBase.h"
#include <QString>

class MarbleUVProjectionRC: public RenderingContextRSIGBase {
public:
  MarbleUVProjectionRC();
  virtual ~MarbleUVProjectionRC();
  
  virtual void bind();
  virtual void unbind();
  virtual void initShaderProgram();

  virtual void setParameters( GLuint paletteid,
                              const GLfloat uvcenter[2],
                              const GLfloat viewsize[2],
                              float rad2pixel,
                              const std::vector<float>& lutValues,
                              const std::vector<float>& paletteR,
                              const std::vector<float>& paletteG,
                              const std::vector<float>& paletteB,
                              float texweight = 1.0 );

  virtual void setMaplineFlag( GLuint paletteid, int isMaplines = 0,
                               QString maplineColor = "Black" );

  virtual void bind( const int,
                     const GLfloat*,
                     const GLuint* ) {}

  virtual void initRenderTarget(const int, const int) {};
};


class MarbleSphericalProjectionRC: public RenderingContextRSIGBase {
public:
  MarbleSphericalProjectionRC();
  virtual ~MarbleSphericalProjectionRC();
  virtual void bind();
  virtual void unbind();
  virtual void initShaderProgram();

  virtual void setParameters( GLuint paletteID,
                              const GLfloat axis[4],
                              const GLfloat viewSize[2],
                              float radius,
                              const std::vector<float>& lutValues,
                              const std::vector<float>& paletteR,
                              const std::vector<float>& paletteG,
                              const std::vector<float>& paletteB,
                              float texweight = 1.0 );

  virtual void setMaplineFlag( GLuint paletteid, int isMaplines = 0,
                               QString maplineColor = "Black" );

  virtual void bind( const int,
                     const GLfloat*,
                     const GLuint* ) {}

  virtual void initRenderTarget(const int, const int) {};
};


class MarbleMercatorProjectionRC: public RenderingContextRSIGBase {
public:
  MarbleMercatorProjectionRC();
  virtual ~MarbleMercatorProjectionRC();
  virtual void bind();
  virtual void unbind();
  virtual void initShaderProgram();

  virtual void setParameters( GLuint paletteid,
                              const GLfloat uvcenter[2],
                              const GLfloat viewsize[2],
                              float rad2pixel,
                              const std::vector<float>& lutValues,
                              const std::vector<float>& paletteR,
                              const std::vector<float>& paletteG,
                              const std::vector<float>& paletteB,
                              float texweight = 1.0 );

  virtual void setMaplineFlag( GLuint paletteid, int isMaplines = 0,
                               QString maplineColor = "Black" );

  virtual void bind( const int,
                     const GLfloat*,
                     const GLuint* ) {}

  virtual void initRenderTarget(const int, const int) {};
};


class QMapMercatorProjectionRC: public RenderingContextRSIGBase {
public:
  QMapMercatorProjectionRC();
  virtual ~QMapMercatorProjectionRC();
  virtual void bind();
  virtual void unbind();
  virtual void initShaderProgram();

  virtual void setParameters( GLuint paletteid,
                              GLfloat imageHeight,
                              GLfloat numberOfTiles,
                              GLfloat mytilesize,
                              const GLfloat midp[2],
                              const GLfloat screen_middle[2],
                              const std::vector<float>& lutValues,
                              const std::vector<float>& paletteR,
                              const std::vector<float>& paletteG,
                              const std::vector<float>& paletteB,
                              float texweight = 1.0 );

  virtual void setMaplineFlag( GLuint paletteid, int isMaplines = 0,
                               QString maplineColor = "Black" );

  virtual void bind( const int,
                     const GLfloat*,
                     const GLuint* ) {}

  virtual void initRenderTarget(const int, const int) {};
};


class LambertConformalConicProjectionRC: public RenderingContextRSIGBase {
public:
  LambertConformalConicProjectionRC();
  virtual ~LambertConformalConicProjectionRC();
  virtual void bind();
  virtual void unbind();
  virtual void initShaderProgram();

  virtual void setParameters( GLuint paletteid,
                              GLfloat phi1,
                              GLfloat phi2,
                              const GLfloat centerLonLat[2],
                              GLfloat radius,
                              const GLfloat screensize[2],
                              const std::vector<float>& lutValues,
                              const std::vector<float>& paletteR,
                              const std::vector<float>& paletteG,
                              const std::vector<float>& paletteB,
                              float texweight = 1.0 );

  virtual void setMaplineFlag( GLuint paletteid, int isMaplines = 0,
                               QString maplineColor = "Black" );

  virtual void bind( const int,
                     const GLfloat*,
                     const GLuint* ) {}

  virtual void initRenderTarget(const int, const int) {};
};


class StereographicProjectionRC: public RenderingContextRSIGBase {
public:
  StereographicProjectionRC();
  virtual ~StereographicProjectionRC();
  virtual void bind();
  virtual void unbind();
  virtual void initShaderProgram();

  virtual void setParameters( GLuint paletteid,
                              const GLfloat centerLonLat[2],
                              GLfloat radius,
                              const GLfloat screensize[2],
                              const std::vector<float>& lutValues,
                              const std::vector<float>& paletteR,
                              const std::vector<float>& paletteG,
                              const std::vector<float>& paletteB,
                              float texweight = 1.0 );

  virtual void setMaplineFlag( GLuint paletteid, int isMaplines = 0,
                               QString maplineColor = "Black" );

  virtual void bind( const int,
                     const GLfloat*,
                     const GLuint* ) {}

  virtual void initRenderTarget(const int, const int) {};
};



// The global function and global variables are in RenderingContextRSIG.cpp:

extern void initRenderingContexts();
extern MarbleUVProjectionRC marbleUVProjectionRC;
extern MarbleSphericalProjectionRC marbleSphericalProjectionRC;
extern MarbleMercatorProjectionRC marbleMercatorProjectionRC;
extern QMapMercatorProjectionRC qmapMercatorProjectionRC;
extern LambertConformalConicProjectionRC lambertConformalProjectionRC;
extern StereographicProjectionRC stereographicProjectionRC;

#endif //INC_RENDERCONTEXTRSIG_H
