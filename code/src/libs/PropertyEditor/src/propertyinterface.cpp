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

#include "propertyinterface.h"
#include "propertymodel.h"
#include "propertyeditor.h"
#include <QMetaProperty>
#include <QMetaEnum>


namespace PropertyEditor
{
	
bool ObjectInfo::isEditable(void) const
{
	const QMetaProperty pro = this->m_object->metaObject()->property(this->m_property);
	const bool b0 = pro.isDesignable();
	const bool b1 = pro.isValid();
	const bool b2 = pro.isWritable();
	const bool b3 = !pro.isConstant();
	return m_editable && b0 && b1 && b2 && b3; 
}
	

ObjectInfo::ObjectInfo():
	m_object(NULL),
	m_interface(NULL),
	m_property(-1),
	m_privatePtr(0),
	m_privateInt(0),
	m_parentIndex(),
	m_editable(true)
{
}

//==============================================================	
	
PropertyInterface::PropertyInterface(QObject *parent , QObject * object, int property, const PropertyModel* pm):
    QObject(parent)
	,m_properties()
	,m_objectInfo(0)
    ,m_propertyModel(pm)
	,m_shared(false)
{
}

PropertyInterface::~PropertyInterface()
{
	const int n = m_properties.size();
	if (n==0) return;

	if (m_propertyModel){
		for (int i=0; i<n; i++){
			PropertyInterface *pi = m_properties[i].m_interface;
			if ((pi != NULL) && (pi->m_shared == false))
				delete pi;
		}
	}
	m_properties.clear();
}
	
ObjectInfo* PropertyInterface::allocateProperty(QObject *obj, int propid)
{
	const int n = rowCount();
	if (propid<0 || propid>=n) return NULL;

	if (m_properties.size()==0) m_properties.resize(n);
	ObjectInfo *info = &m_properties[propid];
	if (info->m_object==NULL){
		propid += this->metaObject()->superClass()->propertyCount();
		info->m_property = propid;
		info->m_interface = m_propertyModel->findPropertyInterfaceHandler(this, propid);
		info->m_object = this;
	}	
	return info;
}

QVariant PropertyInterface::data(const QModelIndex & index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());	
	QObject *obj = info->m_object;
	if (!index.isValid() || !obj || -1 == info->m_property)
		return QVariant();
	
	switch (index.column())
	{
		case 0:
			return obj->metaObject()->property(info->m_property).name();
		case 1:
			return value(info->m_object, info->m_property);
	}
	return QVariant();
}

	
bool PropertyInterface::setData(QVariant data, const QModelIndex & index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());	   		
	return setValue(info->m_object, info->m_property, data);
}
	
	
QVariant PropertyInterface::value(QObject *obj, int propid) const
{
	return obj->property(obj->metaObject()->property(propid).name());
}

bool PropertyInterface::setValue(QObject *object, int propid, QVariant data)
{
	if (object==NULL) return false;

	const char *strname = object->metaObject()->property(propid).name();
	QVariant old_data=object->property(strname);
	bool ret=object->setProperty(strname, data);
	if (ret) emit(propertyChanged(object, QString(strname), old_data, data));	
	return ret;
}

PropertyValidator* PropertyInterface::validator(QVariant::Type type)
{
	/*
	if (dynamic_cast<PropertyEdit*>(m_propertyModel->parent()))
		return dynamic_cast<PropertyEdit*>(m_propertyModel->parent())->validator(type);
		*/
	return NULL;
}

int PropertyInterface::rowCount(const QModelIndex & ) const
{	
	return rowCount();
}
	
	
int PropertyInterface::rowCount(void) const
{	
	const QMetaObject* mo = this->metaObject();
	return mo->propertyCount() - mo->superClass()->propertyCount();
}	
	

int PropertyInterface::columnCount(const QModelIndex & parent)
{
	return 2;
}

Qt::ItemFlags PropertyInterface::flags(const QModelIndex &index)
{
	if (index.column())
		return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
	else
		return Qt::ItemIsEnabled;
}

PropertyInterface* PropertyInterface::createInstance(QObject *parent, QObject *object, int property, const PropertyModel* propertyModel) const
{
	return NULL;
}
	
}

