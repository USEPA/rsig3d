/********************************************************************************
** Form generated from reading UI file 'schema_mobiles.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEMA_MOBILES_H
#define UI_SCHEMA_MOBILES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SchemaMainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QComboBox *schemaSelection;
    QTextBrowser *schemaView;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout;
    QComboBox *instanceSelection;
    QTextBrowser *instanceEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *validationStatus;
    QPushButton *validateButton;

    void setupUi(QMainWindow *SchemaMainWindow)
    {
        if (SchemaMainWindow->objectName().isEmpty())
            SchemaMainWindow->setObjectName(QString::fromUtf8("SchemaMainWindow"));
        SchemaMainWindow->resize(187, 179);
        centralwidget = new QWidget(SchemaMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_3 = new QVBoxLayout(centralwidget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetNoConstraint);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        schemaSelection = new QComboBox(tab);
        schemaSelection->setObjectName(QString::fromUtf8("schemaSelection"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(schemaSelection->sizePolicy().hasHeightForWidth());
        schemaSelection->setSizePolicy(sizePolicy);
        schemaSelection->setMaxVisibleItems(3);

        verticalLayout_2->addWidget(schemaSelection);

        schemaView = new QTextBrowser(tab);
        schemaView->setObjectName(QString::fromUtf8("schemaView"));

        verticalLayout_2->addWidget(schemaView);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout = new QVBoxLayout(tab_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        instanceSelection = new QComboBox(tab_2);
        instanceSelection->setObjectName(QString::fromUtf8("instanceSelection"));
        sizePolicy.setHeightForWidth(instanceSelection->sizePolicy().hasHeightForWidth());
        instanceSelection->setSizePolicy(sizePolicy);
        instanceSelection->setMaxVisibleItems(3);

        verticalLayout->addWidget(instanceSelection);

        instanceEdit = new QTextBrowser(tab_2);
        instanceEdit->setObjectName(QString::fromUtf8("instanceEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(instanceEdit->sizePolicy().hasHeightForWidth());
        instanceEdit->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(8);
        instanceEdit->setFont(font);
        instanceEdit->setReadOnly(false);

        verticalLayout->addWidget(instanceEdit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        validationStatus = new QLabel(tab_2);
        validationStatus->setObjectName(QString::fromUtf8("validationStatus"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(validationStatus->sizePolicy().hasHeightForWidth());
        validationStatus->setSizePolicy(sizePolicy2);
        validationStatus->setWordWrap(true);

        horizontalLayout_2->addWidget(validationStatus);

        validateButton = new QPushButton(tab_2);
        validateButton->setObjectName(QString::fromUtf8("validateButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(validateButton->sizePolicy().hasHeightForWidth());
        validateButton->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(validateButton);


        verticalLayout->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_3->addWidget(tabWidget);

        SchemaMainWindow->setCentralWidget(centralwidget);

        retranslateUi(SchemaMainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(SchemaMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SchemaMainWindow)
    {
        SchemaMainWindow->setWindowTitle(QApplication::translate("SchemaMainWindow", "XML Schema Validation", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SchemaMainWindow", "XML Schema", 0, QApplication::UnicodeUTF8));
        validationStatus->setText(QApplication::translate("SchemaMainWindow", "Not validated", 0, QApplication::UnicodeUTF8));
        validateButton->setText(QApplication::translate("SchemaMainWindow", "Validate", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SchemaMainWindow", "XML Instance", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SchemaMainWindow: public Ui_SchemaMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEMA_MOBILES_H
