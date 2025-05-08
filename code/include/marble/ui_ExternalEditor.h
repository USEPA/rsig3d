/********************************************************************************
** Form generated from reading UI file 'ExternalEditor.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTERNALEDITOR_H
#define UI_EXTERNALEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ExternalEditor
{
public:
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *label_2;
    QComboBox *editorComboBox;
    QCheckBox *saveDefaultCheckBox;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *screenshotLabel;
    QLabel *descriptionLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExternalEditor)
    {
        if (ExternalEditor->objectName().isEmpty())
            ExternalEditor->setObjectName(QString::fromUtf8("ExternalEditor"));
        ExternalEditor->resize(387, 465);
        verticalLayout_2 = new QVBoxLayout(ExternalEditor);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_2 = new QLabel(ExternalEditor);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        editorComboBox = new QComboBox(ExternalEditor);
        editorComboBox->setObjectName(QString::fromUtf8("editorComboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, editorComboBox);

        saveDefaultCheckBox = new QCheckBox(ExternalEditor);
        saveDefaultCheckBox->setObjectName(QString::fromUtf8("saveDefaultCheckBox"));
        saveDefaultCheckBox->setChecked(false);

        formLayout->setWidget(1, QFormLayout::SpanningRole, saveDefaultCheckBox);


        verticalLayout_2->addLayout(formLayout);

        groupBox = new QGroupBox(ExternalEditor);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        screenshotLabel = new QLabel(groupBox);
        screenshotLabel->setObjectName(QString::fromUtf8("screenshotLabel"));
        screenshotLabel->setMinimumSize(QSize(360, 280));
        screenshotLabel->setFrameShape(QFrame::Box);
        screenshotLabel->setPixmap(QPixmap(QString::fromUtf8(":/data/editors/potlatch.png")));

        verticalLayout->addWidget(screenshotLabel);

        descriptionLabel = new QLabel(groupBox);
        descriptionLabel->setObjectName(QString::fromUtf8("descriptionLabel"));
        descriptionLabel->setMinimumSize(QSize(0, 50));
        descriptionLabel->setWordWrap(true);

        verticalLayout->addWidget(descriptionLabel);


        verticalLayout_2->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(ExternalEditor);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(ExternalEditor);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExternalEditor, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExternalEditor, SLOT(reject()));

        QMetaObject::connectSlotsByName(ExternalEditor);
    } // setupUi

    void retranslateUi(QDialog *ExternalEditor)
    {
        ExternalEditor->setWindowTitle(QApplication::translate("ExternalEditor", "External Map Editor Selection", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ExternalEditor", "External Map Editor", 0, QApplication::UnicodeUTF8));
        editorComboBox->clear();
        editorComboBox->insertItems(0, QStringList()
         << QApplication::translate("ExternalEditor", "Web browser (Potlatch)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ExternalEditor", "Merkaartor", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ExternalEditor", "JOSM", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        saveDefaultCheckBox->setToolTip(QApplication::translate("ExternalEditor", "Deactivate to be prompted for a map editor selection next time again. Otherwise, your choice is remembered and will be used automatically in the future. If you want to alter the default map editor later, you can do so in the Marble Settings.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        saveDefaultCheckBox->setText(QApplication::translate("ExternalEditor", "Make my selection the default map editor", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ExternalEditor", "Editor Details", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        screenshotLabel->setToolTip(QApplication::translate("ExternalEditor", "Editor Preview", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        screenshotLabel->setText(QString());
        descriptionLabel->setText(QApplication::translate("ExternalEditor", "Potlatch is an OpenStreetMap editor for quick editing tasks and beginners. It requires a web browser with flash support.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExternalEditor: public Ui_ExternalEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTERNALEDITOR_H
