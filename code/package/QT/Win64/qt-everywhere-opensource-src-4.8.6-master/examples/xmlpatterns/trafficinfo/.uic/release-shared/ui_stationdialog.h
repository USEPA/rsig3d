/********************************************************************************
** Form generated from reading UI file 'stationdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONDIALOG_H
#define UI_STATIONDIALOG_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_StationDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *m_input;
    QPushButton *m_searchButton;
    QListView *m_view;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *m_line1;
    QLineEdit *m_line2;
    QLineEdit *m_line3;
    QLineEdit *m_line4;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *StationDialog)
    {
        if (StationDialog->objectName().isEmpty())
            StationDialog->setObjectName(QString::fromUtf8("StationDialog"));
        StationDialog->resize(462, 391);
        verticalLayout = new QVBoxLayout(StationDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_input = new QLineEdit(StationDialog);
        m_input->setObjectName(QString::fromUtf8("m_input"));

        gridLayout->addWidget(m_input, 0, 0, 1, 1);

        m_searchButton = new QPushButton(StationDialog);
        m_searchButton->setObjectName(QString::fromUtf8("m_searchButton"));

        gridLayout->addWidget(m_searchButton, 0, 1, 1, 1);

        m_view = new QListView(StationDialog);
        m_view->setObjectName(QString::fromUtf8("m_view"));

        gridLayout->addWidget(m_view, 1, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(StationDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        m_line1 = new QLineEdit(StationDialog);
        m_line1->setObjectName(QString::fromUtf8("m_line1"));

        horizontalLayout->addWidget(m_line1);

        m_line2 = new QLineEdit(StationDialog);
        m_line2->setObjectName(QString::fromUtf8("m_line2"));

        horizontalLayout->addWidget(m_line2);

        m_line3 = new QLineEdit(StationDialog);
        m_line3->setObjectName(QString::fromUtf8("m_line3"));

        horizontalLayout->addWidget(m_line3);

        m_line4 = new QLineEdit(StationDialog);
        m_line4->setObjectName(QString::fromUtf8("m_line4"));

        horizontalLayout->addWidget(m_line4);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 2);

        buttonBox = new QDialogButtonBox(StationDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 2);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(StationDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), StationDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), StationDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(StationDialog);
    } // setupUi

    void retranslateUi(QDialog *StationDialog)
    {
        StationDialog->setWindowTitle(QApplication::translate("StationDialog", "Select Station", 0, QApplication::UnicodeUTF8));
        m_searchButton->setText(QApplication::translate("StationDialog", "Search", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StationDialog", "Lines:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StationDialog: public Ui_StationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONDIALOG_H
