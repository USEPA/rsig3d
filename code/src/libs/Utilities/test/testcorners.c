/*
gcc -Wall -g -o testcorners testcorners.c ; testcorners
*/

#include <stdio.h>  /* For printf(). */
#include <string.h> /* For memset(). */

#define PRE08(unused1,unused2,unused3,unused4,unused5,unused6,unused7,unused8)\
  extern int errno
#define POST02(unused1,unused2)

#define CLAMPED_TO_RANGE( value, low, high ) \
((value) < (low) ? (low) : (value) > (high) ? (high) : (value))

#define PRE07(unused1,unused2,unused3,unused4,unused5,unused6,unused7)\
  extern int errno
#define POST0(unused)
#define OR2(a,b) ((a)||(b))
typedef long long Integer;
typedef double Real;

void computeGridCellVertices( const int rows, const int columns,
                              const float longitudes[],
                              const float latitudes[], float vertices[] );

static void computeCorners( Integer rows, Integer columns,
                            const Real longitudes[], const Real latitudes[],
                            Real corners[] );

int main( void ) {
#if 0
  enum {
    ROWS = 2, COLUMNS = 3, CELLS = ROWS * COLUMNS,
    VROWS = ROWS + 1, VCOLUMNS = COLUMNS + 1
  };
  const float longitudes[ ROWS * COLUMNS ] = {
    -93.0f, -92.0f, -91.0f,
    -93.1f, -92.1f, -91.1f
  };
  const float latitudes[ ROWS * COLUMNS ] = {
    30.0f, 30.1f, 30.2f,
    31.0f, 31.1f, 31.2f
  };
#else
  enum {
    ROWS = 3, COLUMNS = 4, CELLS = ROWS * COLUMNS,
    VROWS = ROWS + 1, VCOLUMNS = COLUMNS + 1
  };
  const float longitudes[ ROWS * COLUMNS ] = {
    -93.0f, -92.0f, -91.0f, -90.0f,
    -93.1f, -92.1f, -91.1f, -90.1f,
    -93.2f, -92.2f, -91.2f, -90.2f
  };
  const float latitudes[ ROWS * COLUMNS ] = {
    30.0f, 30.1f, 30.2f, 30.3f,
    31.0f, 31.1f, 31.2f, 31.3f,
    32.0f, 32.1f, 32.2f, 32.3f
  };  
#endif
  float vertices[ VROWS * VCOLUMNS * 2 ];
  Real longitudes2[ ROWS * COLUMNS ];
  Real latitudes2[ ROWS * COLUMNS ];
  Real corners[ ROWS * COLUMNS * 8 ];
  Real* const longitudes_sw = corners;
  Real* const longitudes_se = longitudes_sw + CELLS;
  Real* const longitudes_nw = longitudes_se + CELLS;
  Real* const longitudes_ne = longitudes_nw + CELLS;
  Real* const latitudes_sw  = longitudes_ne + CELLS;
  Real* const latitudes_se  = latitudes_sw + CELLS;
  Real* const latitudes_nw  = latitudes_se + CELLS;
  Real* const latitudes_ne  = latitudes_nw + CELLS;
  int index = 0;
  int row = 0;

  memset( vertices, 0, sizeof vertices );
  memset( longitudes2, 0, sizeof longitudes2 );
  memset( latitudes2, 0, sizeof latitudes2 );
  memset( corners, 0, sizeof corners );

  for ( index = 0; index < CELLS; ++index ) {
    longitudes2[ index ] = longitudes[ index ];
    latitudes2[  index ] = latitudes[  index ];
  }

  computeGridCellVertices( ROWS, COLUMNS, longitudes, latitudes, vertices );

  printf( "\ninput:\n" );

  for ( row = 0, index = 0; row < ROWS; ++row ) {
    int column = 0;

    for ( column = 0; column < COLUMNS; ++column, ++index ) {
      const float longitude = longitudes[ index ];
      const float latitude  = latitudes[ index ];
      printf( "[%g %g] ", longitude, latitude );
    }

    printf( "\n" );
  }

  printf( "\noutput vertices:\n" );

  for ( row = 0, index = 0; row < VROWS; ++row ) {
    int column = 0;

    for ( column = 0; column < VCOLUMNS; ++column, index += 2 ) {
      const float longitude = vertices[ index ];
      const float latitude  = vertices[ index + 1 ];
      printf( "[%g %g] ", longitude, latitude );
    }

    printf( "\n" );
  }

  printf( "\ncomputeCorners():\n" );

  computeCorners( ROWS, COLUMNS, longitudes2, latitudes2, corners );

  printf( "\noutput corners:\n" );

  for ( row = 0, index = 0; row < ROWS; ++row ) {
    int column = 0;

    for ( column = 0; column < COLUMNS; ++column, ++index ) {
      const Real longitude_sw = longitudes_sw[ index ];
      const Real longitude_se = longitudes_se[ index ];
      const Real longitude_nw = longitudes_nw[ index ];
      const Real longitude_ne = longitudes_ne[ index ];
      const Real latitude_sw = latitudes_sw[ index ];
      const Real latitude_se = latitudes_se[ index ];
      const Real latitude_nw = latitudes_nw[ index ];
      const Real latitude_ne = latitudes_ne[ index ];
      printf( "sw[%lg %lg] ", longitude_sw, latitude_sw );
      printf( "se[%lg %lg] ", longitude_se, latitude_se );
      printf( "nw[%lg %lg] ", longitude_nw, latitude_nw );
      printf( "ne[%lg %lg] ", longitude_ne, latitude_ne );
      printf( "\n" );
    }

    printf( "\n" );
  }

  return 0;
}



/******************************************************************************
PURPOSE: computeGridCellVertices - Compute lon-lat coordinates of rectangular
         grid cell corners.
INPUTS:  const int rows                            Number of grid row cells.
         const int columns                         Number of grid column cells.
         const float longitudes[ rows * columns ]  Longitude of cell center.
         const float latitudes[  rows * columns ]  Latitude of cell center.
OUTPUTS: float vertices[ ( rows + 1 ) * ( columns + 1 ) * 2 ]
         Corners in grid cell order with interleaved lon-lat coordinates.
NOTES:   Uses linear interpolation and extrapolation to the edges.
******************************************************************************/

void computeGridCellVertices( const int rows, const int columns,
                              const float longitudes[],
                              const float latitudes[], float vertices[] ) {

  PRE08( rows > 1, columns > 1, rows * columns >= 4,
         longitudes, latitudes, vertices,
         is_valid_longitude_latitude( longitudes[ 0 ], latitudes[ 0 ] ),
         is_valid_longitude_latitude( longitudes[ rows * columns - 1 ],
                                      latitudes[ rows * columns - 1 ] ) );

  const int rows_1    = rows - 1;
  const int columns_1 = columns - 1;
  const int columnsPlus1 = columns + 1;
  const int columnsPlus1Times2 = columnsPlus1 + columnsPlus1;
  const int count = ( rows + 1 ) * ( columns + 1 ) * 2;
  int row    = 0;
  int column = 0;
  int index = 0;
  int vIndex = 0;

  /*
   * First compute linearly interpolated corners of all interior cells:
   * Note: rows increase north to south and columns increase west to east.
   */

  printf( "\ninterpolated interior vertices:\n" );

#pragma omp parallel for private( column )

  for ( row = 0; row < rows_1; ++row ) {
    const int nextRow        = row + 1;
    const int rowOffset      = row     * columns;
    const int nextRowOffset  = nextRow * columns;
    const int verticesOffset = nextRow * columnsPlus1Times2 + 2;

    /* Interior row, interior columns: */

    for ( column = 0; column < columns_1; ++column ) {
      const int nextColumn             = column + 1;
      const int verticesIndex          = verticesOffset + column + column;
      const int dataIndex              = rowOffset + column;
      const int nextColumnIndex        = dataIndex + 1;
      const int nextRowIndex           = nextRowOffset + column;
      const int nextRowNextColumnIndex = nextRowOffset + nextColumn;

      const float longitude                  = longitudes[ dataIndex ];
      const float nextColumnLongitude        = longitudes[ nextColumnIndex ];
      const float nextRowLongitude           = longitudes[ nextRowIndex ];
      const float nextRowNextColumnLongitude =
        longitudes[ nextRowNextColumnIndex ];

      const float latitude                  = latitudes[ dataIndex ];
      const float nextColumnLatitude        = latitudes[ nextColumnIndex ];
      const float nextRowLatitude           = latitudes[ nextRowIndex ];
      const float nextRowNextColumnLatitude =
        latitudes[ nextRowNextColumnIndex ];
      const float interpolatedLongitude = 0.25 *
        ( longitude + nextColumnLongitude +
          nextRowLongitude + nextRowNextColumnLongitude );
      const float interpolatedLatitude = 0.25 *
        ( latitude + nextColumnLatitude +
          nextRowLatitude + nextRowNextColumnLatitude );

      vertices[ verticesIndex     ] = interpolatedLongitude;
      vertices[ verticesIndex + 1 ] = interpolatedLatitude;

      printf( "[%d] = %g, ", verticesIndex, interpolatedLongitude );
      printf( "[%d] = %g\n", verticesIndex + 1, interpolatedLatitude );

    } /* End loop on interior columns. */

  } /* End parallel loop on interior rows. */

  /* Serial region (not worth parallelizing): */

  /* Last row, interior columns (extrapolated top edge, except corners): */

  printf( "\nextrapolated top edge, except corners:\n" );

  for ( column = 1, index = rows_1 * columns + 1,
        vIndex = rows_1 * columnsPlus1Times2 + 2;
        column < columns; ++column, vIndex += 2, ++index ) {
    const int previousColumnIndex       = index - 1;
    const int extrapolatedIndex         = vIndex + columnsPlus1Times2;
    const float longitude               = longitudes[ index ];
    const float latitude                = latitudes[ index ];
    const float previousColumnLongitude = longitudes[ previousColumnIndex ];
    const float previousColumnLatitude  = latitudes[ previousColumnIndex ];
    const float midpointLongitude       =
      0.5 * ( longitude + previousColumnLongitude );
    const float midpointLatitude        =
      0.5 * ( latitude  + previousColumnLatitude );
    const float extrapolatedInputLongitude = vertices[ vIndex ];
    const float extrapolatedInputLatitude  = vertices[ vIndex + 1 ];
    const float longitudeDifference =
      midpointLongitude - extrapolatedInputLongitude;
    const float latitudeDifference  =
      midpointLatitude - extrapolatedInputLatitude;
    const float extrapolatedLongitude = midpointLongitude +longitudeDifference;
    const float extrapolatedLatitude = midpointLatitude + latitudeDifference;
    vertices[ extrapolatedIndex     ] = extrapolatedLongitude;
    vertices[ extrapolatedIndex + 1 ] = extrapolatedLatitude;
    printf( "[%d] = %g, ", extrapolatedIndex, extrapolatedLongitude );
    printf( "[%d] = %g\n", extrapolatedIndex + 1, extrapolatedLatitude );
  }

  /* First row, interior columns (extrapolated bottom edge, except corners): */

  printf( "\nextrapolated bottom edge, except corners:\n" );

  for ( column = 1, index = 1, vIndex = columnsPlus1Times2 + 2;
        column < columns; ++column, vIndex += 2, ++index ) {
    const int previousColumnIndex       = index - 1;
    const int extrapolatedIndex         = vIndex - columnsPlus1Times2;
    const float longitude               = longitudes[ index ];
    const float latitude                = latitudes[ index ];
    const float previousColumnLongitude = longitudes[ previousColumnIndex ];
    const float previousColumnLatitude  = latitudes[ previousColumnIndex ];
    const float midpointLongitude       =
      0.5 * ( longitude + previousColumnLongitude );
    const float midpointLatitude        =
      0.5 * ( latitude  + previousColumnLatitude );
    const float extrapolatedInputLongitude = vertices[ vIndex ];
    const float extrapolatedInputLatitude  = vertices[ vIndex + 1 ];
    const float longitudeDifference =
      midpointLongitude - extrapolatedInputLongitude;
    const float latitudeDifference  =
      midpointLatitude - extrapolatedInputLatitude;
    const float extrapolatedLongitude = midpointLongitude +longitudeDifference;
    const float extrapolatedLatitude = midpointLatitude + latitudeDifference;
    vertices[ extrapolatedIndex     ] = extrapolatedLongitude;
    vertices[ extrapolatedIndex + 1 ] = extrapolatedLatitude;
    printf( "[%d] = %g, ", extrapolatedIndex, extrapolatedLongitude );
    printf( "[%d] = %g\n", extrapolatedIndex + 1, extrapolatedLatitude );
  }

  /* First column, interior rows (extrapolated left edge, except corners): */

  printf( "\nextrapolated left edge, except corners:\n" );

  for ( row = 1, index = columns, vIndex = columnsPlus1Times2 + 2;
        row < rows; ++row, vIndex += columnsPlus1Times2, index += columns ) {
    const int previousRowIndex          = index - columns;
    const int extrapolatedIndex         = vIndex - 2;
    const float longitude               = longitudes[ index ];
    const float latitude                = latitudes[ index ];
    const float previousRowLongitude    = longitudes[ previousRowIndex ];
    const float previousRowLatitude     = latitudes[ previousRowIndex ];
    const float midpointLongitude       =
      0.5 * ( longitude + previousRowLongitude );
    const float midpointLatitude        =
      0.5 * ( latitude  + previousRowLatitude );
    const float extrapolatedInputLongitude = vertices[ vIndex ];
    const float extrapolatedInputLatitude  = vertices[ vIndex + 1 ];
    const float longitudeDifference =
      midpointLongitude - extrapolatedInputLongitude;
    const float latitudeDifference  =
      midpointLatitude - extrapolatedInputLatitude;
    const float extrapolatedLongitude = midpointLongitude +longitudeDifference;
    const float extrapolatedLatitude = midpointLatitude + latitudeDifference;
    vertices[ extrapolatedIndex     ] = extrapolatedLongitude;
    vertices[ extrapolatedIndex + 1 ] = extrapolatedLatitude;
    printf( "[%d] = %g, ", extrapolatedIndex, extrapolatedLongitude );
    printf( "[%d] = %g\n", extrapolatedIndex + 1, extrapolatedLatitude );
  }

  /* Last column, interior rows (extrapolated right edge, except corners): */

  printf( "\nextrapolated right edge, except corners:\n" );

  for ( row = 1, index = columns + columns - 1,
        vIndex = columnsPlus1Times2 + columnsPlus1Times2 - 4;
        row < rows; ++row, vIndex += columnsPlus1Times2, index += columns ) {
    const int previousRowIndex          = index - columns;
    const int extrapolatedIndex         = vIndex + 2;
    const float longitude               = longitudes[ index ];
    const float latitude                = latitudes[ index ];
    const float previousRowLongitude    = longitudes[ previousRowIndex ];
    const float previousRowLatitude     = latitudes[ previousRowIndex ];
    const float midpointLongitude       =
      0.5 * ( longitude + previousRowLongitude );
    const float midpointLatitude        =
      0.5 * ( latitude  + previousRowLatitude );
    const float extrapolatedInputLongitude = vertices[ vIndex ];
    const float extrapolatedInputLatitude  = vertices[ vIndex + 1 ];
    const float longitudeDifference =
      midpointLongitude - extrapolatedInputLongitude;
    const float latitudeDifference  =
      midpointLatitude - extrapolatedInputLatitude;
    const float extrapolatedLongitude = midpointLongitude +longitudeDifference;
    const float extrapolatedLatitude = midpointLatitude + latitudeDifference;
    vertices[ extrapolatedIndex     ] = extrapolatedLongitude;
    vertices[ extrapolatedIndex + 1 ] = extrapolatedLatitude;
    printf( "[%d] = %g, ", extrapolatedIndex, extrapolatedLongitude );
    printf( "[%d] = %g\n", extrapolatedIndex + 1, extrapolatedLatitude );
  }

  /* First row, first column cell (extrapolated bottom-left corner): */

  printf( "\nextrapolated bottom-left corner:\n" );
  
  vIndex = columnsPlus1Times2 + 2;

  {
    const float longitude             = longitudes[ 0 ];
    const float latitude              = latitudes[ 0 ];
    const float diagonalLongitude     = vertices[ vIndex ];
    const float diagonalLatitude      = vertices[ vIndex + 1 ];
    const float longitudeDifference   = longitude - diagonalLongitude;
    const float latitudeDifference    = latitude  - diagonalLatitude;
    const float extrapolatedLongitude = longitude + longitudeDifference;
    const float extrapolatedLatitude  = latitude  + latitudeDifference;
    vertices[ 0 ]                     = extrapolatedLongitude;
    vertices[ 1 ]                     = extrapolatedLatitude;
    printf( "[0] = %g, ", extrapolatedLongitude );
    printf( "[1] = %g\n", extrapolatedLatitude );
  }

  /* First row, last column cell (extrapolated bottom-right corner): */

  printf( "\nextrapolated bottom-right corner:\n" );

  vIndex = columnsPlus1Times2 + columnsPlus1Times2 - 4;

  {
    const int extrapolatedIndex       = columnsPlus1Times2 - 2;
    const int dataIndex               = columns - 1;
    const float longitude             = longitudes[ dataIndex ];
    const float latitude              = latitudes[ dataIndex ];
    const float diagonalLongitude     = vertices[ vIndex ];
    const float diagonalLatitude      = vertices[ vIndex + 1 ];
    const float longitudeDifference   = longitude - diagonalLongitude;
    const float latitudeDifference    = latitude  - diagonalLatitude;
    const float extrapolatedLongitude = longitude + longitudeDifference;
    const float extrapolatedLatitude  = latitude  + latitudeDifference;
    vertices[ extrapolatedIndex ]     = extrapolatedLongitude;
    vertices[ extrapolatedIndex + 1 ] = extrapolatedLatitude;
    printf( "[%d] = %g, ", extrapolatedIndex, extrapolatedLongitude );
    printf( "[%d] = %g\n", extrapolatedIndex + 1, extrapolatedLatitude );
  }

  /* Last row, first column cell (extrapolated top-left corner): */

  printf( "\nextrapolated top-left corner:\n" );

  vIndex = rows_1 * columnsPlus1Times2 + 2;

  {
    const int extrapolatedIndex       = rows * columnsPlus1Times2;
    const int dataIndex               = rows_1 * columns;
    const float longitude             = longitudes[ dataIndex ];
    const float latitude              = latitudes[ dataIndex ];
    const float diagonalLongitude     = vertices[ vIndex ];
    const float diagonalLatitude      = vertices[ vIndex + 1 ];
    const float longitudeDifference   = longitude - diagonalLongitude;
    const float latitudeDifference    = latitude  - diagonalLatitude;
    const float extrapolatedLongitude = longitude + longitudeDifference;
    const float extrapolatedLatitude  = latitude  + latitudeDifference;
    vertices[ extrapolatedIndex     ] = extrapolatedLongitude;
    vertices[ extrapolatedIndex + 1 ] = extrapolatedLatitude;
    printf( "[%d] = %g, ", extrapolatedIndex, extrapolatedLongitude );
    printf( "[%d] = %g\n", extrapolatedIndex + 1, extrapolatedLatitude );
  }

  /* Last row, last column cell (extrapolated top-right corner): */

  printf( "\nextrapolated top-right corner:\n" );

  vIndex = rows * columnsPlus1Times2 - 4;

  {
    const int extrapolatedIndex       = vIndex + columnsPlus1Times2 + 2;
    const int dataIndex               = rows * columns - 1;
    const float longitude             = longitudes[ dataIndex ];
    const float latitude              = latitudes[ dataIndex ];
    const float diagonalLongitude     = vertices[ vIndex ];
    const float diagonalLatitude      = vertices[ vIndex + 1 ];
    const float longitudeDifference   = longitude - diagonalLongitude;
    const float latitudeDifference    = latitude  - diagonalLatitude;
    const float extrapolatedLongitude = longitude + longitudeDifference;
    const float extrapolatedLatitude  = latitude  + latitudeDifference;
    vertices[ extrapolatedIndex ]     = extrapolatedLongitude;
    vertices[ extrapolatedIndex + 1]  = extrapolatedLatitude;
    printf( "[%d] = %g, ", extrapolatedIndex, extrapolatedLongitude );
    printf( "[%d] = %g\n", extrapolatedIndex + 1, extrapolatedLatitude );
  }

  /* Clamp any out-of-range values: */

  printf( "\nclamping:\n" );

#pragma omp parallel for

  for ( vIndex = 0; vIndex < count; vIndex += 2 ) {
    const int vIndex1 = vIndex + 1;
    vertices[ vIndex  ] = CLAMPED_TO_RANGE( vertices[ vIndex ], -180.0, 180.0);
    vertices[ vIndex1 ] = CLAMPED_TO_RANGE( vertices[ vIndex1 ], -90.0, 90.0 );
    printf( "[%d] = %g, ", vIndex, vertices[ vIndex  ] );
    printf( "[%d] = %g\n", vIndex1, vertices[ vIndex1 ] );
  }

  printf( "\n" );

  POST02( is_valid_longitude_latitude( vertices[ 0 ], vertices[ 1 ] ),
          is_valid_longitude_latitude( vertices[ (rows+1) * (columns+1) - 2 ],
                                       vertices[ (rows+1) * (columns+1) - 1]));
}



/******************************************************************************
PURPOSE: computeCorners - Compute and store the corner variables
         (Longitude_SW, ... , Latitude_NE) for each center/pixel.
INPUTS:  Integer rows          Number of rows in swath.
         Integer columns       Number of columns in swath.
         const Real longitudes[ rows * columns ]  Longitudes of swath points.
         const Real latitudes[  rows * columns ]  Latitudes  of swath points.
OUTPUTS: Real corners[ 8 * rows * columns ]       longitudes_sw
                                                  longitudes_se
                                                  longitudes_nw
                                                  longitudes_ne
                                                  latitudes_sw
                                                  latitudes_se
                                                  latitudes_nw
                                                  latitudes_ne
NOTES:   Uses linear interpolation and extrapolation to the edges.
******************************************************************************/

static void computeCorners( Integer rows, Integer columns,
                            const Real longitudes[], const Real latitudes[],
                            Real corners[] ) {

  PRE07( rows > 0, columns > 0, rows * columns > 0, longitudes, latitudes,
         corners,
         validLongitudesAndLatitudes( rows * columns, longitudes, latitudes ));

  const Integer rows_1      = rows - 1;
  const Integer columns_1   = columns - 1;
  const Integer cells       = rows * columns;
  Real* const longitudes_sw = corners;
  Real* const longitudes_se = longitudes_sw + cells;
  Real* const longitudes_nw = longitudes_se + cells;
  Real* const longitudes_ne = longitudes_nw + cells;
  Real* const latitudes_sw  = longitudes_ne + cells;
  Real* const latitudes_se  = latitudes_sw + cells;
  Real* const latitudes_nw  = latitudes_se + cells;
  Real* const latitudes_ne  = latitudes_nw + cells;
  Integer cell = 0;
  Integer index = 0;

  if ( OR2( rows < 2, columns < 2 ) ) {

    /* Copy all center values to the corners in such degenerate cases: */

#pragma omp parallel for

    for ( cell = 0; cell < cells; ++cell ) {
      longitudes_sw[ cell ] =
      longitudes_se[ cell ] =
      longitudes_nw[ cell ] =
      longitudes_ne[ cell ] = longitudes[ cell ];
      latitudes_sw[ cell ] =
      latitudes_se[ cell ] =
      latitudes_nw[ cell ] =
      latitudes_ne[ cell ] = latitudes[ cell ];
    }

  } else { /* Linearly interpolate and extrapolate the corner points: */
    Integer row    = 0;
    Integer column = 0;

    /*
     * First compute linearly interpolated corners of all interior cells:
     * Note: rows increase north to south and columns increase west to east.
     */

    printf( "\ninterpolated interior corners:\n" );

#pragma omp parallel for private( column )

    for ( row = 0; row < rows_1; ++row ) {
      const Integer rowOffset     = row * columns;
      const Integer nextRowOffset = rowOffset + columns;

      /* Interior row, interior columns: */

      for ( column = 0; column < columns_1; ++column ) {
        const Integer thisIndex         = rowOffset + column;
        const Integer nextColumn        = thisIndex + 1;
        const Integer nextRow           = nextRowOffset + column;
        const Integer nextRowNextColumn = nextRow + 1;

        const Real longitude                  = longitudes[ thisIndex ];
        const Real nextColumnLongitude        = longitudes[ nextColumn ];
        const Real nextRowLongitude           = longitudes[ nextRow ];
        const Real nextRowNextColumnLongitude = longitudes[ nextRowNextColumn];

        const Real latitude                  = latitudes[ thisIndex ];
        const Real nextColumnLatitude        = latitudes[ nextColumn ];
        const Real nextRowLatitude           = latitudes[ nextRow ];
        const Real nextRowNextColumnLatitude = latitudes[ nextRowNextColumn ];

        const Real interpolatedLongitude = 0.25 *
          ( longitude + nextColumnLongitude + nextRowLongitude +
            nextRowNextColumnLongitude );

        const Real interpolatedLatitude = 0.25 *
          ( latitude + nextColumnLatitude + nextRowLatitude +
            nextRowNextColumnLatitude );

        longitudes_ne[ thisIndex         ] = interpolatedLongitude;
        longitudes_nw[ nextColumn        ] = interpolatedLongitude;
        longitudes_se[ nextRow           ] = interpolatedLongitude;
        longitudes_sw[ nextRowNextColumn ] = interpolatedLongitude;

        latitudes_ne[ thisIndex         ] = interpolatedLatitude;
        latitudes_nw[ nextColumn        ] = interpolatedLatitude;
        latitudes_se[ nextRow           ] = interpolatedLatitude;
        latitudes_sw[ nextRowNextColumn ] = interpolatedLatitude;

        printf( "lon_ne[%lld] = %lg\n", thisIndex, longitudes_ne[ thisIndex ]);
        printf( "lon_nw[%lld] = %lg\n", nextColumn, longitudes_nw[nextColumn]);
        printf( "lon_se[%lld] = %lg\n", nextRow, longitudes_se[ nextRow ] );
        printf( "lon_sw[%lld] = %lg\n", nextRowNextColumn,
                                        longitudes_sw[ nextRowNextColumn ] );
        printf( "lat_ne[%lld] = %lg\n", thisIndex, latitudes_ne[ thisIndex ] );
        printf( "lat_nw[%lld] = %lg\n", nextColumn, latitudes_nw[nextColumn]);
        printf( "lat_se[%lld] = %lg\n", nextRow, latitudes_se[ nextRow ] );
        printf( "lat_sw[%lld] = %lg\n", nextRowNextColumn,
                                        latitudes_sw[ nextRowNextColumn ] );
        printf( "\n" );

      } /* End loop on interior columns. */

    } /* End parallel loop on interior rows. */

    /* Serial region (not worth parallelizing): */

    /* Last row, interior columns (extrapolated top edge): */

    printf( "extrapolated top edge, except corners:\n" );

    for ( column = 1, index = rows_1 * columns + 1; column < columns;
          ++column, ++index ) {
      const Integer previousColumn = index - 1;

      const Real longitude = longitudes[ index ];
      const Real previousColumnLongitude = longitudes[ previousColumn ];
      const Real midpointLongitude =
        0.5 * ( longitude + previousColumnLongitude );
      const Real interpolatedLongitude = longitudes_sw[ index ];
      const Real longitudeDifference =
        midpointLongitude - interpolatedLongitude;
      const Real extrapolatedLongitude =
        midpointLongitude + longitudeDifference;

      const Real latitude = latitudes[ index ];
      const Real previousColumnLatitude = latitudes[ previousColumn ];
      const Real midpointLatitude = 0.5 * ( latitude + previousColumnLatitude);
      const Real interpolatedLatitude = latitudes_sw[ index ];
      const Real latitudeDifference = midpointLatitude - interpolatedLatitude;
      const Real extrapolatedLatitude = midpointLatitude + latitudeDifference;

      longitudes_nw[ index          ] = extrapolatedLongitude;
      longitudes_ne[ previousColumn ] = extrapolatedLongitude;

      latitudes_nw[ index          ] = extrapolatedLatitude;
      latitudes_ne[ previousColumn ] = extrapolatedLatitude;

      printf( "lon_nw[%lld] = %lg\n", index, longitudes_nw[ index ] );
      printf( "lon_ne[%lld] = %lg\n",
              previousColumn, longitudes_ne[previousColumn ] );
      printf( "lat_nw[%lld] = %lg\n", index, latitudes_nw[ index ] );
      printf( "lat_ne[%lld] = %lg\n",
              previousColumn, latitudes_ne[previousColumn ] );
      printf( "\n" );
    }

    /* First row, interior columns (extrapolated top edge): */

    printf( "extrapolated bottom edge, except corners:\n" );

    for ( column = 1, index = 1; column < columns; ++column, ++index ) {
      const Integer previousColumn = index - 1;

      const Real longitude = longitudes[ index ];
      const Real previousColumnLongitude = longitudes[ previousColumn ];
      const Real midpointLongitude =
        0.5 * ( longitude + previousColumnLongitude );
      const Real interpolatedLongitude = longitudes_nw[ index ];
      const Real longitudeDifference =
        midpointLongitude - interpolatedLongitude;
      const Real extrapolatedLongitude =
        midpointLongitude + longitudeDifference;

      const Real latitude = latitudes[ index ];
      const Real previousColumnLatitude = latitudes[ previousColumn ];
      const Real midpointLatitude = 0.5 * ( latitude + previousColumnLatitude);
      const Real interpolatedLatitude = latitudes_nw[ index ];
      const Real latitudeDifference = midpointLatitude - interpolatedLatitude;
      const Real extrapolatedLatitude = midpointLatitude + latitudeDifference;

      longitudes_sw[ index          ] = extrapolatedLongitude;
      longitudes_se[ previousColumn ] = extrapolatedLongitude;

      latitudes_sw[ index          ] = extrapolatedLatitude;
      latitudes_se[ previousColumn ] = extrapolatedLatitude;

      printf( "lon_nw[%lld] = %lg\n", index, longitudes_sw[ index ] );
      printf( "lon_ne[%lld] = %lg\n",
              previousColumn, longitudes_se[previousColumn ] );
      printf( "lat_nw[%lld] = %lg\n", index, latitudes_sw[ index ] );
      printf( "lat_ne[%lld] = %lg\n",
              previousColumn, latitudes_se[previousColumn ] );
      printf( "\n" );
    }

    /* First column, interior rows (extrapolated left edge, except corners): */

    printf( "extrapolated left edge, except corners:\n" );

    for ( row = 1, index = columns; row < rows; ++row, index += columns ) {
      const Integer previousRow = index - columns;

      const Real longitude = longitudes[ index ];
      const Real previousRowLongitude = longitudes[ previousRow ];
      const Real midpointLongitude =
        0.5 * ( longitude + previousRowLongitude );
      const Real interpolatedLongitude = longitudes_se[ index ];
      const Real longitudeDifference =
        midpointLongitude - interpolatedLongitude;
      const Real extrapolatedLongitude =
        midpointLongitude + longitudeDifference;

      const Real latitude = latitudes[ index ];
      const Real previousRowLatitude = latitudes[ previousRow ];
      const Real midpointLatitude = 0.5 * ( latitude + previousRowLatitude );
      const Real interpolatedLatitude = latitudes_se[ index ];
      const Real latitudeDifference = midpointLatitude - interpolatedLatitude;
      const Real extrapolatedLatitude = midpointLatitude + latitudeDifference;

      longitudes_sw[ index       ] = extrapolatedLongitude;
      longitudes_nw[ previousRow ] = extrapolatedLongitude;

      latitudes_sw[ index       ] = extrapolatedLatitude;
      latitudes_nw[ previousRow ] = extrapolatedLatitude;

      printf( "lon_nw[%lld] = %lg\n", index, longitudes_sw[ index ] );
      printf( "lon_ne[%lld] = %lg\n", previousRow, longitudes_nw[previousRow]);
      printf( "lat_nw[%lld] = %lg\n", index, latitudes_sw[ index ] );
      printf( "lat_ne[%lld] = %lg\n", previousRow, latitudes_nw[previousRow ]);
      printf( "\n" );
    }

    /* Last column, interior rows (extrapolated right edge, except corners): */

    printf( "extrapolated right edge, except corners:\n" );

    for ( row = 1, index = columns + columns - 1;
          row < rows; ++row, index += columns ) {
      const Integer previousRow = index - columns;

      const Real longitude = longitudes[ index ];
      const Real previousRowLongitude = longitudes[ previousRow ];
      const Real midpointLongitude =
        0.5 * ( longitude + previousRowLongitude );
      const Real interpolatedLongitude = longitudes_sw[ index ];
      const Real longitudeDifference =
        midpointLongitude - interpolatedLongitude;
      const Real extrapolatedLongitude =
        midpointLongitude + longitudeDifference;

      const Real latitude = latitudes[ index ];
      const Real previousRowLatitude = latitudes[ previousRow ];
      const Real midpointLatitude = 0.5 * ( latitude + previousRowLatitude );
      const Real interpolatedLatitude = latitudes_sw[ index ];
      const Real latitudeDifference = midpointLatitude - interpolatedLatitude;
      const Real extrapolatedLatitude = midpointLatitude + latitudeDifference;

      longitudes_se[ index       ] = extrapolatedLongitude;
      longitudes_ne[ previousRow ] = extrapolatedLongitude;

      latitudes_se[ index       ] = extrapolatedLatitude;
      latitudes_ne[ previousRow ] = extrapolatedLatitude;

      printf( "lon_nw[%lld] = %lg\n", index, longitudes_se[ index ] );
      printf( "lon_ne[%lld] = %lg\n", previousRow, longitudes_ne[previousRow]);
      printf( "lat_nw[%lld] = %lg\n", index, latitudes_se[ index ] );
      printf( "lat_ne[%lld] = %lg\n", previousRow, latitudes_ne[previousRow ]);
      printf( "\n" );
    }

    /* First row, first column cell (extrapolated bottom-left corner): */

    printf( "extrapolated bottom-left corner:\n" );

    {
      const Real longitude             = longitudes[ 0 ];
      const Real latitude              = latitudes[ 0 ];
      const Real diagonalLongitude     = longitudes_ne[ 0 ];
      const Real diagonalLatitude      = latitudes_ne[ 0 ];
      const Real longitudeDifference   = longitude - diagonalLongitude;
      const Real latitudeDifference    = latitude  - diagonalLatitude;
      const Real extrapolatedLongitude = longitude + longitudeDifference;
      const Real extrapolatedLatitude  = latitude  + latitudeDifference;
      longitudes_sw[ 0 ]                = extrapolatedLongitude;
      latitudes_sw[  0 ]                = extrapolatedLatitude;
      printf( "lon_sw[0] = %g, ", longitudes_sw[ 0 ] );
      printf( "lat_sw[0] = %g\n", latitudes_sw[  0 ] );
      printf( "\n" );
    }

    /* First row, last column cell (extrapolated bottom-right corner): */

    printf( "extrapolated bottom-right corner:\n" );

    {
      const Real longitude             = longitudes[ columns_1 ];
      const Real latitude              = latitudes[ columns_1 ];
      const Real diagonalLongitude     = longitudes_nw[ columns_1 ];
      const Real diagonalLatitude      = latitudes_nw[ columns_1 ];
      const Real longitudeDifference   = longitude - diagonalLongitude;
      const Real latitudeDifference    = latitude  - diagonalLatitude;
      const Real extrapolatedLongitude = longitude + longitudeDifference;
      const Real extrapolatedLatitude  = latitude  + latitudeDifference;
      longitudes_se[ columns_1 ]        = extrapolatedLongitude;
      latitudes_se[  columns_1 ]        = extrapolatedLatitude;
      printf( "lon_se[%lld] = %g, ", columns_1, longitudes_se[ columns_1 ] );
      printf( "lat_se[%lld] = %g\n", columns_1, latitudes_se[  columns_1 ] );
      printf( "\n" );
    }

    /* Last row, first column cell (extrapolated top-left corner): */

    printf( "extrapolated top-left corner:\n" );

    index = cells - columns;

    {
      const Real longitude             = longitudes[ index ];
      const Real latitude              = latitudes[ index ];
      const Real diagonalLongitude     = longitudes_se[ index ];
      const Real diagonalLatitude      = latitudes_se[ index ];
      const Real longitudeDifference   = longitude - diagonalLongitude;
      const Real latitudeDifference    = latitude  - diagonalLatitude;
      const Real extrapolatedLongitude = longitude + longitudeDifference;
      const Real extrapolatedLatitude  = latitude  + latitudeDifference;
      longitudes_nw[ index ]            = extrapolatedLongitude;
      latitudes_nw[  index ]            = extrapolatedLatitude;
      printf( "lon_nw[%lld] = %g, ", index, longitudes_nw[ index ] );
      printf( "lat_nw[%lld] = %g\n", index, latitudes_nw[  index ] );
      printf( "\n" );
    }

    /* Last row, last column cell (extrapolated top-right corner): */

    printf( "extrapolated top-right corner:\n" );

    index = cells - 1;

    {
      const Real longitude             = longitudes[ index ];
      const Real latitude              = latitudes[ index ];
      const Real diagonalLongitude     = longitudes_sw[ index ];
      const Real diagonalLatitude      = latitudes_sw[ index ];
      const Real longitudeDifference   = longitude - diagonalLongitude;
      const Real latitudeDifference    = latitude  - diagonalLatitude;
      const Real extrapolatedLongitude = longitude + longitudeDifference;
      const Real extrapolatedLatitude  = latitude  + latitudeDifference;
      longitudes_ne[ index ]            = extrapolatedLongitude;
      latitudes_ne[  index ]            = extrapolatedLatitude;
      printf( "lon_ne[%lld] = %g, ", index, longitudes_ne[ index ] );
      printf( "lat_ne[%lld] = %g\n", index, latitudes_ne[  index ] );
      printf( "\n" );
    }

    /* Clamp any out-of-range values: */

    printf( "clamping:\n" );

#pragma omp parallel for

    for ( cell = 0; cell < cells; ++cell ) {
      longitudes_nw[cell] = CLAMPED_TO_RANGE(longitudes_nw[cell],-180.0,180.0);
      longitudes_sw[cell] = CLAMPED_TO_RANGE(longitudes_sw[cell],-180.0,180.0);
      longitudes_se[cell] = CLAMPED_TO_RANGE(longitudes_se[cell],-180.0,180.0);
      longitudes_ne[cell] = CLAMPED_TO_RANGE(longitudes_ne[cell],-180.0,180.0);
      latitudes_nw[cell] = CLAMPED_TO_RANGE( latitudes_nw[cell], -90.0, 90.0 );
      latitudes_sw[cell] = CLAMPED_TO_RANGE( latitudes_sw[cell], -90.0, 90.0 );
      latitudes_se[cell] = CLAMPED_TO_RANGE( latitudes_se[cell], -90.0, 90.0 );
      latitudes_ne[cell] = CLAMPED_TO_RANGE( latitudes_ne[cell], -90.0, 90.0 );

      printf( "lon_sw[%lld] = %lg, ", cell, longitudes_sw[ cell  ] );
      printf( "lat_sw[%lld] = %lg, ", cell, latitudes_sw[ cell  ] );
      printf( "lon_se[%lld] = %lg, ", cell, longitudes_se[ cell  ] );
      printf( "lat_se[%lld] = %lg, ", cell, latitudes_se[ cell  ] );
      printf( "lon_nw[%lld] = %lg, ", cell, longitudes_nw[ cell  ] );
      printf( "lat_nw[%lld] = %lg, ", cell, latitudes_nw[ cell  ] );
      printf( "lon_ne[%lld] = %lg, ", cell, longitudes_ne[ cell  ] );
      printf( "lat_ne[%lld] = %lg, ", cell, latitudes_ne[ cell  ] );
      printf( "\n" );
    }

  } /* End else non-degenerate cases: */

  POST0( validLongitudesAndLatitudes( 4 * rows * columns,
                                      corners, corners + 4 * rows * columns ));
}



