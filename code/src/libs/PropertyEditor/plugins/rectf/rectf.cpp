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

#include "rectf.h"

namespace PropertyEditor
{

RectF::RectF(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
	refreshPropertis(parent, propertyModel);
}


QWidget* RectF::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant RectF::data(const QModelIndex & index)
{
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return QString("[(%1,%2) %3,%4]").arg(value().toRectF().x()).arg(value().toRectF().y()).arg(value().toRectF().width()).arg(value().toRectF().height());
	}
	return QVariant();
}

bool RectF::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	Q_UNUSED(index);
	return false;
}

bool RectF::canHandle(QObject * object, int propertyID) const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	if (pro.isEnumType() || pro.isFlagType())
		return false;

	switch (object->property(pro.name()).type())
	{
		case QVariant::RectF:
			return true;
		default:
			return false;
	}

	return false;
}

qreal RectF::x()
{
	return value().toRectF().x();
}

void RectF::setX(qreal x)
{
	QRectF s = value().toRectF();
	qreal w = s.width();
	s.setX(x);
	s.setWidth(w);
	setValue(s);
}

qreal RectF::y()
{
	return value().toRectF().y();
}

void RectF::setY(qreal y)
{
	QRectF s = value().toRectF();
	qreal h = s.height();
	s.setY(y);
	s.setHeight(h);
	setValue(s);
}


qreal RectF::height()
{
	return value().toRectF().height();
}

void RectF::setHeight(qreal height)
{
	QRectF s = value().toRectF();
	s.setHeight(height);
	setValue(s);
}

qreal RectF::width()
{
	return value().toRectF().width();
}

void RectF::setWidth(qreal width)
{
	QRectF s = value().toRectF();
	s.setWidth(width);
	setValue(s);
}

Qt::ItemFlags RectF::flags(const QModelIndex &/*index*/)
{
	return Qt::ItemIsEnabled;
}

PropertyInterface* RectF::createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new RectF(parentptr, object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(rect, RectF);

}//namespace PropertyEditor

