/******************************************************************************
PURPOSE: VectorRenderer.h - A class for rendering vector datasets.

NOTES:   Implementation is based on VisDataObject class.

HISTORY: 2018-08-20 plessel.todd@epa.gov based on PolygonRenderer.
STATUS:  unreviewed untested
******************************************************************************/

#ifndef VECTORRENDERER_H
#define VECTORRENDERER_H

#include <QColor>
#include "vector2f.h"
#include "vector4i.h"
#include "VisDataObject.h"

class QObject;
class QPainter;
class RenderingContextRSIGBase;

class VectorRenderer: public VisDataObject {

  Q_OBJECT

public:

  Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth DESIGNABLE true USER true)
  qreal lineWidth() const;
  void setLineWidth(qreal w);

  Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor DESIGNABLE true USER true)
  QColor lineColor() const;
  void setLineColor(const QColor& color);

  Q_PROPERTY(bool showNullSample READ showNullSample WRITE setShowNullSample DESIGNABLE true USER true)
  bool showNullSample() const;
  void setShowNullSample(bool flag);

public:
  VectorRenderer(QObject * parent = 0);

  virtual ~VectorRenderer();
  virtual bool isLookupTableWidget() const;
  virtual  bool isWidgetOfType(QString inquiry) const;
  virtual bool is3DObject() const;
  virtual void render(QPainter * painter, const QRect & rect,
                      RenderingContextRSIGBase * context, bool flagPopupPlot);
  virtual void renderEMVL(QPainter * painter, const QRect & rect, bool flagPopupPlot);
  virtual void renderDisplayPanel(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context);
  virtual QString description() const;

protected:
  void drawVectors( QPainter* painter, const QRect& rect,
                    RenderingContextRSIGBase* context );

private:
  float mLineWidth;        //line width of each vector
  QColor mLineColor;       //line color of each vector
  bool mShowNullSample;

};

#endif /* VECTORRENDERER_H */

