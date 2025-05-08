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
#include <QPushButton>
#include <QSpinBox>
#include <iostream>
#include "bool.h"


#define CONTROLUI BoolSpinBox

namespace PropertyEditor
{

class BoolSpinBox: public QSpinBox
{
public:
	BoolSpinBox(QWidget* parent):
	    QSpinBox(parent)
	{}
	
	virtual ~BoolSpinBox(){}
	
	virtual QString textFromValue(int value) const
	{
		if (value==0) return QString("false");
		return QString("true");
	}
	
	virtual int valueFromText ( const QString & text ) const
	{
		int rtval= 1;
		if (text == QString("false")) rtval = 0;
		return rtval;
	}
		
	virtual QValidator::State validate ( QString & text, int & pos ) const
	{
		return QValidator::Acceptable;	
	}		
};

//###########################################################

inline void initMyResource()
{
	//Q_INIT_RESOURCE(resource);
}


Bool::Bool(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
	initMyResource();
}


QWidget* Bool::createEditor(QWidget* parent, const QModelIndex& index)
{
	Q_UNUSED(index);
	CONTROLUI *button = new CONTROLUI(parent);
	connect(button, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
	return button;
}


void Bool::setValue(int val)
{
	this->setValue(bool(val));
}

void Bool::setValue(bool checked)
{
	PropertyInterface::setValue(checked);	
	//CONTROLUI *button = reinterpret_cast<CONTROLUI*> (m_pushButton);
	//if (button==NULL) return;
	//button->setText(QVariant(checked).toString());
	//button->setIcon(QIcon(":/" + QVariant(checked).toString() + ".png"));
}


QVariant Bool::data(const QModelIndex & index)
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


bool Bool::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}


void Bool::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	CONTROLUI *button = dynamic_cast<CONTROLUI *>(editor);
	button->setRange(0, 1);
	button->setSingleStep(1);
	button->setWrapping(true);
	const int val = (value().toBool()==false)?0:1;
	button->setValue(val);
	button->setReadOnly(false); /// a bug in QT, has to be false
}


bool Bool::canHandle(QObject* object, int propertyID) const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	if (pro.type() == QVariant::Bool)
		return true;
	return false;
}


PropertyInterface* Bool::createInstance(
	QObject* parentptr, QObject* object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new Bool(parentptr, object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(boolean, Bool);
}//namespace PropertyEditor
	