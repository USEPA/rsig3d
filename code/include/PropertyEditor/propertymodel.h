/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2012 by Nan Zhang                                       *
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

#ifndef INC_PROPERTYMODEL_2012_H_
#define INC_PROPERTYMODEL_2012_H_

#include <QList>
#include <QString>
#include <QAbstractItemModel>
#include "propertyinterface.h"


namespace PropertyEditor
{

class PropertyModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	PropertyModel(QObject* parent, bool useClassName, int classDepth);

	~PropertyModel();

	void addObject(QObject * object);

	void deleteObject(QObject * object);

	void deleteAllObjects(void);	
	
	QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex & parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	QModelIndex parent(const QModelIndex & index) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	PropertyInterface* findPropertyInterfaceHandler(QObject *object, int propid) const;
	
protected:
	friend class PropertyEdit;
	void sortObjectsByName(int order=1);  //default is inc order: 1: inc, 0: dec
	
	QList<QObject*> m_objects;            //list of objects in the model
	QList<ObjectInfo> m_properties;       //list of properties in the model, a memory pool
	ObjectInfo * m_headOfPool;            //a pointer used by memory management
	QMap<int, PropertyInterface*> m_plugins;
	QString m_header0Name;  
	QString m_header1Name;  
	
    ObjectInfo *allocateSpace();
	void freeSpace(ObjectInfo *);

private:
	void createObjectProperty(QObject *obj);
	void loadInterfaceClasses(void);

private:
	int mUseClassDepth;
	bool mUseClassName;
	bool _pad[3];

};


}//namespace PropertyEditor


#endif //INC_PROPERTYMODEL_2012_H_
