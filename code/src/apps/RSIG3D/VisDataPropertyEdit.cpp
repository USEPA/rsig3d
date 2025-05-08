/******************************************************************************
PURPOSE: VisDataPorpertyEdit.C - Defines the VisDataPropertyEdit class

NOTES: Implemented using QPropertyEditor package

HISTORY: 2013/03/08, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#include <QDebug>
#include <QPoint>
#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QModelIndex>
#include <PropertyEditor/propertymodel.h>
#include "VisDataPropertyEdit.h"


/*!
 \class VisDataPropertyEdit
 
 \brief Property editor for visualization objects.
 
 This class is based on the PropertyEditor class in the PropertyEdit library. 
 It has a QListView based object browser, and in enhanced with an object menu. 
 */


/*!
 Constructor.
 */
VisDataPropertyEdit::VisDataPropertyEdit(QWidget *parent, bool useClassName, int useClassDepth):
    PropertyEditor::PropertyEdit(parent, useClassName, useClassDepth) {
}




/*!
 Destructor.
 */
VisDataPropertyEdit::~VisDataPropertyEdit() {
}



void VisDataPropertyEdit::showContextMenu(const QPoint& localPos) {
  const QModelIndex mindex = this->indexAt(localPos);
  const QModelIndex topIndex = this->topLevelModelIndex(mindex);

  if (!topIndex.isValid()) {
    return;
  }

  const int objID = topIndex.row();
  QMenu myMenu(this);
  QAction actionDel(QString("Delete current object"), NULL);
  myMenu.addAction(&actionDel);
  myMenu.addSeparator();

  QAction actionSortInc(QString("Sort by name acendingly"), NULL);
  QAction actionSortDec(QString("Sort by name decendingly"), NULL);
  myMenu.addAction(&actionSortInc);
  myMenu.addAction(&actionSortDec);
  myMenu.addSeparator();

  QAction actionRefresh(QString("Refresh Window"), NULL);
  myMenu.addAction(&actionRefresh);

  const QPoint globalPos = this->mapToGlobal( localPos );
  QAction* selectedItem = myMenu.exec(globalPos);

  if (selectedItem == &actionDel) {
    deleteObjectWithConfirmation(objID);
    update();
  } else if (selectedItem == &actionSortInc) {
    sortObjectsByName(1);
    update();
  } else if (selectedItem == &actionSortDec) {
    sortObjectsByName(0);
    update();
  } else if (selectedItem == &actionRefresh) {
    Q_EMIT propertyChanged();
    update();
  }
}



void VisDataPropertyEdit::mouseReleaseEvent( QMouseEvent* event ) {
  QTreeView::mouseReleaseEvent(event);
}



void VisDataPropertyEdit::mousePressEvent( QMouseEvent* event ) {
  QTreeView::mousePressEvent(event);
}



void VisDataPropertyEdit::onParentObjectRemoved( QObject* obj ) {

  if (m_model==NULL) {
    return;
  }

  QObject *ptr;
  int i=0;

  while ( ( ptr=object( i ) ) != NULL ) {

    if (ptr->parent() == obj) {
      removeObject(i);
    } else {
      i++;
    }
  }
}




