
/******************************************************************************
PURPOSE: KMLFile.c - Write KML files.
NOTES:
HISTORY: 2011-06-17 plessel.todd@epa.gov Created.
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h>  /* For FILE, stderr, fprintf(), fopen(), perror(). */
#include <string.h> /* For strncpy(), memset(). */
#include <math.h>   /* For sqrt(). */

#include <Assertions.h> /* For PRE(), IMPLIES_ELSE(). */
#include <Utilities.h>  /* For Bounds, Color, forEachFile(). */
#include <DateTime.h>   /* For isValidYYYYMMDDHH(), incrementDateTime(). */
#include <Shapefile.h>  /* For PolygonShape, ShapeData. */
#include <KMLFile.h>    /* For public interface. */

/*================================== TYPES ==================================*/

enum { KML_COLOR_LEVELS = 8 };
assert_static( 256 % KML_COLOR_LEVELS == 0 );

typedef char BGR[ 7 ]; /* KML blue-green-red hexedecimal color string. */

typedef struct {
  FILE* file;                  /* Stream to write KML content. */
  const char* inputDirectory;  /* Directory with map image files. */
  const char* outputDirectory; /* Directory to copy map image files. */
  const char* startsWith;      /* Input file name. E.g., "modisTrueColor". */
  const char* endsWith;        /* Input file extension. E.g., ".png". */
  Bounds bounds;               /* Bounds of image. */
} MapImageFileInfo;

/*=========================== FORWARD DECLARATIONS ==========================*/

static int isValidMapImageFileInfo( const MapImageFileInfo* const info );

static void writeMapImagesToKMLHelper( const char* const fileName, void* data );

static void writeKMLColors( FILE* file, int levels );

static void colorKML( Color color, int levels, BGR bgr );

/*============================= PUBLIC FUNCTIONS ============================*/



/******************************************************************************
PURPOSE: writeStartKML - Write start of KML file.
INPUTS:  FILE* file                     File to write to.
         const char* name               Name of document.
         const char* description        Description of document.
         const Bounds bounds            0 or Bounds of KML data.
******************************************************************************/

void writeStartKML( FILE* file,
                    const char* name, const char* description,
                    const Bounds bounds ) {

  PRE06( file, name, *name, description, *description,
         IMPLIES( bounds, isValidBounds( bounds ) ) );

  const char* const fileStartingContent =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<kml xmlns=\"http://earth.google.com/kml/2.1\">\n"
    "  <Document>\n"
    "    <name>%s</name>\n"
    "    <description>%s</description>\n";

  fprintf( file, fileStartingContent, name, description );
  writeKMLColors( file, KML_COLOR_LEVELS );

  if ( bounds ) { /* Save Overall Bounds: */
    fprintf( file,
             "    <Placemark>\n"
             "      <name>Overall_Bounds</name>\n"
             "      <description>Boundary of region of study."
             "</description>\n"
             "      <styleUrl>#ffffffff</styleUrl>\n"
             "      <LineString>\n"
             "        <coordinates>\n"
             "          %lf,%lf\n"
             "          %lf,%lf\n"
             "          %lf,%lf\n"
             "          %lf,%lf\n"
             "          %lf,%lf\n"
             "        </coordinates>\n"
             "      </LineString>\n"
             "    </Placemark>\n",
             bounds[ LONGITUDE ][ MINIMUM ],
             bounds[ LATITUDE  ][ MINIMUM ],
             bounds[ LONGITUDE ][ MAXIMUM ],
             bounds[ LATITUDE  ][ MINIMUM ],
             bounds[ LONGITUDE ][ MAXIMUM ],
             bounds[ LATITUDE  ][ MAXIMUM ],
             bounds[ LONGITUDE ][ MINIMUM ],
             bounds[ LATITUDE  ][ MAXIMUM ],
             bounds[ LONGITUDE ][ MINIMUM ],
             bounds[ LATITUDE  ][ MINIMUM ] );
  }
}



/******************************************************************************
PURPOSE: writeEndKML - Write end of KML file.
INPUTS:  FILE* file  File to write to.
******************************************************************************/

void writeEndKML( FILE* file ) {
  PRE0( file );
  const char* const fileEndingContent =
    "  </Document>\n"
    "</kml>\n";
  fprintf( file, "%s", fileEndingContent );
}



/******************************************************************************
PURPOSE: writeMapImagesToKML - Write satellite map images files to KML.
INPUTS:  FILE* file                         File to write to.
         const char* const inputDirectory   Directory path to check for
                                            map_image_yyyymmdd.png files.
         const char* const startsWith       File names start with. "map_image_"
         const char* const endsWith         File names end with. E.g., ".png".
         const char* const outputDirectory  Directory path for outputing files.
         const Bounds bounds                Lon-lat bounds of images.
******************************************************************************/

void writeMapImagesToKML( FILE* file,
                          const char* const inputDirectory,
                          const char* const startsWith,
                          const char* const endsWith,
                          const char* const outputDirectory,
                          const Bounds bounds ) {

  PRE011( file, inputDirectory, *inputDirectory,
          startsWith, *startsWith, endsWith, *endsWith,
          outputDirectory, *outputDirectory,
          strcmp( inputDirectory, outputDirectory ),
          isValidBounds( bounds ) );

  MapImageFileInfo info;
  memset( &info, 0, sizeof info );
  info.file = file;
  info.inputDirectory = inputDirectory;
  info.outputDirectory = outputDirectory;
  info.startsWith = startsWith;
  info.endsWith = endsWith;
  info.bounds[ LONGITUDE ][ MINIMUM ] = bounds[ LONGITUDE ][ MINIMUM ];
  info.bounds[ LONGITUDE ][ MAXIMUM ] = bounds[ LONGITUDE ][ MAXIMUM ];
  info.bounds[ LATITUDE  ][ MINIMUM ] = bounds[ LATITUDE  ][ MINIMUM ];
  info.bounds[ LATITUDE  ][ MAXIMUM ] = bounds[ LATITUDE  ][ MAXIMUM ];

  fprintf( file, "    <Folder>\n" );
  fprintf( file, "      <name>Satellite images</name>\n" );
  fprintf( file, "      <description>Satellite images."
                       "</description>\n" );

  /* Calls writeMapImagesToKMLHelper() with each matched file name: */

  forEachFile( inputDirectory, startsWith, endsWith,
               writeMapImagesToKMLHelper, (void*) &info );

  fprintf( file, "    </Folder>\n" );
}



/******************************************************************************
PURPOSE: writeMapPolylinesToKML - Write map polylines to KML file.
INPUTS:  FILE* file                         KML file to write to.
         int polylineCount                  Number of polylines.
         const int counts[ polylineCount ]  Number of vertices per polyline.
         const float vertices[]             Lon-lat vertices.
         const char* name                   Name of polyline set placemark.
         Color color                        Color of lines.
******************************************************************************/

void writeMapPolylinesToKML( FILE* file,
                             int polylineCount,
                             const int counts[],
                             const float vertices[],
                             const char* name,
                             Color color ) {

  PRE010( file,
          polylineCount > 0,
          counts,
          counts[ 0 ] > 1,
          counts[ polylineCount - 1 ] > 1,
          vertices,
          isValidLongitudeLatitude( vertices[ 0 ], vertices[ 1 ] ),
          name,
          *name,
          isValidColor( color ) );

  BGR bgr = "";
  colorKML( color, KML_COLOR_LEVELS, bgr );

  DEBUG( fprintf( stderr, "writeMapPolylinesToKML: polylineCount = %d...\n",
                  polylineCount ); )

  fprintf( file,
           "    <Placemark>\n"
           "      <name>%s</name>\n"
           "      <description>%s of region of study."
           "</description>\n"
           "      <styleUrl>#ff%s</styleUrl>\n"
           "      <MultiGeometry>\n",
           name, name, bgr );
  {
    const float* longitudes = vertices;
    const float* latitudes  = vertices + 1;
    int polyline = 0;

    for ( polyline = 0; polyline < polylineCount; ++polyline ) {
      const int count = counts[ polyline ];
      int vertex = 0;
      CHECK( count > 1 );
      fprintf( file,
               "        <LineString>\n"
               "          <coordinates>\n" );

      for ( vertex = 0; vertex < count; ++vertex,
            longitudes += 2, latitudes += 2 ) {
        CHECK( isValidLongitudeLatitude( *longitudes, *latitudes ) );
        fprintf( file,
                 "            %f,%f\n", *longitudes, *latitudes );

      }

      fprintf( file,
               "          </coordinates>\n"
               "        </LineString>\n" );
    }

    fprintf( file,
             "      </MultiGeometry>\n"
             "    </Placemark>\n" );
  }
}



/******************************************************************************
PURPOSE: writePolygonsToKML - Write a polygon dataset to KML.
INPUTS:  FILE* file                       File to write to.
         const LongName source            E.g., "HMS".
         const Name name                  E.g., "smoke".
         const Units units                E.g., "ug/m3".
         const int yyyymmddhhStart        Starting timestamp or 0.
         const int yyyymmddhhEnd          Ending timestamp or 0.
         const int count                  Number of polygons.
         const PolygonShape polygons[ count ]  Polygons to write.
         const ShapeData* shapeData       ShapeData for polygons.
         const int column                 Data column of value.
         const double sameValue           If not -9999.0 then use for all
                                          polygons instead of shapeData->values
         const double dataRange[ 2 ]      Minimum and maximum data values.
         const DataColor dataColor        DataColor function to use.
******************************************************************************/

void writePolygonsToKML( FILE* file,
                         const LongName source,
                         const Name name,
                         const Units units,
                         const int yyyymmddhhStart,
                         const int yyyymmddhhEnd,
                         const int count,
                         const PolygonShape polygons[],
                         const ShapeData* shapeData,
                         const int column,
                         const double sameValue,
                         const double dataRange[ 2 ],
                         DataColor dataColor ) {

  PRE017( file,
          OR2( IS_ZERO2( yyyymmddhhStart, yyyymmddhhEnd ),
               AND3( isValidYYYYMMDDHH( yyyymmddhhStart ),
                     isValidYYYYMMDDHH( yyyymmddhhEnd ),
                     yyyymmddhhStart <= yyyymmddhhEnd ) ),
          count > 0, polygons, shapeData, source, *source, name, *name,
          IN_RANGE( column, 0, shapeData->columns - 1 ),
          sameValue >= -9999.0,
          units, *units,
          ! isNan( dataRange[ 0 ] ), ! isNan( dataRange[ 1 ] ),
          dataRange[ MINIMUM ] <= dataRange[ MAXIMUM ],
          dataColor );

  const unsigned char opacity = 127; /* 50% transparent so ground is seen. */
  const double dataMinimum = dataRange[ MINIMUM ];
  const double dataMaximum = dataRange[ MAXIMUM ];
  const int columns = shapeData->columns;
  const int dataType =
    sameValue > -9999.0 ? FTDouble : shapeData->columnTypes[ column ];
  const Value* const values = shapeData->values;
  const int isPolyline = polygons[ 0 ].triangles.num_strips == 0;
  const char* const geoTypeName = isPolyline ? "Polyline" : "Polygon";
  int yyyy1 = yyyymmddhhStart / 1000000;
  int mm1   = yyyymmddhhStart / 10000 % 100;
  int dd1   = yyyymmddhhStart / 100 % 100;
  int hh1   = yyyymmddhhStart % 100;
  int yyyy2 = yyyymmddhhEnd / 1000000;
  int mm2   = yyyymmddhhEnd / 10000 % 100;
  int dd2   = yyyymmddhhEnd / 100 % 100;
  int hh2   = yyyymmddhhEnd % 100;
  int set = 0;
  int index = 0;

  CHECK( IMPLIES_ELSE( sameValue > -9999.0,
                       dataType == FTDouble,
                       IN_RANGE( column, 0, columns - 1 ) ) );

  fprintf( file,
           "    <Folder>\n"
           "      <name>%s_Data:%s_%s(%s)</name>\n"
           "      <description>%s data.</description>\n",
           geoTypeName, source, name, units, geoTypeName );

  for ( set = 0, index = column; set < count; ++set, index += columns ) {
    const PolygonShape* const polygonShape = polygons + set;
    const gpc_vertex_list* const vertexList =
      isPolyline ? polygonShape->polygon.contour
      : polygonShape->triangles.strip;
    const int strips =
      isPolyline ? polygonShape->polygon.num_contours
      : polygonShape->triangles.num_strips;
    const double value =
      sameValue > -9999.0 ? sameValue
      : dataType == FTDouble ? values[ index ].d
      : dataType == FTInteger ? (double) values[ index ].i
      : 0.0;
    Color color = { 0.0, 0.0, 0.0 };
    BGR bgr = "";
    color = dataColor( value, dataMinimum, dataMaximum );

    if ( ! IS_ZERO3( color.r, color.g, color.b ) ) { /* Non-missing data: */
      int strip = 0;
      colorKML( color, KML_COLOR_LEVELS, bgr );
      fprintf( file,
              "      <Placemark>\n" );

      if ( dataType == FTString ) {
        fprintf( file,
                 "        <name>%s</name>\n", values[ index ].s );
      } else if ( dataType == FTInteger ) {
        fprintf( file,
                 "        <name>%d(%s)</name>\n", values[ index ].i, units );
      } else {
        fprintf( file,
                 "        <name>%5.3lg(%s)</name>\n", value, units );
      }

      if ( AND5( shapeData->columns > 4,
                 ! strcmp( shapeData->columnNames[ 0 ], "YYYYDDD1" ),
                 ! strcmp( shapeData->columnNames[ 1 ], "HHMM1" ),
                 ! strcmp( shapeData->columnNames[ 2 ], "YYYYDDD2" ),
                 ! strcmp( shapeData->columnNames[ 3 ], "HHMM2" ) ) ) {
        const int rowOffset = set * columns;
        const int yyyyddd1 = values[ rowOffset     ].i;
        const int hhmm1    = values[ rowOffset + 1 ].i;
        const int yyyyddd2 = values[ rowOffset + 2 ].i;
        const int hhmm2    = values[ rowOffset + 3 ].i;
        int mo1 = 0;
        int mo2 = 0;
        monthAndDay( yyyyddd1, &mo1, &dd1 );
        monthAndDay( yyyyddd2, &mo2, &dd2 );
        yyyy1 = yyyyddd1 / 1000;
        hh1   = hhmm1 / 100;
        mm1   = hhmm1 % 100;
        yyyy2 = yyyyddd2 / 1000;
        hh2   = hhmm2 / 100;
        mm2   = hhmm2 % 100;
        fprintf( file,
                 "        <description>%s %s (%s) during hours "
                 "%04d-%02d-%02d %02d:00 to %04d-%02d-%02d %02d:00 UTC."
                 "</description>\n"
                 "        <TimeSpan>\n"
                 "          <begin>%04d-%02d-%02dT%02d:%02d:00Z</begin>\n"
                 "          <end>%04d-%02d-%02dT%02d:%02d:59Z</end>\n"
                 "        </TimeSpan>\n",
                 source, name, units,
                 yyyy1, mo1, dd1, hh1,
                 yyyy2, mo2, dd2, hh2,
                 yyyy1, mo1, dd1, hh1, mm1,
                 yyyy2, mo2, dd2, hh2, mm2 );
      } else if ( yyyymmddhhStart ) {
        fprintf( file,
                 "        <description>%s %s (%s) at "
                 "%04d-%02d-%02d %02d:00 UTC.</description>\n"
                 "        <TimeSpan>\n"
                 "          <begin>%04d-%02d-%02dT%02d:00:00Z</begin>\n"
                 "          <end>%04d-%02d-%02dT%02d:59:59Z</end>\n"
                 "        </TimeSpan>\n",
                 source, name, units,
                 yyyy1, mm1, dd1, hh1,
                 yyyy1, mm1, dd1, hh1,
                 yyyy2, mm2, dd2, hh2 );
      } else {
        fprintf( file,
                 "        <description>%s %s (%s).</description>\n",
                 source, name, units );
      }

      fprintf( file,
               "        <styleUrl>#%02x%s</styleUrl>\n"
               "        <MultiGeometry>\n",
               opacity, bgr );

      /*
       * For each triangle strip, draw it as a concave polygon by
       * first drawing one edge (even vertices) then drawing the
       * odd vertices starting at the end back to the first vertex.
       */

      for ( strip = 0; strip < strips; ++strip ) {
        const int vertexCount = vertexList[ strip ].num_vertices;
        const gpc_vertex* const v = vertexList[ strip ].vertex;
        int vertex = 0;
        const int lastOddVertex =
          vertexCount % 2 == 0 ? vertexCount - 1 : vertexCount - 2;

        if ( isPolyline ) {
          fprintf( file,
                   "          <LineString>\n"
                   "            <coordinates>\n" );
        } else {
          fprintf( file,
                   "          <Polygon>\n"
                   "            <outerBoundaryIs>\n"
                   "              <LinearRing>\n"
                   "                <coordinates>\n" );
        }

        for ( vertex = 0; vertex < vertexCount; vertex += 2 ) {
          const double x = v[ vertex ].x;
          const double y = v[ vertex ].y;
          fprintf( file, "                %lf,%lf\n", x, y );
        }

        for ( vertex = lastOddVertex; vertex >= 0; vertex -= 2 ) {
          const double x = v[ vertex ].x;
          const double y = v[ vertex ].y;
          fprintf( file, "                %lf,%lf\n", x, y );
        }

        if ( vertex != 0 ) { /* End with first vertex to close polygon: */
          const double x = v[ 0 ].x;
          const double y = v[ 0 ].y;
          fprintf( file, "                %lf,%lf\n", x, y );
        }

        if ( isPolyline ) {
          fprintf( file,
                   "            </coordinates>\n"
                   "          </LineString>\n" );
        } else {
          fprintf( file,
                   "                </coordinates>\n"
                   "              </LinearRing>\n"
                   "            </outerBoundaryIs>\n"
                   "          </Polygon>\n" );
        }
      }

      fprintf( file,
               "        </MultiGeometry>\n"
               "      </Placemark>\n" );
    } /* Non-black (missing data) color. */
  }

  fprintf( file, "    </Folder>\n" );
}



/******************************************************************************
PURPOSE: writeStartFolderKML - Write initial descriptive folder tags to KML.
INPUTS:  FILE* file                     File to write to.
         const LongName source          E.g., "CMAQ".
         const Name name                E.g., "WIND", "O3".
         const Units units              E.g., "m/s", "ppm".
         const char* const dataType     E.g., "Gridded", "Gridded_Vector".
         const char* const description  E.g., "Modeled 3D wind.", "Ozone.".
******************************************************************************/

void writeStartFolderKML( FILE* file,
                          const LongName source,
                          const Name name,
                          const Units units,
                          const char* const dataType,
                          const char* const description ) {

  PRE07( file, source, *source, name, *name, units, *units );

  fprintf( file,
           "    <Folder>\n"
           "      <name>%s_Data:%s_%s(%s)</name>\n"
           "      <description>%s</description>\n",
           dataType, source, name, units, description );
}



/******************************************************************************
PURPOSE: writeEndFolderKML - Write indented ending folder tag to KML.
INPUTS:  FILE* file  File to write to.
******************************************************************************/

void writeEndFolderKML( FILE* file ) {

  PRE0( file );
  fprintf( file, "    </Folder>\n" );
}



/******************************************************************************
PURPOSE: writeCellDataToKML - Write one colored point, quadrilateral or vector
         to KML.
INPUTS:  FILE* file                     File to write to.
         const LongName source          E.g., "CMAQ".
         const Name name                E.g., "WIND", "O3".
         const Units units              E.g., "m/s", "ppm".
         const long long timestamp      One of: yyyy, yyyymm, yyyymmdd,
                                        yyyymmddhh, yyyymmddhhmm.
         const int components           1 = scalar 2 or 3 is vector.
         const double missing           Value that indicates missing.
         const double datum             Value for cell.
         const double datum2            missing or 2nd vector component value.
         const double datum3            missing or 3rd vector component value.
         const int vertices             1 = point, 4 = quadrilateral,
                                        8 = hexahedron.
         const double coordinates[ vertices * 3 ] Counter-clockwise
                                                  (right-hand-rule) vertex
                                                  order coordinates for
                                                  quad cell faces.
         const Color color                        Color of cell.
******************************************************************************/

void writeCellDataToKML( FILE* file,
                         const LongName source,
                         const Name name,
                         const Units units,
                         const long long timestamp,
                         const int components,
                         const double missing,
                         const double datum,
                         const double datum2,
                         const double datum3,
                         const int vertices,
                         const double coordinates[],
                         const Color color ) {

  PRE017( file,
          source, *source, name, *name, units, *units,
          timestamp < 3000 ? isYear( timestamp )
            : timestamp < 300012 ? isValidYearMonthDay( timestamp * 100 + 1 )
            : timestamp < 30001231 ? isValidYearMonthDay( timestamp )
            : timestamp < 3000123123LL ? isValidYYYYMMDDHH( timestamp )
            : AND2( isValidYYYYMMDDHH( timestamp / 100 ),
                    isValidTime( timestamp % 10000 * 100 ) ),
          IN_RANGE( components, 0, 3 ),
          datum > missing, datum2 >= missing, datum3 >= missing,
          IN4( vertices, 1, 4, 8 ),
          coordinates,
          isValidLongitudeLatitude( coordinates[ 0 ], coordinates[ 1 ] ),
          isValidLongitudeLatitude( coordinates[ ( vertices - 1 ) * 3 ],
                                    coordinates[ ( vertices - 1 ) * 3 + 1 ] ),
          isValidColor( color ) );

  const double degreesPerPixel = 0.001;
  const double pixelsPerUnitLength = ! strcmp( name, "wind" ) ? 5.0 : 20.0;
  const unsigned char opacity = components == 1 ? 127 : 255;
  /* 50% transparent for polygons so ground is seen. */
  double value = datum;
  BGR bgr = "";
  colorKML( color, KML_COLOR_LEVELS, bgr );

  /* Compute cell datum magnitude: */

  if ( datum2 > missing ) {
    value *= value;
    value += datum2 * datum2;

    if ( datum3 > missing ) {
      value += datum3 * datum3;
    }

    value = sqrt( value );
  }

  /* Create placemark with value, description and time info: */

  if ( timestamp < 3000 ) { /* Yearly data: */
    const int yyyy = timestamp;
    fprintf( file,
             "      <Placemark>\n"
             "        <name>%g(%s)</name>\n"
             "        <description>%s %s (%s) for year "
             "%04d.</description>\n"
             "        <TimeSpan>\n"
             "          <begin>%04d-01-01dT00:00:00Z</begin>\n"
             "          <end>%04d-12-31dT00:00:00Z</end>\n"
             "        </TimeSpan>\n"
             "        <styleUrl>#%02x%s</styleUrl>\n",
            value, units,
            source, name, units, yyyy,
            yyyy,
            yyyy,
            opacity, bgr );

  } else if ( timestamp < 300012 ) { /* Monthly data: */
    const int yyyy = timestamp / 100;
    const int mm   = timestamp % 100;
    const int dd = daysInMonth( yyyy, mm );
    fprintf( file,
             "      <Placemark>\n"
             "        <name>%g(%s)</name>\n"
             "        <description>%s %s (%s) for month "
             "%04d-%02d.</description>\n"
             "        <TimeSpan>\n"
             "          <begin>%04d-%02d-01dT00:00:00Z</begin>\n"
             "          <end>%04d-%02d-%02dT00:00:00Z</end>\n"
             "        </TimeSpan>\n"
             "        <styleUrl>#%02x%s</styleUrl>\n",
            value, units,
            source, name, units, yyyy, mm,
            yyyy, mm,
            yyyy, mm, dd,
            opacity, bgr );
  } else if ( timestamp < 30001231 ) { /* Daily data: */
    const int yyyy = timestamp / 10000;
    const int mm   = timestamp / 100 % 100;
    const int dd   = timestamp % 100;
    fprintf( file,
             "      <Placemark>\n"
             "        <name>%g(%s)</name>\n"
             "        <description>%s %s (%s) for day "
             "%04d-%02d-%02d UTC.</description>\n"
             "        <TimeSpan>\n"
             "          <begin>%04d-%02d-%02dT00:00:00Z</begin>\n"
             "          <end>%04d-%02d-%02dT23:59:59Z</end>\n"
             "        </TimeSpan>\n"
             "        <styleUrl>#%02x%s</styleUrl>\n",
            value, units,
            source, name, units,
            yyyy, mm, dd,
            yyyy, mm, dd,
            yyyy, mm, dd,
            opacity, bgr );
  } else if ( timestamp < 3000123123LL ) { /* Hourly data: */
    const int yyyy = timestamp / 1000000;
    const int mm   = timestamp / 10000 % 100;
    const int dd   = timestamp / 100 % 100;
    const int hh   = timestamp % 100;
    fprintf( file,
             "      <Placemark>\n"
             "        <name>%g(%s)</name>\n"
             "        <description>%s %s (%s) for hour "
            "%04d-%02d-%02d %02d:00 UTC.</description>\n"
             "        <TimeSpan>\n"
             "          <begin>%04d-%02d-%02dT%02d:00:00Z</begin>\n"
             "          <end>%04d-%02d-%02dT%02d:59:59Z</end>\n"
             "        </TimeSpan>\n"
             "        <styleUrl>#%02x%s</styleUrl>\n",
            value, units,
            source, name, units,
            yyyy, mm, dd, hh,
            yyyy, mm, dd, hh,
            yyyy, mm, dd, hh,
            opacity, bgr );
  } else { /* Sub-hourly YYYYMMDDHHMM data: */
    const int yyyy = timestamp / 100000000;
    const int mo   = timestamp / 1000000 % 100;
    const int dd   = timestamp / 10000 % 100;
    const int hh   = timestamp / 100 % 100;
    const int mm   = timestamp % 100;
    fprintf( file,
             "      <Placemark>\n"
             "        <name>%g(%s)</name>\n"
             "        <description>%s %s (%s) for time "
            "%04d-%02d-%02d %02d:%02d UTC.</description>\n"
             "        <TimeSpan>\n"
             "          <begin>%04d-%02d-%02dT%02d:%02d:00Z</begin>\n"
             "          <end>%04d-%02d-%02dT%02d:%02d:59Z</end>\n"
             "        </TimeSpan>\n"
             "        <styleUrl>#%02x%s</styleUrl>\n",
            value, units,
            source, name, units,
            yyyy, mo, dd, hh, mm,
            yyyy, mo, dd, hh, mm,
            yyyy, mo, dd, hh, mm,
            opacity, bgr );
  }

  /* Write coordinates: */

  if ( AND2( vertices == 1, components == 1 ) ) { /* Point cell: draw ruler. */
    const double longitude = coordinates[ 0 ];
    const double latitude  = coordinates[ 1 ];
    const double altitude0 = coordinates[ 2 ];
    const double altitude  = altitude0 > missing ? altitude0 : 0.0;
    const char* const altitudeMode = altitude ? "absolute" : "clampToGround";

    fprintf( file,
             "        <Point>\n"
             "          <altitudeMode>%s</altitudeMode>\n"
             "          <coordinates>%f,%f,%f</coordinates>\n"
             "        </Point>\n"
             "      </Placemark>\n",
             altitudeMode,
             longitude, latitude, altitude );
  } else if ( IN3( components, 2, 3 ) ) { /* Vector cell: draw as arrow. */
    const double u = datum;
    const double v = datum2;
    const double longitude = coordinates[ 0 ];
    const double latitude  = coordinates[ 1 ];
    const double altitude  = coordinates[ 2 ];
    const double altitude2 = datum3 > missing ? altitude + datum3 : altitude;
    const char* const altitudeMode = altitude ? "absolute" : "clampToGround";
    double point0X = 0.0;
    double point0Y = 0.0;
    double point1X = 0.0;
    double point1Y = 0.0;
    double point2X = 0.0;
    double point2Y = 0.0;
    double point3X = 0.0;
    double point3Y = 0.0;
    /* UNIMPLEMENTED: 3D vector arrow. */
    computeArrowVectorCoordinates( longitude, latitude,
                                   u, v,
                                   degreesPerPixel,
                                   pixelsPerUnitLength,
                                   &point0X, &point0Y,
                                   &point1X, &point1Y,
                                   &point2X, &point2Y,
                                   &point3X, &point3Y );
    fprintf( file,
             "        <LineString>\n"
             "          <altitudeMode>%s</altitudeMode>\n"
             "          <coordinates>\n"
             "            %f,%f,%f\n"
             "            %f,%f,%f\n"
             "            %f,%f,%f\n"
             "            %f,%f,%f\n"
             "            %f,%f,%f\n"
             "          </coordinates>\n"
             "        </LineString>\n"
             "      </Placemark>\n",
             altitudeMode,
             point0X, point0Y, altitude,
             point1X, point1Y, altitude2,
             point2X, point2Y, altitude2,
             point1X, point1Y, altitude2,
             point3X, point3Y, altitude2 );
  } else if ( vertices == 4 ) { /* Quadrilateral: */

          /*
           * Ensure the quad's normal points upward by using
           * right-hand-rule of vertex order.
           * Note: stored cell vertex order is counter-clockwise:
           *   v0 = (west, south)
           *   v1 = (east, south)
           *   v2 = (east, north)
           *   v3 = (west, north)
           *
           *   v3-------v2
           *   |        |
           *   |        |
           *   |        |
           *   v0-------v1
           */

    const double longitudeSW = coordinates[ 0 ];
    const double latitudeSW  = coordinates[ 1 ];
    const double altitudeSW  = coordinates[ 2 ] > missing ? coordinates[2] :0.0;
    const double longitudeSE = coordinates[ 3 ];
    const double latitudeSE  = coordinates[ 4 ];
    const double altitudeSE  = coordinates[ 5 ] > missing ? coordinates[5] :0.0;
    const double longitudeNE = coordinates[ 6 ];
    const double latitudeNE  = coordinates[ 7 ];
    const double altitudeNE  = coordinates[ 8 ] > missing ? coordinates[8] :0.0;
    const double longitudeNW = coordinates[ 9 ];
    const double latitudeNW  = coordinates[ 10 ];
    const double altitudeNW  = coordinates[ 11] > missing ? coordinates[11]:0.0;
    const char* const altitudeMode =
      OR4( altitudeSW, altitudeSE, altitudeNE, altitudeNW ) ? "absolute"
      : "relativeToGround";
    fprintf( file,
             "        <Polygon>\n"
             "          <altitudeMode>%s</altitudeMode>\n"
             "          <outerBoundaryIs>\n"
             "            <LinearRing>\n"
             "              <coordinates>\n"
             "                %f,%f,%f\n"
             "                %f,%f,%f\n"
             "                %f,%f,%f\n"
             "                %f,%f,%f\n"
             "                %f,%f,%f\n"
             "              </coordinates>\n"
             "            </LinearRing>\n"
             "          </outerBoundaryIs>\n"
             "        </Polygon>\n"
             "      </Placemark>\n",
            altitudeMode,
            longitudeSW, latitudeSW, altitudeSW,
            longitudeSE, latitudeSE, altitudeSE,
            longitudeNE, latitudeNE, altitudeNE,
            longitudeNW, latitudeNW, altitudeNW,
            longitudeSW, latitudeSW, altitudeSW );
  } else { /* Hexahedron: */

    /*
     * Ensure each cell face's normal points outward from cell center by using
     * right-hand-rule of vertex order.
     * Note: stored cell vertex order is:
     *   v0 = (west, south, lower)
     *   v1 = (east, south, lower)
     *   v2 = (east, north, lower)
     *   v3 = (west, north, lower)
     *   v4 = (west, south, upper)
     *   v5 = (east, south, upper)
     *   v6 = (east, north, upper)
     *   v7 = (west, north, upper)
     *
     *             v7---------v6
     *            / |         /|
     *           /  |        / |
     *          /   |       /  |
     *        v4----------v5   |
     *         |   /v3----|---|v2
     *         |  /       |  /
     *         | /        | /
     *         |/         |/
     *        v0---------v1
     *
     * Draw faces in this order:
     * top, bottom, south, north, west, east.
     */

    enum { ELEVATION = LATITUDE + 1 };
    const double* const v0 = coordinates;
    const double* const v1 = v0 + 3;
    const double* const v2 = v1 + 3;
    const double* const v3 = v2 + 3;
    const double* const v4 = v3 + 3;
    const double* const v5 = v4 + 3;
    const double* const v6 = v5 + 3;
    const double* const v7 = v6 + 3;
    const double v0_longitude = v0[ LONGITUDE ];
    const double v1_longitude = v1[ LONGITUDE ];
    const double v2_longitude = v2[ LONGITUDE ];
    const double v3_longitude = v3[ LONGITUDE ];
    const double v4_longitude = v4[ LONGITUDE ];
    const double v5_longitude = v5[ LONGITUDE ];
    const double v6_longitude = v6[ LONGITUDE ];
    const double v7_longitude = v7[ LONGITUDE ];
    const double v0_latitude  = v0[ LATITUDE ];
    const double v1_latitude  = v1[ LATITUDE ];
    const double v2_latitude  = v2[ LATITUDE ];
    const double v3_latitude  = v3[ LATITUDE ];
    const double v4_latitude  = v4[ LATITUDE ];
    const double v5_latitude  = v5[ LATITUDE ];
    const double v6_latitude  = v6[ LATITUDE ];
    const double v7_latitude  = v7[ LATITUDE ];
    const double v0_elevation = v0[ ELEVATION ];
    const double v1_elevation = v1[ ELEVATION ];
    const double v2_elevation = v2[ ELEVATION ];
    const double v3_elevation = v3[ ELEVATION ];
    const double v4_elevation = v4[ ELEVATION ];
    const double v5_elevation = v5[ ELEVATION ];
    const double v6_elevation = v6[ ELEVATION ];
    const double v7_elevation = v7[ ELEVATION ];

    /* Draw multi-geometry: a set of 6 quadrilaterals: */

    fprintf( file,
            "        <MultiGeometry>\n" );

    /* Draw top face - v4, v5, v6, v7 - normal points up. */


    fprintf( file,
             "          <Polygon>\n"
             "            <altitudeMode>absolute</altitudeMode>\n"
             "            <outerBoundaryIs>\n"
             "              <LinearRing>\n"
             "                <coordinates>\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                </coordinates>\n"
             "              </LinearRing>\n"
             "            </outerBoundaryIs>\n"
             "          </Polygon>\n",
             v4_longitude, v4_latitude, v4_elevation,
             v5_longitude, v5_latitude, v5_elevation,
             v6_longitude, v6_latitude, v6_elevation,
             v7_longitude, v7_latitude, v7_elevation,
             v4_longitude, v4_latitude, v4_elevation );

    /* Draw bottom face - v3, v2, v1, v0 - normal points down. */

    fprintf( file,
             "          <Polygon>\n"
             "            <altitudeMode>absolute</altitudeMode>\n"
             "            <outerBoundaryIs>\n"
             "              <LinearRing>\n"
             "                <coordinates>\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                </coordinates>\n"
             "              </LinearRing>\n"
             "            </outerBoundaryIs>\n"
             "          </Polygon>\n",
             v3_longitude, v3_latitude, v3_elevation,
             v2_longitude, v2_latitude, v2_elevation,
             v1_longitude, v1_latitude, v1_elevation,
             v0_longitude, v0_latitude, v0_elevation,
             v3_longitude, v3_latitude, v3_elevation );

    /* Draw south face - v0, v1, v5, v4 - normal points south. */

    fprintf( file,
             "          <Polygon>\n"
             "            <altitudeMode>absolute</altitudeMode>\n"
             "            <outerBoundaryIs>\n"
             "              <LinearRing>\n"
             "                <coordinates>\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                </coordinates>\n"
             "              </LinearRing>\n"
             "            </outerBoundaryIs>\n"
             "          </Polygon>\n",
             v0_longitude, v0_latitude, v0_elevation,
             v1_longitude, v1_latitude, v1_elevation,
             v5_longitude, v5_latitude, v5_elevation,
             v4_longitude, v4_latitude, v4_elevation,
             v0_longitude, v0_latitude, v0_elevation );

    /* Draw north face - v2, v3, v7, v6 - normal points north. */

    fprintf( file,
             "          <Polygon>\n"
             "            <altitudeMode>absolute</altitudeMode>\n"
             "            <outerBoundaryIs>\n"
             "              <LinearRing>\n"
             "                <coordinates>\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                </coordinates>\n"
             "              </LinearRing>\n"
             "            </outerBoundaryIs>\n"
             "          </Polygon>\n",
             v2_longitude, v2_latitude, v2_elevation,
             v3_longitude, v3_latitude, v3_elevation,
             v7_longitude, v7_latitude, v7_elevation,
             v6_longitude, v6_latitude, v6_elevation,
             v2_longitude, v2_latitude, v2_elevation );

    /* Draw west face - v3, v0, v4, v7 - normal points west. */

    fprintf( file,
             "          <Polygon>\n"
             "            <altitudeMode>absolute</altitudeMode>\n"
             "            <outerBoundaryIs>\n"
             "              <LinearRing>\n"
             "                <coordinates>\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                </coordinates>\n"
             "              </LinearRing>\n"
             "            </outerBoundaryIs>\n"
             "          </Polygon>\n",
             v3_longitude, v3_latitude, v3_elevation,
             v0_longitude, v0_latitude, v0_elevation,
             v4_longitude, v4_latitude, v4_elevation,
             v7_longitude, v7_latitude, v7_elevation,
             v3_longitude, v3_latitude, v3_elevation );

    /* Draw east face - v1, v2, v6, v5 - normal points west. */

    fprintf( file,
             "          <Polygon>\n"
             "            <altitudeMode>absolute</altitudeMode>\n"
             "            <outerBoundaryIs>\n"
             "              <LinearRing>\n"
             "                <coordinates>\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                  %f,%f,%f\n"
             "                </coordinates>\n"
             "              </LinearRing>\n"
             "            </outerBoundaryIs>\n"
             "          </Polygon>\n",
             v1_longitude, v1_latitude, v1_elevation,
             v2_longitude, v2_latitude, v2_elevation,
             v6_longitude, v6_latitude, v6_elevation,
             v5_longitude, v5_latitude, v5_elevation,
             v1_longitude, v1_latitude, v1_elevation );

    fprintf( file,
             "        </MultiGeometry>\n"
             "      </Placemark>\n" );
  }
}



/*============================= PRIVATE FUNCTIONS ===========================*/



/******************************************************************************
PURPOSE: isValidwriteMapSubsetToKML - Check MapImageFileInfo object.
INPUTS:  const MapImageFileInfo* const info  Object to check.
RETURNS: int 1 if valid, else 0.
******************************************************************************/

static int isValidMapImageFileInfo( const MapImageFileInfo* const info ) {
  const int result = 
    AND11( info, info->file,
           info->inputDirectory, info->inputDirectory[ 0 ],
           info->outputDirectory, info->outputDirectory[ 0 ],
           info->startsWith, info->startsWith[ 0 ],
           info->endsWith, info->endsWith[ 0 ],
           isValidBounds( info->bounds ) );
  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: writeMapImagesToKMLHelper - Write map_image_yyyymmdd.png file to KML.
INPUTS:  const char* const fileName   File name map_image_yyyymmdd.png.
         void* data                   MapImageFileInfo.
******************************************************************************/

static void writeMapImagesToKMLHelper( const char* const fileName,
                                       void* data ) {

  PRE04( fileName, *fileName, data,
         isValidMapImageFileInfo( (const MapImageFileInfo*) data ) );

  const MapImageFileInfo* const info = (const MapImageFileInfo*) data;
  FILE* const file = info->file;
  const char* const inputDirectory = info->inputDirectory;
  const char* const outputDirectory = info->outputDirectory;
  const char* const startsWith = info->startsWith;
  const char* const endsWith = info->endsWith;
  const double west  = info->bounds[ LONGITUDE ][ MINIMUM ];
  const double east  = info->bounds[ LONGITUDE ][ MAXIMUM ];
  const double south = info->bounds[ LATITUDE  ][ MINIMUM ];
  const double north = info->bounds[ LATITUDE  ][ MAXIMUM ];
  const size_t startLength = strlen( startsWith );
  const size_t fileNameLength = strlen( fileName );

  if ( fileNameLength > startLength ) {
    const char* const yyyymmddString = fileName + startLength;
    const int yyyymmdd = (int) atoi( yyyymmddString );
    const int yyyy = yyyymmdd / 10000;
    const int mm   = yyyymmdd / 100 % 100;
    const int dd   = yyyymmdd % 100;

    if ( isValidYearMonthDay( yyyymmdd ) ) {
      char inputFileName[ 256 ] = "";
      char outputFileName[ 256 ] = "";
      memset( inputFileName, 0, sizeof inputFileName );
      memset( outputFileName, 0, sizeof outputFileName );
      snprintf( inputFileName,
                sizeof inputFileName / sizeof *inputFileName - 1,
                "%s/%s%d%s", inputDirectory, startsWith, yyyymmdd,  endsWith );
      snprintf( outputFileName,
                sizeof outputFileName / sizeof *outputFileName - 1,
                "%s/%s%d%s", outputDirectory, startsWith, yyyymmdd,  endsWith );

      if ( OR2( ! strcmp( inputFileName, outputFileName ),
                copyFile( inputFileName, outputFileName ) ) ) {
        const char* const slash = strrchr( outputFileName, '/' );
        const char* const name = slash ? slash + 1 : outputFileName;
        fprintf( file, "      <GroundOverlay>\n" );
        fprintf( file, "        <name>image_%d</name>\n", yyyymmdd );
        fprintf( file, "        <description>Satellite image on "
                 "%d-%02d-%02d UTC.</description>\n", yyyy, mm, dd );
        fprintf( file, "        <TimeSpan>\n" );
        fprintf( file, "          <begin>%d-%02d-%02dT00:00:00Z</begin>\n",
                 yyyy, mm, dd );
        fprintf( file, "          <end>%d-%02d-%02dT23:59:59Z</end>\n",
                 yyyy, mm, dd );
        fprintf( file, "        </TimeSpan>\n" );
        fprintf( file, "        <Icon>\n" );
        fprintf( file, "          <href>%s</href>\n", name );
        fprintf( file, "        </Icon>\n" );
        fprintf( file, "        <LatLonBox>\n" );
        fprintf( file, "          <north>%lf</north>\n", north );
        fprintf( file, "          <south>%lf</south>\n", south );
        fprintf( file, "          <east>%lf</east>\n", east );
        fprintf( file, "          <west>%lf</west>\n", west );
        fprintf( file, "        </LatLonBox>\n" );
        fprintf( file, "      </GroundOverlay>\n" );
      }
    }
  }
}



/******************************************************************************
PURPOSE: writeKMLColors - Write color styles to KML file.
INPUTS:  FILE* file  KML file to write to.
int levels  Number of levels per color component.
******************************************************************************/

static void writeKMLColors( FILE* file, int levels ) {

  PRE02( file, 256 % levels == 0 );

  const int increment = 256 / levels;
  int alpha = 0;

  for ( alpha = 127; alpha < 256; alpha += 128 ) {
    int blue = 0;

    for ( blue = 0; blue <= 256; blue += increment ) {
      int green = 0;
      blue = blue == 256 ? 255 : blue;

      for ( green = 0; green <= 256; green += increment ) {
        int red = 0;
        green = green == 256 ? 255 : green;

        for ( red = 0; red <= 256; red += increment ) {
          red = red == 256 ? 255 : red;

          fprintf( file,
                  "    <Style id=\"%02x%02x%02x%02x\">\n"
                  "      <IconStyle>\n"
                  "        <color>%02x%02x%02x%02x</color>\n"
                  "        <Icon>\n"
                  "          <href>"
                  "http://maps.google.com/mapfiles/kml/pal5/icon5.png"
                  "</href>\n"
                  "        </Icon>\n"
                  "      </IconStyle>\n"
                  "      <LabelStyle>\n"
                  "        <color>%02x%02x%02x%02x</color>\n"
                  "      </LabelStyle>\n"
                  "      <LineStyle>\n"
                  "        <color>%02x%02x%02x%02x</color>\n"
                  "        <altitudeMode>absolute</altitudeMode>\n"
                  "        <width>2</width>\n"
                  "      </LineStyle>\n"
                  "      <PointStyle>\n"
                  "        <color>%02x%02x%02x%02x</color>\n"
                  "        <altitudeMode>absolute</altitudeMode>\n"
                  "      </PointStyle>\n"
                  "      <PolyStyle>\n"
                  "        <color>%02x%02x%02x%02x</color>\n"
                  "        <altitudeMode>absolute</altitudeMode>\n"
                  "        <outline>0</outline>\n"
                  "      </PolyStyle>\n"
                  "    </Style>\n",
                  alpha, blue, green, red,
                  alpha, blue, green, red,
                  alpha, blue, green, red,
                  alpha, blue, green, red,
                  alpha, blue, green, red,
                  alpha, blue, green, red );
        }
      }
    }
  }
}



/******************************************************************************
PURPOSE: colorKML - Compute KML color string from Color.
INPUTS:  Color color  Color to convert to KML.
         int levels   Number of levels per color component.
OUTPUTS: BGR bgr      KML bblue-green-red color string.
******************************************************************************/

static void colorKML( Color color, int levels, BGR result ) {
  PRE02( isValidColor( color ), result );
  const int increment = 256 / levels;
  int b = (int) ( color.b * 255 );
  int g = (int) ( color.g * 255 );
  int r = (int) ( color.r * 255 );
  b = (int) ( (float) b / increment + 0.5 );
  b *= increment;
  b = b < 256 ? b : 255;
  g = (int) ( (float) g / increment + 0.5 );
  g *= increment;
  g = g < 256 ? g : 255;
  r = (int) ( (float) r / increment + 0.5 );
  r *= increment;
  r = r < 256 ? r : 255;
  snprintf( result, sizeof (BGR) / sizeof (char), "%02x%02x%02x", b, g, r );
  POST03( strlen( result ) == 6,
          strcmp( result, "000000" ) >= 0,
          strcmp( "ffffff", result ) >= 0 );
}



