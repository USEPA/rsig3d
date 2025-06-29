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
#include "sizef.h"


namespace PropertyEditor{

SizeF::SizeF(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
	refreshPropertis(parent, propertyModel);
}


QWidget* SizeF::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant SizeF::data(const QModelIndex & index)
{
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return QString("[%1,%2]").arg(value().toSizeF().width()).arg(value().toSizeF().height());
	}
	return QVariant();
}

bool SizeF::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	Q_UNUSED(index);
	return false;
}

bool SizeF::canHandle(QObject * object, int propertyID) const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	if (pro.isEnumType() || pro.isFlagType())
		return false;

	switch (object->property(pro.name()).type())
	{
		case QVariant::SizeF:
			return true;
		default:
			return false;
	}

	return false;
}

qreal SizeF::height()
{
	return value().toSizeF().height();
}

void SizeF::setHeight(qreal height)
{
	QSizeF s = value().toSizeF();
	s.setHeight(height);
	setValue(s);
}

qreal SizeF::width()
{
	return value().toSizeF().width();
}

void SizeF::setWidth(qreal width)
{
	QSizeF s = value().toSizeF();
	s.setWidth(width);
	setValue(s);
}

Qt::ItemFlags SizeF::flags(const QModelIndex &/*index*/)
{
	return Qt::ItemIsEnabled;
}

PropertyInterface* SizeF::createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new SizeF(parentptr, object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(sizef, SizeF);
}//namespace PropertyEditor