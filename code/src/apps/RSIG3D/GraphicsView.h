/******************************************************************************
PURPOSE: GraphicsView.h - defines GraphicsView class

NOTES:   GraphicsView is based QGraphicsView, a 2D QT built-in scene graph class

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#ifndef GRAPHICSVIEW_H_
#define GRAPHICSVIEW_H_

#include <QGraphicsView>
#include <QGLWidget>

namespace Marble {
  class MarbleWidget;
  class MarbleNavigator;
};

class QKeyEvent;
class QWheelEvent;
class QResizeEvent;
class QGraphicsScene;

class GraphicsView : public QGraphicsView {
  Q_OBJECT

public:
  GraphicsView(QWidget *parent, QGraphicsScene * scene);

  virtual ~GraphicsView();

  Marble::MarbleWidget* marbleWidget();

  QGLWidget* getQGLlWidget();

Q_SIGNALS:

  void boundingGeoBoxChanged(const QRectF& rectf);

public Q_SLOTS:
  void onMapServerChanged(bool useMarbleWidget, const char *theme);
  void onMapProjectionChanged(int mode);
  void onBoundingGeoBoxChanged(const QRectF& rectf);

protected:
  virtual void resizeEvent(QResizeEvent *event);
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void wheelEvent ( QWheelEvent * event );
  QGLWidget mGLWidget;
};


#endif //GRAPHICSVIEW_H_


