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

#ifndef INC_DATE_H_
#define INC_DATE_H_

#include <propertyinterface.h>
#include <QDate>
#include <QDateEdit>


namespace PropertyEditor
{
	
	class DateEdit: public QDateEdit
	{
		Q_OBJECT
		
	public:
		DateEdit(QWidget* parent, ObjectInfo *info):
		QDateEdit(parent), 
		mObjectInfo(info)
		{
			connect(this, SIGNAL(dateChanged(const QDate& )), this, SLOT(onDateChanged(const QDate&)));		
		}
		
		virtual ~DateEdit(){}
		
		
	Q_SIGNALS:
		void valueChanged(const QDate &datetime, ObjectInfo *info);
		
	private Q_SLOTS:
		void onDateChanged(const QDate& date)
		{
			emit valueChanged(date, mObjectInfo);
		}
		
	private:
		ObjectInfo* mObjectInfo;
	};
	
	
	class Date : public PropertyEditor::PropertyInterface
	{
		Q_OBJECT
		//Q_INTERFACES(PropertyEditor::PropertyInterface);
		
	public:
		Date(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);

		QWidget* createEditor(QWidget * parent, const QModelIndex & index);
		
		void setEditorData(QWidget * editor, const QModelIndex & index);
		
		static PropertyInterface* getInstance(void);
		
		int supportedTypes(int typebuffer[]) const;
		
	private Q_SLOTS:
		void setValue(const QDate& date, ObjectInfo* info);
	};
	
}//namespace PropertyEditor

#endif  //INC_DATETIME_H
