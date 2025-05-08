/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2012 by Nan Zhang                                    *
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

#include <QSize>
#include <QFontMetrics>
#include <QMetaProperty>
#include <iostream>

#include "propertymodel.h"
#include "qtobject.h"
#include "qttrigger.h"

#include "types/bool.h"	
#include "types/color.h"	
#include "types/datetime.h"
#include "types/enums.h"	
#include "types/date.h"	
#include "types/numbers.h"	
#include "types/strings.h"	
#include "types/stringlist.h"	
#include "types/rect.h"	
#include "types/rectf.h"	
#include "types/flags.h"	


namespace PropertyEditor
{

void PropertyModel::loadInterfaceClasses(void)
{
	PropertyInterface* ptrbuffer[] = {
		Bool::getInstance()
		,Strings::getInstance()
		,Numbers::getInstance()
		,Color::getInstance()
		,Rect::getInstance()
		,RectF::getInstance()
		,Flags::getInstance()
		,DateTime::getInstance()
		,Date::getInstance()
	};
	
	for (unsigned int j=0; j<sizeof(ptrbuffer)/sizeof(ptrbuffer[0]); j++){
		PropertyInterface* p = ptrbuffer[j];
		p->m_propertyModel = this;
		p->m_shared = true;
		int typebuffer[1024];
		const int len = p->supportedTypes(typebuffer);
		for (int i=0; i<len; i++) m_plugins[typebuffer[i]] = p;			
	}		
}

PropertyModel::PropertyModel(QObject* parent, bool useClassName, int useClassDepth): 
	QAbstractItemModel(parent) 
	,m_objects() 
	,m_properties()
	,m_headOfPool(NULL)
	,m_plugins()
	,m_header0Name("Property")
	,m_header1Name("Value")  
	,mUseClassName(useClassName)
	,mUseClassDepth(useClassDepth)
{
	if (mUseClassDepth<1) mUseClassDepth = 1;
    
	loadInterfaceClasses();		
}
	
PropertyModel::~PropertyModel()
{
}
	
PropertyInterface* PropertyModel::findPropertyInterfaceHandler(QObject *obj, int propid) const
{
	PropertyInterface *pi = NULL;
	const QMetaProperty objprop = obj->metaObject()->property(propid);
	if (objprop.isFlagType()){
		pi = Flags::getInstance();
		return pi;
	}
	
	QVariant::Type objtype =objprop.type();
	QMap<int, PropertyInterface*>::const_iterator itr = m_plugins.find(objtype);
	if (itr != m_plugins.end()){
		pi = itr.value();
		if (pi->rowCount()>0){
			PropertyInterface *newpi = pi->createInstance(0, obj, propid, this);
			return newpi;
		}
	}
	return pi;
}

void PropertyModel::createObjectProperty(QObject *obj)
{
	const QMetaObject* metaObject = obj->metaObject();
	
	if (!mUseClassName){
		int start = 0;
		const int end = metaObject->propertyCount();
		for (int i=0; i<mUseClassDepth; i++){
			metaObject = metaObject->superClass();
			if (metaObject != NULL){
				start = metaObject->propertyCount();
			}
			else {
				start = 0;
				break;
			}
		}
		
		QTObject* pi = new QTObject(this, obj, start, end, mUseClassName, this);
		connect(pi, SIGNAL(propertyChanged(QObject*,QString,QVariant,QVariant)), 
				QObject::parent(), SIGNAL(propertyChanged(QObject*,QString,QVariant,QVariant)));
		
		ObjectInfo info;
		info.m_interface = pi;
		info.m_object = obj;
		m_properties.push_back(info);		
	}
	else{
		for (; metaObject!=NULL; metaObject=metaObject->superClass()){
			int start = 0;
			if (metaObject->superClass()!=NULL){
				start = metaObject->superClass()->propertyCount();
			}
			const int end = metaObject->propertyCount();
			//std::cout<<metaObject->className()<<" Start "<<start<<", End "<<end<<"\n";
			
			QTObject* pi = new QTObject(this, obj, start, end, mUseClassName, this);
			connect(pi, SIGNAL(propertyChanged(QObject*,QString,QVariant,QVariant)), 
					QObject::parent(), SIGNAL(propertyChanged(QObject*,QString,QVariant,QVariant)));
			
			ObjectInfo info;
			info.m_interface = pi;
			info.m_object = obj;
			m_properties.push_back(info);
		}
	}
}

void PropertyModel::addObject(QObject* object)
{
	m_objects.push_back(object);
	createObjectProperty(object);
	this->reset();
}

void PropertyModel::deleteObject(QObject* object)
{
	bool findptr = true;
	while (findptr){
		int index = -1;
		for (int i=0; i<m_properties.size(); i++){
			if (m_properties[i].m_object == object){
				index = i;
				break;
			}
		}
		if (index>=0 && index<m_properties.size()){
			PropertyInterface *&pi = m_properties[index].m_interface;
			if (pi != NULL && !pi->isShared()) pi->deleteLater();
			pi = NULL;
			m_properties.erase(m_properties.begin() + index);
		}
		else{
			findptr = false;
		}
	}

	if (1){
		const int index = m_objects.indexOf(object);
		if (index != -1){
			this->removeRow(index, QModelIndex());
			this->reset();
			m_objects.erase(m_objects.begin() + index);
		}
	}
}
	
void PropertyModel::deleteAllObjects(void)
{
    while (m_objects.size()!=0){
		QObject *obj = m_objects[0];
		deleteObject(obj);
	}
	this->reset();
}

QModelIndex PropertyModel::index(int row, int column, const QModelIndex & parent) const
{
	const QModelIndex nullindex;	
	const int nobjs = m_objects.size();
    if (nobjs == 0) return nullindex;	
	
	if (parent.isValid()){
		ObjectInfo *ptr = reinterpret_cast<ObjectInfo*>(parent.internalPointer());
		if (ptr->m_interface==NULL)
			return nullindex;
		
		if ((row==0 && column==0) || ptr->m_property==-1)
			ptr->m_editable = true; 
		else 
			ptr->m_editable = ptr->isEditable(); 
		
		ObjectInfo* info = ptr->m_interface->allocateProperty(ptr->m_object, row);
		if (info==NULL) return nullindex;
		info->m_parentIndex = parent;
		info->m_editable &= ptr->m_editable;
		
		return createIndex(row, column, info);
	}

	//top level widgets
	if (row<0 || row >= m_properties.size()) return nullindex;
	ObjectInfo* info = const_cast<ObjectInfo*>(&m_properties[row]);
	return createIndex(row, column, info);
}

int PropertyModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid()){
		ObjectInfo* info = reinterpret_cast<ObjectInfo*>(parent.internalPointer());
		PropertyInterface* pi = info->m_interface;
		if (pi==NULL) return 0;
		return pi->rowCount(parent);
	}
	return m_properties.size();
}

int PropertyModel::columnCount(const QModelIndex & parent) const
{
	if (parent.isValid()){
		ObjectInfo* info = reinterpret_cast<ObjectInfo*>(parent.internalPointer());
		PropertyInterface* pi = info->m_interface;
		if (pi==NULL) return 2;
		return pi->columnCount(parent);
	}
	return 2;
}

QVariant PropertyModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid() && Qt::DisplayRole == role){
		ObjectInfo* info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		PropertyInterface* pi = info->m_interface;
		if (pi==NULL) return QVariant();
		return pi->data(index);
	}

	if (Qt::SizeHintRole == role)
	    return QSize(0, QFontMetrics(QFont()).lineSpacing()+6);
	return QVariant();	
}

bool PropertyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (Qt::EditRole != role) return false;
	bool ret = false;
	if (index.isValid() && Qt::DisplayRole == role){
		ObjectInfo* info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		PropertyInterface* pi = info->m_interface;
		ret = pi->setData(value, index);
	}
	if (ret) emit dataChanged(index, index);
	return ret;
}

QModelIndex PropertyModel::parent(const QModelIndex & index) const
{
	if (index.isValid()){
		ObjectInfo* info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		return info->m_parentIndex;
	}
	return QModelIndex();
}

Qt::ItemFlags PropertyModel::flags(const QModelIndex &index) const
{
	if (m_objects.size() != 0 && index.isValid()){
		ObjectInfo* info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		if (info->m_interface==NULL) return Qt::NoItemFlags;
		return info->m_interface->flags(index);
	}
	return Qt::NoItemFlags;
}

QVariant PropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
		switch (section){
			case 0:
				return m_header0Name;  
			case 1:
				return m_header1Name;  
			default:
				return QVariant();
		}
	}

	return QVariant();
}
		
void PropertyModel::sortObjectsByName(int order)
{
	//clear all properties
	const int n = m_objects.size();
	QList<QObject *> objects = m_objects;
    this->deleteAllObjects();		
	
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			const QString si = objects[i]->objectName();
			const QString sj = objects[j]->objectName();
			bool swap = false;
			if (order>0 && si>sj){//increasing order
				swap = true;
			}
			else if (order<=0 && si<sj){
				swap = true;
			}
			if (swap){
				QObject *tmp = objects[i];
				objects[i] = objects[j];
				objects[j] = tmp;
			}
		}
	}
	
	for (int i=0; i<n; i++)
		this->addObject(objects[i]);
}
	
	ObjectInfo * PropertyModel::allocateSpace()
	{
		if (m_headOfPool == NULL){
			ObjectInfo * ptr = m_headOfPool;
			m_headOfPool = m_headOfPool->m_next;
			return ptr;
		}
		
		const int nsize = m_properties.size();
		m_properties.push_back(ObjectInfo());
		return &m_properties[nsize];
	}
	
	void PropertyModel::freeSpace(ObjectInfo *)
	{
		//not implemented yet! Re consider!!
	}
	
	

}//namespace PropertyEditor
