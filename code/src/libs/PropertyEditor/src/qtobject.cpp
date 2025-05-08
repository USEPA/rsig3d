/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2010 by Nan Zhang	                                    *
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

#include <iostream>
#include <QMetaProperty>
#include <QMetaEnum>
#include "qtobject.h"
#include "propertymodel.h"


namespace PropertyEditor{

QTObject::QTObject(QObject* parent, QObject* object, int propertyStart, int propertyEnd, bool useclassname, const PropertyModel* propertyModel):
	PropertyInterface(parent, object, -1, propertyModel)
	,m_object(object)
	,mPropertyStart(propertyStart)
	,mPropertyEnd(propertyEnd)
	,mUseClassName(useclassname)
{
}

QTObject::~QTObject()
{
	//std::cout<<"QTObject deleted!\n";
}

inline int _rowCount(int istart, int iend)
{
	return (iend - istart);
}
	
int QTObject::rowCount(const QModelIndex & parent) const
{
	return _rowCount(mPropertyStart, mPropertyEnd);
}

QWidget* QTObject::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	Q_UNUSED(parent);
	return 0;
}

QVariant QTObject::data(const QModelIndex & index)
{
	ObjectInfo* info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	if (info->m_object != m_object){
		std::cout << "Error in ObjectInfo->m_object pointer!\n";
		return QVariant();
	}
	
	if (m_object==NULL) return QString();
	const char *classname = "";
	const QMetaObject *metaObj = m_object->metaObject();

	switch (index.column())
	{
		case 0:
			if (mUseClassName){
				while (metaObj!=NULL){
					const QMetaObject* par = metaObj->superClass();
					if (metaObj->propertyCount()==mPropertyEnd){
						if (par==NULL || (par!=NULL && par->propertyCount()==mPropertyStart)){
							classname = metaObj->className();							
							break;
						}
				    }
					metaObj = par;
				}
				return QString(classname);
			}
			return m_object->objectName();

		case 1:
			return QString();
	}
	return QVariant();
}

bool QTObject::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(data);
	Q_UNUSED(index);
	return false;
}

Qt::ItemFlags QTObject::flags(const QModelIndex & index)
{
	const Qt::ItemFlags flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return flag;
}

PropertyInterface* QTObject::getInstance(void)
{
	static QTObject var;
	var.m_shared = true;
	return &var;
}

ObjectInfo* QTObject::allocateProperty(QObject *obj, int propid)
{
	const int n = _rowCount(mPropertyStart, mPropertyEnd);
	if (propid<0 || propid>=n) return NULL;

	if (m_properties.size()==0) m_properties.resize(n);
	ObjectInfo *info = &m_properties[propid];
	if (info->m_object==NULL){
		info->m_property = (propid+=mPropertyStart);
		info->m_interface = m_propertyModel->findPropertyInterfaceHandler(obj, propid);
		connect(info->m_interface, SIGNAL(propertyChanged(QObject*, const QString&, const QVariant&, const QVariant&)), 
				this, SIGNAL(propertyChanged(QObject*, const QString&, const QVariant&, const QVariant&)));
		info->m_object = obj;
	}
	return info;
}

//Q_EXPORT_PLUGIN2(rect, Rect);
}//namespace PropertyEditor