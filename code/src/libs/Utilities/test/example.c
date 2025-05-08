/*
cc -g -I. -o example example.c Shapefile.c lambert.c projections.c -lm
*/

#include <lambert.h> /* For initialize_lambert(). */
#include <Shapefile.h> /* For writeGridToShapefile(). */

int main( void ) {
  const char* const fileName = "example";
  enum { TIMESTAMP = 20060703, TIMESTEPS = 2, ROWS = 5, COLUMNS = 4 };
  const double westEdge    = 1578000.0;
  const double southEdge   = -1270000.0;
  const double cellWidth   = 2000.0;
  const double cellHeight  = cellWidth;
  const char* const variable = "ozone(ppm)";
  const float data[ TIMESTEPS * ROWS * COLUMNS ] = {
    0.03664103, 0.03672275, 0.03678665, 0.03682948,
    0.03651726, 0.03648462, 0.03639814, 0.03637281,
    0.03616524, 0.03602019, 0.03601136, 0.03608273,
    0.03577007, 0.03582532, 0.03591563, 0.03603813,
    0.03574514, 0.0357785,  0.0358368,  0.03597659,
    0.13664103, 0.13672275, 0.13678665, 0.13682948,
    0.13651726, 0.13648462, 0.13639814, 0.13637281,
    0.13616524, 0.13602019, 0.13601136, 0.13608273,
    0.13577007, 0.13582532, 0.13591563, 0.13603813,
    0.13574514, 0.1357785,  0.1358368,  0.13597659
  };
  int ok = 0;
  initialize_lambert( 6370000.0, 6370000.0, 30.0, 60.0, 40.0, -100.0, 0.0,0.0);
  ok = writeGridToShapefile( fileName, TIMESTAMP, TIMESTEPS, ROWS, COLUMNS,
                             westEdge, southEdge, cellWidth, cellHeight,
                             variable, data, unproject_lambert );
  return ! ok;
}

