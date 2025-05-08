/******************************************************************************
 PURPOSE: GeoDataList.C - Implements the GeoDataList class
 
 NOTES: Implemented using QT's QObject class
 
 HISTORY: 2013/02/25, Nan Zhang <zhang.nan@epa.gov>, Created.
 ******************************************************************************/

#include <QDebug>
#include <QVarLengthArray>
#include "GeoDataList.h"
#include "GeoDataObject.h"

static const char * errorMessage0 = "Error: indexed object is not a child of GeoDataList class";



/*
 * UGLY. Since this class uses QT's delayed events to handle deletion of
 * stored GeoDataObjects, it is necessary to first set the GeoDataObject's
 * mObject/model/Dataset pointer to zero to avoid dangling pointers to deleted
 * Dataset objects when the GeoDataObject destructor is finally called.
 */

void GeoDataList::markGeoDataObjectForDeletion( QObject* const qObject,
                                                const bool emitRemoveSignal ) {

  if ( qObject ) {
    GeoDataObject* const geoDataObject = dynamic_cast<GeoDataObject*>(qObject);

    if ( geoDataObject ) {
      geoDataObject->setModel( 0 ); // Must clear pointer to dataset now!
      geoDataObject->deleteLater(); // GeoDataObject destructor will be called
                                    // later.

      if ( emitRemoveSignal ) {
        Q_EMIT objectRemoved(geoDataObject);
      }
    }
  }
}



/*!
 \class GeoDataList
 
 */
GeoDataList::GeoDataList() : QObject(0) {
}



GeoDataList::~GeoDataList() {
}



GeoDataObject* GeoDataList::getObject(int id) const {
  const QList<QObject*>& ch = this->children();
  GeoDataObject* p = NULL;

  if ( id >= 0 || id < ch.size() ) {
    QObject* pobj = ch[ id ];
    p = qobject_cast<GeoDataObject*>( pobj );
  }

  return p;
}



void GeoDataList::push_back(GeoDataObject* obj) {
  obj->setParent(this);
  Q_EMIT objectAdded(obj);
}



void GeoDataList::erase(GeoDataObject* obj) {

  if (obj==NULL) return;

  if (obj->parent() == this) {
    markGeoDataObjectForDeletion( obj, true );
  } else {
    qDebug()<<errorMessage0;
  }
}



void GeoDataList::clear() {
  const QList<QObject*>& lst = children();
  QVarLengthArray<QObject *, 64> buffer; // Why create a copy?
  buffer.clear();

  for (int i=0; i<lst.size(); i++) {
    buffer.append(lst[i]);
  }

  for (int i=0; i<buffer.size(); i++) {
    QObject * obj = buffer[i];

    if (obj->parent() == this) {
      markGeoDataObjectForDeletion( obj, true );
    }
  }
}


// Handle objectRemoved signal:

void GeoDataList::onObjectRemoved(int indexVal) {
  const QObjectList & objlist = this->children();

  if (indexVal >=0 && indexVal < objlist.size()) {
    QObject *obj = objlist[indexVal];

    if (obj->parent() == this) {
      markGeoDataObjectForDeletion( obj, false );
    } else {
      qDebug()<<errorMessage0;
    }
  }
}


// Handle objectRemoved signal:

void GeoDataList::onObjectRemoved(QObject * obj) {

  if (obj==NULL) return;

  const QObjectList & objlist = this->children();
  const int indexVal = objlist.indexOf(obj);
  onObjectRemoved(indexVal);
}



int GeoDataList::size() const  {
  const QObjectList & objlist = this->children();
  return int(objlist.size());
}



// Handle childObjectRemoved signal:

void GeoDataList::onChildObjectRemoved(QObject * childobj) {

  if (childobj==NULL) return;

  QObject *parobj = childobj->parent();

  if (parobj==NULL) return;

  const QObjectList & objlist = this->children();

  for (int i=0; i<objlist.size(); i++) {
    QObject * obj = objlist[i];

    if (obj == parobj) {
      //need to do nothing at this time, since QT will auto manage the removement of child ptr
      break;
    }
  }
}

