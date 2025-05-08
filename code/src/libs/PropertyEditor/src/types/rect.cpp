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
#include "rect.h"
#include <iostream>
#include "propertymodel.h"


namespace PropertyEditor{

Rect::Rect(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Rect::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(parent);
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	m_objectInfo = info;
	return 0;
}

QVariant Rect::data(const QModelIndex & index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	m_objectInfo = info;
	QObject *obj = info->m_object;
	const int propertyid = info->m_property;
	const QRect rect = value(info).toRect();
	switch (index.column())
	{
		case 0:
			return obj->metaObject()->property(propertyid).name();
		case 1:
			return QString("[(%1,%2) %3,%4]").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
	}
	return QVariant();
}

bool Rect::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	m_objectInfo = info;
	return false;
}

int Rect::x()
{
	return value(m_objectInfo).toRect().x();
}

void Rect::setX(int x)
{	
    QRect s = value(m_objectInfo).toRect();
    s.setX(x);
    setValue(m_objectInfo, s);
}

int Rect::y()
{
	return value(m_objectInfo).toRect().y();
}

void Rect::setY(int y)
{
	QRect s = value(m_objectInfo).toRect();
	s.setY(y);
	setValue(m_objectInfo, s);
}

int Rect::height()
{
	return value(m_objectInfo).toRect().height();
}

void Rect::setHeight(int height)
{
	QRect s = value(m_objectInfo).toRect();
	s.setHeight(height);
	setValue(m_objectInfo, s);
}

int Rect::width()
{
	return value(m_objectInfo).toRect().width();
}

void Rect::setWidth(int width)
{
	QRect s = value(m_objectInfo).toRect();
	s.setWidth(width);
	setValue(m_objectInfo, s);
}

Qt::ItemFlags Rect::flags(const QModelIndex &/*index*/)
{
	return Qt::ItemIsEnabled;
}

PropertyInterface* Rect::createInstance(QObject *parent, QObject *object, int property, const PropertyModel* propertyModel) const
{
	return new Rect(parent, object, property, propertyModel);
}

PropertyInterface* Rect::getInstance(void)
{
	static Rect var;
	var.m_shared = true;	
	return &var;
}

int Rect::supportedTypes(int typebuffer[]) const
{
	typebuffer[0] = QVariant::Rect;
	return 1;
}


//Q_EXPORT_PLUGIN2(rect, Rect);
}//namespace PropertyEditor