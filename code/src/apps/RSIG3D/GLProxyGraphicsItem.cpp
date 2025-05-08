/******************************************************************************
PURPOSE: GLProxyGraphicsItem.cpp - Implements a full-window proxy item for QGraphicsItem.

NOTES: Implemented using QT's QGraphicsScene class.

 dummy class to represent a OpenGL scene graph as a QGraphicsItem.

 For mixed rendering of OpenGL based 3D objects and QPainter based 2D objects,
 the 3d objects should be placed into the QGraphicsScene.
 Therefore, a QGraphicsItem should be created in the scene graph.
 The item is shown in full-screen. When the rendering function of this item is called,
 the 3D objects in it are then rendered using OpenGL APIs.
 This is the technique used in RSIG3D for mixed rendering.

 The render order of 3D objects are determined by the user.

 THe rendering sequence of the 3D reprensenting item and other 2D QPainter objects are determined by
 a Z-value used in QGraphicsItem class. See the corresponding document for QT for details.

 a QGraphicsView, QGraphicsScene, QGraphicsItem

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
******************************************************************************/

#if _MSC_VER
#include <qt_windows.h>
#endif
#include <iostream>
#include <QtOpenGL>
#include "GLProxyGraphicsItem.h"
#include "OpenGLScene.h"

#include "VisOptions.h"
#include "ui_controldockwidget.h"   //UI form

const int GL_WIN_WIDTH=64;
const int GL_WIN_HEIGHT=64;



GLProxyGraphicsItem::GLProxyGraphicsItem(QGraphicsItem * parent):
  QGraphicsRectItem(0, 0, GL_WIN_WIDTH, GL_WIN_HEIGHT, parent) {

  setFlag(QGraphicsItem::ItemIsMovable, false);
  setFlag(QGraphicsItem::ItemIsSelectable, false);
  setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  setCacheMode(QGraphicsItem::NoCache);    //default mode is NoCache
  setOpacity(0.999);

  //To disable mouse events for an item (i.e., make it transparent for mouse events),
  setAcceptedMouseButtons(0);
}



/*!
 \fn QRectF GLProxyGraphicsItem::boundingRect ( void ) const 
 
 \brief A derived class of QGraphicsItem must override this function to provide its own rect size
 */

QRectF GLProxyGraphicsItem::boundingRect ( void ) const {
  QRectF rectf(0, 0, GL_WIN_WIDTH, GL_WIN_HEIGHT);
  OpenGLScene *scenePtr = dynamic_cast<OpenGLScene*>(scene());

  if (scenePtr) {
    rectf= scenePtr->storedSceneRect();
  }

  return rectf;
}



/*!
 \fn void GLProxyGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
 
 \brief Customized paint function must be provided.
*/

void GLProxyGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
  OpenGLScene* scenePtr = dynamic_cast<OpenGLScene*>(scene());

  if (scenePtr) {
    painter->save();
    VisOptions *vOptions = VisOptionsSingleton::getInstance();
    //bool mode2D = vOptions->getMode2D();
    int displayMode = vOptions->getDisplayMode();
    //qDebug() << "displaymode = " << displayMode; 
    //if (mode2D) {
    if (displayMode == 0) { // 2D
      scenePtr->draw3DScene(painter);
    } else if (displayMode == 1) { // 3D
      scenePtr->draw3DSceneEMVL(painter);
    } else if (displayMode == 2) { // Display panel
      scenePtr->drawDisplayPanelSceneEMVL(painter);
    }

    painter->restore();
  }
}




