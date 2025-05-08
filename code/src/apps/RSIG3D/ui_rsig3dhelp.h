/********************************************************************************
** Form generated from reading UI file 'rsig3dhelp.ui'
**
** Created: Thu May 8 12:58:25 2025
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RSIG3DHELP_H
#define UI_RSIG3DHELP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RSIG3DHelp
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *rsig3dHelpTextBrowser;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *rsig3dHelpOkButton;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *dontShowAgain;

    void setupUi(QDialog *RSIG3DHelp)
    {
        if (RSIG3DHelp->objectName().isEmpty())
            RSIG3DHelp->setObjectName(QString::fromUtf8("RSIG3DHelp"));
        RSIG3DHelp->resize(500, 600);
        RSIG3DHelp->setModal(true);
        verticalLayout = new QVBoxLayout(RSIG3DHelp);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        rsig3dHelpTextBrowser = new QTextBrowser(RSIG3DHelp);
        rsig3dHelpTextBrowser->setObjectName(QString::fromUtf8("rsig3dHelpTextBrowser"));
        rsig3dHelpTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rsig3dHelpTextBrowser->setOpenExternalLinks(true);

        verticalLayout->addWidget(rsig3dHelpTextBrowser);

        groupBox = new QGroupBox(RSIG3DHelp);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 39));
        groupBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        rsig3dHelpOkButton = new QPushButton(groupBox);
        rsig3dHelpOkButton->setObjectName(QString::fromUtf8("rsig3dHelpOkButton"));
        rsig3dHelpOkButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(rsig3dHelpOkButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        dontShowAgain = new QCheckBox(groupBox);
        dontShowAgain->setObjectName(QString::fromUtf8("dontShowAgain"));
        dontShowAgain->setMaximumSize(QSize(120, 16777215));
        dontShowAgain->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(dontShowAgain);


        verticalLayout->addWidget(groupBox);


        retranslateUi(RSIG3DHelp);

        QMetaObject::connectSlotsByName(RSIG3DHelp);
    } // setupUi

    void retranslateUi(QDialog *RSIG3DHelp)
    {
        RSIG3DHelp->setWindowTitle(QApplication::translate("RSIG3DHelp", "RSIG3D Tips", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        rsig3dHelpOkButton->setText(QApplication::translate("RSIG3DHelp", "Done", 0, QApplication::UnicodeUTF8));
        dontShowAgain->setText(QApplication::translate("RSIG3DHelp", "Don't show again", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RSIG3DHelp: public Ui_RSIG3DHelp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RSIG3DHELP_H
