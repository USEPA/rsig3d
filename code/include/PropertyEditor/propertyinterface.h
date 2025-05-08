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

#ifndef PROPERTYEDITORPROPERTYINTERFACE_H
#define PROPERTYEDITORPROPERTYINTERFACE_H

#include <QObject>
#include <QVector>
#include <QVariant>
#include <QModelIndex>


#if _MSC_VER
#ifdef LIB_EXPORTS
//#define PROPERTYEDITOR_EXPORTS __declspec(dllexport)
#else
//#define PROPERTYEDITOR_EXPORTS __declspec(dllimport)
#endif
#else
#define PROPERTYEDITOR_EXPORTS
#endif

#include "propertyvalidator.h"

class QWidget;
class QMetaProperty;
class PropertyEdit;

namespace PropertyEditor
{

class PropertyModel;
class PropertyInterface;

struct ObjectInfo
{
public:
	ObjectInfo(); 
	bool isEditable(void) const;

public:	
	QObject* m_object;
	PropertyInterface * m_interface;
	int m_property;
	
	union{
		void * m_privatePtr;    //A private pointer used internally
        ObjectInfo * m_next;    //or, the next pointer in a linked list, used by memory management
	};
	
	int m_privateInt;

	QModelIndex m_parentIndex;
	bool m_editable;
	bool _tmp[3];
};


class PROPERTYEDITOR_EXPORTS PropertyInterface: public QObject
{
	Q_OBJECT

public:
	PropertyInterface(QObject *parent = 0, QObject * object = 0, int property = -1, const PropertyModel * propertyModel = 0);
	
	virtual ~PropertyInterface();
	
	bool isShared(void) const
	{
		return m_shared;
	}

	virtual int rowCount(const QModelIndex & parent) const;
	virtual int columnCount(const QModelIndex & parent);

	virtual QWidget* createEditor(QWidget * parent, const QModelIndex & index) = 0;

	virtual QVariant data(const QModelIndex & index);
	virtual bool setData(QVariant data, const QModelIndex & index);

	virtual void setEditorData(QWidget * editor, const QModelIndex & index) = 0;

	virtual QVariant value(QObject *obj, int propid) const;
	virtual bool setValue(QObject *obj, int propid, QVariant data);
	inline QVariant value(ObjectInfo *info) const
	{
		return value(info->m_object, info->m_property);
	}
	inline bool setValue(ObjectInfo *info, const QVariant& data)
	{
		return setValue(info->m_object, info->m_property, data);
	}
	
	PropertyValidator* validator(QVariant::Type type);

	virtual ObjectInfo* allocateProperty(QObject *obj, int propid);

	virtual Qt::ItemFlags flags(const QModelIndex &index);

	virtual int supportedTypes(int typebuffer[]) const = 0;

	virtual PropertyInterface* createInstance(QObject *parent, QObject *object, int property, const PropertyModel* propertyModel) const;

Q_SIGNALS:
	void propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value);

protected:
	friend class PropertyModel;
	friend class PropertyEdit;
	virtual int rowCount(void) const;

	QVector<ObjectInfo> m_properties;   //for children
	ObjectInfo* m_objectInfo;			//for itself
	const PropertyModel* m_propertyModel;
	bool m_shared;
};

}

Q_DECLARE_INTERFACE(PropertyEditor::PropertyInterface, "eu.licentia.PropertyEditor.PropetyInterface/1.0");

#endif
