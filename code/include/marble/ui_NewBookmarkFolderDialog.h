/********************************************************************************
** Form generated from reading UI file 'NewBookmarkFolderDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWBOOKMARKFOLDERDIALOG_H
#define UI_NEWBOOKMARKFOLDERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UiNewBookmarkFolderDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *m_namelbl;
    QLineEdit *m_name;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *UiNewBookmarkFolderDialog)
    {
        if (UiNewBookmarkFolderDialog->objectName().isEmpty())
            UiNewBookmarkFolderDialog->setObjectName(QString::fromUtf8("UiNewBookmarkFolderDialog"));
        UiNewBookmarkFolderDialog->resize(302, 67);
        verticalLayout = new QVBoxLayout(UiNewBookmarkFolderDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        m_namelbl = new QLabel(UiNewBookmarkFolderDialog);
        m_namelbl->setObjectName(QString::fromUtf8("m_namelbl"));

        horizontalLayout_2->addWidget(m_namelbl);

        m_name = new QLineEdit(UiNewBookmarkFolderDialog);
        m_name->setObjectName(QString::fromUtf8("m_name"));

        horizontalLayout_2->addWidget(m_name);


        verticalLayout->addLayout(horizontalLayout_2);

        buttonBox = new QDialogButtonBox(UiNewBookmarkFolderDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#ifndef QT_NO_SHORTCUT
        m_namelbl->setBuddy(m_name);
#endif // QT_NO_SHORTCUT

        retranslateUi(UiNewBookmarkFolderDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), UiNewBookmarkFolderDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), UiNewBookmarkFolderDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(UiNewBookmarkFolderDialog);
    } // setupUi

    void retranslateUi(QDialog *UiNewBookmarkFolderDialog)
    {
        UiNewBookmarkFolderDialog->setWindowTitle(QApplication::translate("UiNewBookmarkFolderDialog", "New Bookmark Folder - Marble", 0, QApplication::UnicodeUTF8));
        m_namelbl->setText(QApplication::translate("UiNewBookmarkFolderDialog", "&Folder Name", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiNewBookmarkFolderDialog: public Ui_UiNewBookmarkFolderDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWBOOKMARKFOLDERDIALOG_H
