//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013	   Nan Zhang <zhang.nan@epa.gov>
//

#ifndef MARBLE_CONICALPROJECTIONPRIVATE_H
#define MARBLE_CONICALPROJECTIONPRIVATE_H

#include "AbstractProjection_p.h"


namespace Marble
{

class ConicalProjection;

class ConicalProjectionPrivate : public AbstractProjectionPrivate
{
  public:
    ConicalProjectionPrivate( ConicalProjection * parent );

    bool lineStringToPolygon( const GeoDataLineString &lineString,
                              const ViewportParams *viewport,
                              QVector<QPolygonF*> &polygons ) const;

    void repeatPolygons( const ViewportParams *viewport,
                         QVector<QPolygonF *> &polygons ) const;

    Q_DECLARE_PUBLIC( ConicalProjection )
};

} // namespace Marble

#endif
