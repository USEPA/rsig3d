/******************************************************************************
PURPOSE: LonlatLonlatBoundingBoxWidgetWidget.cpp - Implements LonlatLonlatBoundingBoxWidgetWidget class.

NOTES:

HISTORY: 2013/3/31, zhang.nan@epa.gov, Created.
******************************************************************************/

#include "LonlatBoundingBoxWidget.h"
#include "ui_boundingbox.h"


/*!
 \class LonlatBoundingBoxWidget
 \brief The LonlatBoundingBoxWidget class provides a scenegraph for RSIG3D application. 
 
 The LonlatBoundingBoxWidget class provides a high-level programming interface for
 timers. To use it, create a QTimer, connect its timeout() signal
 to the appropriate slots, and call start(). From then on it will
 emit the timeout() signal at constant intervals.
 
 \sa OpenGLScene, QGraphicsScene
 */

LonlatBoundingBoxWidget::LonlatBoundingBoxWidget(QWidget *parent)
  : QWidget(parent), ui(0) {
  ui = new Ui_BoundingBox;
  ui->setupUi(this);
  connect(ui->northDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));
  connect(ui->southDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));
  connect(ui->eastDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));
  connect(ui->westDSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onBoundingGeoBoxChanged()));
}



LonlatBoundingBoxWidget::~LonlatBoundingBoxWidget() {
  delete ui;
}



void LonlatBoundingBoxWidget::onBoundingGeoBoxChanged(const QRectF &rectf) {
  double x0 = rectf.x();
  double x1 = x0+rectf.width();
  double y0 = rectf.y();
  double y1 = y0+rectf.height();

  if (x0>x1) {
    double t=x0;
    x0=x1; x1=t;
  }
  if (y0>y1) {
    double t=y0;
    y0=y1; y1=t;
  }

  const bool f = blockSignals(true);
  QDoubleSpinBox* widgets[4]={ui->northDSpinBox, ui->southDSpinBox, ui->eastDSpinBox, ui->westDSpinBox};
  double values[4]={y1, y0, x1, x0};

  for (int i=0; i<4; i++) {
    QDoubleSpinBox *w = widgets[i];
    double v = values[i];

    if (w->value()!=v) {
      w->setValue(v);
    }
  }

  blockSignals(f);
}



void LonlatBoundingBoxWidget::onBoundingGeoBoxChanged() {
  double x0 = ui->westDSpinBox->value();
  double x1 = ui->eastDSpinBox->value();
  double y0 = ui->southDSpinBox->value();
  double y1 = ui->northDSpinBox->value();

  if (x0>x1) {
    double t=x0;
    x0=x1;
    x1=t;
  }

  if (y0>y1) {
    double t=y0;
    y0=y1;
    y1=t;
  }

  const QRectF r(x0, y0, x1-x0, y1-y0);
  Q_EMIT boundingGeoBoxChanged(r);
}



