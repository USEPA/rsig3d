/******************************************************************************
PURPOSE: PointRenderer.cpp - Implements a point dataset rendering class.

NOTES: for rendering point data sets using OpenGL APIs

HISTORY: 2013-04-11 zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
******************************************************************************/

#include <assert.h>  // For assert().
#include <stdio.h>   // For snprintf().
#include <string.h>  // For strrchr(), strcmp().
#include <math.h>    // For fabs().

#include <exception> // for std::exception
#include <algorithm> // for std::find
#include <iterator>  // for std::begin, std::end
#include <vector>    // std::vector

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h> /* For glut*. */
#else
#include <GL/glut.h> /* For gl*. */
#endif

#include <QObject>
#include <QDebug>
#include <QPainter> 
#include <QGLWidget>
#include <QRectF>
#include <QString>

#include <Dataset.h>        // For emvl::Dataset.
#include <DatasetManager.h> // For emvl::DatasetManager.
#include <ProfileDataset.h> // For emvl::ProfileDataset.
#include <Bounds.h>         // For emvl::Bounds.
#include <Timestamp.h>      // For emvl::Timestamp.

#include "vector4f.h"
#include "vector4i.h"
#include "RenderingContextRSIG.h"
#include "PointRenderer.h"
#include "GeoDataObject.h"
#include "RSIGGlobal.h"
#include "MainWin.h"

#include "LookupTableWidget.h"
#include "ControlDockWidget.h"   //UI 
#include "VisDataObject.h"
#include "VisDataList.h"
#include "VisOptions.h"
#include "RSIGScene.h"
#include "vector3f.h"
#include "vector2f.h"

#include "RegridParmsDialog.h"
#include "RegridParmsData.h"
#include "FilterSettingDialog.h"
#include "FilterSettingData.h"
#include "GetData.h"
#include "CeilometerCurtainArray.h"



static LookupTableWidget* myLut;
static bool hasElevation;
static bool flagPopupPlot;
static bool flagPopup3D; // whether we are rendering to the 3D view
static bool flagCalipso; // whether or not calipso is a selected variable. Needed for neubrew plot positioning
static QString descrip;
static Vector2f theMinmax;
static bool traceFunctions = false; // print every time a function is entered




static const int IMAGE_WIDTH = 64; // Of point glyphs.


// Helper to paint point glyph image:

static void paintGlyphImage( const int glyph,
                             const int c,
                             QImage* const image ) {
  QPainter painter( image );
  painter.setRenderHint( QPainter::Antialiasing, true );
  painter.setBrush( Qt::white );

  if ( glyph == 1 ) { // Circle:
    painter.drawEllipse( QPoint( c, c ), c, c );
  } else if ( glyph == 2 ) { // Triangle:
    static const QPointF points[3] = {
      QPointF(0, 0),
      QPointF(c, IMAGE_WIDTH - 1),
      QPointF(IMAGE_WIDTH - 1, 0),
    };

    painter.drawPolygon( points, sizeof points / sizeof *points );
  } else if ( glyph == 3 ) { // Plus:
    painter.fillRect(0, IMAGE_WIDTH/4, IMAGE_WIDTH, IMAGE_WIDTH/2, Qt::white);
    painter.fillRect(IMAGE_WIDTH/4, 0, IMAGE_WIDTH/2, IMAGE_WIDTH, Qt::white);
  }

  painter.end();
}



static int genPointSpriteTexture( int glyph, int flag3D ) {
  GLuint texid = 0;
  QImage image( IMAGE_WIDTH,IMAGE_WIDTH, QImage::Format_ARGB32 );
  image.fill( Qt::black );

  switch ( glyph ) {
  case 0: // Square:
    image.fill( Qt::white );
    break;
  case 1: // Circle:
    paintGlyphImage( glyph, IMAGE_WIDTH >> 1, &image );
    break;
  case 2: // Triangle:
    paintGlyphImage( glyph, IMAGE_WIDTH >> 1, &image );
    break;
  case 3: // Plus:
    paintGlyphImage( glyph, IMAGE_WIDTH >> 1, &image );
    break;
  case 4: // Star:
    {
      QImage tmp( ":/image/star_image.png" );
      image = tmp;
      const int imageWidth = image.width();
      const int imageHeight = image.height();

      for ( int j = 0; j < imageHeight; j++ ) {
        const int j1 = imageHeight - 1 - j;

        for ( int i = 0; i < imageWidth; i++ ) {
          const QRgb rgb = tmp.pixel(i, j);
          const int r = 255 - qRed(rgb);
          const int g = 255 - qGreen(rgb);
          const int b = 255 - qBlue(rgb);
          int opacity = 255;

          if (r==0 && g==0 && b==0) {
            opacity=0;
          }

          const QRgb rgb2 = QColor(r,g,b, opacity).rgba();
          image.setPixel(i, j1, rgb2);
        }
      }
    }
  break;
  case 5: // Pentagon:
    {
      QImage tmp( ":/image/pentagon.png" );
      const int imageWidth = image.width();
      const int imageHeight = image.height();

      for ( int j = 0; j < imageHeight; j++ ) {
        const int j1 = imageHeight - 1 - j;

        for ( int i = 0; i < imageWidth; i++ ) {
          const QRgb rgb = tmp.pixel(i, j);
          image.setPixel(i, j1, QColor(rgb).rgba());
        }
      }
    }
    break;
  default:
    break;
  }

  // set black pixels in texture to transparent
  // (only needed id 3D rendering)

  if (flag3D) {
    const int imageWidth = image.width();
    const int imageHeight = image.height();

    for ( int j = 0; j < imageHeight; j++ ) {
      for ( int i = 0; i < imageWidth; i++ ) {
        const QRgb rgb = image.pixel(i, j);
        const int r = 255 - qRed(rgb);
        const int g = 255 - qGreen(rgb);
        const int b = 255 - qBlue(rgb);

        if (r!=0 && g!=0 && b!=0) {
          const QRgb rgb2 = QColor(r,g,b, 0.0).rgba();
          image.setPixel(i, j, rgb2);
        }
      }
    }
  }

  image = QGLWidget::convertToGLFormat(image);
  const int w = image.width();
  const int h = image.height();
  unsigned char *prgb = image.bits();
  const int depth = image.depth();

  const GLuint target = GL_TEXTURE_2D;

  if (depth==32) {
    glGenTextures(1, &texid);
    glBindTexture(target, texid);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(target, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, prgb);
    glBindTexture(target, 0);
  } else if (depth==24) {
    glGenTextures(1, &texid);
    glBindTexture(target, texid);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(target, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, prgb);
    glBindTexture(target, 0);
  } else {
    std::cerr<<"Not supported texture!\n";
    return 0;
  }
  
  CHECK_GL_ERRORS;
  return texid;
}




//============================================================================
/*!
 \class PointRenderer
 
 \brief An example class for point dataset rendering.
 */

/*!
 Constructor.
 */
PointRenderer::PointRenderer(QObject* parent): VisDataObject(parent)
    ,mPointShape(0)
    ,mPointSize(9.0f)
    ,mLineWidth(1.0f)
    ,mPointColor(0, 0, 0)
    ,mShowNullSample(false)
    ,mUseOpenglPointSmooth(false) {
}



/*!
 Destructor.
 */
PointRenderer::~PointRenderer() {
}



static void
copyAndDrawPoints(const size_t count,
                  const size_t offset,
                  const size_t coordinatesCount,
                  const emvl::Point* const coordinates,
                  const double* const data,
                  Vector4f vertices[],
                  float threshold,
                  bool showThreshold,
                  bool showNullSample,
                  const QRectF& viewBounds) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  const size_t end = offset + count;
  int vertexCount = 0;

  for ( size_t index = offset; index < end; ++index ) {
    const float value = data[ index ];
    const bool flag = ( value > emvl::MISSING ) ^ showNullSample;
    bool flag2 = false;

    if (showThreshold) {
      flag2 = value >= threshold;
    } else {
      flag2 = true; // no thresholding
    }
    
    if ( flag && flag2 ) {
      const emvl::Point& point = coordinates[ index % coordinatesCount ];
      
      if ( viewBounds.contains( point.longitude, point.latitude ) ) {
        Vector4f& vector = vertices[ vertexCount++ ];
        vector.x = float( point.longitude );
        vector.y = float( point.latitude );
        vector.z = 0.0f;
        vector.w = value;

        // HACK for testing point data
        //if (point.longitude < -120)      { vector.w = 600.2; }
        //else if (point.longitude < -110) { vector.w = 450.2; }
        //else if (point.longitude < -100) { vector.w = 250.2; }
        //else if (point.longitude < -90)  { vector.w = 175.2; }
        //else if (point.longitude < -80)  { vector.w = 140.2; }
        //else                             { vector.w = -100.3; } 
        
      }
    }
  }
  
  if ( vertexCount > 0 ) {
    glVertexPointer( 4, GL_FLOAT, 0, &vertices[0].x );
    CHECK_GL_ERRORS; // Report any errors before shader programs execute.
    glDrawArrays( GL_POINTS, 0, vertexCount );
    CHECK_GL_ERRORS; // Report any errors in shader programs.
  }
}



static void
pagedBufferDraw(const size_t coordinatesCount,
                const emvl::Point* coordinates,
                const size_t dataCount,
                const double* data,
                const float threshold,
                bool showThreshold,
                bool showNullSample,
                const QRectF& viewBounds) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  const size_t BUFFSIZE = 4096;
  Vector4f vertices[BUFFSIZE*4];
  const size_t chunks = dataCount / BUFFSIZE;
  size_t offset = 0;

  for ( size_t chunk = 0; chunk < chunks; ++chunk, offset += BUFFSIZE ) {
    copyAndDrawPoints(BUFFSIZE, offset, coordinatesCount, coordinates, data,
          vertices, threshold, showThreshold, showNullSample, viewBounds);
  }

  if ( offset < dataCount ) {
    const size_t remainingCount = dataCount - offset;
    copyAndDrawPoints(remainingCount, offset, coordinatesCount, coordinates,
                      data,
                      vertices, threshold, showThreshold, showNullSample, viewBounds);
  }
}



void PRrenderBitmapString( float x, float y, float z, void* font, char* string) {

  if ( string ) {
    glRasterPos3f(x, y, z);

    for ( const char* c = string; *c != '\0'; c++ ) {
      glutBitmapCharacter( font, *c );
    }
  }
}



void createLinePlot(const size_t coordinatesCount,
                    const emvl::Point* coordinates,
                    const size_t /* dataCount */,
                    const double* data,
                    const std::string& dataName,
                    const std::string& note,
                    float /* mPointSize */,
                    float /* mLineWidth */,
                    const QColor& /* mPointColor */,
                    float alpha,
                    float /* threshold */,
                    bool /* showThreshold */,
                    int /* IDindex */,
                    int /* varIndex */,
                    int quad_sw_x, int quad_sw_y,
                    int quad_se_x, int /*quad_se_y */,
                    int /* quad_ne_x */, int /* quad_ne_y */,
                    int /* quad_nw_x */, int quad_nw_y) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }
  
  int xMargin = 25;
  int yMargin = 40;

  int xStart = quad_sw_x + xMargin;
  int xEnd   = quad_se_x - xMargin;

  int yStart = quad_sw_y - yMargin;
  int yEnd   = quad_nw_y + xMargin;

  double xMin = 0.0;
  double xMax = 0.0;
  double yMin = 0.0;
  double yMax = 0.0;
  const char* xTickFormat = 0;
  int nXticks = 0;
  int xLabelOffset = 0;
  char xlabelString[50] = "";
  char titleString[50] = "";

  const unsigned int nDays = RSIGStateSingleton::getInstance()->mDuration;
  double ceilometer_min_elev = VisOptionsSingleton::getInstance()->getCeilometerMinAltitude() * 1000.;
  double ceilometer_max_elev = VisOptionsSingleton::getInstance()->getCeilometerMaxAltitude() * 1000.;
  double neubrew_min_elev = VisOptionsSingleton::getInstance()->getNeubrewMinAltitude() * 1000.;
  double neubrew_max_elev = VisOptionsSingleton::getInstance()->getNeubrewMaxAltitude() * 1000.;

  //qDebug() << user_min_elev << user_max_elev << plotScrollerValue;

  if ( dataName.find( "ceilometer" ) == 0 ) {
    yMin = ceilometer_min_elev;        // ceilometer
    yMax = ceilometer_max_elev; 
    xMin = 0;
    xMax = nDays * 24;
    xTickFormat = "%2.0f";
    nXticks = 6; // evenly divisible into 24
    xLabelOffset = 5;
    memset( xlabelString, 0, sizeof xlabelString );
    strncpy( xlabelString, "Relative hours",
             sizeof xlabelString / sizeof *xlabelString - 1 );
    assert( myLut );
    const std::string lutTitle( myLut->title().toStdString() );
    snprintf( titleString, sizeof titleString / sizeof *titleString,
              "%s %s", note.c_str(), lutTitle.c_str() );
  } else {
    yMin = neubrew_min_elev;        // neubrew
    yMax = neubrew_max_elev; 
    xMin = theMinmax.x/1e12;
    xMax = theMinmax.y/1e12;
    xTickFormat = "%5.1f";
    nXticks = 5;
    xLabelOffset = 20;
    assert( myLut );
    const std::string lutTitle( myLut->title().toStdString() );
    snprintf( xlabelString, sizeof xlabelString / sizeof *xlabelString,
              "%s x 10^12", lutTitle.c_str() );
    snprintf( titleString, sizeof titleString / sizeof *titleString,
              "%s", note.c_str() );
  }

  // set up linear transform between data and screen space
  double xSlope     = (xStart-xEnd) / (xMin - xMax);
  double xIntercept = ((xStart+xEnd) - xSlope*(xMin+xMax)) / 2;
  double ySlope     = (yStart-yEnd) / (yMin - yMax);
  double yIntercept = ((yStart+yEnd) - ySlope*(yMin+yMax)) / 2;

  glColor4f(0.0, 0.0, 0.0, alpha);
  glLineWidth(1.0);

  // draw X-axis
  glBegin(GL_LINES);
  glVertex2f(xStart, yStart);
  glVertex2f(xEnd, yStart);
  glEnd();

  // X-axis label
  PRrenderBitmapString(xStart, yStart+30, 0, GLUT_BITMAP_HELVETICA_10, xlabelString);
  PRrenderBitmapString(xStart+35, yEnd-5, 0, GLUT_BITMAP_HELVETICA_10, titleString);

  // draw Y-axis
  glBegin(GL_LINES);
  glVertex2f(xStart, yStart);
  glVertex2f(xStart, yEnd);
  glEnd();

  // Y-axis label
  char ylabelString[50];
  sprintf(ylabelString, "%s", "Alt [km]");
  PRrenderBitmapString( quad_sw_x+10, yEnd-10, 0, GLUT_BITMAP_HELVETICA_10, ylabelString);

  // draw X ticks
  int xTickLen = 10;
  float xTickSize = (xMax - xMin) / nXticks; // data space

  for (int xTickInd=0; xTickInd<=nXticks; xTickInd++) {
    float thisTick = xTickInd*xTickSize;
    int xTickScreenX = xSlope*thisTick + xIntercept;
    glBegin(GL_LINES);
    glVertex2f(xTickScreenX, yStart-xTickLen);
    glVertex2f(xTickScreenX, yStart);
    glEnd();
    // label tick
    char tickString[50];
    sprintf(tickString, xTickFormat, thisTick);
    PRrenderBitmapString(xTickScreenX-xLabelOffset, yStart+12, 0, GLUT_BITMAP_HELVETICA_10, tickString);
  }

  // draw Y ticks
  int nYticks = 5;
  float yTickSize = (yMax - yMin) / nYticks; // data space
  char yTickFormat[10];

  if (yTickSize/1000. > 2) {
    sprintf(yTickFormat, "%s", "%3.0f");
  } else {
    sprintf(yTickFormat, "%s", "%3.1f");
  }

  for (int yTickInd=0; yTickInd<=nYticks; yTickInd++) {
    float thisTick = yMin + yTickInd*yTickSize;
    int yTickScreenY = ySlope*thisTick + yIntercept;
    glBegin(GL_LINES);
    glVertex2f(xStart, yTickScreenY);
    glVertex2f(xStart+xTickLen, yTickScreenY);
    glEnd();
    // label tick
    char tickString[50];
    sprintf(tickString, yTickFormat, thisTick/1000);
    PRrenderBitmapString(xStart-25, yTickScreenY, 0, GLUT_BITMAP_HELVETICA_10, tickString);
  }
    
  // draw data
  RSIGScene& rsigScene = RSIGScene::getInstance();
  const bool isCeilometer = rsigScene.ceilometerCCADrawFlag && !rsigScene.ceilometerCCADrawn && dataName.find( "ceilometer" ) == 0;

  if ( ! isCeilometer ) { // neubrew
    glPointSize(6.0);

    for ( int index =  coordinatesCount-1; index >= 0; index-- ) {
      const emvl::Point& point = coordinates[ index ];
      const double thisData = data[ index ];
      assert( myLut );
      //const Vector4f thisColor = myLut->colorByValue(thisData);
      float my_elevation = point.elevation;
   
      if ( (my_elevation >= neubrew_min_elev) && (my_elevation <= neubrew_max_elev) ) {
        // compute X screen location for datapoint
        const int screenX0 = xSlope*thisData/1e12 + xIntercept;
        const int screenY0 = ySlope*my_elevation + yIntercept;

        if (screenX0 > xStart &&
            screenX0 < xEnd   &&
            screenY0 < yStart &&
            screenY0 > yEnd) {
          glBegin(GL_POINTS);
          glVertex2f(screenX0, screenY0);
          glEnd();
        }
      }
    }
  }
}



void PointRenderer::drawCCAPopup(QString popupType) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  RSIGScene& rsigScene = RSIGScene::getInstance();
  VisDataList* vislist = VisDataListSingleton::getInstance();

  if ( (popupType == "ceilometer" && rsigScene.ceilometerCCADrawFlag == false) ||
       (popupType == "pandora"    && rsigScene.pandoraCCADrawFlag    == false)) {
    return;
  }
  

  float aspect_ratio = 1.4;
  int yPopupSize = 0;
  int xPopupSize = 0;

  if (popupType == "ceilometer") {
    yPopupSize = vOptions->getCeilometerSize();
  } else if (popupType == "pandora") {
    yPopupSize = vOptions->getPandoraSize();
  }

  xPopupSize = yPopupSize * aspect_ratio;

  int yOffset = 0; // set below

  if (flagPopup3D) { // 3D mode
    yOffset = 10;
  } else if (!flagCalipso) { // 2D mode, but without calipso
    yOffset = -80; // 10;
  } else { // 2D mode with calipso
    yOffset = -215;
  }

  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  
  glGetIntegerv(GL_VIEWPORT, viewport);   
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);

  GLdouble xWinLoc, yWinLoc, zWinLoc;

  // push the matrix to prepare for doing 2D overplot
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  // set up 2D rendering
  float orig_linewidth;
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);

  glDisable(GL_TEXTURE_2D);
  glOrtho(0.0f, rsigScene.sceneRect().width(), rsigScene.sceneRect().height(), 0.0f, 0.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glGetFloatv(GL_LINE_WIDTH, &orig_linewidth);

  int quad_sw_y;
  int quad_se_y;
  int quad_ne_y;
  int quad_nw_y;
  int quad_sw_x;
  int quad_se_x;
  int quad_ne_x;
  int quad_nw_x;
  int myCounter = 0;

  const RSIGState* const rsigState = RSIGStateSingleton::getInstance();
  const unsigned int timestep = rsigState->mCounter;
  char xlabelString[80];
  char titleString[80];
  char title1String[40]; // for location name
  char title2String[40]; // for variable name
  sprintf(xlabelString, "%s", "Relative hours");

  double xSlope;
  double xIntercept;
  double ySlope;
  double yIntercept;
  double xMin;
  double xMax;
  double yMin;
  double yMax;

  // Get info an all loaded unique ceilometers or pandoras in DatasetManager:
  
  //std::vector<size_t> ceilometerDatasetIndices;
  //std::vector<std::string> ceilometerDatasetVariables;
  //std::vector<std::string> ceilometerDatasetUnits;
  //std::vector<long long> ceilometerDatasetIds;
  //std::vector<double> ceilometerDatasetLongitudes;
  //std::vector<double> ceilometerDatasetLatitudes;
  //std::vector<std::string> ceilometerDatasetCeilometerNames;

  std::vector<size_t>      profileDatasetIndices;
  std::vector<std::string> profileDatasetVariables;
  std::vector<std::string> profileDatasetUnits;
  std::vector<long long>   profileDatasetIds;
  std::vector<double>      profileDatasetLongitudes;
  std::vector<double>      profileDatasetLatitudes;
  std::vector<std::string> profileDatasetNames;
  
  
  cca::getCeilometerInfo( profileDatasetIndices,
                          profileDatasetVariables,
                          profileDatasetUnits,
                          profileDatasetIds,
                          profileDatasetLongitudes,
                          profileDatasetLatitudes,
                          profileDatasetNames );

  const size_t plotCount = profileDatasetIndices.size(); // Includes MLH if ceilometer.
  int plotScrollerValue = 0;

  if (popupType == "ceilometer") {
    plotScrollerValue = VisOptionsSingleton::getInstance()->getCeilometerPlotScrollerValue();
  } else if (popupType == "pandora") {
    plotScrollerValue = VisOptionsSingleton::getInstance()->getPandoraPlotScrollerValue();
  }

  const int scrollerOffset =
    plotScrollerValue / 100.0 * ( ( plotCount * yPopupSize ) - yOffset );
  yOffset = yOffset + scrollerOffset;

  const emvl::DatasetManager& datasetManager = *getDatasetManager();
  const emvl::Timestamp globalStartTime( rsigState->getStartingTimestamp() );
  const emvl::Timestamp globalEndTime( rsigState->getEndingTimestamp() );

  for ( size_t plotIndex = 0; plotIndex < plotCount; ++plotIndex ) {
    const size_t       profileDatasetIndex     = profileDatasetIndices[ plotIndex ];
    const std::string& profileDatasetVariable  = profileDatasetVariables[ plotIndex ];
    const std::string& profileDatasetUnit      = profileDatasetUnits[ plotIndex ];
    const double       profileDatasetLongitude = profileDatasetLongitudes[ plotIndex ];
    const double       profileDatasetLatitude  = profileDatasetLatitudes[ plotIndex ];
    const std::string& profileDatasetName      = profileDatasetNames[ plotIndex ];

    // Project ceilometer/pandora location (ground point) to window point:

    if ( flagPopup3D ) {
      gluProject( profileDatasetLongitude, profileDatasetLatitude, 0.0,
                  modelview, projection, viewport, &xWinLoc, &yWinLoc, &zWinLoc);
    } else { // 2D
      const QPointF groundPoint( profileDatasetLongitude,
                                 profileDatasetLatitude );
      const QPointF screenPoint( rsigScene.toScreenPointF( groundPoint ) );
      xWinLoc = screenPoint.x();
      yWinLoc = rsigScene.sceneRect().height() - screenPoint.y();
      zWinLoc = 0.5;
    }

    const emvl::Dataset& dataset( datasetManager[ profileDatasetIndex ] );
    const VisDataObject* const visDataObject = rsigScene.getVisDataObjectForDataset( &dataset );

    /*
     * Datasets such as implicit ceilometer.mixing_layer_height have no
     * associated/wrapper geo/visDataObject and so should not have a curtain.
     * But if the user manually selected ceilometer.mixing_layer_height dataset
     * then there would be a geo/visDataObject for the dataset and it should be
     * drawn as a curtain if visDataObject->isVisible().
     */

    const bool isVisible = visDataObject ? visDataObject->isVisible() : false;

    if ( isVisible ) {
      const float alpha = visDataObject->alpha();
      quad_sw_y = (myCounter +1)*yPopupSize - yOffset;
      quad_se_y = (myCounter +1)*yPopupSize - yOffset;
      quad_ne_y = myCounter*yPopupSize+10 - yOffset;
      quad_nw_y = myCounter*yPopupSize+10 - yOffset;
      myCounter += 1;

      int xoffset = 10;
      quad_sw_x = rsigScene.sceneRect().width() - xPopupSize - xoffset;
      quad_se_x = rsigScene.sceneRect().width() - 0.0 - xoffset;
      quad_ne_x = rsigScene.sceneRect().width() - 0.0 - xoffset;
      quad_nw_x = rsigScene.sceneRect().width() - xPopupSize - xoffset;

      // shadow
      glColor4f(0.0, 0.0, 0.0, alpha-.8);
      int shadow_offsetX = 5;
      int shadow_offsetY = 5;
      glBegin(GL_QUADS);
      glVertex3f(quad_sw_x+shadow_offsetX, quad_sw_y+shadow_offsetY, 0.0);
      glVertex3f(quad_se_x+shadow_offsetX, quad_se_y+shadow_offsetY, 0.0);
      glVertex3f(quad_ne_x+shadow_offsetX, quad_ne_y+shadow_offsetY, 0.0);
      glVertex3f(quad_nw_x+shadow_offsetX, quad_nw_y+shadow_offsetY, 0.0);
      glEnd();

      // main quad
      glColor4f(253./256., 245./256., 230./256., alpha); // linen
      glBegin(GL_QUADS);
      glVertex3f(quad_sw_x, quad_sw_y, 0.0);
      glVertex3f(quad_se_x, quad_se_y, 0.0);
      glVertex3f(quad_ne_x, quad_ne_y, 0.0);
      glVertex3f(quad_nw_x, quad_nw_y, 0.0);
      glEnd();

      bool showLines = false;

      if (popupType == "ceilometer") {
        showLines = VisOptionsSingleton::getInstance()->getCeilometerShowLocations();
      } else if (popupType == "pandora") {
        showLines = VisOptionsSingleton::getInstance()->getPandoraShowLocations();
      }
      
      if (showLines) {
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        // fat white line
        glLineWidth(2.0);
        glColor4f(1.0, 1.0, 1.0, alpha);
        glBegin(GL_LINES);
        glVertex3f(quad_nw_x, quad_nw_y+(quad_sw_y-quad_nw_y)/2, 0.0);
        glVertex3f(xWinLoc, rsigScene.sceneRect().height()-yWinLoc, 0.0);
        glEnd();

        // thin black line
        glLineWidth(1.0);
        glColor4f(0.0, 0.0, 0.0, alpha);
        glBegin(GL_LINES);
        glVertex3f(quad_nw_x, quad_nw_y+(quad_sw_y-quad_nw_y)/2, 0.0);
        glVertex3f(xWinLoc, rsigScene.sceneRect().height()-yWinLoc, 0.0);
        glEnd();
      }

      int xMargin = 25;
      int yMargin = 40;

      int xStart = quad_sw_x + xMargin;
      int xEnd   = quad_se_x - xMargin;

      int yStart = quad_sw_y - yMargin;
      int yEnd   = quad_nw_y + xMargin;

      const char * xTickFormat;
      const unsigned int nDays = RSIGStateSingleton::getInstance()->mDuration;
      double profile_min_elev = 0.0;
      double profile_max_elev = 0.0;

      if (popupType == "ceilometer") {
        profile_min_elev = VisOptionsSingleton::getInstance()->getCeilometerMinAltitude() * 1000.;
        profile_max_elev = VisOptionsSingleton::getInstance()->getCeilometerMaxAltitude() * 1000.;
      } else if (popupType == "pandora") {
        profile_min_elev = VisOptionsSingleton::getInstance()->getPandoraMinAltitude() * 1000.;
        profile_max_elev = VisOptionsSingleton::getInstance()->getPandoraMaxAltitude() * 1000.;
      }
      
      yMin = profile_min_elev;
      yMax = profile_max_elev;
      xMin = 0;
      xMax = nDays * 24;
      xTickFormat = "%2.0f";
      const int nXticks = 6; // evenly divisible into 24
      const int xLabelOffset = 5;

      snprintf( title1String, sizeof title1String / sizeof *title1String,
                "%s", profileDatasetName.c_str() );
      snprintf( title2String, sizeof title2String / sizeof *title2String,
                "%s(%s)",
                 profileDatasetVariable.c_str(),
                 profileDatasetUnit.c_str() );
      snprintf(titleString, 80, "%s %s", title1String, title2String );

      // set up linear transform between data and screen space
      xSlope     = (xStart-xEnd) / (xMin - xMax);
      xIntercept = ((xStart+xEnd) - xSlope*(xMin+xMax)) / 2;
      ySlope     = (yStart-yEnd) / (yMin - yMax);
      yIntercept = ((yStart+yEnd) - ySlope*(yMin+yMax)) / 2;
      glColor4f(0.0, 0.0, 0.0, alpha);
      glLineWidth(1.0);
      // draw X-axis
      glBegin(GL_LINES);
      glVertex2f(xStart, yStart);
      glVertex2f(xEnd, yStart);
      glEnd();
      // X-axis label
      PRrenderBitmapString(xStart, yStart+30, 0, GLUT_BITMAP_HELVETICA_10, xlabelString);
      PRrenderBitmapString(xStart+35, yEnd-5, 0, GLUT_BITMAP_HELVETICA_10, titleString);

      // draw Y-axis
      glBegin(GL_LINES);
      glVertex2f(xStart, yStart);
      glVertex2f(xStart, yEnd);
      glEnd();
      // Y-axis label
      char ylabelString[50];
      sprintf(ylabelString, "%s", "Alt [km]");
      PRrenderBitmapString( quad_sw_x+10, yEnd-10, 0, GLUT_BITMAP_HELVETICA_10, ylabelString);
      // draw X ticks
      int xTickLen = 10;
      float xTickSize = (xMax - xMin) / nXticks; // data space

      for (int xTickInd=0; xTickInd<=nXticks; xTickInd++) {
        float thisTick = xTickInd*xTickSize;
        int xTickScreenX = xSlope*thisTick + xIntercept;
        glBegin(GL_LINES);
        glVertex2f(xTickScreenX, yStart-xTickLen);
        glVertex2f(xTickScreenX, yStart);
        glEnd();
        // label tick
        char tickString[50];
        sprintf(tickString, xTickFormat, thisTick);
        PRrenderBitmapString(xTickScreenX-xLabelOffset, yStart+12, 0, GLUT_BITMAP_HELVETICA_10, tickString);
      }

      // draw Y ticks
      int nYticks = 5;
      //int yTickLen = 10;
      float yTickSize = (yMax - yMin) / nYticks; // data space
      char yTickFormat[10];

      if (yTickSize/1000. > 2) {
        sprintf(yTickFormat, "%s", "%3.0f");
      } else {
        sprintf(yTickFormat, "%s", "%3.1f");
      }

      for (int yTickInd=0; yTickInd<=nYticks; yTickInd++) {
        float thisTick = yMin + yTickInd*yTickSize;
        int yTickScreenY = ySlope*thisTick + yIntercept;
        glBegin(GL_LINES);
        glVertex2f(xStart, yTickScreenY);
        glVertex2f(xStart+xTickLen, yTickScreenY);
        glEnd();
        // label tick
        char tickString[50];
        sprintf(tickString, yTickFormat, thisTick/1000);
        PRrenderBitmapString(xStart-25, yTickScreenY, 0, GLUT_BITMAP_HELVETICA_10, tickString);
      }


      // Reinitialize arrays cca::CCA, cca::Xticks, cca::Yticks from dataset:

      bool ok = cca::createCCA( globalStartTime, globalEndTime, dataset, profileDatasetName );

      if ( ok ) {

        // find yInd corresponding to ceilometer_max_elev

        int maxYInd = 0; // default

        for ( int yInd = 0; yInd < cca::SIZE_Y; ++yInd ) {

          if ( cca::Yticks[yInd] <= profile_max_elev ) {
            maxYInd = yInd;
          } else {
            yInd = cca::SIZE_Y;
          }
        }

        if ( maxYInd == cca::SIZE_Y - 1 ) {
          --maxYInd; // Reduce for use in loop below.
        }

        glPointSize(6.0);
        myLut =
          vislist->getLutByName( visDataObject->objectName().replace( ".point",
                                                                      ".lut" ));

        double ccamax = 0.0;
        const int denominator = cca::SECONDS_PER_HOUR + xIntercept;

        for ( int xInd = 0; xInd < cca::SIZE_X - 1; ++xInd ) {
          const int screenX0 = xSlope * cca::Xticks[ xInd ] / denominator;

          if ( screenX0 > xStart && screenX0 < xEnd ) {
            const int screenX1 = xSlope * cca::Xticks[ xInd + 1 ] / denominator;

            if ( screenX1 > xStart && screenX1 < xEnd ) {

              for ( int yInd = 0; yInd < maxYInd; ++yInd ) {
                const int screenY0 = ySlope * cca::Yticks[ yInd ] + yIntercept;

                if (screenY0 < yStart && screenY0 > yEnd ) {
                  const int screenY1 =
                    ySlope * cca::Yticks[ yInd + 1 ] + yIntercept;

                  if ( screenY1 < yStart && screenY1 > yEnd ) {
                    Vector4f thisColor;

                    if ( myLut ) {
                      thisColor = myLut->colorByValue(cca::CCA[ xInd ][ yInd ]);
                    }

                    if (cca::CCA[ xInd ][ yInd ] > ccamax) {
                      ccamax = cca::CCA[ xInd ][ yInd ];
                    }

                    glColor4f(thisColor.x, thisColor.y, thisColor.z, alpha);
                    glBegin(GL_QUADS);
                    glVertex2f(screenX0, screenY0);
                    glVertex2f(screenX1, screenY0);
                    glVertex2f(screenX1, screenY1);
                    glVertex2f(screenX0, screenY1);
                    glEnd();
                  }
                }
              }
            }
          }
        }

        //qDebug() << "cca max = " << ccamax;
        
        // Optionally overlay mixing layer height:

        if ( popupType == "ceilometer" && vOptions->getCeilometerShowMixingLayer() ) {
          size_t profileMixingLayerHeightDatasetIndex = 0;
          ok = cca::haveCeilometerMixingLayerHeightDataset( profileMixingLayerHeightDatasetIndex );

          if ( ok ) {
            const emvl::Dataset& profileMixingLayerHeightDataset =
              datasetManager[ profileMixingLayerHeightDatasetIndex ];
            ok = cca::createMLA( globalStartTime, globalEndTime,
                                 profileMixingLayerHeightDataset,
                                 profileDatasetName );

            if ( ok ) {
              glPointSize(1.0);
              glColor4f(1.0, 1.0, 1.0, alpha);
              glBegin(GL_POINTS);

              for ( size_t thisPoint = 0, count = cca::MLA_times.size();
                    thisPoint < count; ++thisPoint ) {
                const double thisElevation = cca::MLA_elevations[ thisPoint ];

                if ( thisElevation >= yMin && thisElevation <= yMax ) {
                  const double thisX =
                    xSlope * cca::MLA_times[ thisPoint ] / cca::SECONDS_PER_HOUR
                    + xIntercept;
                  const double thisY = ySlope * thisElevation + yIntercept;
                  glVertex2f(thisX, thisY);
                }
              }

              glEnd();
            }
          }
        }

        // draw time marker
        // Get timestamp of beginning and end of current hour:
        const float timeScreenX = xSlope * timestep + xIntercept;
        // fat black line
        glLineWidth(2.0);
        glColor4f(0.0, 0.0, 0.0, alpha);
        glBegin(GL_LINES);
        glVertex2f(timeScreenX, yStart);
        glVertex2f(timeScreenX, yEnd);
        glEnd();
        // thin white line
        glLineWidth(1.0);
        glColor4f(1.0, 1.0, 1.0, alpha);
        glBegin(GL_LINES);
        glVertex2f(timeScreenX, yStart);
        glVertex2f(timeScreenX, yEnd);
        glEnd();
      }
    }
  }

  glEnable(GL_TEXTURE_2D);
  
  // restore the original 3D matrix
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glLineWidth(orig_linewidth);
  glDisable(GL_LINE_SMOOTH);
  
  rsigScene.ceilometerCCADrawFlag = false;
  
}


// Draw pop-up plots of neubrew data:

void popupPlot(const QRectF& /* viewBounds */, bool /* showNullSample */,
               const size_t coordinatesCount, const emvl::Point* coordinates,
               const size_t dataCount, const double* data,
               int /* thisSubsetInd */, int maxSubsetInd,
               float mPointSize, float mLineWidth, const QColor& mPointColor,
               float alpha, float threshold, bool showThreshold, bool /* flag3D */,
               const std::string& dataName, const std::string& note ) {
  
  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  const bool isNeubrew = dataName.find( "neubrew" ) == 0;

  if ( ! isNeubrew ) {
    qDebug() << "popup type not supported";
    return;
  }

  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  RSIGScene& rsigScene = RSIGScene::getInstance();
  VisDataList* vislist = VisDataListSingleton::getInstance();

  float aspect_ratio = 1.4;
  const int yPopupSize = vOptions->getNeubrewSize();
  int xPopupSize = yPopupSize * aspect_ratio;

  int yOffset; // set below

  if (flagPopup3D) {
    // 3D mode
    yOffset = 10;
  } else if (!flagCalipso) {
    // 2D mode, but without calipso
    yOffset = -80; // Move below HUD banner.
  } else {
    // 2D mode with calipso
    yOffset = -215;
  }

  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  
  glGetIntegerv(GL_VIEWPORT, viewport);   
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);

  GLdouble xWinLoc = 0.0, yWinLoc = 0.0, zWinLoc = 0.0;

  // push the matrix to prepare for doing 2D overplot
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  // set up 2D rendering
  float orig_linewidth;
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  glOrtho(0.0f, rsigScene.sceneRect().width(), rsigScene.sceneRect().height(), 0.0f, 0.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glGetFloatv(GL_LINE_WIDTH, &orig_linewidth);
  glDisable(GL_TEXTURE_2D);

  for ( int index =  coordinatesCount-1; index >= 0; index-- ) { 
    const emvl::Point& point = coordinates[ index ];

    // figure out screen coordinates of point[0]
    if (index == 0) {

      if (flagPopup3D) {
        gluProject(point.longitude, point.latitude, 0.0, modelview, projection, viewport, &xWinLoc, &yWinLoc, &zWinLoc);
      } else {
        // HACK for 2D
        QPointF screenpoint = rsigScene.toScreenPointF(QPointF(point.longitude, point.latitude));
        xWinLoc = screenpoint.x();
        yWinLoc = rsigScene.sceneRect().height() - screenpoint.y();
        zWinLoc = 0.5;
      }
    }
  }

  int quad_sw_y;
  int quad_se_y;
  int quad_ne_y;
  int quad_nw_y;
  int quad_sw_x;
  int quad_se_x;
  int quad_ne_x;
  int quad_nw_x;
  int myCounter;
  int IDindex = 0; // default
  int varIndex = 0; //default
  bool continueMakingPopups = true; // default
  
  if ( isNeubrew ) {
    // coordinates for placing quad at extreme left of window, stacked vertically according to subset number
    myCounter = vislist->getCounterNeubrew();
    int plotScrollerValue = VisOptionsSingleton::getInstance()->getNeubrewPlotScrollerValue();
    int nPlots = maxSubsetInd;
    int scrollerOffset = plotScrollerValue * (nPlots*yPopupSize)/100;
    yOffset = yOffset + scrollerOffset;

    quad_sw_y = (myCounter +1)*yPopupSize - yOffset;
    quad_se_y = (myCounter +1)*yPopupSize - yOffset;
    quad_ne_y = myCounter*yPopupSize+10 - yOffset;
    quad_nw_y = myCounter*yPopupSize+10 - yOffset;
    vislist->setCounterNeubrew(myCounter + 1);
    
    quad_sw_x = 0;
    quad_se_x = xPopupSize;
    quad_ne_x = xPopupSize;
    quad_nw_x = 0.0;
  } else {
    qDebug() << "popup type not supported";
    return;
  }

  if (continueMakingPopups) {
    // shadow
    glColor4f(0.0, 0.0, 0.0, (alpha-0.8)>0.0?(alpha-0.8):0.0);
    int shadow_offsetX = 5;
    int shadow_offsetY = 5;
    glBegin(GL_QUADS);
    glVertex3f(quad_sw_x+shadow_offsetX, quad_sw_y+shadow_offsetY, 0.0);
    glVertex3f(quad_se_x+shadow_offsetX, quad_se_y+shadow_offsetY, 0.0);
    glVertex3f(quad_ne_x+shadow_offsetX, quad_ne_y+shadow_offsetY, 0.0);
    glVertex3f(quad_nw_x+shadow_offsetX, quad_nw_y+shadow_offsetY, 0.0);
    glEnd();

    // main quad
    glColor4f(253./256., 245./256., 230./256., alpha); // linen
    glBegin(GL_QUADS);
    glVertex3f(quad_sw_x, quad_sw_y, 0.0);
    glVertex3f(quad_se_x, quad_se_y, 0.0);
    glVertex3f(quad_ne_x, quad_ne_y, 0.0);
    glVertex3f(quad_nw_x, quad_nw_y, 0.0);
    glEnd();
    
    // draw lines from plot right-edge center to 3D point
    bool showLines = true; // default

    if (isNeubrew && VisOptionsSingleton::getInstance()->getNeubrewShowLocations() == false) {
      showLines = false;
    }

    if (showLines) {
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      // fat white line
      glLineWidth(2.0);
      glColor4f(1.0, 1.0, 1.0, alpha);
      glBegin(GL_LINES);

      if ( isNeubrew ) {
        glVertex3f(quad_ne_x, quad_ne_y+(quad_se_y-quad_ne_y)/2, 0.0);
      } else { // ceilometer and others
        glVertex3f(quad_nw_x, quad_nw_y+(quad_sw_y-quad_nw_y)/2, 0.0);
      }

      glVertex3f(xWinLoc, rsigScene.sceneRect().height()-yWinLoc, 0.0);
      glEnd();
      // thin black line
      glLineWidth(1.0);
      glColor4f(0.0, 0.0, 0.0, alpha);
      glBegin(GL_LINES);

      if ( isNeubrew ) {
        glVertex3f(quad_ne_x, quad_ne_y+(quad_se_y-quad_ne_y)/2, 0.0);
      } else { // ceilometer and others
        glVertex3f(quad_nw_x, quad_nw_y+(quad_sw_y-quad_nw_y)/2, 0.0);
      }

      glVertex3f(xWinLoc, rsigScene.sceneRect().height()-yWinLoc, 0.0);
      glEnd();
    }

    // plot data  
    // create line plot on top of quad
    createLinePlot(coordinatesCount, coordinates,
                   dataCount, data, dataName, note,
                   mPointSize, mLineWidth, mPointColor,
                   alpha, threshold, showThreshold,
                   IDindex, varIndex, // used for ceilometer, dummy otherwise
                   quad_sw_x, quad_sw_y,
                   quad_se_x, quad_se_y,
                   quad_ne_x, quad_ne_y,
                   quad_nw_x, quad_nw_y);
  }

  // restore the original 3D matrix
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glLineWidth(orig_linewidth);
  glDisable(GL_LINE_SMOOTH);
  glEnable(GL_TEXTURE_2D);
}



void render3d(const QRectF& /* viewBounds */, bool /* showNullSample */,
              const size_t coordinatesCount, const emvl::Point* coordinates,
              const size_t /* dataCount */, const double* data,
              const std::string& /* dataName */, const std::string& /* note */,
              float mPointSize, float /* mLineWidth */,
              const QColor& /* mPointColor */,
              float alpha, float threshold, bool showThreshold) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  glEnable(GL_DEPTH_TEST);
  bool useSprites = false; // Matt: delete sprite code since not used?

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (useSprites) {
    // for using point sprites
    int textureID = genPointSpriteTexture(1, 1);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_POINT_SMOOTH);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  } 
  
  // Draw colored 'fat points':

  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  const double degrees_per_meter = 1.0/110574.2727;
  const double z_scaleFactor = vOptions->getCalipsoScaleFactor(); 

  // nudge 2D point values slightly upward, mainly to get Airnow, AQS, etc above CMAQ layer 1.
  float elevation_nudge = 0.0;

  // hack elevation in the case of hrsl mixing_height
  // make sure string compare is outside of point loop below!
  bool hsrl_hack = false; // default

  if (descrip.contains("hsrl"))  {
    hsrl_hack = true;
  }

  // hack for ceilometer - don't plot data outside of range
  bool ceilometer_hack = false; // default

  if (descrip.contains("eilometer"))  {
    ceilometer_hack = true;
  }

  for (int pass=0; pass<=0; pass++) {
    // pass=1, large gray points
    // pass=0, slightly smaller colored points

    if (pass == 1) {
      glPointSize( mPointSize + 2);
    } else {
      glPointSize( mPointSize );
    }

    glBegin( GL_POINTS );
    bool flag2;

    // HACK: drawing points in reverse order seems to improve rendering.
    // Speed or quality?

    for ( int index =  coordinatesCount-1; index >= 0; index-- ) {
      const emvl::Point& point = coordinates[ index ];
      const double thisData = data[ index ];
      assert( myLut );
      const Vector4f thisColor = myLut->colorByValue(thisData);
      float my_elevation = point.elevation;
      // hack for ceilometer - don't plot data outside of range
      bool plot_flag = true; //default

      if (ceilometer_hack && thisData >= 9e5) {
        plot_flag = false;
      }

      // hack elevation in the case of hrsl mixing_height
      if (hsrl_hack)  {
        my_elevation = thisData;
        hasElevation = true;
      }


      if (plot_flag) { // for ceilometer hack

        if (pass == 1) {
          glColor4f(0.6, 0.6, 0.6, alpha); // gray outline
        } else {
          glColor4f(thisColor.x, thisColor.y, thisColor.z, alpha);
        }

        if (showThreshold) {
          flag2 = thisData >= threshold;
        } else {
          flag2 = true; // no thresholding
        }

        if (thisData != emvl::MISSING && flag2) {

          if (hasElevation) {
            // draw point
            glVertex3f( point.longitude,
                        point.latitude,
                        fabs(my_elevation) * degrees_per_meter * z_scaleFactor);

            // draw shadow
            glColor4f( 0.1, 0.1, 0.1, alpha*0.3);
            glVertex2f( point.longitude, point.latitude);

          } else {
              glVertex3f( point.longitude,
                          point.latitude,
                          elevation_nudge);
          }
        }
      }
    }
    
    glEnd();
  }

  glPointSize( 1.0 );

  if (useSprites) {
    // back to decal mode so that the map doesn't get messed up
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 
    glBindTexture(GL_TEXTURE_2D, 0);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_FALSE);
    glDisable(GL_POINT_SPRITE);
  }

  glDisable(GL_BLEND);

}



static void 
drawOneGroup(const QRectF& viewBounds, bool showNullSample,
             const size_t coordinatesCount, const emvl::Point* coordinates,
             const size_t dataCount, const double* data,
             int thisSubsetInd, int maxSubsetInd,
             float mPointSize, float mLineWidth, const QColor& mPointColor,
             float alpha, float threshold, bool showThreshold,
             RenderingContextRSIGBase *context,
             const std::string& dataName, const std::string& note) {


  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  float pointSize2 = mPointSize - mLineWidth*2.0f;
  if (pointSize2<0.5f) pointSize2=0.5f;
  const unsigned char gray = 120;
  const unsigned char black = 255;
  const bool isNeubrew = dataName.find( "neubrew" ) == 0;

  if (context == NULL) {
    // 3d mode rendering
    render3d(viewBounds, showNullSample,
             coordinatesCount, coordinates,
             dataCount, data, dataName, note,
             mPointSize, mLineWidth, mPointColor,
             alpha, threshold, showThreshold);

    
    if (flagPopupPlot && isNeubrew) {
      popupPlot(viewBounds, showNullSample,
                coordinatesCount, coordinates,
                dataCount, data,
                thisSubsetInd, maxSubsetInd,
                mPointSize, mLineWidth, mPointColor,
                alpha, threshold, showThreshold, true, dataName, note );
    }

  } else { // 2d mode rendering

    //==================draw the NULL sample points========================
    //draw the boundary lines for points

    if (showNullSample) {

      if (context != NULL) {
        context->setTextureWeight(0.0f);
      }
      
      glDisable(GL_BLEND);
      glColor3ub(black, black, black);
      glPointSize(mPointSize);
      pagedBufferDraw( coordinatesCount, coordinates, dataCount, data,
           threshold, showThreshold, true, viewBounds );
      
      //draw inner content for points
      float pointSize2 = mPointSize - 2.0f;

      if (pointSize2 > 0) {
        glColor3ub(gray, gray, gray);
        glPointSize(pointSize2);
        pagedBufferDraw( coordinatesCount, coordinates, dataCount, data,
                         threshold, showThreshold, true, viewBounds );
      }
    }
    
    //==================draw the non-stationary points========================
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    const int ialpha = int(alpha * 255.0f);
    glColor4ub(mPointColor.red(), mPointColor.green(), mPointColor.blue(), ialpha);
 
    //draw the boundary lines for points

    if (mPointSize != pointSize2) {
      glPointSize(mPointSize);

      if (context != NULL) {
        context->setTextureWeight(0.0f);
      }

      pagedBufferDraw( coordinatesCount, coordinates, dataCount, data,
                       threshold, showThreshold, false, viewBounds );
    }
    
    //draw inner content for points
    glPointSize(pointSize2);

    if (context != NULL) {
      context->setTextureWeight(1.0f);
    }

    pagedBufferDraw( coordinatesCount, coordinates, dataCount, data,
                     threshold, showThreshold, false, viewBounds );
    glDisable(GL_BLEND);

    if (flagPopupPlot && isNeubrew) {
      popupPlot(viewBounds, showNullSample,
                coordinatesCount, coordinates,
                dataCount, data,
                thisSubsetInd, maxSubsetInd,
                mPointSize, mLineWidth, mPointColor,
                alpha, threshold, showThreshold, false, dataName, note );
    }
  }
}



/*!
 \fn void PointRenderer::drawPoints()
 Draw the point dataset using the point representation. 
 */
//FIX rename this routine withou the gl prefix!
void PointRenderer::drawPoints(RenderingContextRSIGBase *context) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  GeoDataObject *obj = qobject_cast<GeoDataObject *>( parent() );
  
  if ( obj == NULL ) {
    return;
  }

  const emvl::Dataset* const dataset = obj->model();

  if ( dataset == NULL ) {
    return;
  }

  // Get the lon-lat bounding box of the view window:
  
  const QRectF viewBounds = obj->getDataRequestBoundingBox();
  
  // Get timestamp of beginning and end of current hour:
  
  assert( dataset->cellVertexCount() == 1 );
  const RSIGState* const rsigState = RSIGStateSingleton::getInstance();
  emvl::Timestamp beginTime( rsigState->getCurrentTimestamp() );
  const emvl::Timestamp endTime( rsigState->getEndRenderTimestamp() );

  VisOptions* vOptions = VisOptionsSingleton::getInstance();
  const bool isHMS = dataset->description().find("hmsserver") != std::string::npos;

  if ( isHMS && vOptions->getPersistHMS() )  {
    const int persistHours = vOptions->getPersistHMSHours();
    beginTime.offsetHours( -persistHours );
  }
  
  glEnableClientState(GL_VERTEX_ARRAY);
  
  //bind the texture sprite data

  if (context != NULL) {
    setPointSpriteTexture(mPointShape, context);
  }
  
  if (mUseOpenglPointSmooth) {
    glEnable(GL_POINT_SMOOTH); // squares will be rendered as circles on supported graphics cards
  } else if (mPointShape != 0) {
    //Enable point sprites and set params for points.
    glEnable(GL_POINT_SPRITE);
    //glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }

  // Determine if drawing a ceilometer curtain:

  const std::string& datasetName = dataset->name();
  const bool isCeilometer = datasetName.find( "ceilometer" ) == 0;
  const bool isPandora    = datasetName.find( "pandora" )    == 0;

  //qDebug() << "datasetName = " << QString::fromStdString(datasetName);
  
  const bool drawCeilometerCurtain =
    isVisible()  &&
    isCeilometer &&
    RSIGScene::getInstance().ceilometerCCADrawFlag;

  const bool drawPandoraCurtain =
    isVisible() &&
    isPandora   &&
    RSIGScene::getInstance().pandoraCCADrawFlag;

  if ( drawCeilometerCurtain ) {
    drawCCAPopup("ceilometer");
  } else if (drawPandoraCurtain) {
    //drawCCAPopup("pandora");
  }

  if ( isVisible() ) {

    // Get groups of pointers to subset of coordinates and data:

    const std::vector<emvl::Dataset::SubsetPointers> subsets =
      dataset->subset( beginTime, endTime );

    for ( size_t subset = 0; subset < subsets.size(); subset++ ) {
      const emvl::Dataset::SubsetPointers subsetPointers( subsets[ subset ] );
      assert( subsetPointers.coordinates );
      std::string note;

      if ( subsetPointers.notesCount ) {
        note = subsetPointers.notes[ 0 ];
      }

      drawOneGroup(viewBounds, mShowNullSample,
         subsetPointers.coordinatesCount,
         subsetPointers.coordinates,
         subsetPointers.dataCount,
         subsetPointers.data,
         subset,
         subsets.size(),
         mPointSize, mLineWidth, mPointColor, mAlpha, mThreshold, mShowThreshold,
         context,
         datasetName, note );
    }
  }
    
  glDisableClientState(GL_VERTEX_ARRAY);

  if (mUseOpenglPointSmooth) {
    glDisable(GL_POINT_SMOOTH);
  } else if (mPointShape != 0) {
    glDisable(GL_POINT_SPRITE_ARB); // Delete unused SPRITE code?
    //glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_FALSE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  }
}



bool PointRenderer::isLookupTableWidget() const {
  return false;
}



bool PointRenderer::isWidgetOfType(QString inquiry) const {
  GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());
  return par->varName().contains(inquiry, Qt::CaseInsensitive);
}



bool PointRenderer::is3DObject() const {
  return true;
}



void PointRenderer::render(QPainter* /* painter */,
                           const QRect& /* rect */,
                           RenderingContextRSIGBase* context,
                           bool /* makePopupPlot */) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }
  
  if ( parent() && isVisible() ) {

    if ( context ) {
      GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());

      if ( par ) {
        myLut = par->getLut();
        assert( myLut );
        setContextColors( true, myLut, context );
        // Use vis object minmax rather than data minmax
        theMinmax = this->minmaxVal();
        hasElevation = par->hasElevation();
        flagPopupPlot = false;
        flagPopup3D = false;
      }
    }
    
    drawPoints( context );
  }
}



void PointRenderer::renderEMVL(QPainter* /* painter */,
                               const QRect& /* rect */,
                               bool makePopupPlot) {
  
  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }
  
  if ( parent() && isVisible() ) {
    GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());
    
    if ( par ) {
      myLut = par->getLut();
      hasElevation = par->hasElevation();
      descrip = par->description();
      
      // Use vis object minmax rather than data minmax
      theMinmax = this->minmaxVal();
      flagPopupPlot = makePopupPlot;
      flagPopup3D = true;
      drawPoints( NULL );
    }
  }
}


void PointRenderer::renderDisplayPanel(QPainter* /* painter */,
                                       const QRect& /* rect */,
                                       RenderingContextRSIGBase* context) {
  
  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }
  
  if ( parent() && isVisible() ) {
    
    if ( context ) {
      GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());
      
      if ( par ) {
        myLut = par->getLut();
        assert( myLut );
        const int texID = par->lutTextureID();
        context->setLutToTextureChannel0( texID );
        std::vector<float> myValues( myLut->getValues() );
        context->setLutValues( myValues );
        context->setIsPointUniformParameter( 1 );
        
        // Use vis object minmax rather than data minmax
        theMinmax = this->minmaxVal();
        hasElevation = par->hasElevation();
        flagPopupPlot = false;
        flagPopup3D = false;
      }
    }
    
    //drawPoints( context );
  }
}



void PointRenderer::render2DCalipsoCurtain(QPainter*, const QRect& ) {
  // this will never be called if regridding is on
}




// this function is what will lead to rendering popups in the 2D plot

void PointRenderer::renderPopupNeubrew(QPainter* /* painter */,
                                       const QRect& /* rect */,
                                       bool haveCalipso) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }

  if (parent()==NULL ||!isVisible()) {
    return;
  }

  GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());

  if (par != NULL) {
    myLut = par->getLut();
    hasElevation = par->hasElevation();
    descrip = par->description();
    theMinmax = this->minmaxVal();
    flagPopupPlot = true;
    flagPopup3D = false;
    flagCalipso = haveCalipso;
    VisDataList* vislist = VisDataListSingleton::getInstance();
    vislist->setCounterNeubrew(0);
    drawPoints( NULL );
  }
}




void PointRenderer::renderPopupCeilometer(QPainter* /* painter */,
                                          const QRect& /* rect */,
                                          bool haveCalipso) {

  if (traceFunctions) { qDebug() << "in" << __FUNCTION__ ; }

  GeoDataObject *par = qobject_cast<GeoDataObject*>(parent());

  if (par != NULL) {
    flagCalipso = haveCalipso;
    drawCCAPopup("ceilometer");
  }
}

void PointRenderer::renderPopupPandora(QPainter* /* painter */,
                                       const QRect& /* rect */,
                                       bool haveCalipso) {

  if (traceFunctions) { qDebug() << "in" << __FUNCTION__ ; }

  GeoDataObject *par = qobject_cast<GeoDataObject*>(parent());

  if (par != NULL) {
    flagCalipso = haveCalipso;
    //drawCCAPopup("pandora");
  }
}



int PointRenderer::pointShape() const {
  return mPointShape;
}



void PointRenderer::setPointShape(int shape) {

  if (traceFunctions) {
    qDebug() << "in" << __FUNCTION__ ;
  }
  
  if (shape >= PointRenderer::POINTSPRITE_TEXBUFF_SIZE) {
    shape = PointRenderer::POINTSPRITE_TEXBUFF_SIZE-1;
  }

  if (shape < 0) {
    shape = 0;
  }

  if (mPointShape != shape) {
    mPointShape = shape;
    Q_EMIT valueChanged();
  }
}



float PointRenderer::pointSize() const {
  return mPointSize;
}



void PointRenderer::setPointSize(float r) {
  const float PNTSIZE_MIN = 1.0f;
  const float PNTSIZE_MAX = 100.0f;

  if ( r < PNTSIZE_MIN ) {
    r = PNTSIZE_MIN;
  }

  if ( r > PNTSIZE_MAX ) {
    r = PNTSIZE_MAX;
  }

  if (r != mPointSize) {
    mPointSize = r;
    Q_EMIT valueChanged();
  }
}



float PointRenderer::lineWidth() const {
  return mLineWidth;
}



void PointRenderer::setLineWidth(float w) {

  if ( w < 0 ) {
    w = 0;
  }

  if ( w > 10) {
    w = 10;
  }

  if (mLineWidth != w) {
    mLineWidth = w;
    Q_EMIT valueChanged();
  }
}



QColor PointRenderer::pointColor() const {
  return mPointColor;
}



void PointRenderer::setPointColor(const QColor& color) {

  if ( mPointColor != color ) {
    mPointColor = color;
    Q_EMIT valueChanged();
  }
}



bool PointRenderer::showNullSample() const {
  return mShowNullSample;
}



void PointRenderer::setShowNullSample(bool flag) {
  
  if (mShowNullSample != flag) {
    mShowNullSample = flag;
    Q_EMIT valueChanged();
  }
}


bool PointRenderer::useOpenglPointSmooth() const {
  return mUseOpenglPointSmooth;
}



void PointRenderer::setUseOpenglPointSmooth(bool flag) {
  
  if (mUseOpenglPointSmooth != flag) {
    mUseOpenglPointSmooth = flag;
    Q_EMIT valueChanged();
  }
}



QString PointRenderer::description() const {
  return "point";
}



