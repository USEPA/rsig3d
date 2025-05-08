/******************************************************************************
PURPOSE: GeoDataPorpertyEdit.h - Defines the GeoDataPropertyEdit class

NOTES: Implemented using QPropertyEditor package

HISTORY: 2013/03/08, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#ifndef INC_GEODATAPROPERTYEDIT_H_
#define INC_GEODATAPROPERTYEDIT_H_

#include <PropertyEditor/propertyeditor.h>

class GeoDataPropertyEdit: public PropertyEditor::PropertyEdit {
  Q_OBJECT

public:

  GeoDataPropertyEdit(QWidget *parent, bool useClassName, int useClassDepth=1);
  ~GeoDataPropertyEdit();

protected Q_SLOTS:
  virtual void showContextMenu(const QPoint&);

protected:
  void mousePressEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );

private:
  Q_DISABLE_COPY(GeoDataPropertyEdit);
};


#endif  //INC_GEODATAPROPERTYEDIT_H_


