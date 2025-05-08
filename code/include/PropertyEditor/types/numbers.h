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

#ifndef INC_NUMBERS_2012_H_
#define INC_NUMBERS_2012_H_

#include "propertyinterface.h"

#include <QDoubleSpinBox>
#include <QSpinBox>


namespace PropertyEditor
{

	class NumbersSpinBox: public QSpinBox
	{
		Q_OBJECT		
		
	public:
		NumbersSpinBox(QWidget *parent = 0, ObjectInfo* info=NULL): 
		QSpinBox(parent), mObjectInfo(info)
		{
			connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));				
		}
		
	Q_SIGNALS:
		void valueChanged(ObjectInfo *info, int v);		
		
	private Q_SLOTS:
        void onValueChanged(int v){  emit valueChanged(mObjectInfo, v); }
		
	private:
		ObjectInfo* mObjectInfo;		
	};

	
	class NumbersDSpinBox: public QDoubleSpinBox
	{
		Q_OBJECT		
		
	public:
		NumbersDSpinBox(QWidget *parent = 0, ObjectInfo* info=NULL): 
		QDoubleSpinBox(parent), mObjectInfo(info)
		{
			connect(this, SIGNAL(valueChanged(double)), this, SLOT(onValueChanged(double)));				
		}
		
	Q_SIGNALS:
		void valueChanged(ObjectInfo *info, double v);		
		
	private Q_SLOTS:
        void onValueChanged(double v){  emit valueChanged(mObjectInfo, v); }
		
	private:
		ObjectInfo* mObjectInfo;		
	};
	
	
class Numbers : public PropertyInterface
{
	Q_OBJECT
	//Q_INTERFACES(PropertyEditor::PropertyInterface);

public:
	Numbers(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);
	QWidget* createEditor(QWidget * parent, const QModelIndex & index);
	void setEditorData(QWidget * editor, const QModelIndex & index);

	static PropertyInterface* getInstance(void);
	int supportedTypes(int typebuffer[]) const;
	
private Q_SLOTS:
	void setValue(ObjectInfo* info, int);
	void setValue(ObjectInfo* info, double);

};

}
#endif //INC_NUMBERS_2012_H_
