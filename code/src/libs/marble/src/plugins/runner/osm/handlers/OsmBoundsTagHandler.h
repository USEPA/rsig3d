//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011      Konstantin Oblaukhov <oblaukhov.konstantin@gmail.com>
//

#ifndef MARBLE_OSMBOUNDSTAGHANDLER_H
#define MARBLE_OSMBOUNDSTAGHANDLER_H

#include "GeoTagHandler.h"
#include "marble_export.h"

namespace Marble
{
namespace osm
{

class OsmBoundsTagHandler : public GeoTagHandler
{
public:
    virtual GeoNode* parse( GeoParser& ) const;
};

}

}

#endif // MARBLE_OSMBOUNDSTAGHANDLER_H
