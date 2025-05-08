/********************************************************************************
** Form generated from reading UI file 'NavigationWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAVIGATIONWIDGET_H
#define UI_NAVIGATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "MarbleSearchListView.h"

QT_BEGIN_NAMESPACE

class Ui_NavigationWidget
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *m_splitter;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *boundedCheckBox;
    MarbleSearchListView *locationListView;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *spacerItem;
    QGridLayout *_3;
    QToolButton *zoomOutButton;
    QToolButton *goHomeButton;
    QToolButton *moveRightButton;
    QToolButton *moveUpButton;
    QSlider *zoomSlider;
    QToolButton *zoomInButton;
    QToolButton *moveDownButton;
    QToolButton *moveLeftButton;
    QSpacerItem *spacerItem1;
    QFrame *m_pSpacerFrame;

    void setupUi(QWidget *NavigationWidget)
    {
        if (NavigationWidget->objectName().isEmpty())
            NavigationWidget->setObjectName(QString::fromUtf8("NavigationWidget"));
        NavigationWidget->resize(137, 446);
        verticalLayout = new QVBoxLayout(NavigationWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_splitter = new QSplitter(NavigationWidget);
        m_splitter->setObjectName(QString::fromUtf8("m_splitter"));
        m_splitter->setOrientation(Qt::Vertical);
        verticalLayoutWidget = new QWidget(m_splitter);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        boundedCheckBox = new QCheckBox(verticalLayoutWidget);
        boundedCheckBox->setObjectName(QString::fromUtf8("boundedCheckBox"));

        verticalLayout_2->addWidget(boundedCheckBox);

        locationListView = new MarbleSearchListView(verticalLayoutWidget);
        locationListView->setObjectName(QString::fromUtf8("locationListView"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(locationListView->sizePolicy().hasHeightForWidth());
        locationListView->setSizePolicy(sizePolicy);
        locationListView->setMinimumSize(QSize(0, 75));
        locationListView->setFocusPolicy(Qt::WheelFocus);
        locationListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        locationListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        locationListView->setAlternatingRowColors(false);
        locationListView->setIconSize(QSize(16, 16));
        locationListView->setLayoutMode(QListView::Batched);
        locationListView->setViewMode(QListView::ListMode);
        locationListView->setUniformItemSizes(true);

        verticalLayout_2->addWidget(locationListView);

        m_splitter->addWidget(verticalLayoutWidget);
        layoutWidget = new QWidget(m_splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(5, 5, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacerItem);

        _3 = new QGridLayout();
#ifndef Q_OS_MAC
        _3->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        _3->setContentsMargins(0, 0, 0, 0);
#endif
        _3->setObjectName(QString::fromUtf8("_3"));
        zoomOutButton = new QToolButton(layoutWidget);
        zoomOutButton->setObjectName(QString::fromUtf8("zoomOutButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/zoom-out.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomOutButton->setIcon(icon);

        _3->addWidget(zoomOutButton, 5, 1, 1, 1);

        goHomeButton = new QToolButton(layoutWidget);
        goHomeButton->setObjectName(QString::fromUtf8("goHomeButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/go-home.png"), QSize(), QIcon::Normal, QIcon::Off);
        goHomeButton->setIcon(icon1);

        _3->addWidget(goHomeButton, 1, 1, 1, 1);

        moveRightButton = new QToolButton(layoutWidget);
        moveRightButton->setObjectName(QString::fromUtf8("moveRightButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/arrow-right.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveRightButton->setIcon(icon2);

        _3->addWidget(moveRightButton, 1, 2, 1, 1);

        moveUpButton = new QToolButton(layoutWidget);
        moveUpButton->setObjectName(QString::fromUtf8("moveUpButton"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/arrow-up.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveUpButton->setIcon(icon3);

        _3->addWidget(moveUpButton, 0, 1, 1, 1);

        zoomSlider = new QSlider(layoutWidget);
        zoomSlider->setObjectName(QString::fromUtf8("zoomSlider"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(zoomSlider->sizePolicy().hasHeightForWidth());
        zoomSlider->setSizePolicy(sizePolicy1);
        zoomSlider->setMinimum(900);
        zoomSlider->setMaximum(2400);
        zoomSlider->setValue(1050);
        zoomSlider->setSliderPosition(1050);
        zoomSlider->setTracking(true);
        zoomSlider->setOrientation(Qt::Vertical);
        zoomSlider->setTickPosition(QSlider::TicksAbove);
        zoomSlider->setTickInterval(100);

        _3->addWidget(zoomSlider, 4, 1, 1, 1);

        zoomInButton = new QToolButton(layoutWidget);
        zoomInButton->setObjectName(QString::fromUtf8("zoomInButton"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
        zoomInButton->setIcon(icon4);

        _3->addWidget(zoomInButton, 3, 1, 1, 1);

        moveDownButton = new QToolButton(layoutWidget);
        moveDownButton->setObjectName(QString::fromUtf8("moveDownButton"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/arrow-down.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveDownButton->setIcon(icon5);

        _3->addWidget(moveDownButton, 2, 1, 1, 1);

        moveLeftButton = new QToolButton(layoutWidget);
        moveLeftButton->setObjectName(QString::fromUtf8("moveLeftButton"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/arrow-left.png"), QSize(), QIcon::Normal, QIcon::Off);
        moveLeftButton->setIcon(icon6);

        _3->addWidget(moveLeftButton, 1, 0, 1, 1);


        horizontalLayout->addLayout(_3);

        spacerItem1 = new QSpacerItem(5, 5, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(spacerItem1);

        m_splitter->addWidget(layoutWidget);

        verticalLayout->addWidget(m_splitter);

        m_pSpacerFrame = new QFrame(NavigationWidget);
        m_pSpacerFrame->setObjectName(QString::fromUtf8("m_pSpacerFrame"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_pSpacerFrame->sizePolicy().hasHeightForWidth());
        m_pSpacerFrame->setSizePolicy(sizePolicy2);
        m_pSpacerFrame->setMinimumSize(QSize(16, 0));
        m_pSpacerFrame->setMaximumSize(QSize(16, 16777215));
        m_pSpacerFrame->setFrameShape(QFrame::NoFrame);
        m_pSpacerFrame->setFrameShadow(QFrame::Plain);

        verticalLayout->addWidget(m_pSpacerFrame);


        retranslateUi(NavigationWidget);

        QMetaObject::connectSlotsByName(NavigationWidget);
    } // setupUi

    void retranslateUi(QWidget *NavigationWidget)
    {
        NavigationWidget->setWindowTitle(QApplication::translate("NavigationWidget", "Navigation", 0, QApplication::UnicodeUTF8));
        boundedCheckBox->setText(QApplication::translate("NavigationWidget", "Around", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        locationListView->setToolTip(QApplication::translate("NavigationWidget", "Location Browser", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        locationListView->setWhatsThis(QApplication::translate("NavigationWidget", "Double click an entry of this list to get it displayed on the map.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        zoomOutButton->setToolTip(QApplication::translate("NavigationWidget", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        zoomOutButton->setWhatsThis(QApplication::translate("NavigationWidget", "Zoom out by pressing this button to see less detail.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        zoomOutButton->setText(QApplication::translate("NavigationWidget", "...", 0, QApplication::UnicodeUTF8));
        zoomOutButton->setShortcut(QApplication::translate("NavigationWidget", "-", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        goHomeButton->setToolTip(QApplication::translate("NavigationWidget", "Navigate to your 'Home Location'", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        goHomeButton->setWhatsThis(QApplication::translate("NavigationWidget", "Click this button to restore the original zoom level and view angle.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        goHomeButton->setText(QApplication::translate("NavigationWidget", "...", 0, QApplication::UnicodeUTF8));
        goHomeButton->setShortcut(QApplication::translate("NavigationWidget", "Home", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveRightButton->setToolTip(QApplication::translate("NavigationWidget", "Right", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveRightButton->setWhatsThis(QApplication::translate("NavigationWidget", "Rotates the earth clockwise around its axis.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveRightButton->setText(QApplication::translate("NavigationWidget", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveUpButton->setToolTip(QApplication::translate("NavigationWidget", "Up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveUpButton->setWhatsThis(QApplication::translate("NavigationWidget", "Tilts the earth's axis towards the user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveUpButton->setText(QApplication::translate("NavigationWidget", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        zoomSlider->setToolTip(QApplication::translate("NavigationWidget", "Zoom Slider", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        zoomSlider->setWhatsThis(QApplication::translate("NavigationWidget", "Use this slider to adjust the zoom level of the map.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        zoomInButton->setToolTip(QApplication::translate("NavigationWidget", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        zoomInButton->setWhatsThis(QApplication::translate("NavigationWidget", "Zoom in by pressing this button to see more detail.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        zoomInButton->setText(QApplication::translate("NavigationWidget", "...", 0, QApplication::UnicodeUTF8));
        zoomInButton->setShortcut(QApplication::translate("NavigationWidget", "+", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveDownButton->setToolTip(QApplication::translate("NavigationWidget", "Down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveDownButton->setWhatsThis(QApplication::translate("NavigationWidget", "Tilts the earth's axis away from the user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveDownButton->setText(QApplication::translate("NavigationWidget", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveLeftButton->setToolTip(QApplication::translate("NavigationWidget", "Left", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveLeftButton->setWhatsThis(QApplication::translate("NavigationWidget", "Rotates the earth counterclockwise around its axis.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveLeftButton->setText(QApplication::translate("NavigationWidget", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NavigationWidget: public Ui_NavigationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAVIGATIONWIDGET_H
