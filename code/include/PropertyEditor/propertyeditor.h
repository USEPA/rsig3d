/***************************************************************************
 *   This file is part of the PropertyEditor project                       *
 *   Copyright (C) 2012 by Nan Zhang                                       *
 *   nanzhang790@gmail.com                                                 *
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#ifndef INC_PROPERTYEDITOR_H_
#define INC_PROPERTYEDITOR_H_

#include <QTreeView>
#include <QList>
#include <QPoint>


#define PROPERTYEDITOR_EXPORTS

#if _MSC_VER
	#ifdef LIB_EXPORTS
		#define PROPERTYEDITOR_EXPORTS __declspec(dllexport)
	#else
	//#define PROPERTYEDITOR_EXPORTS __declspec(dllimport)
	#endif
#else
	#define PROPERTYEDITOR_EXPORTS
#endif

namespace PropertyEditor
{

class PropertyValidator;
class PropertyInterface;
class PropertyModel;
class PropertyDelegate;
	
	
class PROPERTYEDITOR_EXPORTS PropertyEdit : public QTreeView
{
	Q_OBJECT
		
public:
	PropertyEdit(QWidget *parent, bool useClassName, int useClassDepth=1);
	~PropertyEdit();
	QObject *object(int indexid=0) const;
	void setValidator(QVariant::Type type, PropertyValidator * validator);
	PropertyValidator* validator(QVariant::Type type);
	void clearValidators();
	void clear();
	QModelIndex getRowIndex(int level, int row);
	
	void setHeader0Name(const QString &name);
	void setHeader1Name(const QString &name);
	
public Q_SLOTS:
	void addObject(QObject* object);
	void removeObject(QObject* object);
	void removeObject(int id);	
	void resetProperties();

Q_SIGNALS:
	void propertyChanged();
	void propertyChanged(QObject * obj, const QString & propertyName, const QVariant & old_value, const QVariant & new_value);
	void objectAdded(QObject * obj);
	void objectDeleted(QObject * obj);

protected Q_SLOTS:
	virtual void showContextMenu(const QPoint&);
	
protected:
    void keyPressEvent ( QKeyEvent * event );
    void deleteObjectWithConfirmation(int id);
	QModelIndex topLevelModelIndex(const QModelIndex& mindex);
	void sortObjectsByName(int order);
	
	PropertyModel* m_model;
	PropertyDelegate* m_delegate;
	QMap<int, PropertyValidator*> m_validators;
	
private:
	void loadPlugins();
};


}

#endif //INC_PROPERTYEDITOR_H_
