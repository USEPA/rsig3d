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

#include "pen.h"

namespace PropertyEditor
{

Pen::Pen(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
	refreshPropertis(parent, propertyModel);
}


QWidget* Pen::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant Pen::data(const QModelIndex & index)
{
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return value();
	}
	return QVariant();
}

bool Pen::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	Q_UNUSED(index);
	return false;
}

bool Pen::canHandle(QObject * object, int propertyID) const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	if (pro.isEnumType() || pro.isFlagType())
		return false;

	switch (object->property(pro.name()).type())
	{
		case QVariant::Pen:
			return true;
		default:
			return false;
	}

	return false;
}

Pen::PenStyle Pen::style()
{
	return (Pen::PenStyle)value().value<QPen>().style();
}

void Pen::setStyle(Pen::PenStyle style)
{
	QPen p = value().value<QPen>();
	p.setStyle((Qt::PenStyle)style);
	setValue(p);
}

Pen::PenCapStyle Pen::capStyle()
{
	return (Pen::PenCapStyle)value().value<QPen>().capStyle();
}

void Pen::setCapStyle(Pen::PenCapStyle capStyle)
{
	QPen p = value().value<QPen>();
	p.setCapStyle((Qt::PenCapStyle)capStyle);
	setValue(p);
}

Pen::PenJoinStyle Pen::joinStyle()
{
	return (Pen::PenJoinStyle)value().value<QPen>().joinStyle();
}

void Pen::setJoinStyle(Pen::PenJoinStyle joinStyle)
{
	QPen p = value().value<QPen>();
	p.setJoinStyle((Qt::PenJoinStyle)joinStyle);
	setValue(p);
}

qreal Pen::miterLimit()
{
	return value().value<QPen>().miterLimit();
}

void Pen::setMiterLimit(qreal miterLimit)
{
	QPen p = value().value<QPen>();
	p.setMiterLimit(miterLimit);
	setValue(p);
}

QColor Pen::color()
{
	return value().value<QPen>().color();
}

void Pen::setColor(const QColor & color)
{
	QPen p = value().value<QPen>();
	p.setColor(color);
	setValue(p);
}

QBrush Pen::brush()
{
	return value().value<QPen>().brush();
}

void Pen::setBrush(const QBrush & brush)
{
	QPen p = value().value<QPen>();
	p.setBrush(brush);
	setValue(p);
}

qreal Pen::width()
{
	return value().value<QPen>().widthF();
}

void Pen::setWidth(qreal width)
{
	QPen p = value().value<QPen>();
	p.setWidthF(width);
	setValue(p);
}


Qt::ItemFlags Pen::flags(const QModelIndex &/*index*/)
{
	return Qt::ItemIsEnabled;
}

PropertyInterface* Pen::createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new Pen(parentptr, object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(pen, Pen);
}//namespace PropertyEditor