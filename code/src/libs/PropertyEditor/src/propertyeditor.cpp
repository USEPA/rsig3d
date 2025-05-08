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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QMetaProperty>
#include <QPluginLoader>
#include <QDir>
#include <QMenu>
#include <QAction>
#include <QMetaClassInfo>
#include <QApplication>
#include <QKeyEvent>

#include "propertyeditor.h"
#include "propertyinterface.h"
#include "propertymodel.h"
#include "propertydelegate.h"
#include "qttrigger.h"


//#ifndef LIB_INSTALL_DIR
//#define LIB_INSTALL_DIR "../lib"
//#endif

namespace PropertyEditor
{
	
	PropertyEdit::PropertyEdit(QWidget *parent, bool useClassName, int useClassDepth): 
	    QTreeView(parent) 
	    ,m_model(NULL)
	    ,m_delegate(NULL)
	{	
		try{
			//not needed, since they are all static libs now
			//loadPlugins();
			//loadClasses();
			
			m_delegate = new PropertyDelegate(this);
			setItemDelegate(m_delegate);
			
			m_model = new PropertyModel(this, useClassName, useClassDepth);
			setModel(m_model);
			
			setExpandsOnDoubleClick(true);
			setAlternatingRowColors(true);
			setAnimated(false);
			setColumnWidth(0, 160);
			setIndentation(10);
			setSelectionMode(QTreeView::SingleSelection);
			setSelectionBehavior(QTreeView::SelectRows);
			setRootIsDecorated(true);
			setEditTriggers(QAbstractItemView::AllEditTriggers);
			
			//use context menu
			setContextMenuPolicy(Qt::CustomContextMenu);
			connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
		}
		catch (...){
			const char *msg = "Error: PropertyEdit constructor";
			qDebug() << msg;
			throw msg;
		}
	}
	
	/*
	 void PropertyEdit::loadClasses()
	 {
	 PropertyInterface* ptrbuffer[] ={
	 Bool::getInstance()
	 ,Strings::getInstance()
	 ,Numbers::getInstance()
	 ,Color::getInstance()
	 ,Rect::getInstance()
	 };
	 
	 for (int j=0; j<sizeof(ptrbuffer)/sizeof(ptrbuffer[0]); j++){
	 PropertyInterface* p = ptrbuffer[j];
	 int typebuffer[1024];
	 int len = p->supportedTypes(typebuffer);
	 for (int i=0; i<len; i++) m_plugins[typebuffer[i]] = p;
	 }
	*/	
	
	void PropertyEdit::loadPlugins()
	{
		/*
		 QDir pluginsDir = QDir(qApp->applicationDirPath());
		 #if defined(Q_OS_WIN)
		 if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		 pluginsDir.cdUp();
		 #elif defined(Q_OS_MAC)
		 if (pluginsDir.dirName() == "MacOS")
		 {
		 pluginsDir.cdUp();
		 pluginsDir.cd("PlugIns");
		 }
		 #else
		 pluginsDir.cd(LIB_INSTALL_DIR);
		 #endif
		 pluginsDir.cd("propertyEditor");
		 
		 foreach(QString fileName, pluginsDir.entryList(QDir::Files))
		 {
		 QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		 loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);
		 QObject *plugin = loader.instance();
		 if (plugin && dynamic_cast<PropertyInterface*>(plugin))
		 m_plugins.push_back(dynamic_cast<PropertyInterface*>(plugin));
		 else
		 qCritical() << plugin << loader.errorString();
		 }
		 */
	}
	
	PropertyEdit::~PropertyEdit()
	{
		delete m_model; m_model=0;
		delete m_delegate; m_delegate = 0;
	}
	
	void PropertyEdit::showContextMenu(const QPoint& localPos)
	{	
		/*
        const QModelIndex mindex = this->indexAt(localPos);	
		const QModelIndex topIndex = this->topLevelModelIndex(mindex);
		if (!topIndex.isValid()) return;
		
		const int objID = topIndex.row();
		qDebug() << "Object id is " << objID;
		
		QMenu myMenu(this);
		QAction action0(QString("Delete Current Object"), NULL);
		myMenu.addAction(&action0);
		
		const QPoint globalPos = this->mapToGlobal( localPos );
		QAction* selectedItem = myMenu.exec(globalPos);
		if (selectedItem == &action0){
			deleteObjectWithConfirmation(objID);					
			update();
		}
		*/
	}
	
	void PropertyEdit::setValidator(QVariant::Type type, PropertyValidator * validator)
	{
		m_validators[type]=validator;
	}
	
	PropertyValidator* PropertyEdit::validator(QVariant::Type type)
	{
		return m_validators[type];
	}
	
	void PropertyEdit::clearValidators()
	{
		m_validators.clear();
	}
	
	void PropertyEdit::addObject(QObject * object)
	{
		if (m_model==NULL || object==NULL) return;
		if (m_model->m_objects.indexOf(object)!=-1) return;
		
		m_model->addObject(object);
		this->setCurrentIndex( QModelIndex() );			
		Q_EMIT objectAdded(object);
	}
	
	void PropertyEdit::removeObject(QObject* object)
	{
		if (m_model==NULL || object==NULL) return;
		const int index = m_model->m_objects.indexOf(object);
		if (index == -1) return;
		
		m_model->deleteObject(object);
		Q_EMIT objectDeleted(object);
	}

	void PropertyEdit::removeObject(int id)
	{
		if (m_model==NULL) return;
		if (id<0 || id>= m_model->m_objects.size()) return;
		
		QObject * object = m_model->m_objects[id];		
		m_model->deleteObject(object);
		Q_EMIT objectDeleted(object);
	}
	
	void PropertyEdit::clear()
	{
		if (m_model){
			while (m_model->m_objects.size()!=0){
				QObject *obj = m_model->m_objects[0];
				m_model->deleteObject(obj);
				Q_EMIT objectDeleted(obj);
			}
			resetProperties();		
		}
		update();
	}
	
	QObject *PropertyEdit::object(int x) const
	{
		if (m_model==NULL) return NULL;
		if (x<0 || x>=m_model->m_objects.size()) return NULL;
		return m_model->m_objects[x];
	}
	
	void PropertyEdit::resetProperties()
	{
		if (m_model) m_model->reset();
	}
	
    void PropertyEdit::keyPressEvent ( QKeyEvent * event )
	{
		switch (event->key()){
			case Qt::Key_Delete:
			{
				const QModelIndex topIndex = topLevelModelIndex(this->currentIndex());
				if (topIndex.isValid()){
					const int id = topIndex.row();
					deleteObjectWithConfirmation(id);					
					update();
				}
			}
				break;
			default:
				break;
		};
	}
	
	void PropertyEdit::sortObjectsByName(int order)
	{
		if (m_model){
		    m_model->sortObjectsByName( order );
			update();
		}
	}
	
	QModelIndex PropertyEdit::getRowIndex(int level, int row)
	{		
		if (m_model){
			PropertyInterface* pi = m_model->m_properties[row].m_interface;
			return pi->m_properties[0].m_parentIndex;
		}
		return QModelIndex();
	}

    void PropertyEdit::deleteObjectWithConfirmation(int id)
	{
		QMessageBox msgBox(this);
		msgBox.addButton(QMessageBox::Yes);
		msgBox.addButton(QMessageBox::No);
		msgBox.setText("Do you want to delete the object?");
		int selection = msgBox.exec();
		if(selection == QMessageBox::Yes){
			this->removeObject(id);
		}		
	}
	
	QModelIndex PropertyEdit::topLevelModelIndex(const QModelIndex& mindex)
	{
		QModelIndex parentIndex = mindex;
		QModelIndex tmp = mindex.parent();
		while (tmp.isValid()){ 
			parentIndex = tmp;
			tmp = tmp.parent();
		}
		return parentIndex;
	}
	
	
	void PropertyEdit::setHeader0Name(const QString &name)
	{
		if (m_model) m_model->m_header0Name = name;
	}
	
	void PropertyEdit::setHeader1Name(const QString &name)
	{
		if (m_model) m_model->m_header1Name = name;		
	}
	
} //namespace PropertyEditor


void initPropertyEditClass(void)
{
	qRegisterMetaType<Trigger>("Trigger");
}

