/******************************************************************************
PURPOSE: VisDataObject.cpp - Implements the VisDataObject class.
         Represent the base object of visualization data.
         The parent of the visualization object should be provided in the ctor.
         The parent should be geometric object, containing a emvl::Dataset which is
         downloaded from the web or loaded from a file and owned by
         emvl::DatasetManager.

NOTES: Implemented using QT's QObject class

HISTORY: 2013/02/25, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#include <assert.h>

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h> /* For glut*. */
#else
#include <GL/glut.h> /* For gl*. */
#endif

#include <QPainter>
#include <QGLWidget>
#include <QDebug>

#include "VisDataObject.h"
#include "GeoDataObject.h"
#include "LookupTableWidget.h"
#include "RenderingContextRSIG.h"



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


// Class/static variables

int VisDataObject::mPointSpriteTexID[VisDataObject::POINTSPRITE_TEXBUFF_SIZE]={0};




// This global function is called by child classes and RSIGScene.cpp:

void setPointSpriteTexture(int mode, RenderingContextRSIGBase *context) {
  //bind the texture sprite data
  int& spriteTexID = VisDataObject::mPointSpriteTexID[mode];

  if ( spriteTexID == 0) {
    spriteTexID = genPointSpriteTexture(mode, 0);
  }

  context->setLutToTextureChannel(2, spriteTexID, "texSprite", 1);
}



class FreeTextureResource {
public:
  FreeTextureResource() {

    for (int i=0; i<VisDataObject::POINTSPRITE_TEXBUFF_SIZE; i++) {
      VisDataObject::mPointSpriteTexID[i] = 0;
    }
  }

  ~FreeTextureResource() {

    for (int i=0; i<VisDataObject::POINTSPRITE_TEXBUFF_SIZE; i++) {
      const GLuint texid = VisDataObject::mPointSpriteTexID[i];

      if ( texid ) {
        glDeleteTextures(1, &texid);
        VisDataObject::mPointSpriteTexID[i] = 0;
      }
    }
  }
};


static FreeTextureResource gFreeTextureResource; // Singleton?



VisDataObject::VisDataObject(QObject *parent):
    QObject(parent)
    ,mShowObject(true)
    ,mAlpha(1.0f)
    ,mThreshold(1.0)
    ,mThresholdSliderValue(0)
    ,mShowCellBoundary(false)
    ,mShowThreshold(false)
    ,mIsDefaultRangeChecked(true)
    ,mMinval(0.0)
    ,mMaxval(0.0)
    ,mLastMinval(0.0)
    ,mLastMaxval(0.0)
    ,mMinValChangedThisSession(false)
    ,mMaxValChangedThisSession(false)    
    ,mCurColormapIndex(0)
    ,mShowCMAQRow(false)
    ,mShowCMAQCol(false)
    ,mShowCMAQLayer(true)
    ,mCMAQRow(0)
    ,mCMAQCol(0)
    ,mCMAQLayer(0)
    ,mMaxCMAQRow(0)
    ,mMaxCMAQCol(0)
    ,mMaxCMAQLayer(0)
    ,mAggregate(QString("none"))
    ,mDataMenuName(QString("")) {
}



VisDataObject::~VisDataObject() {
}



/*!
 \fn bool VisDataObject::showObject() const
 
 Get the value of visibility.
 */

bool VisDataObject::showObject() const {
  return mShowObject;
}



/*!
 \fn void VisDataObject::setShowObject(bool flag)

 Set the value of visibility.
 */

void VisDataObject::setShowObject(bool flag) {

  if (mShowObject != flag) {
    mShowObject = flag;
    Q_EMIT valueChanged();
  }
}



bool VisDataObject::showCellBoundary() const {
  return mShowCellBoundary;
}



void VisDataObject::setShowCellBoundary(bool flag) {

  if (mShowCellBoundary != flag) {
    mShowCellBoundary = flag;
    Q_EMIT valueChanged();
  }
}



QString VisDataObject::getDataMenuName() const {
  return mDataMenuName;
}



void VisDataObject::setDataMenuName(QString x) {
  mDataMenuName = x;
}



QString VisDataObject::getAggregate() const {
  return mAggregate;
}



void VisDataObject::setAggregate(QString x) {
  mAggregate = x;
}



/*!
 \fn bool VisDataObject::isVisible() const

 Return whether the object is visible/hidden. 
 Here the visibility is controlled by its parent geometry object and its own property value \e mShowObject.
 If the parent object is not visible, the object will also be hidden. 
 If the \e mShowObject is false, it will also be hidden.
 */

bool VisDataObject::isWidgetOfType(QString inquiry) const {
  GeoDataObject *par = qobject_cast<GeoDataObject*>(parent());

  if (par->varName().contains(inquiry, Qt::CaseInsensitive)) {
    return true;
  } else {
    return false;
  }
}



bool VisDataObject::isVisible() const {
  bool vis = true;

  if (mShowObject == false) {
    vis = false;
  } else {
    GeoDataObject* w = qobject_cast<GeoDataObject*>(parent());

    if ( w != NULL ) {
      vis = w->getShowObject();
    }
  }

  return vis;
}



Vector2f VisDataObject::minmaxData() const {
  // get the absolute min/max for this dataset
  Vector2f minmax(0, 0);
  GeoDataObject *obj = qobject_cast<GeoDataObject *>( parent() );

  if (obj != NULL) {
    minmax = obj->minmaxVal();
  }

  return minmax;
}



Vector2f VisDataObject::minmaxVal() const {
  // get the user specified min/max for this dataset
  Vector2f minmax(0, 0);

  if ( !((mMinval == 0.0) && (mMaxval == 0.0)) ) {
    minmax.x = mMinval;
    minmax.y = mMaxval;
  } else {
    minmax = minmaxData();
  }

  return minmax;
}



qreal VisDataObject::alpha() const {
  return mAlpha;
}



void VisDataObject::setAlpha(qreal alpha) {

  if ( alpha != mAlpha ) {
    mAlpha = alpha;

    if (mAlpha > 1.0f) {
      mAlpha = 1.0f;
    }

    if (mAlpha < 0.0f) {
      mAlpha = 0.0f;
    }

    Q_EMIT valueChanged();
  }
}



qreal VisDataObject::threshold() const {
  return mThreshold;
}



void VisDataObject::setThreshold(qreal threshold) {

  if ( threshold != mThreshold ) {
    mThreshold = threshold;
    Q_EMIT valueChanged();
  }
}



int VisDataObject::thresholdSliderValue() const {
  return mThresholdSliderValue;
}



void VisDataObject::setThresholdSliderValue(int thresholdSliderValue) {

  if ( thresholdSliderValue != mThresholdSliderValue ) {
    mThresholdSliderValue = thresholdSliderValue;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setDefaultRangeChecked(bool flag) {

  if (mIsDefaultRangeChecked != flag) {
    mIsDefaultRangeChecked = flag;
    Q_EMIT valueChanged();  
  }
}



bool VisDataObject::isDefaultRangeChecked() const {
  return mIsDefaultRangeChecked;
}


double VisDataObject::minval() const {
  return mMinval;
}



double VisDataObject::maxval() const {
  return mMaxval;
}



double VisDataObject::lastMinval() const {
  return mLastMinval;
}



double VisDataObject::lastMaxval() const {
  return mLastMaxval;
}



bool VisDataObject::minValChangedThisSession() const {
  return mMinValChangedThisSession;
}



bool VisDataObject::maxValChangedThisSession() const {
  return mMaxValChangedThisSession;
}



int VisDataObject::curColormapIndex() const {
  return mCurColormapIndex;
}


void VisDataObject::setMinval(double minval) {

  if ( minval != mMinval ) {
    mMinval = minval;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setMaxval(double maxval) {

  if ( maxval != mMaxval ) {
    mMaxval = maxval;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setLastMinval(double minval) {

  if ( minval != mLastMinval ) {
    mLastMinval = minval;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setLastMaxval(double maxval) {

  if ( maxval != mLastMaxval ) {
    mLastMaxval = maxval;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setMinValChangedThisSession(bool flag) {
  mMinValChangedThisSession = flag;
  Q_EMIT valueChanged();
}



void VisDataObject::setMaxValChangedThisSession(bool flag) {
  mMaxValChangedThisSession = flag;
  Q_EMIT valueChanged();
}



void VisDataObject::setCurColormapIndex(int curColormapIndex) {

  if ( curColormapIndex != mCurColormapIndex ) {
    mCurColormapIndex = curColormapIndex;
    Q_EMIT valueChanged();
  }
}



int VisDataObject::CMAQRow() const {
  return mCMAQRow;
}



int VisDataObject::CMAQCol() const {
  return mCMAQCol;
}



int VisDataObject::CMAQLayer() const {
  return mCMAQLayer;
}



int VisDataObject::MaxCMAQRow() const {
  return mMaxCMAQRow;
}



int VisDataObject::MaxCMAQCol() const {
  return mMaxCMAQCol;
}



int VisDataObject::MaxCMAQLayer() const {
  return mMaxCMAQLayer;
}



void VisDataObject::setCMAQRow(int row) {

  if ( row != mCMAQRow ) {
    mCMAQRow = row;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setCMAQCol(int col) {

  if ( col != mCMAQCol ) {
    mCMAQCol = col;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setCMAQLayer(int layer) {

  if ( layer != mCMAQLayer ) {
    mCMAQLayer = layer;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setMaxCMAQRow(int maxRow) {

  if ( maxRow != mMaxCMAQRow ) {
    mMaxCMAQRow = maxRow;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setMaxCMAQCol(int maxCol) {

  if ( maxCol != mMaxCMAQCol ) {
    mMaxCMAQCol = maxCol;
    Q_EMIT valueChanged();
  }
}



void VisDataObject::setMaxCMAQLayer(int maxLayer) {

  if ( maxLayer != mMaxCMAQLayer ) {
    mMaxCMAQLayer = maxLayer;
    Q_EMIT valueChanged();
  }
}



bool VisDataObject::ShowCMAQRow() const {
  return mShowCMAQRow;
}



bool VisDataObject::ShowCMAQCol() const {
  return mShowCMAQCol;
}



bool VisDataObject::ShowCMAQLayer() const {
  return mShowCMAQLayer;
}



void VisDataObject::setShowCMAQRow(bool flag) {

  if (mShowCMAQRow != flag) {
    mShowCMAQRow = flag;
  }
}



void VisDataObject::setShowCMAQCol(bool flag) {
  if (mShowCMAQCol != flag) {
    mShowCMAQCol = flag;
  }
}



void VisDataObject::setShowCMAQLayer(bool flag) {

  if (mShowCMAQLayer != flag) {
    mShowCMAQLayer = flag;
  }
}



bool VisDataObject::ShowThreshold() const {
  return mShowThreshold;
}



void VisDataObject::setShowThreshold(bool flag) {

  if (mShowThreshold != flag) {
    mShowThreshold = flag;
  }
}



// Helper called by render() to setup colors used in fragment shader:

void VisDataObject::setContextColors( const bool isPoint,
                                      /*const*/ LookupTableWidget* const lut,
                                      RenderingContextRSIGBase * context ) {

  assert( context ); assert( lut );

  if ( isVisible() ) {
    const GeoDataObject* const thisParent =
      qobject_cast<const GeoDataObject*>( parent() );

    if ( thisParent ) {
      const int texID = thisParent->lutTextureID();
      context->setLutToTextureChannel0( texID );
      std::vector<float> myValues( lut->getValues() );
      context->setLutValues( myValues );

      if ( isPoint ) {
        context->setIsPointUniformParameter( 1 );
      } else { // 0 = Avoid texture2D() in fragmentShader:
        context->setIsPointUniformParameter( 0 );
        setPointSpriteTexture( 0, context );
      }

      // Copy RGBA palette to separate RGB float arrays to replace 1D texmap:

      const std::vector<Vector4f>& myPalette = lut->colorPalette();
      const size_t count = myPalette.size();
      std::vector<float> myPaletteR( count ); //x
      std::vector<float> myPaletteG( count ); //x
      std::vector<float> myPaletteB( count ); //x

      for ( size_t index = 0; index < count; ++index ) {
        const Vector4f& value = myPalette[ index ];
        myPaletteR[ index ] = value.x;
        myPaletteG[ index ] = value.y;
        myPaletteB[ index ] = value.z;
      }

      context->setColorPaletteR( myPaletteR );
      context->setColorPaletteG( myPaletteG );
      context->setColorPaletteB( myPaletteB );
    }
  }
}

