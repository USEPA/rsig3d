/********************************************************************************
** Form generated from reading UI file 'FileViewWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEVIEWWIDGET_H
#define UI_FILEVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileViewWidget
{
public:
    QVBoxLayout *verticalLayout;
    QListView *m_fileView;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *m_saveButton;
    QPushButton *m_closeButton;
    QTreeView *m_treeView;

    void setupUi(QWidget *FileViewWidget)
    {
        if (FileViewWidget->objectName().isEmpty())
            FileViewWidget->setObjectName(QString::fromUtf8("FileViewWidget"));
        FileViewWidget->resize(229, 282);
        verticalLayout = new QVBoxLayout(FileViewWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_fileView = new QListView(FileViewWidget);
        m_fileView->setObjectName(QString::fromUtf8("m_fileView"));

        verticalLayout->addWidget(m_fileView);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        m_saveButton = new QPushButton(FileViewWidget);
        m_saveButton->setObjectName(QString::fromUtf8("m_saveButton"));
        m_saveButton->setEnabled(false);

        hboxLayout->addWidget(m_saveButton);

        m_closeButton = new QPushButton(FileViewWidget);
        m_closeButton->setObjectName(QString::fromUtf8("m_closeButton"));
        m_closeButton->setEnabled(false);

        hboxLayout->addWidget(m_closeButton);


        verticalLayout->addLayout(hboxLayout);

        m_treeView = new QTreeView(FileViewWidget);
        m_treeView->setObjectName(QString::fromUtf8("m_treeView"));

        verticalLayout->addWidget(m_treeView);


        retranslateUi(FileViewWidget);

        QMetaObject::connectSlotsByName(FileViewWidget);
    } // setupUi

    void retranslateUi(QWidget *FileViewWidget)
    {
        FileViewWidget->setWindowTitle(QApplication::translate("FileViewWidget", "File View", 0, QApplication::UnicodeUTF8));
        m_saveButton->setText(QApplication::translate("FileViewWidget", "&Save", 0, QApplication::UnicodeUTF8));
        m_closeButton->setText(QApplication::translate("FileViewWidget", "&Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FileViewWidget: public Ui_FileViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEVIEWWIDGET_H
