//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Nan Zhang <zhang.nan@epa.gov>
//


#include "StereographicScanlineTextureMapper.h"

#include <cmath>

#include <QtCore/QRunnable>

#include "MarbleGlobal.h"
#include "GeoPainter.h"
#include "MarbleDebug.h"
#include "Quaternion.h"
#include "ScanlineTextureMapperContext.h"
#include "StackedTileLoader.h"
#include "TextureColorizer.h"
#include "ViewportParams.h"
#include "MathHelper.h"
#include "Projections/StereographicProjection.h"


using namespace Marble;

class StereographicScanlineTextureMapper::RenderJob : public QRunnable
{
public:
    RenderJob( StackedTileLoader *tileLoader, int tileLevel, QImage *canvasImage, const ViewportParams *viewport, MapQuality mapQuality, int yTop, int yBottom );

    virtual void run();

private:
    StackedTileLoader *const m_tileLoader;
    const int m_tileLevel;
    QImage *const m_canvasImage;
    const ViewportParams *const m_viewport;
    const MapQuality m_mapQuality;
    int const m_yTop;
    int const m_yBottom;
};

StereographicScanlineTextureMapper::RenderJob::RenderJob( StackedTileLoader *tileLoader, int tileLevel, QImage *canvasImage, const ViewportParams *viewport, MapQuality mapQuality, int yTop, int yBottom )
    : m_tileLoader( tileLoader ),
      m_tileLevel( tileLevel ),
      m_canvasImage( canvasImage ),
      m_viewport( viewport ),
      m_mapQuality( mapQuality ),
      m_yTop( yTop ),
      m_yBottom( yBottom )
{
}

StereographicScanlineTextureMapper::StereographicScanlineTextureMapper( StackedTileLoader *tileLoader )
    : TextureMapperInterface()
    , m_tileLoader( tileLoader )
    , m_repaintNeeded( true )
    , m_radius( 0 )
    , m_threadPool()
{
}

void StereographicScanlineTextureMapper::mapTexture( GeoPainter *painter,
                                                 const ViewportParams *viewport,
                                                 const QRect &dirtyRect,
                                                 TextureColorizer *texColorizer )
{
    if ( m_canvasImage.size() != viewport->size() || m_radius != viewport->radius() ) {
        const QImage::Format optimalFormat = ScanlineTextureMapperContext::optimalCanvasImageFormat( viewport );
        if ( m_canvasImage.size() != viewport->size() || m_canvasImage.format() != optimalFormat ) {
            m_canvasImage = QImage( viewport->size(), optimalFormat );
        }

        if ( !viewport->mapCoversViewport() ) {
            m_canvasImage.fill( -1L );
        }

        m_radius = viewport->radius();
        m_repaintNeeded = true;
    }

    if ( m_repaintNeeded ) {
        mapTexture( viewport, painter->mapQuality() );

        if ( texColorizer ) {
            texColorizer->colorize( &m_canvasImage, viewport, painter->mapQuality() );
        }

        m_repaintNeeded = false;
    }

    const QRect rect( 0, 0, viewport->width(), viewport->height());
    painter->drawImage( rect, m_canvasImage, rect );
}


void StereographicScanlineTextureMapper::setRepaintNeeded()
{
    m_repaintNeeded = true;
}


static const StereographicProjection *myProjection = 0;

void StereographicScanlineTextureMapper::mapTexture( const ViewportParams *viewport, MapQuality mapQuality )
{
    // Reset backend
    m_tileLoader->resetTilehash();

    // Initialize needed constants:
	m_stereographicProjection = dynamic_cast<const StereographicProjection*>(viewport->currentProjection());
	myProjection = m_stereographicProjection;
    if (m_stereographicProjection == NULL){
        qDebug()<<"Error: projection is NULL, StereographicScanlineTextureMapper is needed!";
		return;
	}
	
    const int imageHeight = m_canvasImage.height();
    const int yTop = 0; 
    const int yBottom = imageHeight;

    const int numThreads = m_threadPool.maxThreadCount();
	int yStep = ( yBottom - yTop ) / numThreads;
	if ( (yBottom - yTop)%numThreads != 0 ) yStep++;
    for ( int i = 0; i < numThreads; ++i ) {
        const int yStart = yTop + i * yStep;
		int yEnd = yTop + (i + 1) * yStep;
		if (yEnd > yBottom) yEnd = yBottom;
        QRunnable *const job = new RenderJob( m_tileLoader, tileZoomLevel(), &m_canvasImage, viewport, mapQuality, yStart, yEnd );
        m_threadPool.start( job );
    }

    m_threadPool.waitForDone();
    m_tileLoader->cleanupTilehash();
}

void StereographicScanlineTextureMapper::RenderJob::run()
{
    const int imageWidth  = m_canvasImage->width();

    // initialize needed variables that are modified during texture mapping:
    ScanlineTextureMapperContext context( m_tileLoader, m_tileLevel );
    const qreal minlat = myProjection->minLat();
    const qreal maxlat = myProjection->maxLat();

    // Scanline based algorithm to texture map a sphere
    const QRgb background= QColor(255, 255, 255).rgb();

    for ( int y = m_yTop; y < m_yBottom ; ++y ) {
        const int xLeft  = 0; 
        const int xRight = imageWidth; 
        QRgb * scanLine = (QRgb*)( m_canvasImage->scanLine( y ) ) + xLeft;

        for ( int x = xLeft; x < xRight; ++x,++scanLine ) {
			qreal lon, lat;
            myProjection->geoCoordinates(x, y, m_viewport, lon, lat, GeoDataCoordinates::Radian);
            if (lat >maxlat || lat < minlat
                //|| lon<centerLon0 || lon>centerLon1
                ){
                *scanLine = background;
            }
            else
                context.pixelValue( lon, lat, scanLine );
        }
    }
}
