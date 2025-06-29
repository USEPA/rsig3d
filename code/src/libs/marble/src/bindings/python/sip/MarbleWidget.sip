//
// Copyright 2008 Simon Edwards <simon@simonzone.com>
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public 
// License along with this library.  If not, see <http://www.gnu.org/licenses/>.
//

namespace Marble
{

class MarbleWidget : QWidget
{
%TypeHeaderCode
#include <MarbleWidget.h>
%End

public:
    explicit                MarbleWidget (QWidget* parent /TransferThis/ = 0);
    Marble::MarbleModel*    model () const;
    int                     radius () const;
    void                    setRadius (int radius);
    int                     zoom () const;
    qreal                   distance () const;
    void                    setDistance (qreal distance);
    QString                 distanceString () const;
    int                     minimumZoom () const;
    int                     maximumZoom () const;
    qreal                   centerLongitude () const;
    qreal                   centerLatitude () const;
    QPixmap                 mapScreenShot ();
    bool                    showOverviewMap () const;
    bool                    showScaleBar () const;
    bool                    showCompass () const;
    bool                    showClouds () const;
    bool                    showAtmosphere () const;
    bool                    showCrosshairs () const;
    bool                    showGrid () const;
    bool                    showPlaces () const;
    bool                    showCities () const;
    bool                    showTerrain () const;
    bool                    showOtherPlaces () const;
    bool                    showRelief () const;
    bool                    showIceLayer () const;
    bool                    showBorders () const;
    bool                    showRivers () const;
    bool                    showLakes () const;
    bool                    showFrameRate () const;
    quint64                 volatileTileCacheLimit () const;
    QList<Marble::RenderPlugin*>  renderPlugins () const;
    QList<Marble::AbstractFloatItem*>  floatItems () const;
    Marble::Projection      projection () const;
    QString                 mapThemeId () const;
    Marble::ViewContext     viewContext () const;
    bool                    animationsEnabled () const;
    Marble::AngleUnit       defaultAngleUnit () const;
    void                    setDefaultAngleUnit (Marble::AngleUnit angleUnit);
    QFont                   defaultFont () const;
    void                    setDefaultFont (const QFont& font);
    void                    zoomView (int zoom, Marble::FlyToMode mode = Marble::Instant);
    void                    zoomViewBy (int zoomStep, Marble::FlyToMode mode = Marble::Instant);
    void                    zoomIn (Marble::FlyToMode mode = Marble::Automatic);
    void                    zoomOut (Marble::FlyToMode mode = Marble::Automatic);
    void                    centerOn (const Marble::GeoDataCoordinates& point, bool animated = 0);
    void                    setCenterLatitude (qreal lat, Marble::FlyToMode mode = Marble::Instant);
    void                    setCenterLongitude (qreal lon, Marble::FlyToMode mode = Marble::Instant);
    void                    setProjection (int projection);
    void                    setProjection (Marble::Projection projection /Constrained/);
    void                    moveLeft (Marble::FlyToMode mode = Marble::Automatic);
    void                    moveRight (Marble::FlyToMode mode = Marble::Automatic);
    void                    moveUp (Marble::FlyToMode mode = Marble::Automatic);
    void                    moveDown (Marble::FlyToMode mode = Marble::Automatic);
    void                    goHome (Marble::FlyToMode mode = Marble::Automatic);
    void                    setMapThemeId (const QString& maptheme);
    void                    setPropertyValue (const QString& name, bool value);
    void                    setShowOverviewMap (bool visible);
    void                    setShowScaleBar (bool visible);
    void                    setShowCompass (bool visible);
    void                    setShowClouds (bool visible);
    void                    setShowAtmosphere (bool visible);
    void                    setShowCrosshairs (bool visible);
    void                    setShowGrid (bool visible);
    void                    setShowPlaces (bool visible);
    void                    setShowCities (bool visible);
    void                    setShowTerrain (bool visible);
    void                    setShowOtherPlaces (bool visible);
    void                    setShowRelief (bool visible);
    void                    setShowIceLayer (bool visible);
    void                    setShowBorders (bool visible);
    void                    setShowRivers (bool visible);
    void                    setShowLakes (bool visible);
    void                    setShowFrameRate (bool visible);
    void                    setShowTileId (bool visible);
    void                    notifyMouseClick (int x, int y);
    void                    clearVolatileTileCache ();
    void                    setVolatileTileCacheLimit (quint64 kiloBytes);
    void                    creatingTilesStart (Marble::TileCreator* creator, const QString& name, const QString& description);
    void                    setViewContext (Marble::ViewContext viewContext);
    void                    setAnimationsEnabled (bool enabled);
    void                    setSelection (const QRect& region);

signals:
    void                    zoomChanged (int zoom);
    void                    distanceChanged (const QString& distanceString);
    void                    projectionChanged (Marble::Projection);
    void                    mouseClickGeoPosition (qreal lon, qreal lat, Marble::GeoDataCoordinates::Unit);
    void                    framesPerSecond (qreal fps);

protected:
    void                    leaveEvent (QEvent* event);
    void                    paintEvent (QPaintEvent* event);
    virtual void            customPaint (Marble::GeoPainter* painter);
    void                    resizeEvent (QResizeEvent* event);
    void                    connectNotify (const char* signal);
    void                    disconnectNotify (const char* signal);

public:
    virtual ~MarbleWidget ();
//ig    MarbleWidgetInputHandler*  inputHandler () const;
//ig    void                    setInputHandler (MarbleWidgetInputHandler* handler);
    void                    readPluginSettings (QSettings& settings);
    void                    writePluginSettings (QSettings& settings) const;
//ig    Marble::GeoSceneDocument*  mapTheme () const;
    void                    setInputEnabled (bool);
    void                    centerOn (const Marble::GeoDataLatLonBox& box, bool animated = 0);
signals:
//ig    void                    regionSelected (const QList<double>&);
    void                    pluginSettingsChanged ();
    void                    renderPluginInitialized (Marble::RenderPlugin* renderPlugin);
protected:
    void                    changeEvent (QEvent* event);
public:
//ig    Marble::ViewportParams*  viewport ();
    const Marble::ViewportParams*  viewport () const;
    Marble::GeoDataLookAt   lookAt () const;
    Marble::AbstractFloatItem*  floatItem (const QString& nameId) const;
//ig    RoutingLayer*           routingLayer ();
    void                    rotateBy (const qreal deltaLon, const qreal deltaLat, Marble::FlyToMode mode = Marble::Instant);
    void                    centerOn (const qreal lon, const qreal lat, bool animated = 0);
    void                    flyTo (const Marble::GeoDataLookAt& lookAt, Marble::FlyToMode mode = Marble::Automatic);
    void                    reloadMap ();
signals:
    void                    themeChanged (const QString& theme);
    void                    mouseMoveGeoPosition (const QString&);
    void                    visibleLatLonAltBoxChanged (const Marble::GeoDataLatLonAltBox& visibleLatLonAltBox);
public:
    int                     tileZoomLevel () const;
    QList<Marble::AbstractDataPluginItem*>  whichItemAt (const QPoint& curpos) const;
    void                    downloadRegion (const QString& sourceDir, const QVector<Marble::TileCoordsPyramid>&);
signals:
    void                    tileLevelChanged (int level);
public:
    void                    addLayer (Marble::LayerInterface* layer);
    void                    removeLayer (Marble::LayerInterface* layer);
    qreal                   radiusFromDistance (qreal distance) const;
    qreal                   distanceFromRadius (qreal radius) const;
    qreal                   zoomFromDistance (qreal distance) const;
    qreal                   distanceFromZoom (qreal zoom) const;
    bool                    showBackground () const;
    void                    setShowBackground (bool visible);
    void                    setMapQualityForViewContext (Marble::MapQuality quality, Marble::ViewContext viewContext);
//FIXME
//ig    QVector<const Marble::GeoDataPlacemark*>  whichFeatureAt (const QPoint&) const;
//ig    MarbleWidgetPopupMenu*  popupMenu ();
    bool                    showSunShading () const;
    bool                    showCityLights () const;
    bool                    showSunInZenith () const;
    void                    centerOn (const Marble::GeoDataPlacemark& placemark, bool animated = 0);
    void                    setShowSunShading (bool visible);
    void                    setShowCityLights (bool visible);
    void                    setShowSunInZenith (bool visible);
    QRegion                 mapRegion () const;
    bool                    screenCoordinates (qreal lon, qreal lat, qreal& x, qreal& y) const;
    bool                    geoCoordinates (int x, int y, qreal& lon, qreal& lat, Marble::GeoDataCoordinates::Unit = Marble::GeoDataCoordinates::Degree) const;
    qreal                   moveStep () const;
    Marble::GeoDataCoordinates  focusPoint () const;
    void                    setFocusPoint (const Marble::GeoDataCoordinates& focusPoint);
    void                    resetFocusPoint ();
    Marble::MapQuality      mapQuality (Marble::ViewContext = Marble::Still) const;
//ig    const TextureLayer*     textureLayer () const;
};
// MarbleWidget

};
// Marble


