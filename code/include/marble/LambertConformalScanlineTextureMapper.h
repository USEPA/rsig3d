//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2007      Torsten Rahn     <tackat@kde.org>
// Copyright 2011      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

#ifndef MARBLE_LAMBERTCONFORMAL_SCANLINETEXTUREMAPPER_H
#define MARBLE_LAMBERTCONFORMAL_SCANLINETEXTUREMAPPER_H


#include "TextureMapperInterface.h"

#include "MarbleGlobal.h"

#include <QtCore/QThreadPool>
#include <QtGui/QImage>


namespace Marble
{

class StackedTileLoader;
class LambertConformalConicProjection;

/*
 * @short Texture mapping onto a sphere
 *
 * This class provides a fast way to map textures onto a sphere
 * without making use of hardware acceleration. 
 *
 * @author Torsten Rahn <rahn@kde.org>
 */

class LambertConformalScanlineTextureMapper : public TextureMapperInterface
{
 public:
    explicit LambertConformalScanlineTextureMapper( StackedTileLoader *tileLoader );

    virtual void mapTexture( GeoPainter *painter,
                             const ViewportParams *viewport,
                             const QRect &dirtyRect,
                             TextureColorizer *texColorizer );

    virtual void setRepaintNeeded();

 private:
    void mapTexture( const ViewportParams *viewport, MapQuality mapQuality );

 private:
    class RenderJob;
    StackedTileLoader *const m_tileLoader;
    bool m_repaintNeeded;
    int m_radius;
    QImage m_canvasImage;
    QThreadPool m_threadPool;
	const LambertConformalConicProjection *m_lambertProjection;
};

}

#endif //MARBLE_LAMBERTCONFORMAL_SCANLINETEXTUREMAPPER_H
