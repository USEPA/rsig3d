/*
    Copyright (C) 2008 Jens-Michael Hoffmann <jensmh@gmx.de>

    This file is part of the KDE project

    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include "osm-namefinder/PlaceTagHandler.h"

#include "MarbleDebug.h"

#include "GeoDataPlacemark.h"
#include "GeoParser.h"
#include "osm-namefinder/ElementDictionary.h"

namespace Marble
{
namespace OsmNamefinder
{

static GeoTagHandlerRegistrar
placeTagHandler( GeoParser::QualifiedName( tag_place, tag_namespace ),
         new PlaceTagHandler );


GeoNode * PlaceTagHandler::parse( GeoParser & parser ) const
{
    Q_ASSERT( parser.isStartElement() && parser.isValidElement( tag_place ));
    mDebug() << "PlaceTagHandler";

    GeoStackItem parentItem = parser.parentElement();
    GeoDataPlacemark * place = 0;
    if ( parentItem.represents( tag_named ))
        place = parentItem.nodeAs<GeoDataPlacemark>();
    return place;
}

}
}
