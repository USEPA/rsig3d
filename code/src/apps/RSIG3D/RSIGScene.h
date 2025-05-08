#ifndef RSIGSCENE_H
#define RSIGSCENE_H

/******************************************************************************
PURPOSE: RSIGScene.h - A scene graph class for integrating RSIG3D widgets and
         3D scene.

NOTES: Based on OpenGLScene class.

HISTORY: 2012-11-12 zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
STATUS:  unreviewed untested
******************************************************************************/

#include <QWidget>       //QWidget desclare
#include <QDomElement>
#include <QScopedPointer>

#include <Dataset.h> // For class emvl::Dataset.

#include "OpenGLScene.h"
#include "RSIGGlobal.h"
#include "VisDataObject.h"
#include "CMAQFileReaderModel.h" // For class CMAQFileReaderModel.

//map widgets from the library Marble 
namespace Marble {
  class MarbleWidget;
  class MarbleNavigator;
};

//map widget from the library QMapControl
namespace qmapcontrol {
  class MapControl;
  class MapAdapter;
  class MapLayer;
};

class QRectF;
class QPointF;
class QLabel;
class QWidget;
class QGraphicsWidget;
class LonlatBoundingBoxWidget;

class RSIGScene : public OpenGLScene {
  Q_OBJECT

public:

  RSIGScene();
  ~RSIGScene();
  QDomElement domElement(const QString& name, QDomDocument& document) const;
  void initFromDOMElement(const QDomElement& element);

  Marble::MarbleWidget *marbleWidget();
    
  virtual void draw3DSceneEMVL(QPainter *painter);
  virtual void drawDisplayPanelSceneEMVL(QPainter *painter);
  void resetView2D();
  void resetView3D(int flag); // 1=reset, 4=autozoom (same as VisOptions: viewResetFlag)
  virtual void draw2DSceneEMVL (QPainter *painter);
  void draw2DTrueColor (QPainter *painter);

  bool ceilometerCCACreated;
  bool ceilometerCCADrawFlag;
  bool ceilometerCCADrawn;
  std::vector< bool > ceilometerCurtainComputed;

  bool pandoraCCACreated;
  bool pandoraCCADrawFlag;
  bool pandoraCCADrawn;
  std::vector< bool > pandoraCurtainComputed;


  bool ceilometerShowRegridWarning;
  bool pandoraShowRegridWarning;
  bool neubrewShowRegridWarning;

  int numCeilometerVars;
  int numPandoraVars;

  QString getResourceDirectory();
  void drawMapOverlay();

  float * getProbeLocation();
  void drawTimeseries();
  void drawPanelTimeseries(QPainter *painter);
  int* panelPlot1Init(QPainter *painter, int nPlots, int maxPlots, QString probeTitle, QString targetTitle, int x0, int y0, int width, int height, double xmin, double xmax, double ymin, double ymax);
  void panelPlot2Init(QPainter *painter, QString title, int x0, int y0, int width, int height);
  double* panelPlot3Init(QPainter *painter, QString title, QString probeUnits, int x0, int y0, int width, int height, double xmin, double xmax, double ymin, double ymax);

  QPoint panelPlot1DatatoScreen(QPointF dataPoint, int xOrigin, int yOrigin, int axisWidth, int axisHeight, double xmin, double xmax, double ymin, double ymax);


  void panelPlot1AddPoint(QPainter *painter, int plotNum, QPoint screenPoint, bool highlightFlag);

  void panelPlot3AddTimeseries(QPainter *painter, QString note, int plotIndex, double* conversion, std::vector<double> timeseries, double xmin, double xmax, double ymin, double ymax);

  virtual void draw3DScene(QPainter *painter);
  virtual void draw2DScene (QPainter *painter);
  virtual void draw2DSceneLegend (QPainter *painter);

  QPointF toScreenPointF(const QPointF& geopoint) const;
  QRectF getBoundingGeoBox() const { return screenGeoRectF(); }

  // Get reference to last instance created (Singleton?).

  static RSIGScene& getInstance() { return *lastInstanceSceneHACK; } 

Q_SIGNALS:

  void boundingGeoBoxChanged(const QRectF& rect);

public Q_SLOTS:

  void setBackgroundColor();
  void setMapServer(const char *theme);
  void setMapProjection(int mode);
  void setBoundingGeoBox(const QRectF& rectf);
  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
  void zoomIn();
  void zoomOut();
  void zoomChanged(int);
  void goHome();
  void goAutozoom();
  void goLambert();
  void goStereographic();
  void centerAt(const QRectF &);
  void centerAt(const QPointF &);
  void centerByOffset(qreal dx, qreal dy, qreal sdx, qreal sdy);

  QRectF toScreenRectF(const QRectF& geoRectF) const;
  QPointF toGeoPointF(const QPointF& screenCoord) const;
  QRectF screenGeoRectF() const;
  void setScreenGeoRectF(const QRectF & rect);
  bool isSphericalProj() const;
  bool isLambertConformalConicProj() const;
  bool isStereographicProj() const;

protected:

  friend class GraphicsView;

  QPoint navigatorPosition() const;
  void setNavigatorPosition(const QPoint& pos);

  void mousePressEvent ( QGraphicsSceneMouseEvent * event );
  void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
  void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
  void wheelEvent ( QGraphicsSceneWheelEvent * wheelEvent );
  void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
  void resizeEvent(QResizeEvent * event);

  virtual void drawBackground(QPainter *painter, const QRectF &rectf);
  virtual void drawForeground(QPainter *painter, const QRectF &rectf);

  QPointF screenGeoCenterF() const;
  void setScreenGeoCenterF(const QPointF &);

  QRectF enforceMapLimits(QRectF inRect);
  void updateProbe();
  void updateScene();

private:

  static RSIGScene* lastInstanceSceneHACK;

  void createMarbleAndQMapWidgets();
  void createOtherWidgets();
  int marbleZoomValueToQMap(int zoomval) const;
  int qmapZoomValueToMarble(int zoomval) const;

  void drawLongLatLines(QPainter *painter);
  void drawRegionOfInterest(QPainter *painter);
  void drawGraphTitle(QPainter *painter);
  void drawCenteringRectangle(QPainter *painter);
  void drawLookupTables(QPainter *painter, const QRect& screenregion);
  void drawCurvedLine(QPainter *painter, const QPointF& lonlat0, const QPointF& lonlat1);

  QRectF m_homeRect;        //home rect, in geo space (long-lat pair)
  QRectF m_roiRect;         //region-of-interest rect, in screen space
  QRectF m_crectRect;       //center of zooming rect, in screen space
  QPointF m_prevGeoPos;     //previous step geographical location
  QPointF m_startGeoPos;    //start geographical location at mouse button down
  QPointF m_prevScreenPos;  //previous step screen location
  QPointF m_startScreenPos; //start screen location at mouse button down

  int m_useMarbleWidget;
  int m_projectionMode;
  int m_marbleMaxZoom;
  int m_mapcontrolMinZoom;
  int m_mapcontrolMaxZoom;

  bool m_dragRoiMode;       //dragging region-of-interest
  bool m_dragCRectMode;     //dragging centering rectangle
  Marble::MarbleWidget *m_marbleWidget;
  Marble::MarbleNavigator *m_marbleNavigator;
  qmapcontrol::MapControl *m_mapControl;
  LonlatBoundingBoxWidget* m_boundingBox;

  QScopedPointer<qmapcontrol::MapLayer> m_googleMapLayer;
  QScopedPointer<qmapcontrol::MapLayer> m_googleTerrainMapLayer;
  QScopedPointer<qmapcontrol::MapLayer> m_googleSatMapLayer;
  QScopedPointer<qmapcontrol::MapLayer> m_googleSatHybridMapLayer;
  QScopedPointer<qmapcontrol::MapLayer> m_osmMapLayer;
  QScopedPointer<qmapcontrol::MapLayer> m_mapquestMapLayer;

  QGraphicsWidget* m_mapProxy;
  QGraphicsWidget* m_marbleProxy;
  QGraphicsWidget* m_navigatorProxy;

  QColor m_backgroundColor;

public:

  void renderBitmapString(float x, float y, float z, void *font, char *string);
  void reloadModisTrueColor(int dayNum);

  // Return non-LUT VisDataObject corresponding to a dataset:

  VisDataObject*
  getVisDataObjectForDataset(const emvl::Dataset* const dataset) const;

  CMAQFileReaderModel cmaqFileReaderModel;
};

#endif /* RSIGSCENE_H */
