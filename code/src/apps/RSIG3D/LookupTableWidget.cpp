/******************************************************************************
PURPOSE: LookupTableWidget.cpp - several widget classes to encapsulate the LookupTable class

NOTES:   based on QT's QWidget class

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <assert.h>
#include <math.h>
#include <float.h>
#include <iostream>
#include <fstream>
#include <vector>

#if _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif

#include <GL/glew.h>

#include <QPainter>
#include <QDebug>

#include <Dataset.h>

#include "RSIGGlobal.h"
#include "GeoDataObject.h"
#include "LookupTableWidget.h"
#include "RSIGScene.h"

using namespace std;

//helper class

void drawHorizontalLookupTable( QPainter* painter,
                                const QRect& rect,
                                const std::vector<Vector4f>& colors,
                                const std::vector<float>& values,
                                const QStringList* slanttexts,
                                const QString& title,
                                const QString& /* timestep */,
                                const int tickStep ) {

  const int leftmargin = 10;
  const int rightmargin = 10;
  int topmargin = 35;
  const int barheight = 12;

  if (slanttexts) {
    topmargin+=26;
  }

  const int x0 = rect.x() + leftmargin;
  // Leave space for title at top
  int yOffset = -48;
  int titlespace = 20;
  const int y0 = rect.y() + topmargin + titlespace +yOffset;
  const int x1 = rect.x() + rect.width() - rightmargin;
  const int y1 = y0 + barheight;
  const int w = (x1 - x0);
  const int h = barheight;

  float posbuffer[1024];
  float t0 = 0;

  for (unsigned int i=0; i<colors.size(); i++) {
    const float t1 = (i+1) * float(w)/colors.size();
    const float K = 255.0f;
    const Vector4f c = colors[i]*K;
    QBrush brush(QColor(c.x, c.y, c.z, c.w*0.99));
    float bposx = x0 + t0;
    float bwidth = (t1-t0);
    const QRectF rectf(bposx, y0, bwidth, h);
    painter->fillRect(rectf, brush);
    posbuffer[i]=bposx; //store position for drawing text
    t0 = t1;
  }

  // set up default font
  QFont font("Helvetica", 8, QFont::Normal);
  int  pointsize_tiny   = 9;
  int  pointsize_small  = 10;
  int  pointsize_large  = 16;

  //draw a framebox for the color bars
  QPen whitepen(QColor(0, 0, 0, 250));
  painter->setPen(whitepen);
  painter->drawRect(QRect(x0, y0, w, h));

  //draw the values as legend

  font.setPixelSize(pointsize_small);
  font.setBold(false);
  painter->setFont(font);
  painter->rotate(90);
  posbuffer[colors.size()] = x1;
        
  for (unsigned int i=0; i<=colors.size(); i+=tickStep) {
    float bposx = posbuffer[i];
    QString text; text.setNum(values[i], 'g', 3);
    painter->drawText(QPoint(y1+4, -(bposx-4)), text);
  }

  painter->rotate(-90);

  //draw the title of the widget

  if (1) {

    if (slanttexts && slanttexts->size() > 0) {
      titlespace += 10;
    }

    if (title.length() < 60) {
      font.setPixelSize(pointsize_small);
    } else {
      font.setPixelSize(pointsize_tiny);
    }

    font.setBold(false);
    painter->setFont(font);
    // Adjust title to be above colorbar
    QRect box(x0-20, y0-titlespace, w+40, h+8);
    painter->drawText(box, Qt::AlignHCenter, title);
  }

  //draw the timestep of the widget
  font.setPixelSize(pointsize_large);
  font.setBold(true);
  painter->setFont(font);

  //draw the slant notes

  if (slanttexts && slanttexts->size() > 0) {
    font.setPixelSize(pointsize_tiny);
    font.setBold(false);
    painter->setFont(font);
    const float slantangle = -10;
    QStringList const & stlist = *slanttexts;
    const int loopsize = std::min(int(stlist.size()), int(values.size()));

    for (int i=0; i<loopsize; i++) {

      if (stlist[i].length()) {
        const float xx = posbuffer[i];
        const float yy = y0;
        painter->translate(xx, yy);
        painter->rotate(slantangle);
        painter->drawText(QPoint( 0,  0), stlist[i]);
        painter->rotate(-slantangle);
        painter->translate(-xx, -yy);
      }
    }
  }
}



//================================================================
/*!
 \class LookupTableWidget
 
 \brief The class for rendering a lookup table.
 */

LookupTableWidget::LookupTableWidget(QObject *parent) :
  VisDataObject(parent)
  ,LookupTable()
  ,mOrientation(Horizontal)
  ,mTickStep(2)
  ,mInterpolation(false) {
}



LookupTableWidget::~LookupTableWidget() {
}



bool LookupTableWidget::interpolation() const {
  return mInterpolation;
}



void LookupTableWidget::setInterpolation(bool flag) {

  if (mInterpolation != flag) {
    mInterpolation = flag;
    this->rebuildPalette(flag);
    this->updateTexture();
    Q_EMIT valueChanged();
  }
}



void LookupTableWidget::setOrientation ( Orientation r ) {
  mOrientation = r;
}



int LookupTableWidget::orientation () const {
    return mOrientation;
}



QString LookupTableWidget::timestepString() const {
  QString strTimestep;
  GeoDataObject *object = qobject_cast<GeoDataObject*>(parent());

  if (object!=NULL) {
    emvl::Dataset* data = object->model();

    if ( data ) {
      const unsigned int timesteps = data->timesteps();
      const unsigned int gTime = RSIGStateSingleton::getInstance()->mCounter;
      const unsigned int tm = gTime % timesteps;

      emvl::Timestamp startTime = data->startingTimestamp();
      startTime.offsetHours(tm);

      emvl::Timestamp::UTCString result;
      startTime.toUTCString( result );

      result[16]=0;
      strTimestep = QString(result);
    }
  }

  return strTimestep;
}



void LookupTableWidget::paint(QPainter* painter, const QRect& rect) {

  if (!isVisible()) {
    return;
  }

  const QString timestep = this->timestepString();
        
  if (mOrientation == Horizontal) {
    drawHorizontalLookupTable(painter, rect, mColors, mValues, &mSlantText, title(), timestep, mTickStep);
  } else {
    drawHorizontalLookupTable(painter, rect, mColors, mValues, NULL, title(), timestep, 2);
  }
}



void LookupTableWidget::paintEvent ( QPaintEvent* ) {
}



bool LookupTableWidget::isLookupTableWidget() const {
  return true;
}



bool LookupTableWidget::isWidgetOfType(QString inquiry) const {
  const GeoDataObject* const par = qobject_cast<GeoDataObject*>(parent() );
  return par->varName().contains( inquiry, Qt::CaseInsensitive );
}



bool LookupTableWidget::is3DObject() const {
  return false;
}



void LookupTableWidget::render(QPainter* painter,
                               const QRect& rect,
                               RenderingContextRSIGBase*,
                               bool ) {
  this->paint(painter, rect);
}


void LookupTableWidget::renderDisplayPanel(QPainter* painter,
                               const QRect& rect,
                               RenderingContextRSIGBase*) {
  this->paint(painter, rect);
}




void LookupTableWidget::renderEMVL(QPainter* painter, const QRect& rect, bool ) {
  this->paint(painter, rect); 
}




void LookupTableWidget::render2DCalipsoCurtain(QPainter*, const QRect& ) {
}



void LookupTableWidget::renderPopupNeubrew(QPainter*, const QRect&, bool ) {
}



void LookupTableWidget::renderPopupCeilometer(QPainter*, const QRect&, bool ) {
}



void LookupTableWidget::setTitle(const QString& title) {
  mTitle = title;
}



void LookupTableWidget::setSlantText(const QStringList& slantText) {
  mSlantText = slantText;
}



void LookupTableWidget::setTickStep(const int tickStep) {
  mTickStep = tickStep;
}



QString LookupTableWidget::title() const {
  return mTitle;
}

QString LookupTableWidget::description() const {
  return "lut";
}





