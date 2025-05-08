
/******************************************************************************
PURPOSE: GeoDataObject.C - Class that wraps an emvl::Dataset object (that is
         owned by emvl::DatasetManager) with rendering info such as a
         colormap object.

NOTES: Implemented using QT's QObject class

HISTORY: 2013/02/25, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#include <string>
#include <vector>
#include <QVarLengthArray>
#include <QDebug>

#include <Dataset.h>   // For class emvl::Dataset.
#include <Timestamp.h> // For class emvl::Timestamp.

#include "GeoDataObject.h"
#include "LookupTableWidget.h"
#include "SlantLookupTableWidget.h"
#include "PointRenderer.h"
#include "PolygonRenderer.h"
#include "VectorRenderer.h"


//============================================================


GeoDataObject::GeoDataObject( QObject* parent,
                              GeometryObjectType* data,
                              const QRectF* dataRequestBoundingBox )
  : QObject(parent), mMinmaxVal(0.0f, 0.0f), mObject(0), mShowObject(true) {

  if ( dataRequestBoundingBox ) {
    _dataRequestBoundingBox.setLeft( dataRequestBoundingBox->left() );
    _dataRequestBoundingBox.setRight( dataRequestBoundingBox->right() );
    _dataRequestBoundingBox.setBottom( dataRequestBoundingBox->bottom() );
    _dataRequestBoundingBox.setTop( dataRequestBoundingBox->top() );
  }

  try {
    mObject = data; // Store pointer to dataset which is owned by DatasetManager

    if (mObject != NULL ) {
      setObjectName( varName() );
    }
  } catch (...) {
    const char *msg = "Error: GeoDataObject constructor!";
    qDebug() << msg;
    throw msg;
  }
}



GeoDataObject::~GeoDataObject() {

  // BUG Do not delete the emvl::Dataset object since it is owned by the
  // emvl::DatasetManager Singleton object and will be deleted by it instead!!
  // Just set the pointer to 0.
  //delete mObject;

  mObject = 0;
}



GeometryObjectType* GeoDataObject::model() {
  return mObject;
}



const GeometryObjectType* GeoDataObject::model() const {
  return mObject;
}



void GeoDataObject::setModel(GeometryObjectType* dataset) {
  mObject = dataset;
  this->computeMinmaxVal();
}



void GeoDataObject::computeMinmaxVal() {

  if ( model() ) {
    const GeometryObjectType* m = this->model();
    mMinmaxVal.x = m->minimum();
    mMinmaxVal.y = m->maximum();

    // HACK data ranges
    QString varname = QString::fromStdString( m->name());;

    if (varname.contains("cmaq_") && varname.contains("_aod")) {
      mMinmaxVal.x=0;
      mMinmaxVal.y=1;
    } else if (varname.contains("cmaq_") && varname.contains("_pm25")) {
      mMinmaxVal.x=0;
      mMinmaxVal.y=50;
    } else if (varname.contains("airnow_") && varname.contains("_pm25")) {
      mMinmaxVal.x=0;
      mMinmaxVal.y=50;
    } else if (varname.contains("aqs_") && varname.contains("_pm25") ) {
      mMinmaxVal.x=0;
      mMinmaxVal.y=50;
    } else if (varname.contains("hms_") && varname.contains("_smoke") ) {
      mMinmaxVal.x=0;
      mMinmaxVal.y=27;
    }
  }
}



Vector2f GeoDataObject::minmaxVal() {
  Vector2f r;

  if ( model() ) {
    r = mMinmaxVal;
  }

  return r;
}



bool GeoDataObject::getShowObject() const {
  return mShowObject;
}



void GeoDataObject::setShowObject(bool flag) {

  if (mShowObject != flag) {
    mShowObject = flag;
    Q_EMIT valueChanged();
  }
}



QString GeoDataObject::varName() const {
  QString name("demo");
  
  if ( mObject ) {
    name = QString::fromStdString( mObject->name() );

    // Remove multi-day date range: _yyyy-mm-dd_to_yyyy-mm-dd
    // or single-day date: _yyyy-mm-dd
    // from end of name:
    
    const int length = name.length();
    int i = -1;
    
    if ( name.length() > 25 && ( i = name.lastIndexOf( "_to_" ) ) > 11 ) {
      name.truncate( i - 11 );
    } else if ( length > 11 &&
                name.lastIndexOf( '-'     ) == length - 3 &&
                name.lastIndexOf( '-', -4 ) == length - 6 &&
                name.lastIndexOf( '_', -7 ) == length - 11 ) {
      name.truncate( length - 11 );
    }
  }

  return name;
}



QString GeoDataObject::description() const {
  QString name("");

  if (mObject != NULL) {
    name = QString::fromStdString( mObject->description());
  }

  return name;
}



QString GeoDataObject::units() const {
  QString units("");

  if (mObject != NULL) {
    units = QString::fromStdString( "[" + mObject->variableUnits() + "]");
  }

  return units;
}



int GeoDataObject::rows() const {
  int r = 0;

  if (mObject != NULL) {
    r = mObject->rows();
  }

  return r;
}



int GeoDataObject::columns() const {
  int r = 0;

  if (mObject != NULL) {
    r = mObject->columns();
  }

  return r;
}



int GeoDataObject::layers() const {
  int r = 0;

  if (mObject != NULL) {
    r = mObject->layers();
  }

  return r;
}


QRectF GeoDataObject::getRegionRect() const {
  QRectF rectf;

  if (mObject != NULL) {
    emvl::Bounds b = mObject->bounds();
    const QPointF x0(b.west(), b.south());
    const QPointF x1(b.east(), b.north());
    rectf = QRectF(x0, x1);
  } else {
    rectf = QRectF(0, 0, 0, 0);
  }

  return rectf;
}



QRectF GeoDataObject::getDataRequestBoundingBox() const {
  return _dataRequestBoundingBox;
}

double GeoDataObject::dataMin() const {
  return mMinmaxVal.x;
}



double GeoDataObject::dataMax() const {
  return mMinmaxVal.y;
}



QString GeoDataObject::getStartDate() const {
  QString strDate("");

  if (mObject != NULL) {
    emvl::Timestamp timestamp = mObject->startingTimestamp();
    emvl::Timestamp::UTCString timestr;
    timestamp.toUTCString(timestr);
    timestr[19]='Z';
    timestr[20]=0;
    strDate = QString( timestr );
  } else {
    QDateTime d(QDate(2008, 7, 3));
    d.setTimeSpec(Qt::UTC);
    strDate = d.toString(Qt::ISODate);
  }

  return strDate;
}


QString GeoDataObject::getEndDate() const {
  QString strDate("");

  if (mObject != NULL) {
    emvl::Timestamp timestamp = mObject->endingTimestamp();
    emvl::Timestamp::UTCString timestr;
    timestamp.toUTCString(timestr);
    timestr[19]='Z';
    timestr[20]=0;
    strDate = QString( timestr );
  } else {
    QDateTime d(QDate(2008, 7, 3));
    d.setTimeSpec(Qt::UTC);
    strDate = d.toString(Qt::ISODate);
  }

  return strDate;
}



int GeoDataObject::timesteps() const {
  int r = 0;

  if (mObject != NULL) {
    r = mObject->timesteps();
  }

  return r;
}



bool GeoDataObject::hasElevation() const {
  bool r = false;

  if (mObject != NULL) {
    r = mObject->hasElevation();
  }

  return r;
}

bool GeoDataObject::hasArea() const {
  bool r = false;

  if (mObject != NULL) {
    r = mObject->hasArea();
  }

  return r;
}



bool GeoDataObject::hasVolume() const {
  bool r = false;

  if (mObject != NULL) {
    r = mObject->hasVolume();
  }

  return r;
}



bool GeoDataObject::isStationary() const {
  bool r = true;

  if (mObject != NULL) {
    r = mObject->isStationary();
  }

  return r;
}

bool GeoDataObject::isGroup() const {
  bool r = true;

  if (mObject != NULL) {
    r = mObject->isGroup();
  }

  return r;
}



bool GeoDataObject::isStructuredGrid() const {
  bool r = true;

  if (mObject != NULL) {
    r = mObject->isStructuredGrid();
  }

  return r;
}



int GeoDataObject::cellType() const {
  int r = 0;

  if (mObject != NULL) {
    r = mObject->cellType();
  }

  return r;
}



VisDataObject* GeoDataObject::createDefaultVisualizationObject() {

  if (mObject == NULL) {
    return NULL;
  }
  
  VisDataObject* obj = NULL;
  const int ctype = cellType(); 
  PointRenderer* pntRender = NULL;
  
  if ( mObject->isVector() ) {
    obj = new VectorRenderer( this );
  } else if (ctype == emvl::Dataset::POINT) {
    pntRender = new PointRenderer(this);
    obj = pntRender;
  } else if ( ctype == emvl::Dataset::QUADRILATERAL ||
              ctype == emvl::Dataset::POLYGONS ||
              ctype == emvl::Dataset::HEXAHEDRON ) {
    PolygonRenderer* p = new PolygonRenderer(this);
    obj = p;
  } else {
    qDebug() << "Volume or surface data: not supported yet";
  }
  
  if (obj != NULL) {
    const QString name = this->objectName() + "." + obj->description();
    obj->setObjectName(name);

    if (ctype == emvl::Dataset::POINT && pntRender) {
      QString basename("");
      const int pos = name.indexOf(QChar('.'));

      for (int i=0; i<pos; i++) {
        basename.append(name[i]);
      }

      basename = basename.toUpper();

      // NOTE: some graphics cards (Jim's, Luke's) do not support sprites.
      // So make everything shape 0. MJF - 6-19-2018

      if (basename == "AIRNOW") {
        pntRender->setPointShape(0);
      } else if (basename=="AQS") {
        pntRender->setPointShape(0);
      } else if (basename=="UVNET") {
        pntRender->setPointShape(0);
      } else if (basename=="FAQSD") {
        pntRender->setPointShape(0);
      } else if (basename=="NESDIS") {
        pntRender->setPointShape(0);
      }
    }
  }
  
  if (obj && mObject->timestepSize() == emvl::Dataset::DAYS) {
    obj->setAggregate(QString("daily"));
  }
  
  return obj;
}



int GeoDataObject::lutTextureID() const {
  int id = 0;
  const QList<QObject*>& childList = this->children();

  for (int i=0; i<childList.size(); i++) {
    LookupTableWidget * lut = qobject_cast<LookupTableWidget*>( childList[i] );

    if (lut!=NULL) {
      id = lut->texureID();
      break;
    }
  }

  return id;
}



LookupTableWidget * GeoDataObject::getLut() const {
  LookupTableWidget* lut = 0;
  const QList<QObject*>& childList = this->children();

  for (int i=0; lut == 0 && i<childList.size(); i++) {
    lut = qobject_cast<LookupTableWidget*>( childList[i] );
  }

  return lut;
}



// Get 0-based index of colormap used to draw this object:

int GeoDataObject::getColormapIndex() const {
  LookupTableWidget* const lut = getLut();
  const int result = lut ? lut->getColormapIndex() : -1;
  return result;
}



LookupTableWidget* GeoDataObject::createDefaultLookupTable() {
  LookupTableWidget * lut = NULL;
  static int objectCount = 1;

  if ((objectCount&1) == 1) {
    lut = new LookupTableWidget( this );

    // Use default color map. "setMatt..." UGLY.
    lut->setMattLookupTable(0, 0, 1); // default range will be adjusted below
    lut->setTitle("Default");
  } else {
    SlantTextLookupTableWidget *lut0 = new SlantTextLookupTableWidget( this );
    lut0->setDefaultLookupTable();
    lut0->setTitle("AOS PM2.5(ug/m3)");
    const QString texts[] = { // UGLY. Non-portable: initialize array of non-POD
      QString("Good"),
      QString("Mod"),
      QString("Sensitive"),
      QString("Unhealthy"),
      QString("Very Unhealthy"),
      QString("Hazardous")
    };
    lut0->setTextList(texts, sizeof(texts)/sizeof(texts[0]));
    lut = lut0;
  }

  if (lut!=NULL) {
    lut->setTitle(varName() + " " + this->units());
    const Vector2f minmax = this->minmaxVal();
    lut->adjustRange(minmax.x, minmax.y);
    lut->genTexture();
    const QString nodeName = varName() + ".lut";
    lut->setObjectName(nodeName);
  }

  return lut;
}



