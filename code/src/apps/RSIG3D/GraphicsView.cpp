/******************************************************************************
PURPOSE: GraphicsView.C - A view class inherited from QT's QGraphicsView class.
The main usage is to have layered rendering: background, middle, and foreground
layers. The back/fore layers are QT widgets, while the middle layer is an OpenGL
scene graph.

NOTES: The whole viewport of this widget is based on QT's QGLWidget, so OpenGL
native rendering APIs can be applied without problems.

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <QGraphicsView>  //parent class
#include <QGLWidget>        //QGLWidget class
#include <QWheelEvent>      //QWheelEvent class
#include <QDebug>           //qDebug()
#include <QMouseEvent>      //QMouseEvent class
#include "RSIGScene.h"
#include "GraphicsView.h"
#include <QDebug>     

/*!
\externalpage http://doc.qt.digia.com/qq/qq26-openglcanvas.html
\title Accelerate your Widgets with OpenGL
*/

/*!
 \class GraphicsView
 \brief The GraphicsView class is the view class for rendering the scenegraph 
 for the RSIG3D application, e.g. the viewer in the Model-View-Controller model. 
 
 \ingroup  
 
 The GraphicsView class owns a scenegraph pointer of type RSIGScene. 
 It deletes the pointer when the class GraphicsView is destroyed itself. 
 This is normal, since it is derived from the QGrahicsView class. 
 It automatically calls the rendering and resizing function of RSIGScene.
 
 An important feature implemented in this class is that it is capable of 
 mixed rendering of OpenGL objects and QT's widgets. This makes a more powerful 
 UI possible, where 2D control widgets and 3D scene can be integrated together 
 in the scene. 
  
 \section1 Mixed rendering of OpenGL objects and QT widgets
 Qt 4.4 introduced a powerful feature to allow any QWidget subclass to be put into QGraphicsView. 
 It is now possible to embed ordinary widgets in a QGLWidget, 
 a feature that has been missing in Qt's OpenGL support for some time. 
 In a recent article \l {Accelerate your Widgets with OpenGL},
 it is shown how to use QGraphicsView to place widgets 
 in a scene rendered with standard OpenGL commands 
 
 \sa OpenGLScene, QGraphicsView
 */

/*! 
 This function constructs a RSIGScene scenegraph. 
 It also associate the viewport of the view with the viewport of a QGLWidget instance.
 In this way, the viewer now contains an OpenGL context, which can be used for rendering
 3D objects using OpenGL APIs.
 */
GraphicsView::GraphicsView( QWidget * parent, QGraphicsScene * sceneptr)
  : QGraphicsView( parent ),
  mGLWidget(QGLFormat(QGL::DoubleBuffer|QGL::Rgba|QGL::DepthBuffer)) {

  try {
    this->setAutoFillBackground(false);
    this->setMouseTracking(true);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setViewport(&mGLWidget);

    this->setScene(sceneptr);
    typedef RSIGScene SCENETYPE;
    SCENETYPE *rsigscene = qobject_cast<SCENETYPE*>(sceneptr);

    if (rsigscene != NULL) {
      connect(rsigscene, SIGNAL(boundingGeoBoxChanged(const QRectF &)), this, SIGNAL(boundingGeoBoxChanged(const QRectF &)));
    }
  } catch (...) {
    qDebug()<<"Error in new GraphicsView class: file "<<__FILE__<<" line: "<<__LINE__;
  }
}



/*!
 In the destruction process, this function does not deletes the RSIGScene scenegraph pointer, 
 since the scene graph is constructed from a global one. 
 Therefore, there are no problems for destructing derived classes.
 */
GraphicsView::~GraphicsView() {
  this->setScene(NULL);
}



/*!
 \fn QGLWidget * GraphicsView::getQGLlWidget()

 Returns the instance of the QGLWidget class member in this class.
 */
QGLWidget* GraphicsView::getQGLlWidget() {
  return &mGLWidget;
}



/*!
 \fn Marble::MarbleWidget * GraphicsView::marbleWidget()
 
 Returns the instance of the MarbleWidget object in the scenegraph of this class.
 */
Marble::MarbleWidget * GraphicsView::marbleWidget() {
  Marble::MarbleWidget *ptr = NULL;
  RSIGScene *scene = qobject_cast<RSIGScene*>(this->scene());

  if (scene) {
    ptr = scene->marbleWidget();
  }

  return ptr;
}



/*!
 \fn void GraphicsView::onMapServerChanged(bool useMarbleWidget, const char *theme)
 
 Change the map server and its theme. 
 The first parameter \e useMarbleWidget indicates whether this is a MarbleWidget theme.
 The second parameter \e theme is the name of the theme.
 */
void GraphicsView::onMapServerChanged(bool, const char* theme) {
  RSIGScene* rsigscene = qobject_cast<RSIGScene*>(this->scene());

  if (rsigscene) {
    rsigscene->setMapServer(theme);
    update();
  }
}



/*!
 \fn void GraphicsView::onMapProjectionChanged(int mode)
 
 Change the map projection mode. The avaible \e mode values can be found in RSIGState::MapProjectionType .
 */
void GraphicsView::onMapProjectionChanged(int mode) {
  RSIGScene* rsigscene = qobject_cast<RSIGScene*>(this->scene());

  if (rsigscene) {
    rsigscene->setMapProjection(mode);
    update();
  }
}



/*!
 \fn void GraphicsView::onBoundingGeoBoxChanged(const QRectF& rectf)
 
 Process the geometry bounding box change. 
 The main response is to change the LonlatBoundingBoxWidget instance.
 */
void GraphicsView::onBoundingGeoBoxChanged(const QRectF& rectf) {
  RSIGScene* rsigscene = qobject_cast<RSIGScene*>(this->scene());

  if (rsigscene) {
    rsigscene->setBoundingGeoBox(rectf);
    update();
  }
}



/*!
 \fn void GraphicsView::resizeEvent(QResizeEvent *)
 
 Pass the resize event to the scenegraph.
 */
void GraphicsView::resizeEvent(QResizeEvent* event)  {
  RSIGScene* this_scene = dynamic_cast<RSIGScene*> (scene());

  if (this_scene) {
    this_scene->resizeEvent(event);
  }

  QGraphicsView::resizeEvent(event);
  update();
}



/*!
 \fn void GraphicsView::keyPressEvent(QKeyEvent *)
 
 Pass the resize event to the scenegraph.
 */
void GraphicsView::keyPressEvent(QKeyEvent* e) {
  RSIGScene *rsigscene = qobject_cast<RSIGScene*>(this->scene());

  if (!rsigscene) {
    QGraphicsView::keyPressEvent(e);
    return;
  }

  const int N = 10;

  switch(e->key()) {
  case Qt::Key_A:

    for (int i=0; i<N; i++) {
      rsigscene->moveLeft();
    }

    break;
  case Qt::Key_D:

    for (int i=0; i<N; i++) {
      rsigscene->moveRight();
    }
    break;
  case Qt::Key_PageUp:
  case Qt::Key_W:

    for (int i=0; i<N; i++) {
      rsigscene->moveUp();
    }

    break;
  case Qt::Key_PageDown:
  case Qt::Key_X:

    for (int i=0; i<N; i++) {
      rsigscene->moveDown();
    }

    break;
  case Qt::Key_Up:
    rsigscene->moveUp();
    break;
  case Qt::Key_Down:
    rsigscene->moveDown();
    break;
  default:
    QGraphicsView::keyPressEvent(e);
    return;
  }

  update();
}



/*!
 \fn void GraphicsView::wheelEvent ( QWheelEvent * )
 
 Pass the resize event to the scenegraph.
 */
void GraphicsView::wheelEvent( QWheelEvent* ) {
}



