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

#include <iostream>
#include <QCheckBox>
#include "bool.h"

//#define CONTROLUI BoolSpinBox
//#define CONTROLUI BoolCheckBox
//#define CONTROLUI BoolComboBox

#if defined(__linux) && defined(__x86_64__)
    #define CONTROLUI BoolComboBox
#else
    #define CONTROLUI BoolCheckBox
#endif



namespace PropertyEditor
{

inline void initMyResource()
{
	//Q_INIT_RESOURCE(resource);
}

Bool::Bool(QObject* parent, QObject* object, int property, const PropertyModel* propertyModel): 
	PropertyInterface(parent, object, property, propertyModel)
{
	initMyResource();
}

QWidget* Bool::createEditor(QWidget* parent, const QModelIndex& index)
{
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	if (!info->isEditable()) return NULL;
	
	CONTROLUI *button = new CONTROLUI(parent, info);
	connect(button, SIGNAL(valueChanged(int, ObjectInfo*)), this, SLOT(setValue(int, ObjectInfo*)));
	return button;
 }

void Bool::setValue(int val, ObjectInfo *info)
{
	const bool checked = bool(val);
	PropertyInterface::setValue(info->m_object, info->m_property, checked);	
	
	//CONTROLUI *button = reinterpret_cast<CONTROLUI*> (m_pushButton);
	//if (button==NULL) return;
	//button->setText(QVariant(checked).toString());
	//button->setIcon(QIcon(":/" + QVariant(checked).toString() + ".png"));
}

void Bool::setEditorData(QWidget * editor, const QModelIndex& index)
{
	CONTROLUI *button = qobject_cast<CONTROLUI *>(editor);
	ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());
	const int val = (value(info->m_object, info->m_property).toBool()==false)?0:1;
	button->setChecked(val);	
}

PropertyInterface* Bool::getInstance(void)
{
	static Bool var;
	var.m_shared = true;
	return &var;
}

int Bool::supportedTypes(int typebuffer[]) const
{
	typebuffer[0] = QVariant::Bool;
	return 1;
}


//Q_EXPORT_PLUGIN2(boolean, Bool);
}//namespace PropertyEditor
	
