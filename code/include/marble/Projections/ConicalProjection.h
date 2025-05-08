//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013		Nan Zhang	<zhang.nan@epa.gov>
//

#ifndef MARBLE_CONICALPROJECTION_H
#define MARBLE_CONICALPROJECTION_H

/** @file
 * This file contains the headers for ConicalProjection.
 *
 */

#include "AbstractProjection.h"

namespace Marble 
{

class ConicalProjectionPrivate;
class AbstractProjectionPrivate;

/**
 * @short A base class for the LambertConformalConic projections in Marble
 */

class ConicalProjection : public AbstractProjection
{
public:	
    ConicalProjection();

    virtual ~ConicalProjection();

    virtual bool repeatableX() const { return true; };

    virtual bool traversablePoles()  const { return false; }
    virtual bool traversableDateLine()  const { return false; }

    virtual SurfaceType surfaceType() const { return Conical; }

    virtual bool screenCoordinates( const GeoDataLineString &lineString,
                            const ViewportParams *viewport,
                            QVector<QPolygonF*> &polygons ) const;

    using AbstractProjection::screenCoordinates;

    virtual QPainterPath mapShape( const ViewportParams *viewport ) const;

protected: 
    ConicalProjection( ConicalProjectionPrivate &dd );

private:
    Q_DECLARE_PRIVATE( ConicalProjection )
    Q_DISABLE_COPY( ConicalProjection )
};

}

#endif

