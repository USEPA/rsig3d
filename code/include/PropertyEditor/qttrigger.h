/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2012 by Nan Zhang                                   *
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

#ifndef INC_QTTRIGGER_2012_H_
#define INC_QTTRIGGER_2012_H_

#include "propertyinterface.h"


class Trigger
{
public:	
	Trigger(const char * name = NULL):
	mName()
	{
		if (name) mName = name;
	}
	
	Trigger(const QString& name):
	mName(name)
	{}
	
	~Trigger(){}
	
	
	QString name(void){ return mName; }
	
	void setName(const QString& name)
	{
		mName = name;
	}
	
	Trigger& operator = (const Trigger& obj)
	{
		mName = obj.mName;
		return *this;
	}
	
	operator QString()
	{
		return mName;
	}

	operator int()
	{
		return 0;
	}	
	
private:
	QString mName;
	
};


namespace PropertyEditor
{
	
class QTTrigger : public PropertyInterface
{
	Q_OBJECT
	Q_INTERFACES(PropertyEditor::PropertyInterface);

public:
	QTTrigger(QObject* parent = 0, QObject* object = 0, int propertyID=-1, const PropertyModel * propertyModel = 0);
	~QTTrigger();

	QWidget* createEditor(QWidget * parent, const QModelIndex & index);

	void setValue(void);
	
	QVariant data(const QModelIndex & index);
	bool setData(QVariant data, const QModelIndex & index);
	void setEditorData(QWidget *, const QModelIndex & ){};

	bool canHandle(QObject * object, int propid)const;

	Qt::ItemFlags flags(const QModelIndex &index);

	PropertyInterface* createInstance(QObject* parentptr, QObject * object, int property, const PropertyModel * propertyModel) const;

	PropertyInterface* getInstance(void);

protected:
}; 

}//namespace PropertyEditor

#endif //INC_QTTRIGGER_2012_H_
