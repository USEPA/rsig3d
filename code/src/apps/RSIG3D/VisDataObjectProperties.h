/******************************************************************************
PURPOSE: VisDataObjectProperties.h - Defines the VisDataObject class

NOTES: Implemented using QT's QObject class

PURPOSE: Used to maintain coherence when new data is loaded (e.g. colormap, etc)

HISTORY: 2018/11/12, Matt Freeman, Created.
******************************************************************************/

#ifndef INC_VISDATAOBJECTPROPERTIES_H_
#define INC_VISDATAOBJECTPROPERTIES_H_

#include <QString>
#include <QObject>
#include <QList>
#include <QRect>
#include <QRectF>
#include <QDateTime>
#include <QDate>

#include "vector2f.h"


class VisDataObjectProperties: public QObject {
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

  bool isDefaultRangeChecked() const;
  void setDefaultRangeChecked(bool flag);

  double minval();
  double maxval();
  double lastMinval();
  double lastMaxval();
  int curColormapIndex();
  int CMAQRow();
  int CMAQCol();
  int CMAQLayer();
  int MaxCMAQRow();
  int MaxCMAQCol();
  int MaxCMAQLayer();
  bool ShowCMAQRow();
  bool ShowCMAQCol();
  bool ShowCMAQLayer();

  bool mMinValChangedThisSession;
  bool mMaxValChangedThisSession;

  bool ShowThreshold();
  QString name() const;

  void setMinval(double minval);
  void setMaxval(double maxval);
  void setLastMinval(double minval);
  void setLastMaxval(double maxval);
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

  void setName(QString name);

  void setMinValChangedThisSession(bool flag);
  void setMaxValChangedThisSession(bool flag);
  bool minValChangedThisSession();
  bool maxValChangedThisSession();

 public:
  VisDataObjectProperties(QObject * parent = 0);

  ~VisDataObjectProperties();

  Vector2f minmaxVal() const;
  Vector2f minmaxData() const;

  bool isVisible() const;
  bool isWidgetOfType(QString inquiry) const;

public Q_SLOTS:

protected:
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
  int   mCurColormapIndex;
  bool mShowCMAQRow;
  bool mShowCMAQCol;
  bool mShowCMAQLayer;
  int mCMAQRow;
  int mCMAQCol;
  int mCMAQLayer;
  int mMaxCMAQRow;
  int mMaxCMAQCol;
  int mMaxCMAQLayer;
  QString mName;

 private:
  Q_DISABLE_COPY(VisDataObjectProperties);
};

#endif  //INC_VISDATAOBJECTPROPERTIES_H_

