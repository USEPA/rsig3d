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

#include "date.h"

namespace PropertyEditor
{

	Date::Date(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
	{
	}
	
	QWidget* Date::createEditor(QWidget * parent, const QModelIndex & index)
	{
		ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		if (!info->isEditable()) return NULL;
		
		DateEdit *de = new DateEdit(parent, info);
		connect(de, SIGNAL(valueChanged(const QDate&, ObjectInfo*)), this, SLOT(setValue(const QDate&, ObjectInfo*)));
		return de;
	}
	
	void Date::setValue(const QDate& d, ObjectInfo* info)
	{
		PropertyInterface::setValue(info->m_object, info->m_property, d);
	}
	

	void Date::setEditorData(QWidget * editor, const QModelIndex & index)
	{
		DateEdit *button = qobject_cast<DateEdit *>(editor);
		if (button){
			button->setReadOnly(false); /// a bug in QT? has to be false
			button->setCalendarPopup(true);			
			ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
			const QDate val = value(info->m_object, info->m_property).toDate();
			button->setDate(val);
		}
	}
	
	PropertyInterface* Date::getInstance(void)
	{
		static Date var;
		var.m_shared = true;
		return &var;
	}
	
	int Date::supportedTypes(int typebuffer[]) const
	{
		typebuffer[0] = QVariant::Date;
		return 1;
	}
		
	
	//Q_EXPORT_PLUGIN2(date, DateTime);
}//namespace PropertyEditor

