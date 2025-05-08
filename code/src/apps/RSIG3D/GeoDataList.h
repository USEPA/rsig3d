/******************************************************************************
PURPOSE: GeoDataList.h - Defines the GeoDataList class

NOTES: Implemented using QT's QObject class

HISTORY: 2013/02/25, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/


#ifndef INC_GEODATALIST_H_
#define INC_GEODATALIST_H_

#include <QObject>
#include <QList>
#include "Singleton.h"

//the object retrieved from the web
class GeoDataObject;


//This class is a container for objects. There should be a data structure such as
//  QList<QObjects*> to store the objects. However, since QObject class already has
//  such a strucuture, we use it. 
//Note: the limitation is such all objects contained in this class should be children 
//  of this class. DO NOT use it when you want the objects to have different parents!!!

class GeoDataList: public QObject {
  Q_OBJECT

public:
  void push_back(GeoDataObject* obj);
  void erase(GeoDataObject *obj);
  void clear();
  int size() const;
  GeoDataObject* getObject(int id) const;

Q_SIGNALS:
  void objectAdded(QObject * obj);
  void objectRemoved(QObject * obj);

public Q_SLOTS:
  void onObjectRemoved(QObject * obj);
  void onObjectRemoved(int objID);
  void onChildObjectRemoved(QObject * obj);

protected:
  GeoDataList();
  ~GeoDataList();

private:
  Q_DISABLE_COPY(GeoDataList);
  friend class Singleton<GeoDataList>;

  void markGeoDataObjectForDeletion( QObject* const qObject,
                                     const bool emitRemoveSignal );
};

typedef Singleton<GeoDataList> GeoDataListSingleton;

#endif  //INC_GEODATALIST_H_


