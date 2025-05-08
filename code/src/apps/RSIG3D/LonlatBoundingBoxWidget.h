/******************************************************************************
PURPOSE: LonlatBoundingBoxWidget.h - A boundingbox widget for the interested Lon/Lat region.

NOTES:

HISTORY: 2013/3/31, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef INC_LONLATBOUNDINGBOXWIDGET_H_
#define INC_LONLATBOUNDINGBOXWIDGET_H_

#include <QWidget>       //QWidget desclare
#include <QDomElement>
#include <QScopedPointer>
#include "RSIGGlobal.h"
#include "OpenGLScene.h"

class Ui_BoundingBox;

class LonlatBoundingBoxWidget: public QWidget {
  Q_OBJECT

public:
  LonlatBoundingBoxWidget(QWidget *parent=0);
  ~LonlatBoundingBoxWidget();

Q_SIGNALS:
  void boundingGeoBoxChanged(const QRectF& );

public Q_SLOTS:
  void onBoundingGeoBoxChanged(const QRectF &rectf);

protected Q_SLOTS:
  void onBoundingGeoBoxChanged();

public:
  Ui_BoundingBox* ui;
};

#endif //INC_LONLATBOUNDINGBOXWIDGET_H_
