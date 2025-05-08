/******************************************************************************
PURPOSE: GeoDataObject.h - Class that wraps an emvl::Dataset object (that is
         owned by emvl::DatasetManager) with rendering info such as a
         colormap object.

NOTES: Implemented using QT's QObject class

HISTORY: 2013/02/25, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#ifndef INC_GEODATAOBJECT_H_
#define INC_GEODATAOBJECT_H_

#include <vector>
#include <QObject>
#include <QRect>
#include <QRectF>
#include <QDateTime>
#include <QDate>
#include <QVector2D>

#include "vector4f.h"
#include "vector2f.h"

namespace emvl{
  class Dataset;
};

typedef emvl::Dataset GeometryObjectType;

class VisDataObject;
class LookupTableWidget;


class GeoDataObject: public QObject {
Q_OBJECT

public:
  GeoDataObject( QObject* parent = 0, GeometryObjectType* dataset = 0,
                 const QRectF* dataRequestBoundingBox = 0 );

  virtual ~GeoDataObject();

  const GeometryObjectType *model() const;

  GeometryObjectType *model();

  void setModel(GeometryObjectType *model);

  virtual int lutTextureID() const;

  LookupTableWidget *getLut() const; // Get pointer to LUT used to display this object.

  int getColormapIndex() const; // Get index of colormap used to draw this object:

  virtual void computeMinmaxVal();

  virtual Vector2f minmaxVal();

  Q_PROPERTY(bool showObject READ getShowObject WRITE setShowObject DESIGNABLE true USER true)
  bool getShowObject() const;
  void setShowObject(bool flag);

  Q_PROPERTY(QString varName READ varName DESIGNABLE true USER true CONSTANT)
  QString varName() const;

  Q_PROPERTY(QString description READ description DESIGNABLE true USER true CONSTANT)
  QString description() const;

  Q_PROPERTY(QRectF bounds READ getRegionRect DESIGNABLE true USER true CONSTANT)
  QRectF getRegionRect() const;

  Q_PROPERTY(QRectF bounds READ getDataRequestBoundingBox DESIGNABLE true USER true CONSTANT)
  QRectF getDataRequestBoundingBox() const;

  Q_PROPERTY(QString startDate READ getStartDate DESIGNABLE true USER true CONSTANT)
  QString getStartDate() const;

  Q_PROPERTY(QString endDate READ getEndDate DESIGNABLE true USER true)
  QString getEndDate() const;

  Q_PROPERTY(int timesteps READ timesteps DESIGNABLE true USER true CONSTANT)
  int timesteps() const;

  Q_PROPERTY(int cellType READ cellType DESIGNABLE true USER true CONSTANT)
  int cellType() const;

  Q_PROPERTY(bool hasElevation READ hasElevation DESIGNABLE true USER true CONSTANT)
  bool hasElevation() const;

  Q_PROPERTY(bool hasArea READ hasArea DESIGNABLE true USER true CONSTANT)
  bool hasArea() const;

  Q_PROPERTY(bool hasVolume READ hasVolume DESIGNABLE true USER true CONSTANT)
  bool hasVolume() const;

  Q_PROPERTY(bool isStationary READ isStationary DESIGNABLE true USER true CONSTANT)
  bool isStationary() const;

  Q_PROPERTY(bool isGroup READ isGroup DESIGNABLE true USER true CONSTANT)
  bool isGroup() const;

  Q_PROPERTY(bool isStructuredGrid READ isStructuredGrid DESIGNABLE true USER true CONSTANT)
  bool isStructuredGrid() const;

  Q_PROPERTY(double dataMin READ dataMin DESIGNABLE true USER true CONSTANT)
  double dataMin() const;

  Q_PROPERTY(double dataMax READ dataMax DESIGNABLE true USER true CONSTANT)
  double dataMax() const;

  Q_PROPERTY(QString units READ units DESIGNABLE true USER true CONSTANT)
  QString units() const;

  Q_PROPERTY(int rows READ rows DESIGNABLE true USER true CONSTANT)
  int rows() const;

  Q_PROPERTY(int columns READ columns DESIGNABLE true USER true CONSTANT)
  int columns() const;

  Q_PROPERTY(int layers READ layers DESIGNABLE true USER true CONSTANT)
  int layers() const;

Q_SIGNALS:
  void objectInserted(GeoDataObject *obj);
  void objectRemoved(GeoDataObject *obj);
  void valueChanged();

public Q_SLOTS:
  VisDataObject* createDefaultVisualizationObject();
  LookupTableWidget* createDefaultLookupTable();

protected:
  Vector2f mMinmaxVal;

private:
  Q_DISABLE_COPY(GeoDataObject);
  GeometryObjectType* mObject; // Pointer to object owned by the DatasetManager!
                               // Do not delete this pointer!
  bool mShowObject;
  QRectF _dataRequestBoundingBox;
};

#endif //INC_GEODATAOBJECT_H_


