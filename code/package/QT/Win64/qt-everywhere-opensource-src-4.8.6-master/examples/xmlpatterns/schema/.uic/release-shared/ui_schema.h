/********************************************************************************
** Form generated from reading UI file 'schema.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEMA_H
#define UI_SCHEMA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SchemaMainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *schemaLabel;
    QComboBox *schemaSelection;
    QTextBrowser *schemaView;
    QLabel *instanceLabel;
    QComboBox *instanceSelection;
    QTextEdit *instanceEdit;
    QLabel *label;
    QLabel *validationStatus;
    QPushButton *validateButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SchemaMainWindow)
    {
        if (SchemaMainWindow->objectName().isEmpty())
            SchemaMainWindow->setObjectName(QString::fromUtf8("SchemaMainWindow"));
        SchemaMainWindow->resize(417, 594);
        centralwidget = new QWidget(SchemaMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        schemaLabel = new QLabel(centralwidget);
        schemaLabel->setObjectName(QString::fromUtf8("schemaLabel"));

        gridLayout->addWidget(schemaLabel, 0, 0, 1, 2);

        schemaSelection = new QComboBox(centralwidget);
        schemaSelection->setObjectName(QString::fromUtf8("schemaSelection"));

        gridLayout->addWidget(schemaSelection, 0, 2, 1, 2);

        schemaView = new QTextBrowser(centralwidget);
        schemaView->setObjectName(QString::fromUtf8("schemaView"));

        gridLayout->addWidget(schemaView, 1, 0, 1, 4);

        instanceLabel = new QLabel(centralwidget);
        instanceLabel->setObjectName(QString::fromUtf8("instanceLabel"));

        gridLayout->addWidget(instanceLabel, 2, 0, 1, 2);

        instanceSelection = new QComboBox(centralwidget);
        instanceSelection->setObjectName(QString::fromUtf8("instanceSelection"));

        gridLayout->addWidget(instanceSelection, 2, 2, 1, 2);

        instanceEdit = new QTextEdit(centralwidget);
        instanceEdit->setObjectName(QString::fromUtf8("instanceEdit"));

        gridLayout->addWidget(instanceEdit, 3, 0, 1, 4);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);

        validationStatus = new QLabel(centralwidget);
        validationStatus->setObjectName(QString::fromUtf8("validationStatus"));

        gridLayout->addWidget(validationStatus, 4, 1, 1, 2);

        validateButton = new QPushButton(centralwidget);
        validateButton->setObjectName(QString::fromUtf8("validateButton"));

        gridLayout->addWidget(validateButton, 4, 3, 1, 1);

        SchemaMainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(SchemaMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        SchemaMainWindow->setStatusBar(statusbar);

        retranslateUi(SchemaMainWindow);

        QMetaObject::connectSlotsByName(SchemaMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SchemaMainWindow)
    {
        SchemaMainWindow->setWindowTitle(QApplication::translate("SchemaMainWindow", "XML Schema Validation", 0, QApplication::UnicodeUTF8));
        schemaLabel->setText(QApplication::translate("SchemaMainWindow", "XML Schema Document:", 0, QApplication::UnicodeUTF8));
        instanceLabel->setText(QApplication::translate("SchemaMainWindow", "XML Instance Document:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SchemaMainWindow", "Status:", 0, QApplication::UnicodeUTF8));
        validationStatus->setText(QApplication::translate("SchemaMainWindow", "not validated", 0, QApplication::UnicodeUTF8));
        validateButton->setText(QApplication::translate("SchemaMainWindow", "Validate", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SchemaMainWindow: public Ui_SchemaMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEMA_H
