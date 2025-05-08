/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        * 
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      * 
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include <QMetaProperty>
#include <QMetaEnum>

#include "color.h"
#include "changecolor.h"
#include "propertyinterface.h"


namespace PropertyEditor
{

Color::Color(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Color::createEditor(QWidget * parent, const QModelIndex & index)
{
	ObjectInfo *infox = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	ChangeColor *cf = new ChangeColor(parent, infox);
	connect(cf, SIGNAL(colorChanged(ObjectInfo*, const QColor&)), this, SLOT(setValue(ObjectInfo*, const QColor&)));
	return cf;
}

void Color::setValue(ObjectInfo* info, const QColor& f)
{
	PropertyInterface::setValue(info, f);
}

void Color::setEditorData(QWidget * editor, const QModelIndex & index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	const QColor color = value(info).value<QColor>();
	dynamic_cast<ChangeColor *>(editor)->setColor(color);
}

PropertyInterface* Color::getInstance(void)
{
	static Color var;
	var.m_shared = true;
	return &var;
}
	
int Color::supportedTypes(int typebuffer[]) const
{
	typebuffer[0] = QVariant::Color;
	return 1;
}
	
	
//Q_EXPORT_PLUGIN2(color, Color);

}//namespace PropertyEditor
