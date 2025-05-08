/********************************************************************************
** Form generated from reading UI file 'MapWizard.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPWIZARD_H
#define UI_MAPWIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_MapWizard
{
public:
    QWizardPage *wizardPageIntro;
    QFormLayout *formLayout_3;
    QLabel *labelIntroInfo;
    QRadioButton *radioButtonWms;
    QRadioButton *radioButtonBitmap;
    QRadioButton *radioButtonStaticUrl;
    QWizardPage *wizardPageWms;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer_10;
    QGridLayout *gridLayout;
    QLabel *labelWmsServer;
    QComboBox *comboBoxWmsServer;
    QLabel *labelWmsUrl;
    QLineEdit *lineEditWmsUrl;
    QSpacerItem *verticalSpacer_9;
    QWizardPage *wizardPageWmsAvailableMaps;
    QGridLayout *gridLayout_4;
    QLabel *labelWmsMaps;
    QListWidget *listWidgetWmsMaps;
    QWizardPage *wizardPageBitmap;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLabel *labelSource;
    QLineEdit *lineEditSource;
    QPushButton *pushButtonSource;
    QSpacerItem *verticalSpacer;
    QWizardPage *wizardPageStaticUrl;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QSpacerItem *verticalSpacer_6;
    QFormLayout *formLayout_4;
    QLabel *labelStaticUrlServer;
    QComboBox *comboBoxStaticUrlServer;
    QSpacerItem *verticalSpacer_2;
    QWizardPage *wizardPageMetadata;
    QGridLayout *gridLayout_2;
    QLabel *label_6;
    QLabel *labelMapTitle;
    QLineEdit *lineEditTitle;
    QLabel *labelMapTheme;
    QLineEdit *lineEditTheme;
    QLabel *labelMapDesc;
    QTextEdit *textEditDesc;
    QLabel *labelMapIcon;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QLabel *labelPreview;
    QPushButton *pushButtonPreview;
    QSpacerItem *horizontalSpacer_3;
    QWizardPage *wizardPageLegend;
    QGridLayout *gridLayout_3;
    QLabel *labelLegend_2;
    QLineEdit *lineEditLegend_2;
    QPushButton *pushButtonLegend_2;
    QHBoxLayout *horizontalLayout_2;
    QTextBrowser *textBrowserLegend;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *checkBoxInterest;
    QCheckBox *checkBoxPop;
    QCheckBox *checkBoxTer;
    QCheckBox *checkBoxCoord;
    QCheckBox *checkBoxBorder;
    QWizardPage *wizardPageSummary;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_7;
    QFormLayout *formLayout;
    QLabel *labelSumName;
    QLabel *labelSumMName;
    QLabel *labelSumTheme;
    QLabel *labelSumMTheme;
    QLabel *labelSumMPreview;
    QLabel *labelThumbnail;
    QPushButton *pushButtonPreviewMap;

    void setupUi(QWizard *MapWizard)
    {
        if (MapWizard->objectName().isEmpty())
            MapWizard->setObjectName(QString::fromUtf8("MapWizard"));
        MapWizard->resize(600, 392);
        MapWizard->setMinimumSize(QSize(600, 392));
        MapWizard->setWizardStyle(QWizard::ClassicStyle);
        wizardPageIntro = new QWizardPage();
        wizardPageIntro->setObjectName(QString::fromUtf8("wizardPageIntro"));
        formLayout_3 = new QFormLayout(wizardPageIntro);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        labelIntroInfo = new QLabel(wizardPageIntro);
        labelIntroInfo->setObjectName(QString::fromUtf8("labelIntroInfo"));
        labelIntroInfo->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelIntroInfo->setWordWrap(true);

        formLayout_3->setWidget(0, QFormLayout::SpanningRole, labelIntroInfo);

        radioButtonWms = new QRadioButton(wizardPageIntro);
        radioButtonWms->setObjectName(QString::fromUtf8("radioButtonWms"));
        radioButtonWms->setChecked(true);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, radioButtonWms);

        radioButtonBitmap = new QRadioButton(wizardPageIntro);
        radioButtonBitmap->setObjectName(QString::fromUtf8("radioButtonBitmap"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, radioButtonBitmap);

        radioButtonStaticUrl = new QRadioButton(wizardPageIntro);
        radioButtonStaticUrl->setObjectName(QString::fromUtf8("radioButtonStaticUrl"));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, radioButtonStaticUrl);

        MapWizard->setPage(0, wizardPageIntro);
        wizardPageWms = new QWizardPage();
        wizardPageWms->setObjectName(QString::fromUtf8("wizardPageWms"));
        verticalLayout = new QVBoxLayout(wizardPageWms);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(wizardPageWms);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_10);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelWmsServer = new QLabel(wizardPageWms);
        labelWmsServer->setObjectName(QString::fromUtf8("labelWmsServer"));
        labelWmsServer->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelWmsServer, 0, 0, 1, 1);

        comboBoxWmsServer = new QComboBox(wizardPageWms);
        comboBoxWmsServer->setObjectName(QString::fromUtf8("comboBoxWmsServer"));

        gridLayout->addWidget(comboBoxWmsServer, 0, 1, 1, 1);

        labelWmsUrl = new QLabel(wizardPageWms);
        labelWmsUrl->setObjectName(QString::fromUtf8("labelWmsUrl"));
        labelWmsUrl->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(labelWmsUrl, 1, 0, 1, 1);

        lineEditWmsUrl = new QLineEdit(wizardPageWms);
        lineEditWmsUrl->setObjectName(QString::fromUtf8("lineEditWmsUrl"));

        gridLayout->addWidget(lineEditWmsUrl, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_9);

        MapWizard->addPage(wizardPageWms);
        wizardPageWmsAvailableMaps = new QWizardPage();
        wizardPageWmsAvailableMaps->setObjectName(QString::fromUtf8("wizardPageWmsAvailableMaps"));
        gridLayout_4 = new QGridLayout(wizardPageWmsAvailableMaps);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        labelWmsMaps = new QLabel(wizardPageWmsAvailableMaps);
        labelWmsMaps->setObjectName(QString::fromUtf8("labelWmsMaps"));
        labelWmsMaps->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_4->addWidget(labelWmsMaps, 0, 0, 2, 1);

        listWidgetWmsMaps = new QListWidget(wizardPageWmsAvailableMaps);
        listWidgetWmsMaps->setObjectName(QString::fromUtf8("listWidgetWmsMaps"));

        gridLayout_4->addWidget(listWidgetWmsMaps, 0, 1, 2, 1);

        MapWizard->addPage(wizardPageWmsAvailableMaps);
        wizardPageBitmap = new QWizardPage();
        wizardPageBitmap->setObjectName(QString::fromUtf8("wizardPageBitmap"));
        verticalLayout_2 = new QVBoxLayout(wizardPageBitmap);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(wizardPageBitmap);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        verticalLayout_2->addWidget(label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelSource = new QLabel(wizardPageBitmap);
        labelSource->setObjectName(QString::fromUtf8("labelSource"));
        labelSource->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(labelSource);

        lineEditSource = new QLineEdit(wizardPageBitmap);
        lineEditSource->setObjectName(QString::fromUtf8("lineEditSource"));

        horizontalLayout->addWidget(lineEditSource);

        pushButtonSource = new QPushButton(wizardPageBitmap);
        pushButtonSource->setObjectName(QString::fromUtf8("pushButtonSource"));

        horizontalLayout->addWidget(pushButtonSource);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        MapWizard->addPage(wizardPageBitmap);
        wizardPageStaticUrl = new QWizardPage();
        wizardPageStaticUrl->setObjectName(QString::fromUtf8("wizardPageStaticUrl"));
        verticalLayout_3 = new QVBoxLayout(wizardPageStaticUrl);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_4 = new QLabel(wizardPageStaticUrl);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setWordWrap(true);

        verticalLayout_3->addWidget(label_4);

        verticalSpacer_6 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer_6);

        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        labelStaticUrlServer = new QLabel(wizardPageStaticUrl);
        labelStaticUrlServer->setObjectName(QString::fromUtf8("labelStaticUrlServer"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, labelStaticUrlServer);

        comboBoxStaticUrlServer = new QComboBox(wizardPageStaticUrl);
        comboBoxStaticUrlServer->setObjectName(QString::fromUtf8("comboBoxStaticUrlServer"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBoxStaticUrlServer->sizePolicy().hasHeightForWidth());
        comboBoxStaticUrlServer->setSizePolicy(sizePolicy);
        comboBoxStaticUrlServer->setEditable(true);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, comboBoxStaticUrlServer);


        verticalLayout_3->addLayout(formLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        MapWizard->addPage(wizardPageStaticUrl);
        wizardPageMetadata = new QWizardPage();
        wizardPageMetadata->setObjectName(QString::fromUtf8("wizardPageMetadata"));
        gridLayout_2 = new QGridLayout(wizardPageMetadata);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_6 = new QLabel(wizardPageMetadata);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setWordWrap(true);

        gridLayout_2->addWidget(label_6, 0, 0, 1, 2);

        labelMapTitle = new QLabel(wizardPageMetadata);
        labelMapTitle->setObjectName(QString::fromUtf8("labelMapTitle"));
        labelMapTitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(labelMapTitle, 1, 0, 1, 1);

        lineEditTitle = new QLineEdit(wizardPageMetadata);
        lineEditTitle->setObjectName(QString::fromUtf8("lineEditTitle"));

        gridLayout_2->addWidget(lineEditTitle, 1, 1, 1, 1);

        labelMapTheme = new QLabel(wizardPageMetadata);
        labelMapTheme->setObjectName(QString::fromUtf8("labelMapTheme"));
        labelMapTheme->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(labelMapTheme, 2, 0, 1, 1);

        lineEditTheme = new QLineEdit(wizardPageMetadata);
        lineEditTheme->setObjectName(QString::fromUtf8("lineEditTheme"));

        gridLayout_2->addWidget(lineEditTheme, 2, 1, 1, 1);

        labelMapDesc = new QLabel(wizardPageMetadata);
        labelMapDesc->setObjectName(QString::fromUtf8("labelMapDesc"));
        labelMapDesc->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);

        gridLayout_2->addWidget(labelMapDesc, 3, 0, 1, 1);

        textEditDesc = new QTextEdit(wizardPageMetadata);
        textEditDesc->setObjectName(QString::fromUtf8("textEditDesc"));

        gridLayout_2->addWidget(textEditDesc, 3, 1, 1, 1);

        labelMapIcon = new QLabel(wizardPageMetadata);
        labelMapIcon->setObjectName(QString::fromUtf8("labelMapIcon"));
        labelMapIcon->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);

        gridLayout_2->addWidget(labelMapIcon, 4, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        labelPreview = new QLabel(wizardPageMetadata);
        labelPreview->setObjectName(QString::fromUtf8("labelPreview"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelPreview->sizePolicy().hasHeightForWidth());
        labelPreview->setSizePolicy(sizePolicy1);
        labelPreview->setMinimumSize(QSize(128, 128));

        verticalLayout_4->addWidget(labelPreview);

        pushButtonPreview = new QPushButton(wizardPageMetadata);
        pushButtonPreview->setObjectName(QString::fromUtf8("pushButtonPreview"));

        verticalLayout_4->addWidget(pushButtonPreview);


        horizontalLayout_5->addLayout(verticalLayout_4);

        horizontalSpacer_3 = new QSpacerItem(354, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        gridLayout_2->addLayout(horizontalLayout_5, 4, 1, 1, 1);

        MapWizard->addPage(wizardPageMetadata);
        wizardPageLegend = new QWizardPage();
        wizardPageLegend->setObjectName(QString::fromUtf8("wizardPageLegend"));
        gridLayout_3 = new QGridLayout(wizardPageLegend);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        labelLegend_2 = new QLabel(wizardPageLegend);
        labelLegend_2->setObjectName(QString::fromUtf8("labelLegend_2"));

        gridLayout_3->addWidget(labelLegend_2, 0, 0, 1, 1);

        lineEditLegend_2 = new QLineEdit(wizardPageLegend);
        lineEditLegend_2->setObjectName(QString::fromUtf8("lineEditLegend_2"));

        gridLayout_3->addWidget(lineEditLegend_2, 0, 1, 1, 1);

        pushButtonLegend_2 = new QPushButton(wizardPageLegend);
        pushButtonLegend_2->setObjectName(QString::fromUtf8("pushButtonLegend_2"));

        gridLayout_3->addWidget(pushButtonLegend_2, 0, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textBrowserLegend = new QTextBrowser(wizardPageLegend);
        textBrowserLegend->setObjectName(QString::fromUtf8("textBrowserLegend"));

        horizontalLayout_2->addWidget(textBrowserLegend);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        checkBoxInterest = new QCheckBox(wizardPageLegend);
        checkBoxInterest->setObjectName(QString::fromUtf8("checkBoxInterest"));

        verticalLayout_6->addWidget(checkBoxInterest);

        checkBoxPop = new QCheckBox(wizardPageLegend);
        checkBoxPop->setObjectName(QString::fromUtf8("checkBoxPop"));

        verticalLayout_6->addWidget(checkBoxPop);

        checkBoxTer = new QCheckBox(wizardPageLegend);
        checkBoxTer->setObjectName(QString::fromUtf8("checkBoxTer"));

        verticalLayout_6->addWidget(checkBoxTer);

        checkBoxCoord = new QCheckBox(wizardPageLegend);
        checkBoxCoord->setObjectName(QString::fromUtf8("checkBoxCoord"));

        verticalLayout_6->addWidget(checkBoxCoord);

        checkBoxBorder = new QCheckBox(wizardPageLegend);
        checkBoxBorder->setObjectName(QString::fromUtf8("checkBoxBorder"));

        verticalLayout_6->addWidget(checkBoxBorder);


        horizontalLayout_2->addLayout(verticalLayout_6);


        gridLayout_3->addLayout(horizontalLayout_2, 1, 0, 1, 2);

        MapWizard->addPage(wizardPageLegend);
        wizardPageSummary = new QWizardPage();
        wizardPageSummary->setObjectName(QString::fromUtf8("wizardPageSummary"));
        verticalLayout_5 = new QVBoxLayout(wizardPageSummary);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_7 = new QLabel(wizardPageSummary);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setWordWrap(true);

        verticalLayout_5->addWidget(label_7);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelSumName = new QLabel(wizardPageSummary);
        labelSumName->setObjectName(QString::fromUtf8("labelSumName"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        labelSumName->setFont(font);

        formLayout->setWidget(0, QFormLayout::LabelRole, labelSumName);

        labelSumMName = new QLabel(wizardPageSummary);
        labelSumMName->setObjectName(QString::fromUtf8("labelSumMName"));

        formLayout->setWidget(0, QFormLayout::FieldRole, labelSumMName);

        labelSumTheme = new QLabel(wizardPageSummary);
        labelSumTheme->setObjectName(QString::fromUtf8("labelSumTheme"));
        labelSumTheme->setFont(font);

        formLayout->setWidget(1, QFormLayout::LabelRole, labelSumTheme);

        labelSumMTheme = new QLabel(wizardPageSummary);
        labelSumMTheme->setObjectName(QString::fromUtf8("labelSumMTheme"));

        formLayout->setWidget(1, QFormLayout::FieldRole, labelSumMTheme);

        labelSumMPreview = new QLabel(wizardPageSummary);
        labelSumMPreview->setObjectName(QString::fromUtf8("labelSumMPreview"));
        labelSumMPreview->setFont(font);

        formLayout->setWidget(2, QFormLayout::LabelRole, labelSumMPreview);

        labelThumbnail = new QLabel(wizardPageSummary);
        labelThumbnail->setObjectName(QString::fromUtf8("labelThumbnail"));
        labelThumbnail->setAlignment(Qt::AlignCenter);

        formLayout->setWidget(2, QFormLayout::FieldRole, labelThumbnail);


        verticalLayout_5->addLayout(formLayout);

        pushButtonPreviewMap = new QPushButton(wizardPageSummary);
        pushButtonPreviewMap->setObjectName(QString::fromUtf8("pushButtonPreviewMap"));

        verticalLayout_5->addWidget(pushButtonPreviewMap);

        MapWizard->addPage(wizardPageSummary);
#ifndef QT_NO_SHORTCUT
        labelSource->setBuddy(lineEditSource);
        labelMapTitle->setBuddy(lineEditTitle);
        labelMapDesc->setBuddy(textEditDesc);
#endif // QT_NO_SHORTCUT

        retranslateUi(MapWizard);

        QMetaObject::connectSlotsByName(MapWizard);
    } // setupUi

    void retranslateUi(QWizard *MapWizard)
    {
        MapWizard->setWindowTitle(QApplication::translate("MapWizard", "Marble Map Theme Creation Wizard", 0, QApplication::UnicodeUTF8));
        labelIntroInfo->setText(QApplication::translate("MapWizard", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Welcome to the Map Creation Wizard!</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">We will guide you through the process of creating a map theme for Marble. In the end you will have your own map that you can view in Marble.</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-l"
                        "eft:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Please choose the source that you want to use for the data of your map theme:</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        radioButtonWms->setToolTip(QApplication::translate("MapWizard", "Connects to a server on the Internet. The map data is queried via the WMS protocol.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        radioButtonWms->setText(QApplication::translate("MapWizard", "Web Map Service (WMS)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        radioButtonBitmap->setToolTip(QApplication::translate("MapWizard", "You need to provide a bitmap image of a world map that is stored on disk.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        radioButtonBitmap->setText(QApplication::translate("MapWizard", "A single image showing the whole world", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        radioButtonStaticUrl->setToolTip(QApplication::translate("MapWizard", "Connects to a server on the Internet. The map data is queried via a static URL.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        radioButtonStaticUrl->setText(QApplication::translate("MapWizard", "Online map providing indexed tiles (e.g. Open Street Map)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MapWizard", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">WMS Server</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Please choose a <a href=\"http://en.wikipedia.org/wiki/Web_Map_Service\"><span style=\" text-decoration: underline; color:#0057ae;\">WMS</span></a> server or enter a custom server URL.</p></body></html>", 0, QApplication::UnicodeUTF8));
        labelWmsServer->setText(QApplication::translate("MapWizard", "WMS Server:", 0, QApplication::UnicodeUTF8));
        comboBoxWmsServer->clear();
        comboBoxWmsServer->insertItems(0, QStringList()
         << QApplication::translate("MapWizard", "Custom", 0, QApplication::UnicodeUTF8)
        );
        labelWmsUrl->setText(QApplication::translate("MapWizard", "URL:", 0, QApplication::UnicodeUTF8));
        lineEditWmsUrl->setText(QApplication::translate("MapWizard", "http://", 0, QApplication::UnicodeUTF8));
        labelWmsMaps->setText(QApplication::translate("MapWizard", "Available Maps:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MapWizard", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">World Map Bitmap Image</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Please choose the location of a world map that is stored in a single image file (JPG, PNG, etc.). It needs to be provided in <a href=\"http://en.wikipedia.org/wiki/Equirectangular_projection\"><span sty"
                        "le=\" text-decoration: underline; color:#0057ae;\">Equirectangular projection</span></a>, so the shape should look like this (size ratio: 2:1):</p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/data/svg/worldmap.svg\" width=\"150\" /></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelSource->setText(QApplication::translate("MapWizard", "Source Image:", 0, QApplication::UnicodeUTF8));
        pushButtonSource->setText(QApplication::translate("MapWizard", "...", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MapWizard", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Bitstream Vera Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-weight:600;\">URL Scheme for Indexed Tiles</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif';\">Please enter the URL used for downloading individual tiles. Use the placeholders </span><span style=\" font-family:'Sans Serif'; font-style:italic;\">{x}</span><span style=\" font-family:'Sans Serif';\">, </span><span style=\" font-family:'Sans Serif'; font-style:italic;\">{y}</span><s"
                        "pan style=\" font-family:'Sans Serif';\">, and </span><span style=\" font-family:'Sans Serif'; font-style:italic;\">{zoomLevel}</span><span style=\" font-family:'Sans Serif';\"> to reference a tile from the index.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelStaticUrlServer->setText(QApplication::translate("MapWizard", "URL Scheme:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MapWizard", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Bitstream Vera Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-weight:600;\">General Information</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif';\">Please specify a title, a name and an icon for your new map. Add a description to let users know what your map is about. </span></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelMapTitle->setText(QApplication::translate("MapWizard", "Map Title:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEditTitle->setToolTip(QApplication::translate("MapWizard", "A short concise name for your map theme.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        labelMapTheme->setText(QApplication::translate("MapWizard", "Map Name:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lineEditTheme->setToolTip(QApplication::translate("MapWizard", "The lowercase map theme id.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        lineEditTheme->setWhatsThis(QApplication::translate("MapWizard", "The folder name of your new theme.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        labelMapDesc->setText(QApplication::translate("MapWizard", "Description:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        textEditDesc->setToolTip(QApplication::translate("MapWizard", "A short description of your map theme. Here you can provide information about the purpose, origin, copyright and license of the data used in your map theme.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        textEditDesc->setWhatsThis(QApplication::translate("MapWizard", "It will appear in a tool tip and may contain HTML formatting.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        labelMapIcon->setText(QApplication::translate("MapWizard", "Preview Image:", 0, QApplication::UnicodeUTF8));
        labelPreview->setText(QApplication::translate("MapWizard", "preview image", 0, QApplication::UnicodeUTF8));
        pushButtonPreview->setText(QApplication::translate("MapWizard", "Change...", 0, QApplication::UnicodeUTF8));
        labelLegend_2->setText(QApplication::translate("MapWizard", "Legend Image:", 0, QApplication::UnicodeUTF8));
        pushButtonLegend_2->setText(QApplication::translate("MapWizard", "...", 0, QApplication::UnicodeUTF8));
        checkBoxInterest->setText(QApplication::translate("MapWizard", "Places of Interest", 0, QApplication::UnicodeUTF8));
        checkBoxPop->setText(QApplication::translate("MapWizard", "Populated Places", 0, QApplication::UnicodeUTF8));
        checkBoxTer->setText(QApplication::translate("MapWizard", "Terrain", 0, QApplication::UnicodeUTF8));
        checkBoxCoord->setText(QApplication::translate("MapWizard", "Coordinate Grid", 0, QApplication::UnicodeUTF8));
        checkBoxBorder->setText(QApplication::translate("MapWizard", "Border Lines", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MapWizard", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Summary</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Congratulations! You have entered all the data that is necessary to create your map theme. Check the summary below carefully and press the &quot;Finish&quot; button to create the theme. </p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Please consider contributing the map theme back to the Marble commu"
                        "nity if the license and the terms of use allow for it.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        labelSumName->setText(QApplication::translate("MapWizard", "Map Name:", 0, QApplication::UnicodeUTF8));
        labelSumMName->setText(QApplication::translate("MapWizard", "Map name goes here.", 0, QApplication::UnicodeUTF8));
        labelSumTheme->setText(QApplication::translate("MapWizard", "Map Theme:", 0, QApplication::UnicodeUTF8));
        labelSumMTheme->setText(QApplication::translate("MapWizard", "Map theme goes here.", 0, QApplication::UnicodeUTF8));
        labelSumMPreview->setText(QApplication::translate("MapWizard", "Preview Image:", 0, QApplication::UnicodeUTF8));
        labelThumbnail->setText(QApplication::translate("MapWizard", "Thumbnail image goes here.", 0, QApplication::UnicodeUTF8));
        pushButtonPreviewMap->setText(QApplication::translate("MapWizard", "Preview Map", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapWizard: public Ui_MapWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPWIZARD_H
