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

#include <limits.h>
#include <float.h>
#include <QtCore>
#include <QMetaProperty>
#include <QMetaEnum>

#include <QDoubleSpinBox>
#include <QSpinBox>

#include "numbers.h"


namespace PropertyEditor
{
	
Numbers::Numbers(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
}

Numbers::~Numbers()
{
}

QWidget* Numbers::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	if (value().type() == QVariant::Double)
	{
		QDoubleSpinBox *dsb = new QDoubleSpinBox(parent);
		dsb->setMaximum(FLT_MAX);
		dsb->setMinimum(-FLT_MAX);
		dsb->setDecimals(3);
		dsb->setSingleStep(0.01);
		connect(dsb, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
		return dsb;
	}
	QSpinBox *sb = new QSpinBox(parent);
	sb->setMaximum(INT_MAX);
	sb->setMinimum(INT_MIN);
	connect(sb, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
	return sb;
}

void Numbers::setValue(int v)
{
	PropertyInterface::setValue(v);
}

void Numbers::setValue(double v)
{
	PropertyInterface::setValue(v);
}

QVariant Numbers::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || objectProperty()<0)
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

void Numbers::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	if (value().type() == QVariant::Double)
		dynamic_cast<QDoubleSpinBox *>(editor)->setValue(value().toDouble());
	else
		dynamic_cast<QSpinBox *>(editor)->setValue(value().toDouble());
}

bool Numbers::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

bool Numbers::canHandle(QObject * object, int propertyID)const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	if (pro.isEnumType() || pro.isFlagType())
		return false;

	switch (object->property(pro.name()).type())
	{
		case QVariant::Double:
		case QVariant::Int:
		case QVariant::LongLong:
		case QVariant::UInt:
		case QVariant::ULongLong:
			return true;
		default:
			return false;
	}
}

PropertyInterface* Numbers::createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new Numbers(parentptr, object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(numbers, Numbers);
}//namespace PropertyEditor
