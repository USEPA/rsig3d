/********************************************************************************
** Form generated from reading UI file 'EditBookmarkDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITBOOKMARKDIALOG_H
#define UI_EDITBOOKMARKDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include "LatLonEdit.h"

QT_BEGIN_NAMESPACE

class Ui_UiEditBookmarkDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *name_lbl;
    QLineEdit *m_name;
    QLabel *position_lbl;
    QHBoxLayout *latLonLayout;
    Marble::LatLonEdit *m_longitude;
    Marble::LatLonEdit *m_latitude;
    QLabel *description_lbl;
    QTextEdit *m_description;
    QLabel *folder_lbl;
    QHBoxLayout *horizontalLayout;
    QComboBox *m_folders;
    QPushButton *m_newFolderButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *UiEditBookmarkDialog)
    {
        if (UiEditBookmarkDialog->objectName().isEmpty())
            UiEditBookmarkDialog->setObjectName(QString::fromUtf8("UiEditBookmarkDialog"));
        UiEditBookmarkDialog->resize(309, 228);
        verticalLayout = new QVBoxLayout(UiEditBookmarkDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        name_lbl = new QLabel(UiEditBookmarkDialog);
        name_lbl->setObjectName(QString::fromUtf8("name_lbl"));

        formLayout->setWidget(0, QFormLayout::LabelRole, name_lbl);

        m_name = new QLineEdit(UiEditBookmarkDialog);
        m_name->setObjectName(QString::fromUtf8("m_name"));
        m_name->setCursorPosition(0);

        formLayout->setWidget(0, QFormLayout::FieldRole, m_name);

        position_lbl = new QLabel(UiEditBookmarkDialog);
        position_lbl->setObjectName(QString::fromUtf8("position_lbl"));

        formLayout->setWidget(1, QFormLayout::LabelRole, position_lbl);

        latLonLayout = new QHBoxLayout();
        latLonLayout->setContentsMargins(0, 0, 0, 0);
        latLonLayout->setObjectName(QString::fromUtf8("latLonLayout"));
        m_longitude = new Marble::LatLonEdit(UiEditBookmarkDialog);
        m_longitude->setObjectName(QString::fromUtf8("m_longitude"));

        latLonLayout->addWidget(m_longitude);

        m_latitude = new Marble::LatLonEdit(UiEditBookmarkDialog);
        m_latitude->setObjectName(QString::fromUtf8("m_latitude"));

        latLonLayout->addWidget(m_latitude);


        formLayout->setLayout(1, QFormLayout::FieldRole, latLonLayout);

        description_lbl = new QLabel(UiEditBookmarkDialog);
        description_lbl->setObjectName(QString::fromUtf8("description_lbl"));

        formLayout->setWidget(2, QFormLayout::LabelRole, description_lbl);

        m_description = new QTextEdit(UiEditBookmarkDialog);
        m_description->setObjectName(QString::fromUtf8("m_description"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_description->sizePolicy().hasHeightForWidth());
        m_description->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::FieldRole, m_description);

        folder_lbl = new QLabel(UiEditBookmarkDialog);
        folder_lbl->setObjectName(QString::fromUtf8("folder_lbl"));

        formLayout->setWidget(3, QFormLayout::LabelRole, folder_lbl);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        m_folders = new QComboBox(UiEditBookmarkDialog);
        m_folders->setObjectName(QString::fromUtf8("m_folders"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_folders->sizePolicy().hasHeightForWidth());
        m_folders->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(m_folders);

        m_newFolderButton = new QPushButton(UiEditBookmarkDialog);
        m_newFolderButton->setObjectName(QString::fromUtf8("m_newFolderButton"));

        horizontalLayout->addWidget(m_newFolderButton);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(UiEditBookmarkDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#ifndef QT_NO_SHORTCUT
        name_lbl->setBuddy(m_name);
        position_lbl->setBuddy(m_latitude);
        description_lbl->setBuddy(m_description);
        folder_lbl->setBuddy(m_folders);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(m_name, m_latitude);
        QWidget::setTabOrder(m_latitude, m_longitude);
        QWidget::setTabOrder(m_longitude, m_description);
        QWidget::setTabOrder(m_description, m_folders);

        retranslateUi(UiEditBookmarkDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), UiEditBookmarkDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), UiEditBookmarkDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(UiEditBookmarkDialog);
    } // setupUi

    void retranslateUi(QDialog *UiEditBookmarkDialog)
    {
        UiEditBookmarkDialog->setWindowTitle(QApplication::translate("UiEditBookmarkDialog", "Edit Bookmark - Marble", 0, QApplication::UnicodeUTF8));
        name_lbl->setText(QApplication::translate("UiEditBookmarkDialog", "&Name", 0, QApplication::UnicodeUTF8));
        m_name->setText(QString());
        position_lbl->setText(QApplication::translate("UiEditBookmarkDialog", "&Position", 0, QApplication::UnicodeUTF8));
        description_lbl->setText(QApplication::translate("UiEditBookmarkDialog", "&Description", 0, QApplication::UnicodeUTF8));
        m_description->setHtml(QApplication::translate("UiEditBookmarkDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<table border=\"0\" style=\"-qt-table-type: root; margin-top:4px; margin-bottom:4px; margin-left:4px; margin-right:4px;\">\n"
"<tr>\n"
"<td style=\"border: none;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></td></tr></table></body></html>", 0, QApplication::UnicodeUTF8));
        folder_lbl->setText(QApplication::translate("UiEditBookmarkDialog", "&Folder", 0, QApplication::UnicodeUTF8));
        m_newFolderButton->setText(QApplication::translate("UiEditBookmarkDialog", "&Add Folder", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiEditBookmarkDialog: public Ui_UiEditBookmarkDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITBOOKMARKDIALOG_H
