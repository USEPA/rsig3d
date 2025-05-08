
/******************************************************************************
PURPOSE: Shapefile.c - Write time-varying scalar grid and point data to ESRI
         Shapefiles (shp, shx, dbf) and ASCIIGrid files (asc, prj).
NOTES:   Uses Shapefile and GPC free, open-source libraries.
         http://shapelib.maptools.org/shp_api.html
         http://www.cs.man.ac.uk/~toby/alan/software/gpc.html
         http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf
         http://en.wikipedia.org/wiki/ESRI_grid
         http://en.wikipedia.org/wiki/Well-known_text
         When adding new shapefile files, edit table[] and writePolygonDBF().
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h>  /* For FILE, stderr, fprintf(), fopen(), perror(). */
#include <string.h> /* For strncpy(), memset(), strdup(). */
#include <ctype.h>  /* For tolower(), isprint(), isspace(). */
#include <math.h>   /* For sqrt(), fabs(). */
#include <limits.h> /* For INT_MAX. */
#include <stdlib.h> /* For malloc(), free(). */
#include <unistd.h> /* For unlink(). */

#include <Assertions.h> /* For PRE*(), POST*(), DEBUG(), IMPLIES_ELSE(). */
#include <DateTime.h>   /* For isValidYearMonthDay(). */
#include <Utilities.h>  /* For isValidLongitudeLatitude, copyFile(). */
#include <MapFile.h>    /* For subsetMapDouble(). */
#include <Shapefile.h>  /* For public interface. */

/*================================== TYPES ==================================*/


static const int includeMissingValuesInCSVFile = 0; /* Write missing values? */

enum { BIG = 4321, LITTLE = 1234 };
enum { MAXIMUM_FILE_NAME_LENGTH = 255 };


/*
 * The following table defines the translation of input DBF columns to
 * output DBF columns.
 * This is used by the ShapeSubset program when it reads PI-supplied DBF files
 * (stored on rtpmeta.epa.gov) and, after spatial subsetting, creates
 * subsetted SHP and DBF files for streaming back to users.
 */

#define ACRES_TO_HECTARES 0.404685642
#define FEET_TO_METERS 0.3048
#define CUBIC_FEET_TO_CUBIC_METERS \
  ( FEET_TO_METERS * FEET_TO_METERS * FEET_TO_METERS )

/* Define columns of output DBF file: */

typedef struct {
  const char* fileName;   /* Unique part of input file name "temperature_". */
  int inputColumn;        /* 0-based column number of original input DBF. */
  const char* columnName; /* E.g., "TEMP_C". */
  int columnType;         /* FTString, FTInteger, FTDouble. */
  int fieldWidth;         /* E.g., 7 characters wide. */
  int decimals;           /* E.g., 3 digits to right of the decimal. */
  double offset;          /* Offset to map input to output. -32.0. */
  double scale;           /* Scale  to map input to output. 5.0/9.0. */
} ColumnEntry;

static const ColumnEntry table[] = {

  /* HMS Smoke files used by RSIG: */

  { "hms_smoke", -1, "YYYYDDD1",  FTInteger, 7, 0, 0.0, 1.0 },
  { "hms_smoke", -1, "HHMM1",     FTInteger, 4, 0, 0.0, 1.0 },
  { "hms_smoke", -1, "YYYYDDD2",  FTInteger, 7, 0, 0.0, 1.0 },
  { "hms_smoke", -1, "HHMM2",     FTInteger, 4, 0, 0.0, 1.0 },
  { "hms_smoke",  3, "DENS_UGM3", FTInteger, 2, 0, 0.0, 1.0 },

  /* End of table: */

  { 0, 0, 0, 0, 0, 0, 0.0, 0.0 } /* End of table. */
};


/******************************************************************************
PURPOSE: defineDBFColumns - Define columns of an output DBF file.
INPUTS:  const char* inputFileName  File name of input dbf.
         const int defineColumns    If 1 then define columns else just compute
                                    tableIndex and column count.
OUTPUTS: int* tableIndex            Index into table of first column.
         int* longitudeColumn       Index of column LONGITUDE, else -1 if none.
         int* latitudeColumn        Index of column LATITUDE,  else -1 if none.
         int* hucIdColumn           Index of column HUC_ID,    else -1 if none.
         DBFHandle outputFile       DBF file to write to.
RETURNS: int number of columns defined if successful,
         else 0 and a failure message is printed ro stderr.
******************************************************************************/

static int defineDBFColumns( const char* const inputFileName,
                             const int defineColumns,
                             int* const tableIndex,
                             int* const longitudeColumn,
                             int* const latitudeColumn,
                             int* const hucIdColumn,
                             DBFHandle outputFile ) {
  PRE05( inputFileName, *inputFileName, IS_BOOL( defineColumns ), tableIndex,
         outputFile );
  int result = 0;
  int ok = 0;
  int entry = 0;
  char lowercaseInputFileName[ 256 ] = "";
  memset( lowercaseInputFileName, 0, sizeof lowercaseInputFileName );
  strncpy( lowercaseInputFileName, inputFileName,
           sizeof lowercaseInputFileName /
           sizeof *lowercaseInputFileName - 1 );
  lowercase( lowercaseInputFileName );
  *tableIndex = -1;

  for ( entry = 0, ok = 1; AND2( ok, table[ entry ].fileName ); ++entry ) {
    const ColumnEntry* const columnEntry = table + entry;
    const int matchedColumn =
      strstr( lowercaseInputFileName, columnEntry->fileName ) != 0;

    if ( matchedColumn ) {
      ++result;

      if ( *tableIndex == -1 ) {
        *tableIndex = entry;
      }

      if ( longitudeColumn ) {

        if ( ! strcmp( columnEntry->columnName, "LONGITUDE" ) ) {
          *longitudeColumn = columnEntry->inputColumn;
        }
      }

      if ( latitudeColumn ) {

        if ( ! strcmp( columnEntry->columnName, "LATITUDE" ) ) {
          *latitudeColumn = columnEntry->inputColumn;
        }
      }

      if ( hucIdColumn ) {

        if ( ! strcmp( columnEntry->columnName, "HUC_ID" ) ) {
          *hucIdColumn = columnEntry->inputColumn;
        }
      }

      if ( defineColumns ) {
        ok = DBFAddField( outputFile,
                          columnEntry->columnName,
                          columnEntry->columnType,
                          columnEntry->fieldWidth,
                          columnEntry->decimals ) != -1;
      }
    }
  }

  ok = AND3( ok,
             IN_RANGE( result,      1, sizeof table / sizeof *table - 1 ),
             IN_RANGE( *tableIndex, 0, sizeof table / sizeof *table - 2 ) );

  if ( ! ok ) {
    fprintf( stderr, "\nFailed to define DBF columns for file %s.\n",
             inputFileName );
    result = 0;
    *tableIndex = -1;

    if ( longitudeColumn ) {
      *longitudeColumn = -1;
    }

    if ( latitudeColumn ) {
      *latitudeColumn = -1;
    }

    if ( hucIdColumn ) {
      *hucIdColumn = -1;
    }
  }

  POST0( OR2( AND5( result == 0,
                    *tableIndex == -1,
                    IMPLIES( longitudeColumn, *longitudeColumn == -1 ),
                    IMPLIES( latitudeColumn,  *latitudeColumn  == -1 ),
                    IMPLIES( hucIdColumn,     *hucIdColumn     == -1 ) ),
              AND2( result > 0,
                    *tableIndex >= 0 ) ) );
                        
  return result;
}



/*=========================== FORWARD DECLARATIONS ==========================*/

static int writePRJFile( const char* fileName, const int useASCIIGridForm );

static int writeDataToDBFFile( const char* fileName, const char* variable,
                               const char* units,
                               const int timesteps, const int yyyymmddhh[],
                               const int timestepType,
                               const int count, const int type,
                               const int components,
                               const void* data,
                               const float lonlats[], const float z[],
                               const char* const sids[] );

static int writeDataToCSVFile( const char* fileName, const char* variable,
                               const char* units,
                               const int timesteps, const int yyyymmddhh[],
                               const int timestepType,
                               const int count, const int type,
                               const int components,
                               const void* data,
                               const float lonlats[], const float z[],
                               const char* const sids[] );

static const char* storeStringValue( const char* value, ShapeData* shapeData );

static int copyStringAttribute( DBFHandle inputFile,
                                int inputRow, int inputColumn,
                                DBFHandle outputFile,
                                int outputRow, int outputColumn );

static int copyIntegerAttribute( DBFHandle inputFile,
                                int inputRow, int inputColumn,
                                DBFHandle outputFile,
                                int outputRow, int outputColumn );

static int copyDoubleAttribute( DBFHandle inputFile,
                                int inputRow, int inputColumn,
                                DBFHandle outputFile,
                                int outputRow, int outputColumn,
                                int invalidIfNegative, double missing,
                                double offset, double scale );

static int computeSparsedPartCount( const SHPObject* shape,
                                    const double minimumAdjacentVertexDistance,
                                    const int minimumSparsedVertices );

static int computeSparsedVertexCount( const int vertexCount,
                                      const double x[],
                                      const double y[],
                                      const double minimumAdjacentVertexDistance,
                                      const int isPolygon );

static void copySparseVertices( const int vertexCount,
                                const int sparsedVertices,
                                const double x[],
                                const double y[],
                                const double minimumAdjacentVertexDistance,
                                const int isPolygon,
                                int* const initializedBounds,
                                Bounds bounds,
                                gpc_vertex* const vertices );

static void computeGridBounds( const int rows,
                               const int columns,
                               const double westEdge,
                               const double southEdge,
                               const double cellWidth,
                               const double cellHeight,
                               Unproject unproject,
                               double range[ 4 ] );

static void updatePointMinmax( Unproject unproject,
                               const double x,
                               const double y,
                               double* minimumX,
                               double* maximumX,
                               double* minimumY,
                               double* maximumY );

static void computeGridCellCenters( const int rows,
                                    const int columns,
                                    const double westEdge,
                                    const double southEdge,
                                    const double cellWidth,
                                    const double cellHeight,
                                    Unproject unproject,
                                    float lonlats[] );

static void computePointBounds( const int points,
                                const float lonlats[],
                                double range[ 4 ] );

static void computeVertexBounds( const int count, const float xy[],
                                 double xyRange[] );

static void computeRange( const double array[], const int count,
                          const int stride, double range[ 2 ] );

static int convertTimestamp( const char* inputFileName,
                             const char* timestamp,
                             int* const yyyyddd, int* const hhmm );

static int writeShort( unsigned char bytes[], const int index,
                       const short value, const int endian );

static int writeInt( unsigned char bytes[], const int index,
                     const int value, const int endian );

static int writeDouble( unsigned char bytes[], const int index,
                        const double value, const int endian );

static int minimumInt( int count, const int array[] );

static int maximumInt( int count, const int array[] );

/*============================= PUBLIC FUNCTIONS ============================*/



/******************************************************************************
PURPOSE: writeASCIIGridFile - Write a single timestep-layer of grid cell scalar
         data to an ESRI ASCII Grid file.
INPUTS:  const char* const fileName  File to create. E.g.,"example.asc".
         int rows              Number of grid rows.
         int columns           Number of grid columns.
         double westEdge       Distance from origin to west edge of grid.
         double southEdge      Distance from origin to south edge of grid.
         double cellSize       Width/height of each grid cell.
         int type              Grid cell scalar data type: BYTE_TYPE, etc.
         const void* data      data[ rows * columns ] Scalar at cell centers.
NOTES:   Creates file fileName.
         http://en.wikipedia.org/wiki/ESRI_grid
******************************************************************************/

int writeASCIIGridFile( const char* fileName, int rows, int columns,
                        double westEdge, double southEdge, double cellSize,
                        int type, const void* data ) {
  PRE012( fileName,
          *fileName,
          rows > 0,
          columns > 0,
          rows < INT_MAX / columns,
          ! isNan( westEdge ),
          ! isNan( southEdge ),
          ! isNan( cellSize ),
          cellSize > 0.0,
          IS_VALID_GRID_DATA_TYPE( type ),
          data,
          IMPLIES( type == FLOAT_TYPE,
                   AND2( ! isNan( ((const float*) data)[ 0 ] ),
                         ! isNan(((const float*)data)[rows * columns - 1]))));
  int result = 0;
  FILE* file = fopen( fileName, "w" );

  if ( file ) {
    const size_t last = ( rows - 1 ) * columns;
    const float* const fdata = data;
    const char* const cdata = data;
    const unsigned short* const sdata = data;
    const float* fDataRow = fdata + last;
    const char*  cDataRow = cdata + last;
    const unsigned short* sDataRow = sdata + last;
    const char* const noDataValue = type == UINT16_TYPE ? "0" : "-9999";
    int row = 0;
    result =
      fprintf( file, "ncols %d\nnrows %d\nxllcorner %lg\nyllcorner %lg\n"
                     "cellsize %lg\nNODATA_value %s\n",
               columns, rows, westEdge, southEdge, cellSize, noDataValue ) > 0;

    for ( row = rows - 1; AND2( result, row >= 0 ); --row,
          fDataRow -= columns, cDataRow -= columns, sDataRow -= columns ) {
      int column = 0;

      for ( column = 0; AND2( result, column < columns ); ++column ) {
        const float dataValue =
          type == FLOAT_TYPE ? fDataRow[ column ]
          : type == BYTE_TYPE ? (float) ( cDataRow[ column ] )
          : (float) ( sDataRow[ column ] );
        const float clampedValue = dataValue > -9999.0 ? dataValue : -9999.0;
        result = fprintf( file, "%g ", clampedValue ) > 0;
      }

      result = AND2( result, fprintf( file, "\n" ) > 0 );
    }

    fclose( file ), file = 0;
  }

  if ( ! result ) {
    perror( "\n\nFailed because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: writeWGS84PRJFile - Write a WGS84 ESRI projection file.
INPUTS:  const char* const fileName  File to create. E.g.,"example.prj".
         const int         useASCIIGridForm  1 = use grid form, 0 = shape form.
NOTES:   Creates file fileName.
         http://en.wikipedia.org/wiki/Well-known_text
******************************************************************************/

int writeWGS84PRJFile( const char* fileName, const int useASCIIGridForm ) {
  PRE03( fileName, *fileName, IS_BOOL( useASCIIGridForm ) );
  const char* const content =
    useASCIIGridForm ?
      "Projection    GEOGRAPHIC\n"
      "Datum         NAD83\n"
      "Spheroid      GRS80\n"
      "Units         DD\n"
      "Zunits        NO\n"
    :
      "GEOGCS[\"GCS_North_American_1983\","
      "DATUM[\"D_North_American_1983\","
      "SPHEROID[\"GRS_1980\",6378137.0,298.257223563]],"
      "PRIMEM[\"Greenwich\",0.0],UNIT[\"Degree\",0.0174532925199433]]";
  int result = 0;
  FILE* file = fopen( fileName, "w" );

  if ( file ) {
    result = fprintf( file, "%s", content ) == strlen( content );
    fclose( file ), file = 0;
  }

  if ( ! result ) {
    perror( "\n\nFailed because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: writeLambertPRJFile - Write a Lambert ESRI projection file.
INPUTS:  const char* const fileName  File to create. E.g.,"example.prj".
         double centralLongitude     Longitude of center of projection. -90.0.
         double centralLatitude      Latitude of center of projection. 40.0.
         double lowerLatitude        Latitude of lower tangent. 30.0.
         double upperLatitude        Latitude of upper tangent. 60.0.
         const int useASCIIGridForm  1 = use grid form, 0 = shape form.
NOTES:   Creates file fileName. Uses MM5 sphere of radius 6,370,000m.
         http://en.wikipedia.org/wiki/Well-known_text
******************************************************************************/

int writeLambertPRJFile( const char* fileName,
                         const double centralLongitude,
                         const double centralLatitude,
                         const double lowerLatitude,
                         const double upperLatitude,
                         const int useASCIIGridForm ) {
  PRE011( fileName,
          *fileName,
          isValidLongitudeLatitude( centralLongitude, centralLatitude ),
          IN_RANGE( centralLatitude, -89.0, 89.0 ),
          IN_RANGE( lowerLatitude,   -90.0, 90.0 ),
          IN_RANGE( upperLatitude,   -90.0, 90.0 ),
          lowerLatitude <= upperLatitude,
          SIGN( lowerLatitude ) == SIGN( upperLatitude ),
          IMPLIES_ELSE( lowerLatitude >= 0.0,
                        IN_RANGE( lowerLatitude, 1.0, 89.0 ),
                        IN_RANGE( lowerLatitude, -89.0, -1.0 ) ),
          IMPLIES_ELSE( upperLatitude >= 0.0,
                        IN_RANGE( upperLatitude, 1.0, 89.0 ),
                        IN_RANGE( upperLatitude, -89.0, -1.0 ) ),
          IS_BOOL( useASCIIGridForm ) );
  const char* const content =
    useASCIIGridForm ?
      "Projection    Lambert Conformal Conic\n"
      "Datum         NAD83\n"
      "Spheroid      GRS80\n"
      "Units         METERS\n"
      "Zunits        NO\n"
      "Xshift        0.0\n"
      "Yshift        0.0\n"
      "Parameters\n"
      "%lg /* 1st standard parallel */\n"
      "%lg /* 2nd standard parallel */\n"
      "%lg /* central meridian */\n"
      "%lg /* latitude of projection's origin */\n"
      "0.0 /* false easting (meters) */\n"
      "0.0 /* false northing (meters) */\n"
    :
      "PROJCS[\"Lambert Conformal Conic\","
      "GEOGCS[\"GCS_Sphere_EMEP\","
      "DATUM[\"D_Sphere_EMEP\","
      "SPHEROID[\"Sphere_EMEP\",6370000.0,0.0]],"
      "PRIMEM[\"Greenwich\",0.0],"
      "UNIT[\"Degree\",0.0174532925199433]],"
      "PROJECTION[\"Lambert_Conformal_Conic\"],"
      "PARAMETER[\"Standard_Parallel_1\",%lg],"
      "PARAMETER[\"Standard_Parallel_2\",%lg],"
      "PARAMETER[\"Latitude_Of_Origin\",%lg],"
      "PARAMETER[\"Central_Meridian\",%lg],"
      "PARAMETER[\"False_Easting\",0.0],"
      "PARAMETER[\"False_Northing\",0.0],"
      "UNIT[\"Meter\",1]]";
  int result = 0;
  FILE* file = fopen( fileName, "w" );

  if ( file ) {
    result = fprintf( file, content,
                      lowerLatitude, upperLatitude,
                      centralLatitude, centralLongitude )
             > strlen( content ) - 26;
    fclose( file ), file = 0;
  }

  if ( ! result ) {
    perror( "\n\nFailed because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: writeGridToShapefile - Write a single layer of grid cells as a lon-lat
         Shapefile Polygon file set (shp, shx, dbf, prj) and a csv file
         containing time-varying data.
INPUTS:  const char* const fileName  Base name of file to create. "example".
         const int timesteps         Number of timesteps of data.
         const int yyyymmddhh[ timesteps ]  Timestamps of data.
         const int timestepType      HOURLY, DAILY, MONTHLY, YEARLY.
         const int rows              Number of grid rows.
         const int columns           Number of grid columns.
         const double westEdge       Distance from origin to west edge of grid.
         const double southEdge      Distance from origin to south edge of ".
         const double cellWidth      Width of each grid cell (e.g., 12000 m).
         const double cellWHeight    Height of each grid cell (e.g., 12000 m).
         const char* variable        Name of data variable.
         const char* units           Name of data units.
         int type                    Grid cell scalar data type: BYTE_TYPE...
         const void* data            data[ timesteps * rows * columns ]
                                     Scalar data at grid cell centers.
         Unproject unproject         Function to unproject (x,y) to (lon,lat).
RETURNS: int 1 if successful, else 0 and failure message is printed to stderr.
NOTES:   Creates files fileName.shp and fileName.shx
         then calls routines that create fileName.dbf and fileName.csv.
         See 1998 ESRI Shapefile Specification pages 2, 4, 5, 16, 23, 24.
         http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf
         This routine does not use shapelib API since it was written before
         that library was used in this project and also because this routine
         is more straight-forward and much faster than using the shapelib API.
******************************************************************************/

int writeGridToShapefile( const char* fileName,
                          const int timesteps,
                          const int yyyymmddhh[],
                          const int timestepType,
                          const int rows,
                          const int columns,
                          const double westEdge,
                          const double southEdge,
                          const double cellWidth,
                          const double cellHeight,
                          const char* variable,
                          const char* units,
                          int type,
                          int components,
                          const void* data,
                          Unproject unproject ) {

  PRE016( fileName,
          *fileName,
          timesteps > 0,
          yyyymmddhh,
          IS_VALID_TIMESTEP_TYPE( timestepType ),
          rows > 0,
          columns > 0,
          rows * columns > 0,
          variable,
          *variable,
          units,
          *units,
          IS_VALID_GRID_DATA_TYPE( type ),
          data,
          IMPLIES( type == FLOAT_TYPE,
                   AND2( ! isNan( ((const float*) data)[ 0 ] ),
             ! isNan(((const float*)data)[timesteps * rows * columns - 1]))),
          IMPLIES( unproject == 0,
                   AND2 ( isValidLongitudeLatitude( westEdge, southEdge ),
                          isValidLongitudeLatitude(
                                      westEdge  + columns * cellWidth,
                                      southEdge + rows    * cellHeight ) ) ) );

  int result = 0;
  enum { /* Constants from the Shapefile Spec (see above link): */
    BYTES_PER_INT = 4,
    BYTES_PER_DOUBLE = 8,
    POLYGON = 5,                 /* ShapeType = POLYGON. */
    PARTS_PER_POLYGON = 1,
    VERTICES_PER_POLYGON = 5,    /* Quads with redundant last=first vertex.*/
    HEADER_BYTES = 100,
    RECORD_HEADER_BYTES = 8,
    RECORD_CONTENT_BYTES =
      1 * BYTES_PER_INT +        /* int ShapeType = POLYGON */
      4 * BYTES_PER_DOUBLE +     /* double Box[ 4 ] = xMin,yMin,xMax,yMax */
      1 * BYTES_PER_INT +        /* int NumParts = 1 */
      1 * BYTES_PER_INT +        /* int NumPoints = 5 */
      1 * BYTES_PER_INT +        /* int Parts[ NumParts = 1 ] = 0 */
      VERTICES_PER_POLYGON * 2 * BYTES_PER_DOUBLE /* double [NumPoints*2]. */
  };
  unsigned char header[ HEADER_BYTES ] = "";
  unsigned char recordHeader[ RECORD_HEADER_BYTES ] = "";
  unsigned char recordContents[ RECORD_CONTENT_BYTES ] = "";
  const int records = rows * columns;
  const int shxFileBytes = HEADER_BYTES + records * RECORD_HEADER_BYTES;
  const int shpFileBytes = shxFileBytes + records * RECORD_CONTENT_BYTES;
  int byteIndex = 0;
  double xyRange[ 2 * 2 ] = { 0.0, 0.0, 0.0, 0.0 };
  enum { FILE_NAME_LENGTH = 255 };
  char shxFileName[ FILE_NAME_LENGTH + 1 ] = "";
  char shpFileName[ FILE_NAME_LENGTH + 1 ] = "";
  FILE* file = 0;
  float* lonlats = NEW( float, rows * columns * 2 ); /* Of grid cell centers.*/

  if ( lonlats ) {

    /* Construct file names of resulting files from base file name: */

    memset( shxFileName, 0, sizeof shxFileName );
    memset( shpFileName, 0, sizeof shpFileName );
    strncpy( shxFileName, fileName, FILE_NAME_LENGTH - 8 );
    strncpy( shpFileName, fileName, FILE_NAME_LENGTH - 8 );
    strcat( shxFileName, ".shx" );
    strcat( shpFileName, ".shp" );

    computeGridBounds( rows, columns, westEdge, southEdge,
                       cellWidth, cellHeight, unproject, xyRange );

    computeGridCellCenters( rows, columns, westEdge, southEdge,
                            cellWidth, cellHeight, unproject, lonlats );

    /* Initialize shx file header and records: */

    memset( header, 0, sizeof header );
    memset( recordHeader, 0, sizeof recordHeader );
    memset( recordContents, 0, sizeof recordContents );

    writeInt( header, 0, 9994, BIG );
    byteIndex = writeInt( header, 24, shxFileBytes / 2, BIG );
    byteIndex = writeInt( header, byteIndex, 1000, LITTLE );
    byteIndex = writeInt( header, byteIndex, POLYGON, LITTLE );
    byteIndex = writeDouble( header, byteIndex, xyRange[ 0 ], LITTLE );
    byteIndex = writeDouble( header, byteIndex, xyRange[ 1 ], LITTLE );
    byteIndex = writeDouble( header, byteIndex, xyRange[ 2 ], LITTLE );
    byteIndex = writeDouble( header, byteIndex, xyRange[ 3 ], LITTLE );

    writeInt( recordHeader, 0, HEADER_BYTES / 2, BIG );
    writeInt( recordHeader, 4, RECORD_CONTENT_BYTES / 2, BIG );

    writeInt( recordContents, 0, POLYGON, LITTLE );
    writeInt( recordContents, 36, PARTS_PER_POLYGON, LITTLE );
    writeInt( recordContents, 40, VERTICES_PER_POLYGON, LITTLE );

    /* Write shx file: */

    file = fopen( shxFileName, "wb" );

    if ( file ) {
      int record = 0;
      result = fwrite( header, HEADER_BYTES, 1, file ) == 1;

      for ( record = 0; result && record < records; ++record ) {
        const int offsetBytes =
          HEADER_BYTES +
          record * ( RECORD_HEADER_BYTES + RECORD_CONTENT_BYTES );
        writeInt( recordHeader, 0, offsetBytes / 2, BIG );
        result = fwrite( recordHeader, RECORD_HEADER_BYTES, 1, file ) == 1;
      }

      fclose( file ), file = 0;
    }

    /* Write shp file: */

    if ( result ) {
      file = fopen( shpFileName, "wb" );
      result = 0;

      if ( file ) {
        int record = 0;
        writeInt( header, 24, shpFileBytes / 2, BIG );
        result = fwrite( header, HEADER_BYTES, 1, file ) == 1;

        for ( record = 0; result && record < records; ++record ) {
          writeInt( recordHeader, 0, record + 1, BIG );
          result = fwrite( recordHeader, RECORD_HEADER_BYTES, 1, file ) == 1;

          /* Compute and write POLYGON record contents: */

          if ( result ) {
            const int row    = record / columns;
            const int column = record % columns;
            double xy[ VERTICES_PER_POLYGON * 2 ] = {
              0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
            };

            computePolygonVertices( row, column,
                                    westEdge, southEdge, cellWidth, cellHeight,
                                    unproject, xy, xyRange );

            /* Write polygon bounds: */

            byteIndex = writeDouble( recordContents, 4, xyRange[ 0 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xyRange[ 1 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xyRange[ 2 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xyRange[ 3 ], LITTLE);

            /* Write polygon vertices: */

            byteIndex =
              writeDouble( recordContents, 48, xy[ 0 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 1 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 2 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 3 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 4 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 5 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 6 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 7 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 8 ], LITTLE );
            byteIndex =
              writeDouble( recordContents, byteIndex, xy[ 9 ], LITTLE );
            result =
              fwrite( recordContents, RECORD_CONTENT_BYTES, 1, file ) == 1;
          }
        }

        fclose( file ), file = 0;
      }
    }

    if ( ! result ) {
      perror( "\n\nFailed because" );
    } else {
      result = writePRJFile( fileName, 0 );

      if ( AND3( result, variable, data ) ) {
        result =
          writeDataToDBFFile( fileName, variable, units,
                              timesteps, yyyymmddhh, timestepType,
                              records, type, components, data, lonlats, 0, 0 );
      }
    }

    FREE( lonlats );
  }

  return result;
}



/******************************************************************************
PURPOSE: writePointsToShapefile - Write points as a lon-lat Shapefile Point
         file set (shp, shx, dbf, prj) and a csv file containing time-varying
         data.
INPUTS:  const char* const fileName      Base name of file to create. "storet".
         const char* const variableName  Name of variable. E.g., "salinity".
         const char* const units         Units of variable. E.g., "PSU".
         const int timesteps             Number of timesteps of data.
         const int hoursPerTimestep      Number of hours per timestep. 1, 24.
         const int yyyymmddhh[ timesteps ]  Timestamps of data or 0 if no data.
         const int count                    Number of points per timestep.
         const float lonlats[ count * 2 ]   Lon-lats.
         const float z[ count ]             Optional: z coordinate of points.
         const int components               1 = scalar, 2, 3 = vector.
         const float data[ components ][ timesteps * count ]
                                         Optional: component data per point.
         const char* const sids[ count ]   Station id strings or 0.
RETURNS: int 1 if successful, else 0 and failure message is printed to stderr.
NOTES:   Creates files fileName.shp and fileName.shx
         then calls routines that create fileName.dbf and fileName.csv.
         See 1998 ESRI Shapefile Specification pages 2, 4, 5, 15, 23, 24.
         http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf
         This routine does not use shapelib API since it was written before
         that library was used in this project and also because this routine
         is more straight-forward and much faster than using the shapelib API.
******************************************************************************/

void writePointsToShapefile( const char* const fileName,
                             const char* const variableName,
                             const char* const units,
                             const int timesteps,
                             const int hoursPerTimestep,
                             const int yyyymmddhh[],
                             const int count,
                             const float lonlats[],
                             const float z[],
                             const int components,
                             const float data[],
                             const char* const sids[] ) {

  PRE013( fileName,
          *fileName,
          IMPLIES( data, AND3( variableName, *variableName, units ) ),
          IMPLIES( data, timesteps > 0 ),
          IMPLIES( data, hoursPerTimestep > 0 ),
          IMPLIES( data, yyyymmddhh ),
          count > 0,
          lonlats,
          isValidLongitudeLatitude( lonlats[ 0 ], lonlats[ 1 ] ),
          isValidLongitudeLatitude( lonlats[  count * 2 - 2 ],
                                       lonlats[  count * 2 - 1 ] ),
          IMPLIES( z, AND2( ! isNan( z[ 0 ] ), ! isNan( z[ count - 1 ] ) ) ),
          IMPLIES( timesteps > 1, AND2( components > 0, data ) ),
          IMPLIES( data, AND2( ! isNan( data[ 0 ] ),
                               ! isNan( data[ timesteps * count - 1 ] ) ) ) );

  int result = 0;
  enum { /* Constants from the Shapefile Spec (see above link): */
    BYTES_PER_INT = 4,
    BYTES_PER_DOUBLE = 8,
    POINTZ = 11,
    HEADER_BYTES = 100,
    RECORD_HEADER_BYTES = 8,
    RECORD_CONTENT_BYTES =
      BYTES_PER_INT +     /* int ShapeType = POINTZ */
      4 * BYTES_PER_DOUBLE  /* x, y, z, m */
  };
  unsigned char header[ HEADER_BYTES ] = "";
  unsigned char recordHeader[ RECORD_HEADER_BYTES ] = "";
  unsigned char recordContents[ RECORD_CONTENT_BYTES ] = "";
  const int records = count;
  const int shxFileBytes = HEADER_BYTES + records * RECORD_HEADER_BYTES;
  const int shpFileBytes = shxFileBytes + records * RECORD_CONTENT_BYTES;
  int byteIndex = 0;
  double xyRange[ 2 * 2 ] = { 0.0, 0.0, 0.0, 0.0 };
  enum { FILE_NAME_LENGTH = 255 };
  char shxFileName[ FILE_NAME_LENGTH + 1 ] = "";
  char shpFileName[ FILE_NAME_LENGTH + 1 ] = "";
  FILE* file = 0;

  /* Construct file names of resulting files from base file name: */

  memset( shxFileName, 0, sizeof shxFileName );
  memset( shpFileName, 0, sizeof shpFileName );
  strncpy( shxFileName, fileName, FILE_NAME_LENGTH - 8 );
  strncpy( shpFileName, fileName, FILE_NAME_LENGTH - 8 );
  strcat( shxFileName, ".shx" );
  strcat( shpFileName, ".shp" );

  computePointBounds( count, lonlats, xyRange );

  /* Initialize shx file header and records: */

  memset( header, 0, sizeof header );
  memset( recordHeader, 0, sizeof recordHeader );
  memset( recordContents, 0, sizeof recordContents );

  writeInt( header, 0, 9994, BIG );
  byteIndex = writeInt( header, 24, shxFileBytes / 2, BIG );
  byteIndex = writeInt( header, byteIndex, 1000, LITTLE );
  byteIndex = writeInt( header, byteIndex, POINTZ, LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 0 ], LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 1 ], LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 2 ], LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 3 ], LITTLE );
  
  if ( z ) { /* Compute and store z range (else defaults to [0, 0]): */
    double minimum = z[ 0 ];
    double maximum = minimum;
    int index = 0;

    for ( index = 1; index < count; ++index ) {
      const double value = z[ index ];

      if ( value < minimum ) {
        minimum = value;
      } else if ( value > maximum ) {
        maximum = value;
      }
    }

    byteIndex = writeDouble( header, byteIndex, minimum, LITTLE );
    byteIndex = writeDouble( header, byteIndex, maximum, LITTLE );
  }

  writeInt( recordHeader, 0, HEADER_BYTES / 2, BIG );
  writeInt( recordHeader, 4, RECORD_CONTENT_BYTES / 2, BIG );

  writeInt( recordContents, 0, POINTZ, LITTLE );

  /* Write shx file: */

  file = fopen( shxFileName, "wb" );

  if ( file ) {
    int record = 0;
    result = fwrite( header, HEADER_BYTES, 1, file ) == 1;

    for ( record = 0; result && record < records; ++record ) {
      const int offsetBytes =
        HEADER_BYTES +
        record * ( RECORD_HEADER_BYTES + RECORD_CONTENT_BYTES );
      writeInt( recordHeader, 0, offsetBytes / 2, BIG );
      result = fwrite( recordHeader, RECORD_HEADER_BYTES, 1, file ) == 1;
    }

    fclose( file ), file = 0;
  }

  /* Write shp file: */

  if ( result ) {
    file = fopen( shpFileName, "wb" );

    if ( file ) {
      int record = 0;
      writeInt( header, 24, shpFileBytes / 2, BIG );
      result = fwrite( header, HEADER_BYTES, 1, file ) == 1;

      for ( record = 0; result && record < records; ++record ) {
        const int record2 = record + record;
        writeInt( recordHeader, 0, record + 1, BIG );
        result = fwrite( recordHeader, RECORD_HEADER_BYTES, 1, file ) == 1;

        /* Write POINTZ record contents: */

        if ( result ) {
          const double longitude = lonlats[ record2 ];
          const double latitude = lonlats[ record2 + 1 ];
          byteIndex = writeDouble( recordContents, 4, longitude, LITTLE );
          byteIndex = writeDouble( recordContents, 12, latitude, LITTLE );

          if ( z ) {
            byteIndex = writeDouble( recordContents, 20, z[ record ], LITTLE );
          }

          result =
            fwrite( recordContents, RECORD_CONTENT_BYTES, 1, file ) == 1;
        }
      }

      fclose( file ), file = 0;
    }
  }

  if ( ! result ) {
    perror( "\n\nFailed because" );
  } else {
    result = writePRJFile( fileName, 0 );

    if ( AND2( result, data ) ) {
      const int timestepType = hoursPerTimestep == 24 ? DAILY : HOURLY;
      result =
        writeDataToDBFFile( fileName, variableName, units,
                            timesteps, yyyymmddhh, timestepType,
                            records, FLOAT_TYPE,
                            components, data,
                            lonlats, z, sids );
    }
  }
}



/******************************************************************************
PURPOSE: writePolylinesToShapefile - Write a polyline coordinates as a lon-lat
         Shapefile Polyline file set (shp, shx, dbf, prj).
INPUTS:  const char* baseFileName  Base name of file to create. "edm_bounds".
         const int polylineCount   Number of polylines.
         const int vertexCount     Number of vertices.
         const int counts[ polylineCount ]   Number of vertices per polyline.
         const float lonlats[ vertexCount * 2 ]  Lon-lat vertices.
RETURNS: int 1 if successful, else 0 and a message is printed to stderr.
NOTES:   Creates files fileName.shp and fileName.shx.
         See 1998 ESRI Shapefile Specification pages 2, 4, 5, 16, 23, 24.
         http://www.esri.com/library/whitepapers/pdfs/shapefile.pdf
         This routine does not use shapelib API since it was written before
         that library was used in this project and also because this routine
         is more straight-forward and much faster than using the shapelib API.
******************************************************************************/

int writePolylinesToShapefile( const char* baseFileName,
                               const int polylineCount,
                               const int vertexCount,
                               const int counts[],
                               const float lonlats[] ) {
  int result = 0;
  enum { /* Constants from the Shapefile Spec (see above link): */
    BYTES_PER_INT = 4,
    BYTES_PER_DOUBLE = 8,
    POLYLINE = 3,                 /* ShapeType = POLYLINE. */
    HEADER_BYTES = 100,
    RECORD_HEADER_BYTES = 8,
    RECORD_CONTENT_BYTES =
      1 * BYTES_PER_INT +        /* int ShapeType = POLYLINE */
      4 * BYTES_PER_DOUBLE +     /* double Box[ 4 ] = xMin,yMin,xMax,yMax */
      1 * BYTES_PER_INT +        /* int NumParts */
      1 * BYTES_PER_INT          /* int NumPoints */
  };
  const int recordContentBytes =
    RECORD_CONTENT_BYTES +
    polylineCount * BYTES_PER_INT + vertexCount * 2 * BYTES_PER_DOUBLE;
  unsigned char header[ HEADER_BYTES ] = "";
  unsigned char recordHeader[ RECORD_HEADER_BYTES ] = "";
  unsigned char recordContents[ RECORD_CONTENT_BYTES ] = "";
  const int shxFileBytes = HEADER_BYTES + RECORD_HEADER_BYTES;
  const int shpFileBytes =
    shxFileBytes + RECORD_CONTENT_BYTES +
    polylineCount * BYTES_PER_INT +
    vertexCount * 2 * BYTES_PER_DOUBLE;
  int byteIndex = 0;
  double xyRange[ 2 * 2 ] = { 0.0, 0.0, 0.0, 0.0 };
  enum { FILE_NAME_LENGTH = 255 };
  char shxFileName[ FILE_NAME_LENGTH + 1 ] = "";
  char shpFileName[ FILE_NAME_LENGTH + 1 ] = "";
  FILE* file = 0;

  DEBUG( fprintf( stderr, "baseFileName = %s\n", baseFileName ); )

  /* Construct file names of resulting files from base file name: */

  memset( shxFileName, 0, sizeof shxFileName );
  memset( shpFileName, 0, sizeof shpFileName );
  strncpy( shxFileName, baseFileName, FILE_NAME_LENGTH - 4 );
  strncpy( shpFileName, baseFileName, FILE_NAME_LENGTH - 4 );
  strcat( shxFileName, ".shx" );
  strcat( shpFileName, ".shp" );

  computeVertexBounds( vertexCount, lonlats, xyRange );

  /* Initialize shx file header and records: */

  memset( header, 0, sizeof header );
  memset( recordHeader, 0, sizeof recordHeader );
  memset( recordContents, 0, sizeof recordContents );

  writeInt( header, 0, 9994, BIG );
  byteIndex = writeInt( header, 24, shxFileBytes / 2, BIG );
  byteIndex = writeInt( header, byteIndex, 1000, LITTLE );
  byteIndex = writeInt( header, byteIndex, POLYLINE, LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 0 ], LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 1 ], LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 2 ], LITTLE );
  byteIndex = writeDouble( header, byteIndex, xyRange[ 3 ], LITTLE );

  writeInt( recordHeader, 0, HEADER_BYTES / 2, BIG );
  writeInt( recordHeader, 4, recordContentBytes / 2, BIG );

  writeInt( recordContents, 0, POLYLINE, LITTLE );
  writeInt( recordContents, 36, polylineCount, LITTLE );
  writeInt( recordContents, 40, vertexCount, LITTLE );

  /* Write shx file: */

  file = fopen( shxFileName, "wb" );

  if ( file ) {
    result = fwrite( header, HEADER_BYTES, 1, file ) == 1;
    writeInt( recordHeader, 0, HEADER_BYTES / 2, BIG );
    result = fwrite( recordHeader, RECORD_HEADER_BYTES, 1, file ) == 1;
    fclose( file ), file = 0;
  }

  /* Write shp file: */

  if ( result ) {
    file = fopen( shpFileName, "wb" );

    if ( file ) {
      writeInt( header, 24, shpFileBytes / 2, BIG );
      result = fwrite( header, HEADER_BYTES, 1, file ) == 1;
      writeInt( recordHeader, 0, 1, BIG );
      result = fwrite( recordHeader, RECORD_HEADER_BYTES, 1, file ) == 1;

      /* Compute and write POLYLINE record contents: */

      if ( result ) {
        byteIndex = writeDouble( recordContents, 4, xyRange[ 0 ], LITTLE );
        byteIndex =
          writeDouble( recordContents, byteIndex, xyRange[ 1 ], LITTLE );
        byteIndex =
          writeDouble( recordContents, byteIndex, xyRange[ 2 ], LITTLE );
        byteIndex =
          writeDouble( recordContents, byteIndex, xyRange[ 3 ], LITTLE);
        byteIndex =
          writeInt( recordContents, byteIndex, polylineCount, LITTLE );
        byteIndex =
          writeInt( recordContents, byteIndex, vertexCount, LITTLE );
        result = fwrite( recordContents, RECORD_CONTENT_BYTES, 1, file ) == 1;

        /* Write Parts array: */

        if ( result ) {
          int index = 0;
          int polyline = 0;

          for ( polyline = 0; result && polyline < polylineCount; ++polyline) {
            unsigned char value[ BYTES_PER_INT ] = "";
            writeInt( value, 0, index, LITTLE );
            result = fwrite( value, BYTES_PER_INT, 1, file ) == 1;
            index += counts[ polyline ];
          }
        }

        /* Write Points array: */

        if ( result ) {
          const float* longitudes = lonlats;
          const float* latitudes  = lonlats + 1;
          int vertex = 0;

          for ( vertex = 0; result && vertex < vertexCount;
                ++vertex, longitudes += 2, latitudes += 2 ) {
            unsigned char value[ 2 * BYTES_PER_DOUBLE ] = "";
            byteIndex = writeDouble( value, 0, *longitudes, LITTLE );
            writeDouble( value, byteIndex, *latitudes, LITTLE );
            DEBUG2( fprintf( stderr, "(%g, %g)\n", *longitudes, *latitudes );)
            result = fwrite( value, 2 * BYTES_PER_DOUBLE, 1, file ) == 1;
          }
        }
      }

      fclose( file ), file = 0;
    }
  }

  if ( ! result ) {
    perror( "\n\nFailed because" );
  } else {
    result = writePRJFile( baseFileName, 0 );
  }

  return result;
}



/******************************************************************************
PURPOSE: computePolygonVertices - Compute vertices of grid cell as an
         explicitly closed 2D 5-vertex polygon ring in clockwise order.
INPUTS:  const int row             0-based row index of grid cell.
         const int column          0-based column index of grid cell.
         const double westEdge     Distance from origin to west edge of grid.
         const double southEdge    Distance from origin to south edge of ".
         const double cellWidth    Width of each grid cell (e.g., 12000 m).
         const double cellWHeight  Height of each grid cell (e.g., 12000 m).
OUTPUTS: double xy[ 5 * 2 ]        Sequence of polygon (x, y) vertices.
         double xyRange[ 2 * 2 ]   xMin, yMin, xMax, yMax.
******************************************************************************/

void computePolygonVertices( const int row, const int column,
                             const double westEdge,
                             const double southEdge,
                             const double cellWidth,
                             const double cellHeight,
                             Unproject unproject,
                             double xy[ 5 * 2 ],
                             double xyRange[ 2 * 2 ] ) {

  double x = westEdge + column * cellWidth;
  double y = southEdge + row * cellHeight;
  double px = x;
  double py = y;
  double swap = 0.0;

  if ( unproject ) {
    unproject( x, y, &px, &py );
  }

  xy[ 0 ] = px;
  xy[ 1 ] = py;

  y += cellHeight;

  if ( unproject ) {
    unproject( x, y, &px, &py );
  } else {
    px = x;
    py = y;
  }

  xy[ 2 ] = px;
  xy[ 3 ] = py;

  x += cellWidth;

  if ( unproject ) {
    unproject( x, y, &px, &py );
  } else {
    px = x;
    py = y;
  }

  xy[ 4 ] = px;
  xy[ 5 ] = py;

  y -= cellHeight;

  if ( unproject ) {
    unproject( x, y, &px, &py );
  } else {
    px = x;
    py = y;
  }

  xy[ 6 ] = px;
  xy[ 7 ] = py;

  xy[ 8 ] = xy[ 0 ];
  xy[ 9 ] = xy[ 1 ];

  xyRange[ 0 ] = xyRange[ 1 ] = xyRange[ 2 ] = xyRange[ 3 ] = 0.0;
  computeRange( xy, 10, 2, xyRange );
  computeRange( xy + 1, 10, 2, xyRange + 2 );
  swap = xyRange[ 1 ];
  xyRange[ 1 ] = xyRange[ 2 ];
  xyRange[ 2 ] = swap;
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
         isValidLongitudeLatitude( longitudes[ 0 ], latitudes[ 0 ] ),
         isValidLongitudeLatitude( longitudes[ rows * columns - 1 ],
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

/* TEMP HACK #pragma omp parallel for private( column ) */

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

    } /* End loop on interior columns. */

  } /* End parallel loop on interior rows. */

  /* Serial region (not worth parallelizing): */

  /* Last row, interior columns (extrapolated top edge, except corners): */

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
  }

  /* First row, interior columns (extrapolated bottom edge, except corners): */

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
  }

  /* First column, interior rows (extrapolated left edge, except corners): */

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
  }

  /* Last column, interior rows (extrapolated right edge, except corners): */

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
  }

  /* First row, first column cell (extrapolated bottom-left corner): */

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
  }

  /* First row, last column cell (extrapolated bottom-right corner): */

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
  }

  /* Last row, first column cell (extrapolated top-left corner): */

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
  }

  /* Last row, last column cell (extrapolated top-right corner): */

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
  }

  /* Clamp any out-of-range values: */

/* TEMP HACK #pragma omp parallel for */

  for ( vIndex = 0; vIndex < count; vIndex += 2 ) {
    const int vIndex1 = vIndex + 1;
    vertices[ vIndex  ] = CLAMPED_TO_RANGE( vertices[ vIndex ], -180.0, 180.0);
    vertices[ vIndex1 ] = CLAMPED_TO_RANGE( vertices[ vIndex1 ], -90.0, 90.0 );
  }

  POST02( isValidLongitudeLatitude( vertices[ 0 ], vertices[ 1 ] ),
          isValidLongitudeLatitude( vertices[(rows+1)*(columns+1)* 2 - 2 ],
                                       vertices[(rows+1)*(columns+1)* 2 - 1]));
}




/******************************************************************************
PURPOSE: printShape - Print a ESRI polygon shape for debugging purposes.
INPUTS:  const SHPObject* shape  Shape to print.
NOTES:   http://shapelib.maptools.org/shp_api.html
******************************************************************************/

void printShape( const SHPObject* shape ) {
  PRE02( shape, shape->nVertices >= 2 );
  int index = 0;
  fprintf( stderr, "nSHPType = %d\n", shape->nSHPType );
  fprintf( stderr, "nShapeId = %d\n", shape->nShapeId );
  fprintf( stderr, "nParts = %d\n", shape->nParts );
  fprintf( stderr, "nVertices = %d\n", shape->nVertices );

  fprintf( stderr, "panPartStart:\n" );

  for ( index = 0; index < shape->nParts; ++index ) {
    fprintf( stderr, "%2d %d\n", index, shape->panPartStart[ index ] );
  }

  fprintf( stderr, "\n" );

  fprintf( stderr, "panPartType/Start/Count/X/Y:\n" );

  for ( index = 0; index < shape->nParts; ++index ) {
    const int start = shape->panPartStart[ index ];
    const int count =
      shape->nParts == 1 ? shape->nVertices
      : index < shape->nParts - 1 ?
        shape->panPartStart[ index + 1 ] - shape->panPartStart[ index ]
      : shape->nVertices - shape->panPartStart[ index ];
    const double x0 = shape->padfX[ start ];
    const double y0 = shape->padfY[ start ];
    const double x1 = shape->padfX[ start + 1 ];
    const double y1 = shape->padfY[ start + 1 ];
    const double xn_2 = shape->padfX[ start + count - 2 ];
    const double yn_2 = shape->padfY[ start + count - 2 ];
    const double xn_1 = shape->padfX[ start + count - 1 ];
    const double yn_1 = shape->padfY[ start + count - 1 ];
    fprintf( stderr,
             "# %3d: <%d, %4d, #%4d (%lg, %lg),(%lg, %lg)..."
             "(%lg, %lg), (%lg, %lg)>\n",
             index,
             shape->panPartType[ index ], shape->panPartStart[ index ],
             count,
             x0, y0, x1, y1, xn_2, yn_2, xn_1, yn_1 );
    CHECK( shape->panPartStart[ index ] >= 0 );
    CHECK( shape->panPartStart[ index ] < shape->nVertices );
    CHECK( shape->panPartType[ index ] == SHPP_RING );
    CHECK( IMPLIES( IN3( shape->nSHPType, SHPT_POLYGON, SHPT_POLYGONZ ),
                    AND3( count >= 4, x0 == xn_1, y0 == yn_1 ) ) );
  }
}



/******************************************************************************
PURPOSE: printPolygon - Print a polygon for tracing/debugging purposes.
INPUTS:  const gpc_polygon* polygon  Polygon to print.
NOTES:   http://www.cs.man.ac.uk/~toby/alan/software/gpc.html
******************************************************************************/

void printPolygon( const gpc_polygon* polygon ) {
  PRE0 ( polygon );
  int index = 0;
  fprintf( stderr, "num_contours = %d\n", polygon->num_contours );
  fprintf( stderr, "  hole[] =" );

  for ( index = 0; index < polygon->num_contours; ++index ) {
    fprintf( stderr, " %d", polygon->hole[ index ] );
  }

  fprintf( stderr, "\n  contour[]:\n" );

  for ( index = 0; index < polygon->num_contours; ++index ) {
    int v = 0;
    fprintf( stderr, "    num_vertices = %d:\n",
             polygon->contour[ index ].num_vertices );

    for ( v = 0; v < polygon->contour[ index ].num_vertices; ++v ) {

      if ( v < 2 || v > polygon->contour[ index ].num_vertices - 3 ) {
        fprintf( stderr, " #%4d (%lf, %lf)",
                 v,
                 polygon->contour[ index ].vertex[ v ].x,
                 polygon->contour[ index ].vertex[ v ].y );
      }
    }

    fprintf( stderr, "\n" );
  }
}



/******************************************************************************
PURPOSE: printTriangles - Print a triangle strip for debugging purposes.
INPUTS:  const gpc_tristrip* tristrip  Tristrip to print.
NOTES:   http://www.cs.man.ac.uk/~toby/alan/software/gpc.html
******************************************************************************/

void printTriangles( const gpc_tristrip* tristrip ) {
  PRE0 ( tristrip );
  int index = 0;
  fprintf( stderr, "num_strips = %d\n", tristrip->num_strips );

  fprintf( stderr, "\n  strip[]:\n" );

  for ( index = 0; index < tristrip->num_strips; ++index ) {
    const int vertexCount = tristrip->strip[ index ].num_vertices;
    int v = 0;
    fprintf( stderr, "    num_vertices = %d:\n", vertexCount );

    for ( v = 0; v < vertexCount; ++v ) {

      if ( v < 5 || v > vertexCount - 6 ) {
        fprintf( stderr, " #%4d (%lg, %lg)",
                 v,
                 tristrip->strip[ index ].vertex[ v ].x,
                 tristrip->strip[ index ].vertex[ v ].y );
      }
    }

    fprintf( stderr, "\n" );
  }
}



/******************************************************************************
PURPOSE: deallocatePolygons - Deallocate polygonShapes.
OUTPUTS: int count                            Number of polygonShapes.
         PolygonShape polygonShapes[ count ]  PolygonShapes to deallocate.
******************************************************************************/

void deallocatePolygons( int count, PolygonShape polygonShapes[] ) {
  PRE02( count >= 0, IMPLIES( count, polygonShapes ) );

  while ( count-- ) {

    if ( polygonShapes[ count ].polygon.num_contours > 0 ) {
      gpc_free_polygon( &polygonShapes[ count ].polygon );
      polygonShapes[ count ].polygon.num_contours = 0;
      polygonShapes[ count ].polygon.hole = 0;
      polygonShapes[ count ].polygon.contour = 0;
    }

    if ( polygonShapes[ count ].triangles.num_strips > 0 ) {
      gpc_free_tristrip( &polygonShapes[ count ].triangles );
      polygonShapes[ count ].triangles.num_strips = 0;
      polygonShapes[ count ].triangles.strip = 0;
    }

    polygonShapes[ count ].id = 0;
  }

  FREE( polygonShapes );
  POST0( polygonShapes == 0 );
}



/******************************************************************************
PURPOSE: copyPolygons - Copy polygons.
OUTPUTS: int count                                  Number of polygons.
         const PolygonShape polygonShapes[ count ]  polygonShapes to copy.
RETURNS: PolygonShape* Allocated array of count objects or 0 if failed and
         a message is printed to stderr.
******************************************************************************/

PolygonShape* copyPolygons( int count, const PolygonShape polygonShapes[] ) {
  PRE02( count >= 0, IMPLIES( count, polygonShapes ) );
  PolygonShape* result = count > 0 ? NEW( PolygonShape, count ) : 0;

  if ( result ) {
    int ok = 1;
    int shape = 0;

    for ( shape = 0; AND2( ok, shape < count ); ++shape ) {
      const PolygonShape* const sourcePolygonShape = polygonShapes + shape;
      PolygonShape* const destinationPolygonShape = result + shape;
      const gpc_polygon* const sourceGPCPolygon = &sourcePolygonShape->polygon;
      const int num_contours = sourceGPCPolygon->num_contours;

      destinationPolygonShape->id = sourcePolygonShape->id;
      memcpy( destinationPolygonShape->bounds, sourcePolygonShape->bounds,
              sizeof (Bounds) );

      /* Copy gpc_polygon: */

      if ( num_contours > 0 ) {
        gpc_polygon* const destinationGPCPolygon =
          &destinationPolygonShape->polygon;

        if ( sourceGPCPolygon->hole ) {
          destinationGPCPolygon->hole = NEW( int, num_contours );
          ok = destinationGPCPolygon->hole != 0;

          if ( ok ) {
            memcpy( destinationGPCPolygon->hole, sourceGPCPolygon->hole,
                    num_contours * sizeof (int) );
          }
        }

        if ( AND2( ok, sourceGPCPolygon->contour ) ) {
          const gpc_vertex_list* const sourceGPCVertexList =
            sourceGPCPolygon->contour;
          gpc_vertex_list* const destinationGPCVertexList =
            destinationGPCPolygon->contour =
            NEW( gpc_vertex_list, num_contours );
          ok = destinationGPCVertexList != 0;

          if ( ok ) {
            int contour = 0;

            for ( contour = 0; AND2( ok, contour < num_contours ); ++contour ) {
              const gpc_vertex_list* const thisSourceGPCVertexList =
                sourceGPCVertexList + contour;
              const int num_vertices = thisSourceGPCVertexList->num_vertices;

              if ( num_vertices > 0 ) {
                gpc_vertex_list* const thisDestinationGPCVertexList =
                  destinationGPCVertexList + contour;
                thisDestinationGPCVertexList->vertex =
                  NEW( gpc_vertex, num_vertices );
                ok = thisDestinationGPCVertexList->vertex != 0;

                if ( ok ) {
                  thisDestinationGPCVertexList->num_vertices = num_vertices;
                  memcpy( thisDestinationGPCVertexList->vertex,
                          thisSourceGPCVertexList->vertex,
                          num_vertices * sizeof (gpc_vertex) );
                  destinationGPCPolygon->num_contours += 1;
                }
              }
            }
          }
        }
      }

      /* Copy gpc_tristrip: */

      if ( AND2( ok, sourcePolygonShape->triangles.num_strips > 0 ) ) {
        const gpc_tristrip* const sourceGPCTristrip =
          &sourcePolygonShape->triangles;
        const int num_strips = sourceGPCTristrip->num_strips;
        const gpc_vertex_list* const sourceGPCVertexList =
          sourceGPCTristrip->strip;
        gpc_tristrip* const destinationGPCTristrip =
          &destinationPolygonShape->triangles;
        gpc_vertex_list* const destinationGPCVertexList =
          destinationGPCTristrip->strip =
          NEW( gpc_vertex_list, num_strips );
        ok = destinationGPCVertexList != 0;

        if ( ok ) {
          int strip = 0;

          for ( strip = 0; AND2( ok, strip < num_strips ); ++strip ) {
            const gpc_vertex_list* const thisSourceGPCVertexList =
              sourceGPCVertexList + strip;
            const int num_vertices = thisSourceGPCVertexList->num_vertices;

            if ( num_vertices > 0 ) {
              gpc_vertex_list* const thisDestinationGPCVertexList =
                destinationGPCVertexList + strip;
              thisDestinationGPCVertexList->vertex =
                NEW( gpc_vertex, num_vertices );
              ok = thisDestinationGPCVertexList->vertex != 0;

              if ( ok ) {
                thisDestinationGPCVertexList->num_vertices = num_vertices;
                memcpy( thisDestinationGPCVertexList->vertex,
                        thisSourceGPCVertexList->vertex,
                        num_vertices * sizeof (gpc_vertex) );
                destinationGPCTristrip->num_strips += 1;
              }
            }
          }
        }
      }
    }

    if ( ! ok ) {
      deallocatePolygons( count, result );
      result = 0;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: shapefileType - Get type of shapefile: SHPT_POLYGON, SHPT_ARC, etc.
INPUTS:  const char* baseFileName   Base name (no ext) of Shapefile to read.
RETURNS: int type if successful, else 0 and a failure message is printed.
******************************************************************************/

int shapefileType( const char* baseFileName ) {
  PRE02( baseFileName, *baseFileName );
  int result = 0;
  SHPHandle handle = SHPOpen( baseFileName, "rb" );

  if ( ! handle ) {
    fprintf( stderr, "\nFailed to open Shapefile '%s'\n", baseFileName );
    perror( "because" );
  } else {
    int type = 0;
    int shapes = 0;
    SHPGetInfo( handle, &shapes, &type, 0, 0 );
    SHPClose( handle ), handle = 0;

    if ( AND2( type >= 0, shapes > 0 ) ) {
      result = type;
    }
  }

  POST0( result >= 0 );
  return result;
}


    
/******************************************************************************
PURPOSE: masklipShapes - Read and clip and return array of
         shapes (with ids) clipped to the given bounds.
INPUTS:  const char* baseFileName   Base name (no ext) of Shapefile to read.
         const Bounds bounds        Clip bounds.
         double minimumAdjacentVertexDistance  Adjacent vertices closer than
                                               this (in either x or y) will
                                               be merged.
         const char* mask           0 or mask[] to filter shapes by.
OUTPUTS: int* count                 Length of returned array.
         int* isPolyline            Is shape a polyline?
RETURNS: PolygonShape* if successful, else 0 and a failure message is printed.
******************************************************************************/

PolygonShape* readAndClipShapes( const char* baseFileName, const Bounds bounds,
                                 double minimumAdjacentVertexDistance,
                                 const char* mask,
                                 int* count, int* isPolyline ) {

  PRE04( baseFileName, bounds, count, isPolyline );
  PolygonShape* result = 0;
  int ok = 0;
  SHPHandle handle = SHPOpen( baseFileName, "rb" );
  *count = 0;

  if ( ! handle ) {
    fprintf( stderr, "\nFailed to open Shapefile '%s'\n", baseFileName );
    perror( "because" );
  } else {
    int shapes = 0;
    int type = 0;
    double minimums[ 4 ] = { 0.0, 0.0, 0.0, 0.0 };
    double maximums[ 4 ] = { 0.0, 0.0, 0.0, 0.0 };
    Bounds dataBounds = { { 0.0, 0.0 }, { 0.0, 0.0 } };
    SHPGetInfo( handle, &shapes, &type, minimums, maximums );
    dataBounds[ LONGITUDE ][ MINIMUM ] = minimums[ 0 ];
    dataBounds[ LATITUDE  ][ MINIMUM ] = minimums[ 1 ];
    dataBounds[ LONGITUDE ][ MAXIMUM ] = maximums[ 0 ];
    dataBounds[ LATITUDE  ][ MAXIMUM ] = maximums[ 1 ];
    ok = 1;
    *isPolyline = IN3( type, SHPT_ARC, SHPT_ARCZ );

    if ( AND3( shapes > 0,
               IN5( type, SHPT_POLYGON, SHPT_POLYGONZ, SHPT_ARC, SHPT_ARCZ ),
               boundsOverlap( (const double (*)[2]) dataBounds, bounds ) ) ) {
      result = NEW( PolygonShape, shapes );
      ok = result != 0;

      if ( result ) {
        int index = 0;

        /* Make a static GPC clipping polygon, 'clip', from bounds: */

        gpc_vertex clipVertices[ 5 ] =
          { {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0} };
        int holes[ 5 ] = { 0, 0, 0, 0, 0 };
        gpc_vertex_list clipContour = { 5, 0 };
        gpc_polygon clip = { 1, 0, 0 };

        clipVertices[ 0 ].x = bounds[ LONGITUDE ][ MINIMUM ];
        clipVertices[ 0 ].y = bounds[ LATITUDE  ][ MINIMUM ];
        clipVertices[ 1 ].x = bounds[ LONGITUDE ][ MINIMUM ];
        clipVertices[ 1 ].y = bounds[ LATITUDE  ][ MAXIMUM ];
        clipVertices[ 2 ].x = bounds[ LONGITUDE ][ MAXIMUM ];
        clipVertices[ 2 ].y = bounds[ LATITUDE  ][ MAXIMUM ];
        clipVertices[ 3 ].x = bounds[ LONGITUDE ][ MAXIMUM ];
        clipVertices[ 3 ].y = bounds[ LATITUDE  ][ MINIMUM ];
        clipVertices[ 4 ].x = clipVertices[ 0 ].x;
        clipVertices[ 4 ].y = clipVertices[ 0 ].y;

        clipContour.vertex = clipVertices;

        clip.hole = holes;
        clip.contour = &clipContour;
        DEBUG( fprintf( stderr, "clip:\n" ); printPolygon( &clip ); )

        /* Clip each shape: */

        for ( index = 0; AND2( ok, index < shapes ); ++index ) {

          if ( OR2( mask == 0, mask[ index ] ) ) {
            SHPObject* shape = 0;
            DEBUG( fprintf( stderr, "Calling SHPReadObject()...\n" ); )
            shape = SHPReadObject( handle, index );
            DEBUG( fprintf( stderr, "done.\n" ); )
            ok = shape != 0;

            if ( shape ) {
              dataBounds[ LONGITUDE ][ MINIMUM ] = shape->dfXMin;
              dataBounds[ LATITUDE  ][ MINIMUM ] = shape->dfYMin;
              dataBounds[ LONGITUDE ][ MAXIMUM ] = shape->dfXMax;
              dataBounds[ LATITUDE  ][ MAXIMUM ] = shape->dfYMax;

              if ( boundsOverlap( (const double (*)[2]) dataBounds, bounds )) {
                DEBUG2(fprintf(stderr,
                            "overlap( [%lg %lg][%lg %lg], [%lg %lg][%lg %lg] )"
                              " = 1\n",
                              dataBounds[0][0], dataBounds[0][1],
                              dataBounds[1][0], dataBounds[1][1],
                              bounds[0][0], bounds[0][1],
                              bounds[1][0], bounds[1][1] ); )
                gpc_polygon copy = { 0, 0, 0 };
                DEBUG2(fprintf(stderr,"calling makePolygon() #%d ...\n",index);)
                ok = makePolygon( shape,  minimumAdjacentVertexDistance, &copy,
                                  result[ *count ].bounds );
                DEBUG2( fprintf( stderr, "done. makePolygon = %d\n", ok ); )
                DEBUG2( fprintf( stderr, "copy:\n" ); printPolygon( &copy ); )

                if ( AND2( ok, copy.num_contours > 0 ) ) {
                  int contours = 0;

                  if ( *isPolyline ) {
                    DEBUG2( fprintf( stderr, "calling clipPolylines()...\n"); )
                    clipPolylines( &copy, bounds, &result[ *count ].polygon,
                                   result[ *count ].bounds );
                  } else {
                    DEBUG2(fprintf(stderr, "calling gpc_polygon_clip()...\n");)
                    gpc_polygon_clip( GPC_INT, &copy, &clip,
                                      &result[ *count ].polygon );
                  }

                  DEBUG2( fprintf( stderr, "done.\n"); )
                  gpc_free_polygon( &copy );
                  DEBUG2( fprintf( stderr, "result:\n" ); )
                  DEBUG2( fprintf( stderr, "bounds = [%lg, %lg] [%lg, %lg]\n",
                                  result[ *count ].bounds[ 0 ][ 0 ],
                                  result[ *count ].bounds[ 0 ][ 1 ],
                                  result[ *count ].bounds[ 1 ][ 0 ],
                                  result[ *count ].bounds[ 1 ][ 1 ] ); )
                  DEBUG2( printPolygon( &result[ *count ].polygon ); )
                  contours = result[ *count ].polygon.num_contours;

                  if ( AND3( contours > 0,
                             minimumInt( contours,
                                         result[ *count ].polygon.hole ) == 0,
                             OR2( *isPolyline,
                                  ensureCorrectVertexOrder(
                                    &result[ *count ].polygon ) ) ) ) {
                    DEBUG2( fprintf(stderr, "keep shape id %d (%d contours).\n",
                                     shape->nShapeId, contours ); )
                    result[ *count ].id = shape->nShapeId;
                    *count += 1;
                  } else {
                    gpc_free_polygon( &result[ *count ].polygon );
                  }
                }
              }

              SHPDestroyObject( shape ), shape = 0;
            }
          }
        }
      }
    }

    SHPClose( handle ), handle = 0;
  }

  if ( AND2( OR2( ! ok, *count == 0 ), result != 0 ) ) {
    deallocatePolygons( *count, result ), *count = 0, result = 0;
  }

  POST0( IMPLIES_ELSE( result == 0,
                        *count == 0,
                        AND6( *count > 0,
                              IS_BOOL( *isPolyline ),
                              result[ 0 ].id >= 0,
                              result[ 0 ].polygon.num_contours > 0,
                              result[ 0 ].polygon.hole != 0,
                              result[ 0 ].polygon.contour != 0 ) ) );
  return result;
}




/******************************************************************************
PURPOSE: readAndTriangulateShapes - Read and triangulate and return array of
         shapes (with ids).
INPUTS:  const char* baseFileName   Base name (no ext) of Shapefile to read.
OUTPUTS: int* count                 Length of returned array.
RETURNS: PolygonShape* if successful, else 0 and a failure message is printed.
******************************************************************************/

PolygonShape* readAndTriangulateShapes( const char* baseFileName, int* count) {

  PRE02( baseFileName, count );
  PolygonShape* result = 0;
  int ok = 0;
  SHPHandle handle = SHPOpen( baseFileName, "rb" );
  *count = 0;

  if ( ! handle ) {
    fprintf( stderr, "\nFailed to open Shapefile '%s'\n", baseFileName );
    perror( "because" );
  } else {
    int shapes = 0;
    int type = 0;
    SHPGetInfo( handle, &shapes, &type, 0, 0 );
    ok = AND2( shapes > 0, IN3( type, SHPT_POLYGON, SHPT_POLYGONZ ) );

    if ( ok ) {
      result = NEW( PolygonShape, shapes );
      ok = result != 0;

      if ( result ) {
        int index = 0;

        /* Read each shape: */

        for ( index = 0; AND2( ok, index < shapes ); ++index ) {
          SHPObject* shape = 0;
          DEBUG( fprintf( stderr, "Calling SHPReadObject()...\n" ); )
          shape = SHPReadObject( handle, index );
          DEBUG( fprintf( stderr, "done.\n" ); )
          ok = shape != 0;

          if ( shape ) {
            gpc_polygon copy = { 0, 0, 0 };
            const double minimumAdjacentVertexDistance = 0.0;
            DEBUG2( fprintf( stderr, "calling makePolygon()...\n"); )
            ok = makePolygon( shape,  minimumAdjacentVertexDistance,
                              &copy, result[ *count ].bounds );
            DEBUG2( fprintf( stderr, "done. makePolygon = %d\n", ok ); )
            DEBUG( fprintf( stderr, "copy:\n" ); printPolygon( &copy ); )

            if ( AND2( ok, copy.num_contours > 0 ) ) {
              int strips = 0;
              DEBUG2(fprintf(stderr,"calling gpc_polygon_to_tristrip()...\n");)
              gpc_polygon_to_tristrip( &copy, &result[ *count ].triangles );
              DEBUG2( fprintf( stderr, "done.\n"); )
              gpc_free_polygon( &copy );
              memset( &copy, 0, sizeof copy );
              DEBUG( fprintf( stderr, "result:\n" ); )
              DEBUG( fprintf( stderr, "bounds = [%lg, %lg] [%lg, %lg]\n",
                              result[ *count ].bounds[ 0 ][ 0 ],
                              result[ *count ].bounds[ 0 ][ 1 ],
                              result[ *count ].bounds[ 1 ][ 0 ],
                              result[ *count ].bounds[ 1 ][ 1 ] ); )
              DEBUG( printTriangles( &result[ *count ].triangles ); )
              strips = result[ *count ].triangles.num_strips;

              if ( strips > 0 ) {
                DEBUG2( fprintf( stderr, "keep.\n" ); )
                result[ *count ].id = shape->nShapeId;
                *count += 1;
              } else {
                gpc_free_tristrip( &result[ *count ].triangles );
              }
            }

            SHPDestroyObject( shape ), shape = 0;
          }
        }
      }
    }

    SHPClose( handle ), handle = 0;
  }

  if ( AND2( OR2( ! ok, *count == 0 ), result != 0 ) ) {
    deallocatePolygons( *count, result ), *count = 0, result = 0;
  }

  POST0( IMPLIES_ELSE( result == 0,
                        *count == 0,
                        AND4( *count > 0,
                              result[ 0 ].id >= 0,
                              result[ 0 ].triangles.num_strips > 0,
                              result[ 0 ].triangles.strip != 0 ) ) );
  return result;
}



/******************************************************************************
PURPOSE: pointInTriangles - Is the specified point (x, y) in any of the set of
         triangles? If so return its index, else -1.
INPUTS:  double x          X-coordinate of point to test.
         double y          Y-coordinate of point to test.
         int count         Number of polygons in polygons[].
         const PolygonShape polygons[ count ]  Array of triangulated polygons.
RETURNS: int index [0, count - 1] if the point is inside the indexed triangles,
         else -1.
******************************************************************************/

int pointInTriangles( double x, double y,
                      int count, const PolygonShape polygons[] ) {

  PRE07( ! isNan( x ) , ! isNan( y ), count > 0, polygons,
         isValidBounds( (const double (*)[2]) polygons[ 0 ].bounds ),
         polygons[ 0 ].triangles.num_strips > 0,
         polygons[ count - 1 ].triangles.num_strips > 0 );

  int result = -1;
  int index = 0;

  for ( index = 0; index < count; ++index ) {
    const PolygonShape* const polygon = polygons + index;
    const double xMinimum = polygon->bounds[ LONGITUDE ][ MINIMUM ];
    const double xMaximum = polygon->bounds[ LONGITUDE ][ MAXIMUM ];
    const double yMinimum = polygon->bounds[ LATITUDE  ][ MINIMUM ];
    const double yMaximum = polygon->bounds[ LATITUDE  ][ MAXIMUM ];
    const int outsideBounds =
      OR4( x < xMinimum, x > xMaximum, y < yMinimum, y > yMaximum );

    if ( ! outsideBounds ) {
      const gpc_tristrip* const tristrip = &polygon->triangles;
      const int strips = tristrip->num_strips;
      int strip = 0;

      for ( strip = 0; strip < strips; ++strip ) {
        const gpc_vertex_list* const vertex_list = tristrip->strip + strip;
        const int vertexCount = vertex_list->num_vertices;
        const gpc_vertex* const vertices = vertex_list->vertex;
        int vertexIndex = 0;
        double x1 = vertices[ 0 ].x;
        double y1 = vertices[ 0 ].y;
        double x2 = vertices[ 1 ].x;
        double y2 = vertices[ 1 ].y;
        CHECK( vertexCount >= 3 );

        for ( vertexIndex = 2; vertexIndex < vertexCount; ++vertexIndex ) {
          const double x3 = vertices[ vertexIndex ].x;
          const double y3 = vertices[ vertexIndex ].y;
          const int insideTriangle =
            pointInsideTriangle( x, y, x1, y1, x2, y2, x3, y3 );

          if ( insideTriangle ) {
            result = index;
            vertexIndex = vertexCount; /* Stop looping. */
            strip = strips;
            index = count;
          } else {
            x1 = x2;
            y1 = y2;
            x2 = x3;
            y2 = y3;
          }
        }
      }
    }
  }

  POST0( IN_RANGE( result, -1, count - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: pointInsideTriangle - Determine if point (x, y) is inside triangle
         with vertices (x1, y1), (x2, y2), (x3, y3).
INPUTS:  double x    X-Coordinate of point to test.
         double y    Y-Coordinate of point to test.
         double x1   X-Coordinate of 1st vertex of triangle.
         double y1   Y-Coordinate of 1st vertex of triangle.
         double x2   X-Coordinate of 2nd vertex of triangle.
         double y2   Y-Coordinate of 2nd vertex of triangle.
         double x3   X-Coordinate of 3rd vertex of triangle.
         double y3   Y-Coordinate of 3rd vertex of triangle.
RETURNS: int 1 if inside, else 0.
******************************************************************************/

int pointInsideTriangle( double x, double y,
                         double x1, double y1,
                         double x2, double y2,
                         double x3, double y3 ) {
  const double scale = 1.01; /* Allow within 1% larger for round-off error. */
  const double triangleArea = scale * areaOfTriangle( x1, y1, x2, y2, x3, y3 );
  double area = areaOfTriangle( x, y, x2, y2, x3, y3 );
  int result = area <= triangleArea;

  if ( result ) {
    area += areaOfTriangle( x, y, x1, y1, x2, y2 );
    result = area <= triangleArea;

    if ( result ) {
      area += areaOfTriangle( x, y, x1, y1, x3, y3 );
      result = area <= triangleArea;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: areaOfTriangle - Area of triangle with vertices
         (x1, y1), (x2, y2), (x3, y3).
INPUTS:  double x1   X-Coordinate of 1st vertex of triangle.
         double y1   Y-Coordinate of 1st vertex of triangle.
         double x2   X-Coordinate of 2nd vertex of triangle.
         double y2   Y-Coordinate of 2nd vertex of triangle.
         double x3   X-Coordinate of 3rd vertex of triangle.
         double y3   Y-Coordinate of 3rd vertex of triangle.
RETURNS: double area.
******************************************************************************/

double areaOfTriangle( double x1, double y1,
                       double x2, double y2,
                       double x3, double y3 ) {
  const double a = x1 - x3;
  const double b = y1 - y3;
  const double c = x2 - x3;
  const double d = y2 - y3;
  const double triangleArea = 0.5 * ( a * d - b * c );
  const double result = triangleArea < 0.0 ? -triangleArea : triangleArea;
  return result;
}



/******************************************************************************
PURPOSE: nearestPolyline - Is the specified point (x, y) on any of the set of
         polylines? If so return its index, else -1.
INPUTS:  const double x          X-coordinate of point to test.
         const double y          Y-coordinate of point to test.
         const int count         Number of polygons in polygons[].
         const PolygonShape polylines[ count ]  Array of polylines.
RETURNS: int index [0, count - 1] if the point is on the indexed polylines,
         else -1.
******************************************************************************/

int nearestPolyline( const double x, const double y,
                     const int count, const PolygonShape polylines[] ) {

  PRE05( ! isNan( x ) , ! isNan( y ), count > 0, polylines,
         isValidBounds( (const double (*)[2]) polylines[ 0 ].bounds ) );

  int result = -1;
  int index = 0;
  const double tolerance = 1e-3; /* Close enough distance to line. */
  const double boundsMargin = 1e-3; /* Close enough to bounds. */
  double nearestDistance = DBL_MAX;

  for ( index = 0; index < count; ++index ) {
    const PolygonShape* const polygonShape = polylines + index;
    const double xMinimum0 = polygonShape->bounds[ LONGITUDE ][ MINIMUM ];
    const double xMaximum0 = polygonShape->bounds[ LONGITUDE ][ MAXIMUM ];
    const double yMinimum0 = polygonShape->bounds[ LATITUDE  ][ MINIMUM ];
    const double yMaximum0 = polygonShape->bounds[ LATITUDE  ][ MAXIMUM ];
    const double xDifference = xMaximum0 - xMinimum0;
    const double yDifference = yMaximum0 - yMinimum0;
    const double xTolerance = xDifference < boundsMargin ? boundsMargin : 0.0;
    const double yTolerance = yDifference < boundsMargin ? boundsMargin : 0.0;
    const double xMinimum = xMinimum0 - xTolerance;
    const double xMaximum = xMaximum0 + xTolerance;
    const double yMinimum = yMinimum0 - yTolerance;
    const double yMaximum = yMaximum0 + yTolerance;
    const int outsideBounds =
      OR4( x < xMinimum, x > xMaximum, y < yMinimum, y > yMaximum );
    CHECK2( xMaximum - xMinimum >= boundsMargin,
            yMaximum - yMinimum >= boundsMargin );

    if ( ! outsideBounds ) {
      const gpc_polygon* const polyline = &polygonShape->polygon;
      const int contours = polyline->num_contours;
      int contour = 0;

      for ( contour = 0; contour < contours; ++contour ) {
        const gpc_vertex_list* const vertexList = polyline->contour + contour;
        const int vertexCount = vertexList->num_vertices;
        const gpc_vertex* const vertices = vertexList->vertex;
        int vertexIndex = 0;
        double x1 = vertices[ 0 ].x;
        double y1 = vertices[ 0 ].y;
        CHECK( vertexCount >= 2 );

        for ( vertexIndex = 1; vertexIndex < vertexCount; ++vertexIndex ) {
          double x2 = vertices[ vertexIndex ].x;
          double y2 = vertices[ vertexIndex ].y;
          int outside = 0; /* Is point (x, y) within line segment bounds? */

          if ( x1 < x2 ) {
            outside = OR2( x < x1 - boundsMargin, x > x2 + boundsMargin );
          } else {
            outside = OR2( x < x2 - boundsMargin, x > x1 + boundsMargin );
          }

          if ( ! outside ) {

            if ( y1 < y2 ) {
              outside = OR2( y < y1 - boundsMargin, y > y2 + boundsMargin );
            } else {
              outside = OR2( y < y2 - boundsMargin, y > y1 + boundsMargin );
            }
          }

          if ( ! outside ) {
            const double distance = pointLineDistance( x, y, x1, y1, x2, y2 );

            if ( distance < nearestDistance ) {
              nearestDistance = distance;
              result = index;
            }
          }

          x1 = x2;
          y1 = y2;
        }
      }
    }
  }

  if ( nearestDistance > tolerance ) {
    result = -1;
  }

  POST0( IN_RANGE( result, -1, count - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: nearestPoint - Is the specified point (x, y) on any of the set of
         points? If so return its index, else -1.
INPUTS:  const double x          X-coordinate of point to test.
         const double y          Y-coordinate of point to test.
         const ShapeData* const shapeData  ShapeData with LONGITUDE, LATITUDE.
RETURNS: int index [0, count - 1] if the point is on the indexed polylines,
         else -1.
******************************************************************************/

int nearestPoint( const double x, const double y,
                  const ShapeData* const shapeData ) {

  PRE03( ! isNan( x ) , ! isNan( y ), isValidShapeData( shapeData ) );

  int result = -1;
  const double tolerance = 1e-3; /* Close enough distance. */
  double nearestDistance = DBL_MAX;
  const int rows = shapeData->rows;
  const int columns = shapeData->columns;
  const int longitudeColumn =
    indexOfString( "LONGITUDE", (const char**) shapeData->columnNames,
                   columns );
  const int latitudeColumn =
    indexOfString( "LATITUDE", (const char**) shapeData->columnNames,
                   columns );
  int row = 0;

  if ( AND2( IN_RANGE( longitudeColumn, 0, columns - 1 ),
             IN_RANGE( latitudeColumn, 0, columns - 1 ) ) ) {
    int longitudeIndex = longitudeColumn;
    int latitudeIndex  = latitudeColumn;

    for ( row = 0; row < rows; ++row,
          longitudeIndex += columns, latitudeIndex += columns ) {

      CHECK2( IN_RANGE( longitudeIndex, 0, rows * columns - 1 ),
              IN_RANGE( latitudeIndex,  0, rows * columns - 1 ) );
              
      {
        const double longitude = shapeData->values[ longitudeIndex ].d;
        const double latitude  = shapeData->values[ latitudeIndex  ].d;
        const double longitudeDistance =
          x < longitude ? longitude - x : x - longitude;
        const double latitudeDistance =
          y < latitude ? latitude - y : y - latitude;
        const double distance = longitudeDistance + latitudeDistance;

        if ( distance < nearestDistance ) {
          nearestDistance = distance;
          result = row;
        }
      }
    }

    if ( nearestDistance > tolerance ) {
      result = -1;
    }
  }

  POST0( IN_RANGE( result, -1, shapeData->rows - 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: pointLineDistance - Nearest distance from point (x, y) to infinite
         undirected line containing points (x1, y1)--(x2, y2).
INPUTS:  const double x   X-Coordinate of point to check.
         const double y   Y-Coordinate of point to check.
         const double x1  X-Coordinate of 1st endpoint of line.
         const double y1  Y-Coordinate of 1st endpoint of line.
         const double x2  X-Coordinate of 2nd endpoint of line.
         const double y2  Y-Coordinate of 2nd endpoint of line.
RETURNS: double distance  Nearest distance from point to line.
NOTES:   Swokowski, Earl, "Calculus With Analytic Geometry, Second Edition",
         Prindle, Weber & Schmidt, Boston, MA, 1979, page 696.
******************************************************************************/

double pointLineDistance( const double x, const double y,
                          const double x1, const double y1,
                          const double x2, const double y2 ) {
  
  PRE06( ! isNan( x ) , ! isNan( y ),
         ! isNan( x1 ) , ! isNan( y1 ),
         ! isNan( x2 ) , ! isNan( y2 ) );
  double result = 0.0;
  const double dx = x2 - x1;
  const double dy = y2 - y1;
  const double lineLength = sqrt( dx * dx + dy * dy );

  if ( lineLength == 0.0 ) { /* Degenerate line so compute point distance: */
    const double dx0 = x - x1;
    const double dy0 = y - y1;
    result = sqrt( dx0 * dx0 + dy0 * dy0 );
  } else {
    const double recipricolLineLength = 1.0 / lineLength;

    if ( recipricolLineLength <= 1e-12 ) {

      if ( ! colinear( x, y, x1, y1, x2, y2 ) ) {
        result = DBL_MAX;
      }

    } else {
      const double px = x - x1;
      const double py = y - y1;
      const double z0 = dx * py - px * dy;
      const double z = z0 < 0.0 ? -z0 : z0;
      result = recipricolLineLength * z;
    }
  }

  POST0( result >= 0.0 );
  return result;
}



/******************************************************************************
PURPOSE: colinear - Do the given three points lie along a line (or are they
         coincident)?
INPUTS:  const double x1  X-Coordinate of 1st point to check.
         const double y1  Y-Coordinate of 1st point to check.
         const double x2  X-Coordinate of 2nd point to check.
         const double y2  Y-Coordinate of 2nd point to check.
         const double x3  X-Coordinate of 3rd point to check.
         const double y3  Y-Coordinate of 3rd point to check.
RETURNS: int 1 if colinear, else 0.
NOTES:   Swokowski, Earl, "Calculus With Analytic Geometry, Second Edition",
         Prindle, Weber & Schmidt, Boston, MA, 1979, page 670.
******************************************************************************/

int colinear( const double x1, const double y1,
              const double x2, const double y2,
              const double x3, const double y3 ) {

  PRE06( ! isNan( x1 ) , ! isNan( y1 ),
         ! isNan( x2 ) , ! isNan( y2 ),
         ! isNan( x3 ) , ! isNan( y3 ) );
  /*
   * First check for coincidence:
   *   if any two (or all three) points are coincident then consider this
   *   degenerate case colinear
   * Else check for reflectance:
   *   if one of the points is the origin and the other two are negatives
   *   of each other { the three points are colinear.
   * Otherwise compute the result as follows:
   * 1. Form three vectors: P1->P2 and P1->P3 and P2->P3.
   * 2. Normalize the vectors and compute the
   *    dot-products: P1->P2 * P1->P3 and P1->P2 * P2->P3
   * 3. If both dot-products are approximately +/-1 then
   *    the points are colinear since the angle between the vectors is
   *    arccosine (+/-1) = 0 (or pi).
   * Swokowski, Earl, "Calculus With Analytic Geometry, Second Edition",
   * Prindle, Weber & Schmidt, Boston, MA, 1979, page 670.
   * Note: the coincidence and reflectance checks are necessary because
   * the angle-test is not numerically robust enough to handle these cases
   * correctly for the full range of non-NaN values.
   */

  const double tolerance = 1e-6;
  int result = 1;

  if ( ! AND2( x1 == x2, y1 == y2 )  ) { /* non-coincident. */

    if ( ! AND2( x1 == x3, y1 == y3 )  ) { /* non-coincident. */

      if ( ! AND2( x2 == x3, y2 == y3 )  ) { /* non-coincident. */

        if ( ! AND4( x1 == 0.0, y1 == 0.0,
                     x2 == -x3, y2 == -y3 ) ) { /* non-reflected */

          if ( ! AND4( x2 == 0.0, y2 == 0.0,
                       x1 == -x3, y1 == -y3 ) ) { /* non-reflected */

            if ( ! AND4( x3 == 0.0, y3 == 0.0,
                         x1 == -x2, y1 == -y2 ) ) { /* non-reflected */
              const double v1x_ = x2 - x1;
              const double v1y_ = y2 - y1;
              const double v1m = 1.0 / sqrt( v1x_ * v1x_ + v1y_ * v1y_ );
              const double v1x = v1x_ * v1m;
              const double v1y = v1y_ * v1m;
              const double v2x_ = x3 - x1;
              const double v2y_ = y3 - y1;
              const double v2m = 1.0 / sqrt( v2x_ * v2x_ + v2y_ * v2y_ );
              const double v2x = v2x_ * v2m;
              const double v2y = v2y_ * v2m;
              const double v1DotV2    = v1x * v2x + v1y * v2y;
              const double magnitude1 = fabs( v1DotV2 );
              const double oneMinus   = 1.0 - tolerance;
              const double onePlus    = 1.0 + tolerance;
              result = IN_RANGE( magnitude1, oneMinus, onePlus );

              if ( result ) {
                const double v3x_ = x3 - x2;
                const double v3y_ = y3 - y2;
                const double v3m = 1.0 / sqrt( v3x_ * v3x_ + v3y_ * v3y_ );
                const double v3x = v3x_ * v3m;
                const double v3y = v3y_ * v3m;
                const double v1DotV3    = v1x * v3x + v1y * v3y;
                const double magnitude2 = fabs( v1DotV3 );
                result = IN_RANGE( magnitude2, oneMinus, onePlus );
              }
            }
          }
        }
      }
    }
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: makePolygon - Make a GPC-polygon from an ESRI Shape polygon/polyline.
INPUTS:  const SHPObject* shape  Shape to copy.
         double minimumAdjacentVertexDistance  Adjacent vertices closer than
                                               this (in either x or y) will
                                               be merged.
OUTPUTS: gpc_polygon* polygon  GPC polygon.
         Bounds bounds         Bounds of sparsed polygon vertices.
RETURNS: int 1 if successful (that is no allocation failures, but possibly no
         contours in polygon), else 0 and a failure message is printed.
NOTES:   Call gpc_free_polygon( polygon ) when finished with it.
         http://shapelib.maptools.org/shp_api.html
         http://www.cs.man.ac.uk/~toby/alan/software/gpc.html
******************************************************************************/

int makePolygon( const SHPObject* shape, double minimumAdjacentVertexDistance,
                 gpc_polygon* polygon, Bounds bounds ) {

  PRE013( shape,
          IN5( shape->nSHPType,
               SHPT_POLYGON, SHPT_POLYGONZ, SHPT_ARC, SHPT_ARCZ ),
          shape->nShapeId >= 0,
          shape->nParts > 0,
          shape->panPartStart,
          shape->panPartType,
          shape->panPartType[ 0 ] == SHPP_RING,
          shape->nVertices > 0,
          shape->padfX,
          shape->padfY,
          minimumAdjacentVertexDistance >= 0.0,
          polygon,
          bounds );

  int result = 1;
  const int isPolygon = IN3( shape->nSHPType, SHPT_POLYGON, SHPT_POLYGONZ );
  const int minimumSparsedVertices = isPolygon ? 3 : 2;
  const int parts = shape->nParts;
  int sparsedParts = 0;
  DEBUG2( printShape( shape ); )
  memset( polygon, 0, sizeof *polygon );
  memset( bounds, 0, sizeof (Bounds) );

  /* Compute sparsed number of parts (with at least minimumSparsedVertices): */

  sparsedParts =
    computeSparsedPartCount( shape, minimumAdjacentVertexDistance,
                             minimumSparsedVertices );

  if ( sparsedParts > 0 ) {
    result = 0;
    polygon->hole = NEW( int, sparsedParts );

    if ( polygon->hole ) {
      polygon->contour = NEW( gpc_vertex_list, sparsedParts );

      if ( polygon->contour ) {
        int ok = 1;
        int part = 0;
        int sparsedPart = 0;
        int initializedBounds = 0;

        polygon->num_contours = sparsedParts;

        /* For each part: */

        for ( part = 0; AND2( ok, part < parts ); ++part ) {
          const int partVertexCount =
            parts == 1 ? shape->nVertices
            : part < parts - 1 ?
              shape->panPartStart[ part + 1 ] - shape->panPartStart[ part ]
            : shape->nVertices - shape->panPartStart[ part ];
          const int offset = shape->panPartStart[ part ];
          const double* const x = shape->padfX + offset;
          const double* const y = shape->padfY + offset;
          const int sparsedVertices =
            computeSparsedVertexCount( partVertexCount, x, y,
                                       minimumAdjacentVertexDistance,
                                       isPolygon );

          /* Allocate and copy sparse vertices: */

          if ( sparsedVertices >= ( minimumSparsedVertices + isPolygon ) ) {
            gpc_vertex* vertices = NEW( gpc_vertex, sparsedVertices );
            ok = vertices != 0;

            if ( vertices ) {
              CHECK( IN_RANGE( sparsedPart, 0, sparsedParts - 1 ) );
              polygon->contour[ sparsedPart ].num_vertices = sparsedVertices;
              polygon->contour[ sparsedPart ].vertex = vertices;
              copySparseVertices( partVertexCount, sparsedVertices, x, y,
                                  minimumAdjacentVertexDistance, isPolygon,
                                  &initializedBounds, bounds, vertices );
              CHECK( IN_RANGE( sparsedPart, 0, sparsedParts - 1 ) );

              /* Compute hole flag of sparse contour: */

              if ( isPolygon ) {
                const int counterClockwise =
                  signedAreaOfPolygon( &polygon->contour[sparsedPart] ) >= 0.0;
                polygon->hole[ sparsedPart ] = counterClockwise;
              }

              ++sparsedPart;
            }
          }
        }

        result = ok;
      }
    }
  }

  if ( ! result ) {
    gpc_free_polygon( polygon );
    memset( polygon, 0, sizeof *polygon );
    memset( bounds, 0, sizeof (Bounds) );
  }

  DEBUG2( printPolygon( polygon ); )

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( AND2( result, polygon->num_contours > 0 ),
                        AND4( IN_RANGE( polygon->num_contours, 1,
                                        shape->nParts ),
                              polygon->hole,
                              polygon->contour,
                              isValidBounds( (const double (*)[2]) bounds ) ),
                        IS_ZERO7( polygon->num_contours,
                                  polygon->hole,
                                  polygon->contour,
                                  bounds[ 0 ][ 0 ],
                                  bounds[ 0 ][ 1 ],
                                  bounds[ 1 ][ 0 ],
                                  bounds[ 1 ][ 1 ] ) ) );
  return result;
}


/******************************************************************************
PURPOSE: clipPolylines - Clip polylines to a given bounds.
INPUTS:  const gpc_polygon* const polylines  GPC polylines to clip.
         const Bounds clipBounds             Clip bounds.
OUTPUTS: gpc_polygon* clippedPolylines       Clipped GPC polylines.
         Bounds clippedPolylinesBounds       Bounds of clippedPolylines.
RETURNS: int 1 if successful (that is no allocation failures, but possibly no
         contours in bounds), else 0 and a failure message is printed.
NOTES:   Call gpc_free_polygon( clippedPolylines ) when finished with it.
         http://www.cs.man.ac.uk/~toby/alan/software/gpc.html
******************************************************************************/

int clipPolylines( const gpc_polygon* const polylines,
                   const Bounds clipBounds,
                   gpc_polygon* clippedPolylines,
                   Bounds clippedPolylinesBounds ) {

  PRE04( polylines,
         isValidBounds( (const double (*)[2]) clipBounds ),
         polylines,
         isValidBounds( (const double (*)[2]) clippedPolylinesBounds ) );
  int result = 0;
  const int polylineCount = polylines->num_contours;
  memset( clippedPolylines, 0, sizeof *clippedPolylines );
  memset( clippedPolylinesBounds, 0, sizeof (Bounds) );

  if ( polylineCount > 0 ) { /* Count number of vertices for all polylines: */
    const int vertexCount = polygonVertexCount( polylines );

    if ( vertexCount > 0 ) { /* Copy vertices: */
      int* inputCounts = NEW( int, polylineCount );
      double* inputVertices = inputCounts ? NEW( double, vertexCount * 2 ) : 0;

      if ( inputVertices ) {
        int outputPolylineCount = 0;
        int outputVertexCount = 0;
        copyPolylineVertices( polylines, inputVertices );

        /* 1st call: get number of clipped polylines and total vertices: */

        subsetMapDouble( polylineCount, vertexCount,
                         inputCounts, inputVertices, 0.0, clipBounds,
                         &outputPolylineCount, &outputVertexCount,
                         0, 0 );

        /* Allocate storage for clipped results: */

        if ( AND2( outputPolylineCount > 0, outputVertexCount >= 2 ) ) {
          int* outputCounts = NEW( int, outputPolylineCount );
          double* outputVertices =
            outputCounts ? NEW( double, outputVertexCount * 2 ) : 0;

          if ( outputVertices ) {
            int outputPolylineCount2 = 0;
            int outputVertexCount2 = 0;

            /* 2nd call: get clipped counts and vertices: */

            subsetMapDouble( polylineCount, vertexCount,
                             inputCounts, inputVertices, 0.0, clipBounds,
                             &outputPolylineCount2, &outputVertexCount2,
                             outputCounts, outputVertices );

            CHECK2( outputPolylineCount2 == outputPolylineCount,
                    outputVertexCount2 == outputVertexCount );

            result = createPolyline( outputPolylineCount, outputCounts,
                                     outputVertices, clippedPolylines,
                                     clippedPolylinesBounds );
          }

          FREE( outputCounts );
          FREE( outputVertices );
        }

        FREE( inputCounts );
        FREE( inputVertices );
      }
    }
  }

  if ( ! result ) {
    gpc_free_polygon( clippedPolylines );
    memset( clippedPolylines, 0, sizeof *clippedPolylines );
    memset( clippedPolylinesBounds, 0, sizeof (Bounds) );
  }

  DEBUG2( printPolygon( clippedPolylines ); )

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( AND2( result, clippedPolylines->num_contours > 0 ),
                        AND4( minimumInt( clippedPolylines->num_contours,
                                          clippedPolylines->hole ) == 0,
                              maximumInt( clippedPolylines->num_contours,
                                          clippedPolylines->hole ) == 0,
                              clippedPolylines->contour,
                              isValidBounds( (const double (*)[2])
                                             clippedPolylinesBounds ) ),
                        IS_ZERO7( clippedPolylines->num_contours,
                                  clippedPolylines->hole,
                                  clippedPolylines->contour,
                                  clippedPolylinesBounds[ 0 ][ 0 ],
                                  clippedPolylinesBounds[ 0 ][ 1 ],
                                  clippedPolylinesBounds[ 1 ][ 0 ],
                                  clippedPolylinesBounds[ 1 ][ 1 ] ) ) );
  return result;
}



/******************************************************************************
PURPOSE: maximumPolygonContours - Maximum number of contours in set of polygons
INPUTS:  const int count                  Number of polygons.
         const PolygonShape polygons[ count ]  Polygons to check.
RETURNS: int maximum number of contours in set of polygons.
******************************************************************************/

int maximumPolygonContours( int count, const PolygonShape polygons[] ) {
  PRE02( count > 0, polygons );
  int result = 0;
  int polygon = 0;

  for ( polygon = 0; polygon < count; ++polygon ) {
    const int contours = polygons[ polygon ].polygon.num_contours;

    if ( contours > result ) {
      result = contours;
    }
  }

  POST0( result > 0 );
  return result;
}



/******************************************************************************
PURPOSE: maximumPolygonVertices - Maximum number of vertices in set of polygons
INPUTS:  const int count                  Number of polygons.
         const PolygonShape polygons[ count ]  Polygons to check.
RETURNS: int maximum number of vertices in set of polygons.
******************************************************************************/

int maximumPolygonVertices( int count, const PolygonShape polygons[] ) {
  PRE02( count > 0, polygons );
  int result = 0;
  int polygon = 0;

  for ( polygon = 0; polygon < count; ++polygon ) {
    result += polygonVertexCount( &polygons[ polygon ].polygon );
  }

  POST0( result > 0 );
  return result;
}



/******************************************************************************
PURPOSE: polygonVertexCount - Number of vertices in polygon.
INPUTS:  const gpc_polygon* polygon  Polygon to check.
RETURNS: int number of vertices in polygon.
******************************************************************************/

int polygonVertexCount( const gpc_polygon* polygon ) {
  PRE0( polygon );
  const int contours = polygon->num_contours;
  int contour = 0;
  int result = 0;

  for ( contour = 0; contour < contours; ++contour ) {
    const gpc_vertex_list vertexList = polygon->contour[ contour ];
    const int vertices = vertexList.num_vertices;
    result += vertices;
  }

  POST0( result > 0 );
  return result;
}



/******************************************************************************
PURPOSE: copyPolygonVertices - Copy vertices from polygon to x and y arrays.
INPUTS:  const gpc_polygon* polygon  Polygon to copy from.
         int closeRing               Copy first vertex to last to close ring?
OUTPUTS: int starts[]                Optional 0-based indices of vertex lists.
         double x[]                  X-coordinates of vertices.
         double y[]                  Y-coordinates of vertices.
******************************************************************************/

void copyPolygonVertices( const gpc_polygon* polygon, int closeRing,
                          int starts[], double x[], double y[] ) {

  PRE07( polygon, IS_BOOL( closeRing ), polygon->num_contours > 0,
         IMPLIES( polygon->num_contours > 1, starts ),
         x, y, x != y );

  double* xp = x;
  double* yp = y;
  const int contours = polygon->num_contours;
  int contour = 0;
  int offset = 0;

  if ( starts ) {
    starts[ 0 ] = 0;
  }

  for ( contour = 0; contour < contours; ++contour ) {
    const gpc_vertex_list* const vertexList = polygon->contour + contour;
    const int vertices = vertexList->num_vertices;
    const gpc_vertex* v = vertexList->vertex;
    const double x0 = v->x;
    const double y0 = v->y;
    int vertex = 0;

    if ( AND2( starts, contour < contours - 1 ) ) {
      offset += vertices + closeRing;
      starts[ contour + 1 ] = offset;
    }

    for ( vertex = 0; vertex < vertices; ++vertex ) {
      *xp++ = v->x;
      *yp++ = v->y;
      ++v;
    }

    if ( closeRing ) { /* Copy 1st vertex to last to ensure closed ring: */
      *xp++ = x0;
      *yp++ = y0;
    }
  }

  POST04( x[ 0 ] == polygon->contour->vertex->x,
          y[ 0 ] == polygon->contour->vertex->y,
          IMPLIES( starts, starts[ 0 ] == 0 ),
          IMPLIES( AND2( starts, polygon->num_contours > 1 ),
                   AND2( starts[ 1 ] > starts[ 0 ],
                         starts[ polygon->num_contours - 1 ] >
                           starts[ polygon->num_contours - 2 ] ) ) );
}



/******************************************************************************
PURPOSE: copyPolylineVertices - Copy vertices from polyline to xy array.
INPUTS:  const gpc_polygon* polygon  Polyline to copy from.
OUTPUTS: double xy[]                 X/Y-coordinates of vertices.
******************************************************************************/

void copyPolylineVertices( const gpc_polygon* polygon, double xy[] ) {

  PRE03( polygon, polygon->num_contours > 0, xy );

  double* xp = xy;
  double* yp = xp + 1;
  const int contours = polygon->num_contours;
  int contour = 0;

  for ( contour = 0; contour < contours; ++contour ) {
    const gpc_vertex_list* const vertexList = polygon->contour + contour;
    const int vertices = vertexList->num_vertices;
    const gpc_vertex* v = vertexList->vertex;
    int vertex = 0;

    for ( vertex = 0; vertex < vertices; ++vertex ) {
      *xp++ = v->x;
      *yp++ = v->y;
      ++v;
    }
  }

  POST02( xy[ 0 ] == polygon->contour->vertex->x,
          xy[ 1 ] == polygon->contour->vertex->y );
}



/******************************************************************************
PURPOSE: createPolyline - Allocate and copy xy vertices to polyline.
INPUTS:  const int polylineCount              Number of polylines.
         const int vertexCounts[ polylineCount ]      # of vertices per polyline.
         const double xy[]                    X/Y-coordinates of all vertices.
OUTPUTS: gpc_polygon* polygon  Polygon with allocated/copied hole and vertices.
         Bounds bounds         Bounds of vertices.
RETURNS: int 1 if successful, else 0 and a failure message is printed to stdout
******************************************************************************/

int createPolyline( const int polylineCount, const int vertexCounts[],
                    const double xy[], gpc_polygon* polygon,
                    Bounds bounds ) {

  PRE08( polylineCount > 0, vertexCounts, xy,
         polygon, polygon->num_contours == 0,
         polygon->hole == 0, polygon->contour == 0, bounds );

  const double* xp = xy;
  const double* yp = xp + 1;
  double xMinimum = *xp;
  double xMaximum = xMinimum;
  double yMinimum = *yp;
  double yMaximum = yMinimum;
  const int contours = polygon->num_contours = polylineCount;
  int contour = 0;
  int result = 0;
  polygon->hole = NEW( int, contours );
  polygon->contour =
    polygon->hole == 0 ? 0 : NEW( gpc_vertex_list, contours );
  result = polygon->contour != 0;

  for ( contour = 0; AND2( result, contour < contours ); ++contour ) {
    gpc_vertex_list* const vertexList = polygon->contour + contour;
    const int vertices = vertexList->num_vertices = vertexCounts[ contour ];
    gpc_vertex* v = vertexList->vertex = NEW( gpc_vertex, vertices );
    result = v != 0;

    if ( result ) {
      int vertex = 0;

      for ( vertex = 0; vertex < vertices; ++vertex ) {
        const double x = *xp++;
        const double y = *yp++;
        v->x = x;
        v->y = y;
        ++v;

        if ( x < xMinimum ) {
          xMinimum = x;
        } else if ( x > xMaximum ) {
          xMaximum = x;
        }

        if ( y < yMinimum ) {
          yMinimum = y;
        } else if ( y > yMaximum ) {
          yMaximum = y;
        }
      }
    }
  }

  bounds[ LONGITUDE ][ MINIMUM ] = xMinimum;
  bounds[ LONGITUDE ][ MAXIMUM ] = xMaximum;
  bounds[ LATITUDE  ][ MINIMUM ] = yMinimum;
  bounds[ LATITUDE  ][ MAXIMUM ] = yMaximum;

  if ( ! result ) {
    gpc_free_polygon( polygon );
    memset( polygon, 0, sizeof *polygon );
  }

  POST03( IS_BOOL( result ),
          isValidBounds( (const double (*)[2]) bounds ),
          IMPLIES_ELSE( result,
                        AND5( polygon->hole,
                              polygon->contour,
                              polygon->num_contours == polylineCount,
                              xy[ 0 ] == polygon->contour->vertex->x,
                              xy[ 1 ] == polygon->contour->vertex->y ),
                        IS_ZERO3( polygon->hole,
                                  polygon->contour,
                                  polygon->num_contours ) ) );

  return result;
}



/******************************************************************************
PURPOSE: ensureCorrectVertexOrder - Check and correct the vertex order to match
         hole designation to ESRI spec - i.e., hole vertices are in CCW order.
INPUTS:  gpc_polygon* polygon  Polygon to check/correct.
RETURNS: int 1 if corrected polygon now has strictly positive net area,
         else 0 which indicates that the area of the holes is >=
         the area of the non-holes.
NOTES:   http://mathworld.wolfram.com/PolygonArea.html
******************************************************************************/

int ensureCorrectVertexOrder( gpc_polygon* polygon ) {
  PRE0( polygon );
  int result = 0;
  double polygonArea = 0.0;
  const int contours = polygon->num_contours;
  int contour = 0;
  DEBUG2( fprintf( stderr, "ensureCorrectVertexOrder():\n" ); )

  for ( contour = 0; contour < contours; ++contour ) {
    const int isHole = polygon->hole[ contour ];
    const gpc_vertex_list vertex_list = polygon->contour[ contour ];
    const gpc_vertex* v = vertex_list.vertex;
    const int vertices = vertex_list.num_vertices;
    double contourArea = 0.0;
    int vertex = 0;

    for ( vertex = 0; vertex < vertices; ++vertex ) {
      const int vertexp1 = vertex + 1;
      const int vertex1 = vertexp1 < vertices ? vertexp1 : 0;
      const double x0 = v[ vertex  ].x;
      const double y0 = v[ vertex  ].y;
      const double x1 = v[ vertex1 ].x;
      const double y1 = v[ vertex1 ].y;
      const double triangleArea = x0 * y1 - x1 * y0;
      contourArea += triangleArea;
    }

    DEBUG2( fprintf( stderr, "hole = %d, contour area = %lf\n",
                     isHole, contourArea ); )

    /* Ensure correct vertex order (per ESRI spec, holes are CCW order): */

    if ( OR2( AND2(   isHole, contourArea < 0.0 ),
              AND2( ! isHole, contourArea > 0.0 ) ) ) {
      DEBUG2( fprintf( stderr, "reversingVertexList...\n" ); )
      reverseVertexList( polygon->contour + contour );
      contourArea = -contourArea;
    }

    CHECK( IMPLIES_ELSE( isHole, contourArea >= 0.0, contourArea <= 0.0 ) );
    polygonArea += -contourArea;
  }

  polygonArea *= 0.5;
  DEBUG2( fprintf( stderr, "polygon area (in square degrees) = %lf\n",
                   polygonArea ); )
  result = polygonArea > 0.0;
  POST0( IS_BOOL( result ) );
  DEBUG2( fprintf( stderr, "result = %d\n", result ); )
  return result;
}



/******************************************************************************
PURPOSE: reverseVertexList - Reverse the vertex order.
INPUTS:  gpc_vertex_list* vertex_list  Vertices to reverse.
OUTPUTS: gpc_vertex_list* vertex_list  Vertices in reversed order.
******************************************************************************/

void reverseVertexList( gpc_vertex_list* vertex_list ) {
  PRE03( vertex_list, vertex_list->vertex, vertex_list->num_vertices > 0 );
  int lower = 0;
  int upper = vertex_list->num_vertices - 1;
  gpc_vertex* const vertices = vertex_list->vertex;

  for ( lower = 0; lower < upper; ++lower, --upper ) {
    const double temp_x = vertices[ lower ].x;
    const double temp_y = vertices[ lower ].y;
    vertices[ lower ].x = vertices[ upper ].x;
    vertices[ lower ].y = vertices[ upper ].y;
    vertices[ upper ].x = temp_x;
    vertices[ upper ].y = temp_y;
  }
}



/******************************************************************************
PURPOSE: signedAreaOfPolygon - Signed area of a single contour of a polygon.
INPUTS:  const gpc_vertex_list* vertex_list  Vertex list to compute area of.
RETURNS: double signed area of polygon.
         Negative if vertices are in clockwise order.
NOTES:   http://mathworld.wolfram.com/PolygonArea.html
******************************************************************************/

double signedAreaOfPolygon( const gpc_vertex_list* vertex_list ) {
  PRE03( vertex_list, vertex_list->num_vertices > 3, vertex_list->vertex );
  double result = 0.0;
  const int count = vertex_list->num_vertices;
  const gpc_vertex* const vertices = vertex_list->vertex;
  int index = 0;

  for ( index = 0; index < count; ++index ) {
    const int indexp1 = index + 1;
    const int index1 = indexp1 < count ? indexp1 : 0;
    const double triangleArea =
      vertices[ index  ].x * vertices[ index1 ].y -
      vertices[ index1 ].x * vertices[ index  ].y;
    result += triangleArea;
  }

  result *= 0.5;
  return result;
}



/******************************************************************************
PURPOSE: writePolygonsToShapefile - Write clipped polygons to Shapefile
         (shx, shp).
INPUTS:  SHPHandle outputFile                  File to write to.
         int isPolyline                        Is shapefile polyline?
         int count                             Number of shapes.
         const PolygonShape polygons[ count ]  Shapes to write.
RETURNS: int 1 if successful, else 0 failure message is printed.
NOTES:   Uses external shapelib API and GPC API polygon data type.
******************************************************************************/

int writePolygonsToShapefile( SHPHandle outputFile, int isPolyline,
                              int count, const PolygonShape polygons[] ) {

  PRE03( outputFile, count > 0, polygons );

  int result = 0;
  const int maximumParts = maximumPolygonContours( count, polygons );
  const int maximumVertices =
    maximumPolygonVertices( count, polygons ) + maximumParts;
  double* x = NEW( double, maximumVertices * 2 );
  int* starts = maximumParts == 1 ? 0 : NEW( int, maximumParts );

  if ( AND2( x, IMPLIES( maximumParts > 1, starts ) ) ) {
    double* const y = x + maximumVertices;
    const int type = isPolyline ? SHPT_ARC : SHPT_POLYGON;
    int index = 0;

    for ( index = 0; index < count; ++index ) {
      const gpc_polygon polygon = polygons[ index ].polygon;
      const int parts = polygon.num_contours;
      SHPObject* shape = 0;
      CHECK( parts >= 1 );

      if ( parts == 1 ) {
        const int vertices = polygon.contour->num_vertices + ! isPolyline;
        CHECK( ! polygon.hole[ 0 ] ); /* Don't allow single holes. */
        copyPolygonVertices( &polygon, ! isPolyline, 0, x, y );
        shape = SHPCreateSimpleObject( type, vertices, x, y, 0 );
      } else {
        const int vertices =
          polygonVertexCount( &polygon ) + parts * ( ! isPolyline );
        copyPolygonVertices( &polygon, ! isPolyline, starts, x, y );
        shape = SHPCreateObject( type, index, parts, starts,
                                 0, vertices, x, y, 0, 0 );
      }

      if ( shape ) {
        DEBUG( const int entityNumber = )
        SHPWriteObject( outputFile, -1, shape );
        DEBUG( fprintf( stderr,
                        "wrote shape # %d with %d parts and %d vertices\n",
                        entityNumber, shape->nParts, shape->nVertices ); )
        DEBUG( printShape( shape ); )
        SHPDestroyObject( shape ), shape = 0;
      } else {
        fprintf( stderr, "\nFailed to create shape.\n" );
        index = count; /* Stop looping. */
      }
    }

    result = index == count;
  }

  FREE( x );
  FREE( starts );

  DEBUG( fprintf(stderr, "writePolygonsToShapefile() returning %d\n", result);)
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: copySubsetShapefile - Copy masked subset of polygons to Shapefile
         (shx, shp).
INPUTS:  SHPHandle inputFile       File to read from.
         SHPHandle outputFile      File to write to.
         const int count           Number of shapes.
         const char mask[ count ]  If mask[i] == 1 write shape i else skip.
RETURNS: int number of subset shapes written if successful,
         else 0 failure message is printed.
NOTES:   Uses external shapelib API.
******************************************************************************/

int copySubsetShapefile( SHPHandle inputFile, SHPHandle outputFile,
                         const int count, const char* mask ) {
  PRE04( inputFile, outputFile, count > 0, mask );

  int result = 0;
  int index = 0;
  DEBUG( fprintf( stderr, "copySubsetShapefile( count = %d, mask = %p )\n",
                 count, mask ); )

  for ( index = 0; index < count; ++index ) {
    const char m = mask[ index ];

    if ( m ) {
      SHPObject* shape = SHPReadObject( inputFile, index );

      if ( ! shape ) {
        fprintf( stderr, "\nFailed to read shape #%d.\n", result );
        index = count; /* Stop looping. */
        result = 0;
      } else {

        if ( SHPWriteObject( outputFile, result, shape ) >= 0 ) {
          ++result;
        } else {
          fprintf( stderr, "\nFailed to write shape #%d.\n", result );
          index = count; /* Stop looping. */
          result = 0;
        }

        SHPDestroyObject( shape );
        shape = 0;
      }
    }
  }

  DEBUG( fprintf( stderr, "copySubsetShapefile() returning %d\n", result ); )
  POST0( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: writePolygonDBF - Write subset of DBF file for clipped polygons.
INPUTS:  const char* inputFileName        File name of input dbf.
         DBFHandle outputFile             DBF file to write to.
         int offset                       Record offset to starting writing.
         int count                        Number of shapes.
         const char* mask                 0 or mask (0,1) to filter shapes by.
         const PolygonShape polygons[ count ]  Shapes to write.
RETURNS: int 1 if successful, else 0 failure message is printed.
NOTES:   Uses static file-global table[] at the top of this file and
         external shapelib API.
******************************************************************************/

int writePolygonDBF( const char* inputFileName, DBFHandle outputFile,
                     int offset, int count, const char* mask,
                     const PolygonShape polygons[] ) {

  PRE06( inputFileName, *inputFileName, outputFile, offset >= 0, count > 0,
         polygons );

  int result = 0;
  DBFHandle inputFile = DBFOpen( inputFileName, "rb" );

  if ( inputFile ) {
    const double F_MISSING = -9999.0;
    int outputIndex = -1; /* Index into table of first column info. */
    int longitudeColumn = -1;
    const int outputColumns =
      defineDBFColumns( inputFileName, offset == 0,
                        &outputIndex, &longitudeColumn, 0, 0, outputFile );
    int ok = outputColumns > 0;
    int index = 0;
    int outputCount = 0;

    /* For each row/shape: */

    for ( index = 0; AND2( ok, index < count ); ++index ) {
      const int id = polygons[ index ].id; /* Input record/row. */

      if ( OR2( mask == 0, mask[ id ] ) ) {
        const int record = offset + outputCount; /* Output record/row. */
        int outputColumn = 0;
        ++outputCount;

        /* Write each column value in order shown in file global table: */

        for ( outputColumn = 0; AND2( ok, outputColumn < outputColumns );
              ++outputColumn ) {
          const ColumnEntry* const columnEntry =
            table + outputIndex + outputColumn;
          const int inputColumn = columnEntry->inputColumn;
          const char* const columnName = columnEntry->columnName;

          if ( inputColumn > -1 ) { /* Just map input value to output value: */
            const int columnType = columnEntry ->columnType;

            if ( columnType == FTDouble ) {
              const int filterNegatives = inputColumn != longitudeColumn;
              ok = copyDoubleAttribute( inputFile, id, inputColumn,
                                        outputFile, record, outputColumn,
                                        filterNegatives, F_MISSING,
                                        columnEntry->offset,
                                        columnEntry->scale );
            } else if ( columnType == FTInteger ) {
              ok = copyIntegerAttribute( inputFile, id, inputColumn,
                                         outputFile, record, outputColumn );
            } else {
              CHECK( columnType == FTString );
              ok = copyStringAttribute( inputFile, id, inputColumn,
                                        outputFile, record, outputColumn );
            }
          } else if ( ! strcmp( columnName, "YYYYDDD1" ) ) { /* HMS smoke: */

            /*
             * Start / End timestamp may be either "HHMM" or "YYYYDDD HHMM".
             * If only HHMM then parse YYYYMMDD from file name and
             * convert it to YYYYDDD. Then write column as an integer.
             */

            const char* const timestamp =
              DBFReadStringAttribute( inputFile, id, 1 );
            int yyyyddd = 0;
            int hhmm = 0;
            ok = convertTimestamp( inputFileName, timestamp, &yyyyddd, &hhmm );

            if ( ok ) {
              ok = DBFWriteIntegerAttribute( outputFile, record, outputColumn,
                                             yyyyddd );
            }
          } else if ( ! strcmp( columnName, "HHMM1" ) ) { /* HMS smoke: */
            const char* const timestamp =
              DBFReadStringAttribute( inputFile, id, 1 );
            int yyyyddd = 0;
            int hhmm = 0;
            ok = convertTimestamp( inputFileName, timestamp, &yyyyddd, &hhmm );

            if ( ok ) {
              ok = DBFWriteIntegerAttribute( outputFile, record, outputColumn,
                                             hhmm );
            }
          } else if ( ! strcmp( columnName, "YYYYDDD2" ) ) { /* HMS smoke: */
            const char* const timestamp =
              DBFReadStringAttribute( inputFile, id, 2 );
            int yyyyddd = 0;
            int hhmm = 0;
            ok = convertTimestamp( inputFileName, timestamp, &yyyyddd, &hhmm );

            if ( ok ) {
              ok = DBFWriteIntegerAttribute( outputFile, record, outputColumn,
                                             yyyyddd );
            }
          } else if ( ! strcmp( columnName, "HHMM2" ) ) { /* HMS smoke: */
            const char* const timestamp =
              DBFReadStringAttribute( inputFile, id, 2 );
            int yyyyddd = 0;
            int hhmm = 0;
            ok = convertTimestamp( inputFileName, timestamp, &yyyyddd, &hhmm );

            if ( ok ) {
              ok = DBFWriteIntegerAttribute( outputFile, record, outputColumn,
                                             hhmm );
            }
          }

          if ( ! ok ) {
            fprintf( stderr, "Failed to write row %d column %d to dbf file.\n",
                     record, outputColumn );
          }
        }
      }
    }

    result = ok;
    DBFClose( inputFile ), inputFile = 0;
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to write to dbf file.\n" );
  }

  DEBUG2( fprintf( stderr, "writePolygonDBF() returning %d\n", result ); )
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: writePointSubsetDBF - Write subset of DBF file for points in bounds.
INPUTS:  const char* inputFileName  File name of input dbf.
         const Bounds          Bounds to subset rows of point multi-data DBF.
         const long long huc   Or huc to filter by (if > 0).
         DBFHandle outputFile  DBF file to write to.
OUTPUTS: char** mask           If not 0, allocate and initialize array of
                               shape indices in subset.
RETURNS: int > 0 if successful (if mask then return length),
         else 0 failure message is printed.
NOTES:   Uses static file-global table[] at the top of this file and
         external shapelib API.
******************************************************************************/

int writePointSubsetDBF( const char* inputFileName, const Bounds bounds,
                         const long long huc, char** mask,
                         DBFHandle outputFile ) {

  PRE05( inputFileName,
         *inputFileName,
         isValidBounds( (const double (*)[2]) bounds ),
         huc >= 0,
         outputFile );

  int result = 0;
  DBFHandle inputFile = DBFOpen( inputFileName, "rb" );

  if ( inputFile ) {
    const double longitudeMinimum = bounds[ LONGITUDE ][ MINIMUM ];
    const double longitudeMaximum = bounds[ LONGITUDE ][ MAXIMUM ];
    const double latitudeMinimum  = bounds[ LATITUDE  ][ MINIMUM ];
    const double latitudeMaximum  = bounds[ LATITUDE  ][ MAXIMUM ];
    const double F_MISSING = -9999.0;
    const int inputRecords = DBFGetRecordCount( inputFile );
    int inputRecord = 0;
    int outputRecord = 0;
    int outputIndex = -1;
    int longitudeColumn = -1;
    int latitudeColumn = -1;
    int hucColumn = -1;
    const int outputColumns =
      defineDBFColumns( inputFileName, 1,
                        &outputIndex, &longitudeColumn, &latitudeColumn,
                        huc > 0 ? &hucColumn : 0, outputFile );
    int ok = outputColumns > 0;

    DEBUG( fprintf( stderr, "huc = %lld, mask = %p, "
                    "table index = %d, outputColumns = %d, "
                    "longitudeColumnn = %d, latitudeColumn = %d, "
                    "hucColumn = %d\n",
                    huc, mask, outputIndex, outputColumns,
                    longitudeColumn, latitudeColumn, hucColumn ); )

    CHECK( inputRecords > 0 );

    if ( mask ) {
      *mask = NEW( char, inputRecords );
      ok = *mask != 0;
      DEBUG( fprintf( stderr, "%p = *mask = NEW( char, inputRecords = %d )\n",
                      *mask, inputRecords ); )
    }

    /* Read rows filtered by bounds: */

    for ( inputRecord = 0; AND2( ok, inputRecord < inputRecords );
          ++inputRecord ) {
      const double longitude =
        DBFReadDoubleAttribute( inputFile, inputRecord, longitudeColumn );
      const double latitude =
        DBFReadDoubleAttribute( inputFile, inputRecord, latitudeColumn );
      const long long hucId =
        hucColumn < 0 ? 0
        : (long long) DBFReadDoubleAttribute(inputFile,inputRecord,hucColumn);
      const int inSubset = huc > 0 ? hucId == huc
        : AND2( IN_RANGE( longitude, longitudeMinimum, longitudeMaximum ),
                IN_RANGE( latitude,  latitudeMinimum,  latitudeMaximum  ) );

      if ( inSubset ) {
        int outputColumn = 0;

        if ( mask ) {
          CHECK( *mask );
          CHECK( inputRecords > 0 );
          CHECK( IN_RANGE( inputRecord, 0, inputRecords - 1 ) );
          (*mask)[ inputRecord ] = 1;
        }

        /* Write each column value in order shown in table at top of file: */

        for ( outputColumn = 0; AND2( ok, outputColumn < outputColumns );
              ++outputColumn ) {
          const ColumnEntry* const columnEntry =
            table + outputIndex + outputColumn;
          const int inputColumn = columnEntry->inputColumn;
          DEBUG( const char* const columnName = columnEntry->columnName; )
          const int columnType = columnEntry->columnType;
          CHECK( inputColumn > -1 );

          if ( columnType == FTDouble ) {
            const int filterNegatives = inputColumn != longitudeColumn;
            const double offset = columnEntry ->offset;
            const double scale  = columnEntry ->scale;

            if ( OR2( offset != 0.0, scale != 1.0 ) ) {
              double value =
                DBFReadDoubleAttribute( inputFile, inputRecord, inputColumn );

              if ( AND2( filterNegatives, value < 0.0 ) ) {
                value = F_MISSING;
              } else {
                value += offset;
                value *= scale;
              }

              ok = DBFWriteDoubleAttribute( outputFile,
                                            outputRecord, outputColumn,
                                            value );
            } else {
              ok = copyDoubleAttribute( inputFile, inputRecord, inputColumn,
                                        outputFile, outputRecord, outputColumn,
                                        filterNegatives, F_MISSING,
                                        columnEntry->offset,
                                        columnEntry->scale );
            }
          } else if ( columnType == FTInteger ) {
            ok = copyIntegerAttribute( inputFile, inputRecord, inputColumn,
                                       outputFile, outputRecord, outputColumn);
          } else {
            CHECK( columnType == FTString );
            ok = copyStringAttribute( inputFile, inputRecord, inputColumn,
                                      outputFile, outputRecord, outputColumn );
          }

          DEBUG( if ( ! ok ) fprintf( stderr, "Table entry %d %s\n",
                                      outputIndex, columnName ); )

        } /* End loop on output columns. */

        ++outputRecord;
      } /* End if point is in bounds. */
    } /* End loop on input records. */

    result = ok;
    DBFClose( inputFile ), inputFile = 0;

    if ( AND2( ok, mask ) ) {
      result = inputRecords;
    }
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to write to dbf file.\n" );
  }

  DEBUG2( fprintf( stderr, "writePointSubsetDBF() returning %d\n", result ); )
  POST0( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: writeShapeData - Write point multi-data ShapeData to DBF and SHP.
INPUTS:  const char* fileName        Pathed base file name to create.
         const ShapeData* shapeData  Data to write.
RETURNS: int 1 if successful, else 0 failure message is printed.
NOTES:   Uses external shapelib API.
******************************************************************************/

int writeShapeData( const char* fileName, const ShapeData* shapeData ) {

  PRE03( fileName, *fileName, isValidShapeData( shapeData ) );

  int result = 0;
  const int rows = shapeData->rows;
  const int columns = shapeData->columns;
  const Value* const values = shapeData->values;
  DBFHandle outputFile = DBFCreate( fileName );
  DEBUG( fprintf( stderr, "writeShapeData( %s, %p )\n", fileName, shapeData );)

  if ( outputFile ) {
    int outputIndex = -1;
    int ok = defineDBFColumns( fileName, 1, &outputIndex, 0,0,0, outputFile );
    int row = 0;

    /* Write rows: */

    for ( row = 0; AND2( ok, row < rows ); ++row ) {
      const int rowOffset = row * columns;
      const Value* const rowValues = values + rowOffset;
      int column = 0;

      for ( column = 0; AND2( ok, column < columns ); ++column ) {
        const int type = shapeData->columnTypes[ column ];

        if ( type == FTDouble ) {
          ok = AND2( ok,
                     DBFWriteDoubleAttribute( outputFile, row, column,
                                              rowValues[ column ].d ) );
        } else if ( type == FTInteger ) {
          ok = AND2( ok,
                     DBFWriteIntegerAttribute( outputFile, row, column,
                                               rowValues[ column ].i ) );
        } else {
          CHECK( type == FTString );
          ok = AND2( ok,
                     DBFWriteStringAttribute( outputFile, row, column,
                                              rowValues[ column ].s ) );
        }
      }
    }

    result = ok;
    DBFClose( outputFile ), outputFile = 0;
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to write to dbf file.\n" );
  } else { /* Write longitude-latitude coordinates to shp file: */
    SHPHandle outputFile = SHPCreate( fileName, SHPT_POINT );
    int ok = outputFile != 0;

    if ( outputFile ) {
      const int longitudeColumn =
        indexOfString( "LONGITUDE", (const char**) shapeData->columnNames,
                       columns );
      const int latitudeColumn =
        indexOfString( "LATITUDE", (const char**) shapeData->columnNames,
                        columns );
      int row = 0;

      /* Write longitude latitude coordinates for each row: */

      for ( row = 0; AND2( ok, row < rows ); ++row ) {
        const int rowOffset = row * columns;
        const Value* const rowValues = values + rowOffset;
        double longitude = rowValues[ longitudeColumn ].d;
        double latitude  = rowValues[ latitudeColumn  ].d;
        SHPObject* object =
          SHPCreateSimpleObject( SHPT_POINT, 1, &longitude, &latitude, 0 );
        ok = object != 0;

        if ( ! object ) {
          fprintf( stderr, "Failed to create SHP object.\n" );
        } else {
          ok = SHPWriteObject( outputFile, -1, object ) >= 0;

          if ( ! ok ) {
            fprintf( stderr, "Failed to write SHP object.\n" );
          }

          SHPDestroyObject( object ), object = 0;
        }
      }

      SHPClose( outputFile ), outputFile = 0;
    }

    result = ok;

    if ( ! result ) {
      fprintf( stderr, "Failed to write to shp file.\n" );
    }
  }

  if ( result ) {
    result = writePRJFile( fileName, 0 );
  }

  DEBUG2( fprintf( stderr, "writeShapeData() returning %d\n", result ); )
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: writeBoundsDBF - Write DBF file for bounds.
INPUTS:  const char* fileName                 File name of output dbf.
         const double areaInSquareKilometers  Area to write to DBF.
RETURNS: int 1 if successful, else 0 failure message is printed.
NOTES:   Uses external shapelib API.
******************************************************************************/

int writeBoundsDBF(const char* fileName, const double areaInSquareKilometers) {

  PRE03( fileName, *fileName, IN_RANGE( areaInSquareKilometers, 0.001, 1e+10));

  int result = 0;
  DBFHandle file = DBFCreate( fileName );

  if ( file ) {
    result = DBFAddField( file, "AREA_SQKM", FTDouble, 11, 3 ) != -1;

    if ( result ) {
      result = DBFWriteDoubleAttribute( file, 0, 0, areaInSquareKilometers );
    }

    DBFClose( file ), file = 0;
  }

  if ( ! result ) {
    fprintf( stderr, "Failed to write to dbf file.\n" );
  }

  DEBUG2( fprintf( stderr, "writeBoundsDBF() returning %d\n", result ); )
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: streamShapefiles - Stream shx, shp, dbf files to stdout preceeded
         by a one line ASCII header listing the (unpathed) base file name and
         sizes in bytes of the three files.
INPUTS:  const char* baseFileName  Base (no ext) name of shx/shp/dbf files.
         const char* name          Name to use in header.
         const int dbfOnly         Only process DBF file (not shx/p)?
         const int csv             Stream csv file too?
RETURNS: int 1 if successful, else 0.
******************************************************************************/

int streamShapefiles( const char* baseFileName, const char* name,
                      const int dbfOnly, const int csv ) {
  PRE06( baseFileName, *baseFileName, name, *name,
         IS_BOOL( dbfOnly ), IS_BOOL( csv ) );
  int result = 0;
  char shxFileName[ MAXIMUM_FILE_NAME_LENGTH + 1 ] = "";
  char shpFileName[ MAXIMUM_FILE_NAME_LENGTH + 1 ] = "";
  char dbfFileName[ MAXIMUM_FILE_NAME_LENGTH + 1 ] = "";
  char csvFileName[ MAXIMUM_FILE_NAME_LENGTH + 1 ] = "";
  size_t shxBytes = 0;
  size_t shpBytes = 0;
  size_t dbfBytes = 0;
  size_t csvBytes = 0;
  memset( shxFileName, 0, sizeof shxFileName );
  memset( shpFileName, 0, sizeof shpFileName );
  memset( dbfFileName, 0, sizeof dbfFileName );
  memset( csvFileName, 0, sizeof csvFileName );
  snprintf( shxFileName, sizeof shxFileName / sizeof *shxFileName,
            "%s.shx", baseFileName );
  snprintf( shpFileName, sizeof shpFileName / sizeof *shpFileName,
            "%s.shp", baseFileName );
  snprintf( dbfFileName, sizeof dbfFileName / sizeof *dbfFileName,
            "%s.dbf", baseFileName );
  snprintf( csvFileName, sizeof csvFileName / sizeof *csvFileName,
            "%s.csv", baseFileName );
  CHECK( shxFileName[ MAXIMUM_FILE_NAME_LENGTH - 1 ] == '\0' );
  CHECK( shpFileName[ MAXIMUM_FILE_NAME_LENGTH - 1 ] == '\0' );
  CHECK( dbfFileName[ MAXIMUM_FILE_NAME_LENGTH - 1 ] == '\0' );
  CHECK( csvFileName[ MAXIMUM_FILE_NAME_LENGTH - 1 ] == '\0' );

  if ( ! dbfOnly ) {
    shxBytes = fileSize( shxFileName );
    shpBytes = fileSize( shpFileName );
  }

  dbfBytes = fileSize( dbfFileName );

  if ( csv ) {
    csvBytes = fileSize( csvFileName );
  }

  if ( dbfOnly ) {

    if ( csv ) {
      printf( "%s 0 0 %lu %lu\n", name, dbfBytes, csvBytes );
      result = streamFile( dbfFileName );
      result = AND2( result, streamFile( csvFileName ) );
    } else {
      printf( "%s 0 0 %lu\n", name, dbfBytes );
      result = streamFile( dbfFileName );
    }
  } else {

    if ( csv ) {
      printf( "%s %lu %lu %lu %lu\n",
              name, shxBytes, shpBytes, dbfBytes, csvBytes );
      result = streamFile( shxFileName );
      result = AND2( result, streamFile( shpFileName ) );
      result = AND2( result, streamFile( dbfFileName ) );
      result = AND2( result, streamFile( csvFileName ) );
    } else {
      printf( "%s %lu %lu %lu\n", name, shxBytes, shpBytes, dbfBytes );
      result = streamFile( shxFileName );
      result = AND2( result, streamFile( shpFileName ) );
      result = AND2( result, streamFile( dbfFileName ) );
    }
  }

  DEBUG( fprintf( stderr, "streamFiles() returning %d\n", result ); )
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: removeShapefiles - Remove set of temporary shx, shp, dbf files.
INPUTS:  const char* baseFileName  Base (no ext) file name of files to remove.
         int dbfOnly               Only process DBF file?
******************************************************************************/

void removeShapefiles( const char* baseFileName, int dbfOnly ) {
  PRE03( baseFileName, *baseFileName, IS_BOOL( dbfOnly ) );
  char outputFileName[ MAXIMUM_FILE_NAME_LENGTH + 1 ] = "";
  memset( outputFileName, 0, sizeof outputFileName );

  if ( ! dbfOnly ) {
    snprintf( outputFileName, sizeof outputFileName / sizeof *outputFileName,
              "%s.shx", baseFileName );
    CHECK( outputFileName[ MAXIMUM_FILE_NAME_LENGTH - 1 ] == '\0' );
    unlink( outputFileName );
    snprintf( outputFileName, sizeof outputFileName / sizeof *outputFileName,
              "%s.shp", baseFileName );
    CHECK( outputFileName[ MAXIMUM_FILE_NAME_LENGTH - 1 ] == '\0' );
    unlink( outputFileName );
  }

  snprintf( outputFileName, sizeof outputFileName / sizeof *outputFileName,
            "%s.dbf", baseFileName );
  CHECK( outputFileName[ MAXIMUM_FILE_NAME_LENGTH - 1 ] == '\0' );
  unlink( outputFileName );
}



/******************************************************************************
PURPOSE: deallocateShapeData - Deallocate storage of a ShapeData.
INPUTS:  ShapeData* shapeData  Shape data to deallocate.
OUTPUTS: ShapeData* shapeData  Deallocated shapeData.
******************************************************************************/

void deallocateShapeData( ShapeData* shapeData ) {

  if ( shapeData ) {

    if ( shapeData->stringStorage ) {
      const int capacity = shapeData->capacity;
      int index = 0;

      for ( index = 0; index < capacity; ++index ) {
        FREE( shapeData->stringStorage[ index ] );
      }
    }

    FREE( shapeData->stringStorage );
    FREE( shapeData->columnTypes );
    FREE( shapeData->values );
    memset( shapeData, 0, sizeof *shapeData );
    FREE( shapeData );
  }

  POST0( shapeData == 0 );
}



/******************************************************************************
PURPOSE: copyShapeData - Allocate and copy a ShapeData.
INPUTS:  const ShapeData* shapeData  Shape data to copy.
RETURNS: ShapeData* Allocated and copied shapeData.
******************************************************************************/

ShapeData* copyShapeData( const ShapeData* const shapeData ) {
  PRE0( isValidShapeData( shapeData ) );
  ShapeData* result = NEW( ShapeData, 1 );
  int ok = result != 0;

  if ( ok ) {
    const int rows = shapeData->rows;
    const int columns = shapeData->columns;
    int column = 0;
    result->values = NEW( Value, rows * columns );
    ok = result->values != 0;

    if ( ok ) {
      result->columnTypes = NEW( int, columns );
      ok = result->columnTypes != 0;

      if ( ok ) {
        const int capacity = shapeData->capacity;
        result->columnNames = result->stringStorage = NEW( char*, capacity );
        ok = result->stringStorage != 0;

        if ( ok ) {
          int index = 0;
          result->capacity = capacity;

          for ( index = 0;
                AND3( ok, index < capacity, shapeData->stringStorage[ index ] );
                ++index ) {
            result->stringStorage[ index ] =
              strdup( shapeData->stringStorage[ index ] );
            ok = result->stringStorage[ index ] != 0;
          }

          if ( ok ) {
            result->rows    = rows;
            result->columns = columns;

            for ( column = 0; column < columns; ++column ) {
              const int columnType = shapeData->columnTypes[ column ];
              int row = 0;
              result->columnTypes[ column ] = columnType;

              for ( row = 0; row < rows; ++row ) {
                index = row * columns + column;
                CHECK( IN_RANGE( index, 0, rows * columns - 1 ) );

                if ( columnType == FTInteger ) {
                  result->values[ index ].i = shapeData->values[ index ].i;
                } else if ( columnType == FTDouble ) {
                  result->values[ index ].d = shapeData->values[ index ].d;
                } else {

                  /* Set string Value to point into stringStorage: */

                  for ( index = 0; index < capacity; ++index ) {
                    const int found =
                      ! strcmp( shapeData->values[ index ].s,
                                result->stringStorage[ index ] );

                    if ( found ) {
                      result->values[ index ].s = result->stringStorage[ index];
                      index = capacity; /* Stop looping. */
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  POST0( IMPLIES( result, equalShapeData( result, shapeData ) ) );
  return result;
}



/******************************************************************************
PURPOSE: equalShapeData - Are ShapeData objects equal?.
INPUTS:  const ShapeData* const shapeData1  Shape data to compare.
         const ShapeData* const shapeData2  Shape data to compare.
RETURNS: int 1 if equal else 0.
******************************************************************************/

int equalShapeData( const ShapeData* const shapeData1,
                    const ShapeData* const shapeData2 ) {
  PRE02( isValidShapeData( shapeData1 ), isValidShapeData( shapeData2 ) );
  const int rows = shapeData1->rows;
  const int columns = shapeData1->columns;
  int result = AND2( rows == shapeData2->rows, columns == shapeData2->columns);
  int row = 0;
  int column = 0;
  int index = 0;

  /* Check column types and names (fast): */

  for ( column = 0; AND2( result, column < columns ); ++column ) {
    const int columnType = shapeData1->columnTypes[ column ];
    result =
      AND2( columnType == shapeData2->columnTypes[ column ],
            ! strcmp( shapeData1->columnNames[ column ],
                      shapeData2->columnNames[ column ] ) );
  }

  /* Check values only if needed (slow): */

  for ( row = 0, index = 0; AND2( result, row < rows ); ++row ) {

    for ( column = 0; AND2( result, column < columns ); ++column, ++index ) {
      const int columnType = shapeData1->columnTypes[ column ];

      if ( columnType == FTInteger ) {
        result = shapeData1->values[ index ].i == shapeData2->values[ index ].i;
      } else if ( columnType == FTDouble ) {
        result = shapeData1->values[ index ].d == shapeData2->values[ index ].d;
      } else {
        result =
          ! strcmp( shapeData1->values[ index ].s,
                    shapeData2->values[ index ].s );
      }
    }
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: printShapeData - Print a ShapeData (for tracing/debugging).
INPUTS:  const ShapeData* shapeData  Shape data to print.
******************************************************************************/

void printShapeData( const ShapeData* shapeData ) {

  if ( shapeData ) {
    const int rows = shapeData->rows;
    const int columns = shapeData->columns;
    int row = 0;
    int column = 0;

    fprintf( stderr, "shapeData: rows = %d, columns = %d, capacity = %d\n",
             shapeData->rows, shapeData->columns, shapeData->capacity );
    CHECK3( shapeData->columns <= shapeData->capacity,
            shapeData->columnNames, shapeData->columnNames[ 0 ] );

    for ( column = 0; column < columns; ++column ) {
      fprintf( stderr, "%s\t", shapeData->columnNames[ column ] );
    }

    fprintf( stderr, "\n" );

    for ( column = 0; column < columns; ++column ) {
      const char* const typeNames[ 3 ] =
        { "FTString", "FTInteger", "FTDouble" };
      const int type = shapeData->columnTypes[ column ];
      CHECK( IN4( type, FTString,  FTInteger , FTDouble ) );
      fprintf( stderr, "%s\t", typeNames[ type ] );
    }

    fprintf( stderr, "\n" );

    for ( row = 0; row < rows; ++row ) {
      const int rowOffset = row * columns;

      for ( column = 0; column < columns; ++column ) {
        const int index = rowOffset + column;

        switch ( shapeData->columnTypes[ column ] ) {
        case FTString:
          fprintf( stderr, "%s\t", shapeData->values[ index ].s );
          break;
        case FTInteger:
          fprintf( stderr, "%d\t", shapeData->values[ index ].i );
          break;
        default:
          CHECK( shapeData->columnTypes[ column ] == FTDouble );
          fprintf( stderr, "%lg\t", shapeData->values[ index ].d );
          break;
        }
      }

      fprintf( stderr, "\n" );
    }
  }
}



/******************************************************************************
PURPOSE: isValidShapeData - Validate a ShapeData.
INPUTS:  const ShapeData* shapeData  Shape data to validate.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidShapeData( const ShapeData* shapeData ) {
  int result =
    AND9( shapeData,
          shapeData->rows > 0,
          shapeData->columns > 0,
          shapeData->rows * shapeData->columns > 0,
          shapeData->stringStorage,
          shapeData->columnNames == shapeData->stringStorage,
          shapeData->columnTypes,
          shapeData->values,
          shapeData->capacity >= shapeData->columns );

  if ( result ) {
    const int rows = shapeData->rows;
    const int columns = shapeData->columns;
    int row = 0;
    int column = 0;

    for ( column = 0; AND2( result, column < columns ); ++column ) {
      const char* const columnName = shapeData->columnNames[ column ];
      result =
        AND2( IN4( shapeData->columnTypes[ column ],
                   FTString,  FTInteger , FTDouble ),
              isValidColumnName( columnName ) );
    }

    /* Check that column names are unique: */

    for ( column = 1; AND2( result, column < columns ); ++column ) {
      const char* const columnName = shapeData->columnNames[ column - 1 ];
      result =
        indexOfString( columnName, 
                       (const char* const *) shapeData->columnNames + column,
                       columns - column ) == -1;
    }

    /* Check data values: */

    for ( row = 0; AND2( result, row < rows ); ++row ) {
      const int rowOffset = row * columns;

      for ( column = 0; AND2( result, column < columns ); ++column ) {
        const int index = rowOffset + column;

        switch ( shapeData->columnTypes[ column ] ) {
        case FTString:
          result = shapeData->values[ index ].s != 0;
          break;
        case FTDouble:
          result = ! isNan( shapeData->values[ index ].d );
          break;
        default:
          break;
        }
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: isValidColumnName - Is DBF column name valid?
INPUTS:  const char* const columnName  Name to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidColumnName( const char* columnName ) {
  const char* c = columnName;
  int result = AND2( c, *c );

  while ( AND2( result, *c ) ) {
    result = AND2( isprint( *c ), ! isspace( *c ) );
    ++c;
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: isValidValue - Is Value valid - i.e., non-missing-valued?
INPUTS:  int type           Type portion to check.
         const char* units  Units of value, e.g., %.
         Value value        Value to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

int isValidValue( int type, const char* units, Value value ) {
  PRE0( IN4( type, FTString, FTInteger, FTDouble ) );
  const double validDoubleMinimum  = *units == '%' ? 0.0 : -98.0;
  const double validIntegerMinimum = *units == '%' ? 0   : -98;
  const int result =
    OR3( AND2( type == FTDouble,  value.d >= validDoubleMinimum ),
         AND2( type == FTInteger, value.i >= validIntegerMinimum ),
         AND3( type == FTString,  value.s, value.s[ 0 ] ) );

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: compareValues - Compare values for sorting order.
INPUTS:  Value value1  1st value to check.
         Value value2  2nd value to check.
RETURNS: int -1 if value1 < value2 else 1 if value1 > value2 else 0.
******************************************************************************/

int compareValues( int type, Value value1, Value value2 ) {
  PRE0( IN4( type, FTString, FTInteger, FTDouble ) );
  int result = 0; /* null == null. */

  if ( type == FTDouble ) {
    result = value1.d < value2.d ? -1 : value1.d > value2.d ? 1 : 0;
  } else if ( type == FTInteger ) {
    result = value1.i < value2.i ? -1 : value1.i > value2.i ? 1 : 0;
  } else {
    CHECK( type == FTString );

    if ( value1.s ) {

      if ( value2.s ) {
        result = strcmp( value1.s, value2.s );

        if ( result < 0 ) {
          result = -1;
        } else if ( result > 0 ) {
          result = 1;
        }
      } else {
        result = 1; /* non-null > null. */
      }
    } else if ( value2.s ) {
      result = -1; /* null < non-null. */
    }
  }

  POST0( IN4( result, -1, 0, 1 ) );
  return result;
}



/******************************************************************************
PURPOSE: readDBF - Read a DBF file.
INPUTS:  const char* fileName  Name of input dbf file to read.
RETURNS: ShapeData* if successful, else 0 and failure message is printed.
******************************************************************************/

ShapeData* readDBF( const char* fileName ) {
  PRE02( fileName, *fileName );
  ShapeData* result = 0;
  DBFHandle inputFile = DBFOpen( fileName, "rb" );
  int ok = 0;

  if ( inputFile ) {
    const int rows = DBFGetRecordCount( inputFile );

    if ( rows > 0 ) {
      const int columns = DBFGetFieldCount( inputFile );

      if ( columns > 0 ) {
        result = NEW( ShapeData, 1 );

        if ( result ) {
          result->rows = rows;
          result->columns = columns;
          result->columnTypes = NEW( int, columns );

          if ( result->columnTypes ) {
            const int estimatedUniqueStringCount = 1000;
            result->capacity = columns + estimatedUniqueStringCount;
            result->stringStorage = NEW( char*, result->capacity );
            result->columnNames = result->stringStorage;
            CHECK2( result->capacity >= columns,
                    result->columnNames == result->stringStorage );

            if ( result->stringStorage ) {
              result->values = NEW( Value, rows * columns );

              if ( result->values ) {
                int row = 0;
                int column = 0;
                ok = 1;

                /* Read header column names/types: */

                for ( column = 0; AND2( ok, column < columns ); ++column ) {
                  char columnName[ 16 ] = "";
                  const DBFFieldType type =
                    DBFGetFieldInfo( inputFile, column, columnName, 0, 0 );
                  ok = AND2( IN4( type, FTString, FTInteger, FTDouble ),
                             isValidColumnName( columnName ) );

                  if ( ok ) {
                    result->columnTypes[ column ] = type;
                    result->stringStorage[ column ] = strdup( columnName );
                    ok = result->stringStorage[ column ] != 0;

                    if ( ok ) {
                      DEBUG2( fprintf( stderr,
                              "result->stringStorage[ column = %d ] = '%s'\n",
                              column, result->stringStorage[ column ] ); )
                      CHECK( ! strcmp( result->stringStorage[ column ],
                                       columnName ) );
                    }
                  }
                }

                /* Read row data: */

                for ( row = 0; AND2( ok, row < rows ); ++row ) {
                  const int rowOffset = row * columns;

                  for ( column = 0; AND2( ok, column < columns ); ++column ) {
                    const int index = rowOffset + column;
                    ok = 0;

                    switch ( result->columnTypes[ column ] ) {
                    case FTString:
                      {
                        const char* stringValue =
                          DBFReadStringAttribute( inputFile, row, column );

                        if ( OR2( stringValue == 0, *stringValue == '\0' ) ) {
                          stringValue = "NULL";
                        }

                        result->values[ index ].s =
                          storeStringValue( stringValue, result );
                        ok = result->values[ index ].s != 0;
                      }
                      break;
                    case FTInteger:
                      result->values[ index ].i =
                        DBFReadIntegerAttribute( inputFile, row, column );
                      ok = 1;
                      break;
                    default:
                      CHECK( result->columnTypes[ column ] == FTDouble );
                      result->values[ index ].d =
                        DBFReadDoubleAttribute( inputFile, row, column );
                      ok = 1;
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    DBFClose( inputFile ), inputFile = 0;
  }

  if ( ! ok ) {
    fprintf( stderr, "Failed to read dbf file %s.\n", fileName );
    deallocateShapeData( result ), result = 0;
  }

  DEBUG2( printShapeData( result ); )
  POST0( IMPLIES( result, isValidShapeData( result ) ) );
  return result;
}



/******************************************************************************
PURPOSE: subsetDBFByTime - Subset ShapeData by timestamp range.
INPUTS:  const char* fileName  Base file name of DBF file to read.
         int yyyymmdd1         1st timestamp to filter by.
         int yyyymmdd2         2nd timestamp to filter by.
RETURNS: char* array of mask flags 1 or 0 for each row.
NOTES:   This is for filtering out rows/shapes in HMS smoke polygon data that
         are outside the subset time range.
******************************************************************************/

char* subsetDBFByTime( const char* fileName, int yyyymmdd1, int yyyymmdd2 ) {

  PRE05( fileName,
         *fileName,
        isValidYearMonthDay( yyyymmdd1 ),
        isValidYearMonthDay( yyyymmdd2 ),
        yyyymmdd1 <= yyyymmdd2 );

  char* result = 0;
  ShapeData* shapeData = readDBF( fileName );

  if ( shapeData ) {
    const int columns = shapeData->columns;
    const int startTimestampColumn =
      indexOfString( "Start",
                     (const char**) shapeData->columnNames, columns );
    const int endTimestampColumn =
      indexOfString( "End",
                     (const char**) shapeData->columnNames, columns );

    if ( AND4( startTimestampColumn != -1,
               endTimestampColumn != -1,
               shapeData->columnTypes[ startTimestampColumn ] == FTString,
               shapeData->columnTypes[ endTimestampColumn ] == FTString ) ) {
      const int yyyyddd1 = (int) ( convertYearMonthDay( yyyymmdd1 ) );
      const int yyyyddd2 = (int) ( convertYearMonthDay( yyyymmdd2 ) );
      const int rows = shapeData->rows;
      int row = 0;
      CHECK2( isValidDate( yyyyddd1 ), isValidDate( yyyyddd2 ) );
      result = (char*) malloc( rows * sizeof (char) );

      if ( ! result ) {
        fprintf( stderr, "\nFailed to allocate memory for subset mask.\n\n" );
      } else {
        int ok = 1;
        memset( result, 0, rows * sizeof (char) );

        for ( row = 0; AND2( ok, row < rows ); ++row ) {
          const int rowOffset = row * columns;
          const int index1 = rowOffset + startTimestampColumn;
          const int index2 = rowOffset + endTimestampColumn;
          const char* const startTimestamp = shapeData->values[ index1 ].s;
          const char* const endTimestamp   = shapeData->values[ index2 ].s;
          int yyyydddStart = 0;
          int yyyydddEnd = 0;
          int hhmm = 0;
          ok = convertTimestamp(fileName, startTimestamp, &yyyydddStart, &hhmm);

          if ( ok ) {
            ok = convertTimestamp( fileName, endTimestamp, &yyyydddEnd, &hhmm );

            if ( ok ) {
              const int outside =
                OR2( yyyydddStart > yyyyddd2, yyyydddEnd < yyyyddd1 );
              result[ row ] = ! outside;
            }
          }
        }

        if ( ! ok ) {
          fprintf( stderr,
                   "\nInvalid date-time data read from DBF file %s.\n\n",
                   fileName );
          free( result ), result = 0;
        }
      }
    }

    deallocateShapeData( shapeData ), shapeData = 0;
  }

  return result;
}



/******************************************************************************
PURPOSE: writeShapeDataToText - Write ShapeData to a tab-delimited text file.
INPUTS:  const ShapeData* shapeData  ShapeData to write.
         const char* outputFileName  Name of output txt file to create.
RETURNS: int 1 if successful, else 0 and failure message is printed.
******************************************************************************/

int writeShapeDataToText( const ShapeData* shapeData,
                          const char* outputFileName ) {

  PRE03( isValidShapeData( shapeData ), outputFileName, *outputFileName );

  int result = 0;
  FILE* outputFile = fopen( outputFileName, "w" );

  if ( ! outputFile ) {
    fprintf( stderr, "Failed to create file %s.\n", outputFileName );
  } else {
    const int rows = shapeData->rows;
    const int columns = shapeData->columns;
    const int columns_1 = columns - 1;
    int column = 0;
    int row = 0;

    for ( column = 0; column < columns_1; ++column ) {
      fprintf( outputFile, "%s\t", shapeData->columnNames[ column ] );
    }

    fprintf( outputFile, "%s\n", shapeData->columnNames[ columns_1 ] );

    for ( row = 0; row < rows; ++row ) {

      for ( column = 0; column < columns; ++column ) {
        const int type = shapeData->columnTypes[ column ];
        const char delimiter = column < columns - 1 ? '\t' : '\n';
        const int index = row * columns + column;

        if ( type == FTString ) {
          fprintf( outputFile, "%s%c",
                   shapeData->values[ index ].s, delimiter );
        } else if ( type == FTInteger ) {
          fprintf( outputFile, "%d%c",
                   shapeData->values[ index ].i, delimiter );
        } else {
          const double value = shapeData->values[ index ].d;
          const double absValue = fabs( value );
          const int useExp =
            AND2( value != 0.0, OR2( absValue < 1e-3, absValue > 1e6 ) );

          if ( useExp ) {
            fprintf( outputFile, "%le%c", value, delimiter );
          } else {
            fprintf( outputFile, "%lf%c", value, delimiter );
          }
        }
      }
    }

    fclose( outputFile ), outputFile = 0;
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/*============================= PRIVATE FUNCTIONS ===========================*/



/******************************************************************************
PURPOSE: writePRJFile - Write a .prj file.
INPUTS:  const char* fileName  Base name of file to create. "example".
         const int   useASCIIGridForm  1 = Write grid form, 0 = shape form.
NOTES:   Creates files fileName.prj.
         http://en.wikipedia.org/wiki/Well-known_text
******************************************************************************/

static int writePRJFile( const char* fileName, const int useASCIIGridForm ) {
  int result = 0;
  enum { FILE_NAME_LENGTH = 255 };
  char prjFileName[ FILE_NAME_LENGTH + 1 ] = "";
  memset( prjFileName, 0, sizeof prjFileName );
  strncpy( prjFileName, fileName, FILE_NAME_LENGTH - 4 );
  strcat( prjFileName, ".prj" );
  result = writeWGS84PRJFile( prjFileName, useASCIIGridForm );
  return result;
}



/******************************************************************************
PURPOSE: writeDataToDBFFile - Write location info to a dbf file and
         time-varying data to a csv file.
INPUTS:  const char* fileName  Base name of file to create. "IOOS".
         const char* variable  Name of data variable. E.g., "salinity".
         const char* units     Name of data units. E.g., "PSU".
         const int timesteps   Number of timesteps of data.
         const int yyyymmddhh[ timesteps ]  Timestamps of data.
         const int timestepType  HOURLY, DAILY, MONTHLY, YEARLY.
         const int count        Number of data rows, e.g., grid rows x columns.
         const int type         Data type: BYTE_TYPE, FLOAT_TYPE, etc.
         const int components   1 = scalar, 2 or 3 = vector.
         const void* data      data[ components * timesteps * count ]
                                           Component data at cells.
         const float lonlats[ count * 2 ]  Lon-lat of each row data point.
         const float z[ count ]            Depth(m) of each row data point or 0
         const char* const sids[ count ]   Station id strings or 0.
NOTES:   Creates files fileName.dbf containing only static location info:
         ID (int)    [Station (string)]  Longitude Latitude [Depth(m)]
         [] denotes optional (if available).
         ID is int in range [0, points - 1] index matching shapes in shp file.
         Station is string-id for the station, if station data, else omitted.
         Depth(m) is omitted if unavailable.
         Calls writeDataToCSVFile() to create the csf file.
         http://www.clicketyclick.dk/databases/xbase/format/dbf.html#DBF_STRUCT
         This routine does not use shapelib API since it was written before
         that library was used in this project and also because this routine
         is more straight-forward and much faster than using the shapelib API.
******************************************************************************/

static int writeDataToDBFFile( const char* fileName,
                               const char* variable, const char* units,
                               const int timesteps, const int yyyymmddhh[],
                               const int timestepType,
                               const int count, const int type,
                               const int components,
                               const void* data,
                               const float lonlats[], const float z[],
                               const char* const sids[] ) {

  PRE017( fileName,
          *fileName,
          variable,
          *variable,
          units,
          *units,
          timesteps > 0,
          yyyymmddhh,
          IS_VALID_TIMESTEP_TYPE( timestepType ),
          count > 0,
          IN_RANGE( type, 0, GRID_DATA_TYPES ),
          data,
          IN_RANGE( components, 1, 3 ),
          lonlats,
          isValidLongitudeLatitude( lonlats[ 0 ], lonlats[ 1 ] ),
          isValidLongitudeLatitude( lonlats[  count * 2 - 2 ],
                                       lonlats[  count * 2 - 1 ] ),
          IMPLIES( z, AND2( ! isNan( z[ 0 ] ), ! isNan( z[ count - 1 ] ))));

  int result = 0;
  enum { /* Constants from the Shapefile Spec (see above link): */
    HEADER_BYTES = 32,
    FIELD_DESCRIPTOR_BYTES = 32,
    INT_FIELD_WIDTH = 20,     /* For ID. */
    FIELD_WIDTH = 15,         /* For Longitude, Latitude, Depth. */
    FIELD_DECIMAL_DIGITS = 6, /* Number of digits to the right of the decimal*/
    STRING_FIELD_WIDTH = 31,  /* For Station from sids[]. */
    FILE_NAME_LENGTH = 255
  };
  const char* const stringFieldFormat = "%31s";
  const int fields = 1 + ( sids != 0 ) + 1 + 1 + ( z != 0 );
  /* ID Station Longitude Latitude Depth. */
  const int records = count;
  const int headerBytes = HEADER_BYTES + fields * FIELD_DESCRIPTOR_BYTES + 1;
  unsigned char* header = NEW( unsigned char, headerBytes + 1 );

  DEBUG( fprintf( stderr, "writeDataToDBFFile( fileName = '%s', "
                  "variable = '%s', units = '%s', "
                  "timesteps = %d, yyyymmddhh[] = [%d ... %d], "
                  "timestepType = %d, count = %d, type = %d, "
                  "components = %d, data = %p, "
                  "lonlats = %p, z = %p, sids = %p )\n",
                  fileName, variable, units,
                  timesteps, yyyymmddhh[ 0 ], yyyymmddhh[ timesteps - 1 ],
                  timestepType, count, type, components, data,
                  lonlats, z, sids ); )

  if ( header ) {
    int headerIndex = 32;
    FILE* file = 0;
    char dbfFileName[ FILE_NAME_LENGTH + 1 ] = "";
    memset( dbfFileName, 0, sizeof dbfFileName );
    strncpy( dbfFileName, fileName, FILE_NAME_LENGTH - 4 );
    strcat( dbfFileName, ".dbf" );

    /* Initialize header: */

    memset( header, 0, headerBytes );

    header[ 0 ] = 3;  /* dBASE Level 5. */
    header[ 1 ] = 95; /* Update timestamp: YY - 1900. */
    header[ 2 ] = 7;  /* Update timestamp: MM. */
    header[ 3 ] = 26; /* Update timestamp: DD. */
    writeInt( header, 4, count, LITTLE ); /* Number of records. */
    writeShort( header, 8, headerBytes, LITTLE ); /* Number of header bytes. */

    /* Length of each record: */

    {
      const int recordLength =
        INT_FIELD_WIDTH                        /* ID. */
        + ( sids != 0 ) * STRING_FIELD_WIDTH   /* Station. */
        + 2 * FIELD_WIDTH                      /* Longitude, Latitude. */
        + ( z != 0 ) * FIELD_WIDTH;            /* Depth. */

       writeShort( header, 10, recordLength, LITTLE );      
    }

    /* Array of Field Descriptors: */

    snprintf( (char*) header + headerIndex, 11, "%-10s", "ID" );
    header[ headerIndex + 11 ] = 'N';  /* Formatted number. */
    header[ headerIndex + 16 ] = INT_FIELD_WIDTH;
    header[ headerIndex + 17 ] = 0; /* No digits to the right of decimal. */
    headerIndex += FIELD_DESCRIPTOR_BYTES;

    if ( sids ) {
      snprintf( (char*) header + headerIndex, 11, "%-10s", "Station" );
      header[ headerIndex + 11 ] = 'C';  /* Character string. */
      header[ headerIndex + 16 ] = STRING_FIELD_WIDTH;
      header[ headerIndex + 17 ] = 0;
      headerIndex += FIELD_DESCRIPTOR_BYTES;
    }

    snprintf( (char*) header + headerIndex, 11, "%-10s", "Longitude" );
    header[ headerIndex + 11 ] = 'N';  /* Formatted real number. */
    header[ headerIndex + 16 ] = FIELD_WIDTH;
    header[ headerIndex + 17 ] = FIELD_DECIMAL_DIGITS;
    headerIndex += FIELD_DESCRIPTOR_BYTES;

    snprintf( (char*) header + headerIndex, 11, "%-10s", "Latitude" );
    header[ headerIndex + 11 ] = 'N';  /* Formatted real number. */
    header[ headerIndex + 16 ] = FIELD_WIDTH;
    header[ headerIndex + 17 ] = FIELD_DECIMAL_DIGITS;
    headerIndex += FIELD_DESCRIPTOR_BYTES;

    if ( z ) {
      snprintf( (char*) header + headerIndex, 11, "%-10s", "Depth(m)" );
      header[ headerIndex + 11 ] = 'N';  /* Formatted real number. */
      header[ headerIndex + 16 ] = FIELD_WIDTH;
      header[ headerIndex + 17 ] = FIELD_DECIMAL_DIGITS;
      headerIndex += FIELD_DESCRIPTOR_BYTES;
    }

    header[ headerIndex ] = 0x0D; /* Terminator. */

    file = fopen( dbfFileName, "wb" );

    if ( file ) {
      int record = 0;

      /* Write header bytes to DBF file: */

      result = fwrite( header, headerBytes, 1, file ) == 1;

      /* Write data rows (records = shapes in shp file) to DBF file: */

      for ( record = 0; AND2( result, record < records ); ++record ) {
        const int record2 = record + record;
        const int id = record;
        const char* const station = sids ? sids[ record ] : 0;
        const float longitude = lonlats[ record2 ];
        const float latitude  = lonlats[ record2 + 1 ];
        const float depth = z ? z[ record ] : 0.0;
        char format[ 32 ] = "";
        char formattedIntValue[ INT_FIELD_WIDTH + 1 ] = "";
        char formattedStringValue[ STRING_FIELD_WIDTH + 1 ] = "";
        char formattedValue[ FIELD_WIDTH + 1 ] = "";

        /* Write Id: */

        memset( format, 0, sizeof format );
        memset( formattedIntValue, 0, sizeof formattedIntValue );
        snprintf( format, sizeof format / sizeof *format,
                  "%%%dd", INT_FIELD_WIDTH );
        snprintf( formattedIntValue,
                  sizeof formattedIntValue / sizeof *formattedIntValue,
                  format, id );
        result = fwrite( formattedIntValue, INT_FIELD_WIDTH, 1, file ) == 1;

        if ( result ) {

          /* Write Station: */

          if ( station ) {
            memset( formattedStringValue, 0, sizeof formattedStringValue );
            snprintf( formattedStringValue,
                      sizeof  formattedStringValue /
                      sizeof *formattedStringValue,
                      stringFieldFormat, station );
            result =
              fwrite( formattedStringValue, STRING_FIELD_WIDTH, 1, file ) == 1;
          }

          /* Write Longitude: */

          if ( result ) {
            memset( format, 0, sizeof format );
            snprintf( format, sizeof format / sizeof *format,
                      "%%%d.%df", FIELD_WIDTH, FIELD_DECIMAL_DIGITS );
            memset( formattedValue, 0, sizeof formattedValue );
            snprintf( formattedValue,
                      sizeof formattedValue / sizeof *formattedValue,
                      format, longitude );
            result = fwrite( formattedValue, FIELD_WIDTH, 1, file ) == 1;

            /* Write Latitude: */

            if ( result ) {
              memset( formattedValue, 0, sizeof formattedValue );
              snprintf( formattedValue,
                        sizeof formattedValue / sizeof *formattedValue,
                        format, latitude );
              result = fwrite( formattedValue, FIELD_WIDTH, 1, file ) == 1;

              /* Write Depth: */

              if (result ) {

                if ( z ) {
                  memset( formattedValue, 0, sizeof formattedValue );
                  snprintf( formattedValue,
                            sizeof formattedValue / sizeof *formattedValue,
                            format, depth );
                  result = fwrite( formattedValue, FIELD_WIDTH, 1, file ) == 1;
                }
              }
            }
          }
        }
      }

      fclose( file ), file = 0;
    }

    if ( ! result ) {
      perror( "\n\nFailed to write DBF file because" );
    }

    memset( header, 0, headerBytes );
    FREE( header );
  }

  if ( result ) {
    result = writeDataToCSVFile( fileName, variable, units,
                                 timesteps, yyyymmddhh, timestepType,
                                 count, type, components, data, lonlats,
                                 z, sids );
  }

  return result;
}



/******************************************************************************
PURPOSE: writeDataToCSVFile - Write location info and
         time-varying data to a csv file.
INPUTS:  const char* fileName  Base name of file to create. "IOOS".
         const char* variable  Name of data variable. E.g., "salinity".
         const char* units     Name of data units. E.g., "PSU".
         const int timesteps   Number of timesteps of data.
         const int yyyymmddhh[ timesteps ]  Timestamps of data.
         const int timestepType  HOURLY, DAILY, MONTHLY, YEARLY.
         const int count        Number of data rows, e.g., grid rows x columns.
         const int type         Data type: BYTE_TYPE, FLOAT_TYPE, etc.
         const int components   1 = scalar, 2 or 3 = vector.
         const void* data      data[ components * timesteps * count ]
                                           Component data at cells.
         const float lonlats[ count * 2 ]  Lon-lat of each row data point.
         const float z[ count ]            Depth(m) of each row data point or 0
         const char* const sids[ count ]   Station id strings or 0.
NOTES:   Creates file fileName.csv containing static location info:
         ID (int)    [Station (string)]  Longitude Latitude [Depth(m)]
         [] denotes optional (if available).
         ID is int in range [0, points - 1] index matching shapes in shp file.
         Station is string-id for the station, if station data, else omitted.
         Depth(m) is omitted if unavailable.
         And time-varying data:
         YYYYMMDDHH   Quantity    Units    Value
         or YYYYMMDD
         or YYYYMM
         or YYYY
         With rows ordered by: ID then timestamp then variable/component.
******************************************************************************/

static int writeDataToCSVFile( const char* fileName,
                               const char* variable, const char* units,
                               const int timesteps, const int yyyymmddhh[],
                               const int timestepType,
                               const int count, const int type,
                               const int components,
                               const void* data,
                               const float lonlats[], const float z[],
                               const char* const sids[] ) {

  PRE017( fileName,
          *fileName,
          variable,
          *variable,
          units,
          *units,
          timesteps > 0,
          yyyymmddhh,
          IS_VALID_TIMESTEP_TYPE( timestepType ),
          count > 0,
          IN_RANGE( type, 0, GRID_DATA_TYPES ),
          data,
          IN_RANGE( components, 1, 3 ),
          lonlats,
          isValidLongitudeLatitude( lonlats[ 0 ], lonlats[ 1 ] ),
          isValidLongitudeLatitude( lonlats[  count * 2 - 2 ],
                                       lonlats[  count * 2 - 1 ] ),
          IMPLIES( z, AND2( ! isNan( z[ 0 ] ), ! isNan( z[ count - 1 ] ))));

  int result = 0;
  enum { MAXIMUM_LINE_LENGTH = 512 }; /* Of formatted data row. */
  size_t bufferSize = timesteps * MAXIMUM_LINE_LENGTH;
  char* buffer = NEW( char, bufferSize ); /* Buffer ASCII output for speed. */

  if ( buffer ) {
    enum { FILE_NAME_LENGTH = 255 };
    FILE* csvFile = 0;
    char csvFileName[ FILE_NAME_LENGTH + 1 ] = "";

    DEBUG( fprintf( stderr, "writeDataToCSVFile( fileName = '%s', "
                    "variable = '%s', units = '%s', "
                    "timesteps = %d, yyyymmddhh[] = [%d ... %d], "
                    "timestepType = %d, count = %d, type = %d, "
                    "components = %d, data = %p, "
                    "lonlats = %p, z = %p, sids = %p )\n",
                    fileName, variable, units,
                    timesteps, yyyymmddhh[ 0 ], yyyymmddhh[ timesteps - 1 ],
                    timestepType, count, type, components, data,
                    lonlats, z, sids ); )

    memset( csvFileName, 0, sizeof csvFileName );
    strncpy( csvFileName, fileName, FILE_NAME_LENGTH - 4 );
    strcat( csvFileName, ".csv" );
    csvFile = fopen( csvFileName, "w" );
    result = csvFile != 0;

    if ( csvFile == 0 ) {
      perror( "\n\nFailed because" );
    } else {
      const int componentOffset = timesteps * count;
      /* ID Station Longitude Latitude Depth YYYYMMDDHH Quantity Units. */
      const float* const fdata = type == FLOAT_TYPE ? data : 0;
      const char* const  cdata = type == BYTE_TYPE  ? data : 0;
      const unsigned short* const sdata = type == UINT16_TYPE ? data : 0;
      const float missingValue = cdata ? -99.0 : -9999.0;
      int point = 0;
      const char* const timestampColumnName =
        timestepType == YEARLY ? "YYYY"
        : timestepType == MONTHLY ? "YYYYMM"
        : timestepType == DAILY ? "YYYYMMDD"
        : "YYYYMMDDHH";
      const char* const valueColumnNames =
        components == 3 ? "U_Value,V_Value,W_Value"
        : components == 2 ? "U_Value,V_Value"
        : "Value";
      const char* const units0 = ! strcmp( units, "%s" ) ? "-" : units;
      /* If units = '%s' then print it as '-' instead. */

      /* Write one-line header column names: */

      fprintf( csvFile, "ID" );

      if ( sids ) {
        fprintf( csvFile, ",Station" );
      }

      fprintf( csvFile, ",Longitude,Latitude" );

      if ( z ) {
        fprintf( csvFile, ",Depth(m)" );
      }

      fprintf( csvFile, ",%s", timestampColumnName );
      fprintf( csvFile, ",Quantity,Units,%s\n", valueColumnNames );

      /* Write data rows: */

      for ( point = 0; AND2( result, point < count ); ++point ) {
        const int point2 = point + point;
        const float longitude = lonlats[ point2 ];
        const float latitude  = lonlats[ point2 + 1 ];
        const int id = point;
        const char* const station = sids ? sids[ point ] : 0;
        const float depth = z ? z[ point ] : 0.0;
        int timestep = 0;
        int bytes = 0;
        size_t bufferBytesStored = 0;
        char* outputBuffer = buffer;

        for ( timestep = 0; timestep < timesteps; ++timestep ) {
          const int yyyymmddhh0 = yyyymmddhh[ timestep ];
          const int timestamp =
            timestepType == YEARLY ? yyyymmddhh0 / 1000000
            : timestepType == MONTHLY ? yyyymmddhh0 / 10000
            : timestepType == DAILY ? yyyymmddhh0 / 100
            : yyyymmddhh0;
          const int timesteppedPointOffset = timestep * count + point;
          int component = 0;
          int rowBytes = 0;
          int eraseRow = 0;

          /* Write row line: */

          bytes = sprintf( outputBuffer, "%d", id );
          outputBuffer += bytes;
          bufferBytesStored += bytes;
          rowBytes += bytes;

          if ( station ) {
            bytes = sprintf( outputBuffer, ",%s", station );
            outputBuffer += bytes;
            bufferBytesStored += bytes;
            rowBytes += bytes;
          }

          bytes = sprintf( outputBuffer, ",%f,%f", longitude, latitude );
          outputBuffer += bytes;
          bufferBytesStored += bytes;
          rowBytes += bytes;

          if ( z ) {
            bytes = sprintf( outputBuffer, ",%f", depth );
            outputBuffer += bytes;
            bufferBytesStored += bytes;
            rowBytes += bytes;
          }

          bytes =
            sprintf( outputBuffer, ",%d,%s,%s", timestamp, variable, units0 );
          outputBuffer += bytes;
          bufferBytesStored += bytes;
          rowBytes += bytes;

          for ( component = 0; component < components; ++component ) {
            const int index =
              component * componentOffset + timesteppedPointOffset;
            const float value =
              fdata ? fdata[ index ]
              : cdata ? (float) (cdata[ index ])
              : sdata ? (float) (sdata[ index ])
              : 0.0;

            if ( OR2( includeMissingValuesInCSVFile, value > missingValue ) ) {
              const float absValue = fabs( value );
              const int useExp =
                AND2( value != 0.0, OR2( absValue < 1e-3, absValue > 1e6 ) );
              const char* const format = useExp ? ",%e" : ",%f";
              CHECK( IN_RANGE( index, 0, components * timesteps * count - 1 ));

              DEBUG( fprintf( stderr,
                        "point = %d, timestep = %d, data[ index = %d ] = %f\n",
                        point, timestep, index, value ); )

              bytes = sprintf( outputBuffer, format, value );
              outputBuffer += bytes;
              bufferBytesStored += bytes;
              rowBytes += bytes;
            } else {
              eraseRow = 1;
            }
          } /* End loop on components. */

          bytes = sprintf( outputBuffer, "\n" ); /* End of row line. */
          outputBuffer += bytes;
          bufferBytesStored += bytes;
          rowBytes += bytes;

          if ( eraseRow ) {
            outputBuffer -= rowBytes;
            bufferBytesStored -= rowBytes;
          }

          CHECK( bufferBytesStored / ( timestep + 1 ) < MAXIMUM_LINE_LENGTH );
        } /* End loop on timesteps. */

        /* Write buffer to CSV file: */

        result =
          fwrite( buffer, 1, bufferBytesStored, csvFile ) == bufferBytesStored;
        memset( buffer, 0, bufferSize );
      } /* End loop on points. */

      fclose( csvFile ), csvFile = 0;
    }

    FREE( buffer );
  }

  if ( ! result ) {
    perror( "\n\nFailed to write CSV file because" );
  }

  return result;
}



/******************************************************************************
PURPOSE: storeStringValue - Store string value in ShapeData.
INPUTS:  const char* value     String value to duplicate and store.
         ShapeData* shapeData  ShapeData to store string value in.
OUTPUTS: ShapeData* shapeData  ShapeData with stored string value.
RETURNS: const char* pointer to stored string if successful, else 0 and
         a failure message is printed to stderr.
NOTES:   Flyweight Pattern: store only unique strings with shared pointers to
         duplicates to greatly reduce the amount of memory required to store
         many instances of relatively few unique strings.
******************************************************************************/

static const char* storeStringValue( const char* value, ShapeData* shapeData) {

  PRE04( value, shapeData, shapeData->capacity > 0, shapeData->stringStorage );

  const char* result = 0;
  int index = 0;
  int ok = 0;
  const int capacity = shapeData->capacity;
  char** stringStorage = shapeData->stringStorage;
  CHECK2( capacity > 0, stringStorage != 0 );

  DEBUG2( fprintf( stderr,
                   "storeStringValue( %s, capacity = %d, [%s, %s ...] )\n",
                   value, capacity,
                   stringStorage[ 0 ],
                   stringStorage[ 1 ] ); )

  /* Linear search for matching string or empty slot: */

  while ( AND3( index < capacity,
                stringStorage[ index ],
                strcmp( stringStorage[ index ], value ) ) ) {
    ++index;
  }

  DEBUG2( fprintf( stderr, "index = %d\n", index ); )

  if ( index < capacity ) { /* Found match or empty slot for new string: */

    if ( stringStorage[ index ] == 0 ) { /* Store copy into empty slot: */
      stringStorage[ index ] = strdup( value );
    }

    ok = stringStorage[ index ] != 0;

    if ( ! ok ) {
      fprintf(stderr, "Failed to allocate enough memory to copy text data.\n");
    }

    CHECK( IMPLIES( ok, ! strcmp( shapeData->stringStorage[ index ], value )));
  } else { /* Must grow stringStorage. */
    const int growthAmount = capacity / 2; /* Grow storage by 50%. */
    const int newCapacity = capacity + growthAmount;
    char** newStrings = newCapacity > 0 ? NEW( char*, newCapacity ) : 0;

    if ( newStrings ) {

      /* Transfer pointers to strings from old storage to new storage: */

      for ( index = 0; index < capacity; ++index ) {
        newStrings[ index ] = stringStorage[ index ];
        stringStorage[ index ] = 0;
      }

      FREE( shapeData->stringStorage );
      stringStorage = 0;
      shapeData->stringStorage = newStrings;
      newStrings = 0;
      shapeData->capacity = newCapacity;
      shapeData->columnNames = shapeData->stringStorage; /* Alias. */

      CHECK2( IN_RANGE( index, 0, shapeData->capacity - 1 ),
              shapeData->stringStorage[ index ] == 0 );

      shapeData->stringStorage[ index ] = strdup( value );
      ok = shapeData->stringStorage[ index ] != 0;
      CHECK( IMPLIES( ok, ! strcmp( shapeData->stringStorage[index], value )));

      if ( ! ok ) {
        fprintf( stderr,
                 "Failed to allocate enough memory to copy text data.\n" );
      }
    }
  }

  if ( ok ) {
    CHECK( IN_RANGE( index, 0, shapeData->capacity - 1 ) );
    result = shapeData->stringStorage[ index ];
  }

  POST0( IMPLIES( result,
                  AND3( IN_RANGE( index, 0, shapeData->capacity - 1 ),
                        ! strcmp( shapeData->stringStorage[ index ], value ),
                        result == shapeData->stringStorage[ index ] ) ) );
  return result;
}



/******************************************************************************
PURPOSE: copyStringAttribute - Copy a string attribute from one DBF file to
         another.
INPUTS:  DBFHandle inputFile      File to read column attribute from.
         int inputRow             Row index to read (0-based).
         int inputColumn          Column index to read (0-based).
         DBFHandle outputFile     File to write column attribute to.
         int outputRow            Row index to write (0-based).
         int outputColumn         Column index to write (0-based).
RETURNS: int 1 if successful, else 0.
******************************************************************************/

static int copyStringAttribute( DBFHandle inputFile,
                                int inputRow, int inputColumn,
                                DBFHandle outputFile,
                                int outputRow, int outputColumn ) {
  const char* const value =
    DBFReadStringAttribute( inputFile, inputRow, inputColumn );
  const int result =
    DBFWriteStringAttribute( outputFile, outputRow, outputColumn, value );
  return result;
}



/******************************************************************************
PURPOSE: copyIntegerAttribute - Copy an integer attribute from one DBF file to
         another.
INPUTS:  DBFHandle inputFile      File to read column attribute from.
         int inputRow             Row index to read (0-based).
         int inputColumn          Column index to read (0-based).
         DBFHandle outputFile     File to write column attribute to.
         int outputRow            Row index to write (0-based).
         int outputColumn         Column index to write (0-based).
RETURNS: int 1 if successful, else 0.
******************************************************************************/

static int copyIntegerAttribute( DBFHandle inputFile,
                                 int inputRow, int inputColumn,
                                 DBFHandle outputFile,
                                 int outputRow, int outputColumn ) {
  const int value =
    DBFReadIntegerAttribute( inputFile, inputRow, inputColumn );
  const int result =
    DBFWriteIntegerAttribute( outputFile, outputRow, outputColumn, value );
  return result;
}



/******************************************************************************
PURPOSE: copyDoubleAttribute - Copy a double attribute from one DBF file to
         another.
INPUTS:  DBFHandle inputFile      File to read column attribute from.
         int inputRow             Row index to read (0-based).
         int inputColumn          Column index to read (0-based).
         DBFHandle outputFile     File to write column attribute to.
         int outputRow            Row index to write (0-based).
         int outputColumn         Column index to write (0-based).
         int invalidIfNegative    Map all negative values to missing?
         double missing           Value to map all negative values to.
         double offset            Value to add to value read.
         double scale             Value to multiply offset-value read.
RETURNS: int 1 if successful, else 0.
******************************************************************************/

static int copyDoubleAttribute( DBFHandle inputFile,
                                int inputRow, int inputColumn,
                                DBFHandle outputFile,
                                int outputRow, int outputColumn,
                                int invalidIfNegative, double missing,
                                double offset, double scale ) {
  const double value0 =
    DBFReadDoubleAttribute( inputFile, inputRow, inputColumn );
  const double value =
    AND2( invalidIfNegative, value0 < 0.0 ) ? missing
    : ( value0 + offset ) * scale;
  const int result =
    DBFWriteDoubleAttribute( outputFile, outputRow, outputColumn, value );
  return result;
}




/******************************************************************************
PURPOSE: computeSparsedPartCount - Compute sparsed polyline/polygon part count.
INPUTS:  const SHPObject* shape  Shape to examine.
         const double minimumAdjacentVertexDistance  Adjacent vertices closer
                                                     than this
                                                     (in either x or y) will
                                                     be merged.
         const int minimumSparsedVertices  2 if polyline, 3 if polygon.
RETURNS: int sparsed part count.
******************************************************************************/

static int computeSparsedPartCount( const SHPObject* shape,
                                    const double minimumAdjacentVertexDistance,
                                    const int minimumSparsedVertices ) {

  PRE012( shape,
          IN5( shape->nSHPType,
               SHPT_POLYGON, SHPT_POLYGONZ, SHPT_ARC, SHPT_ARCZ ),
          shape->nShapeId >= 0,
          shape->nParts > 0,
          shape->panPartStart,
          shape->panPartType,
          shape->panPartType[ 0 ] == SHPP_RING,
          shape->nVertices > 0,
          shape->padfX,
          shape->padfY,
          minimumAdjacentVertexDistance >= 0.0,
          IN3( minimumSparsedVertices, 2, 3 ) );

  int result = 0;
  const int parts = shape->nParts;
  int part = 0;

  /* Compute sparsed number of parts (with at least minimumSparsedVertices): */

  for ( part = 0; part < parts; ++part ) {
    const int partVertexCount =
      parts == 1 ? shape->nVertices
      : part < parts - 1 ?
        shape->panPartStart[ part + 1 ] - shape->panPartStart[ part ]
      : shape->nVertices - shape->panPartStart[ part ];
    const int offset = shape->panPartStart[ part ];
    const double* const x = shape->padfX + offset;
    const double* const y = shape->padfY + offset;
    double x0 = x[ 0 ];
    double y0 = y[ 0 ];
    int sparsedVertices = 1; /* Count first vertex. */
    int vertex = 1;

    for ( ; AND2( vertex < partVertexCount,
                  sparsedVertices < minimumSparsedVertices );
          ++vertex ) {
      const double x1 = x[ vertex ];
      const double y1 = y[ vertex ];
      const double deltaX = x1 > x0 ? x1 - x0 : x0 - x1;

      if ( deltaX >= minimumAdjacentVertexDistance ) {
        ++sparsedVertices;
        x0 = x1;
        y0 = y1;
      } else {
        const double deltaY = y1 > y0 ? y1 - y0 : y0 - y1;

        if ( deltaY >= minimumAdjacentVertexDistance ) {
          ++sparsedVertices;
          x0 = x1;
          y0 = y1;
        }
      }
    }

    result += sparsedVertices == minimumSparsedVertices;
  }

  POST0( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: computeSparsedVertexCount - Compute sparsed polyline/polygon vertex
                                     count.
INPUTS:  const int vertexCount          Number of vertices in part.
         const double x[ vertexCount ]  X-coordinate of part.
         const double y[ vertexCount ]  Y-coordinate of part.
         const double minimumAdjacentVertexDistance  Adjacent vertices closer
                                                     than this
                                                     (in either x or y) will
                                                     be merged.
         const int isPolygon            1 if is polygon, else 0.
RETURNS: int sparsed vertex count of part.
******************************************************************************/

static int computeSparsedVertexCount( const int vertexCount,
                                      const double x[],
                                      const double y[],
                                      const double minimumAdjacentVertexDistance,
                                      const int isPolygon ) {

  PRE05( vertexCount > 0, x, y, minimumAdjacentVertexDistance >= 0.0,
         IS_BOOL( isPolygon ) );

  int result = 1; /* Count first vertex. */
  double x0 = x[ 0 ];
  double y0 = y[ 0 ];
  int vertex = 1;

  for ( ; vertex < vertexCount; ++vertex ) {
    const double x1 = x[ vertex ];
    const double y1 = y[ vertex ];
    const double deltaX = x1 > x0 ? x1 - x0 : x0 - x1;

    if ( deltaX >= minimumAdjacentVertexDistance ) {
      ++result;
      x0 = x1;
      y0 = y1;
    } else {
      const double deltaY = y1 > y0 ? y1 - y0 : y0 - y1;

      if ( deltaY >= minimumAdjacentVertexDistance ) {
        ++result;
        x0 = x1;
        y0 = y1;
      }
    }
  }

  if ( AND2( isPolygon, ! AND2( x0 == x[ 0 ], y0 == y[ 0 ] ) ) ) {
    ++result; /* Replicate first vertex to close polygon. */
  }

  POST0( result >= 1 );
  return result;
}



/******************************************************************************
PURPOSE: copySparseVertices - Copy sparsed polyline/polygon vertices and also
         compute bounds of sparsed vertices.
INPUTS:  const int vertexCount          Number of vertices in part.
         const int sparsedVertices      Number of resulting sparsed vertices.
         const double x[ vertexCount ]  X-coordinates of part.
         const double y[ vertexCount ]  Y-coordinates of part.
         const double minimumAdjacentVertexDistance  Adjacent vertices closer
                                                     than this
                                                     (in either x or y) will
                                                     be merged.
         const int isPolygon            1 if is polygon, else 0.
         const int* initializedBounds    Have bounds been initialized?
         Bounds bounds                   Bounds of sparsed vertices.
OUTPUTS: const int* initializedBounds    Have bounds been initialized?
         Bounds bounds                   Updated bounds of sparsed vertices.
         gpc_vertex* const vertices      GPC vertices copied into.
******************************************************************************/

static void copySparseVertices( const int vertexCount,
                                const int sparsedVertices,
                                const double x[],
                                const double y[],
                                const double minimumAdjacentVertexDistance,
                                const int isPolygon,
                                int* const initializedBounds,
                                Bounds bounds,
                                gpc_vertex* const vertices ) {

  PRE010( vertexCount > 0, sparsedVertices > 0, x, y,
          minimumAdjacentVertexDistance >= 0.0,
          IS_BOOL( isPolygon ), initializedBounds,
          IS_BOOL( *initializedBounds ), bounds, vertices );

  double xMinimum = bounds[ LONGITUDE ][ MINIMUM ];
  double xMaximum = bounds[ LONGITUDE ][ MAXIMUM ];
  double yMinimum = bounds[ LATITUDE ] [ MINIMUM ];
  double yMaximum = bounds[ LATITUDE  ][ MAXIMUM ];
  int vertex = 0;
  int sparsedVertex = 1; /* Count first vertex always copied. */
  double x0 = x[ 0 ];
  double y0 = y[ 0 ];
  vertices[ 0 ].x = x0; /* Copy first vertex. */
  vertices[ 0 ].y = y0;

  for ( vertex = 1; vertex < vertexCount; ++vertex ) {
    const double x1 = x[ vertex ];
    const double y1 = y[ vertex ];
    const double deltaX = x1 > x0 ? x1 - x0 : x0 - x1;
    int nonCoincident = deltaX >= minimumAdjacentVertexDistance;

    if ( ! nonCoincident ) {
      const double deltaY = y1 > y0 ? y1 - y0 : y0 - y1;
      nonCoincident = deltaY >= minimumAdjacentVertexDistance;
    }

    if ( nonCoincident ) {

      /* Copy vertex: */

      CHECK( IN_RANGE( sparsedVertex, 0, sparsedVertices - 1 ) );
      vertices[ sparsedVertex ].x = x1;
      vertices[ sparsedVertex ].y = y1;
      ++sparsedVertex;
      x0 = x1;
      y0 = y1;

      /* Update bounds: */

      if ( ! *initializedBounds ) {
        *initializedBounds = 1;
        xMinimum = xMaximum = x0;
        yMinimum = yMaximum = y0;
      }

      if ( x1 < xMinimum ) {
        xMinimum = x1;
      } else if ( x1 > xMaximum ) {
        xMaximum = x1;
      }

      if ( y1 < yMinimum ) {
        yMinimum = y1;
      } else if ( y1 > yMaximum ) {
        yMaximum = y1;
      }
    }
  }

  /* Update bounds: */

  bounds[ LONGITUDE ][ MINIMUM ] = xMinimum;
  bounds[ LONGITUDE ][ MAXIMUM ] = xMaximum;
  bounds[ LATITUDE  ][ MINIMUM ] = yMinimum;
  bounds[ LATITUDE  ][ MAXIMUM ] = yMaximum;

  /* If needed, replicate first vertex to close polygon. */

  if ( AND2( isPolygon, sparsedVertex < sparsedVertices ) ) {
    vertices[ sparsedVertex ].x = x[ 0 ];
    vertices[ sparsedVertex ].y = y[ 0 ];
    ++sparsedVertex;
  }

  CHECK( sparsedVertex == sparsedVertices );
  POST02( *initializedBounds, isValidBounds( (const double (*)[2]) bounds ) );
}



/******************************************************************************
PURPOSE: computeVertexBounds - Compute bounds of vertices.
INPUTS:  const int count          Number of vertices.
OUTPUTS: const float xy[          Sequence of (x, y) vertices.
         double xyRange[ 2 * 2 ]  xMin, yMin, xMax, yMax.
******************************************************************************/

static void computeVertexBounds( const int count, const float xy[],
                                 double xyRange[] ) {

  const float* x = xy;
  const float* y = xy + 1;
  float xMinimum = *x;
  float xMaximum = xMinimum;
  float yMinimum = *y;
  float yMaximum = yMinimum;
  int vertex = 0;

  for ( vertex = 0; vertex < count; ++vertex, x += 2, y += 2 ) {

    if ( *x < xMinimum ) {
      xMinimum = *x;
    } else if ( *x > xMaximum ) {
      xMaximum = *x;
    }

    if ( *y < yMinimum ) {
      yMinimum = *y;
    } else if ( *y > yMaximum ) {
      yMaximum = *y;
    }
  }

  xyRange[ 0 ] = xMinimum;
  xyRange[ 1 ] = yMinimum;
  xyRange[ 2 ] = xMaximum;
  xyRange[ 3 ] = yMaximum;
}



/******************************************************************************
PURPOSE: computeGridBounds - Compute bounds of grid coordinates.
INPUTS:  const int rows            Number of grid rows.
         const int columns         Number of grid columns.
         const double westEdge     Distance from origin to west edge of grid.
         const double southEdge    Distance from origin to south edge of ".
         const double cellWidth    Width of each grid cell (e.g., 12000 m).
         const double cellWHeight  Height of each grid cell (e.g., 12000 m).
         Unproject unproject       Function to unproject (x,y) to (lon,lat).
OUTPUTS: double range[ 4 ]         xMin, yMin, xMax, yMax.
******************************************************************************/

static void computeGridBounds( const int rows,
                               const int columns,
                               const double westEdge,
                               const double southEdge,
                               const double cellWidth,
                               const double cellHeight,
                               Unproject unproject,
                               double range[ 4 ] ) {

  PRE05( rows > 0,
         columns > 0,
         rows * columns > 0,
         IMPLIES( unproject == 0,
                   AND2 ( isValidLongitudeLatitude( westEdge, southEdge ),
                         isValidLongitudeLatitude(
                                     westEdge  + columns * cellWidth,
                                     southEdge + rows    * cellHeight ) ) ),
         range );

  const int rows_1 = rows + 1;
  const int columns_1 = columns + 1;
  double x = westEdge;
  double y = southEdge;
  double px = x;
  double py = y;
  double minimumX = px;
  double maximumX = minimumX;
  double minimumY = py;
  double maximumY = minimumY;
  int row = 0;
  int column = 0;

  /* Initialize to west/south point: */

  if ( unproject ) {
    unproject( x, y, &px, &py );
    minimumX = maximumX = px;
    minimumY = maximumY = py;
  }

  /* Check against rest of west edge of grid: */

  y += cellHeight;

  for ( row = 1; row < rows_1; ++row, y += cellHeight ) {
    updatePointMinmax( unproject, x, y,
                       &minimumX, &maximumX, &minimumY, &maximumY );
  }

  /* Check against east edge of grid: */

  x = westEdge + columns * cellWidth;
  y = southEdge;

  for ( row = 0; row < rows_1; ++row, y += cellHeight ) {
    updatePointMinmax( unproject, x, y,
                       &minimumX, &maximumX, &minimumY, &maximumY );
  }

  /* Check against rest of south edge of grid: */

  x = westEdge + cellWidth;
  y = southEdge;

  for ( column = 1; column < columns_1; ++column, x += cellWidth ) {
    updatePointMinmax( unproject, x, y,
                       &minimumX, &maximumX, &minimumY, &maximumY );
  }

  /* Check against north edge of grid: */

  x = westEdge;
  y = southEdge + rows * cellHeight;

  for ( column = 0; column < columns_1; ++column, x += cellWidth ) {
    updatePointMinmax( unproject, x, y,
                       &minimumX, &maximumX, &minimumY, &maximumY );
  }

  range[ 0 ] = minimumX;
  range[ 1 ] = minimumY;
  range[ 2 ] = maximumX;
  range[ 3 ] = maximumY;

  POST04( isValidLongitudeLatitude( range[ 0 ], range[ 1 ] ),
          isValidLongitudeLatitude( range[ 2 ], range[ 3 ] ),
          range[ 0 ] <= range[ 2 ],
          range[ 1 ] <= range[ 3 ] );
}


/******************************************************************************
PURPOSE: updatePointMinmax - Update minimum and maximum of grid coordinates.
INPUTS:  Unproject unproject       Function to unproject (x,y) to (lon,lat).
         const double x            X-coordinate to check.
         const double y            Y-coordinate to check.
         double* minimumX          Current minimum X-coordinate.
         double* maximumX          Current maximum X-coordinate.
         double* minimumY          Current minimum Y-coordinate.
         double* maximumY          Current maximum Y-coordinate.
OUTPUTS: double* minimumX          Updated minimum X-coordinate.
         double* maximumX          Updated maximum X-coordinate.
         double* minimumY          Updated minimum Y-coordinate.
         double* maximumY          Updated maximum Y-coordinate.
******************************************************************************/

static void updatePointMinmax( Unproject unproject,
                               const double x,
                               const double y,
                               double* minimumX,
                               double* maximumX,
                               double* minimumY,
                               double* maximumY ) {

  PRE05( minimumX, maximumX, minimumY, maximumY,
         IMPLIES( unproject == 0,
                   AND3( isValidLongitudeLatitude( x, y ),
                         isValidLongitudeLatitude(*minimumX, *minimumY ),
                         isValidLongitudeLatitude(*maximumX, *maximumY ) ) ) );

  double px = x;
  double py = y;

  if ( unproject ) {
    unproject( x, y, &px, &py );
  }

  if ( px < *minimumX ) {
    *minimumX = px;
  } else if ( px > *maximumX ) {
    *maximumX = px;
  }

  if ( py < *minimumY ) {
    *minimumY = py;
  } else if ( py > *maximumY ) {
    *maximumY = py;
  }
}



/******************************************************************************
PURPOSE: computeGridCellCenters - Compute longitude-latitude of each grid cell
         center.
INPUTS:  const int rows            Number of grid rows.
         const int columns         Number of grid columns.
         const double westEdge     Distance from origin to west edge of grid.
         const double southEdge    Distance from origin to south edge of ".
         const double cellWidth    Width of each grid cell (e.g., 12000 m).
         const double cellWHeight  Height of each grid cell (e.g., 12000 m).
         Unproject unproject       Function to unproject (x,y) to (lon,lat).
OUTPUTS: float lonlats[ rows * columns * 2 ]   Lon-lats of grid cell centers.
******************************************************************************/

static void computeGridCellCenters( const int rows,
                                    const int columns,
                                    const double westEdge,
                                    const double southEdge,
                                    const double cellWidth,
                                    const double cellHeight,
                                    Unproject unproject,
                                    float lonlats[] ) {

  PRE05( rows > 0,
         columns > 0,
         rows * columns > 0,
         IMPLIES( unproject == 0,
                   AND2 ( isValidLongitudeLatitude( westEdge, southEdge ),
                          isValidLongitudeLatitude(
                                     westEdge  + columns * cellWidth,
                                     southEdge + rows    * cellHeight ) ) ),
         lonlats );

  float* coordinates = lonlats;
  double y = southEdge;
  int row = 0;

  for ( row = 0; row < rows; ++row, y += cellHeight ) {
    const double centerY = ( y + y + cellHeight ) * 0.5;
    int column = 0;
    double x = westEdge;

    for ( column = 0; column < columns; ++column, x += cellWidth ) {
      const double centerX = ( x + x + cellWidth ) * 0.5;
      double px = x;
      double py = y;

      if ( unproject ) {
        unproject( centerX, centerY, &px, &py );
      }

      *coordinates++ = px;
      *coordinates++ = py;
    }
  }

  POST02( isValidLongitudeLatitude( lonlats[ 0 ], lonlats[ 1 ] ),
          isValidLongitudeLatitude( lonlats[ rows * columns * 2 - 2 ],
                                       lonlats[ rows * columns * 2 - 1 ] ) );
}



/******************************************************************************
PURPOSE: computePointBounds - Compute bounds of point coordinates.
INPUTS:  const int points                   Number of points.
         const float lonlats[ points * 2 ]  Longitude latitude coordinates.
OUTPUTS: double range[ 4 ]                  xMin, yMin, xMax, yMax.
******************************************************************************/

static void computePointBounds( const int points,
                                const float lonlats[],
                                double range[ 4 ] ) {
  double minimumX = lonlats[ 0 ];
  double maximumX = minimumX;
  double minimumY = lonlats[ 1 ];
  double maximumY = minimumY;
  int point = 0;

  /* Check against rest of points: */

  for ( point = 1; point < points; ++point ) {
    const int point2 = point + point;
    const float longitude = lonlats[ point2 ];
    const float latitude = lonlats[ point2 + 1 ];

    if ( longitude < minimumX ) {
      minimumX = longitude;
    } else if ( longitude > maximumX ) {
      maximumX = longitude;
    }

    if ( latitude < minimumY ) {
      minimumY = latitude;
    } else if ( latitude > maximumY ) {
      maximumY = latitude;
    }
  }

  range[ 0 ] = minimumX;
  range[ 1 ] = minimumY;
  range[ 2 ] = maximumX;
  range[ 3 ] = maximumY;
}



/******************************************************************************
PURPOSE: computeRange - Compute minimum and maximum value in array.
INPUTS:  const double array[ count ]  Items to examine.
         const int count              Number of items to compare.
         const int stride             Stride of index.
OUTPUTS: double range[ 2 ]            Minimum and maximum values in array.
******************************************************************************/

static void computeRange( const double array[], const int count,
                          const int stride, double range[ 2 ] ) {
  double minimum = array[ 0 ];
  double maximum = minimum;
  int index = 0;

  for ( index = stride; index < count; index += stride ) {
    const double item = array[ index ];

    if ( item < minimum ) {
      minimum = item;
    } else if ( item > maximum ) {
      maximum = item;
    }
  }

  range[ 0 ] = minimum;
  range[ 1 ] = maximum;
}



/******************************************************************************
PURPOSE: convertTimestamp - Convert HMS smoke timestamp to yyyyddd, hhmm.
INPUTS:  const char* inputFileName  File name of input: hms_smoke20161017.dbf
         const char* timestamp      Timestamp hhmm or yyyyddd hhmm.
OUTPUTS: int* const yyyyddd         Date.
         int* const hhmm            Time.
RETURNS: int 1 if successful, else 0 failure message is printed.
******************************************************************************/

static int convertTimestamp( const char* inputFileName,
                             const char* timestamp,
                             int* const yyyyddd, int* const hhmm ) {

  PRE05( inputFileName, *inputFileName, timestamp, yyyyddd, hhmm );
  int result = 0;

  if ( strlen( timestamp ) == 12 ) { /* YYYYDDD HHMM: */
    result = sscanf( timestamp, "%d %d", yyyyddd, hhmm ) == 2;

    if ( result ) {
      result = isValidDate( *yyyyddd );

      if ( result ) {
        result = isValidTime( *hhmm * 100 );
      }
    }
  } else { /* HHMM only. Parse YYYYMMDD from file name & convert to YYYYDDD: */
    const char* const tag = "hms_smoke";
    const char* name = strstr( inputFileName, tag );

    if ( name ) {
      name += strlen( tag );

      if ( isdigit( *name ) ) {
        const int yyyymmdd = atoi( name );
        result = isValidYearMonthDay( yyyymmdd );

        if ( result ) {
          *yyyyddd = convertYearMonthDay( yyyymmdd );
          *hhmm = atoi( timestamp );
          result = isValidTime( *hhmm * 100 );
        }
      }
    }
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: writeShort - Write a 2-byte integer to a byte array with the given
                    endian format.
INPUTS:  const int index        Starting index into bytes[] to write.
         const short value      Value to write.
         const int endian       Endian format to write.
OUTPUTS: unsigned char bytes[]  bytes[ index ... index + 4 ].
RETURNS: int index + 4.
******************************************************************************/

static int writeShort( unsigned char bytes[], const int index,
                      const short value, const int endian ) {
  const int result = index + 4;
  const unsigned char byte1 =   value & 0x00ff;
  const unsigned char byte2 = ( value & 0xff00 ) >> 8;

  if ( endian == BIG ) {
    bytes[ index     ] = byte2;
    bytes[ index + 1 ] = byte1;
  } else {
    bytes[ index     ] = byte1;
    bytes[ index + 1 ] = byte2;
  }

  return result;
}



/******************************************************************************
PURPOSE: writeInt - Write a 4-byte integer to a byte array with the given
                    endian format.
INPUTS:  const int index        Starting index into bytes[] to write.
         const int value        Value to write.
         const int endian       Endian format to write.
OUTPUTS: unsigned char bytes[]  bytes[ index ... index + 4 ].
RETURNS: int index + 4.
******************************************************************************/

static int writeInt( unsigned char bytes[], const int index,
                     const int value, const int endian ) {
  const int result = index + 4;
  const unsigned char byte1 =   value & 0x000000ff;
  const unsigned char byte2 = ( value & 0x0000ff00 ) >> 8;
  const unsigned char byte3 = ( value & 0x00ff0000 ) >> 16;
  const unsigned char byte4 = ( value & 0xff000000 ) >> 24;

  if ( endian == BIG ) {
    bytes[ index     ] = byte4;
    bytes[ index + 1 ] = byte3;
    bytes[ index + 2 ] = byte2;
    bytes[ index + 3 ] = byte1;
  } else {
    bytes[ index     ] = byte1;
    bytes[ index + 1 ] = byte2;
    bytes[ index + 2 ] = byte3;
    bytes[ index + 3 ] = byte4;
  }

  return result;
}



/******************************************************************************
PURPOSE: writeDouble - Write a 8-byte real to a byte array with the given
                    endian format.
INPUTS:  const int index        Starting index into bytes[] to write.
         const double value     Value to write.
         const int endian       Endian format to write.
OUTPUTS: unsigned char bytes[]  bytes[ index ... index + 8 ].
RETURNS: int index + 8.
******************************************************************************/

static int writeDouble( unsigned char bytes[], const int index,
                        const double value, const int endian ) {
  const int result = index + 8;
  const void* const pvvalue      = &value;
  const long long* const pivalue = pvvalue;
  const long long ivalue         = *pivalue;
  const unsigned char byte1 =   ivalue & 0x00000000000000ffLL;
  const unsigned char byte2 = ( ivalue & 0x000000000000ff00LL ) >> 8;
  const unsigned char byte3 = ( ivalue & 0x0000000000ff0000LL ) >> 16;
  const unsigned char byte4 = ( ivalue & 0x00000000ff000000LL ) >> 24;
  const unsigned char byte5 = ( ivalue & 0x000000ff00000000LL ) >> 32;
  const unsigned char byte6 = ( ivalue & 0x0000ff0000000000LL ) >> 40;
  const unsigned char byte7 = ( ivalue & 0x00ff000000000000LL ) >> 48;
  const unsigned char byte8 = ( ivalue & 0xff00000000000000LL ) >> 56;

  if ( endian == BIG ) {
    bytes[ index     ] = byte8;
    bytes[ index + 1 ] = byte7;
    bytes[ index + 2 ] = byte6;
    bytes[ index + 3 ] = byte5;
    bytes[ index + 4 ] = byte4;
    bytes[ index + 5 ] = byte3;
    bytes[ index + 6 ] = byte2;
    bytes[ index + 7 ] = byte1;
  } else {
    bytes[ index     ] = byte1;
    bytes[ index + 1 ] = byte2;
    bytes[ index + 2 ] = byte3;
    bytes[ index + 3 ] = byte4;
    bytes[ index + 4 ] = byte5;
    bytes[ index + 5 ] = byte6;
    bytes[ index + 6 ] = byte7;
    bytes[ index + 7 ] = byte8;
  }

  return result;
}



/******************************************************************************
PURPOSE: minimumInt - Smallest value in array.
INPUTS:  int count                 Number of values in array.
         const int array[ count ]  Values to check.
RETURNS: int smallest value in array.
******************************************************************************/

static int minimumInt( int count, const int array[] ) {
  PRE02( count > 0, array );
  int index = 0;
  int result = array[ 0 ];

  for ( index = 1; index < count; ++index ) {
    const int value = array[ index ];
    result = result < value ? result : value;
  }

  POST02( result <= array[ 0 ], result <= array[ count - 1 ] );
  return result;
}



/******************************************************************************
PURPOSE: maximumInt - Largest value in array.
INPUTS:  int count                 Number of values in array.
         const int array[ count ]  Values to check.
RETURNS: int largest value in array.
******************************************************************************/

static int maximumInt( int count, const int array[] ) {
  PRE02( count > 0, array );
  int index = 0;
  int result = array[ 0 ];

  for ( index = 1; index < count; ++index ) {
    const int value = array[ index ];
    result = result > value ? result : value;
  }

  POST02( result >= array[ 0 ], result >= array[ count - 1 ] );
  return result;
}




