/********************************************************************************
** Form generated from reading UI file 'LatLonBoxWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LATLONBOXWIDGET_H
#define UI_LATLONBOXWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LatLonBoxWidget
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QDoubleSpinBox *northSpinBox;
    QLabel *northLabel;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QDoubleSpinBox *westSpinBox;
    QLabel *westLabel;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QDoubleSpinBox *eastSpinBox;
    QLabel *eastLabel;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QDoubleSpinBox *southSpinBox;
    QLabel *southLabel;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *LatLonBoxWidget)
    {
        if (LatLonBoxWidget->objectName().isEmpty())
            LatLonBoxWidget->setObjectName(QString::fromUtf8("LatLonBoxWidget"));
        LatLonBoxWidget->setEnabled(true);
        LatLonBoxWidget->resize(313, 100);
        gridLayout = new QGridLayout(LatLonBoxWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer_5 = new QSpacerItem(13, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        northSpinBox = new QDoubleSpinBox(LatLonBoxWidget);
        northSpinBox->setObjectName(QString::fromUtf8("northSpinBox"));
        northSpinBox->setEnabled(true);
        northSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        northSpinBox->setDecimals(3);
        northSpinBox->setMinimum(-90);
        northSpinBox->setMaximum(90);
        northSpinBox->setValue(90);

        horizontalLayout->addWidget(northSpinBox);

        northLabel = new QLabel(LatLonBoxWidget);
        northLabel->setObjectName(QString::fromUtf8("northLabel"));

        horizontalLayout->addWidget(northLabel);


        gridLayout->addLayout(horizontalLayout, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(13, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        westSpinBox = new QDoubleSpinBox(LatLonBoxWidget);
        westSpinBox->setObjectName(QString::fromUtf8("westSpinBox"));
        westSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        westSpinBox->setDecimals(3);
        westSpinBox->setMinimum(-180);
        westSpinBox->setMaximum(180);
        westSpinBox->setValue(-180);

        horizontalLayout_4->addWidget(westSpinBox);

        westLabel = new QLabel(LatLonBoxWidget);
        westLabel->setObjectName(QString::fromUtf8("westLabel"));

        horizontalLayout_4->addWidget(westLabel);


        gridLayout->addLayout(horizontalLayout_4, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(13, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        eastSpinBox = new QDoubleSpinBox(LatLonBoxWidget);
        eastSpinBox->setObjectName(QString::fromUtf8("eastSpinBox"));
        eastSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        eastSpinBox->setDecimals(3);
        eastSpinBox->setMinimum(-180);
        eastSpinBox->setMaximum(180);
        eastSpinBox->setValue(180);

        horizontalLayout_3->addWidget(eastSpinBox);

        eastLabel = new QLabel(LatLonBoxWidget);
        eastLabel->setObjectName(QString::fromUtf8("eastLabel"));

        horizontalLayout_3->addWidget(eastLabel);


        gridLayout->addLayout(horizontalLayout_3, 1, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(13, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 2, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        southSpinBox = new QDoubleSpinBox(LatLonBoxWidget);
        southSpinBox->setObjectName(QString::fromUtf8("southSpinBox"));
        southSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        southSpinBox->setDecimals(3);
        southSpinBox->setMinimum(-90);
        southSpinBox->setMaximum(90);
        southSpinBox->setValue(-90);

        horizontalLayout_2->addWidget(southSpinBox);

        southLabel = new QLabel(LatLonBoxWidget);
        southLabel->setObjectName(QString::fromUtf8("southLabel"));

        horizontalLayout_2->addWidget(southLabel);


        gridLayout->addLayout(horizontalLayout_2, 2, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(13, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 2, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 0, 3, 1);

#ifndef QT_NO_SHORTCUT
        northLabel->setBuddy(northSpinBox);
        westLabel->setBuddy(westSpinBox);
        eastLabel->setBuddy(eastSpinBox);
        southLabel->setBuddy(southSpinBox);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(northSpinBox, westSpinBox);
        QWidget::setTabOrder(westSpinBox, eastSpinBox);
        QWidget::setTabOrder(eastSpinBox, southSpinBox);

        retranslateUi(LatLonBoxWidget);

        QMetaObject::connectSlotsByName(LatLonBoxWidget);
    } // setupUi

    void retranslateUi(QWidget *LatLonBoxWidget)
    {
        LatLonBoxWidget->setWindowTitle(QApplication::translate("LatLonBoxWidget", "Select a geographic region", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        northSpinBox->setToolTip(QApplication::translate("LatLonBoxWidget", "Northern Latitude", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        northLabel->setText(QApplication::translate("LatLonBoxWidget", "&N", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        westSpinBox->setToolTip(QApplication::translate("LatLonBoxWidget", "Western Longitude", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        westLabel->setText(QApplication::translate("LatLonBoxWidget", "&W", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        eastSpinBox->setToolTip(QApplication::translate("LatLonBoxWidget", "Eastern Longitude", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        eastLabel->setText(QApplication::translate("LatLonBoxWidget", "&E", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        southSpinBox->setToolTip(QApplication::translate("LatLonBoxWidget", "Southern Latitude", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        southLabel->setText(QApplication::translate("LatLonBoxWidget", "&S", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LatLonBoxWidget: public Ui_LatLonBoxWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LATLONBOXWIDGET_H
