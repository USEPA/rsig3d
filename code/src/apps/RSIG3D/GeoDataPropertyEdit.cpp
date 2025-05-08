/******************************************************************************
PURPOSE: GeoDataPorpertyEdit.C - Defines the GeoDataPropertyEdit class

NOTES: Implemented using QPropertyEditor package

HISTORY: 2013/03/08, Nan Zhang <zhang.nan@epa.gov>, Created.
******************************************************************************/

#include <QDebug>
#include <QPoint>
#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QModelIndex>
#include "GeoDataPropertyEdit.h"
#include "VisDataList.h"
#include "VisDataObject.h"
#include "PointRenderer.h"
#include "LookupTableWidget.h"

/*!
 \class GeoDataPropertyEdit
  \brief QListView based property browser for a set of GeoDataObject instances.
 */

GeoDataPropertyEdit::GeoDataPropertyEdit(QWidget* parent,
                                         bool useClassName,
                                         int ) :
  PropertyEditor::PropertyEdit(parent, useClassName) {
}


GeoDataPropertyEdit::~GeoDataPropertyEdit() {
}



void GeoDataPropertyEdit::showContextMenu(const QPoint& localPos) {
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
  QAction actionVis0(QString("--New lookup table"), NULL);
  QAction actionVis1(QString("--New default renderer"), NULL);
  myMenu.addAction(&actionVis0);
  myMenu.addAction(&actionVis1);
  myMenu.addSeparator();
  QAction actionRefresh(QString("Refresh window"), NULL);
  myMenu.addAction(&actionRefresh);
  const QPoint globalPos = this->mapToGlobal( localPos );
  QAction* selectedItem = myMenu.exec(globalPos);

  if (selectedItem == &actionDel) {
    deleteObjectWithConfirmation(objID);
    update();
  } else if (selectedItem == &actionSortInc) {
    sortObjectsByName(1);
    update();
  }  else if (selectedItem == &actionSortDec) {
    sortObjectsByName(0);
    update();
  } else if (selectedItem == &actionRefresh) {
    Q_EMIT propertyChanged();
    update();
  }
}



void GeoDataPropertyEdit::mouseReleaseEvent ( QMouseEvent * event ) {
  QTreeView::mouseReleaseEvent(event);
}



void GeoDataPropertyEdit::mousePressEvent ( QMouseEvent * event ) {
  QTreeView::mousePressEvent(event);
}



