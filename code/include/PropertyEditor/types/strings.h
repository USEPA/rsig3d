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
#include <QComboBox>
#include <QLineEdit>

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
		TriggerLabel(QWidget *parent = 0, ObjectInfo* info=NULL): QLabel(parent), mObjectInfo(info){}
				
	Q_SIGNALS:
		void triggered(ObjectInfo *info);		

	protected:
		virtual void mousePressEvent ( QMouseEvent * event ){ emit triggered(mObjectInfo);}
		
	private:
		ObjectInfo* mObjectInfo;
	};


	class StringsComboBox: public QComboBox
	{
		Q_OBJECT		
		
	public:
		StringsComboBox(QWidget *parent = 0, ObjectInfo* info=NULL): QComboBox(parent), mObjectInfo(info)
		{
			connect(this, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onCurrentIndexChanged(const QString&)));				
		}
	
	Q_SIGNALS:
		void currentIndexChanged(const QString &s, ObjectInfo *info);		
				
	private Q_SLOTS:
        void onCurrentIndexChanged(const QString& s)
		{
			if (s==QString() || s.isNull()) return;
			QString t(s);
			for (int i=0; i<t.size(); i++)
				if (t[i]=='(') t[i]='\0';
			emit currentIndexChanged(t, mObjectInfo); 
		}
		
	private:
		ObjectInfo* mObjectInfo;		
	};
	

	class StringsLineEdit: public QLineEdit
	{
		Q_OBJECT		
		
	public:
		StringsLineEdit(QWidget *parent = 0, ObjectInfo* info=NULL): QLineEdit(parent), mObjectInfo(info)
		{
			connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));				
		}
		
	Q_SIGNALS:
		void textChanged(const QString &s, ObjectInfo *info);		
		
	private Q_SLOTS:
        void onTextChanged(const QString& s){  emit textChanged(s, mObjectInfo); }
		
	private:
		ObjectInfo* mObjectInfo;		
	};
	

class Strings : public PropertyInterface
{
	Q_OBJECT
	//Q_INTERFACES(PropertyEditor::PropertyInterface);

public:
	Strings(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyModel * propertyModel = 0);
	QWidget* createEditor(QWidget * parent, const QModelIndex & index);
	QVariant data(const QModelIndex & index);
	void setEditorData(QWidget * editor, const QModelIndex & index);

	static PropertyInterface* getInstance(void);
	int supportedTypes(int typebuffer[]) const;

private Q_SLOTS:
	void setValue(const QString& val, ObjectInfo *info);	
	
};
	
}//namespace PropertyEditor
#endif //INC_STRINGS_H_
