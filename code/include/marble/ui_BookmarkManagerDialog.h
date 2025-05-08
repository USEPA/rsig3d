/********************************************************************************
** Form generated from reading UI file 'BookmarkManagerDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOOKMARKMANAGERDIALOG_H
#define UI_BOOKMARKMANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UiBookmarkManagerDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *importButton;
    QPushButton *exportButton;
    QSpacerItem *horizontalSpacer_3;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QGridLayout *foldersGridLayout;
    QLabel *foldersLabel;
    QPushButton *newFolderButton;
    QPushButton *renameFolderButton;
    QPushButton *removeFolderButton;
    QSpacerItem *horizontalSpacer_2;
    QTreeView *foldersTreeView;
    QWidget *layoutWidget1;
    QGridLayout *bookmarksGridLayout;
    QLabel *bookmarkLabel;
    QListView *bookmarksListView;
    QSpacerItem *horizontalSpacer;
    QPushButton *editBookmarkButton;
    QPushButton *removeBookmarkButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *UiBookmarkManagerDialog)
    {
        if (UiBookmarkManagerDialog->objectName().isEmpty())
            UiBookmarkManagerDialog->setObjectName(QString::fromUtf8("UiBookmarkManagerDialog"));
        UiBookmarkManagerDialog->resize(419, 262);
        verticalLayout = new QVBoxLayout(UiBookmarkManagerDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        importButton = new QPushButton(UiBookmarkManagerDialog);
        importButton->setObjectName(QString::fromUtf8("importButton"));
        importButton->setEnabled(true);

        horizontalLayout->addWidget(importButton);

        exportButton = new QPushButton(UiBookmarkManagerDialog);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));
        exportButton->setEnabled(true);

        horizontalLayout->addWidget(exportButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);

        splitter = new QSplitter(UiBookmarkManagerDialog);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        foldersGridLayout = new QGridLayout(layoutWidget);
        foldersGridLayout->setObjectName(QString::fromUtf8("foldersGridLayout"));
        foldersGridLayout->setContentsMargins(0, 0, 0, 0);
        foldersLabel = new QLabel(layoutWidget);
        foldersLabel->setObjectName(QString::fromUtf8("foldersLabel"));

        foldersGridLayout->addWidget(foldersLabel, 0, 0, 1, 1);

        newFolderButton = new QPushButton(layoutWidget);
        newFolderButton->setObjectName(QString::fromUtf8("newFolderButton"));
        newFolderButton->setEnabled(true);

        foldersGridLayout->addWidget(newFolderButton, 2, 0, 1, 1);

        renameFolderButton = new QPushButton(layoutWidget);
        renameFolderButton->setObjectName(QString::fromUtf8("renameFolderButton"));

        foldersGridLayout->addWidget(renameFolderButton, 2, 1, 1, 1);

        removeFolderButton = new QPushButton(layoutWidget);
        removeFolderButton->setObjectName(QString::fromUtf8("removeFolderButton"));
        removeFolderButton->setEnabled(true);

        foldersGridLayout->addWidget(removeFolderButton, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        foldersGridLayout->addItem(horizontalSpacer_2, 2, 3, 1, 1);

        foldersTreeView = new QTreeView(layoutWidget);
        foldersTreeView->setObjectName(QString::fromUtf8("foldersTreeView"));

        foldersGridLayout->addWidget(foldersTreeView, 1, 0, 1, 4);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        bookmarksGridLayout = new QGridLayout(layoutWidget1);
        bookmarksGridLayout->setObjectName(QString::fromUtf8("bookmarksGridLayout"));
        bookmarksGridLayout->setContentsMargins(0, 0, 0, 0);
        bookmarkLabel = new QLabel(layoutWidget1);
        bookmarkLabel->setObjectName(QString::fromUtf8("bookmarkLabel"));

        bookmarksGridLayout->addWidget(bookmarkLabel, 0, 0, 1, 1);

        bookmarksListView = new QListView(layoutWidget1);
        bookmarksListView->setObjectName(QString::fromUtf8("bookmarksListView"));

        bookmarksGridLayout->addWidget(bookmarksListView, 1, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        bookmarksGridLayout->addItem(horizontalSpacer, 2, 0, 1, 1);

        editBookmarkButton = new QPushButton(layoutWidget1);
        editBookmarkButton->setObjectName(QString::fromUtf8("editBookmarkButton"));

        bookmarksGridLayout->addWidget(editBookmarkButton, 2, 1, 1, 1);

        removeBookmarkButton = new QPushButton(layoutWidget1);
        removeBookmarkButton->setObjectName(QString::fromUtf8("removeBookmarkButton"));
        removeBookmarkButton->setEnabled(true);

        bookmarksGridLayout->addWidget(removeBookmarkButton, 2, 2, 1, 1);

        splitter->addWidget(layoutWidget1);

        verticalLayout->addWidget(splitter);

        buttonBox = new QDialogButtonBox(UiBookmarkManagerDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(UiBookmarkManagerDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), UiBookmarkManagerDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), UiBookmarkManagerDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(UiBookmarkManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *UiBookmarkManagerDialog)
    {
        UiBookmarkManagerDialog->setWindowTitle(QApplication::translate("UiBookmarkManagerDialog", "Bookmark Manager - Marble", 0, QApplication::UnicodeUTF8));
        importButton->setText(QApplication::translate("UiBookmarkManagerDialog", "Import Bookmarks...", 0, QApplication::UnicodeUTF8));
        exportButton->setText(QApplication::translate("UiBookmarkManagerDialog", "Export Bookmarks...", 0, QApplication::UnicodeUTF8));
        foldersLabel->setText(QApplication::translate("UiBookmarkManagerDialog", "Folders", 0, QApplication::UnicodeUTF8));
        newFolderButton->setText(QApplication::translate("UiBookmarkManagerDialog", "New", 0, QApplication::UnicodeUTF8));
        renameFolderButton->setText(QApplication::translate("UiBookmarkManagerDialog", "Rename", 0, QApplication::UnicodeUTF8));
        removeFolderButton->setText(QApplication::translate("UiBookmarkManagerDialog", "Delete", 0, QApplication::UnicodeUTF8));
        bookmarkLabel->setText(QApplication::translate("UiBookmarkManagerDialog", "Bookmarks", 0, QApplication::UnicodeUTF8));
        editBookmarkButton->setText(QApplication::translate("UiBookmarkManagerDialog", "Edit", 0, QApplication::UnicodeUTF8));
        removeBookmarkButton->setText(QApplication::translate("UiBookmarkManagerDialog", "Delete", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UiBookmarkManagerDialog: public Ui_UiBookmarkManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOOKMARKMANAGERDIALOG_H
