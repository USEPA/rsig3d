/******************************************************************************
PURPOSE: regrid.C - Implements RegridParmsData class.

NOTES: a dialog class.

HISTORY: 2012/11/28, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <iostream>
#include <QDebug>
#include "RegridParmsData.h"
#include "RegridParmsDialog.h"
#include "ui_regridparms.h"
#include "MainWin.h"
#include "ControlDockWidget.h"

static const QString cmaqGridList[] = {
  "CONUS (EQUATES) 12km",
  "CONUS (EQUATES)  1km",
  "CONUS 36km",
  "CONUS (CDC)",
  "East (CDC)",
  "Hemisphere",
  "AQMEII",
  "AQMEII_NA",
  "AQMEII_EU",
  "DISCOVER-CONUS",
  "DISCOVER-EAST",
  "DISCOVER-MD",
  "HRRR CONUS 3km",
  "4LISTOS1",
  "4LMOS1",
  "BBOX",
  "LONLAT",
  "From File"
};

static RegridParmsData staticData(0);



static void setUiValues(Ui_RegridParmsDialog* ui, RegridParmsData& data) {

  // populate the ui
  ui->cmaqgridComboBox->setCurrentIndex(data.cmaqGrid);
  ui->regridmethodComboBox->setCurrentIndex(data.regridMethod);
  ui->regridAggregationComboBox->setCurrentIndex(data.regridAggregation);
  
  // get latest bbox parameters if needed

  if (cmaqGridList[data.cmaqGrid].startsWith("BBOX")) {
    data.init_bbox_parameters();
  }
  
  ui->vgtypSpinBox->setValue(data.VGTYP);
  ui->gdtypSpinBox->setValue(data.GDTYP);
  ui->ncolsSpinBox->setValue(data.NCOLS);
  ui->nrowsSpinBox->setValue(data.NROWS);
  ui->nlaysSpinBox->setValue(data.NLAYS);
  
  ui->xorigDSpinBox->setValue(data.XORIG);
  ui->yorigDSpinBox->setValue(data.YORIG);
  ui->xcellDSpinBox->setValue(data.XCELL);
  ui->ycellDSpinBox->setValue(data.YCELL);
  ui->vgtopDSpinBox->setValue(data.VGTOP);
  
  ui->palpDSpinBox->setValue(data.PALP);
  ui->pbetDSpinBox->setValue(data.PBET);
  ui->pgamDSpinBox->setValue(data.PGAM);
  ui->xcentDSpinBox->setValue(data.XCENT);
  ui->ycentDSpinBox->setValue(data.YCENT);
  ui->radiusDSpinBox->setValue(data.RADIUS);
  
  ui->gDSpinBox->setValue(data.G);
  ui->rDSpinBox->setValue(data.R);
  ui->aDSpinBox->setValue(data.A);
  ui->t0DSpinBox->setValue(data.T0);
  ui->p0DSpinBox->setValue(data.P0);
  
  ui->vglvlsLineEdit->setText(data.VGLVLS.c_str());

  ui->label_xOrig->setText("m");
  ui->label_yOrig->setText("m");
  ui->label_xCell->setText("m");
  ui->label_xCell->setText("m");
}



static void getUiValues(const Ui_RegridParmsDialog* ui, RegridParmsData& data) {
  data.cmaqGrid = ui->cmaqgridComboBox->currentIndex();
  data.regridMethod = ui->regridmethodComboBox->currentIndex();
  data.regridAggregation = ui->regridAggregationComboBox->currentIndex();

  data.VGTYP = ui->vgtypSpinBox->value();
  data.GDTYP = ui->gdtypSpinBox->value();
  data.NCOLS = ui->ncolsSpinBox->value();
  data.NROWS = ui->nrowsSpinBox->value();
  data.NLAYS = ui->nlaysSpinBox->value();

  data.XORIG = ui->xorigDSpinBox->value();
  data.YORIG = ui->yorigDSpinBox->value();
  data.XCELL = ui->xcellDSpinBox->value();
  data.YCELL = ui->ycellDSpinBox->value();
  data.VGTOP = ui->vgtopDSpinBox->value();

  data.PALP = ui->palpDSpinBox->value();
  data.PBET = ui->pbetDSpinBox->value();
  data.PGAM = ui->pgamDSpinBox->value();
  data.XCENT = ui->xcentDSpinBox->value();
  data.YCENT = ui->ycentDSpinBox->value();
  data.RADIUS = ui->radiusDSpinBox->value();

  data.G = ui->gDSpinBox->value();
  data.R = ui->rDSpinBox->value();
  data.A = ui->aDSpinBox->value();
  data.T0 = ui->t0DSpinBox->value();
  data.P0 = ui->p0DSpinBox->value();

  const QString text( ui->vglvlsLineEdit->text() );
  data.VGLVLS = text.toStdString();
}



static void disableUiWidgets(Ui_RegridParmsDialog* ui, int index) {

  // BUG: non-portable init of array from non-compiletime constants:

  QWidget* g0[] = {
    ui->vgtypSpinBox,
    ui->gdtypSpinBox,
    ui->ncolsSpinBox,
    ui->nrowsSpinBox,
    ui->nlaysSpinBox,
    ui->xorigDSpinBox,
    ui->yorigDSpinBox,
    ui->xcellDSpinBox,
    ui->ycellDSpinBox,
    ui->vgtopDSpinBox,
    ui->palpDSpinBox,
    ui->pbetDSpinBox,
    ui->pgamDSpinBox,
    ui->xcentDSpinBox,
    ui->ycentDSpinBox,
    ui->radiusDSpinBox,
    ui->gDSpinBox,
    ui->rDSpinBox,
    ui->aDSpinBox,
    ui->t0DSpinBox,
    ui->p0DSpinBox,
    ui->vglvlsLineEdit
  };

  QWidget *g1[2] = {
    ui->vgtypSpinBox,
    ui->gdtypSpinBox,
  };

  const static QString style_disabled("background-color: rgba(190, 190, 190, 30%);  color: rgba(0, 0, 0, 100%);" );  // disabled
  const static QString style_enabled("background-color:  rgba(240, 240, 240, 100%); color: rgba(0, 0, 0, 100%);"); // enabled

  // disable everything

  for (unsigned int i=0; i<sizeof(g0)/sizeof(g0[0]); i++) {
    g0[i]->setDisabled(true);
    //g0[i]->setStyleSheet(style_disabled);
  }

  for (unsigned int i=0; i<2; i++) {
    g1[i]->setDisabled(true);
    //g1[i]->setStyleSheet(style_disabled);
  }
        
  // selectively enable or disable certain elements
  bool disableFlag = true; //default
  QString newStyle;
  if (ui->chk_userOverride->isChecked()) {
    disableFlag = false;
    newStyle = style_enabled;
  } else {
    disableFlag = true;
    newStyle = style_disabled;
  }

  for (unsigned int i=0; i<sizeof(g0)/sizeof(g0[0]); i++) {
    g0[i]->setDisabled(disableFlag);
    //g0[i]->setStyleSheet(newStyle);
  }
  
  for (unsigned int i=0; i<2; i++) {
    g1[i]->setDisabled(disableFlag);
    //g1[i]->setStyleSheet(newStyle);
  }


  if (0) {
  
  switch (index) {
  case 0:  break;
  case 1:  break;
  case 2:  break;
  case 3:  break;
  case 4:  break;
  case 5:  break;
  case 6:  break;
  case 7:  break;
  case 8:  break;
  case 9:  break;
  case 10: break;
  case 11: break;
  case 12: break;
  case 13: break;
  case 14: break;
  case 15: break;
  case 16:  
    // lonlat option
    for (unsigned int i=0; i<sizeof(g0)/sizeof(g0[0]); i++) {
      g0[i]->setDisabled(false);
      //g0[i]->setStyleSheet(style_enabled);
    }

    for (unsigned int i=0; i<2; i++) {
      g1[i]->setDisabled(true);
      //g1[i]->setStyleSheet(style_disabled);
    }

    break;
    //case 17:
    //// custom option
    //for (unsigned int i=0; i<sizeof(g0)/sizeof(g0[0]); i++) {
    //  g0[i]->setDisabled(false);
    //  g0[i]->setStyleSheet(style_enabled);
    //}
    //
    //for (unsigned int i=0; i<2; i++) {
    //  g1[i]->setDisabled(false);
    //  g1[i]->setStyleSheet(style_enabled);
    //}
    //
    //break;
  default:

    for (unsigned int i=0; i<sizeof(g0)/sizeof(g0[0]); i++) {
      g0[i]->setDisabled(true);
      //g0[i]->setStyleSheet(style_disabled);
    }

    break;
  };


  }
  
}



static void setUiUnits(Ui_RegridParmsDialog *ui, int index) {

  if ( cmaqGridList[index].startsWith("BBOX") ||
       cmaqGridList[index].startsWith("LONLAT") ||
       ui->gdtypSpinBox->value() == 1 ) { // bbox regridding
    ui->label_xOrig->setText("deg");
    ui->label_yOrig->setText("deg");
    ui->label_xCell->setText("deg");
    ui->label_yCell->setText("deg");
  } else {
    ui->label_xOrig->setText("m");
    ui->label_yOrig->setText("m");
    ui->label_xCell->setText("m");
    ui->label_yCell->setText("m");
  }
}



void RegridParmsDialog::showRegridElements() {
  // show or hide certain elements on the regrid panel.
  // useful for hiding those pertaining to Lambert projections
  // when selecting a non-Lambert projection. 

  QString curGrid = ui->cmaqgridComboBox->currentText();
  
  const bool isLonLat =
    ( curGrid == "AQMEII_NA" ||
      curGrid == "AQMEII_EU" ||
      curGrid == "BBOX" ||
      curGrid == "LONLAT" ||
      ui->gdtypSpinBox->value() == 1 );

  if ( isLonLat ) {
    ui->palpDSpinBox->hide();
    ui->pbetDSpinBox->hide();
    ui->pgamDSpinBox->hide();
    ui->xcentDSpinBox->hide();
    ui->ycentDSpinBox->hide();
  } else {
    ui->palpDSpinBox->show();
    ui->pbetDSpinBox->show();
    ui->pgamDSpinBox->show();
    ui->xcentDSpinBox->show();
    ui->ycentDSpinBox->show();
  }
}



RegridParmsDialog::RegridParmsDialog(QWidget *parent): QDialog(parent) {
  ui = new Ui_RegridParmsDialog;
  ui->setupUi(this);
  //this->setStyleSheet("QToolTip:disabled {background-color: red; color: blue; }");

  for (unsigned int i=0; i<sizeof(cmaqGridList)/sizeof(cmaqGridList[0]); i++) {
    ui->cmaqgridComboBox->addItem(cmaqGridList[i]);
  }

  const QString  methodVarList[] = {"Weighted ( 1/r2 )", "Mean"};

  for (unsigned int i=0; i<sizeof(methodVarList)/sizeof(methodVarList[0]); i++) {
    ui->regridmethodComboBox->addItem(methodVarList[i]);
  }

  const QString  aggregateList[] = {"None", "All", "Daily"};

  for (unsigned int i=0; i<sizeof(aggregateList)/sizeof(aggregateList[0]); i++) {
    ui->regridAggregationComboBox->addItem(aggregateList[i]);
  }

  setUiValues(ui, staticData);
  setUiUnits(ui, staticData.cmaqGrid);

  disableUiWidgets(ui, staticData.cmaqGrid);
  showRegridElements();

  connect(ui->acceptPushButton, SIGNAL(clicked()), this, SLOT(onOK()));
  connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancel()));
  connect(ui->resetPushButton, SIGNAL(clicked()), this, SLOT(onReset()));
  connect(ui->cmaqgridComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCmaqGridIndexChanged(int)));
  connect(ui->gdtypSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onCmaqGridTypeChanged(int)));
  connect(ui->chk_userOverride, SIGNAL(stateChanged(int)), this, SLOT(onUserOverrideChanged()));

  // tooltips
  ui->vgtypSpinBox->setToolTip("Vertical coordinate type\n1 = hydrostatic sigma-P\n2 = nonhydrostatic sigma-P\n3 = sigma-Z\n4 = pressure (mb)\n5 = Z (m above MSL)\n6 = H (m above ground)");
  ui->gdtypSpinBox->setToolTip("Map projection type\n1 = lat-lon\n2 = Lamber conformal conic\n3 = general tangent Mercator\n4 = general tangent stereographic\n5 = UTM\n6 = polar secant stereographic\n7 = equatorial secant Mercator\n8 = transverse secant Mercator");
  ui->ncolsSpinBox->setToolTip("Number of grid columns");
  ui->nrowsSpinBox->setToolTip("Number of grid rows");
  ui->nlaysSpinBox->setToolTip("Number of grid layers");
  ui->xorigDSpinBox->setToolTip("X-coordinate of lower left corner of the (1,1) grid cell (map units)");
  ui->yorigDSpinBox->setToolTip("Y-coordinate of lower left corner of the (1,1) grid cell (map units)");
  ui->xcellDSpinBox->setToolTip("X-coordinate cell dimension (map units)");
  ui->ycellDSpinBox->setToolTip("Y-coordinate cell dimension (map units)");
  ui->vgtopDSpinBox->setToolTip("Model top (for sigma vertical coordinate types)");
  ui->palpDSpinBox->setToolTip("First map projection descriptive parameter");
  ui->pbetDSpinBox->setToolTip("Second map projection descriptive parameter");
  ui->pgamDSpinBox->setToolTip("Third map projection descriptive parameter");
  ui->xcentDSpinBox->setToolTip("Longitude of the Cartesian map projection coordinate-origin (location where X=Y=0)");
  ui->ycentDSpinBox->setToolTip("Latitude of the Cartesian map projection coordinate-origin (map units)");
  ui->radiusDSpinBox->setToolTip("Earth radius in meters");
  ui->gDSpinBox->setToolTip("Acceleration due to gravity");
  ui->rDSpinBox->setToolTip("Specific gas constant");
  ui->aDSpinBox->setToolTip("Atmospheric lapse rate");
  ui->t0DSpinBox->setToolTip("Reference surface temperature");
  ui->p0DSpinBox->setToolTip("Reference surface pressure");
  ui->vglvlsLineEdit->setToolTip("Array of vertical coordinate level values; level 1 of the grid goes from vertical coordinate VGLEVELS[0] to VGLEVELS[1], etc.");
  ui->acceptPushButton->setToolTip("Use new grid choices");
  ui->cancelPushButton->setToolTip("Cancel new grid choices");
  ui->resetPushButton->setToolTip("Reset all grids to their default values");
  ui->chk_userOverride->setToolTip("Allow user to edit grid parameters to create custom grids");
  ui->cmaqgridComboBox->setToolTip("Choose from popular predefined grid descriptions");
  ui->regridmethodComboBox->setToolTip("Spatial aggregation method. See https://www.epa.gov/hesc/how-rsig-regrids-data");
  ui->regridAggregationComboBox->setToolTip("Temporal aggregation method. See https://www.epa.gov/hesc/how-rsig-regrids-data");
}



RegridParmsDialog::~RegridParmsDialog() {
  delete ui;
}



void RegridParmsDialog::onOK() {
  getUiValues(ui, staticData);
  this->close();
}



void RegridParmsDialog::onCancel() {
  this->close();
}



void RegridParmsDialog::onReset() {
  staticData.init(0);
  setUiValues(ui, staticData);
  setUiUnits(ui, staticData.cmaqGrid);
  disableUiWidgets(ui, 0);
  update();
}



void RegridParmsDialog::onCmaqGridIndexChanged(int index) {
  RegridParmsData data(index);
  data.regridMethod = ui->regridmethodComboBox->currentIndex();
  data.regridAggregation = ui->regridAggregationComboBox->currentIndex();
  setUiValues(ui, data);
  setUiUnits(ui, index);
  disableUiWidgets(ui, index);
  showRegridElements();
  update();
}



void RegridParmsDialog::onCmaqGridTypeChanged(int) {
  const int gridIndex = ui->cmaqgridComboBox->currentIndex();
  RegridParmsData data(gridIndex);
  setUiUnits(ui, gridIndex);
  showRegridElements();
  update();
}


void RegridParmsDialog::onUserOverrideChanged() {
  disableUiWidgets(ui, ui->cmaqgridComboBox->currentIndex());
}



RegridParmsData* RegridParmsDialog::getRegridParmsData() {
  return &staticData;
}



