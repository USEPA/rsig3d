/*
    Copyright (C) 2007 Nikolas Zimmermann <zimmermann@kde.org>

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

#include "DgmlItemTagHandler.h"

#include "MarbleDebug.h"

#include "DgmlElementDictionary.h"
#include "DgmlAttributeDictionary.h"
#include "DgmlAuxillaryDictionary.h"
#include "GeoParser.h"
#include "GeoSceneSection.h"
#include "GeoSceneItem.h"

namespace Marble
{
namespace dgml
{
void handler_init15(){
DGML_DEFINE_TAG_HANDLER(Item)
}

GeoNode* DgmlItemTagHandler::parse(GeoParser& parser) const
{
    // Check whether the tag is valid
    Q_ASSERT(parser.isStartElement() && parser.isValidElement(dgmlTag_Item));

    QString name      = parser.attribute(dgmlAttr_name).trimmed();
    QString checkable = parser.attribute(dgmlAttr_checkable).toLower().trimmed();
    QString connectTo = parser.attribute(dgmlAttr_connect).trimmed();
    int     spacing   = parser.attribute(dgmlAttr_spacing).toInt();

    GeoSceneItem *item = 0;

    // Checking for parent item
    GeoStackItem parentItem = parser.parentElement();
    if (parentItem.represents(dgmlTag_Section)) {
        item = new GeoSceneItem( name );
        item->setCheckable( checkable == dgmlValue_true || dgmlValue_on );
        item->setConnectTo( connectTo );
        item->setSpacing( spacing );
        parentItem.nodeAs<GeoSceneSection>()->addItem( item );
    }

    return item;
}

}
}
