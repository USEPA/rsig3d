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

#include "strings.h"
#include <iostream>


const static QString strTrigger("${trigger}");


namespace PropertyEditor{


Strings::Strings(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Strings::createEditor(QWidget * parent, const QModelIndex & index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());	
	if (!info->isEditable()) return NULL;
	const QString strval = value(info).toString();
	
    if (strval== strTrigger){
		PropertyInterface::setValue(info, QString());
		return NULL;
	}
	
	QStringList * strList = reinterpret_cast<QStringList*> (info->m_privatePtr);
	if (strList!=NULL){
		StringsComboBox *box = new StringsComboBox(parent, info);
		if (validator(QVariant::String))
		{
			//validator(QVariant::String)->setObject(object);
			//validator(QVariant::String)->setProperty(objectPropertyName());
			//box->setValidator(validator(QVariant::String));
		}
		connect(box, SIGNAL(currentIndexChanged(const QString&, ObjectInfo*)), this, SLOT(setValue(const QString&, ObjectInfo*)));	
		return box;
	}
    else{		
		StringsLineEdit *le = new StringsLineEdit(parent, info);
		if (validator(QVariant::String))
		{
			//validator(QVariant::String)->setObject(object());
			//validator(QVariant::String)->setProperty(objectPropertyName());
			//le->setValidator(validator(QVariant::String));
		}
		connect(le, SIGNAL(textChanged(const QString&, ObjectInfo*)), this, SLOT(setValue(const QString&, ObjectInfo*)));
		return le;		
	}
	
	return NULL;
}

void Strings::setValue(const QString& val, ObjectInfo *info)
{
	PropertyInterface::setValue(info, val);
}
	
	
QVariant Strings::data(const QModelIndex & index)
{	
	QVariant v;
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	
	if (!index.isValid() || info->m_object==NULL || info->m_property==-1)
		return v;

	switch (index.column())
	{
		case 0:
			return info->m_object->metaObject()->property(info->m_property).name();
		case 1:
		{
			v = value(info);
			if (v.toString()==strTrigger)
				return QString("<trigger>");
			QStringList *ptr;
			int id;
			if (sscanf(v.toString().toAscii().data(), "${%p:%d}", &ptr, &id)==2){
				info->m_privatePtr = ptr;
				QVariant empty;
				PropertyInterface::setValue(info, empty);
				return empty;
			}
			return v;
		}	
	}
				
	return v;
}


void Strings::setEditorData(QWidget * editor, const QModelIndex & index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());	
	const QString strval = value(info->m_object, info->m_property).toString();
	QLineEdit* le = qobject_cast<QLineEdit *>(editor);
	if (le != NULL){
		le->setText(strval);
		return;
	}

	QComboBox* box = qobject_cast<QComboBox *>(editor);
	QStringList* strList = reinterpret_cast<QStringList*> (info->m_privatePtr);	
	if (box != NULL && strList != NULL){
		const QStringList& strlist = *strList;
		int itemIndex = -1;		
		box->addItem(QString(""));
		for (int i=0; i<strlist.size(); i++){
			char sbuffer[4096];
			int id;
			sscanf(strlist[i].toAscii().data(), "%s %d", sbuffer, &id);
			QString qname(sbuffer);
			qname+="(";
			qname+=QString().setNum(id);
			qname+=")";
			box->addItem(qname);
			if ((itemIndex==-1) && (strval==sbuffer)) itemIndex = i; 
		}
		if (itemIndex!=-1) itemIndex++;
		box->setCurrentIndex(itemIndex);
	}
}

PropertyInterface* Strings::getInstance(void)
{
	static Strings var;
	var.m_shared = true;
	return &var;
}

int Strings::supportedTypes(int typebuffer[]) const
{
	typebuffer[0] = QVariant::String;
	return 1;
}


//Q_EXPORT_PLUGIN2(strings, Strings);	
}

