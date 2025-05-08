/******************************************************************************
PURPOSE: VisDataObject.h - Defines the VisDataObject class

NOTES: Implemented using QT's QObject class

HISTORY: 2013/02/25, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#ifndef INC_VISDATAOBJECT_H_
#define INC_VISDATAOBJECT_H_

#include <QString>
#include <QObject>
#include <QList>
#include <QRect>
#include <QRectF>
#include <QDateTime>
#include <QDate>

#include "vector2f.h"

class QPainter;
class RenderingContextRSIGBase;
class LookupTableWidget;

class VisDataObject: public QObject {
  Q_OBJECT

public:

  Q_PROPERTY(bool showObject READ showObject WRITE setShowObject DESIGNABLE true USER true)
  bool showObject() const;
  void setShowObject(bool flag);
    
  Q_PROPERTY(qreal opacity READ alpha WRITE setAlpha DESIGNABLE true USER true)
  qreal alpha() const;
  void setAlpha(qreal alpha);
    
  Q_PROPERTY(bool showCellBoundary READ showCellBoundary WRITE setShowCellBoundary DESIGNABLE true USER true)
  bool showCellBoundary() const;
  void setShowCellBoundary(bool flag);

  Q_PROPERTY(qreal threshold READ threshold WRITE setThreshold DESIGNABLE true USER true)
  qreal threshold() const;
  void setThreshold(qreal threshold);

  Q_PROPERTY(qreal thresholdSliderValue READ thresholdSliderValue WRITE setThresholdSliderValue DESIGNABLE true USER true)
  int thresholdSliderValue() const;
  void setThresholdSliderValue(int thresholdSliderValue);

  Q_PROPERTY(QString dataMenuName READ getDataMenuName WRITE setDataMenuName DESIGNABLE true USER true)
  QString getDataMenuName() const;
  void setDataMenuName(QString name);

  bool isDefaultRangeChecked() const;
  void setDefaultRangeChecked(bool flag);

  //qreal minval() const;
  //qreal maxval() const;
  //qreal lastMinval() const;
  //qreal lastMaxval() const;
  double minval() const;
  double maxval() const;
  double lastMinval() const;
  double lastMaxval() const;
  
  int curColormapIndex() const;
  int CMAQRow() const;
  int CMAQCol() const;
  int CMAQLayer() const;
  int MaxCMAQRow() const;
  int MaxCMAQCol() const;
  int MaxCMAQLayer() const;
  bool ShowCMAQRow() const;
  bool ShowCMAQCol() const;
  bool ShowCMAQLayer() const;
  bool minValChangedThisSession() const;
  bool maxValChangedThisSession() const;
  bool ShowThreshold() const;
  QString getAggregate() const;

  void setMinval(double minval);
  void setMaxval(double maxval);
  void setLastMinval(double minval);
  void setLastMaxval(double maxval);
  void setMinValChangedThisSession(bool flag);
  void setMaxValChangedThisSession(bool flag);
  void setCurColormapIndex(int curColormapIndex);
  void setCMAQRow(int row);
  void setCMAQCol(int col);
  void setCMAQLayer(int layer);
  void setMaxCMAQRow(int maxRow);
  void setMaxCMAQCol(int maxCol);
  void setMaxCMAQLayer(int maxLayer);
  void setShowCMAQRow(bool flag);
  void setShowCMAQCol(bool flag);
  void setShowCMAQLayer(bool flag);
  void setShowThreshold(bool flag);
  void setAggregate(QString aggregate);

public:

  const static int POINTSPRITE_TEXBUFF_SIZE = 8;
  static int mPointSpriteTexID[POINTSPRITE_TEXBUFF_SIZE]; //OpenGL texture ID buffer for images on point rendering
                                                          //0: square, 1: circle, 2: triangle

  VisDataObject(QObject * parent = 0);

  virtual ~VisDataObject();
  virtual Vector2f minmaxVal() const;
  virtual Vector2f minmaxData() const;
  virtual bool isVisible() const;
  virtual bool isLookupTableWidget() const = 0;
  virtual bool isWidgetOfType(QString inquiry) const;
  virtual bool is3DObject() const = 0;
  virtual void render(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context, bool flagPopupPlot) = 0;
  virtual void renderEMVL(QPainter*, const QRect&, bool) {};
  virtual void renderDisplayPanel(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * contextt) = 0;
  virtual void render2DCalipsoCurtain(QPainter*, const QRect&) {}
  virtual void renderPopupNeubrew(QPainter*, const QRect&, bool) {};
  virtual void renderPopupCeilometer(QPainter*, const QRect&, bool) {};
  virtual void renderPopupPandora(QPainter*, const QRect&, bool) {};
  virtual QString description() const = 0;

 Q_SIGNALS:
  void valueChanged();

public Q_SLOTS:
    
protected:

  void setContextColors( const bool isPoint,
                         /*const*/ LookupTableWidget* const lut,
                         RenderingContextRSIGBase* const context );

  bool mShowObject;        //show on/off
  float mAlpha;            //alpha value of the polygon mesh data
  float mThreshold;
  int mThresholdSliderValue;
  bool mShowCellBoundary;
  bool mShowThreshold;
  bool mIsDefaultRangeChecked;
  float mMinval;
  float mMaxval;
  float mLastMinval;
  float mLastMaxval;
  bool mMinValChangedThisSession;
  bool mMaxValChangedThisSession;
  int  mCurColormapIndex;
  bool mShowCMAQRow;
  bool mShowCMAQCol;
  bool mShowCMAQLayer;
  int mCMAQRow;
  int mCMAQCol;
  int mCMAQLayer;
  int mMaxCMAQRow;
  int mMaxCMAQCol;
  int mMaxCMAQLayer;
  QString mAggregate;
  QString mDataMenuName;

 private:
  Q_DISABLE_COPY(VisDataObject);
};

// Global function called by child classes and RSIGScene.cpp:

void setPointSpriteTexture(int mode, RenderingContextRSIGBase *context);

#endif  //INC_VISDATAOBJECT_H_

