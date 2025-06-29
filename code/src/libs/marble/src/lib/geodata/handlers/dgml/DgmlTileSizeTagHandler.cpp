// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011      Dennis Nienhüser <earthwings@gentoo.org>
//

#include "DgmlTileSizeTagHandler.h"

#include "DgmlAttributeDictionary.h"
#include "DgmlElementDictionary.h"
#include "GeoParser.h"
#include "GeoSceneTexture.h"

namespace Marble
{
namespace dgml
{
void handler_init35(){
DGML_DEFINE_TAG_HANDLER(TileSize)
}

GeoNode* DgmlTileSizeTagHandler::parse( GeoParser& parser ) const
{
    // Check whether the tag is valid
    Q_ASSERT( parser.isStartElement() && parser.isValidElement( dgmlTag_TileSize ));

    // Checking for parent item
    GeoStackItem parentItem = parser.parentElement();
    if ( !parentItem.represents( dgmlTag_Texture ))
        return 0;

    int width = parser.attribute(dgmlAttr_width).toInt();
    int height = parser.attribute(dgmlAttr_height).toInt();
    QSize const size( width, height );
    if ( !size.isEmpty() ) {
        parentItem.nodeAs<GeoSceneTexture>()->setTileSize( size );
    }

    return 0;
}

}
}
