/******************************************************************************
PURPOSE: PolygonRenderer.h - A class for rendering polygon datasets.

NOTES:   Implementation is based on VisDataObject class.

HISTORY: 2013/5/4, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
 ******************************************************************************/

#ifndef INC_POLYGONRENDERER_H
#define INC_POLYGONRENDERER_H

#include <vector> // For std::vector.

#include <QColor>
#include "vector2f.h"
#include "vector4f.h"
#include "vector4i.h"
#include "VisDataObject.h"

class QObject;
class QPainter;
class RenderingContextRSIGBase;

class PolygonRenderer: public VisDataObject {

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

  const static int BUFFSIZE = 512;

public:
  PolygonRenderer(QObject * parent = 0);

  virtual ~PolygonRenderer();

  virtual bool isLookupTableWidget() const;

  virtual  bool isWidgetOfType(QString inquiry) const;

  virtual bool is3DObject() const;

  virtual void render(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context, bool flagPopupPlot);

  virtual void renderEMVL(QPainter * painter, const QRect & rect, bool flagPopupPlot);
  virtual void renderDisplayPanel(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context);
  virtual void render2DCalipsoCurtain(QPainter *painter, const QRect& rect);
  virtual void renderPopupNeubrew(QPainter *painter, const QRect& rect, bool haveCalipso);
  virtual void renderPopupCeilometer(QPainter *painter, const QRect& rect, bool haveCalipso);

  void drawCalipsoCurtain(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context);
  void drawCalipsoAxesOpengl(const QRect & rect);
  void initCalipsoCurtainPlotarea(const QRect & rect);

  virtual QString description() const;

protected:
  void drawQuadArray( QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context );

private:
  static Vector4i mElements[BUFFSIZE];
  std::vector<Vector4f> tridata;
  // Buffer copy of 32-bit <lon,lat,elv,dat> for triangle strip rendering as
  // Required in 2D rendering using the shader program.
  float mLineWidth;        //line width of each polygon
  QColor mLineColor;       //line color of each polygon
  bool mShowNullSample;

};

#endif //INC_POLYGONRENDERER_H
