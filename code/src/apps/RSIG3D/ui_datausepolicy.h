/********************************************************************************
** Form generated from reading UI file 'datausepolicy.ui'
**
** Created: Thu May 8 12:58:25 2025
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAUSEPOLICY_H
#define UI_DATAUSEPOLICY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DataUsePolicy
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *dataPolicyTextBrowser;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *dataPolicyAcceptButton;
    QPushButton *dataPolicyOkButton;
    QPushButton *dataPolicyCancelButton;

    void setupUi(QDialog *DataUsePolicy)
    {
        if (DataUsePolicy->objectName().isEmpty())
            DataUsePolicy->setObjectName(QString::fromUtf8("DataUsePolicy"));
        DataUsePolicy->resize(505, 796);
        DataUsePolicy->setModal(true);
        verticalLayout = new QVBoxLayout(DataUsePolicy);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        dataPolicyTextBrowser = new QTextBrowser(DataUsePolicy);
        dataPolicyTextBrowser->setObjectName(QString::fromUtf8("dataPolicyTextBrowser"));
        dataPolicyTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        dataPolicyTextBrowser->setOpenExternalLinks(true);

        verticalLayout->addWidget(dataPolicyTextBrowser);

        groupBox = new QGroupBox(DataUsePolicy);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 39));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        dataPolicyAcceptButton = new QPushButton(groupBox);
        dataPolicyAcceptButton->setObjectName(QString::fromUtf8("dataPolicyAcceptButton"));
        dataPolicyAcceptButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(dataPolicyAcceptButton);

        dataPolicyOkButton = new QPushButton(groupBox);
        dataPolicyOkButton->setObjectName(QString::fromUtf8("dataPolicyOkButton"));
        dataPolicyOkButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(dataPolicyOkButton);

        dataPolicyCancelButton = new QPushButton(groupBox);
        dataPolicyCancelButton->setObjectName(QString::fromUtf8("dataPolicyCancelButton"));
        dataPolicyCancelButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(dataPolicyCancelButton);


        verticalLayout->addWidget(groupBox);


        retranslateUi(DataUsePolicy);

        QMetaObject::connectSlotsByName(DataUsePolicy);
    } // setupUi

    void retranslateUi(QDialog *DataUsePolicy)
    {
        DataUsePolicy->setWindowTitle(QApplication::translate("DataUsePolicy", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        dataPolicyAcceptButton->setText(QApplication::translate("DataUsePolicy", "Accept", 0, QApplication::UnicodeUTF8));
        dataPolicyOkButton->setText(QApplication::translate("DataUsePolicy", "Ok", 0, QApplication::UnicodeUTF8));
        dataPolicyCancelButton->setText(QApplication::translate("DataUsePolicy", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataUsePolicy: public Ui_DataUsePolicy {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAUSEPOLICY_H
