/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2010 by Nan Zhang	                                    *
 *   nanzhang790@gmail.com                                                  *
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
#include <QMetaType>
#include <QPushButton>
#include "qttrigger.h"
#include "propertymodel.h"
#include <iostream>


Q_DECLARE_METATYPE(Trigger);

namespace PropertyEditor{
	
	
QTTrigger::QTTrigger(QObject* parent, QObject* object, int propertyid, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, propertyid, propertyModel)
{
	std::cout<<"constructor callled\n";
	
}

QTTrigger::~QTTrigger()
{
}
	
QWidget* QTTrigger::createEditor(QWidget * parent, const QModelIndex & index)
{
	std::cout<<"createEditor callled\n";
	Q_UNUSED(index);
	QPushButton *button = new QPushButton(parent);
	connect(button, SIGNAL(clicked()), this, SLOT(setValue()));
	return button;
}

void QTTrigger::setValue(void)
{
	QVariant var;
	var.setValue(Trigger());	
	std::cout<<"setValue callled\n";	
}
	
QVariant QTTrigger::data(const QModelIndex & index)
{
	if (object()==NULL) return QString();

	switch (index.column())
	{
		case 0:
			return objectPropertyName();			
		case 1:
			return QString();
	}
	return QVariant();
}

bool QTTrigger::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

bool QTTrigger::canHandle(QObject * object, int propertyID) const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);

	if (pro.typeName() == QString("Trigger"))
		return true;
	return false;
}	
	
Qt::ItemFlags QTTrigger::flags(const QModelIndex & index)
{
	const Qt::ItemFlags flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return flag;
}

PropertyInterface* QTTrigger::createInstance(QObject* parentptr, QObject * object, int propertyid, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new QTTrigger(parentptr, object, propertyid, propertyModel);
}

PropertyInterface* QTTrigger::getInstance(void)
{
	static QTTrigger var;
	return &var;
}


//Q_EXPORT_PLUGIN2(rect, Rect);
}//namespace PropertyEditor