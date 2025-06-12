/******************************************************************************
PURPOSE: RSIGScene.C - Implements rsigscene.

NOTES: Similar to the model class in Model/View/Control Pattern,
       this class  contains all of the model data.

HISTORY: 2012-11-12 zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov
******************************************************************************/

#include <string.h> // For strchr().
#include <math.h>   // For fabs().
#include <assert.h> // For macro assert().

#include <string>    // For class std::string.
#include <vector>    // For class std::vector.
#include <set>       // For class std::set.
#include <algorithm> // For std::min(), std::max().
#include <iostream>
#include <QDesktopServices>      //QDesktopServices::openUrl(QUrl);

#if _MSC_VER
#define NOMINMAX
#include <qt_windows.h>
#endif

#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <GLUT/glut.h> /* For glut*. */
#else
#include <GL/glu.h>
#include <GL/glut.h> /* For gl*. */
#endif

#include <QScopedPointer>
#include <QString>
#include <QDir>
#include <QGLWidget>
#include <QImage>

//QMapControl library header files
#include <QMapControl/mapcontrol.h>
#include <QMapControl/osmmapadapter.h>
#include <QMapControl/openaerialmapadapter.h>
#include <QMapControl/wmsmapadapter.h>
#include <QMapControl/yahoomapadapter.h>
#include <QMapControl/googlesatmapadapter.h>
#include <QMapControl/googlemapadapter.h>
#include <QMapControl/maplayer.h>

//Marble lib header files (are all in a flattened directory)
#include <marble/GeoDataLatLonBox.h>
#include <marble/MarbleWidget.h>
#include <marble/MarbleNavigator.h>
#include <marble/ViewportParams.h>
#include <marble/LambertConformalConicProjection.h>
#include <marble/StereographicProjection.h>

#include <Dataset.h>        // For class emvl::Dataset.
#include <PointDataset.h>   // For class emvl::PointDataset.
#include <DatasetManager.h> // For class emvl::DatasetManager.

#include "GraphicsView.h"
#include "RSIGGlobal.h"
#include "RSIGScene.h"
#include "DomUtils.h"
#include "RenderingContextRSIG.h"
#include "LookupTableWidget.h"
#include "SlantLookupTableWidget.h"
#include "VisDataList.h"
#include "GeoDataList.h"
#include "LonlatBoundingBoxWidget.h"
#include "GeoDataObject.h"

#include "MainWin.h"
#include "ControlDockWidget.h"
#include "ui_controldockwidget.h"
#include "loadPNG.h"
#include "VisOptions.h"
#include "LoadMapFile.h"
#include "VisDataObject.h" // For setPointSpriteTexture().
#include "RGBColormap.h"

#include "CeilometerCurtainArray.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#ifndef M_PI_2
#define M_PI_2 (M_PI*0.5)
#endif


static GLuint mapTextureId = 0;
static GLuint mapLDisplayList = 0;
static GLuint mapOverlayDisplayList = 0;

static bool mouseDown;
static int xLast = 0;
static int yLast = 0;

static const char* lastMapTheme = "";

// GL parameters
static float zNear = 100.0;
static float zFar  = 10000.0;
static float aspect;

static const float fov_angle_init = 15.0; // field of view angle
static const float eyeX_init = 0.0;
static const float eyeY_init = 0.0;
static const float eyeZ_init = 1000.0;
static const float centerX_init = 0.0;
static const float centerY_init = 0.0;
static const float centerZ_init = 0.0;
static const float upX_init = 0.0;
static const float upY_init = 1.0;
static const float upZ_init = 0.0;
static const float rotAngleX_init = 0.0; // like compass heading
static const float rotAngleY_init = 0.0; // like compass heading
static const float rotAngleZ_init = 0.0; // like compass heading
static const float rotCenterLon_init = 0.0; // default rotation center
static const float rotCenterLat_init = 0.0; // default rotation center
static const float rotCenterZ_init   = 0.0; // default rotation center

static float fov_angle;
static float eyeX;
static float eyeY;
static float eyeZ;
static float centerX;
static float centerY;
static float centerZ;
static float upX;
static float upY;
static float upZ;
static float rotAngleX;
static float rotAngleY;
static float rotAngleZ;
static float rotCenterLon;
static float rotCenterLat;
static float rotCenterZ;

static int mapline_index = 0; // 0=county, 1=na, 2=world

static int draw3DInitialized=0;
static int drawRotPoint=0;
static GLdouble stored_modelview[16]; // modelview when mouse was released
static float rotCenterLon_last = 0.0;
static float rotCenterLat_last = 0.0;
static int zRotateFlag = 0;
static QPointF curScenePos;
static int sceneHeight;
static int sceneWidth;

static int topAnnotHeight;
static int bottomAnnotHeight;


static GLuint modisTrueColorTextureId = 0; // for 3D rendering
//QImage modisTrueColorImage; // for 2D rendering
static QPixmap * modisTrueColorPixmap; // for 2D rendering
static QRectF modisTrueColorRect;
static bool modisTrueColorIsValid = false; //default

static QString resource_dir; // for map files, etc
static QString previousMapOverlayFile = "";

static QString fnLast;
static QString probeMsg2 = "";
static QScopedPointer<LookupTableWidget> lut0(NULL);



RSIGScene* RSIGScene::lastInstanceSceneHACK; // Singleton static attribute.



/*!
 \class RSIGScene
 \brief The RSIGScene class is the main class for manipulating the scenegraph 
 for the RSIG3D application. 
 
 \ingroup  
 
 The OpenGLScene class only provides a rough skeleton for the scenegraph structure.
 Based on it, the RSIGScene class provides a detailed implementation for the structure.
 Specifically, the following objects/items are introduced in the scenegraph:
  
 \list
   \o MableWidget: this is an offline map system. It is used as a full-size scenegraph item, as the background object
   \o QMapControl: this is another online map system. It is used similiar to the MarbleWidget object
   \o MableNavigator: a navigation control widget, on top of the MarbleWidget and QMapControl
   \o LonlatBoundingBoxWidget: a display/input widget for showing the Longitude/Latitude region-of-interest (ROI)
   \o GeoDataList: a singleton manager of GeoDataObject
   \o VisDataList: a singleton manager of VisDataObject
 \endlist
 
 From then on, the \c update() slot is called every second.
   
 \section1 Rendering order of the objects
 For all the scene objects, we have the following rendering order:
 \list
 \o QMapControl, with depth value of -1800
 \o MarbleWidget, with depth value of -1700
 \o GLProxyGraphicsItem, the container for 3D objects, with a depth value of 0
 \o LonlatBoundingBoxWidget, with depth value of +2000
 \o MarbleNavigator, with depth value of +10,000
 \endlist
 The above objects are all contained in a QGraphicsScene class.
 
 The 3D objects, GeoDataList, and VisDataList objects are all rendered in the rendering functions of the GLProxyGraphicsItem.
 Therefore, they are in between of the rendering of maps and UI controls. 
 For more detailed information on them, see the description on GLProxyGraphicsItem.
 
 \sa OpenGLScene, QGraphicsScene, GLProxyGraphicsItem
*/



/*!
 Constructs the class instance. It creates an instance of each of the MarbleWidget, 
 QMapControl, MarbleNavigator, LonlatBoundingBoxWidget. 
 */
RSIGScene::RSIGScene()
  : OpenGLScene()
  ,ceilometerCCACreated(false)
  ,ceilometerCCADrawFlag(false)
  ,ceilometerCCADrawn(false)
  ,ceilometerShowRegridWarning(false)
  ,pandoraShowRegridWarning(false)
  ,neubrewShowRegridWarning(false)
  ,numCeilometerVars(0)
  ,numPandoraVars(0)
  ,m_useMarbleWidget(1)
  ,m_projectionMode(RSIGState::PROJECTION_LONGLAT)
  ,m_marbleMaxZoom(2500)
  ,m_mapcontrolMinZoom(1)
  ,m_mapcontrolMaxZoom(17)
  ,m_dragRoiMode(false)
  ,m_dragCRectMode(false)
  ,m_marbleWidget(0)
  ,m_marbleNavigator(0)
  ,m_mapControl(0)
  ,m_boundingBox(0)
  ,m_mapProxy(0)
  ,m_marbleProxy(0)
  ,m_navigatorProxy(0)
  ,m_backgroundColor(0, 170, 255)
  {

  try {
    const float zval = 2000;
    LonlatBoundingBoxWidget *w = m_boundingBox = new LonlatBoundingBoxWidget; //x
    w->setWindowOpacity(0.99);
    w->setAttribute(Qt::WA_TranslucentBackground);
    w->hide();
    QGraphicsWidget *p = addWidget(w);
    p->setZValue(zval);
    p->setPos(700, 600);

    createMarbleAndQMapWidgets();
    goHome();  //<== m_homeRect is set here
    m_roiRect = m_homeRect;

    if (m_boundingBox) {
      m_boundingBox->onBoundingGeoBoxChanged(m_roiRect);
      connect(m_boundingBox, SIGNAL(boundingGeoBoxChanged(const QRectF &)), this, SLOT(setBoundingGeoBox(const QRectF &)));
      connect(m_boundingBox, SIGNAL(boundingGeoBoxChanged(const QRectF &)), this, SIGNAL(boundingGeoBoxChanged(const QRectF &)));
    }
  } catch (...) {
    throw "Error in RSIGScene constructor!";
  }

  lastInstanceSceneHACK = this;
}



/*!
 Destructor. Keeps all exceptions inside itself.
 */
RSIGScene::~RSIGScene() {
  try {

    if (m_marbleWidget->parent()==NULL) {
      delete m_marbleWidget;
    }

    if (m_marbleNavigator->parent()==NULL) {
      delete m_marbleNavigator;
    }

    if (m_boundingBox) {

      if (m_boundingBox->parent()==NULL) {
        delete m_boundingBox;
      }
    }

    if (m_mapControl) {

      if (m_mapControl->parent()==NULL) {
        delete m_mapControl;
      }
    }
  } catch (...) {
    qDebug()<<"Error in RSIGScene destructor";
  }        
}



/*!
 \fn Marble::MarbleWidget *RSIGScene::marbleWidget()
 
 Return the pointer to the MarbleWidget.
 */
Marble::MarbleWidget *RSIGScene::marbleWidget() {
  return m_marbleWidget;
}



/*!
 \fn int RSIGScene::marbleZoomValueToQMap(int zoomval) const
 
 Transform the zoom factor \e zoomval used in MarbleWidget into the zoom factor used by QMapControl. 
 The two packages use different factors. 
 
 For MarbleWidget, \e zoomval is usually in the range of [800, 2300], which is actually a distance 
 between the eye and the center of the earth. It is used to compute a rad2pixel factor, which scales the 
 size. The larger of \e zoomval, the closer the map.
 
 For QMapControl, it is a value of mip-map level, usually from 1 to 16, 
 which is actually a parameter for the HTTP request to the map server. 
 At the same time, the map size is 1<<zoomval
 
 */

int RSIGScene::marbleZoomValueToQMap(int zoomval) const {
  const int marbleMinZoom = m_marbleNavigator->minimumZoom();
  const int marbleMaxZoom = m_marbleMaxZoom;
  double t = double(zoomval-marbleMinZoom)/double(marbleMaxZoom-marbleMinZoom);

  if (t>1.0) {
    t=1.0;
  }

  if (t<0.0) {
    t=0.0;
  }

  t *= t;
  t*=(m_mapcontrolMaxZoom-m_mapcontrolMinZoom);
  t+=m_mapcontrolMinZoom;
  int r = (int)(t+0.5);

  if (r<m_mapcontrolMinZoom) {
    r = m_mapcontrolMinZoom;
  }

  if (r>m_mapcontrolMaxZoom) {
    r=m_mapcontrolMaxZoom;
  }

  return r;
}



/*!
 \fn int RSIGScene::marbleZoomValueToQMap(int zoomval) const
 
 Transform the zoom factor \e zoomval used in MarbleWidget into the zoom factor used by QMapControl. 
 */
int RSIGScene::qmapZoomValueToMarble(int zoomval) const {
  const int marbleMinZoom = m_marbleNavigator->minimumZoom();
  const int marbleMaxZoom = m_marbleMaxZoom;
  double t = double(zoomval-m_mapcontrolMinZoom)/double(m_mapcontrolMaxZoom-m_mapcontrolMinZoom);

  if (t>1.0) {
    t=1.0;
  }

  if (t<0.0) {
    t=0.0;
  }

  t=sqrt(t);
  t*=(marbleMaxZoom-marbleMinZoom);
  t+=marbleMinZoom;
  int r = (int)(t+0.5);

  if (r<marbleMinZoom) {
    r=marbleMinZoom;
  }

  if (r>marbleMaxZoom) {
    r=marbleMaxZoom;
  }

  return r;
}



/*!
 \fn void RSIGScene::createMarbleAndQMapWidgets()
 
 Internal function used for creating the MarbleWidget, MarbleNavigator, and QMapControl instances. 
 */
void RSIGScene::createMarbleAndQMapWidgets() {
  int minzoom=1000, maxzoom=-minzoom;

  { //create the QMapControl layer
    m_mapControl = new qmapcontrol::MapControl(QSize(512, 512), qmapcontrol::MapControl::None);
    m_mapControl->showScale(true);

    //the initialization takes modes of <MapControl::Panning, None, Dragging>
    {
      qmapcontrol::OSMMapAdapter *mapadapter = new qmapcontrol::OSMMapAdapter;
      qmapcontrol::MapLayer* layer = new qmapcontrol::MapLayer("OSM-Layer", mapadapter, false);
      layer->setVisible(false);
      m_mapControl->addLayer(layer);
      m_osmMapLayer.reset(layer);
      minzoom = std::min(minzoom, mapadapter->minZoom());
      maxzoom = std::max(maxzoom, mapadapter->maxZoom());
    }

    {
      qmapcontrol::MapQuestMapAdapter *mapadapter = new qmapcontrol::MapQuestMapAdapter;
      qmapcontrol::MapLayer* layer = new qmapcontrol::MapLayer("Mapquest-Layer", mapadapter, false);
      layer->setVisible(false);
      m_mapControl->addLayer(layer);
      m_mapquestMapLayer.reset(layer);
      minzoom = std::min(minzoom, mapadapter->minZoom());
      maxzoom = std::max(maxzoom, mapadapter->maxZoom());
    }

    {
      qmapcontrol::GoogleMapAdapter *mapadapter = new qmapcontrol::GoogleMapAdapter;
      qmapcontrol::MapLayer* layer = new qmapcontrol::MapLayer("GoogleMap-Layer", mapadapter, true);
      layer->setVisible(false);
      m_mapControl->addLayer(layer);
      m_googleMapLayer.reset(layer);
      minzoom = std::min(minzoom, mapadapter->minZoom());
      maxzoom = std::max(maxzoom, mapadapter->maxZoom());
    }

    {
      qmapcontrol::GoogleTerrainMapAdapter *mapadapter = new qmapcontrol::GoogleTerrainMapAdapter;
      qmapcontrol::MapLayer* layer = new qmapcontrol::MapLayer("GoogleTerrainMap-Layer", mapadapter, true);
      layer->setVisible(false);
      m_mapControl->addLayer(layer);
      m_googleTerrainMapLayer.reset(layer);
      minzoom = std::min(minzoom, mapadapter->minZoom());
      maxzoom = std::max(maxzoom, mapadapter->maxZoom());
    }

    {
      qmapcontrol::GoogleSatMapAdapter *mapadapter = new qmapcontrol::GoogleSatMapAdapter;
      qmapcontrol::MapLayer* layer = new qmapcontrol::MapLayer("GoogleSatMap-Layer", mapadapter, false);
      layer->setVisible(false);
      m_mapControl->addLayer(layer);
      m_googleSatMapLayer.reset(layer);
      minzoom = std::min(minzoom, mapadapter->minZoom());
      maxzoom = std::max(maxzoom, mapadapter->maxZoom());
    }

    {
      qmapcontrol::GoogleSatHybridMapAdapter *mapadapter = new qmapcontrol::GoogleSatHybridMapAdapter();
      qmapcontrol::MapLayer* layer = new qmapcontrol::MapLayer("GoogleSatHybridMap-Layer", mapadapter, false);
      layer->setVisible(false);
      m_mapControl->addLayer(layer);
      m_googleSatHybridMapLayer.reset(layer);
      minzoom = std::min(minzoom, mapadapter->minZoom());
      maxzoom = std::max(maxzoom, mapadapter->maxZoom());
    }

    m_mapControl->setZoom(4);
    m_mapcontrolMinZoom = minzoom;
    m_mapcontrolMaxZoom = maxzoom;

    QGraphicsWidget* proxy = addWidget(m_mapControl);

    proxy->setPos(0, 0);
    proxy->setCacheMode(QGraphicsItem::NoCache);
    proxy->setZValue(-1800);  //background object, zvalue is for ordered rendering, larger ==> front
    m_mapProxy = proxy;
  }

  const int zoomval = 1160;

  { //create Marble widget
    Marble::MarbleWidget *w = m_marbleWidget = new Marble::MarbleWidget(); //x
    w->setObjectName("MarbleWidget");
    w->setProjection(1);   //0: spherical; 1: planar, 2:mercator
    w->setStyleSheet("background-color:white;");
    //w->setMapThemeId("earth/srtm2/srtm2.dgml");  //work
    w->setMapThemeId("earth/plain/plain.dgml");    //work
    w->setInputHandler(0);
    m_marbleMaxZoom = w->maximumZoom();
    if (m_marbleMaxZoom<2500) m_marbleMaxZoom=2500;
    w->zoomView(zoomval);

    w->setShowScaleBar(true);
    w->setShowAtmosphere(false);
    w->setShowGrid(true);
    w->setShowOverviewMap(true);
    w->setShowCompass(true);
    w->setShowPlaces(true);
    w->setShowCities(true);
    w->setShowBorders(true);
    w->setShowRelief(true);
    w->setShowRivers( true );

    w->setShowBackground(false);
    w->setAutoFillBackground(false);
    w->setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsWidget* proxy = addWidget(w);

    proxy->setPos(0, 0);
    proxy->setZValue(-1700);  //background object, zvalue is for ordered rendering, larger ==> front
    m_marbleProxy = proxy;
  }

  { //create Marble Navigator
    const float zval = 10000;
    Marble::MarbleNavigator *w = m_marbleNavigator = new Marble::MarbleNavigator();
    w->changeZoom(zoomval);
    w->setAttribute(Qt::WA_TranslucentBackground);
    w->setMinimumHeight(380);
    w->setAutoFillBackground(false);
    QGraphicsProxyWidget *p = addWidget(w);
    p->setZValue(zval);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout ;
    layout->addItem(p);
    QGraphicsWidget *proxy = new QGraphicsWidget();   //only QGraphicsWidget is movable
    m_navigatorProxy = proxy;
    proxy->setFlag(QGraphicsItem::ItemIsMovable, true);
    proxy->setLayout(layout);
    proxy->setZValue(zval);
    proxy->setPos(0, 400);
    this->addItem(proxy);
  }

  //connections between marble widget and the navigator
  connect(m_marbleNavigator, SIGNAL(moveLeft()), this, SLOT(moveLeft()));
  connect(m_marbleNavigator, SIGNAL(moveRight()), this, SLOT(moveRight()));
  connect(m_marbleNavigator, SIGNAL(moveUp()), this, SLOT(moveUp()));
  connect(m_marbleNavigator, SIGNAL(moveDown()), this, SLOT(moveDown()));
  connect(m_marbleNavigator, SIGNAL(zoomIn()), this, SLOT(zoomIn()));
  connect(m_marbleNavigator, SIGNAL(zoomOut()), this, SLOT(zoomOut()));
  connect(m_marbleNavigator, SIGNAL(zoomChanged(int)), this, SLOT(zoomChanged(int)));
  connect(m_marbleNavigator, SIGNAL(goHome()), this, SLOT(goHome()));
}



QDialog* createDialog(const QString &windowTitle) {
  QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
  dialog->setWindowTitle(windowTitle);
  dialog->setLayout(new QVBoxLayout);
  return dialog;
}



void RSIGScene::createOtherWidgets() {
  QGraphicsGridLayout *layout = new QGraphicsGridLayout;
  QGraphicsWidget *form = new QGraphicsWidget;
  form->setLayout(layout);
  this->addItem(form);
  QWidget *controls = createDialog("Control");
  QCheckBox *wireframe = new QCheckBox(tr("Render as wireframe"));
  controls->layout()->addWidget(wireframe);
  QPushButton *backgroundButton = new QPushButton(tr("Choose background color"));
  connect(backgroundButton, SIGNAL(clicked()), this, SLOT(setBackgroundColor()));
  controls->layout()->addWidget(backgroundButton);
  QWidget *statistics = createDialog(tr("Model info"));
  statistics->layout()->setMargin(20);

  for (int i = 0; i < 4; ++i) {
    QLabel *p = new QLabel;
    statistics->layout()->addWidget(p);
  }
        
  QWidget *instructions = createDialog(tr("Instructions"));
  instructions->layout()->addWidget(new QLabel(tr("Use mouse wheel to zoom model, and click and drag to rotate model")));
  instructions->layout()->addWidget(new QLabel(tr("Move the sun around to change the light position")));
  instructions->setStyleSheet("background-color: rgba(200, 200, 200, 10%);");
  QGraphicsWidget *p0 = addWidget(instructions);
  QGraphicsWidget *p1 = addWidget(controls);
  QGraphicsWidget *p2 = addWidget(statistics);
  layout->addItem(p0, 0, 0);
  layout->addItem(p1, 0, 1);
  layout->addItem(p2, 0, 2);
  QPointF pos(10, 10);

  foreach (QGraphicsItem *item, items()) {
    item->setFlag(QGraphicsItem::ItemIsMovable);
    //Any caching should not be used, in order to be compatible with OpenGL
    item->setCacheMode(QGraphicsItem::NoCache);
    const QRectF rect = item->boundingRect();
    item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
    pos += QPointF(0, 20 + rect.height());
  }
}



/*!
 \fn void RSIGScene::drawBackground (QPainter *painter, const QRectF &rectf)

 Draw the background of the scene.
 */

void RSIGScene::drawBackground (QPainter*, const QRectF&) {

  if (m_marbleNavigator) {
    const RSIGState *gState = RSIGStateSingleton::getInstance();                
    bool visible = m_marbleNavigator->isVisible();

    if (visible != gState->mShowNavigator) {
      m_marbleNavigator->setVisible(gState->mShowNavigator);
    }
  }

  glClearColor(1, 1, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);        
  glClearDepth(0);
  glClear(GL_DEPTH_BUFFER_BIT);
}        




// Draw the foreground of the scene. Empty.

void RSIGScene::drawForeground (QPainter*, const QRectF&) {
}




void RSIGScene::drawCurvedLine(QPainter *painter, const QPointF& l0, const QPointF& l1) {
  QPointF pos0 = this->toScreenPointF(l0);
  QPointF pos1 = this->toScreenPointF(l1);
  const float len = std::max(fabs(pos0.x()-pos1.x()), fabs(pos0.y()-pos1.y())) + 1e-10f;
  int n = int(len*0.20f + 0.5f);

  if (n<1) {
    n=1;
  }

  const float dx = l1.x() - l0.x();
  const float dy = l1.y() - l0.y();

  for (int i=1; i<=n; i++) {
    float t = float(i)/float(n);
    const float x = l0.x() + t*dx;
    const float y = l0.y() + t*dy;
    pos1 = this->toScreenPointF(QPointF(x, y));
    painter->drawLine(pos0, pos1);
    pos0 = pos1;
  }
}



void RSIGScene::drawLongLatLines(QPainter *painter) {
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  //bool mode2d = vOptions->getMode2D();
  int displayMode = vOptions->getDisplayMode();

  //if (mode2d) {
  if (displayMode == 0) { // 2D
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);    //Qt::DashLine causes stall in Win32 env.
    pen.setColor(QColor(255, 130, 130, 100));
    painter->setPen(pen);

    const QRectF georect = this->screenGeoRectF();
    const QPointF geoupleft(georect.x(), georect.y()+georect.height());
    const QPointF geolowright(georect.x()+georect.width(), georect.y());

    const double geowidth = fabs(geolowright.x() - geoupleft.x());
    const double geoheight = fabs(geoupleft.y() - geolowright.y());

    int xylabelpos[60][3];
    int labelcount=0;

    //draw longitude lines
    const int xsteps[]={1, 2, 5, 10, 15, 20, 30, 40};
    const unsigned int xstepsize = sizeof(xsteps)/sizeof(xsteps[0]);

    for (unsigned int i=0; i<xstepsize; i++) {
      const double nstep = geowidth/xsteps[i];

      if (nstep > 1.0 && nstep <= 6.0) {
        int ubase = (int(geoupleft.x())/xsteps[i])*xsteps[i];

        for (int j=0; j<=nstep+1; j++) {
          const int u = j*xsteps[i] + ubase;

          if (u>=geoupleft.x() && u<=geolowright.x()) {
            const QPointF geopos0(u, geoupleft.y());
            const QPointF geopos1(u, geolowright.y());
            QPointF pos0 = this->toScreenPointF(geopos0);
            QPointF pos1 = this->toScreenPointF(geopos1);

            if (isSphericalProj() || isLambertConformalConicProj() || isStereographicProj()) {
              this->drawCurvedLine(painter, geopos0, geopos1);
            } else {
              painter->drawLine(pos0, pos1);
            }

            int *label = xylabelpos[labelcount++];
            label[0]=pos0.x();
            label[1]=pos0.y()+16;
            label[2]=(u+1980)%360-180;

            if (label[2]==-180) {
              label[2]=180;
            }
          }
        }

        break;
      }
    }

    //draw lat lines

    for (unsigned int i=0; i<xstepsize; i++) {
      const double nstep = geoheight/xsteps[i];

      if (nstep > 1.0 && nstep<= 6.0) {
        int vbase = (int(geolowright.y())/xsteps[i])*xsteps[i];

        for (int j=0; j<=nstep+1; j++) {
          const int v = j*xsteps[i] + vbase;

          if (v>=geolowright.y() && v<=geoupleft.y()) {
            const QPointF geopos0(geoupleft.x(), v);
            const QPointF geopos1(geolowright.x(), v);
            QPointF pos0 = this->toScreenPointF(geopos0);
            QPointF pos1 = this->toScreenPointF(geopos1);

            if (isSphericalProj() || isLambertConformalConicProj() || isStereographicProj()) {
              this->drawCurvedLine(painter, geopos0, geopos1);
            } else {
              painter->drawLine(pos0, pos1);
            }

            int *label = xylabelpos[labelcount++];
            label[0]=pos0.x()+4;
            label[1]=pos0.y();
            label[2]=(v+90)%180-90;
          }
        }

        break;
      }
    }

    //draw lon/lat text
    pen.setColor(QColor(0, 0, 0, 192));
    painter->setPen(pen);
    const QFont font("Times", 12, QFont::Normal);
    painter->setFont(font);

    for (int i=0; i<labelcount; i++) {
      const int *label = xylabelpos[i];
      QString text;

      if (label[2]==180 || label[2]==-180) {
        text="+180/-180";
      } else {
        text.setNum(label[2]);
      }

      painter->drawText(label[0], label[1], text);
    }
  }
}



void RSIGScene::drawRegionOfInterest(QPainter *painter) {
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  //bool mode2d = vOptions->getMode2D();
  int displayMode = vOptions->getDisplayMode();
  QPen pen;

  if (m_dragRoiMode) {
    pen.setColor(QColor(200, 0, 200, 200));
    pen.setWidth(1);
    pen.setStyle(Qt::DotLine);
  } else {
    pen.setColor(QColor(240, 0, 240, 255));
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
  }

  painter->setPen(pen);

  //if (mode2d) {
  if (displayMode == 0) { // 2D

    if (isSphericalProj() || isLambertConformalConicProj() || isStereographicProj()) {
      const float x = m_roiRect.x();
      const float y = m_roiRect.y();
      const float w = m_roiRect.width();
      const float h = m_roiRect.height();
      const QPointF m0(x, y);
      const QPointF m1(x+w, y);
      const QPointF m2(x+w, y+h);
      const QPointF m3(x, y+h);

      this->drawCurvedLine(painter, m0, m1);
      this->drawCurvedLine(painter, m1, m2);
      this->drawCurvedLine(painter, m2, m3);
      this->drawCurvedLine(painter, m3, m0);
    }
    else {
      const QRectF srect = this->toScreenRectF(m_roiRect);
      painter->drawRect(srect);

      if (m_dragRoiMode) {
        QBrush brush(QColor(200, 200, 200, 60) );
        painter->fillRect(srect, brush);
      } else {
        QBrush brush(QColor(250, 250, 250, 20) );
        painter->fillRect(srect, brush);
      }
    }
  } else if (displayMode == 1) { // 3d mode
    const float x = m_roiRect.x();
    const float y = m_roiRect.y();
    const float w = m_roiRect.width();
    const float h = m_roiRect.height();
    QRect * latlonRect = new QRect(x, y, w, h);
    painter->drawRect(*latlonRect);
    QBrush brush(QColor(250, 250, 250, 20) );
    painter->fillRect(*latlonRect, brush);
  }
}



void RSIGScene::drawCenteringRectangle(QPainter *painter) {

  if (!m_dragCRectMode) {
    return;
  }

  QPen pen;
  pen.setColor(QColor(20, 30, 30, 200));
  pen.setWidth(1);
  pen.setStyle(Qt::DotLine);
  painter->setPen(pen);
        
  const QRectF r0 = m_crectRect;

  if (isSphericalProj() || isLambertConformalConicProj() || isStereographicProj()) {
    const float x = r0.x();
    const float y = r0.y();
    const float w = r0.width();
    const float h = r0.height();
    const QPointF m0(x, y);
    const QPointF m1(x+w, y);
    const QPointF m2(x+w, y+h);
    const QPointF m3(x, y+h);

    this->drawCurvedLine(painter, m0, m1);
    this->drawCurvedLine(painter, m1, m2);
    this->drawCurvedLine(painter, m2, m3);
    this->drawCurvedLine(painter, m3, m0);
  } else {
    const QRectF srect = this->toScreenRectF(r0);
    painter->drawRect(srect);

    if (m_dragRoiMode) {
      QBrush brush(QColor(200, 200, 200, 60) );
      painter->fillRect(srect, brush);
    }
    else {
      QBrush brush(QColor(250, 250, 250, 20) );
      painter->fillRect(srect, brush);
    }
  }
}



QString getDataAnnotationString() {

  // variables that require annotation/attribution
  const char* varsToAnnotate[] = {
    "calipso",
    "gasp",
    "goes",
    "modis",
    "behr",
    "omi",
    "purpleair",
    "tempo",
    "tropomi",
    "viirs", // noaa
    "viirs", // nasa
    "metar",
    "acam",
    "hsrl",
    "mozaic",
    "tad",
    "neubrew",
    "hrrr"
  };

  // make sure these are in the same order as above
  // (Yes Todd, I know it would be better to use a 2D table. I'm lazy.)
  const QString annotations[] = {
    QString("  - CALIPSO data courtesy of NASA LaRC ASDC"),
    QString("  - GASP data courtesy of NOAA/NESDIS"),
    QString("  - GOES data courtesy of NOAA/NESDIS"),
    QString("  - MODIS data courtesy of NASA GSFC"),
    QString("  - OMIBEHR data courtesy of KNMI/NASA/UC Berkeley"),
    QString("  - OMI data courtesy of NASA"),
    QString("  - PurpleAir data courtesy of https://purpleair.com"),
    QString("  - TEMPO data courtesy of SAO and NASA LaRC ASDC"),
    QString("  - TROPOMI data courtesy of the ESA/EU Copernicus program"),
    QString("  - VIIRS-NOAA data courtesy of NOAA/NESDIS"),
    QString("  - VIIRS-NASA data courtesy of Univ. of Wisc. SSEC and NASA GSFC"),
    QString("  - METAR data courtesy of World Meteorological Organization"),
    QString("  - ACAM data courtesy of NASA LaRC and GSFC"),
    QString("  - HSRL data courtesy of NASA LaRC"),
    QString("  - MOZAIC data courtesy of Centre National de la Recherche Scientifique"),
    QString("  - TAD data courtesy of NASA LaRC"),
    QString("  - NEUBREW data courtesy of NOAA/ESRL/GMD"),
    QString("  - HRRR data courtesy of NOAA/NCEP via U. Utah")
  };
    
  const size_t size_varsToAnnotate = sizeof varsToAnnotate / sizeof *varsToAnnotate;
  const size_t size_annotations = sizeof annotations / sizeof *annotations;

  // check to be sure the lists are the same size
  if (size_varsToAnnotate != size_annotations) {
    qDebug() << "==== Problem in RSIGScene: size_varsToAnnotate != size_annotations ====";
  }
  
  QString finalAnnot = QString("EPA RSIG3D: www.epa.gov/rsig\n");
  
  GeoDataList *lst = GeoDataListSingleton::getInstance();

  QList<QString> currentAnnotations;
  
  // loop through data objects
  for (int ind=0; ind<lst->size(); ind++) {
    GeoDataObject * obj = lst->getObject(ind);
    QString thisDataName = obj->varName();
    //qDebug() << "in getDataAnnotationString() thisDataName = " << thisDataName;

    // see if data object is in the attribution list
    for (size_t attribInd=0; attribInd<size_annotations; attribInd++) {

      if (thisDataName.toLower().contains(varsToAnnotate[attribInd])) {
        QString thisAnnotation = annotations[attribInd];

        // add the attribution if we haven't already
        if (!currentAnnotations.contains(thisAnnotation)) {
        
          // HACK obj-varName() does not distinguish VIIRS-NOAA and VIIRS-NASA look in description to distinguish them.
          // add annotation for all non-viirs data

          if  (thisDataName.toLower().contains("viirs")) {
            QString thisDataDescription = obj->description();
            //qDebug() << thisDataDescription;

            if (thisDataDescription.contains("noaa.gov") && thisAnnotation.contains("VIIRS-NOAA")) {
              //qDebug() << "here1" << thisAnnotation;
              finalAnnot.append(thisAnnotation);
              finalAnnot.append("\n");
            } else if (thisDataDescription.contains("wisc.edu") && thisAnnotation.contains("VIIRS-NASA")) {
              //qDebug() << "here2" << thisAnnotation;
              finalAnnot.append(thisAnnotation);
              finalAnnot.append("\n");
            }
            
            // add annotation for all non-viirs data
          } else { 
            finalAnnot.append(thisAnnotation);
            finalAnnot.append("\n");
          }

          // update currentAnnotations list
          currentAnnotations.append(thisAnnotation);
          
        }
      }
    }
  }
  return finalAnnot;
}



void RSIGScene::drawGraphTitle(QPainter *painter) {
  GeoDataList *lst = GeoDataListSingleton::getInstance();
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  double scaleFactor = vOptions->getCalipsoScaleFactor();
  const int w = sceneRect().width();
  const int h = sceneRect().height();
  const QString strScaleFactor = QString("Vertical scale factor: ").append(QString::number(scaleFactor, 'f', 1));
  //const QString strDataAnnot = QString(""); // we will append strings as needed below

  QString strSwathPersist = QString(""); // default
  bool persistMODIS = vOptions->getPersistMODIS();
  bool persistBEHR = vOptions->getPersistBEHR();
  bool persistVIIRS = vOptions->getPersistVIIRS();
  bool persistTROPOMI = vOptions->getPersistTROPOMI();
  bool persistTEMPO = vOptions->getPersistTEMPO();
  bool persistHMS = vOptions->getPersistHMS();
  int persistMODISHours = vOptions->getPersistMODISHours();
  int persistBEHRHours = vOptions->getPersistBEHRHours();
  int persistVIIRSHours = vOptions->getPersistVIIRSHours();
  int persistTROPOMIHours = vOptions->getPersistTROPOMIHours();
  int persistTEMPOHours = vOptions->getPersistTEMPOHours();
  int persistHMSHours = vOptions->getPersistHMSHours();

  if (persistMODIS || persistBEHR || persistVIIRS || persistTROPOMI || persistTEMPO || persistHMS) {
    strSwathPersist.append(QString("Swath persistence:"));
  }

  if (persistMODIS) {
    strSwathPersist.append(QString(" MODIS(").append(QString::number(persistMODISHours)).append(QString(" hr)")) );
  }

  if (persistBEHR) {
    strSwathPersist.append(QString(" BEHR(").append(QString::number(persistBEHRHours)).append(QString(" hr)")) );
  }

  if (persistVIIRS) {
    strSwathPersist.append(QString(" VIIRS(").append(QString::number(persistVIIRSHours)).append(QString(" hr)")) );
  }

  if (persistTROPOMI) {
    strSwathPersist.append(QString(" TROPOMI(").append(QString::number(persistTROPOMIHours)).append(QString(" hr)")) );
  }

  if (persistTEMPO) {
    strSwathPersist.append(QString(" TEMPO(").append(QString::number(persistTEMPOHours)).append(QString(" hr)")) );
  }

  if (persistHMS) {
    strSwathPersist.append(QString(" HMS(").append(QString::number(persistHMSHours)).append(QString(" hr)")) );
  }

  const QPen blackpen(QColor(0, 0, 0, 255));
  painter->setPen(blackpen);

  // Move text to RHS of window
  int h_offset;

  if (vOptions->getDisplayMode() == 2) {
    // panel display, no colorbar legends
    h_offset = 30;
  } else {
    // 2d or 3d display, with colobar legends
    if (lst->size() <= 3) {
      h_offset = 125;
    } else {
      h_offset = 220;
    }
  }
  
  QRect box1(w-210, h-h_offset, 200, 30);     // for timestep
  QRect box1a(0, h-h_offset-30, w-20, 30);    // for swath persist
  QRect box2(w-410, h-h_offset, 200, 30);     // for scale factor
  QRect box3(0, h-h_offset, w, 30);           // for background
  QRect box3a(0, h-h_offset-30, w, 30);       // for swath persist background
  QRect box4(0, 5, w, 30);                    // text rect for top data use annotations
  QRect box4fill(0, 0, w, 30);                // fill rect for top data use annotations
  QRect box5(vOptions->getWindVectorLength()-20, h-h_offset, 80, 30);         // for wind vector glyph factor


  // get original font
  const QFont origFont = painter->font();

  // set up default font
  QFont font("Helvetica", 8, QFont::Normal);
  const int pointsize_small  = 12;
  const int pointsize_medium = 14;
  const int pointsize_large  = 16;

  // special fonts
  QFont timeFont = font;
  QFont scaleFont = font;
  QFont persistFont = font;
  QFont dataUseFont = font;
  timeFont.setPixelSize(pointsize_large);
  timeFont.setBold(true);
  scaleFont.setPixelSize(pointsize_large);
  scaleFont.setBold(false);
  persistFont.setPixelSize(pointsize_medium);
  persistFont.setBold(false);
  dataUseFont.setPixelSize(pointsize_small);
  dataUseFont.setBold(false);

  const int bgcolor_r = 200;
  const int bgcolor_g = 200;
  const int bgcolor_b = 160;
  const int bgalpha   = 255;
  const QBrush brush1(QColor(bgcolor_r, bgcolor_g, bgcolor_b, bgalpha));

  const int bg2color_r = 200;
  const int bg2color_g = 200;
  const int bg2color_b = 200;
  const int bg2alpha   = 200;
  const QBrush brush2(QColor(bg2color_r, bg2color_g, bg2color_b, bg2alpha));

  painter->fillRect(box3, brush1); // Draw background beige for scale/date

  // Draw vertical scale annotation when 3D view mode:

  //if ( ! vOptions->getMode2D() ) {
  if ( vOptions->getDisplayMode() == 1 ) { // 3D
    painter->setFont(scaleFont);
    painter->drawText(box2, Qt::AlignCenter, strScaleFactor);
  }

  // Draw timestamp annotation as "YYYY-MM-DD HH:MM UTC".

  RSIGState::UTCString currentUTCString = "";
  RSIGStateSingleton::getInstance()->getCurrentUTCString( currentUTCString );
  const QString timestamp( currentUTCString );
  painter->setFont( timeFont );
  painter->drawText( box1, Qt::AlignRight, timestamp );

  // draw swath persist annotation
  painter->setFont(persistFont);

  bottomAnnotHeight = h_offset + box1.height(); // used in drawPanelTimeseries()
  if (persistMODIS || persistBEHR || persistVIIRS || persistTROPOMI || persistTEMPO || persistHMS) {
    painter->fillRect(box3a, brush1);
    painter->drawText(box1a, (Qt::AlignRight | Qt::AlignVCenter), strSwathPersist);
    bottomAnnotHeight += box1a.height();
  }

  // draw data use annotation
  #ifdef __APPLE__
  #define annotHeight 16
  #else
  #define annotHeight 20
  #endif

  QString strDataAnnot = getDataAnnotationString();

  if (strDataAnnot.length() > 0) {
    int numLines = strDataAnnot.count("\n");

    if (numLines <= 0) {
      numLines = 1;
    }

    box4.setHeight(numLines*annotHeight);
    box4fill.setHeight(numLines*annotHeight);
    painter->setFont(dataUseFont);
    painter->fillRect(box4fill, brush2);
    painter->drawText(box4, Qt::AlignLeft, strDataAnnot);

    topAnnotHeight = box4.height();
  }

  // draw probeMsg2 to the screen
  RSIGState *gState = RSIGStateSingleton::getInstance();
  const int list_size = lst->size();

  if (gState->mShowDataProbe) {
    //int nAnnots = probeMsg2.count("\n");
    //if (nAnnots <= 0) nAnnots = 1;;
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);    //Qt::DashLine causes stall in Win32 env.
    pen.setColor(QColor(0, 0, 0, 255));
    painter->setPen(pen);
    const QFont font("Courier", 8, QFont::Normal);
    painter->setFont(font);  
    QRect brushBox(0, box4.height(), w, (list_size+1) * annotHeight);                    // text rect for top data use annotations
    QRect probeBox(15, box4.height(), w, (list_size+1) * annotHeight);                    // text rect for top data use annotations
    QBrush brush(QColor(bg2color_r, bg2color_g, bg2color_b, bg2alpha) );
    painter->fillRect(brushBox, brush);
    painter->drawText(probeBox, Qt::AlignLeft, probeMsg2);
  }
  
  // Do we have vector data? If so draw a vector glyph
  VisDataList * vislist = VisDataListSingleton::getInstance();

  for (int ind=0; ind<list_size; ind++) {
    GeoDataObject * obj = lst->getObject(ind);
    const QString thisBaseName = obj->objectName();
    VisDataObject * visObjVector  = vislist->getObjectByName(thisBaseName + ".vector");  // or could be vector data

    if (visObjVector != NULL) {
      // Draw wind vector glyph in 2D mode

      //if (vOptions->getMode2D()) {
      if (vOptions->getDisplayMode() == 0) { // 2D
        QPen vectorpen(QColor(0, 0, 0, 255));
        vectorpen.setWidth(2);
        painter->setPen(vectorpen);
        QPoint arrowTail = QPoint(20, h-h_offset+15);
        QPoint arrowHead = QPoint(20+vOptions->getWindVectorLength(), h-h_offset+15);
        QPoint arrowTip1 = QPoint(arrowHead.x()-3, arrowHead.y()-2);
        QPoint arrowTip2 = QPoint(arrowHead.x()-3, arrowHead.y()+2);
        painter->drawLine(arrowTail, arrowHead);
        painter->drawLine(arrowHead, arrowTip1);
        painter->drawLine(arrowHead, arrowTip2);
        painter->drawText(box5, (Qt::AlignRight | Qt::AlignVCenter), vOptions->getWindVectorLabel());
        painter->setPen(blackpen);
      }
    }
  }

  // determine view dimensions in km
  if (gState->mShowScale && vOptions->getDisplayMode() == 0) { // 2D mode only
    // scalebar screen space
    const float scaleScreenPixelWidth = 60.0;
    const QPointF windowX0 = QPointF(w/2.0 - scaleScreenPixelWidth, h);
    const QPointF windowX1 = QPointF(w/2.0 + scaleScreenPixelWidth, h);
    
    // scalebar lon/lat space
    const QPointF geoX0 = this->toGeoPointF(windowX0);
    const QPointF geoX1 = this->toGeoPointF(windowX1);
    
    // scalebar Haversine formula for distance
    const double lonX0  = M_PI/180.0 * geoX0.x();
    const double latX0  = M_PI/180.0 * geoX0.y();
    const double lonX1  = M_PI/180.0 * geoX1.x();
    const double latX1  = M_PI/180.0 * geoX1.y();
    const double radius = 6371.0; // earth radius in km
    
    const double dlat2 = latX1 - latX0;
    const double dlon2 = lonX1 - lonX0;
    const double a2 = sin(dlat2/2.0) * sin(dlat2/2.0) + cos(latX0) * cos(latX1) * sin(dlon2/2.0) * sin(dlon2/2.0);
    const double distanceX = 2.0 * radius * atan2(sqrt(a2), sqrt(1 - a2));
    //qDebug() << distanceX;
    
    // scalebar draw
    QPen vectorpen(QColor(0, 0, 0, 255));
    vectorpen.setWidth(2);
    painter->setPen(vectorpen);
    QPoint scaleTail = QPoint(w/2.0 - scaleScreenPixelWidth, h-h_offset+8);
    QPoint scaleHead = QPoint(w/2.0 + scaleScreenPixelWidth, h-h_offset+8);
    painter->drawLine(scaleTail, scaleHead);
    QPoint scaleTailTick = QPoint(w/2.0 - scaleScreenPixelWidth, h-h_offset);
    QPoint scaleHeadTick = QPoint(w/2.0 + scaleScreenPixelWidth, h-h_offset);
    painter->drawLine(scaleTail, scaleTailTick);
    painter->drawLine(scaleHead, scaleHeadTick);
    
    QString distanceXFormatted;
    if (distanceX > 100) {
      distanceXFormatted = QString::number(distanceX, 'f', 0) + " km";
    } else {
      distanceXFormatted = QString::number(distanceX, 'f', 2) + " km";
    }
    painter->drawText(w/2.0-scaleScreenPixelWidth, h-h_offset+12, scaleScreenPixelWidth*2, 20, (Qt::AlignHCenter), distanceXFormatted);
  }
  
  // reset font
  painter->setFont(origFont);
}



// See draw2DSceneEMVL()
// Moved this functionality to draw2DSceneEMVL, which is called before the OpenGL stuff.
// That way, lon/lat lines and bbox are drawn underneath the 2D curtain and popup plots.

void RSIGScene::draw2DScene(QPainter*) {
}



void RSIGScene::draw2DSceneEMVL (QPainter *painter) {
  //qDebug() << "in  RSIGScene::draw2DSceneEMVL";
  VisOptions *vOptions = VisOptionsSingleton::getInstance();

  if (vOptions) {
    //bool mode2d = vOptions->getMode2D();
    int displayMode= vOptions->getDisplayMode();

    //if (mode2d) {
    if (displayMode == 0) { // 2D
      const RSIGState *gState = RSIGStateSingleton::getInstance();

      if (gState) {

        if (gState->mShowLonlatLines) {
          drawLongLatLines(painter);
        }

        // draw MODIS True Color 
        emvl::Timestamp currentTimestamp( RSIGStateSingleton::getInstance()->getCurrentTimestamp() );
        const int yyyymmdd = currentTimestamp.yyyymmdd();
        vOptions->setCurrentDayNum( yyyymmdd );

        if (vOptions->getDrawModisTrueColor()) {
          reloadModisTrueColor( yyyymmdd );

          QRectF bbox = QRectF(QPointF(vOptions->getBboxMinLon(), vOptions->getBboxMaxLat()), QPointF(vOptions->getBboxMaxLon(), vOptions->getBboxMinLat()));
          modisTrueColorRect = this->toScreenRectF(bbox);
          //painter->drawImage(modisTrueColorRect, modisTrueColorImage);
          //painter->drawPixmap(modisTrueColorRect.x(), modisTrueColorRect.y(), modisTrueColorRect.width(), modisTrueColorRect.height(), QPixmap::fromImage(modisTrueColorImage));

          if (modisTrueColorPixmap) {
            painter->setOpacity(vOptions->getOpacityModisTrueColor()/100.0);
            //painter->drawPixmap(modisTrueColorRect.x(), modisTrueColorRect.y(), modisTrueColorRect.width(), modisTrueColorRect.height(), *modisTrueColorPixmap); // this causes the 2D calipso curtain to go all gray. Bug!
            painter->drawImage(modisTrueColorRect, modisTrueColorPixmap->toImage());
            //painter->setOpacity(1);
          }
        }

        drawRegionOfInterest(painter);
        drawCenteringRectangle(painter);
      } 
    }
  }

  //qDebug() << "out  RSIGScene::draw2DSceneEMVL";

}



void RSIGScene::draw2DSceneLegend (QPainter *painter) {
  VisOptions* vOptions = VisOptionsSingleton::getInstance();

  if (vOptions->getDisplayMode() != 2) { // don't draw legends if panel display mode
    RSIGState *gState = RSIGStateSingleton::getInstance();

    if (gState->mShowLUTs) {
      const QRect screenrect(0, 0, sceneRect().width(), sceneRect().height());
      VisDataListSingleton::getInstance()->renderLookupTables(painter, screenrect);
    }
  }

  drawGraphTitle(painter);
}



void RSIGScene::resetView2D() {
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  int resetFlag = vOptions->getViewResetFlag();

  if (resetFlag == 1) {
    // reset
    this->goHome();
    // set reset flag to false
    vOptions->setViewResetFlag(0);
  } else if (resetFlag == 2) {
    // lock in Lambert projection view
    this->goLambert();
    vOptions->setViewResetFlag(0);
  } else if (resetFlag == 3) {
    // lock in stereographic projection view
    this->goStereographic();
    vOptions->setViewResetFlag(0);
  } else if (resetFlag == 4) {
    // autozoom to data objects
    this->goAutozoom();
    vOptions->setViewResetFlag(0);
  }
}



void RSIGScene::resetView3D (int flag) {
  // flag=1 -> reset
  // flag=4 -> center on region of interest  

  aspect = (float)sceneWidth / (float)sceneHeight;

  // reset
  if (flag == 1) {
    fov_angle      = fov_angle_init;
    eyeX           = eyeX_init;
    eyeY           = eyeY_init;
    eyeZ           = eyeZ_init;
    centerX        = centerX_init;
    centerY        = centerY_init;
    centerZ        = centerZ_init;
    upX            = upX_init;
    upY            = upY_init;
    upZ            = upZ_init;
    rotAngleX      = rotAngleX_init;
    rotAngleY      = rotAngleY_init;
    rotAngleZ      = rotAngleZ_init;
    rotCenterLon   = rotCenterLon_init;
    rotCenterLat   = rotCenterLat_init;
    rotCenterZ     = rotCenterZ_init;
     
    // autozoom
  } else if (flag == 4) {
    //qDebug() << (double)m_roiRect.width() << (double)m_roiRect.height() << fmax((double)m_roiRect.width(), (double)m_roiRect.height());
    fov_angle      = 0.055 * fmax(fabs(m_roiRect.width()), fabs(m_roiRect.height()));
    eyeX           = m_roiRect.center().x();
    eyeY           = m_roiRect.center().y();
    eyeZ           = eyeZ_init;
    centerX        = m_roiRect.center().x();
    centerY        = m_roiRect.center().y();
    centerZ        = centerZ_init;
    upX            = upX_init;
    upY            = upY_init;
    upZ            = upZ_init;
    rotAngleX      = rotAngleX_init;
    rotAngleY      = rotAngleY_init;
    rotAngleZ      = rotAngleZ_init;
    rotCenterLon   = rotCenterLon_init;
    rotCenterLat   = rotCenterLat_init;
    rotCenterZ     = rotCenterZ_init;
  }
   
  // reset stored modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -200.0);
  glGetDoublev(GL_MODELVIEW_MATRIX, stored_modelview);

  // set reset flag to false
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  vOptions->setViewResetFlag(0);
}



void NormalVector(GLdouble p1[3], GLdouble p2[3], GLdouble p3[3], GLdouble n[3]) {
  
  GLdouble v1[3], v2[3], d;
  // calculate two vectors, using the middle point
  // as the common origin
  v1[0] = p3[0] - p1[0];
  v1[1] = p3[1] - p1[1];
  v1[2] = p3[2] - p1[2];
  v2[0] = p3[0] - p2[0];
  v2[1] = p3[1] - p2[1];
  v2[2] = p3[2] - p2[2];
  
  // calculate the cross product of the two vectors
  n[0] = v1[1] * v2[2] - v2[1] * v1[2];
  n[1] = v1[2] * v2[0] - v2[2] * v1[0];
  n[2] = v1[0] * v2[1] - v2[0] * v1[1];
  
  // normalize the vector
  d = ( n[0] * n[0] + n[1] * n[1] + n[2] * n[2] );
  // try to catch very small vectors

  if (d < (GLdouble)0.00000001) {
    d = (GLdouble)100000000.0;
  } else {
    d = (GLdouble)1.0 / sqrt(d);
  }
  
  n[0] *= d;
  n[1] *= d;
  n[2] *= d;
}



float lonlat_to_cartX(float radius, float lon, float lat) {
  float deg2rad = M_PI/180.0; 
  float x = radius * cos(lon*deg2rad) * cos(lat*deg2rad);
  return x;
}

float lonlat_to_cartY(float radius, float lon, float lat) {
  float deg2rad = M_PI/180.0; 
  float y = radius * sin(lon*deg2rad) * cos(lat*deg2rad);
  return y;
}

float lonlat_to_cartZ(float radius, float lat) {
  float deg2rad = M_PI/180.0; 
  float z = radius * sin(lat*deg2rad);
  return z;
}



float * RSIGScene::getProbeLocation() {
  VisOptions* const vOptions = VisOptionsSingleton::getInstance();
  //const bool mode2d = vOptions->getMode2D();
  const int displayMode = vOptions->getDisplayMode();
  float probeLat = 0.0, probeLon = 0.0;

  //if ( mode2d ) {
  if ( displayMode == 0 ) { // 2D
    //find location        
    const QPointF currentScreenPos = curScenePos;
    const QPointF currentGeoPos = this->toGeoPointF(currentScreenPos);
    probeLon = currentGeoPos.x();
    probeLat = currentGeoPos.y();
  } else if (displayMode == 1) { // 3D
    GLdouble rLatNear, rLatFar;
    GLdouble rLonNear, rLonFar;
    GLdouble rZNear, rZFar;
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);   
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    
    // Unproject at normalized near and far points to find possible ray:

    const float xLoc = (float) curScenePos.x();
    const float yLoc = sceneHeight - (float) curScenePos.y();
    const int result1 = gluUnProject(xLoc, yLoc, 0.0, modelview, projection, viewport, &rLonNear, &rLatNear, &rZNear); 
    const int result2 = gluUnProject(xLoc, yLoc, 1.0, modelview, projection, viewport, &rLonFar,  &rLatFar,  &rZFar); 

    // Determine where ray intersects map plane:

    if ( result1 && result2 ) {
      const float dist_percent = (rZNear - 0) / fabs(rZFar - rZNear);
      probeLat = rLatNear + dist_percent*(rLatFar - rLatNear);
      probeLon = rLonNear + dist_percent*(rLonFar - rLonNear);
    }
  }

  static float location[2];
  location[0] = probeLat;
  location[1] = probeLon;
  return location;
}



// Return non-LUT VisDataObject corresponding to a dataset:

VisDataObject*
RSIGScene::getVisDataObjectForDataset(const emvl::Dataset* const dataset) const {
  VisDataObject* result = 0;

  if ( dataset ) {
    const VisDataList* const visDataList = VisDataListSingleton::getInstance();
    const GeoDataList* const geoDataList = GeoDataListSingleton::getInstance();
    assert( visDataList ); assert( geoDataList );
    const int geoDataListCount = geoDataList->size();

    for ( int geoDataListIndex = 0; geoDataListIndex < geoDataListCount;
          ++geoDataListIndex ) {
      const GeoDataObject* const geoDataObject =
        geoDataList->getObject( geoDataListIndex );
      assert( geoDataObject );

      if ( geoDataObject->model() == dataset ) {

        // Construct a name matching the associated VisDataObject:

        const QString geoDataObjectName( geoDataObject->objectName() );

        // Types of (non-LUT) visDataObjects:

        const char* const types[] = { ".point", ".polygon", ".vector" };
        const size_t typeCount = sizeof types / sizeof *types;

        for ( size_t typeIndex = 0; typeIndex < typeCount; ++typeIndex ) {
          VisDataObject* const visDataObject =
            visDataList->getObjectByName( geoDataObjectName + types[typeIndex]);

          if ( visDataObject ) {
            result = visDataObject;
            typeIndex = typeCount;
          }
        }
      }
    }
  }

  return result;
}



void  RSIGScene::updateProbe() {
  VisOptions* const vOptions = VisOptionsSingleton::getInstance();

  float* probeLoc = getProbeLocation();
  float probeLon = probeLoc[1];
  float probeLat = probeLoc[0];

  // probeMsg is for displaying in the status bar
  QString probeMsg  = formatLonLat(probeLon, probeLat);

  // probeMsg2 is for displaying in the heads-up display
  probeMsg2.sprintf("Lon: %f\nLat: %f\n", probeLon, probeLat);
  
  // Probe on the datasets only if the probe point is within the BBOX:
  // Why? Because it is slow on Windows platform.

  const double minlon = vOptions->getBboxMinLon();
  const double maxlon = vOptions->getBboxMaxLon();
  const double minlat = vOptions->getBboxMinLat();
  const double maxlat = vOptions->getBboxMaxLat();
  
  const bool probeFlag =
    probeLon >= minlon &&
    probeLon <= maxlon &&
    probeLat >= minlat &&
    probeLat <= maxlat;

  //fprintf( stderr, "\n=== probe = (%f, %f) bbox = [%f, %f][%f, %f], probeFlag = %d\n",
  //         probeLon, probeLat, minlon, maxlon, minlat, maxlat, probeFlag );

  if ( probeFlag ) {
    GeoDataList* const lst = GeoDataListSingleton::getInstance();
    const int list_size = lst->size();
    VisDataList* const vislist = VisDataListSingleton::getInstance();

    // Loop through data objects and probe each one:

    for ( int ind = 0; ind < list_size; ++ind ) {
      GeoDataObject* const obj = lst->getObject(ind);

      // Construct a name matching the associated VisDataObject:

      const QString thisBaseName = obj->objectName();

      // Continue only if the vis object is visible:

      bool isVisObjVisible = false; // default

      VisDataObject* const visObjPoint   = vislist->getObjectByName(thisBaseName + ".point");   // could be point data
      VisDataObject* const visObjPolygon = vislist->getObjectByName(thisBaseName + ".polygon"); // or could be polygon data
      VisDataObject* const visObjVector  = vislist->getObjectByName(thisBaseName + ".vector");  // or could be vector data
      int CMAQLayer = 0;

      if ( visObjPoint != NULL ) {
        isVisObjVisible = visObjPoint->showObject();
        CMAQLayer = visObjPoint->CMAQLayer();
      } else if ( visObjPolygon != NULL ) {
        isVisObjVisible = visObjPolygon->showObject();
        CMAQLayer = visObjPolygon->CMAQLayer();
      } else if ( visObjVector != NULL ) {
        isVisObjVisible = visObjVector->showObject();
        CMAQLayer = visObjVector->CMAQLayer();
      }

      if ( isVisObjVisible ) {

        // Get current timestamp:

        const emvl::Timestamp timestamp( RSIGStateSingleton::getInstance()->getCurrentTimestamp() );

        // Probe instance of emvl::Dataset object (typedef GeometryObjectType):

        GeometryObjectType* const data = obj->model();

        if ( data ) {
          const double probeLongitude = probeLon;
          const double probeLatitude = probeLat;
          const double probeElevation = emvl::MISSING; // How about 3D probe?
          const int layers = (int) data->layers();
          const int probeLayer =
            IN_RANGE( CMAQLayer, 1, layers ) ? CMAQLayer - 1 : emvl::IMISSING;
          const int probeVariable = emvl::IMISSING; // Default is dataset->dataVariable().
          const double probeValue =
            data->probe( timestamp, probeLongitude, probeLatitude,
                         probeElevation, probeLayer, probeVariable );

          // fprintf( stderr, "\n======== probeValue = %f\n", probeValue );

          if ( probeValue != emvl::MISSING ) {
            QString probeName = obj->varName();

            // Remove non-essential parts of name to make it shorter to display:

            if ( probeName.startsWith( "regridded_mean_" ) ) {
              probeName.remove( "regridded_mean_" );
            } else if ( probeName.startsWith( "regridded_daily_" ) ) {
              probeName.remove( "regridded_daily_" );
            } else if ( probeName.startsWith( "regridded_" ) ) {
              probeName.remove( "regridded_" );
            }

            //  If dataset has a note for probed point use it for probeName:

            const std::string& probedNote( data->probedNote() );

            if ( probedNote.length() ) {
              probeName = probedNote.c_str();
            }

            std::string name( probeName.toStdString() );
            QString probeString;
            probeString.sprintf(", %s:%f", name.c_str(), probeValue );
            probeMsg.append(probeString);

            QString probeString2;
            probeString2.sprintf("%s: %f\n", name.c_str(), probeValue);
            probeMsg2.append(probeString2);
          }
        }
      }
    }
  }

  // write probe message to status bar
  displayProbe(probeMsg);
}



void RSIGScene::drawTimeseries() {

  VisOptions* const vOptions = VisOptionsSingleton::getInstance();

  float * probeLoc = getProbeLocation();
  float probeLat = probeLoc[0];
  float probeLon = probeLoc[1];

  // Probe on the datasets only if the probe point is within the BBOX:
  // Why? Because it is slow on Windows platform.

  const double minlon = vOptions->getBboxMinLon();
  const double maxlon = vOptions->getBboxMaxLon();
  const double minlat = vOptions->getBboxMinLat();
  const double maxlat = vOptions->getBboxMaxLat();
  
  const bool probeFlag =
    probeLon >= minlon &&
    probeLon <= maxlon &&
    probeLat >= minlat &&
    probeLat <= maxlat;

  std::vector<double> timeseries;

  if ( probeFlag ) {
    GeoDataList* const lst = GeoDataListSingleton::getInstance();
    const int list_size = lst->size();
    VisDataList* const vislist = VisDataListSingleton::getInstance();
    
    // Loop through data objects and probe each one:
    for ( int ind = 0; ind < list_size; ++ind ) {
      GeoDataObject* const obj = lst->getObject(ind);

      // Construct a name matching the associated VisDataObject:

      const QString thisBaseName = obj->objectName();

      // Continue only if the vis object is visible:

      bool isVisObjVisible = false; // default

      VisDataObject* const visObjPoint   = vislist->getObjectByName(thisBaseName + ".point");   // could be point data
      VisDataObject* const visObjPolygon = vislist->getObjectByName(thisBaseName + ".polygon"); // or could be polygon data
      VisDataObject* const visObjVector  = vislist->getObjectByName(thisBaseName + ".vector");  // or could be vector data
      int CMAQLayer = 0;

      if ( visObjPoint != NULL ) {
        isVisObjVisible = visObjPoint->showObject();
        CMAQLayer = visObjPoint->CMAQLayer();
      } else if ( visObjPolygon != NULL ) {
        isVisObjVisible = visObjPolygon->showObject();
        CMAQLayer = visObjPolygon->CMAQLayer();
      } else if ( visObjVector != NULL ) {
        isVisObjVisible = visObjVector->showObject();
        CMAQLayer = visObjVector->CMAQLayer();
      }

      if ( isVisObjVisible ) {

        // Get timestamp range:

        const emvl::Timestamp timestamp( RSIGStateSingleton::getInstance()->getCurrentTimestamp() );

        // Probe instance of Todd's emvl::Dataset object (typedef as GeometryObjectType by Nan):

        GeometryObjectType* const data = obj->model();

        if ( data ) {
          // get the timeseries
          const double probeLongitude = probeLon;
          const double probeLatitude = probeLat;
          const double probeElevation = emvl::MISSING; // How about 3D probe?
          const int layers = (int) data->layers();
          const int probeLayer =
            IN_RANGE( CMAQLayer, 1, layers ) ? CMAQLayer - 1 : emvl::IMISSING;
          data->timeseries( data->startingTimestamp(), data->endingTimestamp(),
                            probeLongitude, probeLatitude, probeElevation,
                            probeLayer, timeseries );

        }
      }
    }
  }
}


void RSIGScene::drawPanelTimeseries(QPainter *painter) {  
  //    ---------    ---------
  //    | plot1 |    | plot2 |
  //    |       |    |       |
  //    ---------    ---------
  //    ----------------------
  //    | plot3              |
  //    ----------------------


  // set up plot areas
  int marginX      = 10;
  int marginY      = 10;
  int plot1_x0     = marginX;
  int plot1_y0     = topAnnotHeight + marginY;
  int plot1_width  = (sceneRect().width() - (3*marginX)) / 2 ;
  //int plot1_height = (sceneRect().height())              / 2 - marginY;
  int plot1_height = 0.75 * (sceneRect().height() - bottomAnnotHeight - topAnnotHeight) - (2*marginY);

  int plot2_x0     = plot1_x0 + plot1_width + marginX;
  int plot2_y0     = topAnnotHeight + marginY;
  int plot2_width  = plot1_width;
  int plot2_height = plot1_height;

  int plot3_x0     = plot1_x0;
  int plot3_y0     = plot1_y0 + plot1_height + marginY;
  int plot3_width  = sceneRect().width() - (2*marginX);
  int plot3_height = 0.38 * plot1_height;

  std::vector<double> probeTimeseries;
  std::vector<double> targetTimeseries;

  GeoDataList* const lst = GeoDataListSingleton::getInstance();
  const int list_size = lst->size();
  VisDataList* const vislist = VisDataListSingleton::getInstance();

  MainWin& mainWin = MainWin::getInstance();
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
  QStringList probeList = controlDockWidget->getDisplayPanelProbelist();

  QString probeSourceName  = controlDockWidget->getPanelDisplaySource1Selection(); // point dataset 
  QString targetSourceName = controlDockWidget->getPanelDisplaySource2Selection(); // polygon dataset
  

  // get probe and target data
  QString targetName       = "";
  QString probeBaseName    = "";
  QString targetBaseName   = "";
  GeoDataObject* probeObj = 0;
  GeoDataObject* targetObj = 0;

  for ( int ind = 0; ind < list_size; ++ind ) {
    GeoDataObject* const obj = lst->getObject(ind);
    const QString thisBaseName = obj->objectName();
    if (thisBaseName.startsWith(probeSourceName)) {
      probeObj = lst->getObject(ind);
      probeBaseName = obj->objectName();
    }
    if (thisBaseName.startsWith(targetSourceName)) {
      targetObj = lst->getObject(ind);
      targetBaseName = obj->objectName();
    }
  }


  const unsigned int nDays = RSIGStateSingleton::getInstance()->mDuration;
  // get beginning and ending timestamps
  // TBD. Need full range for selected time period independent of the datasets
  //const emvl::Timestamp globalStartTime( rsigState->getStartingTimestamp() );
  //const emvl::Timestamp globalEndTime( rsigState->getEndingTimestamp() );
  
  QString probeTitle;
  QString probeUnits;
  double probeMin = 0.0;
  double probeMax = 0.0;
  GeometryObjectType* probeData = 0;

  QString targetTitle;
  double targetMin = 0.0;
  double targetMax = 0.0;
  GeometryObjectType* targetData = 0;

  // get probe data
  bool isProbeProfile = false; // default
  if (probeSourceName != "" && probeBaseName.startsWith(probeSourceName)) {
    VisDataObject* visObj = vislist->getObjectByName(QString(probeBaseName).append(".point"));
    if (visObj) {
      probeTitle = probeBaseName;
      probeUnits = probeObj->units();
      probeData  = probeObj->model();
      probeMin   = visObj->minval();
      probeMax   = visObj->maxval();

      if (probeSourceName.contains("ceilometer") || probeSourceName.contains("pandora")) {
        isProbeProfile = true;
      }
      
    }
  }

  // get target data
  if (targetSourceName != "" && targetBaseName.startsWith(targetSourceName)) {
    VisDataObject* visObj = vislist->getObjectByName(QString(targetBaseName).append(".polygon"));
    if (visObj) {
      targetTitle = targetBaseName + " " + targetObj->units();
      targetData  = targetObj->model();
      targetMin   = visObj->minval();
      targetMax   = visObj->maxval();
    }
  }
  //qDebug() << "target" << targetMin << targetMax << targetBaseName << targetSourceName;


  // processing for plot1
  static const int maxPlots = 9;
  int* plot1Origin = 0;

  if (probeData && targetData && probeSourceName != "" && targetSourceName != "") {

    // Init plot2 here. Others are initialized below
    panelPlot2Init(painter, "Plotting area 2", plot2_x0, plot2_y0, plot2_width, plot2_height);

    int nPlots = probeList.length();

    if (nPlots > maxPlots) {
      nPlots = maxPlots;
      QString msg = "The maximum number of panel plots is limited to " + QString::number(maxPlots);
      controlDockWidget->postMessage(msg, RSIGState::WARNING_TEXT);
    } 

    // initialize a subplot
    plot1Origin = panelPlot1Init(painter, nPlots, maxPlots, probeTitle, targetTitle, plot1_x0, plot1_y0, plot1_width, plot1_height, probeMin, probeMax, targetMin, targetMax);
    //qDebug() << "origin" << plot1Origin[0] << plot1Origin[1];

    const std::vector<emvl::Dataset::SubsetPointers> targetSubsets = targetData->subset( probeData->startingTimestamp(), probeData->endingTimestamp() );
    // subsetPointers are only used for airnow, purpleair, etc
    std::vector<emvl::Dataset::SubsetPointers> probeSubsets;
    emvl::Dataset::SubsetPointers probeSubsetPointers;
    memset( &probeSubsetPointers, 0, sizeof probeSubsetPointers ); // init POD.
    // std::vector structure used for ceilometer and pandora
    std::vector<size_t>      profileDatasetIndices;
    std::vector<std::string> profileDatasetVariables;
    std::vector<std::string> profileDatasetUnits;
    std::vector<long long>   profileDatasetIds;
    std::vector<double>      profileDatasetLongitudes;
    std::vector<double>      profileDatasetLatitudes;
    std::vector<std::string> profileDatasetNames;

    int plotNum = 0;
    const size_t curTimestep =  RSIGStateSingleton::getInstance()->mCounter;

    size_t nProbeLocations;
    size_t nProbeSubsets;

    if (isProbeProfile) {
      // for ceilometer, pandora
      cca::getCeilometerInfo( profileDatasetIndices,
                              profileDatasetVariables,
                              profileDatasetUnits,
                              profileDatasetIds,
                              profileDatasetLongitudes,
                              profileDatasetLatitudes,
                              profileDatasetNames );
      nProbeSubsets = 1;
    } else {
      // for airnow, purpleair, etc 
      probeSubsets  = probeData->subset( probeData->startingTimestamp(), probeData->endingTimestamp() );
      nProbeSubsets = probeSubsets.size();
    }

    const std::vector<std::string>& notes = probeData->allDistinctNotes();
    const size_t noteCount = notes.size();
    size_t noteIndex = 0;
    std::set<std::string> memo; // Remember already-probed points.
    
    QString probeNote       = "";
    QString probeNoteUnique = "";
    
    for ( size_t probeSubset = 0; probeSubset < nProbeSubsets; probeSubset++ ) {

      if (isProbeProfile) {
        nProbeLocations = profileDatasetNames.size();
      } else {
        probeSubsetPointers = probeSubsets[ probeSubset ];
        assert( probeSubsetPointers.coordinates );
        nProbeLocations = probeSubsetPointers.coordinatesCount;
      }

      for (size_t probeIndCoord=0; probeIndCoord<nProbeLocations; probeIndCoord++) {
        
        if (isProbeProfile) {
          QString probeNoteFull = QString::fromStdString(profileDatasetNames[probeIndCoord]);
          QStringList probeNoteParse = probeNoteFull.split("_");
          probeNote = probeNoteParse[0] + "_" + probeNoteParse[1];
          probeNoteUnique = QString::fromStdString(profileDatasetNames[probeIndCoord]);
        } else {

          if ( probeSubsetPointers.notesCount ) {

            if ( noteIndex < noteCount ) {
              const std::string& note = notes[ noteIndex ];
              ++noteIndex;
              QString name( QString::fromStdString( note ) );
              probeNote = name;
              probeNoteUnique = name;
            }
          }
        }
        
        if (! memo.count( probeNoteUnique.toStdString() ) && probeList.contains(probeNoteUnique) ) {
          memo.insert( probeNoteUnique.toStdString() ); // Remember to avoid re-probing later.
          
          int thisOriginX = plot1Origin[plotNum*3 + 0];
          int thisOriginY = plot1Origin[plotNum*3 + 1];
          int thisPlotDim = plot1Origin[plotNum*3 + 2];
              
          // get the probe timeseries
          const double probeLongitude =
            isProbeProfile ? profileDatasetLongitudes[ probeIndCoord ]
            : probeSubsetPointers.coordinates[ probeIndCoord ].longitude;
          const double probeLatitude =
            isProbeProfile ? profileDatasetLatitudes[ probeIndCoord ]
            : probeSubsetPointers.coordinates[ probeIndCoord ].latitude;
          const double probeElevation = emvl::MISSING;
          const int probeLayer = emvl::IMISSING;

          probeData->timeseries( probeData->startingTimestamp(), probeData->endingTimestamp(),
                                 probeLongitude, probeLatitude, probeElevation,
                                 probeLayer, probeTimeseries );

          // get target timeseries and plot data on scatter plot
          targetData->timeseries( probeData->startingTimestamp(), probeData->endingTimestamp(),
                                  probeLongitude, probeLatitude, probeElevation,
                                  probeLayer, targetTimeseries );
          
          for (size_t timeInd=0; timeInd<nDays*24; timeInd++) {
            if (targetTimeseries[timeInd] != emvl::MISSING && probeTimeseries[timeInd]  != emvl::MISSING &&
                probeTimeseries[timeInd]  >= probeMin      && probeTimeseries[timeInd]  <= probeMax      &&
                targetTimeseries[timeInd] >= targetMin     && targetTimeseries[timeInd] <= targetMax ) {
              
              bool highlightFlag = false; // default
              if (timeInd == curTimestep) {
                highlightFlag = true;
              }
              
              QPointF thisDataPoint  = QPointF(probeTimeseries[timeInd], targetTimeseries[timeInd]);
              QPoint thisScreenPoint = panelPlot1DatatoScreen(thisDataPoint, thisOriginX, thisOriginY, thisPlotDim, thisPlotDim, probeMin, probeMax, targetMin, targetMax);
              
              //qDebug() << probeLongitude << probeLatitude << thisDataPoint.x() << thisDataPoint.y() << thisScreenPoint.x() << thisScreenPoint.y();
              
              if (thisScreenPoint.x() > 0 && thisScreenPoint.y() > 0) {
                //qDebug() << plotNum << thisScreenPoint << highlightFlag;
                panelPlot1AddPoint(painter, plotNum, thisScreenPoint, highlightFlag);
              }
            }
          }

          // subplot title
          painter->drawText(thisOriginX, thisOriginY-20, thisPlotDim, 20, Qt::AlignHCenter, probeNote);

          plotNum += 1;
        }
      }  
    } // end probe subset loop
  } // end plot1

  // processing for plot3    
  if ( probeData && probeSourceName != "") {

    // initialize plot areas

    // conversion from data space (probe) to screen space:
    double* plot3ProbeConversion = panelPlot3Init(painter, probeTitle, probeUnits, plot3_x0, plot3_y0, plot3_width, plot3_height, 0, nDays*24, probeMin, probeMax);

    // conversion from data space (target) to screen space:
    //double* plot3TargetConversion = panelPlot3Init(painter, targetTitle, targetUnits, plot3_x0, plot3_y0, plot3_width, plot3_height, 0, nDays*24, targetMin, targetMax);

    if (isProbeProfile) {
      // processing for ceilometer/pandora
      std::vector<size_t>      profileDatasetIndices;
      std::vector<std::string> profileDatasetVariables;
      std::vector<std::string> profileDatasetUnits;
      std::vector<long long>   profileDatasetIds;
      std::vector<double>      profileDatasetLongitudes;
      std::vector<double>      profileDatasetLatitudes;
      std::vector<std::string> profileDatasetNames;

      cca::getCeilometerInfo( profileDatasetIndices,
                              profileDatasetVariables,
                              profileDatasetUnits,
                              profileDatasetIds,
                              profileDatasetLongitudes,
                              profileDatasetLatitudes,
                              profileDatasetNames );

      int plotIndex = 1; // plotIndex is one-based

      for (size_t indProfile = 0; indProfile < profileDatasetNames.size(); ++indProfile ) {
        const QString thisProfileName(
          QString::fromStdString( profileDatasetNames[ indProfile ] ) );

        if ( probeList.contains( thisProfileName ) ) {

          // get the timeseries
          const double probeLongitude = profileDatasetLongitudes[indProfile];
          const double probeLatitude  = profileDatasetLatitudes[indProfile];
          const double probeElevation = emvl::MISSING; 
          const int    probeLayer     = emvl::IMISSING;
          probeData->timeseries( probeData->startingTimestamp(), probeData->endingTimestamp(),
                                 probeLongitude, probeLatitude, probeElevation,
                                 probeLayer, probeTimeseries );

          //targetData->timeseries( probeData->startingTimestamp(), probeData->endingTimestamp(),
          //                       probeLongitude, probeLatitude, probeElevation,
          //                       probeLayer, taretTimeseries );
          
          panelPlot3AddTimeseries(painter, thisProfileName, plotIndex, plot3ProbeConversion, probeTimeseries, 0, nDays*24, probeMin, probeMax);
          //panelPlot3AddTimeseries(painter, thisProfileName, plotIndex, plot3TargetConversion, targetTimeseries, 0, nDays*24, targetMin, targetMax);
          plotIndex += 1;
                  
        } 
      }
      
    } else if ( probeData->cellType() == emvl::Dataset::POINT &&
                ! probeData->isVector() ) {
      const int probeSiteCount = probeList.length();
      int plotIndex = 1; // plotIndex is one-based
      const double xMin = 0.0;
      const double xMax = nDays * 24.0;

      for ( int probeSite = 0; probeSite < probeSiteCount; ++probeSite ) {
        const QString probeSiteName( probeList[ probeSite ] );
        const std::string distinctSiteName( probeSiteName.toStdString() );
        const emvl::Point* const probePoint =
          probeData->distinctNotePoint( distinctSiteName );

        if ( probePoint ) {
          probeData->timeseries( probeData->startingTimestamp(),
                                 probeData->endingTimestamp(),
                                 probePoint->longitude, probePoint->latitude,
                                 emvl::MISSING, emvl::IMISSING,
                                 probeTimeseries );
          panelPlot3AddTimeseries( painter, probeSiteName, plotIndex,
                                   plot3ProbeConversion,
                                   probeTimeseries,
                                   xMin, xMax, probeMin, probeMax );
          ++plotIndex;
        }
      } // for each probeSite
    } // else point data
  } // end plot3
}



QPoint RSIGScene::panelPlot1DatatoScreen(QPointF dataPoint, int xOrigin, int yOrigin, int axisWidth, int axisHeight, double xmin, double xmax, double ymin, double ymax) {

  int yStart = yOrigin + axisHeight;
  int yEnd   = yOrigin;
  
  // set up conversion from data space to painter space
  double xSlope     = double( (xOrigin+axisWidth) - xOrigin) / (xmax - xmin);
  double xIntercept = xOrigin - (xSlope*xmin);
  //double ySlope     = double( (yOrigin+axisHeight) - yOrigin) / (ymax - ymin);
  //double yIntercept = yOrigin - (ySlope*ymin);
  double ySlope     = double( yEnd - yStart) / (ymax - ymin);
  double yIntercept = yStart - (ySlope*ymin);

  
  QPoint screenPoint;
  if (dataPoint.x() >= xmin && dataPoint.x() <= xmax && dataPoint.y() >= ymin && dataPoint.y() <= ymax) {
    screenPoint = QPoint( int(xSlope*dataPoint.x() + xIntercept), int(ySlope*dataPoint.y() + yIntercept) ) ;
  } else {
    screenPoint = QPoint(0,0);
  }
  return screenPoint;
}


int * RSIGScene::panelPlot1Init(QPainter *painter, int nPlots, int maxPlots, QString probeTitle, QString targetTitle, int x0, int y0, int panelWidth, int panelHeight, double xmin, double xmax, double ymin, double ymax) {

  
  // set up subplots (all will be square)
  int plotMarginX = 50;
  int plotMarginY = 50;
  int centerX     = x0 + panelWidth/2.0;
  int centerY     = y0 + panelHeight/2.0;
  int workingSize;
  
  int originX[nPlots];
  int originY[nPlots];
  int plotDim[nPlots];
  static int origin[36]; // return value: [x1, y1, plotDim, ... , x9, y9, plotDim]

  // set up the subplot axes
  int thisPlotDim = 0; // the size of each plot (assumed square)
  if (nPlots == 1) {
    workingSize = panelWidth < panelHeight ? panelWidth : panelHeight;
    //if (panelWidth < panelHeight) { workingSize = panelWidth; }
    //else                          { workingSize = panelHeight; }
    thisPlotDim = workingSize - 2*plotMarginX;
    plotDim[0]  = thisPlotDim;
    originX[0]  = centerX - thisPlotDim/2.0;
    originY[0]  = centerY - thisPlotDim/2.0;
  } else if (nPlots == 2) {
    workingSize = panelWidth;
    thisPlotDim = (workingSize - (3*plotMarginX) ) / 2.0;
    plotDim[0]  = thisPlotDim;
    originX[0]  = x0 + plotMarginX;
    originY[0]  = centerY - thisPlotDim/2.0;

    plotDim[1] = thisPlotDim;
    originX[1] = originX[0] + plotDim[0] + plotMarginX;
    originY[1] = originY[0];   
  } else if (nPlots == 3 || nPlots == 4) {
    workingSize = panelWidth < panelHeight ? panelWidth : panelHeight;
    thisPlotDim = (workingSize - (3*plotMarginX) ) / 2.0;

    int yOffset = plotMarginY/2;
    plotDim[0]  = thisPlotDim;
    //originX[0]  = x0 + plotMarginX;
    originX[0]  = centerX - thisPlotDim;
    originY[0]  = centerY - thisPlotDim - yOffset;

    plotDim[1] = thisPlotDim;
    //originX[1] = originX[0] + plotDim[0] + plotMarginX;
    originX[1] = centerX + plotMarginX - 10;
    originY[1] = originY[0];

    plotDim[2]  = thisPlotDim;
    originX[2]  = originX[0];
    originY[2]  = centerY + yOffset;

    plotDim[3]  = thisPlotDim;
    originX[3]  = originX[1];
    originY[3]  = centerY + yOffset;
    
  } else if (nPlots == 5 || nPlots == 6) {
    workingSize = panelWidth < panelHeight ? panelWidth : panelHeight;
    thisPlotDim = (workingSize - (3*plotMarginX) ) / 3.0;
    int yOffset = plotMarginY/2;
    plotDim[0]  = thisPlotDim;
    originX[0]  = centerX - 3*thisPlotDim/2;
    originY[0]  = centerY - thisPlotDim - yOffset;

    plotDim[1]  = thisPlotDim;
    originX[1]  = originX[0] + thisPlotDim + plotMarginX/2 + 5;
    originY[1]  = originY[0];

    plotDim[2]  = thisPlotDim;
    originX[2]  = originX[1] + thisPlotDim + plotMarginX/2 + 5;
    originY[2]  = originY[0];

    plotDim[3]  = thisPlotDim;
    originX[3]  = originX[0];
    originY[3]  = centerY + yOffset;

    plotDim[4]  = thisPlotDim;
    originX[4]  = originX[1];
    originY[4]  = centerY + yOffset;

    plotDim[5]  = thisPlotDim;
    originX[5]  = originX[2];
    originY[5]  = centerY + yOffset;
    
  } else if (nPlots <= 9) {
    workingSize = panelWidth < panelHeight ? panelWidth : panelHeight;
    thisPlotDim = (workingSize - (3*plotMarginX) ) / 3.0 - 5;

    plotDim[0]  = thisPlotDim;
    originX[0]  = centerX - 3*thisPlotDim/2;
    originY[0]  = centerY - 3*thisPlotDim/2 - plotMarginY - 10;

    plotDim[1]  = thisPlotDim;
    originX[1]  = originX[0] + thisPlotDim + plotMarginX/2 + 5;
    originY[1]  = originY[0];
    
    plotDim[2]  = thisPlotDim;
    originX[2]  = originX[1] + thisPlotDim + plotMarginX/2 + 5;
    originY[2]  = originY[0];

    plotDim[3]  = thisPlotDim;
    originX[3]  = originX[0];
    originY[3]  = originY[0] + thisPlotDim + plotMarginY;

    plotDim[4]  = thisPlotDim;
    originX[4]  = originX[1];
    originY[4]  = originY[3];

    plotDim[5]  = thisPlotDim;
    originX[5]  = originX[2];
    originY[5]  = originY[3];

    plotDim[6]  = thisPlotDim;
    originX[6]  = originX[0];
    originY[6]  = originY[3] + thisPlotDim + plotMarginY;

    plotDim[7]  = thisPlotDim;
    originX[7]  = originX[1];
    originY[7]  = originY[6];

    plotDim[8]  = thisPlotDim;
    originX[8]  = originX[2];
    originY[8]  = originY[6];
    
  }

  // order of magnitude of x and y data (to simplify annotations)
  int orderOfMagnitudeX = 0;
  if (xmax > 100) {
    //orderOfMagnitudeX = int(floor(log10(xmax)));
    orderOfMagnitudeX = QString::number(xmax, 'e', 1).right(2).toInt();
    //qDebug() << "orderOfMagnitudeX" << orderOfMagnitudeX;
  }
  int orderOfMagnitudeY = 0;
  if (ymax > 100) {
    //orderOfMagnitudeY = int(floor(log10(ymax)));
    orderOfMagnitudeY = QString::number(ymax, 'e', 1).right(2).toInt();
    //qDebug() << "orderOfMagnitudeY" << orderOfMagnitudeY;
  }
  
  // draw main plot area
  QRect plotRect = QRect(x0, y0, panelWidth, panelHeight);
  painter->drawRect(plotRect);
  //painter->drawText(plotRect, Qt::AlignHCenter, title);

  if (nPlots > 0 && nPlots <= maxPlots) {
    // x label for overall plot
    QRect titleRectX = QRect(x0, originY[nPlots-1] + plotDim[nPlots-1] + plotMarginY/2, panelWidth, 100);
    if (orderOfMagnitudeX > 0) {
      probeTitle = probeTitle.append(" x 10^" + QString::number(orderOfMagnitudeX));
    }
    
    painter->drawText(titleRectX, Qt::AlignHCenter, probeTitle);
    
    // y label for overall plot
    QRect titleRectY2 = QRect(0, 0, panelHeight-plotMarginY, plotMarginX*.45);
    if (orderOfMagnitudeY > 0) {
      targetTitle = targetTitle.append(" x 10^" + QString::number(orderOfMagnitudeY));
    }
    //painter->fillRect(titleRectY2, Qt::red);
    int xTranslate = originX[0] - plotMarginX;
    painter->translate(xTranslate, panelHeight);
    //painter->fillRect(titleRectY2, Qt::blue);
    painter->rotate(-90);
    //painter->fillRect(titleRectY2, Qt::green);
    painter->drawText(titleRectY2, Qt::AlignHCenter | Qt::AlignVCenter, targetTitle);
    painter->rotate(90);
    painter->translate(-xTranslate, -panelHeight);
  }
  
  // tick parameters
  const unsigned int nDays = RSIGStateSingleton::getInstance()->mDuration; 
  int nXticks     = 5;
  int nYticks     = 5;
  int tickLen     = 5;
  double xTickSize = double(thisPlotDim) / nXticks; // painter space
  double yTickSize = double(thisPlotDim) / nYticks; // painter space
  int xTickSkip = 1; // default
  if      (nDays == 2) { xTickSkip = 2; }
  else if (nDays == 3) { xTickSkip = 2; }
  else if (nDays == 4) { xTickSkip = 4; }
  else if (nDays == 5) { xTickSkip = 4; }
  else if (nDays == 6) { xTickSkip = 4; }
  else if (nDays == 7) { xTickSkip = 6; }

  for (int plotInd=0; plotInd<nPlots; plotInd++) {
    int thisOriginX = originX[plotInd];
    int thisOriginY = originY[plotInd];
    origin[plotInd*3 +0] = thisOriginX;
    origin[plotInd*3 +1] = thisOriginY;
    origin[plotInd*3 +2] = thisPlotDim;

    
    // axis limits
    const int yStart = thisOriginY + thisPlotDim;

    // draw axes as box
    painter->drawRect(thisOriginX, thisOriginY, thisPlotDim, thisPlotDim);

    // draw plot title
    
    // draw x ticks
    for (int tickInd=0; tickInd<nXticks; tickInd++) {
      int xTickPos = thisOriginX + tickInd*xTickSize;
      painter->drawLine(xTickPos, thisOriginY+thisPlotDim, xTickPos, thisOriginY+thisPlotDim-tickLen);
      
      // tick annotations
      double thisTickValue = (xmin + (double(tickInd)*(xmax-xmin)/double(nXticks))) / pow(10, orderOfMagnitudeX);
      if (tickInd % xTickSkip == 0) {
        painter->drawText(QPoint(xTickPos, yStart+20), QString::number(thisTickValue, 'f', 1));
      }
    }

    // draw Y axis ticks
    for (int tickInd=0; tickInd<=nYticks; tickInd++) {
      int yTickPos = thisOriginY + thisPlotDim - tickInd*yTickSize;
      painter->drawLine(thisOriginX, yTickPos, thisOriginX+tickLen, yTickPos);
      
      // tick annotations
      double thisTickValue = (ymin + (double(tickInd)*(ymax-ymin)/double(nYticks)) ) / pow(10, orderOfMagnitudeY);
      painter->drawText(QPoint(thisOriginX-20, yTickPos), QString::number(thisTickValue, 'g', 2));
    }    
    
  }

  //qDebug() << "here" << origin[0] << origin[1];
  return origin;
}

void RSIGScene::panelPlot2Init(QPainter *painter, QString title, int x0, int y0, int width, int height) {
  
  QRect plotRect = QRect(x0, y0, width, height);
  painter->drawRect(plotRect);

  //QPoint textPos = QPoint(x0+40, y0+20);
  painter->drawText(plotRect, Qt::AlignHCenter, title);

  painter->drawText(plotRect, Qt::AlignCenter, "(Currently unimplemented)");


}

double * RSIGScene::panelPlot3Init(QPainter *painter, QString title, QString probeUnits, int x0, int y0, int width, int height, double xmin, double xmax, double ymin, double ymax) {
  //qDebug() << "in panelPlot3Init";
  
  QRect plotRect = QRect(x0, y0, width, height);
  painter->drawRect(plotRect);
  painter->drawText(plotRect, Qt::AlignHCenter, title);

  
  // offset from plot area box
  int paddingX = 50;
  int paddingY = 40;

  // axis limits
  int xStart = x0 + paddingX;
  int xEnd   = xStart + width - 2*paddingX;
  int yStart = y0 + height - paddingY;
  int yEnd   = yStart - height + 2*paddingY;

  // set up conversion from data space to painter space
  double xSlope     = double(xEnd - xStart) / (xmax - xmin);
  double xIntercept = xStart - (xSlope*xmin);
  double ySlope     = double(yEnd - yStart) / (ymax - ymin);
  double yIntercept = yStart - (ySlope*ymin);


  // order of magnitude of y data (to simplify annotations)
  int orderOfMagnitudeY = 0;
  if (ymax > 100) {
    //orderOfMagnitudeY = int(floor(log10(ymax)));
    orderOfMagnitudeY = QString::number(ymax, 'e', 1).right(2).toInt();
  }

  // tick parameters
  const unsigned int nDays = RSIGStateSingleton::getInstance()->mDuration;

  int nXticks     = nDays * 24;
  int nYticks     = 10;
  int tickLen     = 5;
  double xTickSize = double(xEnd - xStart) / nXticks; // painter space
  double yTickSize = double(yEnd - yStart) / nYticks; // painter space
  int xTickSkip = 1; // default
  if      (nDays == 1) { xTickSkip = 1; }
  else if (nDays == 2) { xTickSkip = 2; }
  else if (nDays == 3) { xTickSkip = 2; }
  else if (nDays == 4) { xTickSkip = 4; }
  else if (nDays == 5) { xTickSkip = 4; }
  else if (nDays == 6) { xTickSkip = 4; }
  else if (nDays == 7) { xTickSkip = 6; }
  else                 { xTickSkip = 24;}
  
  // draw X-axis
  painter->drawLine(xStart, yStart, xEnd, yStart);
  // X axis title
  QRect titleRectX = QRect(x0, yStart+22, width, 100);
  if (nDays <= 7) {
    painter->drawText(titleRectX, Qt::AlignHCenter, "UTC Hour");
  } else {
    painter->drawText(titleRectX, Qt::AlignHCenter, "Day number");
  }
  
  // draw Y-axis
  QString yAxisTitle = probeUnits;
  painter->drawLine(xStart, yStart, xStart, yEnd);
  // Y axis title
  if (orderOfMagnitudeY > 0) {
    yAxisTitle = yAxisTitle.append(" x 10^" + QString::number(orderOfMagnitudeY));
  }
  QRect titleRectY2 = QRect(0, 0, height, 50);  
  painter->translate(20, yStart + paddingY);
  painter->rotate(-90);
  //painter->fillRect(titleRectY2, Qt::green);
  painter->drawText(titleRectY2, Qt::AlignHCenter, yAxisTitle);
  painter->rotate(90);
  painter->translate(-20, -(yStart + paddingY));

  
  // draw X axis ticks
  for (int tickInd=0; tickInd<nXticks; tickInd++) {
    int xTickPos = xStart + tickInd*xTickSize;
    if (nDays <= 7) {
      painter->drawLine(xTickPos, yStart, xTickPos, yStart-tickLen);
    } else {
      if (tickInd % xTickSkip == 0) {
        painter->drawLine(xTickPos, yStart, xTickPos, yStart-tickLen);
      }
    }
    // tick annotations
    double thisTickValue = xmin + (double(tickInd)*(xmax-xmin)/double(nXticks));
    if (nDays > 7) {
      thisTickValue = thisTickValue/24;
    }

    if (tickInd % xTickSkip == 0) {
      painter->drawText(QPoint(xTickPos, yStart+20), QString::number(thisTickValue));
    }

    // draw timestep marker
    const unsigned int timestep =  RSIGStateSingleton::getInstance()->mCounter;
    if (timestep == thisTickValue) {
      //painter->drawLine(xTickPos, yStart, xTickPos, yEnd);
      painter->fillRect(QRect(xTickPos-5, yStart, 10, yEnd-yStart), Qt::yellow);
    }
  }
  
  // draw Y axis ticks
  for (int tickInd=0; tickInd<=nYticks; tickInd++) {
    int yTickPos = yStart + tickInd*yTickSize;
    painter->drawLine(xStart, yTickPos, xStart+tickLen, yTickPos);

    // tick annotations
    //double thisTickValue = ymin + (double(tickInd)*(ymax-ymin)/double(nYticks));
    double thisTickValue = (ymin + (double(tickInd)*(ymax-ymin)/double(nYticks)) ) / pow(10, orderOfMagnitudeY);
    painter->drawText(QPoint(xStart-20, yTickPos), QString::number(thisTickValue, 'g', 2));
  }

  
  static double conversion[4];
  conversion[0] = xSlope;
  conversion[1] = xIntercept;
  conversion[2] = ySlope;
  conversion[3] = yIntercept;

  //qDebug() << "out panelPlot3Init";

  return conversion;
}

void RSIGScene::panelPlot1AddPoint(QPainter *painter, int, QPoint screenPoint, bool highlightFlag) {

  int rectSize = 5;
  int rectSizeHighlight = 11;

  // highlight current timestep
  if (highlightFlag) {
    painter->fillRect(screenPoint.x()-(rectSizeHighlight/2), screenPoint.y()-(rectSizeHighlight/2), rectSizeHighlight, rectSizeHighlight, Qt::yellow);
    painter->drawRect(screenPoint.x()-(rectSizeHighlight/2), screenPoint.y()-(rectSizeHighlight/2), rectSizeHighlight, rectSizeHighlight);
  }

  // data point
  painter->fillRect(screenPoint.x()-(rectSize/2), screenPoint.y()-(rectSize/2), rectSize, rectSize, Qt::black);
  
}



void RSIGScene::panelPlot3AddTimeseries(QPainter *painter, QString /* note */,
                                        int plotIndex,
                                        double* conversion,
                                        std::vector<double> timeseries,
                                        double xmin, double xmax,
                                        double ymin, double ymax) {

  double xSlope     = conversion[0];
  double xIntercept = conversion[1];
  double ySlope     = conversion[2];
  double yIntercept = conversion[3];

  int thisPointX = 0;
  int thisPointY = 0;
  int lastPointX = 0;
  int lastPointY = 0;

  QPen penOrig = painter->pen();
  if (plotIndex == 1) { painter->setPen(QPen(Qt::black, 2, Qt::SolidLine)); } else 
  if (plotIndex == 2) { painter->setPen(QPen(Qt::black, 2, Qt::DashLine));  } else 
  if (plotIndex == 3) { painter->setPen(QPen(Qt::black, 2, Qt::DotLine));   } else 
  if (plotIndex == 4) { painter->setPen(QPen(Qt::blue,  2, Qt::SolidLine)); } else 
  if (plotIndex == 5) { painter->setPen(QPen(Qt::blue,  2, Qt::DashLine));  } else 
  if (plotIndex == 6) { painter->setPen(QPen(Qt::blue,  2, Qt::DotLine));   } else 
  if (plotIndex == 7) { painter->setPen(QPen(Qt::cyan,  2, Qt::SolidLine)); } else 
  if (plotIndex == 8) { painter->setPen(QPen(Qt::cyan,  2, Qt::DashLine));  } else 
  if (plotIndex == 9) { painter->setPen(QPen(Qt::cyan,  2, Qt::DotLine));   }

  int pointSize = 5;
  const size_t timeseriesCount = timeseries.size();
  for (size_t ind = 0; ind < timeseriesCount; ++ind) {
    // convert data space to screen space
    thisPointX = xSlope*ind + xIntercept;
    thisPointY = ySlope*timeseries[ind] + yIntercept;
    
    if ( ind >= xmin            &&
         ind < xmax             &&
         timeseries[ind] > ymin &&
         timeseries[ind] < ymax ) {
      //painter->drawPoint(thisPointX, thisPointY);
      painter->fillRect(thisPointX-(pointSize/2), thisPointY-(pointSize/2), pointSize, pointSize, Qt::black);

      // draw connecting line
      if (ind > 0 && lastPointX !=0 && lastPointY !=0) {
        painter->drawLine(lastPointX, lastPointY, thisPointX, thisPointY);
      }

      lastPointX = thisPointX;
      lastPointY = thisPointY;
    } else {
      // reset lastPoint so that connecting line will be broken
      lastPointX = 0;
      lastPointY = 0;
    }
  }

  // add note
  //painter->drawText(lastPointX, lastPointY, note);

    painter->setPen(penOrig);

}



QString RSIGScene::getResourceDirectory() {
  //QString working_dir( QCoreApplication::applicationDirPath(); )
  QString working_dir( RSIGState::getProgramDirectory() );


#ifdef __APPLE__

  //QStringList parse = working_dir.split("RSIG3D.app");
  //resource_dir = parse.value(0).append("RSIG3D.app/Contents/resources/data");
  resource_dir = working_dir.append("/data");

#elif defined( _WIN32 )

  const std::string dir( working_dir.toStdString() );
  const char* const path = dir.c_str();

  if ( ! strchr( path, ' ' ) ) {
    resource_dir = working_dir.append( "/data" );    
  } else {
    const char* const dosPath = emvl::DatasetManager::DOSPath( path );
    resource_dir.clear();
    resource_dir.append( dosPath );
    resource_dir.append( "/data" );
  }
  
#else
  resource_dir = working_dir.append("/data");
#endif
  
  return resource_dir;
}



void RSIGScene::renderBitmapString( float x, float y,  float z,
                                    void *font, char *string) {
  glRasterPos3f(x, y, z);

  for ( ; *string; ++string ) {
    glutBitmapCharacter( font, *string );
  }
}



void renderStrokeString( float thisLon, float thisLat,  float thisElev,
                         void *font, char *string,
                         float scaleFactor,  float rotationAngle,
                         int labelOrientation) {
  glPushMatrix();
  glTranslatef(thisLon, thisLat, thisElev);
  glScalef(scaleFactor, scaleFactor, scaleFactor);
  glRotatef(rotationAngle, 0, 0, 1);
  glRasterPos3f(thisLon, thisLat, thisElev);
  
  for ( ; *string; ++string ) {

    if (labelOrientation == 0) {
      glutStrokeCharacter(font, *string);
    } else {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string);
    }
  }

  glPopMatrix();

  glBegin(GL_POINTS);
  glVertex3f(thisLon, thisLat, thisElev);    
  glEnd();
}  



void drawElevationRuler() {

  // get ruler coordinates
  VisOptions *vOptions = VisOptionsSingleton::getInstance();

  if (vOptions->getShowRuler()) {
    float lon = vOptions->getRulerLon();
    float lat = vOptions->getRulerLat();
    const double degreesPerMeter = 1.0/110574.2727;
    const double zScaleFactor = vOptions->getCalipsoScaleFactor(); 
    double elevationScale = degreesPerMeter * zScaleFactor;
    
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2.0);
    
    // base
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    glVertex3f(lon, lat, 0.0);
    glEnd();
    
    float height = 20000.0; // height of ruler in meters

    // tick marks (every 5000 meters)
    float tickLength = 0.1; // degrees
    float tickSpacing = 5000.0; // meters
    float subTickSpacing = 1000.0; // meters
    int nTicks = floor(height / tickSpacing);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i=0; i<nTicks; i++) {
      float tickHeight = i * tickSpacing; // meters
      glBegin(GL_LINES);
      glVertex3f(lon, lat+tickLength, tickHeight * elevationScale);
      glVertex3f(lon, lat-tickLength, tickHeight * elevationScale);
      glVertex3f(lon+tickLength, lat, tickHeight * elevationScale);
      glVertex3f(lon-tickLength, lat, tickHeight * elevationScale);
      glEnd();

      RSIGScene& rsigScene = RSIGScene::getInstance();
      char tickString[50];
      sprintf(tickString, "     %d km", (int)floor(tickHeight/1000) );
      rsigScene.renderBitmapString(lon, lat, tickHeight * elevationScale, GLUT_BITMAP_HELVETICA_12, tickString);

      // sub ticks (between major divisions)
      int nSubTicks = floor(tickSpacing / subTickSpacing);

      for (int j=0; j<nSubTicks; j++) {
        float subTickHeight = tickHeight + j * subTickSpacing; // meters
        glBegin(GL_LINES);
        glVertex3f(lon, lat+(tickLength/2), subTickHeight * elevationScale);
        glVertex3f(lon, lat-(tickLength/2), subTickHeight * elevationScale);
        //glEnd();
        //glBegin(GL_LINES);
        glVertex3f(lon+(tickLength/2), lat, subTickHeight * elevationScale);
        glVertex3f(lon-(tickLength/2), lat, subTickHeight * elevationScale);
        glEnd();
      }
    }

   // main vertical line
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(lon, lat, 0.0);
    glVertex3f(lon, lat, height * elevationScale);
    glEnd();
  }
}



void RSIGScene::drawMapOverlay() {
  // For drawing maplines on top of all data layers (both 2D and 3D)
  
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  QString maplineColor   = vOptions->getMaplineColor();
  //bool mode2d = vOptions->getMode2D();
  int displayMode = vOptions->getDisplayMode();

  // set flag for drawing mapline color

  //if (mode2d) {
  if (displayMode == 0) { // 2D

    if (m_useMarbleWidget) {

      switch(m_projectionMode) {
      case RSIGState::PROJECTION_LONGLAT:
        marbleUVProjectionRC.setMaplineFlag(lut0->texureID(),         1, maplineColor);
        break;
      case RSIGState::PROJECTION_MERCATOR:
        marbleMercatorProjectionRC.setMaplineFlag(lut0->texureID(),   1, maplineColor);
        break;
      case RSIGState::PROJECTION_SPHERICAL:
        marbleSphericalProjectionRC.setMaplineFlag(lut0->texureID(),  1, maplineColor);
        break;
      case RSIGState::PROJECTION_LAMBERTCONFORMALCONIC:
        lambertConformalProjectionRC.setMaplineFlag(lut0->texureID(), 1, maplineColor);
        break;
      case RSIGState::PROJECTION_STEREOGRAPHIC:
        stereographicProjectionRC.setMaplineFlag(lut0->texureID(),    1, maplineColor);
        break;
      default:
        break;
      }

    } else {
      qmapMercatorProjectionRC.setMaplineFlag(lut0->texureID(),     1, maplineColor);
    }
  }


  if (displayMode == 0 || displayMode == 1) { // 2D or 3D modes
  
    const QString& mapOverlayFile = vOptions->getMapOverlayFile(); // May be "none".
    
    if ( mapOverlayFile != previousMapOverlayFile ) { // Map line file changed:
      previousMapOverlayFile = mapOverlayFile; // Remember this new file for later.
      
      if ( mapOverlayDisplayList > 0 ) { // Delete display list if it exists.
        glDeleteLists( mapOverlayDisplayList, 1 );
        mapOverlayDisplayList = 0;
      }
      
      if ( mapOverlayFile != "none" ) {
        const QString pathedFileName( getResourceDirectory().append( mapOverlayFile ) );
        const std::string copy( pathedFileName.toStdString() ); // Must create object.
        const char* const fileName = copy.c_str();
        
        // Load map file into display list:
        
        mapOverlayDisplayList = loadMapFile( fileName );
      }
    }
    
    if ( mapOverlayDisplayList > 0 ) { // If map lines display list exists:
      glLineWidth(1.5);
      
      // Set color from name:
      
      typedef struct { const char* name; float r; float g; float b; } Entry;
      static const Entry table[] = {
                                    { "White",  1.0, 1.0, 1.0 },
                                    { "Black",  0.0, 0.0, 0.0 },
                                    { "Gray10", 0.1, 0.1, 0.1 },
                                    { "Gray20", 0.2, 0.2, 0.2 },
                                    { "Gray30", 0.3, 0.3, 0.3 },
                                    { "Gray40", 0.4, 0.4, 0.4 },
                                    { "Gray50", 0.5, 0.5, 0.5 },
                                    { "Gray60", 0.6, 0.6, 0.6 },
                                    { "Gray70", 0.7, 0.7, 0.7 },
                                    { "Gray80", 0.8, 0.8, 0.8 },
                                    { "Gray90", 0.9, 0.9, 0.9 }
      };
      const size_t count = sizeof table / sizeof *table;
      
      for ( size_t index = 0; index < count; ++index ) {
        const Entry* const entry = table + index;
        
        if ( ! maplineColor.compare( entry->name ) ) {
          glColor4f( entry->r, entry->g, entry->b, 1.0 );
          index = count; // Stop looping.
        }
      }
      
      glCallList( mapOverlayDisplayList );
    }
  }

      
  // unset flag for drawing mapline color

  //if (mode2d) {
  if (displayMode == 0) { // 2D

    if (m_useMarbleWidget) {

      switch(m_projectionMode) {
      case RSIGState::PROJECTION_LONGLAT:
        marbleUVProjectionRC.setMaplineFlag(lut0->texureID(),         0, maplineColor);
        break;
      case RSIGState::PROJECTION_MERCATOR:
        marbleMercatorProjectionRC.setMaplineFlag(lut0->texureID(),   0, maplineColor);
        break;
      case RSIGState::PROJECTION_SPHERICAL:
        marbleSphericalProjectionRC.setMaplineFlag(lut0->texureID(),  0, maplineColor);
        break;
      case RSIGState::PROJECTION_LAMBERTCONFORMALCONIC:
        lambertConformalProjectionRC.setMaplineFlag(lut0->texureID(), 0, maplineColor);
        break;
      case RSIGState::PROJECTION_STEREOGRAPHIC:
        stereographicProjectionRC.setMaplineFlag(lut0->texureID(),    0, maplineColor);
        break;
      default:
        break;
      }
    } else {
      qmapMercatorProjectionRC.setMaplineFlag(lut0->texureID(),     0, maplineColor);
    }
  }
}



static void draw3DMaplines() {
  // For drawing the maplines in the 3D view. Note, these maplines are underneath all data layers.
  // Additional maplines are drawn on top of the data if the user so chooses using drawMapOverlay()
  
  RSIGScene& rsigScene = RSIGScene::getInstance();

  // switch maplines based on zoom level
  if (fov_angle < 0.3) {

    if (mapline_index != 0) {
      mapline_index = 0;
      //printf("loading county map\n");
      glDeleteLists( mapLDisplayList, 1 );
      resource_dir = rsigScene.getResourceDirectory(); // do not factor out because we only want to go to disk if the map needs to change
      const QString fn( resource_dir.append("/image/map_counties.bin") );
      const std::string copy( fn.toStdString() );
      const char* const fileName = copy.c_str();
      mapLDisplayList = loadMapFile(fileName);
    }
  } else if (fov_angle < 5.0) {

    if (mapline_index != 1) {
      mapline_index = 1;
      //printf("loading na map\n");
      glDeleteLists( mapLDisplayList, 1 );
      resource_dir = rsigScene.getResourceDirectory(); // do not factor out because we only want to go to disk if the map needs to change
      const QString fn( resource_dir.append("/image/map_states.bin") );
      const std::string copy( fn.toStdString() );
      const char* const fileName = copy.c_str();
      mapLDisplayList = loadMapFile(fileName);
    }
  } else if (fov_angle >= 5.0) {

    if (mapline_index != 2) {
      mapline_index = 2;
      //printf("loading world map\n");
      glDeleteLists( mapLDisplayList, 1 );
      resource_dir = rsigScene.getResourceDirectory(); // do not factor out because we only want to go to disk if the map needs to change
      const QString fn( resource_dir.append("/image/map_world.bin") );
      const std::string copy( fn.toStdString() );
      const char* const fileName = copy.c_str();
      mapLDisplayList = loadMapFile(fileName);
    }
  }

  if (mapLDisplayList > 0) {
    glLineWidth(0.5);
    //glColor3f(1.0, 1.0, 1.0);
    glColor3f(0.3, 0.3, 0.3);
    glCallList( mapLDisplayList );
  }
}



static void updateMapLines() {
  // only called in 3D mode

  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  int displayMode = vOptions->getDisplayMode();

  if (displayMode == 0 || displayMode == 1) { //2D or 3D
    draw3DMaplines();

    // draw lat/lon lines if desired
    float fovAngleThreshold1 = 1.25;
    float fovAngleThreshold2 = 0.5;
    float fovAngleThreshold3 = 0.15;
    
    RSIGState *gState = RSIGStateSingleton::getInstance();
    
    if (gState->mShowLonlatLines) {
      float lat_spacing; //degrees
      float lon_spacing; //degrees
      
      if (fov_angle < fovAngleThreshold3) {
        lat_spacing = 0.5;
        lon_spacing = 0.5;
      } else if (fov_angle < fovAngleThreshold2) {
        lat_spacing = 1.0;
        lon_spacing = 1.0;
      } else if (fov_angle < fovAngleThreshold1) {
        lat_spacing = 2.0; 
        lon_spacing = 2.0; 
      } else {
        lat_spacing = 10; 
        lon_spacing = 10; 
      }
      
      glColor3f(0.7, 0.7, 0.5);
      glLineWidth(0.5);
      glLineStipple(2, 0xAAAA);
      glEnable(GL_LINE_STIPPLE);
      
      // draw lat lines
      for (int thisInd=0; thisInd<180/lat_spacing; thisInd++) {
        float thisLat = -90 + thisInd*lat_spacing;
        // for main lines at 10 degree increments
        
        if (fmod(thisLat,10) == 0 && fov_angle < fovAngleThreshold1) {
          glLineWidth(2.0);
          glDisable(GL_LINE_STIPPLE);
        } else if (fmod(thisLat,2) == 0 && fov_angle < fovAngleThreshold2) {
          glLineWidth(1.0);
          glDisable(GL_LINE_STIPPLE);
        } else {
          glLineWidth(0.5);
          glEnable(GL_LINE_STIPPLE);
        }
        
        glBegin(GL_LINES);
        glVertex3f(-180, thisLat, 0.0);
        glVertex3f( 180, thisLat, 0.0);
        glEnd();
      } 
      
      // draw lon lines
      for (int thisInd=0; thisInd<360/lon_spacing; thisInd++) {
        float thisLon = -180 + thisInd*lon_spacing;
        
        if (fmod(thisLon,10) == 0 && fov_angle < fovAngleThreshold1) {
          glLineWidth(2.0);
          glDisable(GL_LINE_STIPPLE);
        } else if (fmod(thisLon,2) == 0 && fov_angle < fovAngleThreshold2) {
          glLineWidth(1.0);
          glDisable(GL_LINE_STIPPLE);
        } else {
          glLineWidth(0.5);
          glEnable(GL_LINE_STIPPLE);
        }
        
        glBegin(GL_LINES);
        glVertex3f(thisLon, -90, 0.0);
        glVertex3f(thisLon,  90, 0.0);
        glEnd();
      }
      
      glDisable(GL_LINE_STIPPLE);
      
      // LABEL lines  
      // HACK: Don't draw labels while mouse is down, because Windows takes a long
      // time to draw them. Maybe because of gluProject()?
      
      if (!mouseDown) {
        VisOptions *vOptions = VisOptionsSingleton::getInstance();
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        
        glGetIntegerv(GL_VIEWPORT, viewport);   
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        
        char latlonString[20];
        GLdouble xWinLoc, yWinLoc, zWinLoc;
        float labelIncrement; // degrees
        glColor3f(1.0, 1.0, 0.7);
        glPointSize( 3.0 );
        
        const char * stringFormat;
        
        if (fov_angle < fovAngleThreshold3) {
          labelIncrement = 0.5;
          
          if (vOptions->getLonlatLabelOrientation()) {
            stringFormat = "%9.1f";
          } else {
            //stringFormat = "%5.1f";
            stringFormat = "%9.1f";
          }
        } else if (fov_angle < fovAngleThreshold2) {
          labelIncrement = 1;
          
          if (vOptions->getLonlatLabelOrientation()) {
            stringFormat = "%8.0f";
          } else {
            //stringFormat = "%4.0f";
            stringFormat = "%8.0f";
          }
        } else if (fov_angle < fovAngleThreshold1) {
          labelIncrement = 2;
          
          if (vOptions->getLonlatLabelOrientation()) {
            stringFormat = "%8.0f";
          } else {
            //stringFormat = "%4.0f";
            stringFormat = "%8.0f";
          }
        } else {
          labelIncrement = 10;
          
          if (vOptions->getLonlatLabelOrientation()) {
            stringFormat = "%8.0f";
          } else {
            //stringFormat = "%4.0f";
            stringFormat = "%8.0f";
          }
        }
        
        int winTolerance = 50; // edge tolerance in screen space
        int winOffsetX = 20;
        int winOffsetY = 60;
        float testIncrement = 0.1; // degrees
        // figure out where lat lines intersect left edge of screen
        
        for (int labelInd=0; labelInd<180/labelIncrement; labelInd++) {
          float labelLat = -90 + labelInd*labelIncrement;
          bool labelPrinted = false;
          
          for (int testInd=0; testInd<ceil(360/testIncrement); testInd++) {
            float testLon = -180 + testInd*testIncrement;
            gluProject(testLon, labelLat, 0.0, modelview, projection, viewport, &xWinLoc, &yWinLoc, &zWinLoc);
            
            if (xWinLoc > winOffsetX && xWinLoc < winOffsetX+winTolerance && labelPrinted == false) {
              sprintf(latlonString, stringFormat, labelLat );
              // HACK: Stroke characters look like crap on some Windows machines
              //       Use Bitmap characters instead
              //renderStrokeString(testLon, labelLat, 0, GLUT_STROKE_MONO_ROMAN, latlonString, scaleFactor, 0.0, vOptions->getLonlatLabelOrientation());
              RSIGScene& rsigScene = RSIGScene::getInstance();
              rsigScene.renderBitmapString(testLon, labelLat, 0, GLUT_BITMAP_HELVETICA_12, latlonString);
              labelPrinted = true;
            }
          }
        }
        
        // figure out where lon lines intersect top edge of screen
        for (float labelInd=0; labelInd<360/labelIncrement; labelInd++) {
          float labelLon = -180 + labelInd*labelIncrement;        
          bool labelPrinted = false;
          
          for (int testInd=0; testInd<ceil(180/testIncrement); testInd++) {
            float testLat = 90 - testInd*testIncrement;
            gluProject(labelLon, testLat, 0.0, modelview, projection, viewport, &xWinLoc, &yWinLoc, &zWinLoc);
            
            if (yWinLoc < sceneHeight-winOffsetY && yWinLoc > sceneHeight-winOffsetY-winTolerance && labelPrinted == false) {
              sprintf(latlonString, stringFormat, labelLon );
              // HACK: Stroke characters look like crap on some Windows machines
              //       Use Bitmap characters instead
              //renderStrokeString(labelLon, testLat, 0, GLUT_STROKE_ROMAN, latlonString, scaleFactor, 90.0, vOptions->getLonlatLabelOrientation());
              RSIGScene& rsigScene = RSIGScene::getInstance();
              rsigScene.renderBitmapString(labelLon, testLat, 0, GLUT_BITMAP_HELVETICA_12, latlonString);
              labelPrinted = true;
            }
          }
        }
      }
    }
  }
}



//static void updateSceneSphere() {
//
//  GLUquadricObj *qObj = gluNewQuadric();
//  gluQuadricNormals(qObj, GLU_SMOOTH);
//  gluQuadricTexture(qObj, GL_TRUE);
//
//  glEnable(GL_TEXTURE_2D);
//  glBindTexture(GL_TEXTURE_2D, mapTextureId);
//  gluSphere(qObj, 180.0f, 64, 64);
//}



static void updateSceneSphere3() {

  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, mapTextureId);

  int numLat = 60; // evenly goes into 180
  int numLon = 60; // evenly goes into 360
  float incrementLat = 180.0 / numLat;
  float incrementLon = 360.0 / numLon;
  float minLon = -180.0;
  float minLat = -90.0;
  float radius = 100.0;
  float latSW, latSE, latNE, latNW;
  float lonSW, lonSE, lonNE, lonNW;
  float xSW, xSE, xNE, xNW;
  float ySW, ySE, yNE, yNW;
  float zSW, zSE, zNE, zNW;
  float uSW, uSE, uNE, uNW;
  float vSW, vSE, vNE, vNW;

  glBegin (GL_QUADS);

  for (int lonInd = 0; lonInd<numLon; lonInd++) {

    uSW = lonInd     / float(numLon);
    uSE = (lonInd+1) / float(numLon);
    uNE = uSE;
    uNW = uSW;

    lonSW = minLon + incrementLon * lonInd;
    lonSE = minLon + incrementLon * (lonInd+1);
    lonNE = lonSE;
    lonNW = lonSW;

    for (int latInd = 0; latInd<numLat; latInd++) {
      vSW = (numLat - latInd)     / float(numLat);
      vSE = vSW;
      vNE = (numLat - latInd - 1) / float(numLat);
      vNW = vNE;

      latSW = minLat + incrementLat * latInd;
      latSE = latSW;
      latNE = minLat + incrementLat * (latInd + 1);
      latNW = latNE;

      xSW = lonlat_to_cartX(radius, lonSW, latSW);
      xSE = lonlat_to_cartX(radius, lonSE, latSE);
      xNE = lonlat_to_cartX(radius, lonNE, latNE);
      xNW = lonlat_to_cartX(radius, lonNW, latNW);

      ySW = lonlat_to_cartY(radius, lonSW, latSW);
      ySE = lonlat_to_cartY(radius, lonSE, latSE);
      yNE = lonlat_to_cartY(radius, lonNE, latNE);
      yNW = lonlat_to_cartY(radius, lonNW, latNW);

      zSW = lonlat_to_cartZ(radius, latSW);
      zSE = lonlat_to_cartZ(radius, latSE);
      zNE = lonlat_to_cartZ(radius, latNE);
      zNW = lonlat_to_cartZ(radius, latNW);     
      
      // Vertices must be in counter-clockwise order.
      // Texture coordinates have origin at upper-left corner (NW),
      // while vertex coordinates are in world map lon-lat space.

      glTexCoord2d(uSW, vSW); 
      glVertex3d(xSW, ySW, zSW);

      if (lonInd==0) {
        //printf("%f %f %f %f %f\n", uSW, vSW, xSW, ySW, zSW);
      }

      glTexCoord2d(uSE, vSE); 
      glVertex3d(xSE, ySE, zSE);

      glTexCoord2d(uNE, vNE); 
      glVertex3d(xNE, yNE, zNE);

      glTexCoord2d(uNW, vNW); 
      glVertex3d(xNW, yNW, zNW);
    }
  }

  glEnd();

  updateMapLines();
}

void RSIGScene::updateScene( void ) {
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // texture map the world map
  glBindTexture( GL_TEXTURE_2D, mapTextureId );

  /*
   * Vertices must be in counter-clockwise order.
   * Texture coordinates have origin at upper-left corner (NW),
   * while vertex coordinates are in world map lon-lat space.
   */

  glBegin( GL_QUADS );
  glTexCoord2d( 0.0, 1.0);    /* SW. */
  glVertex2d( -180.0, -90.0 );
  glTexCoord2d( 1.0, 1.0);    /* SE. */
  glVertex2d( 180.0, -90.0 );
  glTexCoord2d( 1.0, 0.0);    /* NE. */
  glVertex2d( 180.0, 90.0 );
  glTexCoord2d( 0.0, 0.0);    /* NW. */
  glVertex2d( -180.0, 90.0 );
  glEnd();

  glBindTexture( GL_TEXTURE_2D, 0 );

  // texture map the MODIS True Color image 
  VisOptions *vOptions = VisOptionsSingleton::getInstance();

  if (vOptions->getDrawModisTrueColor() && modisTrueColorTextureId > 0) {
    float origColor[4];
    glGetFloatv(GL_CURRENT_COLOR, origColor);
    glEnable(GL_BLEND);                
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f(0,0,0,vOptions->getOpacityModisTrueColor()/100.0);
    glBindTexture( GL_TEXTURE_2D, modisTrueColorTextureId );
    const float minlon = vOptions->getBboxMinLon();
    const float minlat = vOptions->getBboxMinLat();
    const float maxlon = vOptions->getBboxMaxLon();
    const float maxlat = vOptions->getBboxMaxLat();

    /*
     * Vertices must be in counter-clockwise order.
     * Texture coordinates have origin at upper-left corner (NW),
     * while vertex coordinates are in world map lon-lat space.
     */

    glBegin( GL_QUADS );
    glTexCoord2d( 0.0, 1.0);    /* SW. */
    glVertex2d( minlon, minlat );
    glTexCoord2d( 1.0, 1.0);    /* SE. */
    glVertex2d( maxlon, minlat );
    glTexCoord2d( 1.0, 0.0);    /* NE. */
    glVertex2d( maxlon, maxlat );
    glTexCoord2d( 0.0, 0.0);    /* NW. */
    glVertex2d( minlon, maxlat );
    glEnd();

    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_BLEND);
    glColor4f(origColor[0], origColor[1], origColor[2], origColor[3]);                
  }

  updateMapLines();
}



void drawGLRect(float minlon, float minlat, float maxlon, float maxlat) {
  glColor3f(0.784f, 0.0f, 0.784f);

  glLineWidth(2.0);
  glBegin(GL_LINE_LOOP);
  // Vertices must be in counter-clockwise order.
  glVertex2f(minlon, minlat);
  glVertex2f(minlon, maxlat);
  glVertex2f(maxlon, maxlat);
  glVertex2f(maxlon, minlat);
  glEnd();
}


void RSIGScene::reloadModisTrueColor(int dayNum) {

  if (dayNum == -1) {
    fnLast = "";
  } else {
    VisOptions *vOptions = VisOptionsSingleton::getInstance();
    MainWin& mainWin = MainWin::getInstance();
    ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
    QString curProjection = controlDockWidget->currentMapProjection();
    QString fn = "";
    QString curTrueColor = vOptions->getTrueColorType();

    //qDebug() << curProjection;
    
    if (curProjection.compare("Longitude/Latitude") == 0 && curTrueColor != "None") {
      fn = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + QString::number(dayNum) + "_4326.png";
    } else if (curProjection.compare("Mercator") == 0 && curTrueColor != "None") {
      fn = QDir::homePath() + "/.rsig/trueColor_" + curTrueColor + "_" + QString::number(dayNum) + "_3857.png";
    } else {
      modisTrueColorIsValid = false;
      delete modisTrueColorPixmap;
      modisTrueColorPixmap = NULL;
      modisTrueColorTextureId = 0;
      fnLast = "";
    }
        
    if ( QFile(fn).exists() && QFile(fn).size() > 2048 ) {

      if (fn.compare(fnLast) != 0) {
        modisTrueColorIsValid = false; // default
        delete modisTrueColorPixmap;
        modisTrueColorPixmap = NULL;
        modisTrueColorTextureId = 0;
        fnLast = fn;

        try {
          const std::string copy( fn.toStdString() );
          const char* const imageFilename = copy.c_str();

          // for 2D rendering

          if (modisTrueColorPixmap) {
            modisTrueColorPixmap->load(imageFilename);
          } else {
            delete modisTrueColorPixmap;
            modisTrueColorPixmap = new QPixmap(imageFilename);
          }
          
          // for 3D rendering

          int imageWidth, imageHeight;
          modisTrueColorTextureId =
            loadPNG( imageFilename, &imageWidth, &imageHeight );

          if (modisTrueColorPixmap && modisTrueColorTextureId>0) {
            // image was successfully loaded
            modisTrueColorIsValid = true;
          }
        } catch (...) {
          //qDebug() << "Could not load trueColor imagery";
          modisTrueColorIsValid = false;
          delete modisTrueColorPixmap;
          modisTrueColorPixmap = NULL;
          modisTrueColorTextureId = 0;
        }
      }
    } else {
      //qDebug() << "trueColor imagery does not exist";
      modisTrueColorIsValid = false;
      delete modisTrueColorPixmap;
      modisTrueColorPixmap = NULL;
      modisTrueColorTextureId = 0;
    }
  }
}


void RSIGScene::drawDisplayPanelSceneEMVL(QPainter *painter) {
  //qDebug() << "in drawDisplayPanelSceneEMVL";

  OpenGLScene::checkPaintEngine(painter);
  const QRect screenrect(0, 0, sceneRect().width(), sceneRect().height());
  sceneHeight = sceneRect().height();
  sceneWidth = sceneRect().width();
  painter->eraseRect(screenrect);
  painter->beginNativePainting();

  // background
  //QColor backgroundColor = QColor(253, 245, 230); // linen
  QColor backgroundColor = QColor(255, 255, 255); // white
  painter->fillRect(0, 0, sceneWidth, sceneHeight, backgroundColor);

  
    // draw data
  VisDataList * vislist = VisDataListSingleton::getInstance();
  CHECK_GL_ERRORS2;
  //vislist->renderGeometryObjects(painter, screenrect, NULL);
  //drawTimeseries();
  drawPanelTimeseries(painter);
  CHECK_GL_ERRORS2;

  if (vislist->size() == 0) {
    // content in case there is no data
    //painter->drawRect(50, 50, 200, 200);
  }

  
  CHECK_GL_ERRORS2;
  painter->endNativePainting();
}


void RSIGScene::draw3DSceneEMVL(QPainter *painter) {
  // pure OpenGL scene that does not use marble (used for 3D mode)

  OpenGLScene::checkPaintEngine(painter);
  const QRect screenrect(0, 0, sceneRect().width(), sceneRect().height());
  sceneHeight = sceneRect().height();
  sceneWidth = sceneRect().width();
  painter->eraseRect(screenrect);
  painter->beginNativePainting();

  // Initialize OpenGL for 3D view

  if (draw3DInitialized == 0) {
    resetView3D(1);
    draw3DInitialized = 1;

    //init rendering contexts of OpenGL

    if (lut0 == NULL) {
      lut0.reset( new LookupTableWidget );
      lut0->setDefaultLookupTable();
      lut0->setObjectName("GASP AOD");
      lut0->genTexture();
    }

    glMatrixMode(GL_MODELVIEW);    
    glEnable( GL_TEXTURE_2D );
    glClearDepth( 1.0 );
    glDepthFunc( GL_LESS );
    glShadeModel( GL_SMOOTH );
    glDisable( GL_LIGHTING );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    resource_dir = getResourceDirectory();
    const QString qFileName( resource_dir.append("/image/map_image_low.png") );
    //const QString qFileName( resource_dir.append("/image/map_image_low_pow2.png") );
    const std::string copy( qFileName.toStdString() );
    const char* const fileName = copy.c_str();
    int imageWidth = 0;
    int imageHeight = 0;
    mapTextureId = loadPNG( fileName, &imageWidth, &imageHeight );

    aspect = (float)sceneRect().width() / (float)sceneRect().height();

    if ( mapTextureId ) {
      
      // translate model to middle of z buffer
      glMatrixMode(GL_MODELVIEW);

      glViewport( 0, 0, sceneRect().width(), sceneRect().height() );
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(fov_angle, aspect, zNear, zFar);
      gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);  

      // store the modelview matrix
      glMatrixMode(GL_MODELVIEW);
      glGetDoublev(GL_MODELVIEW_MATRIX, stored_modelview);
      glLoadIdentity();
      update();
      
    }

  } else { //  screen update for 3D view
    aspect = (float)sceneRect().width() / (float)sceneRect().height();
    glViewport( 0, 0, sceneRect().width(), sceneRect().height() );

    VisOptions *vOptions = VisOptionsSingleton::getInstance();
    int viewResetFlag = vOptions->getViewResetFlag();

    if (viewResetFlag == 1 || viewResetFlag == 4) {
      resetView3D(viewResetFlag);
    }
    
    // apply zoom and pan
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov_angle, aspect , zNear, zFar);
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);  
    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(stored_modelview);

    // pivot around specified point

    if (drawRotPoint) {
      glTranslatef(rotCenterLon, rotCenterLat, rotCenterZ);

      if (zRotateFlag == 0) {
        glRotatef(rotAngleX, 1, 0, 0);
        glRotatef(rotAngleY, 0, 1, 0);
        
        // don't allow view from underneath the map
        GLdouble x1, y1, z1;
        GLdouble x2, y2, z2;
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        glGetIntegerv(GL_VIEWPORT, viewport);   
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        gluProject(-98.5, 39.8, 0, modelview, projection, viewport, &x1, &y1, &z1);
        gluProject(-98.5, 39.8, 0.1, modelview, projection, viewport, &x2, &y2, &z2);
        float xN = x2-x1;
        float yN = y2-y1;
        float zN = z2-z1;
        // find angle between map normal and camera-eye vector
        float cos_theta = zN / sqrt(xN*xN + yN*yN + zN*zN);
        //printf("cos_theta = %f %f %f %f\n", cos_theta, xN, yN, zN);
        
        if (cos_theta >= 0.0) {
          glRotatef(-rotAngleX, 1, 0, 0);
          glRotatef(-rotAngleY, 0, 1, 0);
        }
        
      } else {
        glRotatef(rotAngleZ, 0, 0, 1);
      }

      glTranslatef(-rotCenterLon, -rotCenterLat, -rotCenterZ);
      
      // reset rotation so than if you hold the mouse button down the scene does not keep rotating
      rotAngleX = 0;
      rotAngleY = 0;
      rotAngleZ = 0;
    }

    // store the modelview matrix  
    glGetDoublev(GL_MODELVIEW_MATRIX, stored_modelview);

  }
  
  // Draw MODIS True Color - actual drawing is done in updateScene():

  const emvl::Timestamp timestamp( RSIGStateSingleton::getInstance()->getCurrentTimestamp() );
  const int yyyymmdd = timestamp.yyyymmdd();
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  vOptions->setCurrentDayNum( yyyymmdd );

  if (vOptions->getDrawModisTrueColor()) {
    reloadModisTrueColor( yyyymmdd );
  }

  // draw map
  if (isSphericalProj()) {
    updateSceneSphere3();
  } else {
    updateScene();
    drawElevationRuler();
  }

  // draw bounding box
  const float minlon = m_roiRect.x();
  const float minlat = m_roiRect.y();
  const float w      = m_roiRect.width();
  const float h      = m_roiRect.height();
  drawGLRect(minlon, minlat, minlon+w, minlat+h);
  
  // draw data
  VisDataList * vislist = VisDataListSingleton::getInstance();
  CHECK_GL_ERRORS2;
  vislist->renderGeometryObjects(painter, screenrect, NULL);
  CHECK_GL_ERRORS2;

  if (vislist->size() == 0) {
    // if there are no vis objects, draw maplines here
    // otherwsie they are drawn in VisDataList so that the are not on top of popup plots
    this->drawMapOverlay();
  }

  // draw rotation center point
  if (drawRotPoint) {
    glPointSize( 8.0 );
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(rotCenterLon, rotCenterLat);
    glEnd();
  }

  updateProbe();
  CHECK_GL_ERRORS2;
  painter->endNativePainting();

}



/*!
 \fn void RSIGScene::draw3DScene(QPainter *painter)

 Overloaded function. 
 */
void RSIGScene::draw3DScene(QPainter* painter) {
  //qDebug() << "in draw3DScene";
  CHECK_GL_ERRORS2;

  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  int displayMode = vOptions->getDisplayMode();

  if (displayMode == 0 || displayMode == 1) {
    OpenGLScene::checkPaintEngine(painter);
    const QRect screenrect(0, 0, sceneRect().width(), sceneRect().height());
    
    static int glewInitialized = 0;
    
    if (glewInitialized==0) {  //init glew library
      glewInitialized = 1;
      const GLenum err = glewInit();
      
      if (err != GLEW_OK) {
        fprintf(stderr, "GLEW error: %s\n", glewGetErrorString(err));
        // TEMP HACK qFatal("Initialization of GLEW failed!");
        // TEMP HACK exit(1);
        return;
      }
      
      //init rendering contexts of OpenGL
      
      CHECK_GL_ERRORS2;
      initRenderingContexts();
      CHECK_GL_ERRORS2;
      
      if (lut0 == NULL) {
        // set defaults here. Actual values will be set in PointRenderer, PolygonRenderer, etc.
        lut0.reset( new LookupTableWidget );
        lut0->setDefaultLookupTable();
        lut0->setObjectName("DefaultLookupTable");
        lut0->genTexture();
      }
    }
    
    // draw long/lat lines and bounding box
    
    draw2DSceneEMVL(painter);
    painter->beginNativePainting();
    int viewResetFlag = vOptions->getViewResetFlag();
    
    if (viewResetFlag) {
      resetView2D();
    }
    
    CHECK_GL_ERRORS2;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    const float zdepth = 10000;
    glOrtho(0, screenrect.width(), 0, screenrect.height(), -zdepth, zdepth);
    glViewport(0, 0, screenrect.width(), screenrect.height());
    glMatrixMode(GL_MODELVIEW);
    CHECK_GL_ERRORS2;
    
    RenderingContextRSIGBase *context = NULL;
    std::vector<float> myValues = lut0->getValues();
    std::vector<Vector4f> myPalette = lut0->colorPalette();
    std::vector<float> myPaletteR;
    std::vector<float> myPaletteG;
    std::vector<float> myPaletteB;
    myPaletteR.clear();
    myPaletteG.clear();
    myPaletteB.clear();
    for (size_t palInd=0; palInd<myPalette.size(); palInd++) {
      myPaletteR.push_back( myPalette[palInd].x );
      myPaletteG.push_back( myPalette[palInd].y );
      myPaletteB.push_back( myPalette[palInd].z );
      //qDebug() << "rsigscene" << myPaletteR[palInd] << myPaletteG[palInd] << myPaletteB[palInd];
    }
        
    if (m_useMarbleWidget) {
      const Marble::ViewportParams * viewport = m_marbleWidget->viewport();
      const GLfloat vwidth  = viewport->width();
      const GLfloat vheight = viewport->height();
      const GLfloat viewsize[2]={vwidth, vheight};
      const GLfloat rad2Pixel = viewport->radius() / M_PI_2;
      const GLfloat centerLon = viewport->centerLongitude();
      const GLfloat centerLat = viewport->centerLatitude();
      const GLfloat uvcenter[2]={centerLon, centerLat};
      
      switch (m_projectionMode) {
      case RSIGState::PROJECTION_LONGLAT:
        marbleUVProjectionRC.bind();
        marbleUVProjectionRC.setParameters(lut0->texureID(), uvcenter, viewsize, rad2Pixel, myValues, myPaletteR, myPaletteG, myPaletteB);
        context = &marbleUVProjectionRC;
        break;
      case RSIGState::PROJECTION_MERCATOR:
        marbleMercatorProjectionRC.bind();
        marbleMercatorProjectionRC.setParameters(lut0->texureID(), uvcenter, viewsize, rad2Pixel, myValues, myPaletteR, myPaletteG, myPaletteB);
        context = &marbleMercatorProjectionRC;
        break;
      case RSIGState::PROJECTION_SPHERICAL:
        {
          const Marble::Quaternion qaxis = viewport->planetAxis().inverse();
          // UGLY Truncation from double to float a possible problem?
          const GLfloat axis[4] = {
            (float) qaxis.v[0],
            (float) qaxis.v[1],
            (float) qaxis.v[2],
            (float) qaxis.v[3]};
          marbleSphericalProjectionRC.bind();
          marbleSphericalProjectionRC.setParameters(lut0->texureID(), axis, viewsize, viewport->radius(), myValues, myPaletteR, myPaletteG, myPaletteB);
          context = &marbleSphericalProjectionRC;
        }
        break;
      case RSIGState::PROJECTION_LAMBERTCONFORMALCONIC:
        {
          lambertConformalProjectionRC.bind();
          const Marble::LambertConformalConicProjection *proj = dynamic_cast<const Marble::LambertConformalConicProjection *>(viewport->currentProjection());
          
          if (proj) {
            const double phi1 = proj->standardParallel1(); // hard coded in Marble library LambertConformalConicProjection.cpp
            const double phi2 = proj->standardParallel2(); // hard coded in Marble library LambertConformalConicProjection.cpp
            const GLfloat MYuvcenter[2]={centerLon, centerLat};
            const GLfloat MYrad2Pixel = rad2Pixel;
            lambertConformalProjectionRC.setParameters(lut0->texureID(), phi1, phi2, MYuvcenter, MYrad2Pixel, viewsize, myValues, myPaletteR, myPaletteG, myPaletteB);
          }
          
          context = &lambertConformalProjectionRC;
        }
        break;
      case RSIGState::PROJECTION_STEREOGRAPHIC:
        {
          stereographicProjectionRC.bind();
          // MJF - HACK force CMAQ EQUATES projection parameters
          // There are corresponding HACKS in libs/marble/src/lib/Projections/StereographicProjection.cpp
          const GLfloat MYuvcenter[2]={-98.0 * M_PI/180.0, 90.0 * M_PI/180.0};
          stereographicProjectionRC.setParameters(lut0->texureID(), MYuvcenter, rad2Pixel, viewsize, myValues, myPaletteR, myPaletteG, myPaletteB);
          context = &stereographicProjectionRC;
        }
        break;
      default:
        break;
      } //end switch(m_projectionMode)
    } else { // Non-marble:
      GLfloat numberOfTiles;
      GLfloat mytilesize;
      GLfloat midp[2];
      GLfloat screen_middle[2];
      m_mapControl->getWorldCoordinateToScreenParameters(screen_middle, midp, numberOfTiles, mytilesize);
      qmapMercatorProjectionRC.bind();
      qmapMercatorProjectionRC.setParameters(lut0->texureID(), screenrect.height(), numberOfTiles, mytilesize, midp, screen_middle, myValues, myPaletteR, myPaletteG, myPaletteB);
      context = &qmapMercatorProjectionRC;
    }
    
    //==========================================
    
    glDisable(GL_LIGHTING);
    CHECK_GL_ERRORS2;
    
    VisDataListSingleton::getInstance()->renderGeometryObjects( painter,
                                                                screenrect,
                                                                context );
    
    CHECK_GL_ERRORS2;
    
    if ( context ) {
      context->setIsPointUniformParameter( 0 ); // 0 = Avoid texture2D() in fragmentShader.
      setPointSpriteTexture(0, context);
    }
    
    this->drawMapOverlay();
    updateProbe();
    
    //==========================================
    
    if (m_useMarbleWidget) {
      
      switch(m_projectionMode) {
      case RSIGState::PROJECTION_MERCATOR:
        marbleMercatorProjectionRC.unbind();
        break;
      case RSIGState::PROJECTION_SPHERICAL:
        marbleSphericalProjectionRC.unbind();
        break;
      case RSIGState::PROJECTION_LAMBERTCONFORMALCONIC:
        lambertConformalProjectionRC.unbind();
        break;
      case RSIGState::PROJECTION_STEREOGRAPHIC:
        stereographicProjectionRC.unbind();
        break;
      default:
        marbleUVProjectionRC.unbind();
        break;
      };
    } else {
      qmapMercatorProjectionRC.unbind();
    }
    
    CHECK_GL_ERRORS2;
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    // make sure bounding box is on top of the data layers
    drawRegionOfInterest(painter);
    
    painter->endNativePainting();
    
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_MULTISAMPLE);
    CHECK_GL_ERRORS2;
    
    if ( context ) { // 2D view mode.
      
      // Render 2D CALIPSO curtain (not using shaders):
      
      painter->beginNativePainting();
      VisDataListSingleton::getInstance()->render2DCalipsoCurtain(painter, screenrect);
      painter->endNativePainting();
      
      // Render Neubrew popup (not using shaders):
      
      painter->beginNativePainting();
      VisDataListSingleton::getInstance()->renderPopupNeubrew(painter, screenrect);
      VisDataListSingleton::getInstance()->renderPopupCeilometer(painter, screenrect);
      painter->endNativePainting();
    }
  }
  
}

/*!
 \fn void RSIGScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event )

 Event processing. 
 */
void RSIGScene::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event ) {
  QGraphicsScene::mouseDoubleClickEvent(event);

  if (event->isAccepted()) {
    return;
  }

  const bool hasButton = (event->buttons() & Qt::LeftButton) != 0;

  if (hasButton) {
    MainWin& mainWin = MainWin::getInstance();
    ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
    const RSIGState *gState = RSIGStateSingleton::getInstance();
    
    const QPointF currentGeoPos = this->toGeoPointF(event->scenePos());
    //this->centerAt(currentGeoPos);

    // generate Google Map URL for Luke
    //qDebug() << currentGeoPos.x() << currentGeoPos.y();
    QString myURL = "https://www.google.com/maps/@" + QString::number(currentGeoPos.y()) + "," + QString::number(currentGeoPos.x()) + ",500m";

    if (gState->mShowGooglemapUrl) {
      controlDockWidget->postMessage(myURL, RSIGState::NORMAL_TEXT);
    }
    if (gState->mOpenGooglemapUrl) {
      QDesktopServices::openUrl(myURL);
    }
    
  }
}



/*!
 \fn void RSIGScene::mousePressEvent(QGraphicsSceneMouseEvent *event)

 Event processing. 
 */
void RSIGScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

  mouseDown = true;

  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);

  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  //bool mode2d = vOptions->getMode2D();
  int displayMode = vOptions->getDisplayMode();
  const bool hasRightButton   = (event->buttons() & Qt::RightButton) != 0;

  //if (mode2d) {
  if (displayMode == 0) { // 2D
    xLast = (float)event->scenePos().x();
    yLast = (float)event->scenePos().y();
    QGraphicsScene::mousePressEvent(event);

    if (event->isAccepted()) {
      return;
    }

    m_dragRoiMode = true;
    m_startScreenPos = m_prevScreenPos = event->scenePos();
    m_startGeoPos = m_prevGeoPos = this->toGeoPointF(event->scenePos());

    if (event->buttons() == Qt::MiddleButton) {

      if (vOptions->getCurProjection() != RSIGState::PROJECTION_LAMBERTCONFORMALCONIC
          && vOptions->getCurProjection() != RSIGState::PROJECTION_STEREOGRAPHIC) {

        if (m_roiRect.contains(m_startGeoPos)) {
          //this->centerAt(m_roiRect);
        } else {
          const Qt::KeyboardModifiers keymodifiers = event->modifiers();
          const bool hasModifier = (keymodifiers & Qt::ControlModifier) != 0;

          if (hasModifier) {
            m_roiRect = QRectF(m_startGeoPos, m_startGeoPos);

            if (m_boundingBox) {
              m_boundingBox->onBoundingGeoBoxChanged(m_roiRect);
            }

            Q_EMIT boundingGeoBoxChanged(m_roiRect);
          }
        }
      }
    }

    event->accept();
  } else if (displayMode == 1) { // 3D mode
    xLast = (float)event->scenePos().x();
    yLast = (float)event->scenePos().y();

    if (hasRightButton) {
      // set rotation point
      drawRotPoint=1;

      GLdouble rLatNear, rLatFar;
      GLdouble rLonNear, rLonFar;
      GLdouble rZNear, rZFar;
      GLint viewport[4];
      GLdouble modelview[16];
      GLdouble projection[16];

      glGetIntegerv(GL_VIEWPORT, viewport);   
      glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
      glGetDoublev(GL_PROJECTION_MATRIX, projection);

      // unproject at normalized near and far points to find possible ray
      const int result1 = gluUnProject(xLast, sceneRect().height()-yLast, 0.0, modelview, projection, viewport, &rLonNear, &rLatNear, &rZNear);
      const int result2 = gluUnProject(xLast, sceneRect().height()-yLast, 1.0, modelview, projection, viewport, &rLonFar,  &rLatFar,  &rZFar);
      //printf("%d %d\n", result1, result2);

      // determine where ray intersects map plane
      if (result1 && result2) {
        float dist_percent = (rZNear - 0) / fabs(rZFar - rZNear);
        rotCenterLat = rLatNear + dist_percent*(rLatFar - rLatNear);
        rotCenterLon = rLonNear + dist_percent*(rLonFar - rLonNear);
        rotCenterZ   = 0;
        
        // make sure selected point is not outside the map
        if (rotCenterLon < -180.0) { rotCenterLon = -180.0; }
        if (rotCenterLon >  180.0) { rotCenterLon =  180.0; }
        if (rotCenterLat <  -90.0) { rotCenterLat =  -90.0; }
        if (rotCenterLat >   90.0) { rotCenterLat =   90.0; }
      }
    }
  }
}



/*!
 \fn void RSIGScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)

 Event processing. 
 */
void RSIGScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  //qDebug() << "mouseMoveEvent";

  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  //bool mode2d = vOptions->getMode2D();
  int displayMode = vOptions->getDisplayMode();
  
  const Qt::KeyboardModifiers keymodifiers = event->modifiers();
  const bool hasCtrlModifier  = (keymodifiers & Qt::ControlModifier) != 0;
  const bool hasAltModifier   = (keymodifiers & Qt::AltModifier) != 0;
  const bool hasShiftModifier = (keymodifiers & Qt::ShiftModifier) != 0;
  const bool hasRightButton   = (event->buttons() & Qt::RightButton) != 0;
  const bool hasLeftButton    = (event->buttons() & Qt::LeftButton) != 0;
  const bool hasMiddleButton  = (event->buttons() & Qt::MidButton) != 0;

  curScenePos = event->scenePos();

  // do not update the probe if we are rotataing, panning, or zooming.
  // otherwise, Windows performance is bad.

  RSIGState *gState = RSIGStateSingleton::getInstance();

  if (!hasLeftButton && !hasMiddleButton && !hasRightButton) {
    updateProbe();

    if (gState->mShowDataProbe) {
      update(); // for updating probeMsg2 while moving mouse
    }
  }

  // mouse processing for 2D mode

  //if (mode2d) {        
  if (displayMode == 0) { // 2D        
    //call parent event processing.
    QGraphicsScene::mouseMoveEvent(event);

    if (event->isAccepted()) {
      return;
    }

    // mouse location
    const QPointF currentScreenPos = event->scenePos();
    const QPointF currentGeoPos = this->toGeoPointF(currentScreenPos);

    //if (hasCtrlModifier && hasButton) {

    if (hasRightButton) {
      // Draw new bounding box
      m_dragRoiMode = true;

      if (m_startGeoPos.x() < currentGeoPos.x()) {
        m_roiRect = QRectF(m_startGeoPos, currentGeoPos);
      } else {
        m_roiRect = QRectF(currentGeoPos, m_startGeoPos);
      }
      
      if (m_boundingBox) {
        m_boundingBox->onBoundingGeoBoxChanged(m_roiRect);
      }

      Q_EMIT boundingGeoBoxChanged(m_roiRect);
      event->accept();
      update();
    } else if (hasAltModifier && hasLeftButton) {
      // Zoom the map to region
      m_dragCRectMode = true;
      //m_crectRect = QRectF(m_startGeoPos, currentGeoPos);
      m_crectRect = enforceMapLimits( QRectF(m_startGeoPos, currentGeoPos) );
      event->accept();
      update();
    } else if (hasLeftButton && !hasShiftModifier) {
      // Pan the map (unless lambert or stereographic projection is selected)

      if (1) {
        const QPointF screenoffset = m_prevScreenPos - currentScreenPos;
        const QPointF geodxy =0.5*(currentGeoPos - this->toGeoPointF(QPointF(currentScreenPos.x()+2, currentScreenPos.y()+2)));
        const qreal x = screenoffset.x()*geodxy.x();
        const qreal y = screenoffset.y()*geodxy.y();
        static unsigned int counter=0;
        counter++;

        if ((counter%2)==1) {
          this->centerByOffset(x, y, screenoffset.x(), screenoffset.y());
          m_prevGeoPos = currentGeoPos;
          m_prevScreenPos = currentScreenPos;
        }

        update();
      }
    } else if (hasLeftButton && hasShiftModifier) {
          
    } else if (hasMiddleButton) { // zoom the map
      int y = event->scenePos().y();

      if (m_useMarbleWidget) {

        if ( (y-yLast) > 0 ) {
          zoomIn();
          yLast = y;
        } else if ( (y-yLast) < 0 ) {
          zoomOut();
          yLast = y;
        }
      } else { // for Google

        if (abs(y-yLast) > 50) { // lowers the mouse sensitivity, since Google only has discrete zoom levels

          if ( (y-yLast) > 0 ) {
            zoomIn();
            yLast = y;
          } else if ( (y-yLast) < 0 ) {
            zoomOut();
            yLast = y;
          }
        }
      }
    }
  } else if (displayMode == 1) { // mouse processing for 3D mode

    if (hasLeftButton || hasMiddleButton || hasRightButton) {
      int x = event->scenePos().x();
      int y = event->scenePos().y();
      
      if (hasLeftButton) {
        //printf("panning\n");
        float sensitivity = fov_angle/80.0;
        float panX = -(x-xLast)*sensitivity;
        float panY =  (y-yLast)*sensitivity;
        //printf("sens = %f\n", sensitivity);
        eyeX = eyeX + panX;
        eyeY = eyeY + panY;
        centerX = centerX + panX;
        centerY = centerY + panY;
      } else if (hasRightButton) { // right button - rotate
        zRotateFlag = 0;

        if (hasCtrlModifier) {
          //printf("rotating Z\n");
          rotAngleZ = (x-xLast)/10.0;
          zRotateFlag = 1;
          
        } else {
          //printf("%d %d %d %d\n", x, y, xLast, yLast);
          rotAngleY = (x-xLast)/10.0;
          rotAngleX = (y-yLast)/10.0;
        }
      } else { // middle button - zoom
        float step = (y-yLast)/200.0;
        float scale_fact = 1.0 + step;
        fov_angle = fov_angle / scale_fact;

        // limit the amount we can zoom in and out

        if (fov_angle <  0.01) {
          fov_angle =  0.01;
        }

        if (fov_angle >  60.0) {
          fov_angle =  80.0;
        }
      }

      if (isSphericalProj()) {
        updateSceneSphere3();
      } else {
        updateScene();
      }
      
      xLast = x;
      yLast = y;

      //qDebug() << "mouseMoveEvent calling update";
      update();
    }
  }
}



/*!
 \fn void RSIGScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)

 Event processing. 
 */
void RSIGScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  mouseDown = false;

  // store the modelview matrix 

  drawRotPoint=0;

  if (isSphericalProj()) {
    updateSceneSphere3();
  } else {
    updateScene();
  }
  
  rotCenterLon_last = rotCenterLon;
  rotCenterLat_last = rotCenterLat;

  // make sure navigator does not exceed view window bounds

  const Marble::ViewportParams * viewport = m_marbleWidget->viewport();
  const int w = viewport->width();
  const int h = viewport->height();

  if (m_marbleNavigator!= NULL && m_navigatorProxy!=NULL) {
    int x0 = m_navigatorProxy->x();
    int y0 = m_navigatorProxy->y();

    if (x0<0) {
      x0=0;
    }

    if (x0 >= w-m_marbleNavigator->width()) {
      x0 = w-m_marbleNavigator->width();
    }

    if (y0<0) {
      y0=0;
    }

    if (y0 >= h-m_marbleNavigator->height()) {
      y0 = h-m_marbleNavigator->height();
    }

    m_navigatorProxy->setPos(x0, y0);
  }
  
  QGraphicsScene::mouseReleaseEvent(event);

  if (!event->isAccepted()) {
    m_dragRoiMode= false;

    if (m_dragCRectMode) {
      this->centerAt(m_crectRect);
      m_dragCRectMode= false;                                
    }

    event->accept();
    update();        
  }
}



QRectF RSIGScene::enforceMapLimits(QRectF inRect) {
  //printf("in enforceMapLimits\n");

  QRectF returnRect;
  //qreal minlon = inRect.right();
  //qDebug()<< minlon;
  
  returnRect = inRect;
  return returnRect;
}



/*!
 \fn void RSIGScene::wheelEvent(QGraphicsSceneWheelEvent *event)

 Event processing. 
 */
void RSIGScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
  QGraphicsScene::wheelEvent(event);

  if (!event->isAccepted()) {
    event->accept();
  }

  update();
}



/*!
 \fn void RSIGScene::resizeEvent(QResizeEvent *event)
 
 Event processing. 
 */
void RSIGScene::resizeEvent(QResizeEvent *event)  {
  const QSize wsize = event->size();
  const int w = wsize.width();
  const int h = wsize.height();

  const int OFFSET = -20000;

  if (m_marbleWidget!=NULL) {
    m_marbleWidget->resize(w, h);
    m_marbleProxy->resize(w, h);

    if (m_useMarbleWidget) {
      m_marbleWidget->move(0, 0);
    } else {
      m_marbleWidget->move(OFFSET, OFFSET);
    }
  }

  if (m_mapControl!=NULL) {
    m_mapControl->resize(QSize(w, h));
    m_mapProxy->resize(w, h);

    if (m_useMarbleWidget) {
      m_mapControl->move(OFFSET, OFFSET);
    } else {
      m_mapControl->move(0, 0);
    }
  }

  if (m_boundingBox!=NULL) {
    m_boundingBox->move(w-186, h-70);
  }

  if (m_marbleNavigator!= NULL && m_navigatorProxy!=NULL) {
    int x0 = m_navigatorProxy->x();
    int y0 = m_navigatorProxy->y();

    if (x0<0) {
      x0=0;
    }

    if (x0 >= w-m_marbleNavigator->width()) {
      x0 = w-m_marbleNavigator->width();
    }

    if (y0<0) {
      y0=0;
    }

    if (y0 >= h-m_marbleNavigator->height()) {
      y0 = h-m_marbleNavigator->height();
    }

    m_navigatorProxy->setPos(x0, y0);
  }

  OpenGLScene::resizeEvent(event);
}



/*!
 \fn void RSIGScene::moveLeft()

 Move the map to the left. 
 */
void RSIGScene::moveLeft() {

  if (m_useMarbleWidget) {
    m_marbleWidget->moveLeft();
  } else {
    m_mapControl->scrollLeft(10);
  }
}



/*!
 \fn void RSIGScene::moveRight()

 Move the map to the right. 
 */
void RSIGScene::moveRight() {

  if (m_useMarbleWidget) {
    m_marbleWidget->moveRight();
  } else {
    m_mapControl->scrollRight(10);
  }
}



/*!
 \fn void RSIGScene::moveUp()

 Move the map up.
 */
void RSIGScene::moveUp() {

  if (m_useMarbleWidget) {
    m_marbleWidget->moveUp();
  } else {
    m_mapControl->scrollUp(10);
  }
}



/*!
 \fn void RSIGScene::moveDown()

 Move the map down.
 */
void RSIGScene::moveDown() {

  if (m_useMarbleWidget) {
    m_marbleWidget->moveDown();
  } else {
    m_mapControl->scrollDown(10);
  }
}



/*!
 \fn void RSIGScene::zoomIn()

 Zoom the map in one level.
 */
void RSIGScene::zoomIn() {
  int zoomval;

  if (m_useMarbleWidget) {
    m_marbleWidget->zoomIn();
    zoomval = m_marbleWidget->zoom();
  } else {
    m_mapControl->zoomIn();
    zoomval = m_mapControl->currentZoom();
    zoomval = this->qmapZoomValueToMarble(zoomval);
  }

  m_marbleNavigator->changeZoom(zoomval);
}



/*!
 \fn void RSIGScene::zoomOut()

 Zoom the map out one level.
 */
void RSIGScene::zoomOut() {
  int zoomval;

  if (m_useMarbleWidget) {
    m_marbleWidget->zoomOut();
    zoomval = m_marbleWidget->zoom();
  } else {
    m_mapControl->zoomOut();
    zoomval = m_mapControl->currentZoom();
    zoomval = this->qmapZoomValueToMarble(zoomval);
  }

  m_marbleNavigator->changeZoom(zoomval);
}



/*!
 \fn void RSIGScene::zoomChanged(int zoom)

 Zoom to the specified \e zoom level.
 */
void RSIGScene::zoomChanged(int zoom) {

  if (m_marbleWidget != NULL) {
    m_marbleWidget->zoomView(zoom);
  }
        
  if (m_mapControl != NULL) {
    const int x = marbleZoomValueToQMap(zoom);
    m_mapControl->setZoom(x);
  }
}



/*!
 \fn void RSIGScene::centerAt(const QRectF& r)

 Center the map based on a geographical region <lon, lat> rectangle. 
 */
void RSIGScene::centerAt(const QRectF& r) {
  const QPointF lowleft(r.x(), r.y());
  const QPointF upright(r.x()+r.width(), r.y()+r.height());
  int zoomval = 975;

  if (m_marbleWidget != NULL) {
    const Marble::GeoDataLatLonBox box(upright.y(), lowleft.y(), upright.x(), lowleft.x(), Marble::GeoDataCoordinates::Degree);
    m_marbleWidget->centerOn(box, false);
    zoomval = m_marbleWidget->zoom();
    m_marbleWidget->zoomView(zoomval);
  }

  if (m_mapControl != NULL) {
    const QPointF cc = 0.5*(lowleft+upright);
    m_mapControl->setView(cc);

    QList<QPointF> ptlist;
    ptlist.push_back(lowleft);
    ptlist.push_back(upright);
    m_mapControl->setViewAndZoomIn(ptlist);
  }

  if (m_marbleNavigator != NULL) {
    m_marbleNavigator->blockSignals(true);
    m_marbleNavigator->changeZoom(zoomval);
    m_marbleNavigator->blockSignals(false);
  }
}



/*!
 \fn void RSIGScene::centerAt(const QPointF& centerp)

 Center the map at geographical location \e {centerp}.
 */
void RSIGScene::centerAt(const QPointF& centerp) {

  if (m_marbleWidget != NULL) {
    m_marbleWidget->centerOn(centerp.x(), centerp.y());
  }

  if (m_mapControl != NULL) {
    m_mapControl->moveTo(centerp);
  }
}



/*!
 \fn void RSIGScene::centerByOffset(qreal dx, qreal dy, qreal sdx, qreal sdy)

 Scroll the map by geographical offset <dx, dy>, when using MarbleWidget, 
 or by screen space offset <sdx, sdy>, if using QMapControl.
 */
void RSIGScene::centerByOffset(qreal dx, qreal dy, qreal sdx, qreal sdy) {

  if (m_marbleWidget != NULL ) {
    const double K = -1.0;
    m_marbleWidget->rotateBy(dx*K, -dy*K, Marble::Automatic);
  }

  if (m_mapControl != NULL) {
    const double K = 1.0;
    m_mapControl->scroll(QPoint(sdx, sdy)*K);
  }
}



/*!
 \fn void RSIGScene::goHome()

 Center to the region of US.
 */
void RSIGScene::goHome() {
  m_homeRect = QRectF(QPointF(-126, 24), QPointF(-66, 50));
  centerAt(m_homeRect);
        
  if (m_marbleWidget != NULL && m_mapControl != NULL ) {
    const int zoomval = m_marbleWidget->zoom();
    const int z2 = marbleZoomValueToQMap(zoomval)+1;
    m_mapControl->setZoom(z2);

    if (!m_useMarbleWidget) {
      update();
    }
  }
}



void RSIGScene::goAutozoom() {
  centerAt(m_roiRect);

  if (m_marbleWidget != NULL && m_mapControl != NULL ) {
    const int zoomval = m_marbleWidget->zoom();
    const int z2 = marbleZoomValueToQMap(zoomval)+1;
    m_mapControl->setZoom(z2);

    if (!m_useMarbleWidget) {
      update();
    }                
  }
}



void RSIGScene::goLambert() {
  m_homeRect = QRectF(QPointF(-117, 20), QPointF(-77, 60)); // must be symmetric about Lambert center: (-97, 40)         
  centerAt(m_homeRect);
}



void RSIGScene::goStereographic() {
  centerAt(QPoint(-98.0, 90.0));
}



/*!
 \fn void RSIGScene::setBackgroundColor()
 
 Set the background color.
 */
void RSIGScene::setBackgroundColor() {
  const QColor color = QColorDialog::getColor(m_backgroundColor);

  if (color.isValid()) {
      m_backgroundColor = color;
      update();
  }
}



/*!
 \fn void RSIGScene::setMapServer(const char *theme)
 
 Set the map server used in the system. 
 For QMapControl, the input string \e theme uses two characters to identify the 
 server: GM: google map, GS: google satlite map, OS: openstreet map, MQ: mapquest map.
 
 For MarbleWidget, it uses a string of internal map path, such as "earth/plain/plain.dgml" and 
 "earth/srtm/srtm.dgml". 
 */
void RSIGScene::setMapServer(const char *theme) {
 
  // get the current geo center
  const QPointF geocenter = this->screenGeoCenterF();

  if (theme==NULL || theme[0]==0) {
    return;
  }
  
  const int themeLen = strlen(theme);
  
  bool useMarbleWidget = true;              // default for marble maps

  if (themeLen==2) {
    useMarbleWidget = false; // for google maps
  }
  
  if (m_useMarbleWidget==useMarbleWidget) {

    if (m_useMarbleWidget) {

      if (m_marbleWidget) {
        m_marbleWidget->setMapThemeId(theme);
      }
    } else {
      m_googleMapLayer->setVisible(false);                        
      m_googleTerrainMapLayer->setVisible(false);                        
      m_googleSatMapLayer->setVisible(false);        
      m_googleSatHybridMapLayer->setVisible(false);        
      m_osmMapLayer->setVisible(false);
      m_mapquestMapLayer->setVisible(false);

      if (theme[0]=='G' && theme[1]=='M') {
        m_googleMapLayer->setVisible(true);
      } else if (theme[0]=='G' && theme[1]=='T') {
        m_googleTerrainMapLayer->setVisible(true);
      } else if (theme[0]=='G' && theme[1]=='S') {
        m_googleSatMapLayer->setVisible(true);
      } else if (theme[0]=='G' && theme[1]=='H') {
        m_googleSatHybridMapLayer->setVisible(true);
      } else if (theme[0]=='O' && theme[1]=='S') {
        m_osmMapLayer->setVisible(true);                        
      } else if (theme[0]=='M' && theme[1]=='Q') {
        m_mapquestMapLayer->setVisible(true);
      }
    }
  } else {

    if (m_mapControl) {
      m_googleMapLayer->setVisible(false);
      m_googleTerrainMapLayer->setVisible(false);
      m_googleSatMapLayer->setVisible(false);        
      m_googleSatHybridMapLayer->setVisible(false);                        
      m_osmMapLayer->setVisible(false);
      m_mapquestMapLayer->setVisible(false);
      
      const int OFFSET = -20000;
      
      m_useMarbleWidget = useMarbleWidget;

      if (m_useMarbleWidget) {

        if (m_marbleWidget) {
          m_marbleWidget->setMapThemeId(theme);
          m_marbleWidget->move(0,0);
        }
        m_mapControl->move(OFFSET, OFFSET);
      } else {
        m_marbleWidget->move(OFFSET, OFFSET);
        m_mapControl->move(0, 0);              
        
        if (theme[0]=='G' && theme[1]=='M') {
          m_googleMapLayer->setVisible(true);
        } else if (theme[0]=='G' && theme[1]=='T') {
          m_googleTerrainMapLayer->setVisible(true);
        } else if (theme[0]=='G' && theme[1]=='H') {
          m_googleSatHybridMapLayer->setVisible(true);
        } else if (theme[0]=='G' && theme[1]=='S') {
          m_googleSatMapLayer->setVisible(true);
        } else if (theme[0]=='O' && theme[1]=='S') {
          m_osmMapLayer->setVisible(true);                        
        } else if (theme[0]=='M' && theme[1]=='Q') {
          m_mapquestMapLayer->setVisible(true);
        }
      }
    }
  }
  
  

  // Reorient the map to the original geocenter and zoom level
  // to ensure consistency between Marble map and Google map,
  // but only if we switched from Marble to Google or vice-versa.

  if ( (int) strlen(lastMapTheme) != themeLen) {
    centerAt(geocenter);

    if (m_marbleWidget != NULL && m_mapControl != NULL ) {
      int zoomval = m_marbleWidget->zoom();
      int z2 = marbleZoomValueToQMap(zoomval)+1;
      m_mapControl->setZoom(z2);

      //if (!m_useMarbleWidget) {
      //  update();
      //}                
    }

    lastMapTheme = theme;
  }

  update();
}



/*!
 \fn void RSIGScene::setMapProjection(int mode)
 
 Set the projection type of the map, \e mode can be find in RSIGState::MapProjectionType. 
 For MarbleWidget, it has all the map projection types supported. 
 For QMapControl, it only support the Mercator projection type. 
 
 \sa RSIGState::MapProjectionType
 */
void RSIGScene::setMapProjection(int mode) {

  if (m_projectionMode!=mode) {
    m_projectionMode = mode;

    if (m_useMarbleWidget && m_marbleWidget) {

      switch (mode) {
      case RSIGState::PROJECTION_LONGLAT:
        m_marbleWidget->setProjection(1);
        break;
      case RSIGState::PROJECTION_SPHERICAL:
        m_marbleWidget->setProjection(0);
        break;
      default:
        m_marbleWidget->setProjection(mode);
        break;
      }
    }
  }
}



/*!
 \fn QRectF RSIGScene::toScreenRectF(const QRectF& geoRectF) const

 Convert a geographical location \e {geopoint <lon, lat>} into the screen position (<x, y> pair). 
 */
QRectF RSIGScene::toScreenRectF(const QRectF& geoRectF) const {
  const QRectF &r = geoRectF;
  const QPointF upright(r.x()+r.width(), r.y()+r.height());

  QRectF srect;
  QPointF x0, y0;

  if (m_useMarbleWidget) {
    qreal sx, sy;
    m_marbleWidget->screenCoordinates(r.x(), r.y(), sx, sy);
    x0 = QPointF(sx, sy);
    m_marbleWidget->screenCoordinates(upright.x(), upright.y(), sx, sy);
    y0 = QPointF(sx, sy);
    srect = QRectF(x0, y0);
  } else {
    const QPointF lowleft(r.x(), r.y());
    x0 = m_mapControl->worldCoordinateToScreen(lowleft);
    y0 = m_mapControl->worldCoordinateToScreen(upright);
  }
        
  srect = QRectF(x0, y0);
  return srect;
}



/*!
 \fn QPointF RSIGScene::toScreenPointF(const QPointF& geopoint) const
 
 Convert a geographical location \e {geopoint <lon, lat>} into the screen position (<x, y> pair). 
 */
QPointF RSIGScene::toScreenPointF(const QPointF& geopoint) const {
  QPointF x0;

  if (m_useMarbleWidget) {
    const qreal lon = geopoint.x();
    const qreal lat = geopoint.y();
    qreal sx, sy;
    m_marbleWidget->screenCoordinates(lon, lat, sx, sy);
    x0 = QPointF(sx, sy);
  } else {
    x0 = m_mapControl->worldCoordinateToScreen(geopoint);
  }
        
  return x0;
}



/*!
 \fn QPointF RSIGScene::toGeoPointF(const QPointF& screenCoord) const

 Convert a screen point \e {screenCoord <x,y>} into the geographical location (<lon, lat> pair). 
 */
QPointF RSIGScene::toGeoPointF(const QPointF& screenCoord) const {
  QPointF geopnt(0, 0);

  if (m_useMarbleWidget) {
    qreal lon=0, lat=0;

    if (m_marbleWidget) {
      m_marbleWidget->geoCoordinates( screenCoord.x(), screenCoord.y(), lon, lat);
    }

    geopnt = QPointF(lon, lat);
  } else {
    QPoint p(screenCoord.x(), screenCoord.y());

    if (m_mapControl) {
      geopnt = m_mapControl->screenToWorldCoordinate(p);
    }
  }
        
  return geopnt;
}


/*!
 \fn void RSIGScene::setBoundingGeoBox(const QRectF& rectf)
 
 Set the input \e rectf as the Region-of-interest longitude-latitude box. 
 Also, the LonlatBoundingBoxWidget is changed.
 */
void RSIGScene::setBoundingGeoBox(const QRectF& rectf) {
  m_roiRect = rectf;

  if (m_boundingBox) {
    const bool f = m_boundingBox->blockSignals(true);
    m_boundingBox->onBoundingGeoBoxChanged(rectf);
    m_boundingBox->blockSignals(f);
  }

  update();
}



/*!
 \fn bool RSIGScene::isSphericalProj() const
 
 Whether the current projection type is RSIGState::PROJECTION_SPHERICAL or not.
 */
bool RSIGScene::isSphericalProj() const {
  return m_useMarbleWidget && (m_projectionMode==RSIGState::PROJECTION_SPHERICAL);
}



/*!
 \fn bool RSIGScene::isLambertConformalConicProj() const
 
 Whether the current projection type is RSIGState::PROJECTION_LAMBERTCONFORMALCONIC or not.
 */
bool RSIGScene::isLambertConformalConicProj() const {
  return m_useMarbleWidget && (m_projectionMode==RSIGState::PROJECTION_LAMBERTCONFORMALCONIC);
}



/*!
 \fn bool RSIGScene::isStereographicProj() const

 Whether the current projection type is RSIGState::PROJECTION_STEREOGRAPHIC or not.
 */
bool RSIGScene::isStereographicProj() const {
  return m_useMarbleWidget && (m_projectionMode==RSIGState::PROJECTION_STEREOGRAPHIC);
}



/*!
 \fn QPointF RSIGScene::screenGeoCenterF() const
 
 Get the scene's display center.
 The value is in degree unit.
 */
QPointF RSIGScene::screenGeoCenterF() const {
  QPointF c(0,0);

  if (m_useMarbleWidget) {

    if (m_marbleWidget) {
      const qreal x = m_marbleWidget->centerLongitude();
      const qreal y = m_marbleWidget->centerLatitude();
      c = QPointF(x, y);
    }
  } else {
    if (m_mapControl)
    c= m_mapControl->currentCoordinate();
  }
  return c;
}



/*!
 \fn void RSIGScene::setScreenGeoCenterF(const QPointF &center)
 
 Set the scene's display center to the specified value \e center.
 The input value is in degree unit.
 */
void RSIGScene::setScreenGeoCenterF(const QPointF &center) {
  const QPointF c = center;

  if (m_marbleWidget) {
    m_marbleWidget->centerOn(c.x(), c.y(), false);
  }

  if (m_mapControl) {
    m_mapControl->setView(c);
  }
}



/*!
 \fn QRectF RSIGScene::screenGeoRectF() const
 
 Return the scene's display rectangular area of longitude/latitude.
 The returned QRectF values are in degree unit.
 */
QRectF RSIGScene::screenGeoRectF() const {
  const qreal w0 = (qreal)(sceneRect().width());
  const qreal h0 = (qreal)(sceneRect().height());
  const QRect _screenregion(0, 0, w0, h0);
  QRect screenregion = _screenregion;

  if (isSphericalProj()) // || isLambertConformalConicProj() || isStereographicProj())
    screenregion = m_marbleWidget->mapRegion().boundingRect();

  const QPointF upleft(screenregion.x(), screenregion.y());
  QPointF geoupleft = this->toGeoPointF(upleft);
  const QPointF lowright(screenregion.x()+screenregion.width(), screenregion.y()+screenregion.height());
  QPointF geolowright = this->toGeoPointF(lowright);

  if (m_useMarbleWidget) {
    const qreal centerLon = m_marbleWidget->viewport()->centerLongitude();

    if (geolowright.x() < geoupleft.x()) {

      if (centerLon * geoupleft.x() >=0) {
        geolowright.setX( geolowright.x() + 360.0);
      } else {
        geoupleft.setX( geoupleft.x() - 360.0);
      }
    }
  }

  const double wid = geolowright.x()- geoupleft.x();
  const double hgt = geoupleft.y() - geolowright.y();
  //QRectF r(geoupleft.x(), geolowright.y(), wid*1.15, hgt);
  QRectF r(geoupleft.x(), geolowright.y(), wid, hgt);
  return r;
}



/*!
 \fn void RSIGScene::setScreenGeoRectF(const QRectF &r)
 
 Set the scene's display area to fit the input rectangular region \e {rect}  as close as possible.
 */
void RSIGScene::setScreenGeoRectF(const QRectF &rect) {
  this->centerAt(rect);
  update();
}



/*!
 \fn QPoint RSIGScene::navigatorPosition() const
 
 Return the MarbleNavigator's position as a QPoint.
 */
QPoint RSIGScene::navigatorPosition() const {
  QPoint p( 0, 0 );

  if (m_navigatorProxy) {
    const int posx = m_navigatorProxy->x();
    const int posy = m_navigatorProxy->y();
    p = QPoint(posx, posy);
  }

  return p;
}



/*!
 \fn void RSIGScene::setNavigatorPosition(const QPoint& pos)
 
 Set the MarbleNavigator's position to \e pos.
 */
void RSIGScene::setNavigatorPosition(const QPoint& pos) {

  if ( m_navigatorProxy ) {
    m_navigatorProxy->setPos(pos.x(), pos.y());
  }
}



/*!
 \fn QDomElement RSIGScene::domElement(const QString& name, QDomDocument& document) const
 
 Store the state information into a QDomDocument. 
 The following information is stored:
 the lon-lat rectangular box, the center of the view, and the zoom factor.
 
 \sa RSIGScene::initFromDOMElement()
 */
QDomElement RSIGScene::domElement(const QString& name, QDomDocument& document) const {
  QDomElement node = document.createElement(name);

  //const QRectF georect = this->screenGeoRectF();
  //node.appendChild(DomUtils::QRectFDomElement(georect, "lonlat", document));
  VisOptions *vOptions = VisOptionsSingleton::getInstance();
  const QPointF topLeft     = QPointF(vOptions->getBboxMinLon(), vOptions->getBboxMaxLat());
  const QPointF bottomRight = QPointF(vOptions->getBboxMaxLon(), vOptions->getBboxMinLat());
  const QRectF origGeoRect  = QRectF(topLeft, bottomRight);
  node.appendChild(DomUtils::QRectFDomElement(origGeoRect, "lonlat", document));

  const QPointF geocenter = this->screenGeoCenterF();
  node.appendChild(DomUtils::QPointFDomElement(geocenter, "center", document));

  {//add zoom element
    const int z0 = m_marbleWidget->zoom();
    const int z1 = m_mapControl->currentZoom();
    const QPointF zoomval(z0, z1);
    node.appendChild(DomUtils::QPointFDomElement(zoomval, "zoom", document));
  }

  {//add navigatorpos element
    const QPoint navpos = this->navigatorPosition();
    const QDomElement anode = DomUtils::QPointFDomElement(QPointF(navpos.x(), navpos.y()), "navigatorpos", document);
    node.appendChild(anode);
  }

  node.appendChild(DomUtils::floatDomElement(fov_angle, "fov_angle", document));

  return node;
}



/*!
 \fn void RSIGScene::initFromDOMElement(const QDomElement& element)

 Use the input QDomElement \e {element} value to initialize the scene's state. 
 
 \sa RSIGScene::domElement()
 */
void RSIGScene::initFromDOMElement(const QDomElement& element) {

  for ( QDomElement child = element.firstChild().toElement();
        ! child.isNull();
        child = child.nextSibling().toElement() ) {
    const QString tagName( child.tagName() );

    if ( tagName == "lonlat" ) {
      const QRectF r = DomUtils::QRectFFromDom(child);
      this->setScreenGeoRectF(r);
    } else if ( tagName == "center" ) {
      const QPointF r = DomUtils::QPointFFromDom(child);
      this->setScreenGeoCenterF(r);
    } else if ( tagName == "zoom" ) {
      const QPointF r = DomUtils::QPointFFromDom(child);
      bool f;
      f = m_marbleNavigator->blockSignals(true);
      m_marbleNavigator->changeZoom(r.x());
      m_marbleNavigator->blockSignals(f);

      f = m_marbleWidget->blockSignals(true);
      m_marbleWidget->zoomView(r.x());
      m_marbleWidget->blockSignals(f);

      f = m_mapControl->blockSignals(true);
      m_mapControl->setZoom(r.y());
      m_mapControl->blockSignals(f);
    } else if ( tagName == "navigatorpos" ) {
      const QPointF pos = DomUtils::QPointFFromDom(child);
      this->setNavigatorPosition(QPoint(pos.x(), pos.y()));
    } else if ( tagName == "fov_angle" ) {
      fov_angle = DomUtils::floatFromDom(child, "fov_angle", 5.0);
    }
  }
}


