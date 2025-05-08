/********************************************************************************
** Form generated from reading UI file 'boundingbox.ui'
**
** Created: Thu May 8 12:58:25 2025
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOUNDINGBOX_H
#define UI_BOUNDINGBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BoundingBox
{
public:
    QGroupBox *groupBox;
    QDoubleSpinBox *northDSpinBox;
    QLabel *label_4;
    QLabel *label_11;
    QLabel *label_14;
    QDoubleSpinBox *eastDSpinBox;
    QLabel *label_15;
    QDoubleSpinBox *southDSpinBox;
    QDoubleSpinBox *westDSpinBox;

    void setupUi(QWidget *BoundingBox)
    {
        if (BoundingBox->objectName().isEmpty())
            BoundingBox->setObjectName(QString::fromUtf8("BoundingBox"));
        BoundingBox->resize(415, 329);
        groupBox = new QGroupBox(BoundingBox);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, -2, 186, 68));
        groupBox->setMinimumSize(QSize(186, 68));
        groupBox->setMaximumSize(QSize(186, 68));
        groupBox->setFlat(true);
        groupBox->setCheckable(false);
        northDSpinBox = new QDoubleSpinBox(groupBox);
        northDSpinBox->setObjectName(QString::fromUtf8("northDSpinBox"));
        northDSpinBox->setGeometry(QRect(62, 4, 64, 20));
        northDSpinBox->setMinimumSize(QSize(64, 20));
        northDSpinBox->setMaximumSize(QSize(64, 20));
        northDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        northDSpinBox->setMinimum(-90);
        northDSpinBox->setMaximum(90);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(52, 6, 21, 17));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(6, 26, 21, 17));
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(52, 46, 21, 17));
        eastDSpinBox = new QDoubleSpinBox(groupBox);
        eastDSpinBox->setObjectName(QString::fromUtf8("eastDSpinBox"));
        eastDSpinBox->setGeometry(QRect(106, 24, 64, 20));
        eastDSpinBox->setMinimumSize(QSize(64, 20));
        eastDSpinBox->setMaximumSize(QSize(64, 20));
        eastDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        eastDSpinBox->setMinimum(-180);
        eastDSpinBox->setMaximum(180);
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(172, 26, 21, 17));
        southDSpinBox = new QDoubleSpinBox(groupBox);
        southDSpinBox->setObjectName(QString::fromUtf8("southDSpinBox"));
        southDSpinBox->setGeometry(QRect(62, 44, 64, 20));
        southDSpinBox->setMinimumSize(QSize(64, 20));
        southDSpinBox->setMaximumSize(QSize(64, 20));
        southDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        southDSpinBox->setMinimum(-90);
        southDSpinBox->setMaximum(90);
        westDSpinBox = new QDoubleSpinBox(groupBox);
        westDSpinBox->setObjectName(QString::fromUtf8("westDSpinBox"));
        westDSpinBox->setGeometry(QRect(20, 24, 64, 20));
        westDSpinBox->setMinimumSize(QSize(64, 20));
        westDSpinBox->setMaximumSize(QSize(64, 20));
        westDSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        westDSpinBox->setMinimum(-180);
        westDSpinBox->setMaximum(180);

        retranslateUi(BoundingBox);

        QMetaObject::connectSlotsByName(BoundingBox);
    } // setupUi

    void retranslateUi(QWidget *BoundingBox)
    {
        BoundingBox->setWindowTitle(QApplication::translate("BoundingBox", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        label_4->setText(QApplication::translate("BoundingBox", "N", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("BoundingBox", "W", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("BoundingBox", "S", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("BoundingBox", "E", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BoundingBox: public Ui_BoundingBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOUNDINGBOX_H
