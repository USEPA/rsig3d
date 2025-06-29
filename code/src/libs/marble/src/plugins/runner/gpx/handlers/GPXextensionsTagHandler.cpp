//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Niko Sams <niko.sams@gmail.com>
//

#include "GPXextensionsTagHandler.h"

#include "MarbleDebug.h"

#include "GPXElementDictionary.h"
#include "GeoParser.h"
#include "GeoDataDocument.h"
#include "GeoDataPlacemark.h"
#include "GeoDataPoint.h"
#include "GeoDataTrack.h"
#include "GeoDataExtendedData.h"

namespace Marble
{
namespace gpx
{
GPX_DEFINE_TAG_HANDLER(extensions)

GeoNode* GPXextensionsTagHandler::parse( GeoParser& parser ) const
{
    Q_ASSERT( parser.isStartElement() && parser.isValidElement( gpxTag_extensions ) );
    GeoStackItem parentItem = parser.parentElement();

    if ( parentItem.represents( gpxTag_trkpt ) )
    {
        GeoDataTrack* track = parentItem.nodeAs<GeoDataTrack>();
#ifdef DEBUG_TAGS
        mDebug() << "Parsed <" << gpxTag_extensions << ">";
#endif
        return track;
    }

    mDebug() << "extensions parsing with parentitem" << parentItem.qualifiedName();
    return 0;
}

} // namespace gpx

} // namespace Marble
