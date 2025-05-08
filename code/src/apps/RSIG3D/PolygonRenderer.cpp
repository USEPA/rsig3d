/******************************************************************************
PURPOSE: PolygonRenderer.cpp - Implements a polygon dataset rendering class.

NOTES: for rendering point data sets using OpenGL APIs

HISTORY: 2013-05-44 zhang.nan@epa.gov
         Evolved by freeman.matt@epa.gov and plessel.todd@epa.gov
******************************************************************************/

#include <assert.h>
#include <math.h> // For sqrt().

#include <vector>
#include <algorithm> // For std::min(), std::max().

#include <GL/glew.h>

#include <QObject>
#include <QDebug>
#include <QPainter>
#include <QRectF>

#include <Dataset.h>

#include "vector4f.h"
#include "vector4i.h"
#include "RenderingContextRSIG.h"
#include "GeoDataObject.h"
#include "RSIGGlobal.h"
#include "MainWin.h"
#include "PointRenderer.h"
#include "PolygonRenderer.h"

#include "MainWin.h"
#include "vector3f.h"
#include "VisDataObject.h"
#include "LookupTableWidget.h"
#include "VisOptions.h"
#include "ControlDockWidget.h"

#ifdef __APPLE__
#include <GLUT/glut.h> /* For glut*. */
#else
#include <GL/glut.h> /* For gl*. */
#endif


static LookupTableWidget* myLut = 0;
static QRectF curtainFrame;
static QRectF curtainPlotRect;
static int plotwindow_x0, plotwindow_y0;
static int origin_x0;
static int origin_y0;
static int xAxisLength;
static int yAxisLength;
static bool is2DCurtain = false; // default


typedef emvl::Dataset::SubsetPointers SubsetPointers;

Vector4i PolygonRenderer::mElements[PolygonRenderer::BUFFSIZE];

static int reduceVerticesToLowestLayer( const bool asQuads,
                                        const int vertexCount,
                                        Vector4f vertices[] );

//=====================================================================
/*!
 \class PolygonRenderer

 \brief An example class for polygon dataset rendering.
 */

/*!
 Constructor.
 */
PolygonRenderer::PolygonRenderer(QObject* parent) : VisDataObject(parent)
    ,mLineWidth(1.0f)
    ,mLineColor(0, 0, 0)
    ,mShowNullSample(false) {

  for (int i=0; i<BUFFSIZE; i++) {
    const int j = i << 2;
    mElements[i] = Vector4i(j, j+1, j+2, j+3);
  }
}



/*!
 Destructor.
 */
PolygonRenderer::~PolygonRenderer() {
}



bool PolygonRenderer::isLookupTableWidget() const {
  return false;
}



bool PolygonRenderer::isWidgetOfType(QString inquiry) const {
  GeoDataObject *par = qobject_cast<GeoDataObject*>(parent());

  if (par->varName().contains(inquiry, Qt::CaseInsensitive)) {
    return true;
  } else {
    return false;
  }
}



bool PolygonRenderer::is3DObject() const {
  return true;
}



static void postMessage(const QString &msg, int mode) {
  MainWin& mainWin = MainWin::getInstance();
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
  controlDockWidget->postMessage(msg, mode);
}



// Does the quadrilateral grid cell intersect the view bounds?

static bool
quadrilateralIntersectsViewBounds( const QRectF& viewBounds,
                                   const emvl::Point cellVertices[ 4 ] ) {
  bool result =
    viewBounds.contains( cellVertices[ 0 ].longitude,
                         cellVertices[ 0 ].latitude ); // Quicker point test.

  if ( ! result ) { // Do more expensive test:
    const double longitudeMinimum =
      std::min<double>( cellVertices[ 0 ].longitude,
                        cellVertices[ 3 ].longitude );
    const double longitudeMaximum =
      std::max<double>( cellVertices[ 1 ].longitude,
                        cellVertices[ 2 ].longitude );
    const double latitudeMinimum =
      std::min<double>( cellVertices[ 0 ].latitude,
                        cellVertices[ 1 ].latitude );
    const double latitudeMaximum =
      std::max<double>( cellVertices[ 2 ].latitude,
                        cellVertices[ 3 ].latitude );
    const double cellWidth = longitudeMaximum - longitudeMinimum;
    const double cellheight = latitudeMaximum - latitudeMinimum;
    const QRectF cellQuadrilateral( longitudeMinimum,
                                    latitudeMinimum,
                                    cellWidth, cellheight );
    result = viewBounds.intersects( cellQuadrilateral );
  }

  return result;
}



// 2D drawing mode:

static void renderBitmapString2( float x, float y, float z, void *font, char *string) {
  glRasterPos3f(x, y, z);

  for ( const char* c = string; *c != '\0'; c++ ) {
    glutBitmapCharacter(font, *c);
  }
}



// Clip cell if it crosses the -180/180 line.

static void clipEdgeCell( const size_t vertexCount,
                          emvl::Point vertices[] ) {
  bool crosses = false;
  double previousLongitude = vertices[ 0 ].longitude;

  for ( size_t index = 1; ! crosses && index < vertexCount; ++index ) {
    double longitude = vertices[ index ].longitude;
    crosses =
      ( longitude > previousLongitude && longitude - previousLongitude > 180.0)
      ||
      ( longitude < previousLongitude && previousLongitude - longitude > 180.0);
    previousLongitude = longitude;
  }

  if ( crosses ) {
    previousLongitude = vertices[ 0 ].longitude;

    for ( size_t index = 1; index < vertexCount; ++index ) {
      vertices[ index ].longitude = previousLongitude;
    }
  }
}



static void
copyAndDrawQuads(const QRectF& viewBounds,
                 const emvl::Dataset* const dataset,
                 const SubsetPointers& subsetPointers,
                 const int gridLayer, // [1, layers] or 0 if non-grid dataset.
                 const size_t offset,
                 const size_t count,
                 const float threshold,
                 const bool showThreshold,
                 const Vector4i* const elements,
                 const bool showNullSample,
                 QPainter* /* painter */,
                 const QRect& /* rect */,
                 RenderingContextRSIGBase* /* context */) {

  if (dataset->isStructuredGrid() && gridLayer == 0) { // HACK
    return;
  }
  
  assert( dataset );
  assert( IMPLIES_ELSE( dataset->isStructuredGrid(),
                        AND2( gridLayer >= 1,
                              (size_t) gridLayer <= dataset->layers() ),
                        gridLayer == 0 ) );
  assert( count ); assert( count <= PolygonRenderer::BUFFSIZE << 2 );
  assert( dataset->cellVertexCount() == 4 ||
          dataset->cellVertexCount() == 8 );

  const size_t cellVertexCount = dataset->cellVertexCount();
  const int BUFFSIZE = PolygonRenderer::BUFFSIZE;
  const int BUFFSIZE4 = BUFFSIZE << 2;
  Vector4f vertices[ BUFFSIZE4 ]; // Buffer copy of many quad vertices.
  assert( sizeof vertices / sizeof *vertices >= count * 4 );
  const size_t end = offset + count;
  const size_t coordinatesCount = subsetPointers.coordinatesCount;
  const size_t cellCount = coordinatesCount / cellVertexCount;
  const size_t layer   = gridLayer;
  const size_t layers  = dataset->layers();
  const size_t rows    = dataset->rows();
  const size_t columns = dataset->columns();
  const size_t rowsTimesColumns = rows * columns;
  const bool isGriddedData =
    AND3( dataset->isStructuredGrid(),
          rowsTimesColumns,
          IN_RANGE( layer, 1, layers ) );
  const emvl::Point* const coordinates = subsetPointers.coordinates;
  const double* const data = subsetPointers.data;
  size_t vertexCount = 0;
  emvl::Point computedCellVertices[ 8 ];
  const emvl::Point* cellVertices = &computedCellVertices[ 0 ];
  RSIGScene& rsigScene = RSIGScene::getInstance();
  const bool checkAndClipEdgeCells =
    ! rsigScene.isStereographicProj() && ! rsigScene.isSphericalProj();
  const bool isCalipso = dataset->name().find("calipso") == 0;
  const bool isCalipso2D = is2DCurtain && isCalipso;
  double plot_lonmin    = -180.0;
  double plot_lonmax    = 180.0;
  double plot_loncenter = 0.0;

  if (isCalipso2D) {
    // get lat/lon of axis edges
    QPointF geoLeft   = rsigScene.toGeoPointF(QPointF(origin_x0,             origin_y0+yAxisLength));
    QPointF geoRight  = rsigScene.toGeoPointF(QPointF(origin_x0+xAxisLength, origin_y0+yAxisLength));
    QPointF geoCenter = rsigScene.toGeoPointF(QPointF(rsigScene.sceneRect().width()/2, origin_y0+yAxisLength));

    plot_lonmin = geoLeft.x();
    plot_lonmax = geoRight.x();
    plot_loncenter = geoCenter.x();

    // correct wrapping issues

    if ( plot_lonmin > plot_lonmax ) {

      if ( plot_lonmin > 0 && plot_loncenter < 0 ) {
        plot_lonmin = -180;
      }
    }

    if ( plot_lonmax < 0 && plot_loncenter > 0 ) {
      plot_lonmax = 180.0;
    }

    // push the matrix to prepare for doing 2D overplot
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    // set up 2D rendering
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glOrtho(0.0f, rsigScene.sceneRect().width(), rsigScene.sceneRect().height(), 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  for ( size_t index = offset; index < end; ++index ) {
    const size_t cellIndex = index % cellCount;
    const size_t cellLayer = isGriddedData ? cellIndex / rowsTimesColumns : 0;
    const double value = data[ index ];
    const bool valueIsNonMissing = OR2( showNullSample, value > emvl::MISSING );
    const bool valueIsInThresholdRange =
      IMPLIES( showThreshold, value >= threshold );
    const bool isLayerCell = cellLayer + 1 == layer;
    const bool drawThisData =
      AND3( valueIsNonMissing, valueIsInThresholdRange,
            IMPLIES( isGriddedData, isLayerCell ) );

    if ( drawThisData ) {

      if ( coordinates ) {
        const size_t coordinatesIndex = cellIndex * cellVertexCount;
        cellVertices = coordinates + coordinatesIndex;
      } else {
        dataset->cellVertices( cellIndex, computedCellVertices );

        /*
         * If not stereographic view then clip cells that cross the -180/180
         * line to avoid drawing wide cells that stretch across the globe.
         * When viewing stereographic we must retain such cells to avoid a gap.
         */

        if ( checkAndClipEdgeCells ) {
          clipEdgeCell( cellVertexCount, computedCellVertices );
        }
      }

      if ( quadrilateralIntersectsViewBounds( viewBounds, cellVertices ) ) {
        const emvl::Point& v0 = cellVertices[ 0 ];
        const emvl::Point& v1 = cellVertices[ 1 ];
        const emvl::Point& v2 = cellVertices[ 2 ];
        const emvl::Point& v3 = cellVertices[ 3 ];

        // Regular vertices for polygons and/or calipso groundtrack

        if ( vertexCount + 4 <= sizeof vertices / sizeof *vertices ) {
          vertices[ vertexCount++ ] = Vector4f( v0.longitude, v0.latitude, v0.elevation, value );
          vertices[ vertexCount++ ] = Vector4f( v1.longitude, v1.latitude, v1.elevation, value );
          vertices[ vertexCount++ ] = Vector4f( v2.longitude, v2.latitude, v2.elevation, value );
          vertices[ vertexCount++ ] = Vector4f( v3.longitude, v3.latitude, v3.elevation, value );
        }

        // special processing for CALIPSO curtain

        if (isCalipso2D) {

          // draw calipso curtain using Qpainter
          // get vertex longitudes in screen space

          double v0_lon = 0.0, v1_lon = 0.0, v2_lon = 0.0, v3_lon = 0.0;
          double v0_elev = 0.0, v1_elev = 0.0, v2_elev = 0.0, v3_elev = 0.0;
          int vert1 = 0, vert2 = 0, vert3 = 0, vert4 = 0;

          for (int pass=0; pass<=1; pass++) {

            // pass=0 -> draw quad (non-regridded) or east face of hex cell (regridded)
            // pass=1 -> draw west face of hex cell (regridded)

            if (cellVertexCount == 4 && pass == 0) {
              // polygons (non-regrided)
              v0_lon = v0.longitude;
              v1_lon = v1.longitude;
              v2_lon = v2.longitude;
              v3_lon = v3.longitude;
              v0_elev = v0.elevation;
              v1_elev = v1.elevation;
              v2_elev = v2.elevation;
              v3_elev = v3.elevation;
            } else if (cellVertexCount == 4 && pass == 1) {
              // do nothing
            } else if (cellVertexCount == 8) {
              // hex cells (regrided) - just draw east and west vertical faces

               if (pass == 0) {
                vert1 = 1;
                vert2 = 2;
                vert3 = 6;
                vert4 = 5;
              } else {
                vert1 = 0;
                vert2 = 3;
                vert3 = 7;
                vert4 = 4;
              }
              v0_lon  = cellVertices[ vert1 ].longitude;
              v1_lon  = cellVertices[ vert2 ].longitude;
              v2_lon  = cellVertices[ vert3 ].longitude;
              v3_lon  = cellVertices[ vert4 ].longitude;
              v0_elev = cellVertices[ vert1 ].elevation;
              v1_elev = cellVertices[ vert2 ].elevation;
              v2_elev = cellVertices[ vert3 ].elevation;
              v3_elev = cellVertices[ vert4 ].elevation;
            } else {
              qDebug() << "WARNING: don't know how to draw 2D curtain!";
            }

            if ( (cellVertexCount == 4 && pass == 0) || (cellVertexCount == 8) ) {
              // longitude of data point mapped to screen space
              const double inverse_range = 1.0 / ( plot_lonmax - plot_lonmin );
              double screenLon0 = origin_x0 + xAxisLength*(v0_lon - plot_lonmin) * inverse_range;
              double screenLon1 = origin_x0 + xAxisLength*(v1_lon - plot_lonmin) * inverse_range;
              double screenLon2 = origin_x0 + xAxisLength*(v2_lon - plot_lonmin) * inverse_range;
              double screenLon3 = origin_x0 + xAxisLength*(v3_lon - plot_lonmin) * inverse_range;

              // elevation of data point mapped to screen space
              double screenElev0 = origin_y0 + (40.0-v0_elev/1000.0)*yAxisLength/40.0;
              double screenElev1 = origin_y0 + (40.0-v1_elev/1000.0)*yAxisLength/40.0;
              double screenElev2 = origin_y0 + (40.0-v2_elev/1000.0)*yAxisLength/40.0;
              double screenElev3 = origin_y0 + (40.0-v3_elev/1000.0)*yAxisLength/40.0;

              // rectangle in screen space of data point
              QPointF topLeft = QPointF(screenLon3, screenElev3);
              QPointF bottomRight = QPointF(screenLon1, screenElev1);
              QRectF thisFillRect = QRectF(topLeft, bottomRight);

              // draw the data point

              if (curtainPlotRect.contains(thisFillRect)) {
                assert( myLut );
                Vector4f thisColor = myLut->colorByValue(value);
                glColor3f(thisColor.x, thisColor.y, thisColor.z);
                glBegin(GL_QUADS);
                glVertex2f(screenLon0, screenElev0);
                glVertex2f(screenLon1, screenElev1);
                glVertex2f(screenLon2, screenElev2);
                glVertex2f(screenLon3, screenElev3);
                glEnd();
              }
            }
          }
        }
      }
    }
  } 

  // If this chunk of the dataset had coordinates & data to draw then
  // draw solid quads, vertex prop. value used, quad color is determined from LUT texture

  if ( vertexCount >= 4 && is2DCurtain == false ) {
    assert( IN_RANGE( vertexCount, 4, BUFFSIZE4 ) );
    assert( vertexCount % 4 == 0 ); // Ensure 4 vertices per quad.

    if ( isCalipso ) { // Draw ground track as fat line or (regridded?) quads:
      const bool renderAsQuads = dataset->cellVertexCount() == 8;
      vertexCount = reduceVerticesToLowestLayer( renderAsQuads,
                                                 vertexCount, vertices );
      glLineWidth(7.0);
      glVertexPointer( 4, GL_FLOAT, 0, &vertices[0].x );
      CHECK_GL_ERRORS2; // Report any errors before shader programs execute.
      glDrawArrays( renderAsQuads ? GL_QUADS : GL_LINE_STRIP, 0, vertexCount );
      glLineWidth(1.0);
      CHECK_GL_ERRORS2; // Report any errors in shader programs.
    } else { // Regular processing for all other polygons:
      glVertexPointer( 4, GL_FLOAT, 0, &vertices[0].x );
      CHECK_GL_ERRORS2; // Report any errors before shader programs execute.
      glDrawElements( GL_QUADS, vertexCount, GL_UNSIGNED_INT, &elements[0].x );
      CHECK_GL_ERRORS2; // Report any errors in shader programs.
    }
  }

  if (isCalipso2D) {
    // restore the original 3D matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
  }
}



/******************************************************************************
PURPOSE: reduceVerticesToLowestLayer - Reduce the CALIPSO ground track
         vertices to just the ground or lowest level to avoid drawing many
         points on top of each other in 2D view mode.

INPUTS:  const bool asQuads  Keep vertices as (horizontal) quads (regridded)
                             otherwise vertices will be line segments.
         const int vertexCount  Input vertex count (full curtain or grid cells).
         Vector4f vertices[ vertexCount ]  Vertices of curtain or regrid cells.
OUTPUTS: Vector4f vertices[ vertexCount ]  Reduced to ground/lowest level.
RETURNS: int reduced vertex count.
******************************************************************************/

static int reduceVerticesToLowestLayer( const bool asQuads,
                                        const int vertexCount,
                                        Vector4f vertices[] ) {

  assert( vertexCount >= 4  );
  assert( vertexCount % 4 == 0 ); // Ensure 4 vertices per quad.

  int reducedCount = 1;

  if ( asQuads ) {
    double longitude0 = vertices[ 0 ].x;
    double latitude0  = vertices[ 0 ].y;
    double longitude1 = vertices[ 1 ].x;
    double latitude1  = vertices[ 1 ].y;
    double longitude2 = vertices[ 2 ].x;
    double latitude2  = vertices[ 2 ].y;
    double longitude3 = vertices[ 3 ].x;
    double latitude3  = vertices[ 3 ].y;

    for ( int index = 4; index < vertexCount; index += 4 ) {
      const double thisQuadLongitude0 = vertices[ index ].x;
      const double thisQuadLatitude0  = vertices[ index ].y;
      const int index1 = index + 1;
      const double thisQuadLongitude1 = vertices[ index1 ].x;
      const double thisQuadLatitude1  = vertices[ index1 ].y;
      const int index2 = index + 2;
      const double thisQuadLongitude2 = vertices[ index2 ].x;
      const double thisQuadLatitude2  = vertices[ index2 ].y;
      const int index3 = index + 3;
      const double thisQuadLongitude3 = vertices[ index3 ].x;
      const double thisQuadLatitude3  = vertices[ index3 ].y;

      if ( thisQuadLongitude0 != longitude0 ||
           thisQuadLatitude0  != latitude0  ||
           thisQuadLongitude1 != longitude1 ||
           thisQuadLatitude1  != latitude1  ||
           thisQuadLongitude2 != longitude2 ||
           thisQuadLatitude2  != latitude2  ||
           thisQuadLongitude3 != longitude3 ||
           thisQuadLatitude3  != latitude3 ) {
        longitude0 = thisQuadLongitude0;
        latitude0  = thisQuadLatitude0;
        longitude1 = thisQuadLongitude1;
        latitude1  = thisQuadLatitude1;
        longitude2 = thisQuadLongitude2;
        latitude2  = thisQuadLatitude2;
        longitude3 = thisQuadLongitude3;
        latitude3  = thisQuadLatitude3;
        vertices[ reducedCount ].x = longitude0;
        vertices[ reducedCount ].y = latitude0;
        vertices[ reducedCount ].z = vertices[ index ].z;
        vertices[ reducedCount ].w = vertices[ index ].w;
        ++reducedCount;
        vertices[ reducedCount ].x = longitude1;
        vertices[ reducedCount ].y = latitude1;
        vertices[ reducedCount ].z = vertices[ index1 ].z;
        vertices[ reducedCount ].w = vertices[ index1 ].w;
        ++reducedCount;
        vertices[ reducedCount ].x = longitude2;
        vertices[ reducedCount ].y = latitude2;
        vertices[ reducedCount ].z = vertices[ index2 ].z;
        vertices[ reducedCount ].w = vertices[ index2 ].w;
        ++reducedCount;
        vertices[ reducedCount ].x = longitude3;
        vertices[ reducedCount ].y = latitude3;
        vertices[ reducedCount ].z = vertices[ index3 ].z;
        vertices[ reducedCount ].w = vertices[ index3 ].w;
      }
    }
  } else {
    double longitude0 = vertices[ 0 ].x;
    double latitude0  = vertices[ 0 ].y;

    for ( int index = 4; index < vertexCount; index += 4 ) {
      const double thisQuadLongitude = vertices[ index ].x;
      const double thisQuadLatitude  = vertices[ index ].y;

      if ( thisQuadLongitude != longitude0 ||
           thisQuadLatitude  != latitude0 ) {
        longitude0 = thisQuadLongitude;
        latitude0  = thisQuadLatitude;
        vertices[ reducedCount ].x = longitude0;
        vertices[ reducedCount ].y = latitude0;
        vertices[ reducedCount ].z = vertices[ index ].z;
        vertices[ reducedCount ].w = vertices[ index ].w;
        ++reducedCount;
      }
    }
  }

  assert( ( asQuads && vertexCount >= 4 && vertexCount % 4 == 0 ) ||
          ( ! asQuads && vertexCount >= 1 ) );

  return reducedCount;
}



static void
pagedBufferDraw(const QRectF& viewBounds,
                const emvl::Dataset* const dataset,
                const SubsetPointers& subsetPointers,
                const int gridLayer,
                const float threshold,
                const bool showThreshold,
                const Vector4i* elements,
                bool showNullSample,
                QPainter* painter,
                const QRect& rect,
                RenderingContextRSIGBase* context,
                std::vector<Vector4f>& tridata) {

  if ( dataset->cellType() == emvl::Dataset::POLYGONS ) {
    assert( subsetPointers.dataCount == 1 );

    // Draw polygons as a triangle strip colored by the single data value:

    const double value = subsetPointers.data[ 0 ];
    const bool valueIsNonMissing = OR2( showNullSample, value > emvl::MISSING );
    const bool valueIsInThresholdRange =
      IMPLIES( showThreshold, value >= threshold );
    const bool drawThisData = AND2( valueIsNonMissing, valueIsInThresholdRange);

    if ( drawThisData ) {
      const size_t vertexCount = subsetPointers.coordinatesCount;
      const emvl::Point* const coordinates = subsetPointers.coordinates;
      const double value = subsetPointers.data[ 0 ];

      if ( tridata.size() == 0 ) { // If empty (never allocated) then
        tridata.reserve( 256 ); // Allocate space for 256 triangles (once).
      }

      tridata.resize( vertexCount ); //x (No reallocation if under 256).

      for ( size_t vertex = 0; vertex < vertexCount; ++vertex ) {
        const emvl::Point* const point = coordinates + vertex;
        tridata[ vertex ] =
          Vector4f( point->longitude, point->latitude, point->elevation, value);
      }

      glDepthFunc( GL_ALWAYS ); // Ensure data drawn last (high values) appears.
      glVertexPointer( 4, GL_FLOAT, 0, &tridata[ 0 ] );
      CHECK_GL_ERRORS2; // Report any errors before shader programs execute.
      glDrawArrays( GL_TRIANGLE_STRIP, 0, vertexCount );
      CHECK_GL_ERRORS2; // Report any errors in shader programs.
    }

  } else { // Draw lots of colored quads using a buffer:
    const size_t BUFFSIZE = PolygonRenderer::BUFFSIZE;
    const size_t dataCount = subsetPointers.dataCount;
    const size_t chunks = dataCount / BUFFSIZE;
    size_t offset = 0;

    for ( size_t chunk = 0; chunk < chunks; ++chunk, offset += BUFFSIZE ) {
      copyAndDrawQuads(viewBounds, dataset, subsetPointers, gridLayer, offset,
                       BUFFSIZE, threshold, showThreshold, elements,
                       showNullSample, painter, rect, context);
    }

    if ( offset < dataCount ) {
      const size_t remainingCount = dataCount - offset;
      copyAndDrawQuads(viewBounds, dataset, subsetPointers, gridLayer, offset,
                       remainingCount, threshold, showThreshold, elements,
                       showNullSample, painter, rect, context);
    }
  }
}



// 3D drawing mode - draw colored quads, polygons or hexahedron cell faces:

static void render3d( const QRectF& viewBounds,
                      const emvl::Dataset* const dataset,
                      const SubsetPointers& subsetPointers,
                      const int gridRow, 
                      const int gridCol, 
                      const int gridLayer, 
                      const float threshold,
                      const bool showThreshold,
                      const bool showNullSample,
                      const bool showCellBoundary,
                      const float alpha ) {

  assert( dataset );
  assert( subsetPointers.data );
  assert( subsetPointers.coordinatesCount );
  assert( alpha >= 0.0 ); assert( alpha <= 1.0 );
  assert( dataset->cellVertexCount() == 1 || // polygons as triangle strip.
          dataset->cellVertexCount() == 4 ||
          dataset->cellVertexCount() == 8 );

  const size_t cellVertexCount = dataset->cellVertexCount();
  const size_t coordinatesCount = subsetPointers.coordinatesCount;
  const size_t cellCount = coordinatesCount / dataset->cellVertexCount();
  const size_t dataCount = subsetPointers.dataCount;
  const size_t layer  = gridLayer;
  const size_t row    = gridRow;
  const size_t column = gridCol;
  const size_t layers  = dataset->layers();
  const size_t rows    = dataset->rows();
  const size_t columns = dataset->columns();
  const bool isGridded = dataset->isStructuredGrid();
  const emvl::Point* const coordinates = subsetPointers.coordinates;
  const double* const data = subsetPointers.data;
  emvl::Point computedCellVertices[ 8 ];
  const emvl::Point* cellVertices = &computedCellVertices[ 0 ];
  const VisOptions* const vOptions = VisOptionsSingleton::getInstance();
  const double degreesPerMeter = 1.0/110574.2727;
  const double zScaleFactor = vOptions->getCalipsoScaleFactor();
  double elevationScale = degreesPerMeter * zScaleFactor;
  double previousValue = data[ 0 ];
  assert( myLut );
  Vector4f previousColor = myLut->colorByValue( previousValue );
  const bool isSurfaceLayer = dataset->isSurfaceData();
  const bool flattenFlag = isGridded && vOptions->getFlattenCMAQ();
  RSIGScene& rsigScene = RSIGScene::getInstance();
  const bool checkAndClipEdgeCells =
    ! rsigScene.isStereographicProj() && ! rsigScene.isSphericalProj();
  float elevationFlattenOffset0 = 0.0; // default
  float elevationFlattenOffset1 = 0.0; // default
  float elevationFlattenOffset2 = 0.0; // default
  float elevationFlattenOffset3 = 0.0; // default
  float elevationFlattenOffset4 = 0.0; // default
  float elevationFlattenOffset5 = 0.0; // default
  float elevationFlattenOffset6 = 0.0; // default
  float elevationFlattenOffset7 = 0.0; // default
  bool firstCellFlag = true;

  glEnable(GL_DEPTH_TEST);
  glEnable( GL_BLEND ); // Enable semi-transparency.
  glDisable( GL_CULL_FACE );

  if ( cellVertexCount == 8 ) {
    glEnable( GL_CULL_FACE ); // Don't draw cell face if pointing away.
  }

  if ( cellVertexCount == 1 ) {
    glDepthFunc( GL_ALWAYS );
    glBegin( GL_TRIANGLE_STRIP );
  } else {
    glBegin( GL_QUADS );
  }

  if ( showCellBoundary ) {
    glColor4f( 0.0, 0.0, 0.0, alpha );
  } else {
    glColor4f( previousColor.x, previousColor.y, previousColor.z, alpha );
  }

  for ( size_t dataIndex = 0, cellLayer = 0, cellRow = 0, cellColumn = 0;
        dataIndex < dataCount; ++dataIndex ) {
    const double value = data[ dataIndex ];
    const bool valueIsNonMissing = OR2( showNullSample, value > emvl::MISSING );
    const bool valueIsInThresholdRange =
      IMPLIES( showThreshold, value >= threshold );
    const bool drawThresholdedValue =
      AND2( valueIsInThresholdRange, OR2( ! isGridded, showThreshold ) );
    bool drawSlabCell = false;

    if ( isGridded ) {
      drawSlabCell =
        OR3( cellLayer  + 1 == layer,
             cellRow    + 1 == row,
             cellColumn + 1 == column );

      ++cellColumn;

      if ( cellColumn == columns ) {
        cellColumn = 0;
        ++cellRow;

        if ( cellRow == rows ) {
          cellRow = 0;
          ++cellLayer;

          if ( cellLayer == layers ) {
            cellLayer = 0;
          }
        }
      }
    }

    const bool drawValue =
      AND2( valueIsNonMissing, OR2( drawSlabCell, drawThresholdedValue ) );

    if ( drawValue ) {
      const size_t cell = dataIndex % cellCount;

      if ( coordinates ) {
        const size_t coordinatesIndex = cell * cellVertexCount;
        cellVertices = coordinates + coordinatesIndex;
      } else {
        dataset->cellVertices( cell, computedCellVertices );

        /*
         * If not stereographic view then clip cells that cross the -180/180
         * line to avoid drawing wide cells that stretch across the globe.
         * When viewing stereographic we must retain such cells to avoid a gap.
         */

        if ( checkAndClipEdgeCells ) {
          clipEdgeCell( cellVertexCount, computedCellVertices );
        }
      }

      if ( cellVertexCount == 1 ||
           quadrilateralIntersectsViewBounds( viewBounds, cellVertices ) ) {

        if ( ! showCellBoundary && value != previousValue ) {
          previousValue = value;
          assert( myLut );
          const Vector4f thisColor( myLut->colorByValue( value ) );

          if ( thisColor != previousColor ) {
            previousColor = thisColor;
            glColor4f( thisColor.x, thisColor.y, thisColor.z, alpha );
          }
        }

        /*
         * Draw each cell face as a colored semi-transparent quadrilateral
         * with vertices in counter-clockwise order or triangle strip.
         */

        if ( cellVertexCount == 1 ) { // polygon as triangle strip.
          const size_t vertexCount = subsetPointers.coordinatesCount;

          for ( size_t vertex = 0; vertex < vertexCount; ++vertex ) {
            const emvl::Point& v = coordinates[ vertex ];
            const double longitude = v.longitude;
            const double latitude  = v.latitude;
            const double elevation = v.elevation;
            glVertex3f( longitude, latitude, elevation );
          }

        } else if ( cellVertexCount == 4 ) {

          /*
           * Note: stored cell vertex order is counter-clockwise:
           *   (west, south),
           *   (east, south),
           *   (east, north),
           *   (west, north)
           *
           *   v3-------v2
           *   |        |
           *   |        |
           *   |        |
           *   v0-------v1
           */

          const emvl::Point& v0 = cellVertices[ 0 ];
          const emvl::Point& v1 = cellVertices[ 1 ];
          const emvl::Point& v2 = cellVertices[ 2 ];
          const emvl::Point& v3 = cellVertices[ 3 ];

          if (firstCellFlag && flattenFlag) {
            firstCellFlag = false;
            elevationFlattenOffset0 = v0.elevation * elevationScale;
            elevationFlattenOffset1 = v1.elevation * elevationScale;
            elevationFlattenOffset2 = v2.elevation * elevationScale;
            elevationFlattenOffset3 = v3.elevation * elevationScale;
            elevationScale = 0.0;
          }

          glVertex3f( v0.longitude,
                      v0.latitude,
                      v0.elevation * elevationScale + elevationFlattenOffset0);
          glVertex3f( v1.longitude,
                      v1.latitude,
                      v1.elevation * elevationScale + elevationFlattenOffset1);
          glVertex3f( v2.longitude,
                      v2.latitude,
                      v2.elevation * elevationScale + elevationFlattenOffset2);
          glVertex3f( v3.longitude,
                      v3.latitude,
                      v3.elevation * elevationScale + elevationFlattenOffset3);
        } else {
          assert( cellVertexCount == 8 );

          /*
           * Ensure each cell face's normal points outward from cell center.
           * Note: stored cell vertex order is:
           *   (west, south, lower), (east, south, lower),
           *   (east, north, lower), (west, north, lower),
           *   (west, south, upper), (east, south, upper),
           *   (east, north, upper), (west, north, upper).
           *
           *             v7---------v6
           *            / |         /|
           *           /  |        / |
           *          /   |       /  |
           *        v4----------v5   |
           *         |   /v3----|---|v2
           *         |  /       |  /
           *         | /        | /
           *         |/         |/
           *        v0---------v1
           *
           * Draw faces in this order:
           * top, bottom, south, north, west, east.
           */

          const emvl::Point& v0 = cellVertices[ 0 ];
          const emvl::Point& v1 = cellVertices[ 1 ];
          const emvl::Point& v2 = cellVertices[ 2 ];
          const emvl::Point& v3 = cellVertices[ 3 ];
          const emvl::Point& v4 = cellVertices[ 4 ];
          const emvl::Point& v5 = cellVertices[ 5 ];
          const emvl::Point& v6 = cellVertices[ 6 ];
          const emvl::Point& v7 = cellVertices[ 7 ];

          if (firstCellFlag && flattenFlag) {
            firstCellFlag = false;

            if ( isSurfaceLayer ) {
              elevationFlattenOffset0 = 0.0;
              elevationFlattenOffset1 = 0.0;
              elevationFlattenOffset2 = 0.0;
              elevationFlattenOffset3 = 0.0;
              elevationFlattenOffset4 =
                ( v4.elevation - v0.elevation ) * elevationScale;
              elevationFlattenOffset5 =
                ( v5.elevation - v1.elevation ) * elevationScale;
              elevationFlattenOffset6 =
                ( v6.elevation - v2.elevation ) * elevationScale;
              elevationFlattenOffset7 =
                ( v7.elevation - v7.elevation ) * elevationScale;
              elevationScale = 0.0;
            } else {
              elevationFlattenOffset0 = v0.elevation * elevationScale;
              elevationFlattenOffset1 = v1.elevation * elevationScale;
              elevationFlattenOffset2 = v2.elevation * elevationScale;
              elevationFlattenOffset3 = v3.elevation * elevationScale;
              elevationFlattenOffset4 = v4.elevation * elevationScale;
              elevationFlattenOffset5 = v5.elevation * elevationScale;
              elevationFlattenOffset6 = v6.elevation * elevationScale;
              elevationFlattenOffset7 = v7.elevation * elevationScale;
              elevationScale = 0.0;
            }
          }

          const double v0_longitude = v0.longitude;
          const double v1_longitude = v1.longitude;
          const double v2_longitude = v2.longitude;
          const double v3_longitude = v3.longitude;
          const double v4_longitude = v4.longitude;
          const double v5_longitude = v5.longitude;
          const double v6_longitude = v6.longitude;
          const double v7_longitude = v7.longitude;
          const double v0_latitude = v0.latitude;
          const double v1_latitude = v1.latitude;
          const double v2_latitude = v2.latitude;
          const double v3_latitude = v3.latitude;
          const double v4_latitude = v4.latitude;
          const double v5_latitude = v5.latitude;
          const double v6_latitude = v6.latitude;
          const double v7_latitude = v7.latitude;
          const double v0_elevation =
            v0.elevation * elevationScale + elevationFlattenOffset0;
          const double v1_elevation =
            v1.elevation * elevationScale + elevationFlattenOffset1;
          const double v2_elevation =
            v2.elevation * elevationScale + elevationFlattenOffset2;
          const double v3_elevation =
            v3.elevation * elevationScale + elevationFlattenOffset3;
          const double v4_elevation =
            v4.elevation * elevationScale + elevationFlattenOffset4;
          const double v5_elevation =
            v5.elevation * elevationScale + elevationFlattenOffset5;
          const double v6_elevation =
            v6.elevation * elevationScale + elevationFlattenOffset6;
          const double v7_elevation =
            v7.elevation * elevationScale + elevationFlattenOffset7;

          // Draw top face - v4, v5, v6, v7 - normal points up.

          glVertex3f( v4_longitude, v4_latitude, v4_elevation );
          glVertex3f( v5_longitude, v5_latitude, v5_elevation );
          glVertex3f( v6_longitude, v6_latitude, v6_elevation );
          glVertex3f( v7_longitude, v7_latitude, v7_elevation );

          // Draw bottom face - v3, v2, v1, v0 - normal points down.

          glVertex3f( v3_longitude, v3_latitude, v3_elevation );
          glVertex3f( v2_longitude, v2_latitude, v2_elevation );
          glVertex3f( v1_longitude, v1_latitude, v1_elevation );
          glVertex3f( v0_longitude, v0_latitude, v0_elevation );

          // Draw south face - v0, v1, v5, v4 - normal points south.

          glVertex3f( v0_longitude, v0_latitude, v0_elevation );
          glVertex3f( v1_longitude, v1_latitude, v1_elevation );
          glVertex3f( v5_longitude, v5_latitude, v5_elevation );
          glVertex3f( v4_longitude, v4_latitude, v4_elevation );

          // Draw north face - v2, v3, v7, v6 - normal points north.

          glVertex3f( v2_longitude, v2_latitude, v2_elevation );
          glVertex3f( v3_longitude, v3_latitude, v3_elevation );
          glVertex3f( v7_longitude, v7_latitude, v7_elevation );
          glVertex3f( v6_longitude, v6_latitude, v6_elevation );

          // Draw west face - v3, v0, v4, v7 - normal points west.

          glVertex3f( v3_longitude, v3_latitude, v3_elevation );
          glVertex3f( v0_longitude, v0_latitude, v0_elevation );
          glVertex3f( v4_longitude, v4_latitude, v4_elevation );
          glVertex3f( v7_longitude, v7_latitude, v7_elevation );

          // Draw east face - v1, v2, v6, v5 - normal points west.

          glVertex3f( v1_longitude, v1_latitude, v1_elevation );
          glVertex3f( v2_longitude, v2_latitude, v2_elevation );
          glVertex3f( v6_longitude, v6_latitude, v6_elevation );
          glVertex3f( v5_longitude, v5_latitude, v5_elevation );
        }
      }
    }
  }

  glEnd();

  if ( cellVertexCount == 8 ) {
    glDisable( GL_CULL_FACE );
  }
}



static void
drawOneGroup(const QRectF& viewBounds,
             bool showNullSample,
             bool showCellBoundary,
             const emvl::Dataset* const dataset,
             const SubsetPointers& subsetPointers,
             const int gridRow,
             const int gridCol,
             const int gridLayer,
             const Vector4i * elements,
             float /* mLineWidth */, const QColor& mLineColor,
             float alpha, float threshold, bool showThreshold, 
             QPainter * painter,
             const QRect & rect,
             RenderingContextRSIGBase *context,
             std::vector<Vector4f>& tridata )
{
  const unsigned char gray = 120;
  const unsigned char black = 255;

  if ( context == NULL && is2DCurtain == false) { // 3d mode rendering
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // See both sides of CALIPSO.
    glShadeModel( GL_FLAT ); // Constant-colored polygons.
    glEnable( GL_BLEND ); // Enable semi-transparency.
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    render3d( viewBounds, dataset, subsetPointers, gridRow, gridCol, gridLayer,
              threshold, showThreshold, false, false, alpha ); // Immediate-mode, non-buffered

    if ( showCellBoundary ) {
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      render3d( viewBounds, dataset, subsetPointers, gridRow, gridCol, gridLayer,
                threshold, showThreshold, false, true, alpha ); // Immediate-mode, non-buffer
    }

    glDisable( GL_BLEND );
  } else { // 2d mode rendering

    //================== draw cells with missing or clipped values =============
    //draw the boundary lines for points
    if (showNullSample) {
      glDisable(GL_BLEND);

      if (context != NULL) {
        context->setTextureWeight(0.0f);
      }

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor3ub(gray, gray, gray);
      pagedBufferDraw(viewBounds, dataset, subsetPointers, gridLayer, threshold, showThreshold, elements,
                      true, painter, rect, context, tridata);
      
      if (showCellBoundary) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //draw inner content for points
        glColor3ub(black, black, black);
        pagedBufferDraw(viewBounds, dataset, subsetPointers, gridLayer,
                        threshold, showThreshold, elements,
                        true, painter, rect, context, tridata);
      }
    }

    //================== draw the cells colored by data ========================

    if (context != NULL) {
      glShadeModel( GL_FLAT ); // Constant-colored polygons.
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      context->setTextureWeight(1.0f);
    }

    const unsigned char white = 255;
    const int ialpha = int(alpha * 255.0f);
    glColor4ub(white, white, white, ialpha);
    pagedBufferDraw(viewBounds, dataset, subsetPointers, gridLayer, threshold,
                    showThreshold, elements, false, painter, rect, context,
                    tridata);
    glDisable(GL_BLEND);

    if (showCellBoundary) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glColor3ub(mLineColor.red(), mLineColor.green(), mLineColor.blue());

      if (context != NULL) {
        context->setTextureWeight(0.0f);
      }

      pagedBufferDraw(viewBounds, dataset, subsetPointers, gridLayer,
                      threshold, showThreshold, elements,
                      false, painter, rect, context, tridata);
    }
  }
}


void PolygonRenderer::initCalipsoCurtainPlotarea(const QRect & rect) {
  const int leftmargin = 10;
  const int rightmargin = 10;
  int topmargin = 20;

  plotwindow_x0 = rect.x() + leftmargin;
  plotwindow_y0 = topmargin;
  
  int plotWindowWidth = rect.width() - leftmargin - rightmargin;
  int plotWindowHeight = 200; // evenly divisible by 40 (km)

  curtainFrame = QRectF(plotwindow_x0, plotwindow_y0, plotWindowWidth, plotWindowHeight);
  
  // axis origin
  int xAxisMargin = 50;
  int yAxisMargin = 25;
  origin_x0 = plotwindow_x0 + xAxisMargin;
  origin_y0 = plotwindow_y0 + yAxisMargin - 15;

  // axis lengths
  xAxisLength = plotWindowWidth-2*xAxisMargin;
  yAxisLength = plotWindowHeight-2*yAxisMargin;

  curtainPlotRect = QRectF(origin_x0, origin_y0, xAxisLength, yAxisLength);
}



void PolygonRenderer::drawCalipsoAxesOpengl(const QRect& /* rect */) {

  GeoDataObject* obj = qobject_cast<GeoDataObject *>( parent() );

  if ( obj == NULL || obj->model() == NULL ) {
    return;
  }

  RSIGScene& rsigScene = RSIGScene::getInstance();

  // push the matrix to prepare for doing 2D overplot
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  // set up 2D rendering
  float orig_linewidth;
  glDepthMask(GL_FALSE);
  glDisable(GL_DEPTH_TEST);
  glOrtho(0.0f, rsigScene.sceneRect().width(), rsigScene.sceneRect().height(), 0.0f, 0.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glGetFloatv(GL_LINE_WIDTH, &orig_linewidth);

  // draw background quad
  glColor3f(253./256., 245./256., 230./256.); // linen
  glBegin(GL_QUADS);
  glVertex2f(curtainFrame.left(), curtainFrame.bottom());
  glVertex2f(curtainFrame.right(), curtainFrame.bottom());
  glVertex2f(curtainFrame.right(), curtainFrame.top());
  glVertex2f(curtainFrame.left(), curtainFrame.top());
  glEnd();  

  // draw Y axes
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(1.0);
  glBegin(GL_LINES);
  glVertex2f(origin_x0, origin_y0);
  glVertex2f(origin_x0, origin_y0+yAxisLength);
  glVertex2f(origin_x0+xAxisLength, origin_y0);
  glVertex2f(origin_x0+xAxisLength, origin_y0+yAxisLength);
  glEnd();

  // draw Y axis ticks and labels (down from top)
  //int yTickInterval = 5; //km

  int yTickLength  = 15; // screen space
  int yTextOffsetX = 20; //screen space
  int yTextOffsetY =  5; //screen space

  for (int yTick = 40; yTick >0; yTick-=5) {
    int yTickPosition = origin_y0 + (40-yTick)*yAxisLength/40;

    glBegin(GL_LINES);
    // right side ticks
    glVertex2f(origin_x0+xAxisLength-yTickLength, yTickPosition);
    glVertex2f(origin_x0+xAxisLength, yTickPosition);
    // left side ticks
    glVertex2f(origin_x0, yTickPosition);
    glVertex2f(origin_x0+yTickLength, yTickPosition);
    glEnd();
    
    char ylabelString[50];
    sprintf(ylabelString, "%d", yTick);
    renderBitmapString2(origin_x0-yTextOffsetX, yTickPosition+yTextOffsetY, 0, GLUT_BITMAP_HELVETICA_12, ylabelString);
  }

  // draw X axes
  glBegin(GL_LINES);
  glVertex2f(origin_x0, origin_y0);
  glVertex2f(origin_x0+xAxisLength, origin_y0);
  glVertex2f(origin_x0, origin_y0+yAxisLength);
  glVertex2f(origin_x0+xAxisLength, origin_y0+yAxisLength);
  glEnd();
    
  // X axis title
  int xTitleX = origin_x0 + xAxisLength/2.0 - 30;
  int xTitleY = origin_y0 + yAxisLength + 35;
  char xlabelString[50];
  sprintf(xlabelString, "%s", "Longitude [deg]");
  renderBitmapString2(xTitleX, xTitleY, 0, GLUT_BITMAP_HELVETICA_12, xlabelString);
  
  // X axis ticks and labels
  int xTickLength = 15; // screen space
  QPointF geoLeft   = rsigScene.toGeoPointF(QPointF(origin_x0,             origin_y0+yAxisLength));
  QPointF geoRight  = rsigScene.toGeoPointF(QPointF(origin_x0+xAxisLength, origin_y0+yAxisLength));
  QPointF geoCenter = rsigScene.toGeoPointF(QPointF(rsigScene.sceneRect().width()/2, origin_y0+yAxisLength));

  double plot_lonmin = geoLeft.x();
  double plot_lonmax = geoRight.x();
  double plot_loncenter = geoCenter.x();

  // correct wrapping issues

  if ( plot_lonmin > plot_lonmax ) {

    if ( plot_lonmin > 0 && plot_loncenter < 0 ) {
      plot_lonmin = -180;
    }
  }

  if ( plot_lonmax < 0 && plot_loncenter > 0 ) {
    plot_lonmax = 180.0;
  }

  // choose step size
  const int xsteps[]={1, 2, 5, 10, 15, 20, 30, 40}; // all evenly divisible into 360
  double lonrange = plot_lonmax - plot_lonmin;
  int nSteps = 5;
  double desiredDeltaLon = lonrange / nSteps;
  double deltaLon = xsteps[0];

  for ( size_t index = 1; index < sizeof xsteps / sizeof *xsteps; ++index ) {

    if (desiredDeltaLon - xsteps[ index ] < 0) {
      break;
    }

    deltaLon = xsteps[ index ];
  }

  // loop through all lons and label those within the range

  for (int thisLon=-180; thisLon<180; thisLon+=deltaLon) {

    if (thisLon>plot_lonmin && thisLon<plot_lonmax) {
      double thisScreenX = origin_x0 + xAxisLength*(thisLon - plot_lonmin)/(plot_lonmax - plot_lonmin);
      
      // draw ticks

      glBegin(GL_LINES);
      glVertex2f(thisScreenX, origin_y0);
      glVertex2f(thisScreenX, origin_y0+xTickLength);
      glVertex2f(thisScreenX, origin_y0+yAxisLength-xTickLength);
      glVertex2f(thisScreenX, origin_y0+yAxisLength);
      glEnd();

      // draw label
      char xlabelString[50];
      sprintf(xlabelString, "%d", thisLon);
      renderBitmapString2(thisScreenX, origin_y0+yAxisLength + 20, 0, GLUT_BITMAP_HELVETICA_12, xlabelString);
    }
  }
  
  // restore the original 3D matrix
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glLineWidth(orig_linewidth);
}




/*!
 \fn void PolygonRenderer::drawQuadArray()

 Draw the polygon dataset in an array of rectangular elements.
 */
void PolygonRenderer::drawQuadArray(QPainter * painter, const QRect & rect, RenderingContextRSIGBase * context) {
  GeoDataObject* obj = qobject_cast<GeoDataObject *>( parent() );

  if ( obj == NULL || obj->model() == NULL ) {
    return;
  }

  VisOptions *vOptions = VisOptionsSingleton::getInstance();

  // Get the lon-lat bounding box of the view window:
  
  const QRectF viewBounds = obj->getDataRequestBoundingBox();
  const emvl::Dataset* const dataset = obj->model();
  const std::string dataName = dataset->name();

  const bool isBEHR    = dataset->description().find("BEHR") != std::string::npos;
  const bool isMODIS   = dataset->description().find("MODIS") != std::string::npos;
  const bool isVIIRS   = dataset->description().find("VIIRS") != std::string::npos;
  const bool isTROPOMI = dataset->description().find("TROPOMI") != std::string::npos;
  const bool isHMS     = dataset->description().find("hmsserver") != std::string::npos;
  //const bool isTEMPO   = dataset->description().find("TEMPO") != std::string::npos;

  const RSIGState* const gState = RSIGStateSingleton::getInstance();
  emvl::Timestamp beginTime( gState->getCurrentTimestamp() );
  const emvl::Timestamp endTime( gState->getEndRenderTimestamp() );

  if ( ! dataset->isPagedFromDisk() ) {

    /*
     * Some datasets such as Swath and CMAQ are too large to fit entirely in
     * memory so only one (the current) timestep is in memory at a time
     * and subsequent timesteps are paged in from disk.
     * A consequence of this is that 'persist' (displaying multiple timesteps)
     * is not possible.
     */

    if ( isBEHR && vOptions->getPersistBEHR() )  {
      const int persistHours = vOptions->getPersistBEHRHours();
      beginTime.offsetHours( -persistHours );
    } else if ( isMODIS && vOptions->getPersistMODIS() )  {
      const int persistHours = vOptions->getPersistMODISHours();
      beginTime.offsetHours( -persistHours );
    } else if ( isVIIRS && vOptions->getPersistVIIRS() )  {
      const int persistHours = vOptions->getPersistVIIRSHours();
      beginTime.offsetHours( -persistHours );
    } else if ( isTROPOMI && vOptions->getPersistTROPOMI() )  {
      const int persistHours = vOptions->getPersistTROPOMIHours();
      beginTime.offsetHours( -persistHours );
    } else if ( isHMS && vOptions->getPersistHMS() )  {
      const int persistHours = vOptions->getPersistHMSHours();
      beginTime.offsetHours( -persistHours );
    //} else if (isTEMPO) { // TEMP HACK: hardcoded TEMPO persist
      //const int persistHours = vOptions->getPersistTEMPOHours();
      //beginTime.offsetHours( -persistHours );
    }
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  
  if (mShowCellBoundary) {
    float linewidth = mLineWidth;

    if (linewidth <= 0.001f) {
      linewidth = 0.001f;
    }

    glLineWidth(linewidth);
  }

  const bool isCeilometer = dataName.find( "ceilometer" ) == 0;
  RSIGScene& rsigScene = RSIGScene::getInstance();  

  // Curtain popup plots for regridded ceilometer data are unimplemented.

  if (isCeilometer && rsigScene.ceilometerShowRegridWarning) {
    postMessage("Regridded ceilometer curtain is not implemented.", RSIGState::WARNING_TEXT);
    rsigScene.ceilometerShowRegridWarning = false;
  }

  // Get groups of pointers to subset of coordinates and data for current hour:

  const std::vector<SubsetPointers> subsets = dataset->subset( beginTime, endTime );
  const int gridRow    = dataset->isStructuredGrid() ? this->CMAQRow()   : 0;
  const int gridColumn = dataset->isStructuredGrid() ? this->CMAQCol()   : 0;
  const int gridLayer  = dataset->isStructuredGrid() ? this->CMAQLayer() : 0;

  for ( size_t subset = 0; subset < subsets.size(); ++subset ) {
    const SubsetPointers& subsetPointers = subsets[ subset ];
    drawOneGroup( viewBounds, mShowNullSample, mShowCellBoundary,
                  dataset, subsetPointers, gridRow, gridColumn, gridLayer,
                  mElements, mLineWidth, mLineColor, mAlpha, mThreshold,
                  mShowThreshold, painter, rect, context, tridata );
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void PolygonRenderer::render(QPainter* painter,
                             const QRect& rect,
                             RenderingContextRSIGBase* context,
                             bool /* flagPopupPlot */ ) {

  is2DCurtain = false;

  if ( parent() && isVisible() ) {

    if ( context ) {
      GeoDataObject *par = qobject_cast<GeoDataObject*>(parent());

      if ( par ) {
        myLut = par->getLut();
        assert( myLut );
        setContextColors( false, myLut, context );
      }
    }

    drawQuadArray( painter, rect, context );
  }
}

void PolygonRenderer::renderDisplayPanel(QPainter* /* painter */,
                             const QRect& /* rect */,
                             RenderingContextRSIGBase* /* context */) {

  qDebug() << "in PolygonRenderer::renderDisplayPanel - UNIMPLEMENTED";

}


void PolygonRenderer::renderEMVL(QPainter* /* painter */,
                                 const QRect& /* rect */,
                                 bool /* flagPopupPlot */) {

  is2DCurtain = false;

  if ( parent() && isVisible() ) {
    GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());

    if ( par ) {
      myLut = par->getLut();
      assert( myLut );

      // Use vis object minmax rather than data minmax
      // WTF? theMinmax = this->minmaxVal();
      QRect dummyRect = QRect();
      drawQuadArray( NULL, dummyRect, NULL );
    }
  }
}



void PolygonRenderer::render2DCalipsoCurtain(QPainter* painter,
                                             const QRect& rect) {

  if (parent()==NULL ||!isVisible()) {
    return;
  }

  GeoDataObject* par = qobject_cast<GeoDataObject*>(parent());

  if (par != NULL) {
    glDisable(GL_TEXTURE_2D);
    myLut = par->getLut();
    assert( myLut );
    is2DCurtain = true;
    initCalipsoCurtainPlotarea(rect);
    drawCalipsoAxesOpengl(rect);
    drawQuadArray( painter, rect, NULL );
    glEnable(GL_TEXTURE_2D);
  }
}



void PolygonRenderer::renderPopupNeubrew( QPainter* /* painter */,
                                          const QRect& /* rect */,
                                          bool /* haveCalipso */ ) {

  RSIGScene& rsigScene = RSIGScene::getInstance();

  if (rsigScene.neubrewShowRegridWarning) {
    postMessage("Regridded neubrew curtain is UNIMPLEMENTED.", RSIGState::WARNING_TEXT);
    rsigScene.neubrewShowRegridWarning = false;
  }
}



void PolygonRenderer::renderPopupCeilometer( QPainter*, const QRect&, bool ) {
  // do nothing
}



qreal PolygonRenderer::lineWidth() const {
  return mLineWidth;
}



void PolygonRenderer::setLineWidth(qreal w) {

  if ( w < 0 ) {
    w = 0;
  }

  mLineWidth = w;
  Q_EMIT valueChanged();
}



QColor PolygonRenderer::lineColor() const {
  return mLineColor;
}

void PolygonRenderer::setLineColor(const QColor& color) {

  if ( mLineColor != color ) {
    mLineColor = color;
    Q_EMIT valueChanged();
  }
}



bool PolygonRenderer::showNullSample() const {
  return mShowNullSample;
}



void PolygonRenderer::setShowNullSample(bool flag) {

  if (mShowNullSample != flag) {
    mShowNullSample = flag;
    Q_EMIT valueChanged();
  }
}



QString PolygonRenderer::description() const {
  return "polygon";
}

