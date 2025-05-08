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

#include <QDebug>
#include <QPainter>
#include <QCheckBox>
#include <QSpinBox>
#include <QModelIndex>
#include <QApplication>
#include <iostream>
#include "propertydelegate.h"
#include "propertyinterface.h"

namespace PropertyEditor
{

PropertyDelegate::PropertyDelegate(QObject *parent)
    :QItemDelegate(parent)
{
}


PropertyDelegate::~PropertyDelegate()
{
}

QWidget * PropertyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(option);
	if (index.isValid()){
		ObjectInfo *info = reinterpret_cast<ObjectInfo*>(index.internalPointer());;
		if (info && info->m_interface)
			return info->m_interface->createEditor(parent, index);
	}
	return 0;
}

void PropertyDelegate::paint(QPainter * painter, const QStyleOptionViewItem & opt, const QModelIndex & index) const
{
	bool paintcontent = true;
	bool drawline = true;
	QStyleOptionViewItem option = opt;
	const QPen oldPen = painter->pen();

	if (index.column()!=0){
		option.font.setBold(true);
	    ObjectInfo *ptr = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		PropertyInterface *pi = ptr->m_interface;
		if (pi==NULL){
            paintcontent=false;
        }
		else{
			const QVariant val = pi->value(ptr->m_object, ptr->m_property);
			if (val.type() == QVariant::Color){
				const QColor color = val.value<QColor>();
				//QRect colorRect = option.rect;
				//colorRect.setWidth(10);
				//painter->fillRect(colorRect, color);	
				option.palette.setColor(QPalette::Text, color);
				//option.palette.setColor(QPalette::Window, color );
				option.font.setUnderline(true);
			}
#if !defined(_MSC_VER) && !(defined(__x86_64__) && defined(__linux))
			else if (val.type() == QVariant::Bool){
				const QRect& r = option.rect;				
				static QCheckBox cbox;
				cbox.setAutoFillBackground(false);
				cbox.setAttribute(Qt::WA_TranslucentBackground);
				cbox.setChecked(val.toBool());
				cbox.resize(r.width(), r.height());
				//cbox.render(painter, QPoint(r.x(), r.y()), QRect(0, 0, r.width(), r.height()));
				cbox.render(painter, QPoint(r.x(), r.y()), QRect(0, 0, r.width(), r.height()));
				paintcontent = false;
			}
#endif
		}
	}
	else{
		QModelIndex parentIndex = index.parent();
		if (!parentIndex.isValid()){
			drawline = false;
			option.font.setBold(true);
			option.font.setKerning(true);
			option.font.setItalic(true);
		}
	}

	//draw the content
    if (paintcontent){
		QItemDelegate::paint(painter, option, index);
	}

	//draw vertical lines
	const QColor color = static_cast<QRgb>(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &option));
	painter->setPen(QPen(color));
	if (drawline){
		painter->drawLine(option.rect.right(), option.rect.y(), option.rect.right(), option.rect.bottom());
	}
	painter->drawLine(option.rect.x(), option.rect.bottom(), option.rect.right(), option.rect.bottom());	
	painter->setPen(oldPen);
	
}

void PropertyDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	if (index.isValid()){
	    ObjectInfo *ptr = reinterpret_cast<ObjectInfo*>(index.internalPointer());
		if (ptr->m_interface)
			ptr->m_interface->setEditorData(editor, index);
	}
}

QSize PropertyDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QItemDelegate::sizeHint(option, index) + QSize(2, 0);
}

}
