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

#ifndef INC_BOOL_H__
#define INC_BOOL_H__

#include <QDebug>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QMouseEvent>
#include "propertyinterface.h"

class QVariant;
class QWidget;
class QCheckBox;
class QModelIndex;

namespace PropertyEditor{

	class BoolComboBox: public QComboBox
	{
		Q_OBJECT
		
	public:
		BoolComboBox(QWidget* parent, ObjectInfo *info):
		QComboBox(parent), 
		mObjectInfo(info)
		{
			this->setAutoFillBackground(true);			
			this->addItem("false");
			this->addItem("true");	
			connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onValueChanged(int)));				
		}
		
		virtual ~BoolComboBox(){}
		
		void setChecked(bool val)
		{			
            setCurrentIndex(val);
		}
		
	Q_SIGNALS:
		void valueChanged(int val, ObjectInfo *info);
		
	private Q_SLOTS:
		void onValueChanged(int val)
		{
			Q_EMIT valueChanged(val, mObjectInfo);
		}
		
	private:
		ObjectInfo* mObjectInfo;
	};
		
	
	class BoolCheckBox: public QCheckBox
	{
		Q_OBJECT
		
	public:
		BoolCheckBox(QWidget* parent, ObjectInfo *info):
	        QCheckBox(parent), 
	        mObjectInfo(info)
		{
#ifndef _MSC_VER
			this->setAutoFillBackground(false);
			this->setAttribute(Qt::WA_TranslucentBackground);
#else
			this->setAutoFillBackground(true);
#endif
			connect(this, SIGNAL(stateChanged(int)), this, SLOT(onValueChanged(int)));	
		}
		
		virtual ~BoolCheckBox(){}
		
	Q_SIGNALS:
		void valueChanged(int val, ObjectInfo *info);
		
	protected:
		void mousePressEvent ( QMouseEvent * event )
		{
			setChecked(!isChecked());
			event->ignore();
			//event->accept();
			update();
		}
		void mouseReleaseEvent ( QMouseEvent * event )
		{
			//event->accept();
			event->ignore();
		}
		
		void mouseMoveEvent(QMouseEvent *event)
		{
			//event->accept();
			event->ignore();
		}

		
	private Q_SLOTS:
		void onValueChanged(int val)
		{
			Q_EMIT valueChanged(val, mObjectInfo);
		}
		
	private:
		ObjectInfo* mObjectInfo;
	};
	

	class BoolSpinBox: public QSpinBox
	{
		Q_OBJECT
		
	public:
		BoolSpinBox(QWidget* parent, ObjectInfo *info):
	    QSpinBox(parent), 
	    mObjectInfo(info)
		{
			this->setAutoFillBackground(false);
			this->setRange(0, 1);
			this->setSingleStep(1);
			this->setReadOnly(false); /// a bug in QT? has to be false
			//this->setWrapping(true);
			connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));		
		}
		
		virtual ~BoolSpinBox(){}
		
		virtual QString textFromValue(int value) const
		{
			if (value==0) return QString("false");
			return QString("true");
		}
		
		virtual int valueFromText ( const QString & text ) const
		{
			int rtval= 1;
			if (text == QString("false")) rtval = 0;
			return rtval;
		}
		
		virtual QValidator::State validate ( QString & text, int & pos ) const
		{
			return QValidator::Acceptable;	
		}
		
		void setChecked(bool val)
		{
			setValue(val);
		}
		
		
	Q_SIGNALS:
		void valueChanged(int val, ObjectInfo *info);
		
	private Q_SLOTS:
		void onValueChanged(int val)
		{
			Q_EMIT valueChanged(val, mObjectInfo);
		}
		
	private:
		ObjectInfo* mObjectInfo;
	};
	
	
	class Bool : public PropertyEditor::PropertyInterface
	{
		Q_OBJECT
		//Q_INTERFACES(PropertyEditor::PropertyInterface);
		
	public:
		Bool(QObject* parent = 0, QObject* object = 0, int property = -1, const PropertyEditor::PropertyModel * propertyModel = 0);
		
		QWidget* createEditor(QWidget * parent, const QModelIndex & index);
		
		void setEditorData(QWidget* editor, const QModelIndex & index);
		
		static PropertyInterface* getInstance(void);
	
		int supportedTypes(int typebuffer[]) const;
		
	private Q_SLOTS:
		void setValue(int val, ObjectInfo* info);
		
	};
	
};	
	
#endif //INC_BOOL_H__
