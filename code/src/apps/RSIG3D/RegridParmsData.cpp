/******************************************************************************
PURPOSE: regrid.C - Implements RegridParmsData class.

NOTES: a dialog class.

HISTORY: 2012/11/28, zhang.nan@epa.gov, Created.
******************************************************************************/

#include <iostream>
#include "RegridParmsData.h"
#include "RegridParmsDialog.h"
#include <QString>
#include <QFileDialog>
#include <QDebug> 
#include "CMAQGridParameters.h"
#include "VisOptions.h"
#include <string>
#include "ui_controldockwidget.h"

#include "MainWin.h"
#include "ControlDockWidget.h"

void RegridParmsData::init(int index) {
  cmaqGrid = index;
  
  switch (index) {
  default:
  case 0:
    regridMethod = 0;
    regridAggregation = 0;
    init_2008EQUATES_CONUS_parameters(); break;
  case 1:
    regridMethod = 0;
    regridAggregation = 0;
    init_2008EQUATES_CONUS_1km_parameters(); break;
  case 2: init_36km_CONUS_parameters(); break;
  case 3: init_2006CDC_CONUS_parameters(); break;
  case 4: init_2006CDC_EAST_parameters(); break;
  case 5: init_hemisphere_parameters(); break;
  case 6: init_aqmeii_parameters(); break;
  case 7: init_aqmeii_na_parameters(); break;
  case 8: init_aqmeii_eu_parameters(); break;
  case 9: init_discover_conus_parameters(); break;
  case 10: init_discover_east_parameters(); break;
  case 11: init_discover_md_parameters(); break;
  case 12: init_3km_HRRRCONUS_parameters(); break;
  case 13: init_listos1_parameters(); break;
  case 14: init_lmos1_parameters(); break;
  case 15: init_bbox_parameters(); break; // regrid everything to a giant cell equivalent to the bbox
  case 16: init_lonlat_parameters(); break; // regular lonlat grid
    //case 17: init_custom_lambert_parameters(); break; //User defined
  case 17: init_from_file(); break; // Read from file
  }
}



void RegridParmsData::init_2006CDC_EAST_parameters() {
  VGTYP = 2;
  GDTYP = 2;
  NCOLS = 279;
  NROWS = 240;
  NLAYS = 24;

  XORIG = -1008000.0;
  YORIG = -1620000.0;
  XCELL = 12000.0;
  YCELL = 12000.0;
  VGTOP = 10000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.995, 0.99, 0.98, 0.97, 0.96, 0.94, 0.92, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0");
}



void RegridParmsData::init_2006CDC_CONUS_parameters() {
  VGTYP = 7;
  GDTYP = 2;
  NCOLS = 459;
  NROWS = 299;
  NLAYS = 24;

  XORIG = -2556000.0;
  YORIG = -1728000.0;
  XCELL = 12000.0;
  YCELL = 12000.0;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.995, 0.99, 0.98, 0.97, 0.96, 0.94, 0.92, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0");
}



void RegridParmsData::init_36km_CONUS_parameters() {
  VGTYP = 2;
  GDTYP = 2;
  NCOLS = 148;
  NROWS = 112;
  NLAYS = 1;

  XORIG = -2736000.0;
  YORIG = -2088000.0;
  XCELL = 36000.0;
  YCELL = 36000.0;
  VGTOP = 10000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.0");
}

void RegridParmsData::init_3km_HRRRCONUS_parameters() {
  VGTYP = 2;
  GDTYP = 2;
  NCOLS = 1799;
  NROWS = 1059;
  NLAYS = 1;

  XORIG = -2699020.14252193;
  YORIG = -1588806.15255666;
  XCELL = 3000.0;
  YCELL = 3000.0;
  VGTOP = 10000.0;

  PALP = 38.5;
  PBET = 38.5;
  PGAM = -97.5;
  XCENT = -97.5;
  YCENT = 38.5;
  RADIUS = 6371229.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.0");
}

void RegridParmsData::init_2008EQUATES_CONUS_parameters() {
  VGTYP = 7;
  GDTYP = 2;
  NCOLS = 459;
  NROWS = 299;
  NLAYS = 35;

  XORIG = -2556000.0;
  YORIG = -1728000.0;
  XCELL = 12000.0;
  YCELL = 12000.0;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.9975, 0.995, 0.99, 0.985, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92, 0.91, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.05, 0.0");
}

void RegridParmsData::init_2008EQUATES_CONUS_1km_parameters() {
  VGTYP = 2;
  GDTYP = 2;
  NCOLS = 5508;
  NROWS = 3588;
  NLAYS = 1;

  XORIG = -2556000.0;
  YORIG = -1728000.0;
  XCELL = 1000.0;
  YCELL = 1000.0;
  VGTOP = 10000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.0");
}

void RegridParmsData::init_hemisphere_parameters() {
  VGTYP = 7;
  GDTYP = 6;
  NCOLS = 187;
  NROWS = 187;
  NLAYS = 35;

  XORIG = -10098000.0;
  YORIG = -10098000.0;
  XCELL = 108000.0;
  YCELL = 108000.0;
  VGTOP = 5000.0;

  PALP = 1.0;
  PBET = 45.0;
  PGAM = -98.0;
  XCENT = -98.0;
  YCENT = 90.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 275.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.9975, 0.995, 0.99, 0.985, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92, 0.91, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.05, 0.0");
}



void RegridParmsData::init_aqmeii_parameters() {
  VGTYP = 2;
  GDTYP = 2;
  NCOLS = 459;
  NROWS = 299;
  NLAYS = 34;

  XORIG = -2556000.0;
  YORIG = -1728000.0;
  XCELL = 12000.0;
  YCELL = 12000.0;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 275.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.995, 0.99, 0.985, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92, 0.91, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.05, 0.0");
}



void RegridParmsData::init_aqmeii_na_parameters() {
  VGTYP = 6;
  GDTYP = 1;
  NCOLS = 282;
  NROWS = 140;
  NLAYS = 28;

  XORIG = -130.0;
  YORIG = 23.5;
  XCELL = 0.25;
  YCELL = 0.25;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 275.0;
  P0 = 100000.0;
  VGLVLS = std::string("0, 250, 500, 750, 1000, 1250, 1500, 1750, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000, 10500, 11000, 11500, 12000");
}



void RegridParmsData::init_aqmeii_eu_parameters() {
  VGTYP = 6;
  GDTYP = 1;
  NCOLS = 200;
  NROWS = 140;
  NLAYS = 28;

  XORIG = -15.0;
  YORIG = 35.0;
  XCELL = 0.25;
  YCELL = 0.25;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 275.0;
  P0 = 100000.0;
  VGLVLS = std::string("0, 250, 500, 750, 1000, 1250, 1500, 1750, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000, 10500, 11000, 11500, 12000");
}



void RegridParmsData::init_discover_conus_parameters() {
  VGTYP = 7;
  GDTYP = 2;
  NCOLS = 459;
  NROWS = 299;
  NLAYS = 35;

  XORIG = -2556000;
  YORIG = -1728000;
  XCELL = 12000;
  YCELL = 12000;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.9975, 0.995, 0.99, 0.985, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92, 0.91, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.05, 0.0");
}



void RegridParmsData::init_discover_east_parameters() {
  VGTYP = 7;
  GDTYP = 2;
  NCOLS = 300;
  NROWS = 315;
  NLAYS = 35;

  XORIG = 1056000;
  YORIG = -636000;
  XCELL = 4000;
  YCELL = 4000;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.9975, 0.995, 0.99, 0.985, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92, 0.91, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.05, 0.0");
}



void RegridParmsData::init_discover_md_parameters() {
  VGTYP = 7;
  GDTYP = 2;
  NCOLS = 304;
  NROWS = 336;
  NLAYS = 35;

  XORIG = 1594000;
  YORIG = -98000;
  XCELL = 1000;
  YCELL = 1000;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.9975, 0.995, 0.99, 0.985, 0.98, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92, 0.91, 0.9, 0.88, 0.86, 0.84, 0.82, 0.8, 0.77, 0.74, 0.7, 0.65, 0.6, 0.55, 0.5, 0.45, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.05, 0.0");
}



void RegridParmsData::init_listos1_parameters() {
  VGTYP = 7;
  GDTYP = 2;
  NCOLS = 300;
  NROWS = 315;
  NLAYS = 1;

  XORIG = 1140000;
  YORIG = -456000;
  XCELL = 4000;
  YCELL = 4000;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("0.0");
}



void RegridParmsData::init_lmos1_parameters() {
  VGTYP = 7;
  GDTYP = 2;
  NCOLS = 162;
  NROWS = 228;
  NLAYS = 1;

  XORIG = 420000;
  YORIG = -108000;
  XCELL = 4000;
  YCELL = 4000;
  VGTOP = 5000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("0.0");
}

void RegridParmsData::init_bbox_parameters() {

  VGTYP = 6;
  GDTYP = 1;
  NCOLS = 1;
  NROWS = 1;
  NLAYS = 1;

  MainWin& mainWin = MainWin::getInstance();
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
  XORIG = controlDockWidget->get_currentGuiBbox_minLon();
  YORIG = controlDockWidget->get_currentGuiBbox_minLat();
  XCELL = controlDockWidget->get_currentGuiBbox_maxLon() - XORIG;
  YCELL = controlDockWidget->get_currentGuiBbox_maxLat() - YORIG;
  VGTOP = 5000.0;

  PALP   = 0.0;
  PBET   = 0.0;
  PGAM   = 0.0;
  XCENT  = 0.0;
  YCENT  = 0.0;
  RADIUS = 6370000.0;
  
  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("0.0, 1.0");

}



void RegridParmsData::init_lonlat_parameters() {

  VGTYP = 6;
  GDTYP = 1;
  NCOLS = 1;
  NROWS = 1;
  NLAYS = 1;

  MainWin& mainWin = MainWin::getInstance();
  ControlDockWidget* const controlDockWidget = mainWin.controlWidget();
  XORIG = controlDockWidget->get_currentGuiBbox_minLon();
  YORIG = controlDockWidget->get_currentGuiBbox_minLat();
  XCELL = controlDockWidget->get_currentGuiBbox_maxLon() - XORIG;
  YCELL = controlDockWidget->get_currentGuiBbox_maxLat() - YORIG;
  VGTOP = 5000.0;
  
  PALP   = 0.0;
  PBET   = 0.0;
  PGAM   = 0.0;
  XCENT  = 0.0;
  YCENT  = 0.0;
  RADIUS = 6370000.0;
  
  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("0.0, 1.0");
}



void RegridParmsData::init_custom_lambert_parameters() {
  VGTYP = 2;
  GDTYP = 2;
  NCOLS = 279;
  NROWS = 240;
  NLAYS = 14;

  XORIG = -1008000.0;
  YORIG = -1620000.0;
  XCELL = 12000.0;
  YCELL = 12000.0;
  VGTOP = 10000.0;

  PALP = 33.0;
  PBET = 45.0;
  PGAM = -97.0;
  XCENT = -97.0;
  YCENT = 40.0;
  RADIUS = 6370000.0;

  G = 9.81;
  R = 287.04;
  A = 50.0;
  T0 = 290.0;
  P0 = 100000.0;
  VGLVLS = std::string("1.0, 0.995, 0.99, 0.98, 0.96, 0.94, 0.91, 0.86, 0.8, 0.74, 0.65, 0.55, 0.4, 0.2, 0.0");
}



void RegridParmsData::init_from_file() {
  QFileDialog mydialog;
  mydialog.setFileMode(QFileDialog::AnyFile);
  mydialog.setNameFilter("CMAQ (*.ncf *.*)");
  QString filename( mydialog.getOpenFileName(NULL, "Open File", "", "Files (*)") );

  qDebug() << "Reading " << filename;

  try {
    const emvl::CMAQGridParameters cmaqGridParameters( filename.toUtf8().constData() ); //x
    
    VGTYP = cmaqGridParameters.vgtyp();
    GDTYP = cmaqGridParameters.gdtyp();
    
    NCOLS = cmaqGridParameters.ncols();
    NROWS = cmaqGridParameters.nrows();
    NLAYS = cmaqGridParameters.nlays();

    XORIG = cmaqGridParameters.xorig();
    YORIG = cmaqGridParameters.yorig();
    XCELL = cmaqGridParameters.xcell();
    YCELL = cmaqGridParameters.ycell();
    VGTOP = cmaqGridParameters.vgtop();

    PALP = cmaqGridParameters.p_alp();
    PBET = cmaqGridParameters.p_bet();
    PGAM = cmaqGridParameters.p_gam();
    XCENT = cmaqGridParameters.xcent();
    YCENT = cmaqGridParameters.ycent();
    RADIUS = cmaqGridParameters.sphereRadius();

    G =  cmaqGridParameters.g();
    R =  cmaqGridParameters.R();
    A =  cmaqGridParameters.A();
    T0 = cmaqGridParameters.T0();
    P0 = cmaqGridParameters.P0();
    
    const double * lvl_array = cmaqGridParameters.vglvls();
    int num_lvls = NLAYS+1;
    char this_string[5];

    for (int ind=0; ind<num_lvls; ind++) {
      snprintf(this_string, 5, "%f5.3", lvl_array[ind]);
      VGLVLS.append( this_string );

      if (ind < num_lvls-1) {
        VGLVLS.append( ", " );
      }
    }

  } catch ( ... ) {
    qDebug() << "Could not process CMAQ file";
  }
}



