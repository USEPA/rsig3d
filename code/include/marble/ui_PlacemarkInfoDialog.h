/********************************************************************************
** Form generated from reading UI file 'PlacemarkInfoDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLACEMARKINFODIALOG_H
#define UI_PLACEMARKINFODIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "TinyWebBrowser.h"

QT_BEGIN_NAMESPACE

class Ui_PlacemarkInfoDialog
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *m_pTabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *name_val_lbl;
    QSpacerItem *spacerItem;
    QLabel *flag_val_lbl;
    QLabel *role_val_lbl;
    QLabel *altername_val_lbl;
    QLabel *description_lbl;
    QTextBrowser *description_val_browser;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *coordinates_lbl;
    QLabel *coordinates_val_lbl;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout1;
    QLabel *elevation_lbl;
    QLabel *elevation_val_lbl;
    QSpacerItem *horizontalSpacer;
    QLabel *country_lbl;
    QLabel *country_val_lbl;
    QLabel *population_lbl;
    QLabel *population_val_lbl;
    QLabel *area_lbl;
    QLabel *area_val_lbl;
    QLabel *diameter_lbl;
    QLabel *diameter_val_lbl;
    QLabel *gmtdst_lbl;
    QLabel *gmtdst_val_lbl;
    QLabel *state_lbl;
    QLabel *state_val_lbl;
    QSpacerItem *spacerItem1;
    QWidget *tab_2;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout;
    QToolButton *m_pBackButton;
    QToolButton *m_pPrintButton;
    QSpacerItem *spacerItem2;
    TinyWebBrowser *m_pWikipediaBrowser;
    QLabel *m_pStatusLabel;
    QFrame *m_pLine;
    QDialogButtonBox *m_pButtonBox;

    void setupUi(QDialog *PlacemarkInfoDialog)
    {
        if (PlacemarkInfoDialog->objectName().isEmpty())
            PlacemarkInfoDialog->setObjectName(QString::fromUtf8("PlacemarkInfoDialog"));
        PlacemarkInfoDialog->resize(780, 548);
        vboxLayout = new QVBoxLayout(PlacemarkInfoDialog);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        m_pTabWidget = new QTabWidget(PlacemarkInfoDialog);
        m_pTabWidget->setObjectName(QString::fromUtf8("m_pTabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(0, 0, 0, 0);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        name_val_lbl = new QLabel(tab);
        name_val_lbl->setObjectName(QString::fromUtf8("name_val_lbl"));
        name_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout->addWidget(name_val_lbl, 0, 0, 1, 1);

        spacerItem = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacerItem, 3, 0, 1, 1);

        flag_val_lbl = new QLabel(tab);
        flag_val_lbl->setObjectName(QString::fromUtf8("flag_val_lbl"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(flag_val_lbl->sizePolicy().hasHeightForWidth());
        flag_val_lbl->setSizePolicy(sizePolicy);
        flag_val_lbl->setMinimumSize(QSize(120, 90));

        gridLayout->addWidget(flag_val_lbl, 0, 1, 4, 1);

        role_val_lbl = new QLabel(tab);
        role_val_lbl->setObjectName(QString::fromUtf8("role_val_lbl"));
        role_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout->addWidget(role_val_lbl, 2, 0, 1, 1);

        altername_val_lbl = new QLabel(tab);
        altername_val_lbl->setObjectName(QString::fromUtf8("altername_val_lbl"));
        altername_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout->addWidget(altername_val_lbl, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        description_lbl = new QLabel(tab);
        description_lbl->setObjectName(QString::fromUtf8("description_lbl"));

        verticalLayout->addWidget(description_lbl);

        description_val_browser = new QTextBrowser(tab);
        description_val_browser->setObjectName(QString::fromUtf8("description_val_browser"));
        description_val_browser->setEnabled(false);

        verticalLayout->addWidget(description_val_browser);

        verticalSpacer = new QSpacerItem(15, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        coordinates_lbl = new QLabel(tab);
        coordinates_lbl->setObjectName(QString::fromUtf8("coordinates_lbl"));
        coordinates_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        coordinates_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(coordinates_lbl);

        coordinates_val_lbl = new QLabel(tab);
        coordinates_val_lbl->setObjectName(QString::fromUtf8("coordinates_val_lbl"));
        coordinates_val_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        coordinates_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        horizontalLayout->addWidget(coordinates_val_lbl);

        horizontalSpacer_2 = new QSpacerItem(15, 15, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(15, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        gridLayout1 = new QGridLayout();
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        elevation_lbl = new QLabel(tab);
        elevation_lbl->setObjectName(QString::fromUtf8("elevation_lbl"));
        elevation_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        elevation_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(elevation_lbl, 0, 0, 1, 1);

        elevation_val_lbl = new QLabel(tab);
        elevation_val_lbl->setObjectName(QString::fromUtf8("elevation_val_lbl"));
        elevation_val_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        elevation_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(elevation_val_lbl, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(20, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout1->addItem(horizontalSpacer, 0, 2, 1, 1);

        country_lbl = new QLabel(tab);
        country_lbl->setObjectName(QString::fromUtf8("country_lbl"));
        country_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        country_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(country_lbl, 0, 3, 1, 1);

        country_val_lbl = new QLabel(tab);
        country_val_lbl->setObjectName(QString::fromUtf8("country_val_lbl"));
        country_val_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        country_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(country_val_lbl, 0, 4, 1, 1);

        population_lbl = new QLabel(tab);
        population_lbl->setObjectName(QString::fromUtf8("population_lbl"));
        population_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        population_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(population_lbl, 1, 0, 1, 1);

        population_val_lbl = new QLabel(tab);
        population_val_lbl->setObjectName(QString::fromUtf8("population_val_lbl"));
        population_val_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        population_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(population_val_lbl, 1, 1, 1, 1);

        area_lbl = new QLabel(tab);
        area_lbl->setObjectName(QString::fromUtf8("area_lbl"));
        area_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        area_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(area_lbl, 2, 0, 1, 1);

        area_val_lbl = new QLabel(tab);
        area_val_lbl->setObjectName(QString::fromUtf8("area_val_lbl"));
        area_val_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        area_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(area_val_lbl, 2, 1, 1, 1);

        diameter_lbl = new QLabel(tab);
        diameter_lbl->setObjectName(QString::fromUtf8("diameter_lbl"));
        diameter_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        diameter_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(diameter_lbl, 3, 0, 1, 1);

        diameter_val_lbl = new QLabel(tab);
        diameter_val_lbl->setObjectName(QString::fromUtf8("diameter_val_lbl"));
        diameter_val_lbl->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        diameter_val_lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);

        gridLayout1->addWidget(diameter_val_lbl, 3, 1, 1, 1);

        gmtdst_lbl = new QLabel(tab);
        gmtdst_lbl->setObjectName(QString::fromUtf8("gmtdst_lbl"));

        gridLayout1->addWidget(gmtdst_lbl, 2, 3, 1, 1);

        gmtdst_val_lbl = new QLabel(tab);
        gmtdst_val_lbl->setObjectName(QString::fromUtf8("gmtdst_val_lbl"));

        gridLayout1->addWidget(gmtdst_val_lbl, 2, 4, 1, 1);

        state_lbl = new QLabel(tab);
        state_lbl->setObjectName(QString::fromUtf8("state_lbl"));

        gridLayout1->addWidget(state_lbl, 1, 3, 1, 1);

        state_val_lbl = new QLabel(tab);
        state_val_lbl->setObjectName(QString::fromUtf8("state_val_lbl"));

        gridLayout1->addWidget(state_val_lbl, 1, 4, 1, 1);


        verticalLayout->addLayout(gridLayout1);

        spacerItem1 = new QSpacerItem(330, 16, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(spacerItem1);

        m_pTabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        vboxLayout1 = new QVBoxLayout(tab_2);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout1->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        m_pBackButton = new QToolButton(tab_2);
        m_pBackButton->setObjectName(QString::fromUtf8("m_pBackButton"));
        m_pBackButton->setEnabled(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../../../../../.designer/backup"), QSize(), QIcon::Normal, QIcon::Off);
        m_pBackButton->setIcon(icon);
        m_pBackButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        hboxLayout->addWidget(m_pBackButton);

        m_pPrintButton = new QToolButton(tab_2);
        m_pPrintButton->setObjectName(QString::fromUtf8("m_pPrintButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/document-print.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_pPrintButton->setIcon(icon1);
        m_pPrintButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        hboxLayout->addWidget(m_pPrintButton);

        spacerItem2 = new QSpacerItem(281, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem2);


        vboxLayout1->addLayout(hboxLayout);

        m_pWikipediaBrowser = new TinyWebBrowser(tab_2);
        m_pWikipediaBrowser->setObjectName(QString::fromUtf8("m_pWikipediaBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_pWikipediaBrowser->sizePolicy().hasHeightForWidth());
        m_pWikipediaBrowser->setSizePolicy(sizePolicy1);

        vboxLayout1->addWidget(m_pWikipediaBrowser);

        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/wikipedia.png"), QSize(), QIcon::Normal, QIcon::Off);
        m_pTabWidget->addTab(tab_2, icon2, QString());

        vboxLayout->addWidget(m_pTabWidget);

        m_pStatusLabel = new QLabel(PlacemarkInfoDialog);
        m_pStatusLabel->setObjectName(QString::fromUtf8("m_pStatusLabel"));

        vboxLayout->addWidget(m_pStatusLabel);

        m_pLine = new QFrame(PlacemarkInfoDialog);
        m_pLine->setObjectName(QString::fromUtf8("m_pLine"));
        m_pLine->setFrameShape(QFrame::HLine);
        m_pLine->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(m_pLine);

        m_pButtonBox = new QDialogButtonBox(PlacemarkInfoDialog);
        m_pButtonBox->setObjectName(QString::fromUtf8("m_pButtonBox"));
        m_pButtonBox->setOrientation(Qt::Horizontal);
        m_pButtonBox->setStandardButtons(QDialogButtonBox::Close);

        vboxLayout->addWidget(m_pButtonBox);

#ifndef QT_NO_SHORTCUT
        description_lbl->setBuddy(description_val_browser);
#endif // QT_NO_SHORTCUT

        retranslateUi(PlacemarkInfoDialog);
        QObject::connect(m_pButtonBox, SIGNAL(clicked(QAbstractButton*)), PlacemarkInfoDialog, SLOT(accept()));

        m_pTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PlacemarkInfoDialog);
    } // setupUi

    void retranslateUi(QDialog *PlacemarkInfoDialog)
    {
        PlacemarkInfoDialog->setWindowTitle(QApplication::translate("PlacemarkInfoDialog", "Marble Infocenter", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        name_val_lbl->setToolTip(QApplication::translate("PlacemarkInfoDialog", "Name of the location", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        name_val_lbl->setWhatsThis(QApplication::translate("PlacemarkInfoDialog", "Name of the location", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        name_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600;\">Name</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        flag_val_lbl->setText(QString());
#ifndef QT_NO_TOOLTIP
        role_val_lbl->setToolTip(QApplication::translate("PlacemarkInfoDialog", "Role", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        role_val_lbl->setWhatsThis(QApplication::translate("PlacemarkInfoDialog", "Role of the location.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        role_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Role</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        altername_val_lbl->setToolTip(QApplication::translate("PlacemarkInfoDialog", "Alternative name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        altername_val_lbl->setWhatsThis(QApplication::translate("PlacemarkInfoDialog", "Alternative name of the location.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        altername_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Alternative Name", 0, QApplication::UnicodeUTF8));
        description_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Short description:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        description_val_browser->setToolTip(QApplication::translate("PlacemarkInfoDialog", "Description", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        description_val_browser->setWhatsThis(QApplication::translate("PlacemarkInfoDialog", "Description field", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        description_val_browser->setHtml(QApplication::translate("PlacemarkInfoDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">No description available.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">If you are connected to the Internet please click the &quot;Wikipedia&quot; tab at the top of this page to get more information.</p></body></html>", 0, QApplication::UnicodeUTF8));
        coordinates_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Coordinates:", 0, QApplication::UnicodeUTF8));
        coordinates_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "000\302\260 00' 00\" N\n"
"000\302\260 00' 00\" E ", 0, QApplication::UnicodeUTF8));
        elevation_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Elevation:", 0, QApplication::UnicodeUTF8));
        elevation_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "9000.0 m", 0, QApplication::UnicodeUTF8));
        country_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Country:", 0, QApplication::UnicodeUTF8));
        country_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Country", 0, QApplication::UnicodeUTF8));
        population_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Population:", 0, QApplication::UnicodeUTF8));
        population_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "1000000", 0, QApplication::UnicodeUTF8));
        area_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Area:", 0, QApplication::UnicodeUTF8));
        area_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "0.0 sq km", 0, QApplication::UnicodeUTF8));
        diameter_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Diameter:", 0, QApplication::UnicodeUTF8));
        diameter_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "TextLabel", "KDE::DoNotExtract", QApplication::UnicodeUTF8));
        gmtdst_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "Timezone (Normal/DST):", 0, QApplication::UnicodeUTF8));
        gmtdst_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "gmt / dst", 0, QApplication::UnicodeUTF8));
        state_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "State:", 0, QApplication::UnicodeUTF8));
        state_val_lbl->setText(QApplication::translate("PlacemarkInfoDialog", "State", 0, QApplication::UnicodeUTF8));
        m_pTabWidget->setTabText(m_pTabWidget->indexOf(tab), QApplication::translate("PlacemarkInfoDialog", "Data Sheet", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_pBackButton->setToolTip(QApplication::translate("PlacemarkInfoDialog", "Back", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        m_pBackButton->setWhatsThis(QApplication::translate("PlacemarkInfoDialog", "Move backwards one step in the browsing history.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        m_pBackButton->setText(QApplication::translate("PlacemarkInfoDialog", "Back", 0, QApplication::UnicodeUTF8));
        m_pPrintButton->setText(QApplication::translate("PlacemarkInfoDialog", "Print Page", 0, QApplication::UnicodeUTF8));
        m_pTabWidget->setTabText(m_pTabWidget->indexOf(tab_2), QApplication::translate("PlacemarkInfoDialog", "Wikipedia", 0, QApplication::UnicodeUTF8));
        m_pStatusLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PlacemarkInfoDialog: public Ui_PlacemarkInfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLACEMARKINFODIALOG_H
