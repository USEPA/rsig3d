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

#include <QLineEdit>
#include <QComboBox>
#include "strings.h"
#include <iostream>


const static QString strTrigger("${trigger}");


namespace PropertyEditor{


Strings::Strings(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
	,mStrList(NULL)
	,mID(-1)
{
}

QWidget* Strings::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	
	const QString strval = value().toString();
    if (strval== strTrigger){
		//TriggerLabel* button = new TriggerLabel(parent);
		//button->setText("<<trigger>>");
		//connect(button, SIGNAL(triggered()), this, SLOT(setValue()));	
		PropertyInterface::setValue(QString());
		return NULL;// button;
	}
	
	if (mStrList!=NULL){
		QComboBox *box = new QComboBox(parent);
		if (validator(QVariant::String))
		{
			validator(QVariant::String)->setObject(object());
			validator(QVariant::String)->setProperty(objectPropertyName());
			box->setValidator(validator(QVariant::String));
		}
		connect(box, SIGNAL(highlighted(const QString&)), this, SLOT(setValue(const QString&)));	
		return box;
	}
    else{		
		QLineEdit *le = new QLineEdit(parent);
		if (validator(QVariant::String))
		{
			validator(QVariant::String)->setObject(object());
			validator(QVariant::String)->setProperty(objectPropertyName());
			le->setValidator(validator(QVariant::String));
		}
		connect(le, SIGNAL(textChanged(const QString&)), this, SLOT(setValue(const QString&)));
		return le;		
	}
	
	return NULL;
}

void Strings::setValue(const QString& s)
{
	PropertyInterface::setValue(s);
}
	
void Strings::setValue(void)
{
	PropertyInterface::setValue(strTrigger);
}
	
QVariant Strings::data(const QModelIndex & index)
{
	QVariant v;
	
	if (!index.isValid() || !object() || -1 == objectProperty())
		return v;

	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
		{
			v = value();
			if (v.toString()==strTrigger)
				return QString("<trigger>");
			QStringList *ptr;
			int id;
			if (sscanf(v.toString().toAscii().data(), "${%p:%d}", &ptr, &id)==2){
				mStrList = ptr;
				mID = id;
				QVariant empty;
				PropertyInterface::setValue(empty);
				return empty;
			}
			return v;
		}	
	}
				
	return v;
}

bool Strings::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void Strings::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	const QString strval = value().toString();
	QLineEdit* le = dynamic_cast<QLineEdit *>(editor);
	if (le != NULL){
		le->setText(strval);
		return;
	}

	QComboBox* box = dynamic_cast<QComboBox *>(editor);
	if (box != NULL && mStrList != NULL){
		QStringList::iterator itr = mStrList->begin();
		int itemIndex = -1;		
		for (int i=0; itr!=mStrList->end(); itr++){
			char sbuffer[4096];
			int id;
			if (sscanf((*itr).toAscii().data(), "%s %d", sbuffer, &id)==2){
		        if (mID<id){
					const QString qname(sbuffer);
					box->addItem(qname);
					if ((itemIndex==-1) && (strval==qname)) itemIndex = i; 
					i++;
				}
			}
		}
		box->setCurrentIndex(itemIndex);
		return;
	}
}

bool Strings::canHandle(QObject * object, int propertyID) const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	
	if (pro.isEnumType() || pro.isFlagType()) return false;
	
	if (pro.type() == QVariant::String)
		return true;
	return false;
}

PropertyInterface* Strings::createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new Strings(parentptr, object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(strings, Strings);
	
}
