//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008      David Roberts  <dvdr18@gmail.com>
// Copyright 2008      Inge Wallin    <inge@lysator.liu.se>
// Copyright 2010      Harshit Jain   <hjain.itbhu@gmail.com>
//

// Own
#include "SunControlWidget.h"
#include "ui_SunControlWidget.h"

// Qt
#include <QtGui/QShowEvent>

// Marble
#include "MarbleDebug.h"
#include "MarbleWidget.h"

using namespace Marble;
/* TRANSLATOR Marble::SunControlWidget */

SunControlWidget::SunControlWidget( MarbleWidget* marbleWidget, QWidget* parent )
    : QDialog( parent ),
      m_uiWidget( new Ui::SunControlWidget ),
      m_marbleWidget( marbleWidget ),
      m_shadow( "shadow" )
{
    m_uiWidget->setupUi( this );
	
    connect( m_uiWidget->applyButton, SIGNAL( clicked() ), this, SLOT( apply() ) );
    connect( m_uiWidget->cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( m_uiWidget->okButton, SIGNAL( clicked() ), this, SLOT( apply() ) );
    connect( m_uiWidget->okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    
    setModal( false );

}

SunControlWidget::~SunControlWidget()
{
    delete m_uiWidget;
}

void SunControlWidget::apply()
{
    if( m_uiWidget->sunShading->isChecked() )
    {
        if( m_uiWidget->showShadow->isChecked() )
        {
            emit showSun( true );
            m_marbleWidget->setShowCityLights( false );
            m_shadow = "shadow";
        }
        else if( m_uiWidget->showNightMap->isChecked() )
        {
            emit showSun( true );
            m_marbleWidget->setShowCityLights( true );
            m_shadow = "nightmap";
        }
    }
    else
    {
        emit showSun( false );
        m_marbleWidget->setShowCityLights( false );
    }

    if( m_uiWidget->showZenith->isChecked() )
    {
        m_marbleWidget->setShowSunInZenith( true );
        emit showSunInZenith( true );
    }
    else if( m_uiWidget->hideZenith->isChecked() )
    {
        m_marbleWidget->setShowSunInZenith( false );
        emit showSunInZenith( false );
    }
}

void SunControlWidget::setSunShading( bool active )
{
    m_uiWidget->sunShading->setChecked( active );
}

void SunControlWidget::showEvent( QShowEvent* event )
{
    if( !event->spontaneous() ) 
    {
        // Loading all options
        if( m_marbleWidget->showSunShading() )
        {
            m_uiWidget->sunShading->setChecked( true );
            m_uiWidget->showShadow->setChecked( m_marbleWidget->showSunShading() );
            m_uiWidget->showNightMap->setChecked( m_marbleWidget->showCityLights() );
        }
        else
        {   
            m_uiWidget->showShadow->setChecked( false );
            if( m_shadow == "shadow" )
            {
                m_uiWidget->showShadow->setChecked( true );
            }
            else
            {
                m_uiWidget->showNightMap->setChecked( true );
            }
        }
        m_uiWidget->showZenith->setChecked( m_marbleWidget->showSunInZenith() );
        m_uiWidget->hideZenith->setChecked( !m_marbleWidget->showSunInZenith() );
    }
}

#include "SunControlWidget.moc"
