/********************************************************************************
** Form generated from reading UI file 'MarbleNavigator.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MARBLENAVIGATOR_H
#define UI_MARBLENAVIGATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSlider>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarbleNavigator
{
public:
    QHBoxLayout *hboxLayout;
    QGridLayout *gridLayout;
    QFrame *m_pSpacerFrame;
    QToolButton *moveUpButton;
    QToolButton *moveLeftButton;
    QToolButton *goHomeButton;
    QToolButton *moveRightButton;
    QToolButton *moveDownButton;
    QToolButton *zoomInButton;
    QSlider *zoomSlider;
    QToolButton *zoomOutButton;

    void setupUi(QWidget *MarbleNavigator)
    {
        if (MarbleNavigator->objectName().isEmpty())
            MarbleNavigator->setObjectName(QString::fromUtf8("MarbleNavigator"));
        MarbleNavigator->resize(249, 473);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(5));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MarbleNavigator->sizePolicy().hasHeightForWidth());
        MarbleNavigator->setSizePolicy(sizePolicy);
        hboxLayout = new QHBoxLayout(MarbleNavigator);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_pSpacerFrame = new QFrame(MarbleNavigator);
        m_pSpacerFrame->setObjectName(QString::fromUtf8("m_pSpacerFrame"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_pSpacerFrame->sizePolicy().hasHeightForWidth());
        m_pSpacerFrame->setSizePolicy(sizePolicy1);
        m_pSpacerFrame->setMinimumSize(QSize(16, 0));
        m_pSpacerFrame->setFrameShape(QFrame::NoFrame);
        m_pSpacerFrame->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(m_pSpacerFrame, 6, 1, 1, 1);

        moveUpButton = new QToolButton(MarbleNavigator);
        moveUpButton->setObjectName(QString::fromUtf8("moveUpButton"));
        const QIcon icon = QIcon(QString::fromUtf8(":/icons/arrow-up.png"));
        moveUpButton->setIcon(icon);

        gridLayout->addWidget(moveUpButton, 0, 1, 1, 1);

        moveLeftButton = new QToolButton(MarbleNavigator);
        moveLeftButton->setObjectName(QString::fromUtf8("moveLeftButton"));
        const QIcon icon1 = QIcon(QString::fromUtf8(":/icons/arrow-left.png"));
        moveLeftButton->setIcon(icon1);

        gridLayout->addWidget(moveLeftButton, 1, 0, 1, 1);

        goHomeButton = new QToolButton(MarbleNavigator);
        goHomeButton->setObjectName(QString::fromUtf8("goHomeButton"));
        const QIcon icon2 = QIcon(QString::fromUtf8(":/icons/go-home.png"));
        goHomeButton->setIcon(icon2);

        gridLayout->addWidget(goHomeButton, 1, 1, 1, 1);

        moveRightButton = new QToolButton(MarbleNavigator);
        moveRightButton->setObjectName(QString::fromUtf8("moveRightButton"));
        const QIcon icon3 = QIcon(QString::fromUtf8(":/icons/arrow-right.png"));
        moveRightButton->setIcon(icon3);

        gridLayout->addWidget(moveRightButton, 1, 2, 1, 1);

        moveDownButton = new QToolButton(MarbleNavigator);
        moveDownButton->setObjectName(QString::fromUtf8("moveDownButton"));
        const QIcon icon4 = QIcon(QString::fromUtf8(":/icons/arrow-down.png"));
        moveDownButton->setIcon(icon4);

        gridLayout->addWidget(moveDownButton, 2, 1, 1, 1);

        zoomInButton = new QToolButton(MarbleNavigator);
        zoomInButton->setObjectName(QString::fromUtf8("zoomInButton"));
        const QIcon icon5 = QIcon(QString::fromUtf8(":/icons/zoom-in.png"));
        zoomInButton->setIcon(icon5);

        gridLayout->addWidget(zoomInButton, 3, 1, 1, 1);

        zoomSlider = new QSlider(MarbleNavigator);
        zoomSlider->setObjectName(QString::fromUtf8("zoomSlider"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(3));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(zoomSlider->sizePolicy().hasHeightForWidth());
        zoomSlider->setSizePolicy(sizePolicy2);
        zoomSlider->setMinimum(1000);
        zoomSlider->setMaximum(2500);
        zoomSlider->setValue(1050);
        zoomSlider->setSliderPosition(1050);
        zoomSlider->setOrientation(Qt::Vertical);
        zoomSlider->setTickPosition(QSlider::TicksAbove);
        zoomSlider->setTickInterval(100);

        gridLayout->addWidget(zoomSlider, 4, 1, 1, 1);

        zoomOutButton = new QToolButton(MarbleNavigator);
        zoomOutButton->setObjectName(QString::fromUtf8("zoomOutButton"));
        const QIcon icon6 = QIcon(QString::fromUtf8(":/icons/zoom-out.png"));
        zoomOutButton->setIcon(icon6);

        gridLayout->addWidget(zoomOutButton, 5, 1, 1, 1);


        hboxLayout->addLayout(gridLayout);


        retranslateUi(MarbleNavigator);

        QMetaObject::connectSlotsByName(MarbleNavigator);
    } // setupUi

    void retranslateUi(QWidget *MarbleNavigator)
    {
#ifndef QT_NO_TOOLTIP
        moveUpButton->setToolTip(QApplication::translate("MarbleNavigator", "Up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveUpButton->setWhatsThis(QApplication::translate("MarbleNavigator", "Tilts the earth's axis towards the user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveUpButton->setText(QApplication::translate("MarbleNavigator", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveLeftButton->setToolTip(QApplication::translate("MarbleNavigator", "Left", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveLeftButton->setWhatsThis(QApplication::translate("MarbleNavigator", "Rotates the earth counterclockwise around its axis.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveLeftButton->setText(QApplication::translate("MarbleNavigator", "...", 0, QApplication::UnicodeUTF8));
        moveLeftButton->setShortcut(QApplication::translate("MarbleNavigator", "Left", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        goHomeButton->setToolTip(QApplication::translate("MarbleNavigator", "Reset View", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        goHomeButton->setWhatsThis(QApplication::translate("MarbleNavigator", "Click this button to restore the original zoom level and view angle.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        goHomeButton->setText(QApplication::translate("MarbleNavigator", "...", 0, QApplication::UnicodeUTF8));
        goHomeButton->setShortcut(QApplication::translate("MarbleNavigator", "Home", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveRightButton->setToolTip(QApplication::translate("MarbleNavigator", "Right", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveRightButton->setWhatsThis(QApplication::translate("MarbleNavigator", "Rotates the earth clockwise around its axis.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveRightButton->setText(QApplication::translate("MarbleNavigator", "...", 0, QApplication::UnicodeUTF8));
        moveRightButton->setShortcut(QApplication::translate("MarbleNavigator", "Right", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        moveDownButton->setToolTip(QApplication::translate("MarbleNavigator", "Down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        moveDownButton->setWhatsThis(QApplication::translate("MarbleNavigator", "Tilts the earth's axis away from the user.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        moveDownButton->setText(QApplication::translate("MarbleNavigator", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        zoomInButton->setToolTip(QApplication::translate("MarbleNavigator", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        zoomInButton->setWhatsThis(QApplication::translate("MarbleNavigator", "Zoom in by pressing this button to see more detail.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        zoomInButton->setText(QApplication::translate("MarbleNavigator", "...", 0, QApplication::UnicodeUTF8));
        zoomInButton->setShortcut(QApplication::translate("MarbleNavigator", "+", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        zoomSlider->setToolTip(QApplication::translate("MarbleNavigator", "Zoom Slider", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        zoomSlider->setWhatsThis(QApplication::translate("MarbleNavigator", "Use this slider to adjust the zoom level of the map.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        zoomOutButton->setToolTip(QApplication::translate("MarbleNavigator", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        zoomOutButton->setWhatsThis(QApplication::translate("MarbleNavigator", "Zoom out by pressing this button to see less detail.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        zoomOutButton->setText(QApplication::translate("MarbleNavigator", "...", 0, QApplication::UnicodeUTF8));
        zoomOutButton->setShortcut(QApplication::translate("MarbleNavigator", "-", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MarbleNavigator);
    } // retranslateUi

};

namespace Ui {
    class MarbleNavigator: public Ui_MarbleNavigator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MARBLENAVIGATOR_H
