/******************************************************************************
PURPOSE: OpenGLScene.h - scenegraph class for mixing OpenGL scene and QGraphicsItem scene.

NOTES:

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef OPENGLSCENE_H_
#define OPENGLSCENE_H_

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QWidget>

class GLProxyGraphicsItem;  //a dummy class to represent a OpenGL scene graph as a QGraphicsItem   
class GLProxyGraphicsWidget;//a dummy class to represent a OpenGL scene graph as a QGraphicsItem


class OpenGLScene : public QGraphicsScene {
  Q_OBJECT

public:
  OpenGLScene();

  ~OpenGLScene();

  //OpenGLScene will never use zero as its depth
  float reservedMaxZDepth() const;

  //The reserved minimal value of the z-depth value
  float reservedMinZDepth() const;

  GLProxyGraphicsItem* glSceneItem() const;

  //return stored value, it is the same as the sceneRect function,
  //but provides the flexibility of allowing widget size to be the same as scene rect size
  QRectF storedSceneRect() const;

  virtual void draw3DScene (QPainter *painter);

  virtual void draw3DSceneEMVL (QPainter *painter);

  virtual void drawDisplayPanelSceneEMVL (QPainter *painter);

  virtual void draw2DSceneLegend (QPainter *painter);

  virtual void draw2DScene (QPainter *painter);

  static void checkPaintEngine(QPainter *painter);

protected:
  void drawBackground(QPainter *painter, const QRectF & rectf);

  void resizeEvent(QResizeEvent *event);

  void drawSphere(qreal scale);

private:
  Q_DISABLE_COPY(OpenGLScene);
  GLProxyGraphicsItem* mProxySceneItem;
  QGraphicsWidget *mProxyGrphicsWidget;
  float mMinZDepthValue;
  QRectF mRectf;
  QColor mBackgroundColor;
};

#endif //OPENGLSCENE_H_
