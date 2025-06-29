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

#include <QtCore>
#include <QMetaProperty>
#include <QMetaEnum>

#include "changecolor.h"
#include "color.h"

namespace PropertyEditor
{

Color::Color(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Color::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	ChangeColor *cf = new ChangeColor(parent);
	connect(cf, SIGNAL(colorChanged(const QColor&)), this, SLOT(setValue(const QColor&)));
	return cf;
}

void Color::setValue(const QColor& f)
{
	PropertyInterface::setValue(f);
}

QVariant Color::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();
	switch (index.column())
	{
		case 0:
			return objectPropertyName();
		case 1:
			return value();
	}
	return QVariant();
}

bool Color::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void Color::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	QVariant f;
	dynamic_cast<ChangeColor *>(editor)->setColor(value().value<QColor>());
}

bool Color::canHandle(QObject * object, int propertyID)const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	if (pro.isEnumType() || pro.isFlagType())
		return false;

	if (pro.type() == QVariant::Color)
		return true;
	return false;
}

PropertyInterface* Color::createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new Color(parentptr, object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(color, Color);

}//namespace PropertyEditor
