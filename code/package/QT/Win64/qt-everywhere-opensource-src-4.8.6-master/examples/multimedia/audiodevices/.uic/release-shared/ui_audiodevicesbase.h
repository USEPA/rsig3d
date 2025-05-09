/********************************************************************************
** Form generated from reading UI file 'audiodevicesbase.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIODEVICESBASE_H
#define UI_AUDIODEVICESBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AudioDevicesBase
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_2;
    QLabel *modeLabel;
    QLabel *deviceLabel;
    QComboBox *modeBox;
    QComboBox *deviceBox;
    QTabWidget *tabWidget;
    QWidget *testFormatTab;
    QGridLayout *gridLayout;
    QLabel *actualLabel;
    QLabel *nearestLabel;
    QComboBox *frequencyBox;
    QLineEdit *nearestFreq;
    QComboBox *channelsBox;
    QLineEdit *nearestChannel;
    QComboBox *sampleSizesBox;
    QLineEdit *nearestSampleSize;
    QComboBox *endianBox;
    QLineEdit *nearestEndian;
    QPushButton *testButton;
    QLabel *testResult;
    QLabel *actualFreqLabel;
    QLabel *actualChannelLabel;
    QLabel *actualSampleSizeLabel;
    QLabel *actualEndianLabel;
    QLabel *label;
    QLabel *actualCodecLabel;
    QLineEdit *nearestCodec;
    QComboBox *codecsBox;
    QLabel *actualSampleTypeLabel;
    QComboBox *sampleTypesBox;
    QLineEdit *nearestSampleType;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QPushButton *populateTableButton;
    QTableWidget *allFormatsTable;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *AudioDevicesBase)
    {
        if (AudioDevicesBase->objectName().isEmpty())
            AudioDevicesBase->setObjectName(QString::fromUtf8("AudioDevicesBase"));
        AudioDevicesBase->resize(679, 598);
        centralwidget = new QWidget(AudioDevicesBase);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 659, 558));
        gridLayout_4 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        modeLabel = new QLabel(scrollAreaWidgetContents);
        modeLabel->setObjectName(QString::fromUtf8("modeLabel"));

        gridLayout_2->addWidget(modeLabel, 0, 0, 1, 1);

        deviceLabel = new QLabel(scrollAreaWidgetContents);
        deviceLabel->setObjectName(QString::fromUtf8("deviceLabel"));

        gridLayout_2->addWidget(deviceLabel, 0, 1, 1, 1);

        modeBox = new QComboBox(scrollAreaWidgetContents);
        modeBox->setObjectName(QString::fromUtf8("modeBox"));

        gridLayout_2->addWidget(modeBox, 1, 0, 1, 1);

        deviceBox = new QComboBox(scrollAreaWidgetContents);
        deviceBox->setObjectName(QString::fromUtf8("deviceBox"));

        gridLayout_2->addWidget(deviceBox, 1, 1, 1, 1);

        tabWidget = new QTabWidget(scrollAreaWidgetContents);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        testFormatTab = new QWidget();
        testFormatTab->setObjectName(QString::fromUtf8("testFormatTab"));
        gridLayout = new QGridLayout(testFormatTab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        actualLabel = new QLabel(testFormatTab);
        actualLabel->setObjectName(QString::fromUtf8("actualLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(actualLabel->sizePolicy().hasHeightForWidth());
        actualLabel->setSizePolicy(sizePolicy1);
        actualLabel->setFrameShape(QFrame::NoFrame);
        actualLabel->setFrameShadow(QFrame::Plain);
        actualLabel->setTextFormat(Qt::RichText);
        actualLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(actualLabel, 0, 1, 1, 1);

        nearestLabel = new QLabel(testFormatTab);
        nearestLabel->setObjectName(QString::fromUtf8("nearestLabel"));
        sizePolicy1.setHeightForWidth(nearestLabel->sizePolicy().hasHeightForWidth());
        nearestLabel->setSizePolicy(sizePolicy1);
        nearestLabel->setFrameShape(QFrame::NoFrame);
        nearestLabel->setFrameShadow(QFrame::Plain);
        nearestLabel->setTextFormat(Qt::RichText);
        nearestLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(nearestLabel, 0, 2, 1, 1);

        frequencyBox = new QComboBox(testFormatTab);
        frequencyBox->setObjectName(QString::fromUtf8("frequencyBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frequencyBox->sizePolicy().hasHeightForWidth());
        frequencyBox->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(frequencyBox, 3, 1, 1, 1);

        nearestFreq = new QLineEdit(testFormatTab);
        nearestFreq->setObjectName(QString::fromUtf8("nearestFreq"));
        nearestFreq->setEnabled(false);

        gridLayout->addWidget(nearestFreq, 3, 2, 1, 1);

        channelsBox = new QComboBox(testFormatTab);
        channelsBox->setObjectName(QString::fromUtf8("channelsBox"));

        gridLayout->addWidget(channelsBox, 5, 1, 1, 1);

        nearestChannel = new QLineEdit(testFormatTab);
        nearestChannel->setObjectName(QString::fromUtf8("nearestChannel"));
        nearestChannel->setEnabled(false);

        gridLayout->addWidget(nearestChannel, 5, 2, 1, 1);

        sampleSizesBox = new QComboBox(testFormatTab);
        sampleSizesBox->setObjectName(QString::fromUtf8("sampleSizesBox"));

        gridLayout->addWidget(sampleSizesBox, 9, 1, 1, 1);

        nearestSampleSize = new QLineEdit(testFormatTab);
        nearestSampleSize->setObjectName(QString::fromUtf8("nearestSampleSize"));
        nearestSampleSize->setEnabled(false);

        gridLayout->addWidget(nearestSampleSize, 9, 2, 1, 1);

        endianBox = new QComboBox(testFormatTab);
        endianBox->setObjectName(QString::fromUtf8("endianBox"));

        gridLayout->addWidget(endianBox, 14, 1, 1, 1);

        nearestEndian = new QLineEdit(testFormatTab);
        nearestEndian->setObjectName(QString::fromUtf8("nearestEndian"));
        nearestEndian->setEnabled(false);

        gridLayout->addWidget(nearestEndian, 14, 2, 1, 1);

        testButton = new QPushButton(testFormatTab);
        testButton->setObjectName(QString::fromUtf8("testButton"));

        gridLayout->addWidget(testButton, 15, 1, 1, 1);

        testResult = new QLabel(testFormatTab);
        testResult->setObjectName(QString::fromUtf8("testResult"));

        gridLayout->addWidget(testResult, 15, 2, 1, 1);

        actualFreqLabel = new QLabel(testFormatTab);
        actualFreqLabel->setObjectName(QString::fromUtf8("actualFreqLabel"));

        gridLayout->addWidget(actualFreqLabel, 3, 0, 1, 1);

        actualChannelLabel = new QLabel(testFormatTab);
        actualChannelLabel->setObjectName(QString::fromUtf8("actualChannelLabel"));

        gridLayout->addWidget(actualChannelLabel, 5, 0, 1, 1);

        actualSampleSizeLabel = new QLabel(testFormatTab);
        actualSampleSizeLabel->setObjectName(QString::fromUtf8("actualSampleSizeLabel"));

        gridLayout->addWidget(actualSampleSizeLabel, 9, 0, 1, 1);

        actualEndianLabel = new QLabel(testFormatTab);
        actualEndianLabel->setObjectName(QString::fromUtf8("actualEndianLabel"));

        gridLayout->addWidget(actualEndianLabel, 14, 0, 1, 1);

        label = new QLabel(testFormatTab);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setWordWrap(true);

        gridLayout->addWidget(label, 16, 0, 1, 3);

        actualCodecLabel = new QLabel(testFormatTab);
        actualCodecLabel->setObjectName(QString::fromUtf8("actualCodecLabel"));

        gridLayout->addWidget(actualCodecLabel, 2, 0, 1, 1);

        nearestCodec = new QLineEdit(testFormatTab);
        nearestCodec->setObjectName(QString::fromUtf8("nearestCodec"));
        nearestCodec->setEnabled(false);

        gridLayout->addWidget(nearestCodec, 2, 2, 1, 1);

        codecsBox = new QComboBox(testFormatTab);
        codecsBox->setObjectName(QString::fromUtf8("codecsBox"));

        gridLayout->addWidget(codecsBox, 2, 1, 1, 1);

        actualSampleTypeLabel = new QLabel(testFormatTab);
        actualSampleTypeLabel->setObjectName(QString::fromUtf8("actualSampleTypeLabel"));

        gridLayout->addWidget(actualSampleTypeLabel, 6, 0, 1, 1);

        sampleTypesBox = new QComboBox(testFormatTab);
        sampleTypesBox->setObjectName(QString::fromUtf8("sampleTypesBox"));

        gridLayout->addWidget(sampleTypesBox, 6, 1, 1, 1);

        nearestSampleType = new QLineEdit(testFormatTab);
        nearestSampleType->setObjectName(QString::fromUtf8("nearestSampleType"));
        nearestSampleType->setEnabled(false);

        gridLayout->addWidget(nearestSampleType, 6, 2, 1, 1);

        tabWidget->addTab(testFormatTab, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        populateTableButton = new QPushButton(tab);
        populateTableButton->setObjectName(QString::fromUtf8("populateTableButton"));

        verticalLayout_2->addWidget(populateTableButton);

        allFormatsTable = new QTableWidget(tab);
        if (allFormatsTable->columnCount() < 6)
            allFormatsTable->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        allFormatsTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        allFormatsTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        allFormatsTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        allFormatsTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        allFormatsTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        allFormatsTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        allFormatsTable->setObjectName(QString::fromUtf8("allFormatsTable"));
        allFormatsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        allFormatsTable->setDragDropOverwriteMode(false);
        allFormatsTable->setSelectionMode(QAbstractItemView::NoSelection);
        allFormatsTable->setSelectionBehavior(QAbstractItemView::SelectItems);
        allFormatsTable->setTextElideMode(Qt::ElideNone);
        allFormatsTable->setSortingEnabled(false);
        allFormatsTable->setWordWrap(false);
        allFormatsTable->setCornerButtonEnabled(false);
        allFormatsTable->horizontalHeader()->setHighlightSections(false);
        allFormatsTable->verticalHeader()->setVisible(false);
        allFormatsTable->verticalHeader()->setHighlightSections(false);

        verticalLayout_2->addWidget(allFormatsTable);

        tabWidget->addTab(tab, QString());

        gridLayout_2->addWidget(tabWidget, 2, 0, 1, 2);


        gridLayout_4->addLayout(gridLayout_2, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        AudioDevicesBase->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(AudioDevicesBase);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        AudioDevicesBase->setStatusBar(statusbar);

        retranslateUi(AudioDevicesBase);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AudioDevicesBase);
    } // setupUi

    void retranslateUi(QMainWindow *AudioDevicesBase)
    {
        AudioDevicesBase->setWindowTitle(QApplication::translate("AudioDevicesBase", "Audio Devices", 0, QApplication::UnicodeUTF8));
        modeLabel->setText(QApplication::translate("AudioDevicesBase", "Mode", 0, QApplication::UnicodeUTF8));
        deviceLabel->setText(QApplication::translate("AudioDevicesBase", "Device", 0, QApplication::UnicodeUTF8));
        modeBox->clear();
        modeBox->insertItems(0, QStringList()
         << QApplication::translate("AudioDevicesBase", "Input", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("AudioDevicesBase", "Output", 0, QApplication::UnicodeUTF8)
        );
        actualLabel->setText(QApplication::translate("AudioDevicesBase", "<i>Actual Settings</i>", 0, QApplication::UnicodeUTF8));
        nearestLabel->setText(QApplication::translate("AudioDevicesBase", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-style:italic;\">Nearest Settings</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        testButton->setText(QApplication::translate("AudioDevicesBase", "Test", 0, QApplication::UnicodeUTF8));
        testResult->setText(QString());
        actualFreqLabel->setText(QApplication::translate("AudioDevicesBase", "Frequency (Hz)", 0, QApplication::UnicodeUTF8));
        actualChannelLabel->setText(QApplication::translate("AudioDevicesBase", "Channels", 0, QApplication::UnicodeUTF8));
        actualSampleSizeLabel->setText(QApplication::translate("AudioDevicesBase", "Sample size (bits)", 0, QApplication::UnicodeUTF8));
        actualEndianLabel->setText(QApplication::translate("AudioDevicesBase", "Endianess", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AudioDevicesBase", "Note: an invalid codec 'audio/test' exists in order to allow an invalid format to be constructed, and therefore to trigger a 'nearest format' calculation.", 0, QApplication::UnicodeUTF8));
        actualCodecLabel->setText(QApplication::translate("AudioDevicesBase", "Codec", 0, QApplication::UnicodeUTF8));
        actualSampleTypeLabel->setText(QApplication::translate("AudioDevicesBase", "SampleType", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(testFormatTab), QApplication::translate("AudioDevicesBase", "Test format", 0, QApplication::UnicodeUTF8));
        populateTableButton->setText(QApplication::translate("AudioDevicesBase", "Populate table", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = allFormatsTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AudioDevicesBase", "Codec", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = allFormatsTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AudioDevicesBase", "Frequency (Hz)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = allFormatsTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("AudioDevicesBase", "Channels", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = allFormatsTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("AudioDevicesBase", "Sample type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = allFormatsTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("AudioDevicesBase", "Sample size (bits)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = allFormatsTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("AudioDevicesBase", "Endianness", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("AudioDevicesBase", "All formats", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AudioDevicesBase: public Ui_AudioDevicesBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIODEVICESBASE_H
