//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2007 Torsten Rahn <tackat@kde.org>
// Copyright 2007      Inge Wallin  <ingwa@kde.org>
// Copyright 2010-2012 Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

//
// PlacemarkLayout is responsible for drawing the Placemarks on the map
//

#ifndef MARBLE_PLACEMARKLAYOUT_H
#define MARBLE_PLACEMARKLAYOUT_H


#include <QtCore/QHash>
#include <QtCore/QModelIndex>
#include <QtCore/QRect>
#include <QtCore/QVector>
#include <QtGui/QSortFilterProxyModel>

#include "GeoDataFeature.h"

class QAbstractItemModel;
class QItemSelectionModel;
class QPoint;


namespace Marble
{

class GeoDataCoordinates;
class GeoDataPlacemark;
class GeoDataStyle;
class GeoPainter;
class MarbleClock;
class PlacemarkPainter;
class TileId;
class VisiblePlacemark;
class ViewportParams;

/**
 * Layouts the place marks with a passed QPainter.
 */



class PlacemarkLayout : public QObject
{
    Q_OBJECT

 public:
    /**
     * Creates a new place mark layout.
     */
    PlacemarkLayout( QAbstractItemModel  *placemarkModel,
                     QItemSelectionModel *selectionModel,
                     MarbleClock *clock,
                     QObject *parent = 0 );

    /**
     * Destroys the place mark painter.
     */
    ~PlacemarkLayout();

    /**
     * @reimp
     */
    QVector<VisiblePlacemark *> generateLayout( const ViewportParams *viewport );

    /**
     * Returns a list of model indexes that are at position @p pos.
     */
    QVector<const GeoDataPlacemark*> whichPlacemarkAt( const QPoint &pos );

    QString runtimeTrace() const;

 public Q_SLOTS:
    // earth
    void setShowPlaces( bool show );
    void setShowCities( bool show );
    void setShowTerrain( bool show );
    void setShowOtherPlaces( bool show );

    // other planets
    void setShowLandingSites( bool show );
    void setShowCraters( bool show );
    void setShowMaria( bool show );

    void requestStyleReset();
    void setCacheData();

 Q_SIGNALS:
    void repaintNeeded();

 private:
    /**
     * Returns a the maximum height of all possible labels.
     * WARNING: This is a really slow method as it traverses all placemarks
     * to check the labelheight.
     * FIXME: Once a StyleManager that manages all styles has been implemented
     * just traverse all existing styles.
     */
    int maxLabelHeight() const;

    void styleReset();

    QList<const GeoDataPlacemark*> visiblePlacemarks( const ViewportParams *viewport );
    bool layoutPlacemark( const GeoDataPlacemark *placemark, qreal x, qreal y, bool selected );

    /**
     * Returns the coordinates at which an icon should be drawn for the @p placemark.
     * @p ok is set to true if the coordinates are valid and should be used for drawing,
     * it it set to false otherwise.
     */
    GeoDataCoordinates placemarkIconCoordinates( const GeoDataPlacemark *placemark, bool *ok ) const;

    QRectF  roomForLabel( const GeoDataStyle * style,
                         const qreal x, const qreal y,
                         const QString &labelText );

    bool    placemarksOnScreenLimit( const QSize &screenSize ) const;

 private:
    Q_DISABLE_COPY( PlacemarkLayout )
    QSortFilterProxyModel  m_placemarkModel;
    QItemSelectionModel *const m_selectionModel;
    MarbleClock *const m_clock;

    QVector<VisiblePlacemark*> m_paintOrder;
    QString m_runtimeTrace;
    int m_labelArea;
    QHash<const GeoDataPlacemark*, VisiblePlacemark*> m_visiblePlacemarks;
    QVector< QVector< VisiblePlacemark* > >  m_rowsection;

    /// map providing the list of placemark belonging in TileId as key
    QMap<TileId, QList<const GeoDataPlacemark*> > m_placemarkCache;

    const QVector< GeoDataFeature::GeoDataVisualCategory > m_acceptedVisualCategories;

    // earth
    bool m_showPlaces;
    bool m_showCities;
    bool m_showTerrain;
    bool m_showOtherPlaces;

    // other planets
    bool m_showLandingSites;
    bool m_showCraters;
    bool m_showMaria;

    int     m_maxLabelHeight;
    bool    m_styleResetRequested;
};

}

#endif
