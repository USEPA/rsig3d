/*
    This file is part of the Marble Virtual Globe.

    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2009 Thibaut GRIDEL <tgridel@free.fr>
*/

#include "GPXrteptTagHandler.h"

#include "MarbleDebug.h"

#include "GPXElementDictionary.h"
#include "GeoParser.h"
#include "GeoDataLineString.h"
#include "GeoDataCoordinates.h"
#include "GeoDataPlacemark.h"
#include "GeoDataFolder.h"
#include "GeoDataStyle.h"

namespace Marble
{
namespace gpx
{
GPX_DEFINE_TAG_HANDLER(rtept)

GeoNode* GPXrteptTagHandler::parse(GeoParser& parser) const
{
    Q_ASSERT(parser.isStartElement() && parser.isValidElement(gpxTag_rtept));

    GeoStackItem parentItem = parser.parentElement();
    if (parentItem.represents(gpxTag_rte))
    {
        GeoDataFolder* route = parentItem.nodeAs<GeoDataFolder>();
        GeoDataPlacemark* placemark = static_cast<GeoDataPlacemark*>(route->child(0));
        GeoDataLineString* linestring = static_cast<GeoDataLineString*>(placemark->geometry());
        GeoDataCoordinates coord;

        QXmlStreamAttributes attributes = parser.attributes();
        QStringRef tmp;
        qreal lat = 0;
        qreal lon = 0;
        tmp = attributes.value(gpxTag_lat);
        if ( !tmp.isEmpty() )
        {
            lat = tmp.toString().toFloat();
        }
        tmp = attributes.value(gpxTag_lon);
        if ( !tmp.isEmpty() )
        {
            lon = tmp.toString().toFloat();
        }
        coord.set(lon, lat, 0, GeoDataCoordinates::Degree);
        linestring->append(coord);
        
        // create a placemark too...
        GeoDataPlacemark* routepoint = new GeoDataPlacemark;
        routepoint->setCoordinate(coord);
        route->append(routepoint);
        routepoint->setStyleUrl("#map-routepoint");
        mDebug()<<routepoint->style()->styleId();
        

#ifdef DEBUG_TAGS
        mDebug() << "Parsed <" << gpxTag_rtept << "> waypoint: " << linestring->size()
                << coord.toString(GeoDataCoordinates::Decimal);
#endif
        return static_cast<GeoDataPlacemark*>(routepoint);
    }
    mDebug() << "rtept parsing with parentitem" << parentItem.qualifiedName();
    return 0;
}

} // namespace gpx

} // namespace Marble
