
/******************************************************************************
PURPOSE: LoadMapFile.C - Read binary file of map polylines into a display-list.
HISTORY: 2014-09-02 plessel.todd@epa.gov
******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h>  // For FILE, stderr, fprintf(), fscanf(), fread(), fclose().

#include <vector> // For std::vector.

#ifdef __APPLE__
#include <OpenGL/gl.h> // For GL*, gl*().
#else
#include <GL/gl.h> // For GL*, gl*().
#endif

#include <Assertions.h>      // For PRE*(), POST*(), AND*(), DEBUG().
#include <DataUtilities.h>   // For emvl::reverse4ByteWordsIfLittleEndian().
#include <ResourceHandler.h> // For emvl::ResourceHandler.
#include "LoadMapFile.h"     // For public interface.

//=========================== FORWARD DECLARATIONS ============================

static bool readMapFileHeader( FILE* mapFile,
                              int* polylineCount, int* vertexCount );

static bool readMapFileData( FILE* mapFile,
                             const int polylineCount, const int vertexCount,
                             int counts[], float vertices[] );

static GLuint makePolylinesDisplayList( const int polylineCount,
                                        const int counts[],
                                        const float vertices[] );

//============================= PUBLIC FUNCTIONS ==============================



/******************************************************************************
PURPOSE: loadMapFile - Read binary file of map polylines into a display-list.
INPUTS:  const char* const mapFileName  Name of map_*.bin file to read.
RETURNS: unsigned int OpenGL display-list id if successful,
         else 0 and a failure message is printed.
******************************************************************************/

unsigned int loadMapFile( const char* mapFileName ) {
  PRE02( mapFileName, *mapFileName );
  emvl::ResourceHandler<FILE*> fileCloser( fopen( mapFileName, "rb" ), fclose );
  FILE* const mapFile = fileCloser.get();
  unsigned int result = 0;

  if ( mapFile ) {
    int polylineCount = 0;
    int vertexCount = 0;
    bool ok = readMapFileHeader( mapFile, &polylineCount, &vertexCount );

    if ( ok ) {
      std::vector<int> countsStorage( polylineCount ); //x
      std::vector<float> verticesStorage( vertexCount * 2 ); //x
      int* const counts = &countsStorage[ 0 ];
      float* const vertices = &verticesStorage[ 0 ];
      ok = readMapFileData( mapFile, polylineCount, vertexCount,
                            counts, vertices );

      if ( ok ) {
        result = makePolylinesDisplayList( polylineCount, counts, vertices );
      }
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: readMapFileHeader - Read header dimensions of map_*.bin file.
INPUTS:  FILE* file  Opened readable file.
OUTPUTS: int& polylineCount  Number of polylines in file.
         int& vertexCount    Number of lon-lat vertices in file.
RETURNS: bool true if successful, else false and a failure message is printed.
NOTES:   Map file headers look like this:
Content-type: application/octet-stream; charset=iso-8859-1
# Dimensions: polyline_count vertex_count
463 11986
# IEEE-754/MSB 32-bit int counts[polyline_count]; \
float vertices[vertex_count][2=lon,lat]:
******************************************************************************/

static bool readMapFileHeader( FILE* file,
                               int* polylineCount, int* vertexCount ) {
  PRE03( file, polylineCount, vertexCount );
  const bool result =
    AND3( fscanf( file, "%*[^\n]\n%*[^\n]\n%d %d\n%*[^\n]%*c",
                  polylineCount, vertexCount ) == 2,
          *polylineCount > 0,
          *vertexCount > 0 );

  if ( ! result ) {
    fprintf( stderr, "\nInvalid map file header.\n" );
    *polylineCount = *vertexCount = 0;
  }

  POST0( IMPLIES_ELSE( result,
                      GT_ZERO2( *polylineCount, *vertexCount ),
                      IS_ZERO2( *polylineCount, *vertexCount ) ) );
 return result;
}



/******************************************************************************
PURPOSE: readMapFileData - Read data of map_*.bin file.
INPUTS:  FILE* file  Opened readable file, seeked to data.
         const int polylineCount  Number of polylines in file.
         const int vertexCount    Number of lon-lat vertex pairs in file.
OUTPUTS: int counts[ polylineCount ]       Array of vertex counts per polyline.
         float vertices[ vertexCount * 2 ] Array of lon-lat vertices.
RETURNS: bool true if successful, else false and a failure message is printed.
******************************************************************************/

static bool readMapFileData( FILE* file,
                             const int polylineCount, const int vertexCount,
                             int counts[], float vertices[] ) {
  PRE05( file, polylineCount > 0, vertexCount > 0, counts, vertices );
  DEBUG( assert_static( sizeof (int) == 4 ); )
  DEBUG( assert_static( sizeof (float) == 4 ); )
  const int vertexValues = vertexCount + vertexCount;
  bool result = fread( counts, polylineCount * sizeof (int), 1, file ) == 1;

  if ( result ) {
    result = fread( vertices, vertexValues * sizeof (float), 1, file ) == 1;

    if ( result ) {
      emvl::reverse4ByteWordsIfLittleEndian( counts, polylineCount );
      emvl::reverse4ByteWordsIfLittleEndian( vertices, vertexValues );
    }
  }

  if ( ! result ) {
    fprintf( stderr, "\nInvalid map file data.\n" );
  }

  POST0( IMPLIES( result,
                  AND5( emvl::minimum( counts, polylineCount ) > 0,
                        emvl::isLongitude( vertices[ 0 ] ),
                        emvl::isLongitude( vertices[ vertexCount * 2 - 2 ] ),
                        emvl::isLatitude( vertices[ 1 ] ),
                        emvl::isLatitude( vertices[ vertexCount * 2 - 1 ] ))));
                      
  return result;
}



/******************************************************************************
PURPOSE: makePolylinesDisplayList - Make a display list of polylines.
INPUTS:  const int polylineCount            Number of polylines.
         const int counts[ polylineCount ]  Number of vertices per polyline.
         const float vertices[]             Lon-lat pairs.
RETURNS: GLuint display list id or 0 if failed.
******************************************************************************/

static GLuint makePolylinesDisplayList( int polylineCount,
                                        const int counts[],
                                        const float vertices[] ) {
  GLuint result = 0;
  int polyline = 0;
  int offset = 0;
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 2, GL_FLOAT, 0, vertices );

  result = glGenLists( 1 );
  DEBUG( fprintf( stderr, "gen list = %d\n", result ); )
  glNewList( result, GL_COMPILE );

  for ( polyline = 0; polyline < polylineCount; ++polyline ) {
    const int thisPolylineVertexCount = counts[ polyline ];

    glDrawArrays( GL_LINE_STRIP, offset, thisPolylineVertexCount );
    offset += thisPolylineVertexCount;
  }

  glEndList();
  glDisableClientState( GL_VERTEX_ARRAY );
  return result;
}



