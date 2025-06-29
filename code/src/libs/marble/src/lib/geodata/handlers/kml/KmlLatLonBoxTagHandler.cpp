//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012      Dennis Nienhüser <earthwings@gentoo.org>
//

#include "KmlLatLonBoxTagHandler.h"

#include "MarbleDebug.h"

#include "KmlElementDictionary.h"
#include "GeoParser.h"
#include "GeoDataLatLonBox.h"
#include "GeoDataGroundOverlay.h"

namespace Marble
{
namespace kml
{
KML_DEFINE_TAG_HANDLER( LatLonBox )

GeoNode* KmlLatLonBoxTagHandler::parse( GeoParser& parser ) const
{
    Q_ASSERT( parser.isStartElement() && parser.isValidElement( kmlTag_LatLonBox ) );

    GeoDataLatLonBox box;

    GeoStackItem parentItem = parser.parentElement();
#ifdef DEBUG_TAGS
    mDebug() << "Parsed <" << kmlTag_LatLonBox << ">"
             << " parent item name: " << parentItem.qualifiedName().first;
#endif

    if( parentItem.represents( kmlTag_GroundOverlay ) )
    {
        parentItem.nodeAs<GeoDataGroundOverlay>()->setLatLonBox( box );
        return &parentItem.nodeAs<GeoDataGroundOverlay>()->latLonBox();
    } else {
        return 0;
    }
}

}
}
