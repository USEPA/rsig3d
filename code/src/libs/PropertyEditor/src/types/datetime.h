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

#ifndef INC_DATETIME_H_
#define INC_DATETIME_H_

#include <propertyinterface.h>
#include <QDateTime>
#include <QDateTimeEdit>


namespace PropertyEditor
{
	
	class DateTimeEdit: public QDateTimeEdit
	{
		Q_OBJECT
		
	public:
		DateTimeEdit(QWidget* parent, ObjectInfo *info):
		QDateTimeEdit(parent), 
		mObjectInfo(info)
		{
			connect(this, SIGNAL(dateTimeChanged(const QDateTime& )), this, SLOT(onDateTimeChanged(const QDateTime&)));		
		}
		
		virtual ~DateTimeEdit(){}
		
		
	Q_SIGNALS:
		void valueChanged(const QDateTime &datetime, ObjectInfo *info);
		
	private Q_SLOTS:
		void onDateTimeChanged(const QDateTime& datetime)
		{
			emit valueChanged(datetime, mObjectInfo);
		}
		
	private:
		ObjectInfo* mObjectInfo;
	};
	
	
	class DateTime : public PropertyEditor::PropertyInterface
	{
		Q_OBJECT
		//Q_INTERFACES(PropertyEditor::PropertyInterface);
		
	public:
		DateTime(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);
		
		QWidget* createEditor(QWidget * parent, const QModelIndex & index);
		
		void setEditorData(QWidget * editor, const QModelIndex & index);
		
		static PropertyInterface* getInstance(void);
		
		int supportedTypes(int typebuffer[]) const;
		
	private Q_SLOTS:
		void setValue(const QDateTime& datetime, ObjectInfo* info);
	};
	
}//namespace PropertyEditor

#endif  //INC_DATETIME_H
