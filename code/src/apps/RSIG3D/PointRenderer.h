/******************************************************************************
PURPOSE: PointRenderer.h - A class for rendering point datasets.

NOTES:   Implementation is based on VisDataObject class.

HISTORY: 2013/4/11, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef INC_POINTRENDERER_H
#define INC_POINTRENDERER_H

#include <QColor>
#include "vector2f.h"
#include "VisDataObject.h"

class QObject;
class QPainter;
class RenderingContextRSIGBase;


class PointRenderer: public VisDataObject {

  Q_OBJECT

public:

  Q_PROPERTY(int pointShape READ pointShape WRITE setPointShape DESIGNABLE true USER true)
  int pointShape() const;
  void setPointShape(int shape);

  Q_PROPERTY(qreal pointSize READ pointSize WRITE setPointSize DESIGNABLE true USER true)
  float pointSize() const;
  void setPointSize(float r);

  Q_PROPERTY(qreal lineWidth READ lineWidth WRITE setLineWidth DESIGNABLE true USER true)
  float lineWidth() const;
  void setLineWidth(float w);

  Q_PROPERTY(QColor pointColor READ pointColor WRITE setPointColor DESIGNABLE true USER true)
  QColor pointColor() const;
  void setPointColor(const QColor& color);

  Q_PROPERTY(bool showNullSample READ showNullSample WRITE setShowNullSample DESIGNABLE true USER true)
  bool showNullSample() const;
  void setShowNullSample(bool flag);

  Q_PROPERTY(bool useOpenglPointSmooth READ useOpenglPointSmooth WRITE setUseOpenglPointSmooth DESIGNABLE true USER true)
  bool useOpenglPointSmooth() const;
  void setUseOpenglPointSmooth(bool flag);

public:
  PointRenderer(QObject * parent = 0);

  virtual ~PointRenderer();

  virtual bool isLookupTableWidget() const;

  virtual  bool isWidgetOfType(QString inquiry) const;

  virtual bool is3DObject() const;

  virtual void render(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context, bool flagPopupPlot);

  virtual void renderEMVL(QPainter * painter, const QRect & rect, bool flagPopupPlot);
  virtual void renderDisplayPanel(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context);
  virtual void render2DCalipsoCurtain(QPainter *painter, const QRect& rect);
  virtual void renderPopupNeubrew(QPainter *painter, const QRect& rect, bool haveCalipso);
  virtual void renderPopupCeilometer(QPainter *painter, const QRect& rect, bool haveCalipso);
  virtual void renderPopupPandora(QPainter *painter, const QRect& rect, bool haveCalipso);
  void drawCCAPopup(QString popupType);

  virtual QString description() const;

protected:
  void drawPoints( RenderingContextRSIGBase * context );

  void drawQuadArray( RenderingContextRSIGBase * context );

private:
  int mPointShape;
  float mPointSize;
  float mLineWidth;
  QColor mPointColor;
  bool mShowNullSample;
  bool mUseOpenglPointSmooth;
};


#endif //INC_POINTRENDERER_H
