//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2009      Bastian Holst <bastianholst@gmx.de>
//

#include "KmlMinAltitudeTagHandler.h"

#include "MarbleDebug.h"

#include "KmlElementDictionary.h"
#include "GeoDataLatLonAltBox.h"
#include "GeoParser.h"

namespace Marble
{
namespace kml
{
KML_DEFINE_TAG_HANDLER( minAltitude )

GeoNode* KmlminAltitudeTagHandler::parse( GeoParser& parser ) const
{
    Q_ASSERT( parser.isStartElement() && parser.isValidElement( kmlTag_minAltitude ) );

    GeoStackItem parentItem = parser.parentElement();

    if( parentItem.represents( kmlTag_LatLonAltBox ) ) {
        float minAltitude = parser.readElementText().trimmed().toFloat();

        parentItem.nodeAs<GeoDataLatLonAltBox>()->setMinAltitude( minAltitude );
#ifdef DEBUG_TAGS
        mDebug() << "Parsed <" << kmlTag_minAltitude << "> containing: " << minAltitude
                 << " parent item name: " << parentItem.qualifiedName().first;
#endif // DEBUG_TAGS
    }

    return 0;
}

}
}
