/********************************************************************************
** Form generated from reading UI file 'qtgradientview.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGRADIENTVIEW_H
#define UI_QTGRADIENTVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGradientView
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QToolButton *newButton;
    QToolButton *editButton;
    QToolButton *renameButton;
    QToolButton *removeButton;
    QSpacerItem *spacerItem;
    QListWidget *listWidget;

    void setupUi(QWidget *QtGradientView)
    {
        if (QtGradientView->objectName().isEmpty())
            QtGradientView->setObjectName(QString::fromUtf8("QtGradientView"));
        QtGradientView->resize(484, 228);
        vboxLayout = new QVBoxLayout(QtGradientView);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        newButton = new QToolButton(QtGradientView);
        newButton->setObjectName(QString::fromUtf8("newButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(newButton->sizePolicy().hasHeightForWidth());
        newButton->setSizePolicy(sizePolicy);
        newButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        newButton->setAutoRaise(true);

        hboxLayout->addWidget(newButton);

        editButton = new QToolButton(QtGradientView);
        editButton->setObjectName(QString::fromUtf8("editButton"));
        sizePolicy.setHeightForWidth(editButton->sizePolicy().hasHeightForWidth());
        editButton->setSizePolicy(sizePolicy);
        editButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        editButton->setAutoRaise(true);

        hboxLayout->addWidget(editButton);

        renameButton = new QToolButton(QtGradientView);
        renameButton->setObjectName(QString::fromUtf8("renameButton"));
        sizePolicy.setHeightForWidth(renameButton->sizePolicy().hasHeightForWidth());
        renameButton->setSizePolicy(sizePolicy);
        renameButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        renameButton->setAutoRaise(true);

        hboxLayout->addWidget(renameButton);

        removeButton = new QToolButton(QtGradientView);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        sizePolicy.setHeightForWidth(removeButton->sizePolicy().hasHeightForWidth());
        removeButton->setSizePolicy(sizePolicy);
        removeButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        removeButton->setAutoRaise(true);

        hboxLayout->addWidget(removeButton);

        spacerItem = new QSpacerItem(71, 26, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout);

        listWidget = new QListWidget(QtGradientView);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        vboxLayout->addWidget(listWidget);

        QWidget::setTabOrder(listWidget, newButton);
        QWidget::setTabOrder(newButton, editButton);
        QWidget::setTabOrder(editButton, renameButton);
        QWidget::setTabOrder(renameButton, removeButton);

        retranslateUi(QtGradientView);

        QMetaObject::connectSlotsByName(QtGradientView);
    } // setupUi

    void retranslateUi(QWidget *QtGradientView)
    {
        QtGradientView->setWindowTitle(QApplication::translate("QtGradientView", "Gradient View", 0, QApplication::UnicodeUTF8));
        newButton->setText(QApplication::translate("QtGradientView", "New...", 0, QApplication::UnicodeUTF8));
        editButton->setText(QApplication::translate("QtGradientView", "Edit...", 0, QApplication::UnicodeUTF8));
        renameButton->setText(QApplication::translate("QtGradientView", "Rename", 0, QApplication::UnicodeUTF8));
        removeButton->setText(QApplication::translate("QtGradientView", "Remove", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QtGradientView: public Ui_QtGradientView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGRADIENTVIEW_H
