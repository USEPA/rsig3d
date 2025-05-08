
#ifndef KMLFILE_H
#define KMLFILE_H


#ifdef __cplusplus
extern "C" {
#endif
  
/******************************************************************************
PURPOSE: KMLFile.h - Declare routines for KML file creation.

NOTES:   

HISTORY: 2010/12/22 Todd Plessel EPA/LM Created.

STATUS:  unreviewed, untested.
******************************************************************************/

/*================================= INCLUDES ================================*/

#include <stdio.h> /* For FILE. */

#include <Utilities.h> /* For Bounds, Color. */
#include <Shapefile.h> /* For PolygonShape, ShapeData. */

/*================================ FUNCTIONS ================================*/

/* Call once when KML file is created: */

extern void writeStartKML( FILE* file,
                           const char* name, const char* description,
                           const Bounds bounds );

extern void writeEndKML( FILE* file ); /* Call just before closing KML file. */

/* Write a set of map image files (modisTrueColoryyyymmdd.png): */

extern void writeMapImagesToKML( FILE* file,
                                 const char* const inputDirectory,
                                 const char* const startsWith,
                                 const char* const endsWith,
                                 const char* const outputDirectory,
                                 const Bounds bounds );

/* Writes a set of map polylines: */

extern void writeMapPolylinesToKML( FILE* file,
                                    int polylineCount,
                                    const int counts[],
                                    const float vertices[],
                                    const char* name,
                                    Color color );
/* Write a polygons dataset: */

extern void writePolygonsToKML( FILE* file,
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
                                DataColor dataColor);

/* For non-polygons datasets, call once per dataset: */

extern void writeStartFolderKML( FILE* file,
                                 const LongName source,
                                 const Name name,
                                 const Units units,
                                 const char* const dataType,
                                 const char* const description );

extern void writeEndFolderKML( FILE* file ); /* Call at end of dataset. */

/* Call once per cell of dataset: cell will be point, vector, quad or hex. */

extern void writeCellDataToKML( FILE* file,
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
                                const Color color );


#ifdef __cplusplus
}
#endif
    
#endif /* KMLFILE_H */


