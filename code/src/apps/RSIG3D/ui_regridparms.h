/********************************************************************************
** Form generated from reading UI file 'regridparms.ui'
**
** Created: Thu Jul 23 13:53:07 2026
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGRIDPARMS_H
#define UI_REGRIDPARMS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_RegridParmsDialog
{
public:
    QGroupBox *groupBox_regridSelections;
    QGridLayout *gridLayout;
    QLabel *label_44;
    QComboBox *regridAggregationComboBox;
    QComboBox *cmaqgridComboBox;
    QCheckBox *chk_userOverride;
    QComboBox *regridmethodComboBox;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_regridParams;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *label_27;
    QLabel *label_4;
    QLabel *label_7;
    QDoubleSpinBox *xorigDSpinBox;
    QDoubleSpinBox *t0DSpinBox;
    QLabel *label_8;
    QSpinBox *vgtypSpinBox;
    QDoubleSpinBox *gDSpinBox;
    QSpinBox *nrowsSpinBox;
    QLabel *label_35;
    QLabel *label_16;
    QDoubleSpinBox *palpDSpinBox;
    QLabel *label_32;
    QLabel *label_39;
    QLabel *label_22;
    QLabel *label_28;
    QSpinBox *nlaysSpinBox;
    QLabel *label_21;
    QLabel *label_38;
    QDoubleSpinBox *pgamDSpinBox;
    QSpinBox *gdtypSpinBox;
    QLabel *label_43;
    QLabel *label_31;
    QDoubleSpinBox *rDSpinBox;
    QLabel *label_xOrig;
    QLabel *label_36;
    QLabel *label_15;
    QLabel *label_24;
    QDoubleSpinBox *pbetDSpinBox;
    QLabel *label_23;
    QLabel *label_29;
    QSpinBox *ncolsSpinBox;
    QLabel *label_34;
    QLabel *label_5;
    QLabel *label_33;
    QDoubleSpinBox *aDSpinBox;
    QDoubleSpinBox *yorigDSpinBox;
    QLabel *label_6;
    QLabel *label_yOrig;
    QLineEdit *vglvlsLineEdit;
    QDoubleSpinBox *radiusDSpinBox;
    QLabel *label_25;
    QDoubleSpinBox *vgtopDSpinBox;
    QLabel *label_11;
    QLabel *label_xCell;
    QDoubleSpinBox *p0DSpinBox;
    QLabel *label_yCell;
    QLabel *label_30;
    QLabel *label_40;
    QDoubleSpinBox *ycellDSpinBox;
    QPushButton *acceptPushButton;
    QLabel *label_13;
    QLabel *label_37;
    QDoubleSpinBox *xcentDSpinBox;
    QLabel *label_14;
    QLabel *label_12;
    QPushButton *resetPushButton;
    QLabel *label_26;
    QDoubleSpinBox *ycentDSpinBox;
    QLabel *label_20;
    QLabel *label_10;
    QPushButton *cancelPushButton;
    QDoubleSpinBox *xcellDSpinBox;
    QLabel *label_41;
    QLabel *label_42;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *RegridParmsDialog)
    {
        if (RegridParmsDialog->objectName().isEmpty())
            RegridParmsDialog->setObjectName(QString::fromUtf8("RegridParmsDialog"));
        RegridParmsDialog->resize(1400, 800);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RegridParmsDialog->sizePolicy().hasHeightForWidth());
        RegridParmsDialog->setSizePolicy(sizePolicy);
        RegridParmsDialog->setMinimumSize(QSize(1400, 800));
        RegridParmsDialog->setMaximumSize(QSize(10000, 10000));
        groupBox_regridSelections = new QGroupBox(RegridParmsDialog);
        groupBox_regridSelections->setObjectName(QString::fromUtf8("groupBox_regridSelections"));
        groupBox_regridSelections->setGeometry(QRect(8, 0, 1350, 80));
        sizePolicy.setHeightForWidth(groupBox_regridSelections->sizePolicy().hasHeightForWidth());
        groupBox_regridSelections->setSizePolicy(sizePolicy);
        groupBox_regridSelections->setMinimumSize(QSize(1350, 0));
        gridLayout = new QGridLayout(groupBox_regridSelections);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_44 = new QLabel(groupBox_regridSelections);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setMinimumSize(QSize(70, 35));

        gridLayout->addWidget(label_44, 0, 4, 1, 1);

        regridAggregationComboBox = new QComboBox(groupBox_regridSelections);
        regridAggregationComboBox->setObjectName(QString::fromUtf8("regridAggregationComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(regridAggregationComboBox->sizePolicy().hasHeightForWidth());
        regridAggregationComboBox->setSizePolicy(sizePolicy1);
        regridAggregationComboBox->setMinimumSize(QSize(160, 24));
        regridAggregationComboBox->setMaximumSize(QSize(300, 24));

        gridLayout->addWidget(regridAggregationComboBox, 0, 5, 1, 1);

        cmaqgridComboBox = new QComboBox(groupBox_regridSelections);
        cmaqgridComboBox->setObjectName(QString::fromUtf8("cmaqgridComboBox"));
        sizePolicy1.setHeightForWidth(cmaqgridComboBox->sizePolicy().hasHeightForWidth());
        cmaqgridComboBox->setSizePolicy(sizePolicy1);
        cmaqgridComboBox->setMinimumSize(QSize(160, 24));
        cmaqgridComboBox->setMaximumSize(QSize(300, 24));

        gridLayout->addWidget(cmaqgridComboBox, 0, 1, 1, 1);

        chk_userOverride = new QCheckBox(groupBox_regridSelections);
        chk_userOverride->setObjectName(QString::fromUtf8("chk_userOverride"));
        sizePolicy1.setHeightForWidth(chk_userOverride->sizePolicy().hasHeightForWidth());
        chk_userOverride->setSizePolicy(sizePolicy1);
        chk_userOverride->setMinimumSize(QSize(0, 35));

        gridLayout->addWidget(chk_userOverride, 1, 1, 1, 3);

        regridmethodComboBox = new QComboBox(groupBox_regridSelections);
        regridmethodComboBox->setObjectName(QString::fromUtf8("regridmethodComboBox"));
        sizePolicy1.setHeightForWidth(regridmethodComboBox->sizePolicy().hasHeightForWidth());
        regridmethodComboBox->setSizePolicy(sizePolicy1);
        regridmethodComboBox->setMinimumSize(QSize(160, 24));
        regridmethodComboBox->setMaximumSize(QSize(300, 24));

        gridLayout->addWidget(regridmethodComboBox, 0, 3, 1, 1);

        label = new QLabel(groupBox_regridSelections);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 35));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_regridSelections);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(70, 35));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 6, 1, 1);

        groupBox_regridParams = new QGroupBox(RegridParmsDialog);
        groupBox_regridParams->setObjectName(QString::fromUtf8("groupBox_regridParams"));
        groupBox_regridParams->setGeometry(QRect(8, 84, 1350, 600));
        sizePolicy.setHeightForWidth(groupBox_regridParams->sizePolicy().hasHeightForWidth());
        groupBox_regridParams->setSizePolicy(sizePolicy);
        groupBox_regridParams->setMinimumSize(QSize(1350, 600));
        gridLayout_2 = new QGridLayout(groupBox_regridParams);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(groupBox_regridParams);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 2);

        label_27 = new QLabel(groupBox_regridParams);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_2->addWidget(label_27, 0, 9, 1, 3);

        label_4 = new QLabel(groupBox_regridParams);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        label_7 = new QLabel(groupBox_regridParams);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 5, 1, 2);

        xorigDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        xorigDSpinBox->setObjectName(QString::fromUtf8("xorigDSpinBox"));
        xorigDSpinBox->setMinimumSize(QSize(150, 24));
        xorigDSpinBox->setMaximumSize(QSize(120, 24));
        xorigDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xorigDSpinBox->setDecimals(1);
        xorigDSpinBox->setMinimum(-1e+08);
        xorigDSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(xorigDSpinBox, 5, 1, 1, 1);

        t0DSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        t0DSpinBox->setObjectName(QString::fromUtf8("t0DSpinBox"));
        t0DSpinBox->setMinimumSize(QSize(150, 24));
        t0DSpinBox->setMaximumSize(QSize(120, 24));
        t0DSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        t0DSpinBox->setMinimum(0);
        t0DSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(t0DSpinBox, 4, 10, 1, 1);

        label_8 = new QLabel(groupBox_regridParams);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 1, 4, 1, 2);

        vgtypSpinBox = new QSpinBox(groupBox_regridParams);
        vgtypSpinBox->setObjectName(QString::fromUtf8("vgtypSpinBox"));
        vgtypSpinBox->setMinimumSize(QSize(150, 24));
        vgtypSpinBox->setMaximumSize(QSize(120, 24));
        vgtypSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        vgtypSpinBox->setMinimum(1);
        vgtypSpinBox->setMaximum(7);

        gridLayout_2->addWidget(vgtypSpinBox, 1, 1, 1, 1);

        gDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        gDSpinBox->setObjectName(QString::fromUtf8("gDSpinBox"));
        gDSpinBox->setMinimumSize(QSize(150, 24));
        gDSpinBox->setMaximumSize(QSize(120, 24));
        gDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gDSpinBox->setDecimals(2);
        gDSpinBox->setMaximum(9999);
        gDSpinBox->setValue(100);

        gridLayout_2->addWidget(gDSpinBox, 1, 10, 1, 1);

        nrowsSpinBox = new QSpinBox(groupBox_regridParams);
        nrowsSpinBox->setObjectName(QString::fromUtf8("nrowsSpinBox"));
        nrowsSpinBox->setMinimumSize(QSize(150, 24));
        nrowsSpinBox->setMaximumSize(QSize(120, 24));
        nrowsSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        nrowsSpinBox->setMinimum(1);
        nrowsSpinBox->setMaximum(10000);

        gridLayout_2->addWidget(nrowsSpinBox, 3, 1, 1, 1);

        label_35 = new QLabel(groupBox_regridParams);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_2->addWidget(label_35, 3, 11, 1, 1);

        label_16 = new QLabel(groupBox_regridParams);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 4, 0, 1, 1);

        palpDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        palpDSpinBox->setObjectName(QString::fromUtf8("palpDSpinBox"));
        palpDSpinBox->setMinimumSize(QSize(150, 24));
        palpDSpinBox->setMaximumSize(QSize(120, 24));
        palpDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        palpDSpinBox->setMinimum(-180);
        palpDSpinBox->setMaximum(180);

        gridLayout_2->addWidget(palpDSpinBox, 2, 6, 1, 1);

        label_32 = new QLabel(groupBox_regridParams);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_2->addWidget(label_32, 3, 8, 1, 2);

        label_39 = new QLabel(groupBox_regridParams);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout_2->addWidget(label_39, 3, 7, 1, 1);

        label_22 = new QLabel(groupBox_regridParams);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_2->addWidget(label_22, 3, 4, 1, 2);

        label_28 = new QLabel(groupBox_regridParams);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_2->addWidget(label_28, 1, 8, 1, 2);

        nlaysSpinBox = new QSpinBox(groupBox_regridParams);
        nlaysSpinBox->setObjectName(QString::fromUtf8("nlaysSpinBox"));
        nlaysSpinBox->setMinimumSize(QSize(150, 24));
        nlaysSpinBox->setMaximumSize(QSize(120, 24));
        nlaysSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        nlaysSpinBox->setMinimum(1);
        nlaysSpinBox->setMaximum(100);

        gridLayout_2->addWidget(nlaysSpinBox, 4, 1, 1, 1);

        label_21 = new QLabel(groupBox_regridParams);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_2->addWidget(label_21, 2, 4, 1, 2);

        label_38 = new QLabel(groupBox_regridParams);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_2->addWidget(label_38, 2, 7, 1, 1);

        pgamDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        pgamDSpinBox->setObjectName(QString::fromUtf8("pgamDSpinBox"));
        pgamDSpinBox->setMinimumSize(QSize(150, 24));
        pgamDSpinBox->setMaximumSize(QSize(120, 24));
        pgamDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pgamDSpinBox->setMinimum(-180);
        pgamDSpinBox->setMaximum(180);

        gridLayout_2->addWidget(pgamDSpinBox, 4, 6, 1, 1);

        gdtypSpinBox = new QSpinBox(groupBox_regridParams);
        gdtypSpinBox->setObjectName(QString::fromUtf8("gdtypSpinBox"));
        gdtypSpinBox->setMinimumSize(QSize(150, 24));
        gdtypSpinBox->setMaximumSize(QSize(120, 24));
        gdtypSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        gdtypSpinBox->setMinimum(1);
        gdtypSpinBox->setMaximum(9);

        gridLayout_2->addWidget(gdtypSpinBox, 1, 6, 1, 1);

        label_43 = new QLabel(groupBox_regridParams);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        gridLayout_2->addWidget(label_43, 4, 7, 1, 1);

        label_31 = new QLabel(groupBox_regridParams);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_2->addWidget(label_31, 4, 8, 1, 2);

        rDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        rDSpinBox->setObjectName(QString::fromUtf8("rDSpinBox"));
        rDSpinBox->setMinimumSize(QSize(150, 24));
        rDSpinBox->setMaximumSize(QSize(120, 24));
        rDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        rDSpinBox->setMinimum(0);
        rDSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(rDSpinBox, 2, 10, 1, 1);

        label_xOrig = new QLabel(groupBox_regridParams);
        label_xOrig->setObjectName(QString::fromUtf8("label_xOrig"));

        gridLayout_2->addWidget(label_xOrig, 5, 2, 1, 2);

        label_36 = new QLabel(groupBox_regridParams);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_2->addWidget(label_36, 4, 11, 1, 1);

        label_15 = new QLabel(groupBox_regridParams);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_2->addWidget(label_15, 5, 0, 1, 1);

        label_24 = new QLabel(groupBox_regridParams);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_2->addWidget(label_24, 5, 4, 1, 2);

        pbetDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        pbetDSpinBox->setObjectName(QString::fromUtf8("pbetDSpinBox"));
        pbetDSpinBox->setMinimumSize(QSize(150, 24));
        pbetDSpinBox->setMaximumSize(QSize(120, 24));
        pbetDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pbetDSpinBox->setMinimum(-180);
        pbetDSpinBox->setMaximum(180);

        gridLayout_2->addWidget(pbetDSpinBox, 3, 6, 1, 1);

        label_23 = new QLabel(groupBox_regridParams);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_2->addWidget(label_23, 4, 4, 1, 2);

        label_29 = new QLabel(groupBox_regridParams);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_2->addWidget(label_29, 2, 8, 1, 2);

        ncolsSpinBox = new QSpinBox(groupBox_regridParams);
        ncolsSpinBox->setObjectName(QString::fromUtf8("ncolsSpinBox"));
        ncolsSpinBox->setMinimumSize(QSize(150, 24));
        ncolsSpinBox->setMaximumSize(QSize(120, 24));
        ncolsSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ncolsSpinBox->setMinimum(1);
        ncolsSpinBox->setMaximum(10000);

        gridLayout_2->addWidget(ncolsSpinBox, 2, 1, 1, 1);

        label_34 = new QLabel(groupBox_regridParams);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_2->addWidget(label_34, 2, 11, 1, 1);

        label_5 = new QLabel(groupBox_regridParams);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 2, 0, 1, 1);

        label_33 = new QLabel(groupBox_regridParams);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_2->addWidget(label_33, 1, 11, 1, 1);

        aDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        aDSpinBox->setObjectName(QString::fromUtf8("aDSpinBox"));
        aDSpinBox->setMinimumSize(QSize(150, 24));
        aDSpinBox->setMaximumSize(QSize(120, 24));
        aDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        aDSpinBox->setMinimum(0);
        aDSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(aDSpinBox, 3, 10, 1, 1);

        yorigDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        yorigDSpinBox->setObjectName(QString::fromUtf8("yorigDSpinBox"));
        yorigDSpinBox->setMinimumSize(QSize(150, 24));
        yorigDSpinBox->setMaximumSize(QSize(120, 24));
        yorigDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        yorigDSpinBox->setDecimals(1);
        yorigDSpinBox->setMinimum(-1e+08);
        yorigDSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(yorigDSpinBox, 6, 1, 1, 1);

        label_6 = new QLabel(groupBox_regridParams);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 3, 0, 1, 1);

        label_yOrig = new QLabel(groupBox_regridParams);
        label_yOrig->setObjectName(QString::fromUtf8("label_yOrig"));

        gridLayout_2->addWidget(label_yOrig, 6, 2, 1, 2);

        vglvlsLineEdit = new QLineEdit(groupBox_regridParams);
        vglvlsLineEdit->setObjectName(QString::fromUtf8("vglvlsLineEdit"));
        vglvlsLineEdit->setMinimumSize(QSize(800, 24));
        vglvlsLineEdit->setMaximumSize(QSize(800, 24));

        gridLayout_2->addWidget(vglvlsLineEdit, 10, 1, 1, 11);

        radiusDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        radiusDSpinBox->setObjectName(QString::fromUtf8("radiusDSpinBox"));
        radiusDSpinBox->setMinimumSize(QSize(150, 24));
        radiusDSpinBox->setMaximumSize(QSize(120, 24));
        radiusDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        radiusDSpinBox->setDecimals(0);
        radiusDSpinBox->setMinimum(1);
        radiusDSpinBox->setMaximum(1e+08);
        radiusDSpinBox->setValue(6.37e+06);

        gridLayout_2->addWidget(radiusDSpinBox, 7, 6, 1, 1);

        label_25 = new QLabel(groupBox_regridParams);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_2->addWidget(label_25, 6, 4, 1, 2);

        vgtopDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        vgtopDSpinBox->setObjectName(QString::fromUtf8("vgtopDSpinBox"));
        vgtopDSpinBox->setMinimumSize(QSize(150, 24));
        vgtopDSpinBox->setMaximumSize(QSize(120, 24));
        vgtopDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        vgtopDSpinBox->setMinimum(0);
        vgtopDSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(vgtopDSpinBox, 9, 1, 1, 1);

        label_11 = new QLabel(groupBox_regridParams);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 9, 0, 1, 1);

        label_xCell = new QLabel(groupBox_regridParams);
        label_xCell->setObjectName(QString::fromUtf8("label_xCell"));

        gridLayout_2->addWidget(label_xCell, 7, 2, 1, 1);

        p0DSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        p0DSpinBox->setObjectName(QString::fromUtf8("p0DSpinBox"));
        p0DSpinBox->setMinimumSize(QSize(150, 24));
        p0DSpinBox->setMaximumSize(QSize(120, 24));
        p0DSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        p0DSpinBox->setMinimum(-1e+08);
        p0DSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(p0DSpinBox, 5, 10, 1, 1);

        label_yCell = new QLabel(groupBox_regridParams);
        label_yCell->setObjectName(QString::fromUtf8("label_yCell"));

        gridLayout_2->addWidget(label_yCell, 8, 2, 1, 3);

        label_30 = new QLabel(groupBox_regridParams);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_2->addWidget(label_30, 5, 8, 1, 2);

        label_40 = new QLabel(groupBox_regridParams);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        gridLayout_2->addWidget(label_40, 7, 7, 1, 2);

        ycellDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        ycellDSpinBox->setObjectName(QString::fromUtf8("ycellDSpinBox"));
        ycellDSpinBox->setMinimumSize(QSize(150, 24));
        ycellDSpinBox->setMaximumSize(QSize(120, 24));
        ycellDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ycellDSpinBox->setDecimals(4);
        ycellDSpinBox->setMinimum(0.0001);
        ycellDSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(ycellDSpinBox, 8, 1, 1, 1);

        acceptPushButton = new QPushButton(groupBox_regridParams);
        acceptPushButton->setObjectName(QString::fromUtf8("acceptPushButton"));
        acceptPushButton->setMinimumSize(QSize(92, 24));
        acceptPushButton->setMaximumSize(QSize(92, 24));
        acceptPushButton->setAutoDefault(false);
        acceptPushButton->setDefault(false);

        gridLayout_2->addWidget(acceptPushButton, 11, 10, 1, 1);

        label_13 = new QLabel(groupBox_regridParams);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_2->addWidget(label_13, 7, 0, 1, 1);

        label_37 = new QLabel(groupBox_regridParams);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_2->addWidget(label_37, 5, 11, 1, 1);

        xcentDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        xcentDSpinBox->setObjectName(QString::fromUtf8("xcentDSpinBox"));
        xcentDSpinBox->setMinimumSize(QSize(150, 24));
        xcentDSpinBox->setMaximumSize(QSize(120, 24));
        xcentDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xcentDSpinBox->setMinimum(-180);
        xcentDSpinBox->setMaximum(180);

        gridLayout_2->addWidget(xcentDSpinBox, 5, 6, 1, 1);

        label_14 = new QLabel(groupBox_regridParams);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 6, 0, 1, 1);

        label_12 = new QLabel(groupBox_regridParams);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_2->addWidget(label_12, 8, 0, 1, 1);

        resetPushButton = new QPushButton(groupBox_regridParams);
        resetPushButton->setObjectName(QString::fromUtf8("resetPushButton"));
        resetPushButton->setMinimumSize(QSize(92, 24));
        resetPushButton->setMaximumSize(QSize(92, 24));
        resetPushButton->setAutoDefault(false);

        gridLayout_2->addWidget(resetPushButton, 11, 2, 1, 4);

        label_26 = new QLabel(groupBox_regridParams);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_2->addWidget(label_26, 7, 3, 1, 3);

        ycentDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        ycentDSpinBox->setObjectName(QString::fromUtf8("ycentDSpinBox"));
        ycentDSpinBox->setMinimumSize(QSize(150, 24));
        ycentDSpinBox->setMaximumSize(QSize(120, 24));
        ycentDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ycentDSpinBox->setMinimum(-90);
        ycentDSpinBox->setMaximum(90);

        gridLayout_2->addWidget(ycentDSpinBox, 6, 6, 1, 1);

        label_20 = new QLabel(groupBox_regridParams);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_2->addWidget(label_20, 9, 2, 1, 3);

        label_10 = new QLabel(groupBox_regridParams);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_2->addWidget(label_10, 10, 0, 1, 1);

        cancelPushButton = new QPushButton(groupBox_regridParams);
        cancelPushButton->setObjectName(QString::fromUtf8("cancelPushButton"));
        cancelPushButton->setMinimumSize(QSize(92, 24));
        cancelPushButton->setMaximumSize(QSize(92, 24));
        cancelPushButton->setAutoDefault(false);

        gridLayout_2->addWidget(cancelPushButton, 11, 6, 1, 1);

        xcellDSpinBox = new QDoubleSpinBox(groupBox_regridParams);
        xcellDSpinBox->setObjectName(QString::fromUtf8("xcellDSpinBox"));
        xcellDSpinBox->setMinimumSize(QSize(150, 24));
        xcellDSpinBox->setMaximumSize(QSize(120, 24));
        xcellDSpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        xcellDSpinBox->setDecimals(4);
        xcellDSpinBox->setMinimum(0.0001);
        xcellDSpinBox->setMaximum(1e+08);

        gridLayout_2->addWidget(xcellDSpinBox, 7, 1, 1, 1);

        label_41 = new QLabel(groupBox_regridParams);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_2->addWidget(label_41, 6, 7, 1, 2);

        label_42 = new QLabel(groupBox_regridParams);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        gridLayout_2->addWidget(label_42, 5, 7, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 3, 12, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 12, 2, 1, 1);


        retranslateUi(RegridParmsDialog);

        QMetaObject::connectSlotsByName(RegridParmsDialog);
    } // setupUi

    void retranslateUi(QDialog *RegridParmsDialog)
    {
        RegridParmsDialog->setWindowTitle(QApplication::translate("RegridParmsDialog", "Regrid Parameters", 0, QApplication::UnicodeUTF8));
        groupBox_regridSelections->setTitle(QString());
        label_44->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p><span style=\" font-weight:600;\">Aggregation:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        chk_userOverride->setText(QApplication::translate("RegridParmsDialog", "Allow user override", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RegridParmsDialog", "<b>CMAQ grid:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p><span style=\" font-weight:600;\">Regrid method:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox_regridParams->setTitle(QString());
        label_3->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p><span style=\" font-weight:600;\">CMAQ grid parameters</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p><span style=\" font-weight:600;\">Constants</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">VGTYP:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p><span style=\" font-weight:600;\">CMAQ grid parameters</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">GDTYP:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("RegridParmsDialog", "K", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">NLAYS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">A:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("RegridParmsDialog", "deg", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">P_BET:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">g:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">P_ALP:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("RegridParmsDialog", "deg", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("RegridParmsDialog", "deg", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">T0:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_xOrig->setText(QApplication::translate("RegridParmsDialog", "m", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("RegridParmsDialog", "K", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">XORIG:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">XCENT:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">P_GAM:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">R:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("RegridParmsDialog", "J/kg/K", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">NCOLS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("RegridParmsDialog", "m/s2", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">NROWS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_yOrig->setText(QApplication::translate("RegridParmsDialog", "m", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">YCENT:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">VGTOP</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_xCell->setText(QApplication::translate("RegridParmsDialog", "m", 0, QApplication::UnicodeUTF8));
        label_yCell->setText(QApplication::translate("RegridParmsDialog", "m", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">P0:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("RegridParmsDialog", "m", 0, QApplication::UnicodeUTF8));
        acceptPushButton->setText(QApplication::translate("RegridParmsDialog", "Accept", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">XCELL:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("RegridParmsDialog", "Pa", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">YORIG:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">YCELL:</p></body></html>", 0, QApplication::UnicodeUTF8));
        resetPushButton->setText(QApplication::translate("RegridParmsDialog", "Reset", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">Earth Radius:</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("RegridParmsDialog", "Pa", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("RegridParmsDialog", "<html><head/><body><p align=\"right\">VGLVLS:</p></body></html>", 0, QApplication::UnicodeUTF8));
        cancelPushButton->setText(QApplication::translate("RegridParmsDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("RegridParmsDialog", "deg", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("RegridParmsDialog", "deg", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RegridParmsDialog: public Ui_RegridParmsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGRIDPARMS_H
