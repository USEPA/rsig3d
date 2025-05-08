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

#ifndef INC_STRINGS_H_
#define INC_STRINGS_H_

#include <QPushButton>
#include <QLabel>
#include "propertyinterface.h"
#include <iostream>

class QStringList;

/**
	@author BogDan Vatra <bogdan@licentia.eu>
*/
namespace PropertyEditor
{

	class TriggerLabel: public QLabel
	{
		
		Q_OBJECT		
	public:
		TriggerLabel(QWidget *parent = 0): QLabel(parent)
		{}
		
	protected:
		virtual void mousePressEvent ( QMouseEvent * event )
		{
			emit triggered();
		}
		
	signals:
		void triggered(void);		
	
	};
	

class Strings : public PropertyInterface
{
	Q_OBJECT
	Q_INTERFACES(PropertyEditor::PropertyInterface);

public:
	Strings(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);
	QWidget* createEditor(QWidget * parent, const QModelIndex & index);
	QVariant data(const QModelIndex & index);
	bool setData(QVariant data, const QModelIndex & index);
	void setEditorData(QWidget * editor, const QModelIndex & index);
	bool canHandle(QObject * object, int propid) const;
	PropertyInterface* createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const;

public slots:
	void setValue(const QString& s);
	void setValue();
	
private:
	//the data strucutre is for selecting a string from a stringlist
	//where the stringlist is embeded in the string var as "${%p:%d}",
	//the %p is a pointer to a static memory location, the %d is a user information item 
	QStringList* mStrList;
	int mID;
	
};
	
}//namespace PropertyEditor
#endif //INC_STRINGS_H_
