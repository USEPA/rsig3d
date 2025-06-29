//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010      Dennis Nienhüser <earthwings@gentoo.org>
//

#ifndef MARBLE_ROUTINGLAYER_H
#define MARBLE_ROUTINGLAYER_H

#include "GeoDataCoordinates.h"
#include "LayerInterface.h"
#include "RoutingManager.h"

#include <QtCore/QModelIndex>
#include <QtGui/QItemSelection>

class QAbstractProxyModel;
class QComboBox;

namespace Marble
{

class MarbleWidget;
class RoutingModel;
class MarblePlacemarkModel;
class RoutingLayerPrivate;
class AlternativeRoutesModel;

/**
  * @brief A paint layer that serves as a view on a route model
  */
class RoutingLayer: public QObject, public LayerInterface
{
    Q_OBJECT

public:
    /**
      * @brief Constructor
      * @param widget The marble widget used for geopos <-> screenpos transformations
      *   and repainting of (small) areas. Must not be null
      * @param parent Optional parent widget
      */
    explicit RoutingLayer( MarbleWidget *widget, QWidget *parent = 0 );

    /** Destructor */
    ~RoutingLayer();

    /** Reimplemented from LayerInterface. We'll hover above the surface */
    QStringList renderPosition() const;

    /** Reimplemented from LayerInterface. */
    qreal zValue() const;

    /** Reimplemented from LayerInterface. Paints route items and placemarks */
    bool render( GeoPainter *painter, ViewportParams *viewport,
                 const QString &renderPos = "NONE", GeoSceneLayer *layer = 0 );

    /**
      * Set the proxy model another QAbstractItemView uses that should share
      * its selection model with us. Needed because this class uses an unfiltered
      * model which has different indices than a filtered one.
      */
    void synchronizeWith( QItemSelectionModel *selection );

    /**
      * Stores the alternative routes model and a view working on this model to synchronize
      * the selection with.
      * @todo: Should use a QAbstractItemView instead, but working on this instead of the
      * QComboBox does not work (changing the selection is not reflected by the combo box)
      */
    void synchronizeAlternativeRoutesWith( QComboBox *view );

    /**
      * Set the placemark model to use. Implicitly removes the routing model.
      */
    void setPlacemarkModel ( MarblePlacemarkModel *model );

    /**
      * When enabled, a cross hair cursor is shown in the map and each click
      * in the map is reported via the pointSelected signal. Mouse move,
      * press and release events are fully eaten to avoid things like context
      * menus chiming in.
      */
    void setPointSelectionEnabled( bool enabled );

    /**
      * Set the view context to determine whether the map is used interactively
      */
    void setViewContext( ViewContext viewContext );

Q_SIGNALS:
    /**
      * A placemark was selected (clicked) by the user. The index belongs to
      * the model set via setModel
      */
    void placemarkSelected( const QModelIndex &index );

    /**
      * A point was selected by a mouse click after setPointSelectionEnabled(true) was called
      */
    void pointSelected( const GeoDataCoordinates &coordinates );

    /**
      * Selection of points was aborted by the user without selecting a point
      */
    void pointSelectionAborted();

    /**
      * The route request was changed permanently (no dragging),
      * new route instructions should be downloaded
      */
    void routeDirty();

protected:
    /** Overriding QWidget, used to make the layer interactive */
    bool eventFilter( QObject *obj, QEvent *event );

private Q_SLOTS:
    void removeViaPoint();

    void showAlternativeRoutes();

    /** Export route to a file */
    void exportRoute();

    /**
      * Paint a dashed route when downloading a new route, a solid one otherwise.
      */
    void updateRouteState( RoutingManager::State state );

    /**
      * The viewport has changed, recalculate positions accordingly
      */
    void setViewportChanged();

private:
    RoutingLayerPrivate *const d;
    friend class RoutingLayerPrivate;

};

} // namespace Marble

#endif
