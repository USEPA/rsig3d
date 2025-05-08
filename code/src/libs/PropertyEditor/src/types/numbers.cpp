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

#include "numbers.h"
#include <limits.h>
#include <float.h>


namespace PropertyEditor
{
	
Numbers::Numbers(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
}

QWidget* Numbers::createEditor(QWidget * parent, const QModelIndex & index)
{
 	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	if (!info->isEditable()) return NULL;
	if (value(info).type() == QVariant::Double)
	{
		NumbersDSpinBox *dsb = new NumbersDSpinBox(parent, info);
		dsb->setMaximum(FLT_MAX);
		dsb->setMinimum(-FLT_MAX);
		dsb->setDecimals(3);
		dsb->setSingleStep(0.01);
		connect(dsb, SIGNAL(valueChanged(ObjectInfo*, double)), this, SLOT(setValue(ObjectInfo*, double)));
		return dsb;
	}
	
	NumbersSpinBox *sb = new NumbersSpinBox(parent, info);
	sb->setMaximum(INT_MAX);
	sb->setMinimum(INT_MIN);
	connect(sb, SIGNAL(valueChanged(ObjectInfo*, int)), this, SLOT(setValue(ObjectInfo*, int)));
	return sb;
}

void Numbers::setValue(ObjectInfo* info, int v)
{
	emit propertyChanged(info->m_object, QString(), QVariant(), QVariant());
	PropertyInterface::setValue(info, v);
}

void Numbers::setValue(ObjectInfo* info, double v)
{
	emit propertyChanged(info->m_object, QString(), QVariant(), QVariant());	
	PropertyInterface::setValue(info, v);
}

void Numbers::setEditorData(QWidget * editor, const QModelIndex & index)
{
	ObjectInfo* info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	const QVariant v = value(info);

	NumbersDSpinBox *dsb = dynamic_cast<NumbersDSpinBox *>(editor);
	if (dsb){
		dsb->setValue(v.toDouble()); return;
	}

	NumbersSpinBox* sb = dynamic_cast<NumbersSpinBox *>(editor);
	if (sb){
		sb->setValue(v.toInt()); return;
	}
}

PropertyInterface* Numbers::getInstance(void)
{
	static Numbers var;
	var.m_shared = true;
	return &var;
}
	
int Numbers::supportedTypes(int typebuffer[]) const
{
	const int buffer[] = {
		QVariant::Double, 
		QVariant::Int, 
		QVariant::LongLong, 
		QVariant::UInt, 
		QVariant::ULongLong
	};
	
	const int len = sizeof(buffer)/sizeof(buffer[0]);
	for (int i=0; i<len; i++) typebuffer[i]=buffer[i];
	return len;
}
	
	
//Q_EXPORT_PLUGIN2(numbers, Numbers);
}//namespace PropertyEditor
