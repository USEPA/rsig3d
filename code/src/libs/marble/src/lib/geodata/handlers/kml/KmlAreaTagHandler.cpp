/*
    Copyright (C) 2008 Patrick Spendrin <ps_ml@gmx.de>

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

#if KML_LAZY_IMP
#include "KmlAreaTagHandler.h"

#include "MarbleDebug.h"

#include "KmlElementDictionary.h"

#include "GeoDataPlacemark.h"
#include "GeoParser.h"

namespace Marble
{
namespace kml
{
KML_DEFINE_TAG_HANDLER( area )

GeoNode* KmlareaTagHandler::parse( GeoParser& parser ) const
{
    Q_ASSERT( parser.isStartElement() && parser.isValidElement( kmlTag_area ) );

    GeoStackItem parentItem = parser.parentElement();
    
    if( parentItem.is<GeoDataPlacemark>() ) {
        QString area = parser.readElementText().trimmed();
        qreal ar = area.toDouble();
        if( ar < 0.0 ) ar = 0.0;
        parentItem.nodeAs<GeoDataPlacemark>()->setArea( ar );
#ifdef DEBUG_TAGS
        mDebug() << "Parsed <" << kmlTag_area << "> containing: " << area
                 << " parent item name: " << parentItem.qualifiedName().first;
#endif // DEBUG_TAGS
    }

    return 0;
}

}
}

#endif //KML_LAZY_IMP
