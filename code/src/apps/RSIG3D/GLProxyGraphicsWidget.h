/******************************************************************************
PURPOSE: OpenGLScene.h - scenegraph class for mixing OpenGL scene and QGraphicsItem scene.

NOTES:

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef GLPROXYGRAPHICSWIDGET_H_
#define GLPROXYGRAPHICSWIDGET_H_

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QWidget>

class OpenGLScene;

//========================Helper class====================================
//a dummy class to represent a OpenGL scene graph as a QGraphicsItem

class GLProxyGraphicsWidget: public QWidget {
  Q_OBJECT

public:
  GLProxyGraphicsWidget(OpenGLScene* sceneptr, QWidget * parent = 0);

  virtual void paintEvent(QPaintEvent *event);

  virtual void render(QPainter * painter, const QPoint & targetOffset, const QRegion & sourceRegion, RenderFlags renderFlags );

private:
  Q_DISABLE_COPY(GLProxyGraphicsWidget);
  OpenGLScene * mScenePtr;
};


#endif //GLPROXYGRAPHICSWIDGET_H_
