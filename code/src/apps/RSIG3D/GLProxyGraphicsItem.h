/******************************************************************************
PURPOSE: OpenGLScene.h - scenegraph class for mixing OpenGL scene and QGraphicsItem scene.

NOTES:   

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef GLPROXYGRAPHICSITEM_H_
#define GLPROXYGRAPHICSITEM_H_

#include <QGraphicsScene>
#include <QGraphicsRectItem>


class GLProxyGraphicsItem: public QGraphicsRectItem {
public:
  GLProxyGraphicsItem(QGraphicsItem * parent = 0);

  //a derived class of QGraphicsItem must override this function to provide its own rect size
  virtual QRectF boundingRect ( void ) const;

protected:
  //customized paint function must be provided
  virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

private:
  Q_DISABLE_COPY(GLProxyGraphicsItem);
};


#endif //GLPROXYGRAPHICSITEM_H_


