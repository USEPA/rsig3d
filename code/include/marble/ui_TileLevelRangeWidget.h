/********************************************************************************
** Form generated from reading UI file 'TileLevelRangeWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TILELEVELRANGEWIDGET_H
#define UI_TILELEVELRANGEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TileLevelRangeWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *tileLevelLabel;
    QSpinBox *topSpinBox;
    QLabel *toLabel;
    QSpinBox *bottomSpinBox;
    QSpacerItem *horizontalSpacer_8;

    void setupUi(QWidget *TileLevelRangeWidget)
    {
        if (TileLevelRangeWidget->objectName().isEmpty())
            TileLevelRangeWidget->setObjectName(QString::fromUtf8("TileLevelRangeWidget"));
        TileLevelRangeWidget->resize(273, 76);
        verticalLayout = new QVBoxLayout(TileLevelRangeWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(TileLevelRangeWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        tileLevelLabel = new QLabel(groupBox_2);
        tileLevelLabel->setObjectName(QString::fromUtf8("tileLevelLabel"));

        horizontalLayout_6->addWidget(tileLevelLabel);

        topSpinBox = new QSpinBox(groupBox_2);
        topSpinBox->setObjectName(QString::fromUtf8("topSpinBox"));
        topSpinBox->setMinimum(0);
        topSpinBox->setMaximum(20);
        topSpinBox->setValue(0);

        horizontalLayout_6->addWidget(topSpinBox);

        toLabel = new QLabel(groupBox_2);
        toLabel->setObjectName(QString::fromUtf8("toLabel"));

        horizontalLayout_6->addWidget(toLabel);

        bottomSpinBox = new QSpinBox(groupBox_2);
        bottomSpinBox->setObjectName(QString::fromUtf8("bottomSpinBox"));
        bottomSpinBox->setMinimum(0);
        bottomSpinBox->setMaximum(20);
        bottomSpinBox->setValue(4);

        horizontalLayout_6->addWidget(bottomSpinBox);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_8);


        verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout->addWidget(groupBox_2);

#ifndef QT_NO_SHORTCUT
        tileLevelLabel->setBuddy(topSpinBox);
#endif // QT_NO_SHORTCUT

        retranslateUi(TileLevelRangeWidget);

        QMetaObject::connectSlotsByName(TileLevelRangeWidget);
    } // setupUi

    void retranslateUi(QWidget *TileLevelRangeWidget)
    {
        groupBox_2->setTitle(QApplication::translate("TileLevelRangeWidget", "Zoom", 0, QApplication::UnicodeUTF8));
        tileLevelLabel->setText(QApplication::translate("TileLevelRangeWidget", "&Tile Level Range:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        topSpinBox->setToolTip(QApplication::translate("TileLevelRangeWidget", "Minimum Tile Level", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toLabel->setText(QApplication::translate("TileLevelRangeWidget", "to", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bottomSpinBox->setToolTip(QApplication::translate("TileLevelRangeWidget", "Maximum Tile Level", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Q_UNUSED(TileLevelRangeWidget);
    } // retranslateUi

};

namespace Ui {
    class TileLevelRangeWidget: public Ui_TileLevelRangeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TILELEVELRANGEWIDGET_H
