/********************************************************************************
** Form generated from reading UI file 'GoToDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOTODIALOG_H
#define UI_GOTODIALOG_H

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
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_GoTo
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *searchLineEdit;
    QPushButton *progressButton;
    QListView *bookmarkListView;
    QDialogButtonBox *buttonBox;
    QLabel *descriptionLabel;
    QHBoxLayout *horizontalLayout;
    QRadioButton *browseButton;
    QRadioButton *searchButton;

    void setupUi(QDialog *GoTo)
    {
        if (GoTo->objectName().isEmpty())
            GoTo->setObjectName(QString::fromUtf8("GoTo"));
        GoTo->resize(368, 269);
        gridLayout = new QGridLayout(GoTo);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        searchLineEdit = new QLineEdit(GoTo);
        searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));

        horizontalLayout_2->addWidget(searchLineEdit);

        progressButton = new QPushButton(GoTo);
        progressButton->setObjectName(QString::fromUtf8("progressButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressButton->sizePolicy().hasHeightForWidth());
        progressButton->setSizePolicy(sizePolicy);
        progressButton->setMinimumSize(QSize(32, 32));
        progressButton->setIconSize(QSize(32, 32));
        progressButton->setAutoDefault(false);
        progressButton->setFlat(true);

        horizontalLayout_2->addWidget(progressButton);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        bookmarkListView = new QListView(GoTo);
        bookmarkListView->setObjectName(QString::fromUtf8("bookmarkListView"));
        bookmarkListView->setProperty("isWrapping", QVariant(false));
        bookmarkListView->setWordWrap(true);

        gridLayout->addWidget(bookmarkListView, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(GoTo);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        gridLayout->addWidget(buttonBox, 1, 1, 1, 1);

        descriptionLabel = new QLabel(GoTo);
        descriptionLabel->setObjectName(QString::fromUtf8("descriptionLabel"));
        descriptionLabel->setWordWrap(true);
        descriptionLabel->setOpenExternalLinks(true);

        gridLayout->addWidget(descriptionLabel, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        browseButton = new QRadioButton(GoTo);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));
        browseButton->setChecked(true);

        horizontalLayout->addWidget(browseButton);

        searchButton = new QRadioButton(GoTo);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));

        horizontalLayout->addWidget(searchButton);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 1);


        retranslateUi(GoTo);
        QObject::connect(buttonBox, SIGNAL(accepted()), GoTo, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GoTo, SLOT(reject()));

        QMetaObject::connectSlotsByName(GoTo);
    } // setupUi

    void retranslateUi(QDialog *GoTo)
    {
        GoTo->setWindowTitle(QApplication::translate("GoTo", "Go To...", 0, QApplication::UnicodeUTF8));
        progressButton->setText(QString());
        descriptionLabel->setText(QApplication::translate("GoTo", "Enter a search term and press Enter. <a href=\"http://userbase.kde.org/Marble/Search\">Details...</a>", 0, QApplication::UnicodeUTF8));
        browseButton->setText(QApplication::translate("GoTo", "Browse", 0, QApplication::UnicodeUTF8));
        searchButton->setText(QApplication::translate("GoTo", "Search", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GoTo: public Ui_GoTo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOTODIALOG_H
