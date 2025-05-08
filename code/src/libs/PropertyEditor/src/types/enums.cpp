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

#include <QtCore>
#include <QMetaProperty>
#include <QMetaEnum>
#include <QComboBox>

#include "enums.h"

namespace PropertyEditor
{

	Enums::Enums(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
	{
	}
	
	QWidget* Enums::createEditor(QWidget * parent, const QModelIndex & index)
	{
		/*
		Q_UNUSED(index);
		cb = new QComboBox(parent);
		const QMetaObject *mo = object()->metaObject();
		for (int en = 0;en < mo->enumerator(mo->indexOfEnumerator(mo->property(objectProperty()).typeName())).keyCount();en++)
			cb->insertItem(en, mo->enumerator(mo->indexOfEnumerator(mo->property(objectProperty()).typeName())).key(en), mo->enumerator(mo->indexOfEnumerator(mo->property(objectProperty()).typeName())).value(en));
		
		cb->findData(value().toInt());
		connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
		return cb;
		*/
		
		ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		if (!info->isEditable()) return NULL;
		
		QComboBox *cb = new QComboBox(parent);
		
		const QMetaObject *mo = info->m_object->metaObject();
		//for (int en = 0;en < mo->enumerator(mo->indexOfEnumerator(mo->property(info->objectProperty()).typeName())).keyCount();en++)
		//	cb->insertItem(en, mo->enumerator(mo->indexOfEnumerator(mo->property(objectProperty()).typeName())).key(en), mo->enumerator(mo->indexOfEnumerator(mo->property(objectProperty()).typeName())).value(en));
		
		//cb->findData(value().toInt());
		
		connect(cb, SIGNAL(valueChanged(int, ObjectInfo*)), this, SLOT(setValue(int, ObjectInfo*)));
		return cb;
	}
	
	void Enums::setValue(int item, ObjectInfo* info)
	{
		//PropertyInterface::setValue(cb->itemData(item));
	}
	
	void Enums::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
	{
		//dynamic_cast<QComboBox *>(editor)->setCurrentIndex(dynamic_cast<QComboBox *>(editor)->findData(value().toInt()));
	}
	

	PropertyInterface* Enums::getInstance(void)
	{
		static Enums var;
		var.m_shared = true;
		return &var;
	}
	
	int Enums::supportedTypes(int typebuffer[]) const
	{
		typebuffer[0] = QVariant::Int;
		return 1;
	}
	
}//namespace PropertyEditor
