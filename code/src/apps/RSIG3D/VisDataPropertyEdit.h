/******************************************************************************
PURPOSE: VisDataPorpertyEdit.h - Defines the VisDataPropertyEdit class

NOTES: Implemented using QPropertyEditor package

HISTORY: 2013/03/08, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#ifndef INC_VISDATAPROPERTYEDIT_H_
#define INC_VISDATAPROPERTYEDIT_H_

#include <PropertyEditor/propertyeditor.h>

class VisDataPropertyEdit: public PropertyEditor::PropertyEdit {
  Q_OBJECT

public:
  VisDataPropertyEdit(QWidget *parent, bool useClassName, int useClassDepth = 1);
  ~VisDataPropertyEdit();

public Q_SLOTS:
  virtual void onParentObjectRemoved(QObject*);

protected Q_SLOTS:
  virtual void showContextMenu(const QPoint&);

protected:
  void mousePressEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );

private:
  Q_DISABLE_COPY(VisDataPropertyEdit);
};

#endif  //INC_VISDATAPROPERTYEDIT_H_


