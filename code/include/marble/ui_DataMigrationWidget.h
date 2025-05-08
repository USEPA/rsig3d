/********************************************************************************
** Form generated from reading UI file 'DataMigrationWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAMIGRATIONWIDGET_H
#define UI_DATAMIGRATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DataMigrationWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *m_label;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DataMigrationWidget)
    {
        if (DataMigrationWidget->objectName().isEmpty())
            DataMigrationWidget->setObjectName(QString::fromUtf8("DataMigrationWidget"));
        DataMigrationWidget->resize(413, 200);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DataMigrationWidget->sizePolicy().hasHeightForWidth());
        DataMigrationWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(DataMigrationWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_label = new QLabel(DataMigrationWidget);
        m_label->setObjectName(QString::fromUtf8("m_label"));
        m_label->setTextFormat(Qt::RichText);
        m_label->setWordWrap(true);

        verticalLayout->addWidget(m_label);

        verticalSpacer = new QSpacerItem(20, 144, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(DataMigrationWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::No|QDialogButtonBox::Yes);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DataMigrationWidget);
        QObject::connect(buttonBox, SIGNAL(accepted()), DataMigrationWidget, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DataMigrationWidget, SLOT(reject()));

        QMetaObject::connectSlotsByName(DataMigrationWidget);
    } // setupUi

    void retranslateUi(QDialog *DataMigrationWidget)
    {
        DataMigrationWidget->setWindowTitle(QApplication::translate("DataMigrationWidget", "Data Migration", 0, QApplication::UnicodeUTF8));
        m_label->setText(QApplication::translate("DataMigrationWidget", "Marble has found map data stored by a previous Marble version.<br/>Should this data get converted so that it can be used by the current version?", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataMigrationWidget: public Ui_DataMigrationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAMIGRATIONWIDGET_H
