
#ifndef MAPFILE_H
#define MAPFILE_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
PURPOSE: MapFile.h - Declare routines to read .bin map polyline files.
HISTORY: 2018-11-24 plessel.todd@epa.gov
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h> /* For FILE. */

extern int readMapFileHeader(FILE* file, int* polylineCount, int* vertexCount);

extern int readMapFileData( FILE* file,
                            int polylineCount, int vertexCount,
                            int counts[], float vertices[] );

extern void subsetMapFloat( int inputPolylineCount,
                            int inputVertexCount,
                            const int inputCounts[],
                            const float inputVertices[],
                            double resolution,
                            const double bounds[ 2 ][ 2 ],
                            int* outputPolylineCount,
                            int* outputVertexCount,
                            int outputCounts[],
                            float outputVertices[] );

extern void subsetMapDouble( int inputPolylineCount,
                             int inputVertexCount,
                             const int inputCounts[],
                             const double inputVertices[],
                             double resolution,
                             const double bounds[ 2 ][ 2 ],
                             int* outputPolylineCount,
                             int* outputVertexCount,
                             int outputCounts[],
                             double outputVertices[] );

#ifdef __cplusplus
}
#endif

#endif /* MAPFILE_H */

