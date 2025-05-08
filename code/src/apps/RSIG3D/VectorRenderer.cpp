/******************************************************************************
PURPOSE: VectorRenderer.cpp - Implements a vector dataset rendering class.

NOTES: For rendering vector data sets using OpenGL APIs.

HISTORY: 2018-08-20 plessel.todd@epa.gov based on PolygonRenderer.
         Evolved by freeman.matt@epa.gov
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
#include "VectorRenderer.h"
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


//=====================================================================



VectorRenderer::VectorRenderer( QObject* parent )
  : VisDataObject(parent), mLineWidth(1.0f), mLineColor(0, 0, 0),
  mShowNullSample(false) {
  mAlpha = 1.0f;
  mThreshold = 1.0f;
}



VectorRenderer::~VectorRenderer() {
}



bool VectorRenderer::isLookupTableWidget() const {
  return false;
}



bool VectorRenderer::isWidgetOfType(QString inquiry) const {
  GeoDataObject* const p = qobject_cast<GeoDataObject*>(parent());
  const bool result = p->varName().contains(inquiry, Qt::CaseInsensitive);
  return result;
}



bool VectorRenderer::is3DObject() const {
  return true;
}

// Draw the vector dataset:

void VectorRenderer::drawVectors( QPainter*,
                                  const QRect&,
                                  RenderingContextRSIGBase* context ) {

  const GeoDataObject* const geoDataObject =
    qobject_cast<const GeoDataObject*>( parent() );

  if ( geoDataObject ) {
    const emvl::Dataset* const dataset = geoDataObject->model();

    // Check if dataset is a vector:

    if ( AND4( dataset,
               dataset->isVector(),
               dataset->variables() > 1,
               dataset->cellVertexCount() <= 8 ) ) {

      // Get the time range to draw:

      const RSIGState* const gState = RSIGStateSingleton::getInstance();
      const emvl::Timestamp beginTime( gState->getCurrentTimestamp() );
      const emvl::Timestamp endTime( gState->getEndRenderTimestamp() );

      // Get groups of pointers to subset of coordinates and data for hour:

      const std::vector<emvl::Dataset::SubsetPointers> subsets =
        dataset->subset( beginTime, endTime );
      const size_t subsetsCount = subsets.size();
      const int skipVariable =
        OR2( dataset->variableName(0) == "ELEVATION",
             dataset->variableName(0) == "station" ) ? 1 : 0;
      const int components = dataset->variables() - skipVariable;

      /*
       * Check subsets for valid count -
       * (multiple of 2 for 2D vectors, multiple of 3 for 3D vectors).
       *   vector-u = subsets[ subset + 0 ]
       *   vector-v = subsets[ subset + 1 ]
       *   vector-w = subsets[ subset + 2 ] (if it exists).
       */

      if ( AND3( subsetsCount,
                 IMPLIES( components == 2, subsets.size() % 2 == 0 ),
                 IMPLIES( components == 3, subsets.size() % 3 == 0 ) ) ) {
        const int is3DView   = ! context;
        const size_t cellVertexCount = dataset->cellVertexCount();
        emvl::Point computedCellVertices[ 8 ];
        const emvl::Point* cellVertices = &computedCellVertices[ 0 ];
        const bool isGrid    = dataset->isStructuredGrid();
        const int layers     = dataset->layers();
        const int rows       = dataset->rows();
        const int columns    = dataset->columns();
        const int rowsTimesColumns = rows * columns;
        const int selectedRow    = isGrid && is3DView ? this->CMAQRow() : 0;
        const int selectedColumn = isGrid && is3DView ? this->CMAQCol() : 0;
        const bool isVector3 = components == 3;
        const int selectedLayer  = isGrid && layers > 0 ? this->CMAQLayer() : 0;

        // Get the lon-lat bounding box and pixel width of the view window:
        
        const QRectF viewBounds( geoDataObject->getDataRequestBoundingBox() );
        RSIGScene& rsigScene = RSIGScene::getInstance();
        const double viewWindowWidthInDegrees =
          rsigScene.screenGeoRectF().width();
        const int viewWindowWidthInPixels = rsigScene.sceneRect().width();
        const double degreesPerPixel =
          viewWindowWidthInDegrees / viewWindowWidthInPixels;
        const double meanEarthRadius = 6370997.0; // meters.
        const double meanEarthCircumference = 2.0 * M_PI * meanEarthRadius; // meters
        const double degreesPerMeter = 360.0 / meanEarthCircumference;
        VisOptions* const visOptions = VisOptionsSingleton::getInstance();
        const double zScaleFactor = visOptions->getCalipsoScaleFactor();
        const double elevationScale = degreesPerMeter * zScaleFactor;
        const double pixelsPerUnitLength = 2.0 * (visOptions->getWindVectorScaleFactor()/ 20.0);
        const int headLengthPixels = visOptions->getWindVectorTipSize();

        glLineWidth( 1.5 );  // Set line thickness.

        if ( is3DView ) { // 3D mode
          glEnable( GL_DEPTH_TEST );
          glEnable( GL_BLEND );
          glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
          glShadeModel( GL_FLAT ); 
          glDepthMask( GL_TRUE ); 
        } else { // 2D mode
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glColor4f( 1.0, 1.0, 1.0, mAlpha );
          context->setTextureWeight( 1.0f );
       }

        // Draw reference vector.
        // These parameters are shared by 2D and 3D reference vectors.

        const double referenceU = 10.0;
        const double referenceV = 0.0;
        const double referenceMagnitude = 10.0;
        visOptions->setWindVectorLabel( "10 m/s" );
        visOptions->setWindVectorLength((int)(pixelsPerUnitLength * referenceMagnitude));
        
        /*
         * In 3D view, create and draw reference vector in scene.
         * (In 2D view the reference vector is drawn in RSIGScene.)
         */

        if ( is3DView && visOptions->getShowRuler()) {
          const double referenceLon  = visOptions->getRulerLon();
          const double referenceLat  = visOptions->getRulerLat();
          const double referenceElev = 20000.0 * elevationScale;
          double p0X = 0.0;
          double p0Y = 0.0;
          double p1X = 0.0;
          double p1Y = 0.0;
          double p2X = 0.0;
          double p2Y = 0.0;
          double p3X = 0.0;
          double p3Y = 0.0;

          computeArrowVectorCoordinates2(headLengthPixels,
                                         referenceLon, referenceLat,
                                         referenceU, referenceV,
                                         degreesPerPixel,
                                         pixelsPerUnitLength,
                                         &p0X, &p0Y,
                                         &p1X, &p1Y,
                                         &p2X, &p2Y,
                                         &p3X, &p3Y );

          glColor3f(1.0f, 1.0f, 1.0f);
          glBegin( GL_LINE_STRIP );
          glVertex3f( p0X, p0Y, referenceElev );
          glVertex3f( p1X, p1Y, referenceElev );
          glVertex3f( p2X, p2Y, referenceElev );
          glVertex3f( p1X, p1Y, referenceElev );
          glVertex3f( p3X, p3Y, referenceElev );
          glEnd();

          char vectorLabel[50];
          sprintf(vectorLabel, "     %5.1f m/s", referenceMagnitude );
          rsigScene.renderBitmapString(referenceLon, referenceLat, referenceElev, GLUT_BITMAP_HELVETICA_12, vectorLabel);
        }

        // Set stride:

        int stride = visOptions->getWindVectorSparseness();
        const bool isHRRR =
          dataset->description().find("HRRR") != std::string::npos;

        if ( visOptions->getWindVectorAutosparse() ) {

          if ( isHRRR ) {

            if (     viewWindowWidthInDegrees > 50 ) { stride = 50; }
            else if (viewWindowWidthInDegrees > 20 ) { stride = 25; }
            else if (viewWindowWidthInDegrees >  0 ) { stride = 10; }
          } else {

            if (     viewWindowWidthInDegrees > 50 ) { stride = 10; }
            else if (viewWindowWidthInDegrees > 20 ) { stride =  5; }
            else if (viewWindowWidthInDegrees >  0 ) { stride =  2; }
          }
        }

        const int stride20 = stride / 2;
        const int stride2 = stride20 ? stride20 : 1;
        bool isDrawableCell = true;
        double previousValue = emvl::MISSING;
        Vector4f previousColor( 0.0, 0.0, 0.0, 0.0 );

        /*
         * Aircraft data has pairs of subsets (u, v) per (discontiguous) flight
         * track so we must loop over these multiple pairs per flight track.
         * Other datasets have just 2 (u, v) or 3 (u, v, w) subsets since their
         * data is contiguous by time.
         */

        const size_t subsetStride = 2 + isVector3;

        for (size_t subset = 0; subset < subsetsCount; subset += subsetStride) {
          const emvl::Dataset::SubsetPointers* const subsetU = &subsets[subset];
          const emvl::Dataset::SubsetPointers* const subsetV = subsetU + 1;
          const emvl::Dataset::SubsetPointers* const subsetW =
            isVector3 ? subsetV + 1 : 0;
          const size_t dataCount = subsetU->dataCount;
          const emvl::Point* const coordinates = subsetU->coordinates;

          assert( subsets[ subset     ].dataCount ==
                  subsets[ subset + 1 ].dataCount );
          assert( ! isVector3 ||
                  subsets[ subset     ].dataCount ==
                  subsets[ subset + 2 ].dataCount );
          assert( subsets[ subset     ].coordinatesCount ==
                  subsets[ subset + 1 ].coordinatesCount );
          assert( ! isVector3 ||
                  subsets[ subset     ].coordinatesCount ==
                  subsets[ subset + 2 ].coordinatesCount );
          assert( subsets[ subset     ].coordinates ==
                  subsets[ subset + 1 ].coordinates );
          assert( ! isVector3 ||
                  subsets[ subset     ].coordinates ==
                  subsets[ subset + 2 ].coordinates );

          for ( size_t cell = 0; cell < dataCount; cell += stride ) {

            if ( isGrid ) {
              const int thisRow   = cell / columns % rows   + 1;
              const int thisCol   = cell % columns          + 1;
              const int thisLayer = layers > 0 ? cell / rowsTimesColumns + 1 : 0;

              if ( is3DView ) {
                isDrawableCell = thisRow % stride == 0 // stride over rows
                  && OR3( thisLayer == selectedLayer,
                          (thisRow > selectedRow    - stride2) && (thisRow < selectedRow    + stride2) && (selectedRow > 0)    && (selectedRow <= rows),
                          (thisCol > selectedColumn - stride2) && (thisCol < selectedColumn + stride2) && (selectedColumn > 0) && (selectedColumn <= columns));
              } else {
                isDrawableCell = thisRow % stride == 0 // stride over rows
                  && thisLayer == selectedLayer;
              }
            }

            if ( isDrawableCell ) {

              // Get the cell vertices:

              if ( coordinates ) {
                const size_t coordinatesIndex = cell * cellVertexCount;
                cellVertices = coordinates + coordinatesIndex;
              } else {
                dataset->cellVertices( cell, computedCellVertices );
              }

              // Compute centroid of cell as the vector location:

              double longitude = 0.0;
              double latitude  = 0.0;
              double elevation = 0.0;

              for ( size_t vertex = 0; vertex < cellVertexCount; ++vertex ) {
                longitude += cellVertices[ vertex ].longitude;
                latitude  += cellVertices[ vertex ].latitude;
                elevation += cellVertices[ vertex ].elevation;
              }

              const double oneOverCellVertexCount = 1.0 / cellVertexCount;
              longitude *= oneOverCellVertexCount;
              latitude  *= oneOverCellVertexCount;
              elevation *= oneOverCellVertexCount;

              // Check if the cell is in the current view (possibly zoomed-in):

              const bool cellInView = viewBounds.contains(longitude, latitude);

              if ( cellInView ) {

                // Vector components:

                const double u = subsetU->data[ cell ];
                const double v = subsetV->data[ cell ];
                const double w = isVector3 ? subsetW->data[ cell ] : 0.0;
                const bool valueIsNonMissing =
                  AND3( u != emvl::MISSING,
                        v != emvl::MISSING,
                        w != emvl::MISSING );

                if ( valueIsNonMissing ) {
                  const double magnitude = sqrt( u * u + v * v + w * w );

                  // Check if magnitude is within threshold (if specified):

                  const bool inThreshold =
                    ( ! mShowThreshold || magnitude >= mThreshold );

                  if ( inThreshold ) {

                    /*
                     * Compute the vertices of the arrow representation:
                     *
                     *            v2
                     *             \
                     *    v0--------v1
                     *             /
                     *            v3
                     *
                     */

                    double point0X = 0.0;
                    double point0Y = 0.0;
                    double point1X = 0.0;
                    double point1Y = 0.0;
                    double point2X = 0.0;
                    double point2Y = 0.0;
                    double point3X = 0.0;
                    double point3Y = 0.0;
                    const double elevationEpsilon = 1.0; // to avoid z-fighting with the land surface
                    const double z0 = (elevation + elevationEpsilon) * elevationScale; 
                    const double z1 = z0 + w;
                    computeArrowVectorCoordinates2( headLengthPixels,
                                                    longitude, latitude,
                                                    u, v,
                                                    degreesPerPixel,
                                                    pixelsPerUnitLength,
                                                    &point0X, &point0Y,
                                                    &point1X, &point1Y,
                                                    &point2X, &point2Y,
                                                    &point3X, &point3Y );

                    if ( ! is3DView ) {
                      CHECK_GL_ERRORS2; // Print any errors before shaders.
                      glBegin( GL_LINE_STRIP );
                      glVertex4f( point0X, point0Y, z0, magnitude );
                      glVertex4f( point1X, point1Y, z1, magnitude );
                      glVertex4f( point2X, point2Y, z1, magnitude );
                      glVertex4f( point1X, point1Y, z1, magnitude );
                      glVertex4f( point3X, point3Y, z1, magnitude );
                      glEnd();
                      CHECK_GL_ERRORS2; // Print errors in shader programs.
                   } else {

                      // Set color only if value and color changed:

                      const double value = magnitude;

                      if ( value != previousValue ) {
                        previousValue = value;
                        assert( myLut );
                        const Vector4f thisColor( myLut->colorByValue( value ) );

                        if ( thisColor != previousColor ) {
                          previousColor = thisColor;
                          glColor4f( thisColor.x, thisColor.y, thisColor.z,
                                     mAlpha );
                        }
                      }

                      glBegin( GL_LINE_STRIP );
                      glVertex3f( point0X, point0Y, z0 );
                      glVertex3f( point1X, point1Y, z1 );
                      glVertex3f( point2X, point2Y, z1 );
                      glVertex3f( point1X, point1Y, z1 );
                      glVertex3f( point3X, point3Y, z1 );
                      glEnd();
#if defined(_WIN32) || defined(_WIN64)                      
                      glFlush(); // workaround for Win-32 BUG
#endif
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }  
}



void VectorRenderer::render( QPainter* painter,
                             const QRect& rect,
                             RenderingContextRSIGBase* context,
                             bool ) {

  if ( parent() && isVisible() ) {
  
    if ( context ) {
      const GeoDataObject* const par = qobject_cast<GeoDataObject*>( parent() );

      if ( par ) {
        myLut = par->getLut();
        assert( myLut );
        setContextColors( false, myLut, context );
      }
    }

    drawVectors( painter, rect, context );
  }
}


void VectorRenderer::renderDisplayPanel( QPainter* /* painter */,
                                         const QRect& /* rect */,
                                         RenderingContextRSIGBase* /* context */ ) {

  qDebug() << "in VectorRenderer::renderDisplayPanel - UNIMPLEMENTED";
}


void VectorRenderer::renderEMVL( QPainter*,
                                 const QRect&,
                                 bool ) {

  if ( parent() && isVisible() ) {
    const GeoDataObject* const par = qobject_cast<GeoDataObject*>( parent() );

    if ( par ) {
      myLut = par->getLut();
      assert( myLut );
      //const Vector2f minmax = this->minmaxVal(); // Use vis object minmax.
      QRect dummyRect = QRect();
      drawVectors( NULL, dummyRect, NULL );
    }
  }
}



qreal VectorRenderer::lineWidth() const {
  return mLineWidth;
}



void VectorRenderer::setLineWidth( qreal w ) {

  if ( w < 0 ) {
    w = 0;
  }

  mLineWidth = w;
  Q_EMIT valueChanged();
}



QColor VectorRenderer::lineColor() const {
  return mLineColor;
}



void VectorRenderer::setLineColor( const QColor& color ) {

  if ( mLineColor != color ) {
    mLineColor = color;
    Q_EMIT valueChanged();
  }
}



bool VectorRenderer::showNullSample() const {
  return mShowNullSample;
}



void VectorRenderer::setShowNullSample( bool flag ) {

  if ( mShowNullSample != flag ) {
    mShowNullSample = flag;
    Q_EMIT valueChanged();
  }
}



QString VectorRenderer::description() const {
  return "vector";
}



