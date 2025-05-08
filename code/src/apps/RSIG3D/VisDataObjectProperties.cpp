/******************************************************************************
PURPOSE: VisDataObject.C - Implements the VisDataObject class

NOTES: Implemented using QT's QObject class

PURPOSE: Used to maintain coherence when new data is loaded (e.g. colormap, etc)

HISTORY: 2018/11/12, Matt Freeman, Created.
******************************************************************************/

#include <QDebug>

#include "VisDataObjectProperties.h"
#include "GeoDataObject.h"
#include "LookupTableWidget.h"




VisDataObjectProperties::VisDataObjectProperties(QObject *parent):
    QObject(parent)
    ,mMinValChangedThisSession(false)
    ,mMaxValChangedThisSession(false)
    ,mShowObject(true)
    ,mAlpha(1.0f)
    ,mThreshold(0.0)
    ,mThresholdSliderValue(0)
    ,mShowCellBoundary(false)
    ,mShowThreshold(false)
    ,mIsDefaultRangeChecked(true)
    ,mMinval(0.0f)
    ,mMaxval(0.0f)
    ,mLastMinval(0.0f)
    ,mLastMaxval(0.0f)
    ,mCurColormapIndex(0)
    ,mShowCMAQRow(false)
    ,mShowCMAQCol(false)
    ,mShowCMAQLayer(false)
    ,mCMAQRow(0)
    ,mCMAQCol(0)
    ,mCMAQLayer(0)
    ,mMaxCMAQRow(0)
    ,mMaxCMAQCol(0)
    ,mMaxCMAQLayer(0)
{
}



VisDataObjectProperties::~VisDataObjectProperties() {
}



/*!
 \fn bool VisDataObjectProperties::showObject() const
 
 Get the value of visibility.
 */

QString VisDataObjectProperties::name() const {
  return mName;
}



void VisDataObjectProperties::setName(QString name) {

  if (mName != name) {
    mName = name;
  }
}



bool VisDataObjectProperties::showObject() const {
  return mShowObject;
}



/*!
 \fn void VisDataObjectProperties::setShowObject(bool flag)

 Set the value of visibility.
 */

void VisDataObjectProperties::setShowObject(bool flag) {

  if (mShowObject != flag) {
    mShowObject = flag;
  }
}



bool VisDataObjectProperties::showCellBoundary() const {
  return mShowCellBoundary;
}




void VisDataObjectProperties::setShowCellBoundary(bool flag) {

  if (mShowCellBoundary != flag) {
    mShowCellBoundary = flag;
  }
}



/*!
 \fn bool VisDataObjectProperties::isVisible() const

 Return whether the object is visible/hidden. 
 Here the visibility is controlled by its parent geometry object and its own property value \e mShowObject.
 If the parent object is not visible, the object will also be hidden. 
 If the \e mShowObject is false, it will also be hidden.
 */

bool VisDataObjectProperties::isWidgetOfType(QString inquiry) const {
  GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());

  if (par->varName().contains(inquiry, Qt::CaseInsensitive)) {
    return true;
  } else {
    return false;
  }
}



bool VisDataObjectProperties::isVisible() const {
  bool vis = true;

  if (mShowObject == false) {
    vis = false;
  } else {
    GeoDataObject* w = qobject_cast<GeoDataObject*>(parent());

    if ( w != NULL ) {
      vis = w->getShowObject();
    }
  }

  return vis;
}



Vector2f VisDataObjectProperties::minmaxData() const {
  // get the absolute min/max for this dataset
  Vector2f minmax(0, 0);
  GeoDataObject *obj = qobject_cast<GeoDataObject *>( parent() );

  if (obj != NULL) {
    minmax = obj->minmaxVal();
  }

  return minmax;
}



Vector2f VisDataObjectProperties::minmaxVal() const {
  // get the user specified min/max for this dataset
  Vector2f minmax(0, 0);

  if ( !((mMinval == 0.0) && (mMaxval == 0.0)) ) {
    minmax.x = mMinval;
    minmax.y = mMaxval;
  } else {
    minmax = minmaxData();
  }

  return minmax;
}



qreal VisDataObjectProperties::alpha() const {
  return mAlpha;
}



void VisDataObjectProperties::setAlpha(qreal alpha) {

  if ( alpha != mAlpha ) {
    mAlpha = alpha;

    if (mAlpha > 1.0f) {
      mAlpha = 1.0f;
    }

    if (mAlpha < 0.0f) {
      mAlpha = 0.0f;
    }
  }
}



bool VisDataObjectProperties::minValChangedThisSession() {
  return mMinValChangedThisSession;
}



bool VisDataObjectProperties::maxValChangedThisSession() {
  return mMaxValChangedThisSession;
}



void VisDataObjectProperties::setMinValChangedThisSession(bool flag) {
  mMinValChangedThisSession = flag;
}



void VisDataObjectProperties::setMaxValChangedThisSession(bool flag) {
  mMaxValChangedThisSession = flag;
}



qreal VisDataObjectProperties::threshold() const {
  return mThreshold;
}



void VisDataObjectProperties::setThreshold(qreal threshold) {

  if ( threshold != mThreshold ) {
    mThreshold = threshold;
  }
}



int VisDataObjectProperties::thresholdSliderValue() const {
  return mThresholdSliderValue;
}



void VisDataObjectProperties::setThresholdSliderValue(int thresholdSliderValue) {

  if ( thresholdSliderValue != mThresholdSliderValue ) {
    mThresholdSliderValue = thresholdSliderValue;
  }
}



void VisDataObjectProperties::setDefaultRangeChecked(bool flag) {

  if (mIsDefaultRangeChecked != flag) {
    mIsDefaultRangeChecked = flag;
  }
}



bool VisDataObjectProperties::isDefaultRangeChecked() const {
  return mIsDefaultRangeChecked;
}



double VisDataObjectProperties::minval() {
  return mMinval;
}



double VisDataObjectProperties::maxval() {
  return mMaxval;
}



double VisDataObjectProperties::lastMinval() {
  return mLastMinval;
}



double VisDataObjectProperties::lastMaxval() {
  return mLastMaxval;
}



int VisDataObjectProperties::curColormapIndex() {
  return mCurColormapIndex;
}



void VisDataObjectProperties::setMinval(double minval) {

  if ( minval != mMinval ) {
    mMinval = minval;
  }
}



void VisDataObjectProperties::setMaxval(double maxval) {

  if ( maxval != mMaxval ) {
    mMaxval = maxval;
  }
}



void VisDataObjectProperties::setLastMinval(double minval) {

  if ( minval != mLastMinval ) {
    mLastMinval = minval;
  }
}



void VisDataObjectProperties::setLastMaxval(double maxval) {

  if ( maxval != mLastMaxval ) {
    mLastMaxval = maxval;
  }
}



void VisDataObjectProperties::setCurColormapIndex(int curColormapIndex) {

  if ( curColormapIndex != mCurColormapIndex ) {
    mCurColormapIndex = curColormapIndex;
  }
}



int VisDataObjectProperties::CMAQRow() {
  return mCMAQRow;
}



int VisDataObjectProperties::CMAQCol() {
  return mCMAQCol;
}



int VisDataObjectProperties::CMAQLayer() {
  return mCMAQLayer;
}



int VisDataObjectProperties::MaxCMAQRow() {
  return mMaxCMAQRow;
}



int VisDataObjectProperties::MaxCMAQCol() {
  return mMaxCMAQCol;
}



int VisDataObjectProperties::MaxCMAQLayer() {
  return mMaxCMAQLayer;
}



void VisDataObjectProperties::setCMAQRow(int row) {

  if ( row != mCMAQRow ) {
    mCMAQRow = row;
  }
}



void VisDataObjectProperties::setCMAQCol(int col) {

  if ( col != mCMAQCol ) {
    mCMAQCol = col;
  }
}



void VisDataObjectProperties::setCMAQLayer(int layer) {

  if ( layer != mCMAQLayer ) {
    mCMAQLayer = layer;
  }
}



void VisDataObjectProperties::setMaxCMAQRow(int maxRow) {

  if ( maxRow != mMaxCMAQRow ) {
    mMaxCMAQRow = maxRow;
  }
}



void VisDataObjectProperties::setMaxCMAQCol(int maxCol) {

  if ( maxCol != mMaxCMAQCol ) {
    mMaxCMAQCol = maxCol;
  }
}



void VisDataObjectProperties::setMaxCMAQLayer(int maxLayer) {

  if ( maxLayer != mMaxCMAQLayer ) {
    mMaxCMAQLayer = maxLayer;
  }
}



bool VisDataObjectProperties::ShowCMAQRow() {
  return mShowCMAQRow;
}



bool VisDataObjectProperties::ShowCMAQCol() {
  return mShowCMAQCol;
}



bool VisDataObjectProperties::ShowCMAQLayer() {
  return mShowCMAQLayer;
}



void VisDataObjectProperties::setShowCMAQRow(bool flag) {

  if (mShowCMAQRow != flag) {
    mShowCMAQRow = flag;
  }
}



void VisDataObjectProperties::setShowCMAQCol(bool flag) {

  if (mShowCMAQCol != flag) {
    mShowCMAQCol = flag;
  }
}



void VisDataObjectProperties::setShowCMAQLayer(bool flag) {

  if (mShowCMAQLayer != flag) {
    mShowCMAQLayer = flag;
  }
}




bool VisDataObjectProperties::ShowThreshold() {
  return mShowThreshold;
}



void VisDataObjectProperties::setShowThreshold(bool flag) {

  if (mShowThreshold != flag) {
    mShowThreshold = flag;
  }
}



