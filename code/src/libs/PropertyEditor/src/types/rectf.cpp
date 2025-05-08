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
#include "rectf.h"
#include <iostream>
#include "propertymodel.h"


namespace PropertyEditor{

RectF::RectF(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* RectF::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(parent);
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	m_objectInfo = info;
	return 0;
}

QVariant RectF::data(const QModelIndex & index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	m_objectInfo = info;
	QObject *obj = info->m_object;
	const int propertyid = info->m_property;
	const QRectF rect = value(info).toRectF();
	switch (index.column())
	{
		case 0:
			return obj->metaObject()->property(propertyid).name();
		case 1:
			return QString("[(%1,%2) %3,%4]").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
	}
	return QVariant();
}

bool RectF::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	m_objectInfo = info;
	return false;
}

qreal RectF::x()
{
	return value(m_objectInfo).toRectF().x();
}

void RectF::setX(qreal x)
{	
    QRectF s = value(m_objectInfo).toRectF();
    s.setX(x);
    setValue(m_objectInfo, s);
}

qreal RectF::y()
{
	return value(m_objectInfo).toRectF().y();
}

void RectF::setY(qreal y)
{
	QRectF s = value(m_objectInfo).toRectF();
	s.setY(y);
	setValue(m_objectInfo, s);
}

qreal RectF::height()
{
	return value(m_objectInfo).toRectF().height();
}

void RectF::setHeight(qreal height)
{
	QRectF s = value(m_objectInfo).toRectF();
	s.setHeight(height);
	setValue(m_objectInfo, s);
}

qreal RectF::width()
{
	return value(m_objectInfo).toRectF().width();
}

void RectF::setWidth(qreal width)
{
	QRectF s = value(m_objectInfo).toRectF();
	s.setWidth(width);
	setValue(m_objectInfo, s);
}

Qt::ItemFlags RectF::flags(const QModelIndex &/*index*/)
{
	return Qt::ItemIsEnabled;
}

PropertyInterface* RectF::createInstance(QObject *parent, QObject *object, int property, const PropertyModel* propertyModel) const
{
	return new RectF(parent, object, property, propertyModel);
}

PropertyInterface* RectF::getInstance(void)
{
	static RectF var;
	var.m_shared = true;	
	return &var;
}

int RectF::supportedTypes(int typebuffer[]) const
{
	typebuffer[0] = QVariant::RectF;
	return 1;
}


//Q_EXPORT_PLUGIN2(rect, Rect);
}//namespace PropertyEditor