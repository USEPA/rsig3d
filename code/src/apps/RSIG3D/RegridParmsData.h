/******************************************************************************
PURPOSE: RegridParmsData.h - A class for RegridParmsData.

NOTES:

HISTORY: 2012/11/12, zhang.nan@epa.gov, Created.
STATUS:  unreviewed, untested.
******************************************************************************/

#ifndef REGRIDPARMSDATA_H_
#define REGRIDPARMSDATA_H_

#include <string>

struct RegridParmsData {
  int cmaqGrid;
  int regridMethod;
  int regridAggregation;

  int VGTYP;
  int GDTYP;
  int NCOLS;
  int NROWS;
  int NLAYS;

  double XORIG;
  double YORIG;
  double XCELL;
  double YCELL;
  double VGTOP;

  double PALP;
  double PBET;
  double PGAM;
  double XCENT;
  double YCENT;
  double RADIUS;

  double G;
  double R;
  double A;
  double T0;
  double P0;

  std::string VGLVLS;

  bool   userOverrideChecked;

  RegridParmsData(int index=1) {
    init(index);
  }

  void init(int index);

private:
  void init_36km_CONUS_parameters();
  void init_3km_HRRRCONUS_parameters();
  void init_2006CDC_EAST_parameters();
  void init_2006CDC_CONUS_parameters();
  void init_2008EQUATES_CONUS_parameters();
  void init_2008EQUATES_CONUS_1km_parameters();
  void init_2002EAST_parameters();
  void init_2002WEST_parameters();
  void init_hemisphere_parameters();
  void init_aqmeii_parameters();
  void init_aqmeii_na_parameters();
  void init_aqmeii_eu_parameters();
  void init_custom_lambert_parameters();
  void init_discover_conus_parameters();
  void init_discover_east_parameters();
  void init_discover_md_parameters();
  void init_listos1_parameters();
  void init_lmos1_parameters();
  void init_from_file();

public:
  void init_bbox_parameters();
  void init_lonlat_parameters();
};


#endif //REGRIDPARMSDATA_H_
