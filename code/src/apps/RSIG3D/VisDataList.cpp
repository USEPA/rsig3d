/******************************************************************************
PURPOSE: VisDataList.C - Implements the VisDataList class

NOTES: Implemented using QT's QObject class

HISTORY: 2012-11-11 zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
STATUS: unreviewed untested
******************************************************************************/

#include <stdio.h>
#include <string.h>

#include <algorithm>    //std::max

#include <QPainter>
#include <QDebug>
#include <QVarLengthArray>
#include <QString>

#include "LookupTableWidget.h"
#include "SlantLookupTableWidget.h"
#include "VisDataList.h"
#include "VisDataObject.h"
#include "GeoDataObject.h"
#include "RSIGScene.h"
#include "LoadMapFile.h"
#include "RenderingContextRSIGBase.h"
#include "VisOptions.h"



VisDataList::VisDataList() : QObject(), mObjects() {
}



VisDataList::~VisDataList() {

  for (int i=0; i<mObjects.size(); i++) {
    delete mObjects[i];
  }

  mObjects.clear();
}



void VisDataList::push_back(VisDataObject* obj) {

  if (obj != NULL) {
    mObjects.push_back(obj);
    Q_EMIT objectAdded(obj);
  }
}



// get the name of the visDataObject stored at location index

QString VisDataList::getName(int index) const {
  const int nsize = this->size();

  if (index < nsize) {
    return mObjects[index]->objectName();
  } else {
    return "";
  }
}



VisDataObject* VisDataList::getObjectByIndex( int index ) const {
  VisDataObject* result = 0;

  if ( index >= 0 && index < size() ) {
    result = mObjects[ index ];
  }

  return result;
}



VisDataObject * VisDataList::getObjectByName(QString name) const {
  const int nsize = this->size();

  for (int i=0; i<nsize; i++) {

    if (mObjects[i]->objectName() == name) {
      return mObjects[i];
    }
  }

  return NULL;
}



LookupTableWidget * VisDataList::getLutByName(QString name) const {
  const int nsize = this->size();

  for (int i=0; i<nsize; i++) {

    if (mObjects[i]->objectName() == name) {
      return dynamic_cast<LookupTableWidget*>(mObjects[i]);
    }
  }

  return NULL;
}



void VisDataList::setCounterNeubrew(int value) {
  counterNeubrew = value;
}



int VisDataList::getCounterNeubrew() {
  return counterNeubrew;
}



void VisDataList::setCounterCeilometer(int value) {
  counterCeilometer = value;
}


int VisDataList::getCounterCeilometer() {
  return counterCeilometer;
}



void VisDataList::erase(VisDataObject* obj) {
  const int indexVal = mObjects.indexOf(obj);

  if (indexVal != -1) {
    mObjects.removeAt(indexVal);
    Q_EMIT objectRemoved(obj);
  }
}



void VisDataList::onObjectRemoved(QObject * _obj) {
  VisDataObject *obj = qobject_cast<VisDataObject*>(_obj);

  if ( obj ) {
    const int indexVal = mObjects.indexOf(obj);

    if (indexVal != -1) {
      mObjects.removeAt(indexVal);
      obj->deleteLater();
    }
  }
}



int VisDataList::size() const {
  return int(mObjects.size());
}



void VisDataList::renderLookupTables(QPainter *painter, const QRect& screenrect) {
  const int nsize = this->size();
  const int WBUFF_SIZE = 7;
  QVarLengthArray<LookupTableWidget*, WBUFF_SIZE> wbuff;

  for (int i=0; i<nsize; i++) {

    if (mObjects[i]->isLookupTableWidget()) {
      LookupTableWidget *w = qobject_cast<LookupTableWidget*>(mObjects[i]);

      if (w!=NULL) {
        wbuff.append(w);
      }
    }
  }

  const int nLookupTable = std::min<int>(wbuff.size(), WBUFF_SIZE);
  const int nRow = (nLookupTable <= 3) ? 1: 2;
  const int blockWidth = (screenrect.width() >> 2) * 1.3;
  const int blockHeight = 95;
  const QRect bounds = screenrect;
  const int bgcolor = 200;
  const int bgalpha = 255;
  const QBrush brush1(QColor(bgcolor, bgcolor, bgcolor, bgalpha));
  const QRect r1(0, bounds.height()-nRow*blockHeight, bounds.width(), nRow*blockHeight);
  painter->fillRect(r1, brush1);

  for (int i=0; i<nLookupTable; i++) {
    LookupTableWidget *w = wbuff[i];
    QRect tabRect;

    if (i>=0 && i<3) {
      tabRect = QRect(i*blockWidth, bounds.height()-blockHeight, blockWidth, blockHeight);
    } else {
      const int j = i - 3;
      tabRect = QRect(j*blockWidth, bounds.height()-blockHeight*2, blockWidth, blockHeight);
    }

    w->render(painter, tabRect);
  }
}



void VisDataList::render2DCalipsoCurtain(QPainter *painter, const QRect& rect) {

  const int visObjects = size();

  for ( int objectIndex = 0; objectIndex < visObjects; ++objectIndex ) {
    VisDataObject* const visDataObject = mObjects[ objectIndex ];

    if ( visDataObject && visDataObject->is3DObject() ) {

      if ( visDataObject->objectName().toLower().contains( "calipso" ) ) {
        //qDebug() << "render2DCalipsoCurtain";
        visDataObject->render2DCalipsoCurtain( painter, rect );
      }
    }
  }
}

void VisDataList::renderPopupNeubrew(QPainter *painter, const QRect& rect) {

  const int visObjects = size();

  // first pass - see if we have a calipso variable.
  // if so, the popup plots will be below the calipso curtain in 2D
  bool haveCalipso = false; // default

  for ( int objectIndex = 0; objectIndex < visObjects; ++objectIndex ) {
    VisDataObject* const visDataObject = mObjects[ objectIndex ];

    if ( visDataObject && visDataObject->is3DObject() ) {

      if ( visDataObject->objectName().toLower().contains( "calipso" ) ) {
        haveCalipso = true;
      }
    }
  }

  // second pass - see if we have neubrew
  for ( int objectIndex = 0; objectIndex < visObjects; ++objectIndex ) {
    VisDataObject* const visDataObject = mObjects[ objectIndex ];

    if ( visDataObject && visDataObject->is3DObject() ) {

      if ( visDataObject->objectName().toLower().contains( "neubrew" ) ) {
        visDataObject->renderPopupNeubrew( painter, rect, haveCalipso );
      }
    }
  }
}



void VisDataList::renderPopupCeilometer(QPainter *painter, const QRect& rect) {

  const int visObjects = size();

  // first pass - see if we have a calipso variable.
  // if so, the popup plots will be below the calipso curtain in 2D
  bool haveCalipso = false; // default

  for ( int objectIndex = 0; objectIndex < visObjects; ++objectIndex ) {
    VisDataObject* const visDataObject = mObjects[ objectIndex ];

    if ( visDataObject && visDataObject->is3DObject() ) {

      if ( visDataObject->objectName().toLower().contains( "calipso" )) {
        haveCalipso = true;
      }
    }
  }

  RSIGScene& rsigScene = RSIGScene::getInstance();

  // second pass - see if we have ceilometer or pandora
  for ( int objectIndex = 0; objectIndex < visObjects; ++objectIndex ) {
    VisDataObject* const visDataObject = mObjects[ objectIndex ];

    if ( visDataObject && visDataObject->is3DObject() ) {

      if ( visDataObject->objectName().toLower().contains( "ceilometer" ) ) {
        rsigScene.ceilometerCCADrawFlag = true;
        visDataObject->renderPopupCeilometer( painter, rect, haveCalipso );
        rsigScene.ceilometerCCADrawFlag = false;
      } else if (visDataObject->objectName().toLower().contains( "pandora" )) {
        rsigScene.pandoraCCADrawFlag = true;
        visDataObject->renderPopupPandora( painter, rect, haveCalipso );
        rsigScene.pandoraCCADrawFlag = false;
      }
    }
  }
}



// Draw routine:

void VisDataList::renderGeometryObjects( QPainter *painter,
                                         const QRect& rect,
                                         RenderingContextRSIGBase* context ) {
  const char* renderOrder2D[] = {
    "behr"        ,
    "omibehr"     ,
    "omi"         ,
    "cmaq"        ,
    "daily_mean_cmaq", 
    "regridded"   ,
    "modis"       ,
    "viirs"       ,
    "tempo"       ,
    "tropomi"     ,
    "gasp"        ,
    "goes"        ,
    "satellitepm" ,
    "geoschem"    ,
    "hms"         ,
    "airnow"      ,
    "airnow2"     ,
    "daily_mean_airnow",
    "aqs"         ,
    "daily_mean_aqs",
    "purpleair"   ,
    "daily_mean_purpleair",
    "metar"       ,
    "surfmet"     ,
    "acam"        ,
    "hsrl"        ,
    "mozaic"      ,
    "tad"         ,
    "faqsd"       ,
    "neubrew"     ,
    "pandora"     ,
    "hourly_pandora",
    "ceilometer"  ,
    "uvnet"       ,
    "nesdis"      ,
    "goesbb"      ,
    "calipso"     ,
    "hrrr"        ,
    "null"        , // placeholder for maplines, not needed for 2D rendering (taken care of in RSIGScene)
    "null"        , // 2D plot is rendered using renderPopupNeubrew()
    "null"          // 2D plot is rendered using renderPopupCeilometer()
  };

  const char*  renderOrder3D[] = {
    "airnow"        ,
    "airnow2"       ,
    "daily_mean_airnow",
    "aqs"           ,
    "daily_mean_aqs",
    "purpleair"     ,
    "daily_mean_purpleair",
    "metar"         ,
    "surfmet"       ,
    "hms"           ,
    "neubrew"       ,
    "pandora"       ,
    "hourly_pandora",
    "ceilometer"    ,
    "uvnet"         ,
    "nesdis"        ,
    "goesbb"        ,
    "modis"         ,
    "viirs"         ,
    "tempo"         ,
    "tropomi"       ,
    "gasp"          ,
    "goes"          ,
    "satellitepm"   ,
    "geoschem"      ,
    "faqsd"         ,
    "acam"          ,
    "hsrl"          ,
    "mozaic"        ,
    "tad"           ,
    "behr"          ,
    "omibehr"       ,
    "omi"           ,
    "cmaq"          ,
    "daily_mean_cmaq", 
    "regridded"     ,
    "calipso"       ,
    "hrrr"          ,
    "maplines"      ,
    "neubrewPopup"  ,   
    "ceilometerPopup" 
  };

  const size_t renderOrderNames2D = sizeof renderOrder2D / sizeof *renderOrder2D;
  const size_t renderOrderNames3D = sizeof renderOrder3D / sizeof *renderOrder3D;

  // check to be sure the 2D and 3D tables are the same size

  if (renderOrderNames2D != renderOrderNames3D) {
    qDebug() << "==== VisDataList: renderOrderNames2D != renderOrderNames3D ====";
    qDebug() << renderOrderNames2D << renderOrderNames3D;
  }

  const size_t renderOrderNames = renderOrderNames2D; // we made it this far, so one is as good as the other
 
  const char* renderOrder[renderOrderNames];
  const int renderMode = context == 0;

  for (size_t ind = 0; ind < renderOrderNames; ++ind ) {

    if (renderMode == 0) {
      renderOrder[ind] = renderOrder2D[ind];
    } else {
      renderOrder[ind] = renderOrder3D[ind];
    }
  }

  this->setCounterCeilometer(0);
  this->setCounterNeubrew(0);

  RSIGScene& rsigScene = RSIGScene::getInstance();
  rsigScene.ceilometerCCADrawFlag = false;
  rsigScene.ceilometerCCADrawn = false;
  VisDataObject* visDataObject = NULL;
  VisDataObject* ceilometerDataObject = NULL;
  bool flagPopupPlot;

  bool mapDrawnFlag = false;

  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  int displayMode = vOptions->getDisplayMode();
  
  // loop for opacity - draw opaque objects first to minimize the depth/alpha problem
  
  for ( int pass = 0; pass <= 1; ++pass ) {
    // pass=0: render opaque objects
    // pass=1: render translucent objects

    // loop over render name order

    for ( size_t nameIndex = 0; nameIndex < renderOrderNames; ++nameIndex ) {
      QString thisRenderName( renderOrder[ nameIndex ] );

      flagPopupPlot = false; // whether to render an ortho popup plot in 3D mode

      if (thisRenderName.contains("Popup")) {
        flagPopupPlot = true;
        thisRenderName.replace("Popup", "");
      }

      // loop over vis objects

      const int visObjects = size();

      for ( int objectIndex = 0; objectIndex < visObjects; ++objectIndex ) {
        visDataObject = mObjects[ objectIndex ];
        //qDebug() << "visDataObject->objectName()= " << visDataObject->objectName();

        if ( visDataObject && visDataObject->is3DObject() ) {
          const double alpha = visDataObject->alpha();

          if (visDataObject->objectName().contains( "ceilometer" )) {
            ceilometerDataObject = visDataObject;
          }

          if ( visDataObject->objectName().toLower().startsWith( thisRenderName ) ) {

            //qDebug() << "rendering " << visDataObject->objectName();

            // render in 2D mode (only need to do pass 0)

            if ( context && displayMode == 0) { //2D mode
              if (pass == 0) {
                visDataObject->render( painter, rect, context, flagPopupPlot);
              }

            } else if (context && displayMode == 2) { // display panel mode
              if (pass == 0) {
                visDataObject->renderDisplayPanel(painter, rect, context);
              }
              
            } else { // render in 3D mode

              if ( pass == 0 ) {

                if ( alpha >= 0.85 ) {
                  //qDebug() << "rendering pass 0" << thisRenderName << alpha;
                  visDataObject->renderEMVL( painter, rect, flagPopupPlot);
                }

              } else { //pass 1. If alpha < 0.1, do not draw

                if ( alpha > 0.1 && alpha < 0.85 ) {
                  //qDebug() << "rendering pass 1" << thisRenderName << alpha;
                  visDataObject->renderEMVL( painter, rect, flagPopupPlot);
                }
              }
            }
          }

        } else if (thisRenderName.toLower().contains( "maplines" ) && !mapDrawnFlag) {
          rsigScene.drawMapOverlay();
          mapDrawnFlag = true;
        }
      }
    }
  }
  
  // render ceilometer popup from CCA array

  flagPopupPlot = true;
  rsigScene.ceilometerCCADrawFlag = true;
  this->setCounterCeilometer(0);

  if (context == NULL && ceilometerDataObject) {
    ceilometerDataObject->renderPopupCeilometer( painter, rect, flagPopupPlot);
  }
}



