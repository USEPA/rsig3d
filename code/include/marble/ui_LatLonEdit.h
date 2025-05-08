/********************************************************************************
** Form generated from reading UI file 'LatLonEdit.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LATLONEDIT_H
#define UI_LATLONEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LatLonEditPrivate
{
public:
    QHBoxLayout *horizontalLayout;
    QSpinBox *m_intValueEditor;
    QSpinBox *m_uintValueEditor;
    QDoubleSpinBox *m_floatValueEditor;
    QComboBox *m_sign;

    void setupUi(QWidget *LatLonEditPrivate)
    {
        if (LatLonEditPrivate->objectName().isEmpty())
            LatLonEditPrivate->setObjectName(QString::fromUtf8("LatLonEditPrivate"));
        LatLonEditPrivate->resize(434, 37);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LatLonEditPrivate->sizePolicy().hasHeightForWidth());
        LatLonEditPrivate->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(LatLonEditPrivate);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        m_intValueEditor = new QSpinBox(LatLonEditPrivate);
        m_intValueEditor->setObjectName(QString::fromUtf8("m_intValueEditor"));
        m_intValueEditor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(m_intValueEditor);

        m_uintValueEditor = new QSpinBox(LatLonEditPrivate);
        m_uintValueEditor->setObjectName(QString::fromUtf8("m_uintValueEditor"));
        m_uintValueEditor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        m_uintValueEditor->setMinimum(-1);
        m_uintValueEditor->setMaximum(60);

        horizontalLayout->addWidget(m_uintValueEditor);

        m_floatValueEditor = new QDoubleSpinBox(LatLonEditPrivate);
        m_floatValueEditor->setObjectName(QString::fromUtf8("m_floatValueEditor"));
        m_floatValueEditor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        m_floatValueEditor->setDecimals(2);
        m_floatValueEditor->setMinimum(-1);
        m_floatValueEditor->setMaximum(60);

        horizontalLayout->addWidget(m_floatValueEditor);

        m_sign = new QComboBox(LatLonEditPrivate);
        m_sign->setObjectName(QString::fromUtf8("m_sign"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_sign->sizePolicy().hasHeightForWidth());
        m_sign->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(m_sign);


        retranslateUi(LatLonEditPrivate);

        QMetaObject::connectSlotsByName(LatLonEditPrivate);
    } // setupUi

    void retranslateUi(QWidget *LatLonEditPrivate)
    {
        m_intValueEditor->setSuffix(QApplication::translate("LatLonEditPrivate", "\302\260", 0, QApplication::UnicodeUTF8));
        m_uintValueEditor->setSuffix(QApplication::translate("LatLonEditPrivate", "\342\200\262", 0, QApplication::UnicodeUTF8));
        m_floatValueEditor->setSuffix(QApplication::translate("LatLonEditPrivate", "\342\200\263", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(LatLonEditPrivate);
    } // retranslateUi

};

namespace Ui {
    class LatLonEditPrivate: public Ui_LatLonEditPrivate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LATLONEDIT_H
