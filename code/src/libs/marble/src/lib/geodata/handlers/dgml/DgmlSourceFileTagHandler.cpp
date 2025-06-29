/*
    Copyright (C) 2007, 2008 Nikolas Zimmermann <zimmermann@kde.org>

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

#include "DgmlSourceFileTagHandler.h"

#include "DgmlElementDictionary.h"
#include "DgmlAttributeDictionary.h"
#include "GeoParser.h"
#include "GeoSceneVector.h"
#include "GeoSceneXmlDataSource.h"

namespace Marble
{
namespace dgml
{
void handler_init29(){
DGML_DEFINE_TAG_HANDLER(SourceFile)
}

GeoNode* DgmlSourceFileTagHandler::parse(GeoParser& parser) const
{
    // Check whether the tag is valid
    Q_ASSERT(parser.isStartElement() && parser.isValidElement(dgmlTag_SourceFile));

    QString format = parser.attribute(dgmlAttr_format).trimmed();

    // Checking for parent item
    GeoStackItem parentItem = parser.parentElement();
    if (parentItem.represents(dgmlTag_Vector)) {
        GeoSceneVector *vector = 0;

        vector = parentItem.nodeAs<GeoSceneVector>();
        vector->setSourceFile( parser.readElementText().trimmed() );
        vector->setFileFormat(format);
    } else if(parentItem.represents(dgmlTag_Geodata)) {
        GeoSceneXmlDataSource *dataSource = 0;
        dataSource = parentItem.nodeAs<GeoSceneXmlDataSource>();
        dataSource->setFilename( parser.readElementText().trimmed() );
        dataSource->setFileFormat(format);
    }

    return 0;
}

}
}
