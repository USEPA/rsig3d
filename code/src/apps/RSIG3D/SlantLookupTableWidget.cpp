/******************************************************************************
PURPOSE: SlantLookupTableWidget.cpp - widget classes to encapsulate the LookupTable class

NOTES:   based on QT's QWidget class

HISTORY: 2012/11/11, zhang.nan@epa.gov, Created.
******************************************************************************/

#if _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif
#include <GL/glew.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <QPainter>
#include "SlantLookupTableWidget.h"

using namespace std;

//in file LookupTableWidget.cpp

extern void drawHorizontalLookupTable( QPainter* painter,
                                       const QRect& rect,
                                       const std::vector<Vector4f>& colors,
                                       const std::vector<float>& values,
                                       const QStringList* slanttexts,
                                       const QString& title,
                                       const QString& timestep,
                                       const int tickStep );

//================================================
/*!
 \class SlantTextLookupTableWidget
 
 \brief The class for rendering a slant lookup table.
 */
SlantTextLookupTableWidget::SlantTextLookupTableWidget(QObject* parent)
  : LookupTableWidget(parent), mTextList() {
}



SlantTextLookupTableWidget::~SlantTextLookupTableWidget() {
}



void SlantTextLookupTableWidget::setTextList(const QStringList& list) {
  mTextList = list;
}



void SlantTextLookupTableWidget::setTextList(const QString list[], int len) {
  mTextList.clear();

  for (int i=0; i<len; i++) {
    mTextList.append(list[i]);
  }
}



void SlantTextLookupTableWidget::paint(QPainter* painter, const QRect& rect) {

  if ( ! isVisible() ) {
    return;
  }

  const QString timestep = this->timestepString();

  if (mOrientation == Horizontal) {
    drawHorizontalLookupTable(painter, rect, mColors, mValues, &mTextList, this->title(), timestep, 2);
  } else {
    drawHorizontalLookupTable(painter, rect, mColors, mValues, &mTextList, this->title(), timestep, 1);
  }
}



