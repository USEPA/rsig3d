#ifndef NETCDFUTILITIES_H
#define NETCDFUTILITIES_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
PURPOSE: NetCDFUtilities.h - Declare convenience routines for NetCDF files.
NOTES:
HISTORY: 2007-12-15 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

/*================================= INCLUDES ================================*/

#include <stdlib.h> /* For typedef size_t. */

/*================================== TYPES ==================================*/

/* Expose values used for 'type' parameters in below routines. */

#ifndef _NETCDF_
enum { NC_GLOBAL = -1, NC_CHAR = 2, NC_INT = 4, NC_FLOAT = 5, NC_DOUBLE = 6 };
#endif

/*================================ FUNCTIONS ================================*/


extern int openReadableNetCDFFile( const char* fileName );

extern int createNetCDFFile( const char* fileName,
                             const int create64BitFile );

extern void closeNetCDFFile( const int file );

extern int endNetCDFHeaderDefinition( const int file );

extern int readNetCDFDimension( const int file, const char* name, int* value );

extern int createNetCDFDimensions( const int file,
                                   const int count,
                                   const char* const names[],
                                   const size_t sizes[],
                                   int ids[] );

extern void makeNetCDFIOAPIVariableName( const char* const inputName,
                                         char outputName[ 17 ] );

extern int createNetCDFIOAPIVariables( const int file,
                                       const int dimensionIds[ 6 ],
                                       const int andElevation,
                                       const int forceUppercase,
                                       const int variables,
                                       const char* variableNames[],
                                       const char* variableUnits[] );

extern int createNetCDFIOAPITFLAGVariable( const int file,
                                           const int dimensionIds[ 6 ]  );

extern int createNetCDFIOAPIVariable( const int file,
                                      const int dimensionIds[ 6 ],
                                      const int forceUppercase,
                                      const char* variableName,
                                      const char* variableUnits );

extern int createNetCDFCOARDSCoordinates( const int file,
                                          const int withCorners,
                                          const int withElevation,
                                          const int coordinateRank,
                                          const int dimensionality,
                                          const int dimensions[],
                                          const char* const dimensionNames[] );

extern int writeNetCDFCOARDSAttributes( const int file,
                                        const double domain[2][2],
                                        const char* history );

extern int writeNetCDFCOARDSContents( const int file,
                                      const double domain[2][2],
                                      const char* history,
                                      const char timestamp[ 25 ],
                                      const int dimensions,
                                      const int dimensionIds[],
                                      const size_t dimensionSizes[],
                                      const float hours[] );

extern int createNetCDFCRSVariable( const int file );

extern int createNetCDFVariable( const int file,
                                 const char* name,
                                 const char* units,
                                 const int type,
                                 const int hasMissingValues,
                                 const int dimensionality,
                                 const int* dimensionIds );

extern int readNetCDFIntAttribute( const int file,
                                   const char* name,
                                   int* value );

extern int writeNetCDFIntAttribute( const int file,
                                    const char* name,
                                    const int value );

extern int readNetCDFFloatAttribute( const int file,
                                     const char* name,
                                     float* value );

extern int writeNetCDFFloatAttribute( const int file,
                                      const int id,
                                      const char* name,
                                      const float value );

extern int readNetCDFDoubleAttribute( const int file,
                                      const char* name,
                                      double* value );

extern int writeNetCDFDoubleAttribute( const int file,
                                       const int id,
                                       const char* name,
                                       const double value );

extern int writeNetCDFTextAttribute( const int file,
                                     const int id,
                                     const char* name,
                                     const char* value );

extern int readNetCDFFloatArrayAttribute( const int file,
                                          const char* name,
                                          const int count,
                                          float values[] );

extern int writeNetCDFFloatArrayAttribute( const int file,
                                           const char* name,
                                           const float values[],
                                           const int count );

extern int readNetCDFDoubleArrayAttribute( const int file,
                                           const char* name,
                                           const int count,
                                           double values[] );

extern int writeNetCDFDoubleArrayAttribute( const int file,
                                            const char* name,
                                            const double values[],
                                            const int count );

extern int writeNetCDFAllDoubleData( const int file,
                                     const char* variableName,
                                     const size_t dimensions[],
                                     const double data[] );

extern int writeNetCDFAllFloatData( const int file,
                                    const char* variableName,
                                    const size_t dimensions[],
                                    const float data[] );

extern int writeNetCDFAllIntData( const int file,
                                  const char* variableName,
                                  const size_t dimensions[],
                                  const int data[] );

extern int writeNetCDFCharData( const int file,
                                const char* variableName,
                                const int start,
                                const int length,
                                const char data[] );

extern int writeNetCDFSomeDoubleData( const int file,
                                      const char* variableName,
                                      const int timestep,
                                      const size_t dimensions[],
                                      const double data[] );

extern int writeNetCDFSomeFloatData( const int file,
                                     const char* variableName,
                                     const int timestep,
                                     const size_t dimensions[],
                                     const float data[] );

extern int writeNetCDFSomeIntData( const int file,
                                   const char* variableName,
                                   const int timestep,
                                   const size_t dimensions[],
                                   const int data[] );


#ifdef __cplusplus
}
#endif

#endif /* NETCDFUTILITIES_H */



