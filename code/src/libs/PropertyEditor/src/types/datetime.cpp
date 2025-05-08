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

#include "datetime.h"

namespace PropertyEditor
{

	DateTime::DateTime(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
	{
	}
	
	QWidget* DateTime::createEditor(QWidget * parent, const QModelIndex & index)
	{
		ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		if (!info->isEditable()) return NULL;
		
		DateTimeEdit *de = new DateTimeEdit(parent, info);
		//de->setDateTime(value().toDateTime());
		connect(de, SIGNAL(valueChanged(const QDateTime&, ObjectInfo*)), this, SLOT(setValue(const QDateTime&, ObjectInfo*)));
		return de;
	}
	
	void DateTime::setValue(const QDateTime& d, ObjectInfo* info)
	{
		PropertyInterface::setValue(info->m_object, info->m_property, d);
		//PropertyInterface::setValue(d);
	}
	
	/*
QVariant DateTime::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();
	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return value().toDateTime().toString();
	}
	return QVariant();
}
	 */

	/*
bool DateTime::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}
	 */

	void DateTime::setEditorData(QWidget * editor, const QModelIndex & index)
	{
		DateTimeEdit *button = qobject_cast<DateTimeEdit *>(editor);
		if (button){
			//button->setAutoFillBackground(true);
			//button->setRange(0, 1);
			//button->setSingleStep(1);
			button->setReadOnly(false); /// a bug in QT? has to be false
			button->setCalendarPopup(true);
			
			ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
			const QDateTime val = value(info->m_object, info->m_property).toDateTime();
			button->setDateTime(val);
		}
	}
	
/*
bool DateTime::canHandle(QObject* object, int propertyID)const
{
    if (object==NULL) return false;
    const QMetaProperty pro = object->metaObject()->property(propertyID);
	if (pro.isEnumType() || pro.isFlagType())
		return false;

	switch (object->property(pro.name()).type())
	{
		case QVariant::DateTime:
			return true;
		default:
			return false;
	}
}
 */
/*
PropertyInterface* DateTime::createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const
{
	if (parentptr==NULL) parentptr=parent();
	return new DateTime(parentptr, object, property, propertyModel);
}
*/
	
	PropertyInterface* DateTime::getInstance(void)
	{
		static DateTime var;
		var.m_shared = true;
		return &var;
	}
	
	int DateTime::supportedTypes(int typebuffer[]) const
	{
		typebuffer[0] = QVariant::DateTime;
		return 1;
	}
		
	
	//Q_EXPORT_PLUGIN2(date, DateTime);
}//namespace PropertyEditor

