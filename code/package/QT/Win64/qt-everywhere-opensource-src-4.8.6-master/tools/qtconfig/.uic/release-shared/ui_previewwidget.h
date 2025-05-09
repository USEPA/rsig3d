/*
*********************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
*********************************************************************
*/

/********************************************************************************
** Form generated from reading UI file 'previewwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREVIEWWIDGET_H
#define UI_PREVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollBar>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreviewWidget
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *GroupBox1;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *RadioButton1;
    QRadioButton *RadioButton2;
    QRadioButton *RadioButton3;
    QGroupBox *GroupBox2;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *CheckBox1;
    QCheckBox *CheckBox2;
    QProgressBar *ProgressBar1;
    QVBoxLayout *verticalLayout;
    QLineEdit *LineEdit1;
    QComboBox *ComboBox1;
    QHBoxLayout *horizontalLayout_2;
    QSpinBox *SpinBox1;
    QPushButton *PushButton1;
    QScrollBar *ScrollBar1;
    QSlider *Slider1;
    QTextEdit *textView;
    QSpacerItem *Spacer2;

    void setupUi(QWidget *PreviewWidget)
    {
        if (PreviewWidget->objectName().isEmpty())
            PreviewWidget->setObjectName(QString::fromUtf8("PreviewWidget"));
        PreviewWidget->resize(398, 282);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PreviewWidget->sizePolicy().hasHeightForWidth());
        PreviewWidget->setSizePolicy(sizePolicy);
        verticalLayout_5 = new QVBoxLayout(PreviewWidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
#ifndef Q_OS_MAC
        verticalLayout_2->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
#endif
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        GroupBox1 = new QGroupBox(PreviewWidget);
        GroupBox1->setObjectName(QString::fromUtf8("GroupBox1"));
        verticalLayout_3 = new QVBoxLayout(GroupBox1);
#ifndef Q_OS_MAC
        verticalLayout_3->setSpacing(6);
#endif
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        RadioButton1 = new QRadioButton(GroupBox1);
        RadioButton1->setObjectName(QString::fromUtf8("RadioButton1"));
        RadioButton1->setChecked(true);

        verticalLayout_3->addWidget(RadioButton1);

        RadioButton2 = new QRadioButton(GroupBox1);
        RadioButton2->setObjectName(QString::fromUtf8("RadioButton2"));

        verticalLayout_3->addWidget(RadioButton2);

        RadioButton3 = new QRadioButton(GroupBox1);
        RadioButton3->setObjectName(QString::fromUtf8("RadioButton3"));

        verticalLayout_3->addWidget(RadioButton3);


        verticalLayout_2->addWidget(GroupBox1);

        GroupBox2 = new QGroupBox(PreviewWidget);
        GroupBox2->setObjectName(QString::fromUtf8("GroupBox2"));
        verticalLayout_4 = new QVBoxLayout(GroupBox2);
#ifndef Q_OS_MAC
        verticalLayout_4->setSpacing(6);
#endif
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        CheckBox1 = new QCheckBox(GroupBox2);
        CheckBox1->setObjectName(QString::fromUtf8("CheckBox1"));
        CheckBox1->setChecked(true);

        verticalLayout_4->addWidget(CheckBox1);

        CheckBox2 = new QCheckBox(GroupBox2);
        CheckBox2->setObjectName(QString::fromUtf8("CheckBox2"));

        verticalLayout_4->addWidget(CheckBox2);


        verticalLayout_2->addWidget(GroupBox2);

        ProgressBar1 = new QProgressBar(PreviewWidget);
        ProgressBar1->setObjectName(QString::fromUtf8("ProgressBar1"));
        ProgressBar1->setValue(50);

        verticalLayout_2->addWidget(ProgressBar1);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
        verticalLayout->setSpacing(6);
#endif
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        LineEdit1 = new QLineEdit(PreviewWidget);
        LineEdit1->setObjectName(QString::fromUtf8("LineEdit1"));

        verticalLayout->addWidget(LineEdit1);

        ComboBox1 = new QComboBox(PreviewWidget);
        ComboBox1->setObjectName(QString::fromUtf8("ComboBox1"));

        verticalLayout->addWidget(ComboBox1);

        horizontalLayout_2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        horizontalLayout_2->setSpacing(6);
#endif
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        SpinBox1 = new QSpinBox(PreviewWidget);
        SpinBox1->setObjectName(QString::fromUtf8("SpinBox1"));

        horizontalLayout_2->addWidget(SpinBox1);

        PushButton1 = new QPushButton(PreviewWidget);
        PushButton1->setObjectName(QString::fromUtf8("PushButton1"));

        horizontalLayout_2->addWidget(PushButton1);


        verticalLayout->addLayout(horizontalLayout_2);

        ScrollBar1 = new QScrollBar(PreviewWidget);
        ScrollBar1->setObjectName(QString::fromUtf8("ScrollBar1"));
        ScrollBar1->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(ScrollBar1);

        Slider1 = new QSlider(PreviewWidget);
        Slider1->setObjectName(QString::fromUtf8("Slider1"));
        Slider1->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(Slider1);

        textView = new QTextEdit(PreviewWidget);
        textView->setObjectName(QString::fromUtf8("textView"));
        textView->setMaximumSize(QSize(32767, 55));
        textView->setReadOnly(true);

        verticalLayout->addWidget(textView);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_5->addLayout(horizontalLayout);

        Spacer2 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(Spacer2);


        retranslateUi(PreviewWidget);

        QMetaObject::connectSlotsByName(PreviewWidget);
    } // setupUi

    void retranslateUi(QWidget *PreviewWidget)
    {
        PreviewWidget->setWindowTitle(QApplication::translate("PreviewWidget", "Preview Window", 0, QApplication::UnicodeUTF8));
        GroupBox1->setTitle(QApplication::translate("PreviewWidget", "GroupBox", 0, QApplication::UnicodeUTF8));
        RadioButton1->setText(QApplication::translate("PreviewWidget", "RadioButton1", 0, QApplication::UnicodeUTF8));
        RadioButton2->setText(QApplication::translate("PreviewWidget", "RadioButton2", 0, QApplication::UnicodeUTF8));
        RadioButton3->setText(QApplication::translate("PreviewWidget", "RadioButton3", 0, QApplication::UnicodeUTF8));
        GroupBox2->setTitle(QApplication::translate("PreviewWidget", "GroupBox2", 0, QApplication::UnicodeUTF8));
        CheckBox1->setText(QApplication::translate("PreviewWidget", "CheckBox1", 0, QApplication::UnicodeUTF8));
        CheckBox2->setText(QApplication::translate("PreviewWidget", "CheckBox2", 0, QApplication::UnicodeUTF8));
        LineEdit1->setText(QApplication::translate("PreviewWidget", "LineEdit", 0, QApplication::UnicodeUTF8));
        ComboBox1->clear();
        ComboBox1->insertItems(0, QStringList()
         << QApplication::translate("PreviewWidget", "ComboBox", 0, QApplication::UnicodeUTF8)
        );
        PushButton1->setText(QApplication::translate("PreviewWidget", "PushButton", 0, QApplication::UnicodeUTF8));
        textView->setHtml(QApplication::translate("PreviewWidget", "<p><a href=\"http://qt.nokia.com\">http://qt.nokia.com</a></p>\n"
"<p><a href=\"http://www.kde.org\">http://www.kde.org</a></p>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PreviewWidget: public Ui_PreviewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREVIEWWIDGET_H
