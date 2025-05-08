/******************************************************************************
PURPOSE: OpenGLScene.C - Implements openglscene.

NOTES: Implemented using QT's QGraphicsScene class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#if _MSC_VER
#include <qt_windows.h>
#endif

//#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <QtOpenGL>
#include <QPushButton>
#include <QDebug>

#include "OpenGLScene.h"
#include "GLProxyGraphicsItem.h"
#include "GLProxyGraphicsWidget.h"


/*!
 \class OpenGLScene
 \brief The OpenGLScene class provides the base scenegraph for the RSIG3D application. 
 
 \ingroup  
 
 The RSIG3D application requires a complicated scene graph system. Among the scene objects, 
 there should be some control widgets, background objects, and foreground objects. 
 Some of them are 2D objects, while some others may be 3D objects. 
 The 2D objects can be drawn in QT's own painting functions, 
 while the 3D objects are drawn directly using OpenGL 3D APIs.  
 The OpenGLScene class provides a base class  for orgnizing these objects.
 For 2D objects, they are directly represented as QGraphicsItem into the QGraphicsScene. 
 For 3D objects, a proxy QGraphicsItem is created, which is a full-window sized. 
 All 3D rendering is in this proxy item.
 
 In general, the rendering order of the items in the scene graph is:
 
 \list
   \o the items contained in the \e QGraphicsScene 
   \o the 3D objects \c draw3DScene()
   \o the 2D objects \c draw2DScene()
   \o the 2D objects \c draw2DSceneLegend()
 \endlist
 
 \section1 Section 1
 
 \section2 Section2 
 
 \sa RSIGScene, QGraphicsScene
 */


const int GL_WIN_WIDTH=64;
const int GL_WIN_HEIGHT=64;




OpenGLScene::OpenGLScene(): QGraphicsScene() 
  ,mProxySceneItem(0)
  ,mProxyGrphicsWidget(0)
  ,mMinZDepthValue(1000)
  ,mRectf(0, 0, GL_WIN_WIDTH, GL_WIN_HEIGHT) {

  try {
    mProxySceneItem = new GLProxyGraphicsItem(0); //x

    if (mProxySceneItem) {
      mProxySceneItem->setZValue(-100.0);
      mProxySceneItem->setOpacity(0.90);
      addItem(mProxySceneItem);
      mProxySceneItem->setPos(0, 0);
    }

    GLProxyGraphicsWidget* p = 0;
    p = new GLProxyGraphicsWidget(this, 0); //x
    QGraphicsWidget *proxy = this->addWidget(p);
    proxy->setAutoFillBackground(false);
    proxy->setZValue(-100);
    mProxyGrphicsWidget = proxy;
  } catch (...) {
    qDebug()<<"Memory exception: new OpenGLScene(), at File: "<<__FILE__<<", line "<<__LINE__<<"\n";
    throw;
  }
}



OpenGLScene::~OpenGLScene() {

  try {

    if (mProxySceneItem != NULL) {
      delete mProxySceneItem;
    }

    if (mProxyGrphicsWidget != NULL) {

      if (mProxyGrphicsWidget->parent() == NULL) {
        delete mProxyGrphicsWidget;
      }
    }
  } catch (...) {
    qDebug()<<"Error: exception in OpenGLScene destructor!";
  }
}



/*!
 \fn GLProxyGraphicsItem* OpenGLScene::glSceneItem() const
 
 Returns OpenGL proxy item pointer
 */
GLProxyGraphicsItem* OpenGLScene::glSceneItem() const {
  return mProxySceneItem;
}



/*!
 \fn float OpenGLScene::reservedMinZDepth() const
 
 Returns the reserved minimal value of the z-depth value
 */
float OpenGLScene::reservedMinZDepth() const {
    return mMinZDepthValue;
}



/*!
 \fn float OpenGLScene::reservedMaxZDepth() const
 
 Returns the reserved maximal value of the z-depth value
 */
float OpenGLScene::reservedMaxZDepth() const  {
  return 0;
}



/*!
 \fn QRectF OpenGLScene::storedSceneRect() const 
 
 The scenegraph itself seems to have no place to store its rect size. 
 We use a member variable here to store the QRectF passed from the 
 resizing event.
 */
QRectF OpenGLScene::storedSceneRect() const {
  return mRectf;
}



/*!
 \fn void OpenGLScene::resizeEvent(QResizeEvent *event) 
 
 Resizes the scenegraph. In the derived class of OpenGLScene, 
 there is no need to worry about the resizing event, unless additional
 widgets/items are introduced into the scenegraph.
 */
void OpenGLScene::resizeEvent(QResizeEvent* event)  {
  const QSize s = event->size();

  if (mProxySceneItem) {
    mProxySceneItem->setPos(0, 0);
  }

  if (mProxyGrphicsWidget) {
    mProxyGrphicsWidget->resize(s);
    mProxyGrphicsWidget->setPos(0, 0);
  }

  mRectf = QRectF(mRectf.x(), mRectf.y(), s.width(), s.height());
  setSceneRect(mRectf);
  update();
}


/*!
 \fn void OpenGLScene::drawBackground (QPainter *painter, const QRectF & rectf)
 
 Render to the screen the background. Its output has the lowest level.
 */
void OpenGLScene::drawBackground(QPainter* painter, const QRectF&) {
  const QPaintEngine::Type etype = painter->paintEngine()->type();

  if (!(etype== QPaintEngine::OpenGL || etype==QPaintEngine::OpenGL2)) {
    qWarning("OpenGLScene: needs a QGLWidget as viewport on the graphics view");
  } else {
    glClearColor(1, 1, 1, 0.0f);
    glClearDepth(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}



/*!
 \fn void OpenGLScene::draw2DScene (QPainter *painter)
 
 Render to the screen the 2D objects. Its output is only on top of the 3D rendering 
 \c draw3DScene().
 */
void OpenGLScene::draw2DScene (QPainter*) {
}



/*!
 \fn void OpenGLScene::draw2DSceneLegend (QPainter *painter)
 
 Render to the screen the 2D legends. Its output is on top of the 3D rendering 
 \c draw3DScene() and 2D rendering \c draw2DScene().
 */
void OpenGLScene::draw2DSceneLegend (QPainter*) {
}



/*!
 \fn void OpenGLScene::draw3DScene (QPainter *painter)
 
 In this function, it should draw all the 3D objects in OpenGL (virutal function). 
 The rendering output is below the output of \c draw2DScene() and \c draw2DSceneLegend().
*/
void OpenGLScene::draw3DScene(QPainter *painter) {
  painter->beginNativePainting();
  this->drawSphere(1);
  painter->endNativePainting();
}

void OpenGLScene::draw3DSceneEMVL(QPainter* painter) {
  painter->beginNativePainting();
  this->drawSphere(1);
  painter->endNativePainting();
}

void OpenGLScene::drawDisplayPanelSceneEMVL(QPainter* painter) {
  painter->beginNativePainting();
  painter->endNativePainting();
}



/*!
 \fn void OpenGLScene::drawSphere(qreal scale)

 Draw a sphere to leave a depth buffer footprint of the Earth. 
 */
void OpenGLScene::drawSphere(qreal) {
}



/*!
 \fn void OpenGLScene::checkPaintEngine (QPainter *painter)
 
 Output to the std::out the QPainter::type value for the input \a painter. 
 This function is for debug purpose to find some font rendering issues.
 */
void OpenGLScene::checkPaintEngine(QPainter* painter) {
  static int count = 0;

  if ( count != 0 ) {
    return;
  }

  count = 1;

  struct EngineTypeMessagePair{
    QPaintEngine::Type engineType;
    const char *message;
  };
  const EngineTypeMessagePair pairs[]={
    {QPaintEngine::X11, "X11"}
    ,{QPaintEngine::Windows, "Windows"}
    ,{QPaintEngine::QuickDraw, "QuickDraw"}
    ,{QPaintEngine::CoreGraphics, "CoreGraphics"}
    ,{QPaintEngine::QWindowSystem, "QWindowSystem"}
    ,{QPaintEngine::OpenGL, "OPENGL"}
    ,{QPaintEngine::OpenGL2, "OPENGL2"}
    ,{QPaintEngine::Raster, "Raster"}
  };

  const QPaintEngine::Type egtype = painter->paintEngine()->type();
  qWarning()<<"Paint Engine type value: "<<egtype<<"\n";

  QString msg("OpenGLScene: ");
  bool engineFound = false;

  for (unsigned int i=0; i<sizeof(pairs)/sizeof(pairs[0]); i++) {

    if (egtype==pairs[i].engineType) {
      msg += pairs[i].message;
      engineFound = true;
      break;
    }
  }

  if (!engineFound) {
    msg += "unknown";
  }

  msg+=" engine. OpenGL version: ";
  msg += (const char*) glGetString( GL_VERSION );
  qWarning()<<msg;
}



