
#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
PURPOSE: Utilities.h - Declare utility routines.
HISTORY: 2009-05-26 plessel.todd@epa.gov
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h>  /* For FILE. */
#include <stdlib.h> /* For free(). */

#ifdef _WIN32
extern double drand48( void );
extern char* strtok_r( char*, const char*, char** );
#endif

/*================================= MACROS ==================================*/

/* Value clamped to range [low, high]. */

#define CLAMPED_TO_RANGE( value, low, high ) \
((value) < (low) ? (low) : (value) > (high) ? (high) : (value))

/* Allocate then zero-out the memory: */

#define NEW( type, count ) ( (type*) newMemory( (count), sizeof (type) ) )
extern void* newMemory( size_t count, size_t bytesEach );

/* Deallocate the memory and zero the pointer: */

#define FREE( p ) ( ( (p) ? free(p) : (void) 0 ), (p) = 0 )

/*================================== TYPES ==================================*/

enum { MINIMUM, MAXIMUM };
enum { LONGITUDE, LATITUDE };

enum { BYTE_TYPE, UINT16_TYPE, FLOAT_TYPE, DOUBLE_TYPE, GRID_DATA_TYPES };

#define IS_VALID_GRID_DATA_TYPE(type) ((type) >= 0 && (type) < GRID_DATA_TYPES)

enum { HOURLY, DAILY, MONTHLY, YEARLY }; /* Timestep of data. */

#define IS_VALID_TIMESTEP_TYPE(type) IN5((type), HOURLY, DAILY, MONTHLY,YEARLY)

typedef double Bounds[ 2 ][ 2 ]; /* [ LONGITUDE LATITUDE ][MINIMUM MAXIMUM].*/

typedef struct { float r, g, b; } Color;

typedef Color (*DataColor)( double, double, double ); /* Color function. */
typedef Color (*TextColor)( const char* );

#ifdef RGB
#undef RGB
#endif

typedef struct { unsigned char r, g, b; const char* s; } RGB;

typedef char LongName[ 63 + 1 ];
typedef char Name[  31 + 1 ];
typedef char Units[ 15 + 1 ];

typedef void (*ForEachFileCallback)( const char* const fileName, void* data );

/*=============================== FUNCTIONS ================================*/

/* Numeric routines: */

extern int isNan( const double value );

extern int isNanFreeF( const float data[], const size_t count );

extern int isNanFree( const double data[], const size_t count );


/* Geographic geometry routines: */

extern int isValidLongitudeLatitude( const double longitude,
                                     const double latitude );

extern int isValidBounds( const Bounds bounds );

extern int boundsOverlap( const Bounds a, const Bounds b );

extern double degreesPerPixel( const Bounds bounds, int width, int height );

extern double widthInMeters( const Bounds bounds );

extern void computeArrowVectorCoordinates( const double longitude,
                                           const double latitude,
                                           const double x,
                                           const double y,
                                           const double degreesPerPixel,
                                           const double pixelsPerUnitLength,
                                           double* const point0X,
                                           double* const point0Y,
                                           double* const point1X,
                                           double* const point1Y,
                                           double* const point2X,
                                           double* const point2Y,
                                           double* const point3X,
                                           double* const point3Y );

extern void computeArrowVectorCoordinates2( const int headLengthPixels,
                                            const double longitude,
                                            const double latitude,
                                            const double x,
                                            const double y,
                                            const double degreesPerPixel,
                                            const double pixelsPerUnitLength,
                                            double* const point0X,
                                            double* const point0Y,
                                            double* const point1X,
                                            double* const point1Y,
                                            double* const point2X,
                                            double* const point2Y,
                                            double* const point3X,
                                            double* const point3Y );

/* Color routines: */

extern int isValidColor( Color color );

extern RGB makeRGB( unsigned char r, unsigned char g, unsigned char b,
                    const char* s );

extern Color dataColor( double value, double minimum, double maximum );
extern Color lexicographicTextColor( const char* text );


/* String routines: */

extern int wordCount( const char* string );
extern void lowercase( char string[] );
extern int matchesWord( const char* word, const char* const words );

extern int indexOfString( const char* string, const char* const strings[],
                          int count );

extern void copyPaddedString( char* output, const char* input,
                              const int length, const int uppercase );

extern size_t parseValues( const char* valuesString,
                           const size_t size,
                           double values[] );

/* File routines: */

extern int fileExists( const char* fileName );
extern size_t fileSize( const char* fileName );
extern char* readFile( const char* name, size_t* length, size_t* lines );
extern size_t controlMToSpace( char* string );
extern int copyStream( FILE* input, FILE* output );
extern int copyFile( const char* inputFileName, const char* outputFileName );

extern int copyFileBytes( FILE* inputFile, const char* outputFileName,
                          size_t bytes );

extern int streamBytesToFile( FILE* stream, const char* outputFileName );

extern int streamFile( const char* fileName );

extern int directoryExists( const char* name );

extern void removeAllFiles( const char* name );

extern void removeMatchedFiles( const char* const directoryName,
                                const char* const startsWith,
                                const char* const endsWith );

extern void copyFiles( const char* const fromDirectory,
                       const char* const toDirectory,
                       const char* const startsWith,
                       const char* const endsWith );

extern void forEachFile( const char* const directory,
                         const char* const startsWith,
                         const char* const endsWith,
                         ForEachFileCallback callback,
                         void* callbackData );

extern void directoryListing( const char* directory, const char* extensions,
                              int size, char buffer[] );

extern const char* homeDirectory( void );

#ifdef __cplusplus
}
#endif

#endif /* UTILITIES_H */

