/******************************************************************************
PURPOSE: VisDataList.h - Defines the VisDataList class

NOTES: Implemented using QT's QObject class

HISTORY: 2013/03/02, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#ifndef INC_VISDATALIST_H_
#define INC_VISDATALIST_H_

#include <QObject>
#include <QList>
#include <QRect>
#include "Singleton.h"

#include <QString>
#include "VisDataObject.h"
class LookupTableWidget;

class QPainter;
class VisDataObject;
class RenderingContextRSIGBase;

class VisDataList: public QObject {
    Q_OBJECT

public:
  void push_back(VisDataObject* obj);

  VisDataObject* getObjectByIndex( int index ) const;
  VisDataObject* getObjectByName(QString) const;
  LookupTableWidget* getLutByName(QString) const;
  void render2DCalipsoCurtain(QPainter *painter, const QRect& rect);
  void renderPopupNeubrew(QPainter *painter, const QRect& rect);
  void renderPopupCeilometer(QPainter *painter, const QRect& rect);
  int getCounterNeubrew();
  int getCounterCeilometer();
  void setCounterNeubrew(int value);
  void setCounterCeilometer(int value);
  QString getName(int index) const;

  void erase(VisDataObject *obj);
  int size() const;

  void renderLookupTables(QPainter *painter, const QRect& rect);
  void renderGeometryObjects(QPainter *painter, const QRect& rect, RenderingContextRSIGBase * context);

Q_SIGNALS:
  void objectAdded(QObject * obj);
  void objectRemoved(QObject * obj);

public Q_SLOTS:
  void onObjectRemoved(QObject * obj);

protected:
  VisDataList();
  ~VisDataList();

private:
  Q_DISABLE_COPY(VisDataList);
  friend class Singleton<VisDataList>;
  QList<VisDataObject*> mObjects;

  int counterNeubrew;
  int counterCeilometer;
};

typedef Singleton<VisDataList> VisDataListSingleton;

#endif  //INC_VISDATALIST_H_


