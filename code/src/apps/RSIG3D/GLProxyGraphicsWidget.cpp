/******************************************************************************
PURPOSE: GLProxyGraphicsWidget.C - Implements a full-window proxy widget for QGraphicsWidget.

NOTES: Implemented using QT's QWidget class.

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#if _MSC_VER
#include <qt_windows.h>
#endif

#include <QtOpenGL>
#include <QDebug>

#include "OpenGLScene.h"
#include "GLProxyGraphicsWidget.h"

/*!
 \class GLProxyGraphicsWidget
 
 \brief A dummy class to represent a OpenGL scene graph as a QGraphicsItem. 
 
 For mixed rendering of OpenGL based 3D objects and QPainter based 2D objects, 
 the 3d objects should be placed into the QGraphicsScene. 
 Therefore, a QGraphicsItem should be created in the scene graph. 
 The item is shown in full-screen. When the rendering function of this item is called, 
 the 3D objects in it are then rendered using OpenGL APIs. 
 This is the technique used in RSIG3D for mixed rendering.
 
 This method is not working well. Instead, the GLProxyGraphicsItem class is used. 
 
 \sa QGraphicsView, QGraphicsScene, QGraphicsItem, GLProxyGraphicsItem
 */


GLProxyGraphicsWidget::GLProxyGraphicsWidget(OpenGLScene* sceneptr, QWidget* parent)
  :  QWidget(parent), mScenePtr(sceneptr) {
  //!!set opacity seems to remove text rendering artifact in Windows OS
  this->setWindowOpacity(0.999f);
  this->setAutoFillBackground(false);
  this->setAttribute(Qt::WA_TranslucentBackground);
}



/*!
 \fn void GLProxyGraphicsWidget::paintEvent(QPaintEvent *event)

 Reimplement the paint event.
 */

void GLProxyGraphicsWidget::paintEvent( QPaintEvent* ) {

  if (mScenePtr) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);
    mScenePtr->draw2DScene(&painter);
    mScenePtr->draw2DSceneLegend(&painter);
  }
}


/*!
 \fn void GLProxyGraphicsWidget::render(QPainter * painter, const QPoint & targetOffset, const QRegion & sourceRegion, RenderFlags renderFlags )

 Reimplement the rendering function.
 */

void GLProxyGraphicsWidget::render( QPainter* painter,
                                    const QPoint&,
                                    const QRegion&,
                                    RenderFlags ) {

  if (mScenePtr) {
    mScenePtr->draw3DScene(painter);
  }
}

