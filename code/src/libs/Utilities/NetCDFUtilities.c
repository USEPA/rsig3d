
/******************************************************************************
PURPOSE: NetCDFUtilities.c - Define convenience routines for NetCDF files.

NOTES:   For a description of NetCDF COARDS Conventions see:
         http://ferret.wrc.noaa.gov/noaa_coop/coop_cdf_profile.html

HISTORY: 2007-12-15 plessel.todd@epa.gov
******************************************************************************/

/*================================ INCLUDES =================================*/

#include <stdio.h>  /* For stderr, fprintf(). */
#include <string.h> /* For memset().  */
#include <ctype.h>  /* For toupper(). */

#include <netcdf.h> /* For nc_*. */

#include <Assertions.h>      /* For PRE*(). */
#include <DateTime.h>        /* isUTCTimestamp(). */
#include <Utilities.h>       /* For NEW(), FREE(), isNan(),copyPaddedString()*/
#include <NetCDFUtilities.h> /* For public interface. */

/*================================== TYPES ==================================*/

enum { NAMLEN3 = 16, MXDLEN3 = 80 }; /* From IOAPI Specification. */
typedef char Line[ MXDLEN3 + 1 ];
typedef char Name3[ NAMLEN3 + 1 ];

/*============================ PUBLIC FUNCTIONS =============================*/



/******************************************************************************
PURPOSE: openReadableNetCDFFile - Open a NetCDFFile for reading.
INPUTS:  const char* fileName       Name of file to open.
RETURNS: int NetCDF file ID if successful, else 0 if failed and
         a failure message is printed to stderr.
******************************************************************************/

int openReadableNetCDFFile( const char* fileName ) {
  PRE02( fileName, *fileName );

  int result = 0;
  int ncid = 0;
  const int status = nc_open( fileName, NC_NOWRITE, &ncid );

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't create file '%s' because %s.\n",
             fileName, message );
  } else if ( ncid <= 0 ) {
    nc_close( ncid );
    fprintf( stderr, "\a\nInvalid id for file '%s'.\n", fileName );
  } else {
    result = ncid;
  }

  POST0( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: createNetCDFFile - Create a NetCDFFile for writing.
INPUTS:  const char* fileName       Name of file to create.
         const int create64BitFile  Create 64-bit NetCDF file?
RETURNS: int NetCDF file ID if successful, else 0 if failed and
         a failure message is printed to stderr.
******************************************************************************/

int createNetCDFFile( const char* fileName, const int create64BitFile ) {

  PRE03( fileName, *fileName, IS_BOOL( create64BitFile ) );

  int result = 0;
  const int mode = create64BitFile ? NC_CLOBBER | NC_64BIT_OFFSET : NC_CLOBBER;
  int ncid = 0;
  const int status = nc_create( fileName, mode, &ncid );

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't create file '%s' because %s.\n",
             fileName, message );
  } else if ( ncid <= 0 ) {
    nc_close( ncid );
    fprintf( stderr, "\a\nInvalid id for file '%s'.\n", fileName );
  } else {
    result = ncid;
  }

  POST0( result >= 0 );
  return result;
}



/******************************************************************************
PURPOSE: closeNetCDFFile - Close a NetCDFFile.
INPUTS:  const int file  ID of file to close.
******************************************************************************/

void closeNetCDFFile( const int file ) {
  PRE0( file >= 0 );
  nc_close( file );
  POST0( 1 );
}



/******************************************************************************
PURPOSE: endNetCDFHeaderDefinition - Close a NetCDFFile.
INPUTS:  const int file  ID of file to close.
******************************************************************************/

int endNetCDFHeaderDefinition( const int file ) {
  PRE0( file >= 0 );
  const int status = nc_enddef( file );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't create file definition because %s.\n",
             message );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: readNetCDFDimension - Read the named dimension of a NetCDF file.
INPUTS:  const int file    NetCDF file ID.
         const char* name  Name of dimension.
OUTPUTS: int* value        Value of dimension or 0 if unsuccessful.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int readNetCDFDimension( const int file, const char* name, int* value ) {

  PRE04( file >= 0, name, *name, value );

  int result = 0;
  int id = -1;
  int status = nc_inq_dimid( file, name, &id );
  *value = 0;

  if ( status == NC_NOERR ) {
    size_t length = 0;
    status = nc_inq_dimlen( file, id, &length );

    if ( AND2( status == NC_NOERR, length ) ) {
      *value = length;
      result = 1;
    }
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't read dimension %s because %s.\n",
                     name, message );
  }

  if ( ! result ) {
    *value = 0;
  }

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( result, *value >= 0, *value == 0 ) );

  return result;
}



/******************************************************************************
PURPOSE: createNetCDFDimensions - Create the named dimensions of a NetCDF file.
INPUTS:  const int file             NetCDF file ID.
         const int count            Number of dimensions.
         const char* const names[]  Name of each dimension.
         const size_t sizes[]       Size of each dimension.
OUTPUTS: int ids[]                  Ids of each dimension or -1 if unsuccessful.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int createNetCDFDimensions( const int file,
                            const int count,
                            const char* const names[],
                            const size_t sizes[],
                            int ids[] ) {

  PRE09( file >= 0, count > 0, names, *names, names[ count - 1 ],
         sizes, *sizes >= 1, sizes[ count - 1 ] >= 1, ids );

  int result = 0;
  int index = 0;

  do {
    int id = -1;
    const int status =
      nc_def_dim( file, names[ index ], sizes[ index ], &id );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't create dimension %s because %s.\n",
                      names[ index ], message );
      index = count;
    } else {
      ids[ index ] = id;
    }

    ++index;
  } while ( index < count );

  result = index == count;

  if ( ! result ) {
    memset( ids, -1, count * sizeof *ids );
  }

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( result,
                        AND2( ids[ 0 ] >=  0, ids[ count - 1 ] >=  0 ),
                        AND2( ids[ 0 ] == -1, ids[ count - 1 ] == -1 ) ) );

  return result;
}



/******************************************************************************
PURPOSE: createNetCDFIOAPIVariables - Create variables TFLAG, LONGITUDE,
         LATITUDE and optionally ELEVATION and then other named data variables.
INPUTS:  const int file               NetCDF file to write to.
         const int dimensionIds[ 6 ]  NetCDF ids of dimensions.
                                      TSTEP, DATE-TIME, LAY, VAR, ROW, COL.
         const int andElevation       Create elevation variable too?
         const int forceUppercase     Ensure variableNames are all capitals?
         const int variables          Number of data variables.
         const char* variableNames[ variables ]  Names of data variables.
         const char* variableUnits[ variables ]  Units of data variables.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int createNetCDFIOAPIVariables( const int file,
                                const int dimensionIds[ 6 ],
                                const int andElevation,
                                const int forceUppercase,
                                const int variables,
                                const char* variableNames[],
                                const char* variableUnits[] ) {

  PRE021( file != -1,
          dimensionIds,
          dimensionIds[ 0 ] > -1,
          dimensionIds[ 1 ] > -1,
          dimensionIds[ 2 ] > -1,
          dimensionIds[ 3 ] > -1,
          dimensionIds[ 4 ] > -1,
          dimensionIds[ 5 ] > -1,
          IS_BOOL( andElevation ),
          IS_BOOL( forceUppercase ),
          variables > 0,
          variableNames,
          variableNames[ 0 ],
          variableNames[ 0 ][ 0 ],
          variableNames[ variables - 1 ],
          variableNames[ variables - 1 ][ 0 ],
          variableUnits,
          variableUnits[ 0 ],
          variableUnits[ 0 ][ 0 ],
          variableUnits[ variables - 1 ],
          variableUnits[ variables - 1 ][ 0 ] );

  int result = createNetCDFIOAPITFLAGVariable( file, dimensionIds ) > -1;

  if ( result ) {
    result = createNetCDFIOAPIVariable( file, dimensionIds, 0,
                                        "LONGITUDE", "degrees" ) > -1;

    if ( result ) {
      result = createNetCDFIOAPIVariable( file, dimensionIds, 0,
                                          "LATITUDE", "degrees" ) > -1;

      if ( result ) {
        result =
          IMPLIES( andElevation,
                   createNetCDFIOAPIVariable( file, dimensionIds, 0,
                                              "ELEVATION", "m" ) > -1 );

        if ( result ) {
          int variable = 0;

          for ( variable = 0;
                AND2( variable < variables, result );
                ++variable ) {
            result =
              createNetCDFIOAPIVariable( file, dimensionIds, forceUppercase,
                                         variableNames[ variable ],
                                         variableUnits[ variable ] ) > -1;
          }
        }
      }
    }
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: createNetCDFIOAPITFLAGVariable - Create IOAPI variable TFLAG.
INPUTS:  const int file            NetCDF file to write to.
         const int dimensionIds[]  NetCDF ids of dimensions.
                                   TSTEP, DATE-TIME, LAY, VAR, ROW, COL.
RETURNS: int id if successful,
         else -1 and a failure message is printed to stderr.
******************************************************************************/

int createNetCDFIOAPITFLAGVariable( const int file,
                                    const int dimensionIds[ 6 ] ) {

  PRE08( file != -1,
         dimensionIds,
         dimensionIds[ 0 ] > -1,
         dimensionIds[ 1 ] > -1,
         dimensionIds[ 2 ] > -1,
         dimensionIds[ 3 ] > -1,
         dimensionIds[ 4 ] > -1,
         dimensionIds[ 5 ] > -1 );

  int result = -1;
  int ok = 0;
  int variableDimensionIds[ 3 ] = { -1, -1, -1 };

  /*
   * Create TFLAG:
   * int TFLAG(TSTEP, VAR, DATE-TIME) ;
   *     TFLAG:units = "<YYYYDDD,HHMMSS>" ;
   *     TFLAG:long_name = "TFLAG           " ;
   *     TFLAG:var_desc = "Timestep-valid flags:  (1) YYYYDDD or (2) HHMMSS" ;
   */

  variableDimensionIds[ 0 ] = dimensionIds[ 0 ]; /* TSTEP. */
  variableDimensionIds[ 1 ] = dimensionIds[ 3 ]; /* VAR. */
  variableDimensionIds[ 2 ] = dimensionIds[ 1 ]; /* DATE-TIME. */

  result = createNetCDFVariable( file, "TFLAG", "<YYYYDDD,HHMMSS>",
                                 NC_INT, 0, 3, variableDimensionIds );

  if ( result > -1 ) {

    if ( writeNetCDFTextAttribute( file, result,
                                   "long_name", "TFLAG           " ) ) {
      Line line = "";
      copyPaddedString( line,
                        "Timestep-valid flags:  (1) YYYYDDD or (2) HHMMSS",
                        MXDLEN3, 0 );
      ok = writeNetCDFTextAttribute( file, result, "var_desc", line );
    }
  }

  if ( ! ok ) {
    result = -1;
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: makeNetCDFIOAPIVariableName - Create IOAPI variable name from a
         given possibly longer lowercase variable name.
INPUTS:  const char* inputName  Name of original variable.
OUTPUTS: char outputName[ 17 ]  Name of IOAPI-conforming variable.
******************************************************************************/

void makeNetCDFIOAPIVariableName( const char* const inputName,
                                  char outputName[ 17 ] ) {

  PRE03( inputName, inputName[ 0 ], outputName );

  int index = 0;

  for ( index = 0; AND2( index < 16, inputName[ index ] ); ++index ) {
    outputName[ index ] = toupper( inputName[ index ] );
  }

  outputName[ index ] = '\0';

  /*
   * If outputName was truncated and inputName was a vector ending in _u, _v
   * then make outputName end with U or V to make it unique.
   */

  if ( index == 16 ) {
    const size_t length = strlen( inputName );

    if ( length > 16 ) {
      const char* ending = inputName + length - 2;

      if ( *ending == '_' ) {
        ++ending;

        if ( IN7( *ending, 'u', 'v', 'w', 'U', 'V', 'W' ) ) {
          outputName[ 16 - 1 ] = toupper( *ending );
        }
      }
    }
  }

  POST02( isupper( *outputName ), strlen( outputName ) <= NAMLEN3 );
}



/******************************************************************************
PURPOSE: createNetCDFIOAPIVariable - Create IOAPI variable.
INPUTS:  const int file               NetCDF file to write to.
         const int dimensionIds[ 6 ]  NetCDF ids of dimensions.
                                      TSTEP, DATE-TIME, LAY, VAR, ROW, COL.
         const int forceUppercase     Ensure variableNames are all capitals?
         const char* variableName     Name of variable.
         const char* variableUnits    Units of variable.
RETURNS: int id if successful,
         else -1 and a failure message is printed to stderr.
******************************************************************************/

int createNetCDFIOAPIVariable( const int file,
                               const int dimensionIds[ 6 ],
                               const int forceUppercase,
                               const char* variableName,
                               const char* variableUnits ) {

  PRE013( file != -1,
          IS_BOOL( forceUppercase ),
          dimensionIds,
          dimensionIds[ 0 ] > -1,
          dimensionIds[ 1 ] > -1,
          dimensionIds[ 2 ] > -1,
          dimensionIds[ 3 ] > -1,
          dimensionIds[ 4 ] > -1,
          dimensionIds[ 5 ] > -1,
          variableName,
          variableName[ 0 ],
          variableUnits,
          variableUnits[ 0 ] );

  int result = -1;
  int ok = 0;
  int variableDimensionIds[ 4 ] = { -1, -1, -1, -1 };
  Name3 name = "";
  Name3 unit = "";

  variableDimensionIds[ 0 ] = dimensionIds[ 0 ]; /* TSTEP. */
  variableDimensionIds[ 1 ] = dimensionIds[ 2 ]; /* LAY. */
  variableDimensionIds[ 2 ] = dimensionIds[ 4 ]; /* ROW. */
  variableDimensionIds[ 3 ] = dimensionIds[ 5 ]; /* COL. */

  makeNetCDFIOAPIVariableName( variableName, name );
  copyPaddedString( unit, variableUnits, NAMLEN3, 0 );

  result =
    createNetCDFVariable( file, name, unit,
                          NC_FLOAT, 0, 4, variableDimensionIds );

  if ( result > -1 ) {

    if ( writeNetCDFTextAttribute( file, result, "long_name", variableName )) {
      Line line = "";
      copyPaddedString( line, variableName, MXDLEN3, 0 );
      ok = writeNetCDFTextAttribute( file, result, "var_desc", line );
    }
  }

  if ( ! ok ) {
    result = -1;
  }

  POST0( result >= -1 );
  return result;
}



/******************************************************************************
PURPOSE: createNetCDFCOARDSCoordinates - Create variables longitude, latitude
         and optionally corner coordinates and elevation with COARDS
         attributes.
INPUTS:  const int file            NetCDF file to write to.
         const int withCorners     Create corner lon-lat variables too?
         const int withElevation   Create elevation variable too?
         const int dimensionality  Number of dimensions of each variable.
         const int dimensionIds[]  NetCDF ids of dimensions.
         const char* const dimensionNames[]  Names of dimensions.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int createNetCDFCOARDSCoordinates( const int file,
                                   const int withCorners,
                                   const int withElevation,
                                   const int coordinateRank,
                                   const int dimensionality,
                                   const int dimensionIds[],
                                   const char* const dimensionNames[] ) {

  PRE011( file != -1, IS_BOOL( withCorners ), IS_BOOL( withElevation ),
          IN3( coordinateRank, 1, 2 ),
          dimensionality > 0, dimensionIds,
          dimensionIds[ 0 ] > -1, dimensionIds[ dimensionality - 1 ] > -1,
          dimensionNames, dimensionNames[ 0 ],
          dimensionNames[ dimensionality - 1 ] );

  int result = 1;
  int crsId = createNetCDFCRSVariable( file );

  if ( crsId >= 0 ) {
    const int coordinateCount = 1 + 4 * withCorners;
    const char* const longitudes[] = {
      "longitude", "longitudeSW", "longitudeSE", "longitudeNW", "longitudeNE"
    };
    const char* const latitudes[] = {
      "latitude", "latitudeSW", "latitudeSE", "latitudeNW", "latitudeNE"
    };
    const char* const elevations[] = {
      "elevation", "elevationSW", "elevationSE", "elevationNW", "elevationNE"
    };

    int coordinate = 0;
    const int* longitudeDimensionIds = dimensionIds;
    const int* latitudeDimensionIds  = dimensionIds;

    if ( dimensionality > 1 ) {

      if ( ! strcmp( dimensionNames[ 0 ], "elevation" ) ) {
        longitudeDimensionIds += 1; /* Skip elevation. */
        latitudeDimensionIds  += 1; /* Skip elevation. */

        if ( AND2( coordinateRank == 1,
                   ! strcmp( dimensionNames[ 1 ], "latitude" ) ) ) {
          longitudeDimensionIds += 1; /* Skip latitude. */
        }
      } else if ( ! strcmp( dimensionNames[ 0 ], "latitude" ) ) {

        if ( coordinateRank == 1 ) {
          longitudeDimensionIds += 1; /* Skip latitude. */
        }
      }
    }

    /* Create longitude(s): */

    for ( coordinate = 0; AND2( result, coordinate < coordinateCount );
          ++coordinate ) {
      const int id =
        createNetCDFVariable( file, longitudes[ coordinate ], "degrees_east",
                              NC_DOUBLE, 0, coordinateRank,
                              longitudeDimensionIds );
      result = id != -1;

      if ( result ) {
        result = writeNetCDFTextAttribute( file, id, "range", "[-180, 180]" );

        if ( result ) {
          result = writeNetCDFTextAttribute( file, id, "grid_mapping", "crs" );
        }
      }
    }

    /* Create latitude(s): */

    for ( coordinate = 0; AND2( result, coordinate < coordinateCount );
          ++coordinate ) {
      const int id =
        createNetCDFVariable( file, latitudes[ coordinate ], "degrees_north",
                              NC_DOUBLE, 0, coordinateRank,
                              latitudeDimensionIds );
      result = id != -1;

      if ( result ) {
        result = writeNetCDFTextAttribute( file, id, "range", "[-90, 90]" );

        if ( result ) {
          result = writeNetCDFTextAttribute( file, id, "grid_mapping", "crs" );
        }
      }
    }

    if ( withElevation ) { /* Create elevation(s): */

      for ( coordinate = 0; AND2( result, coordinate < coordinateCount );
          ++coordinate ) {
        const int id =
          createNetCDFVariable( file, elevations[ coordinate ], "meters",
                                NC_DOUBLE, 0, dimensionality, dimensionIds );
        result = id != -1;

        if ( result ) {
          result = writeNetCDFTextAttribute( file, id, "positive", "up" );

          if ( result ) {
            result = writeNetCDFTextAttribute(file, id, "grid_mapping", "crs");
          }
        }
      }
    }
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFCOARDSAttributes - Write COARDS header attributes.
INPUTS:  const int file             NetCDF file to write to.
         const double domain[2][2]  domain[LONGITUDE LATITUDE][MIMIMUM MAXIMUM]
         const char* history        Source/history of data (e.g., URL).
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFCOARDSAttributes( const int file,
                                 const double domain[2][2],
                                 const char* history ) {

  PRE08( file != -1,
         domain,
         isValidLongitudeLatitude( domain[ LONGITUDE ][ MINIMUM ],
                                   domain[ LATITUDE  ][ MINIMUM ] ),
         isValidLongitudeLatitude( domain[ LONGITUDE ][ MAXIMUM ],
                                   domain[ LATITUDE  ][ MAXIMUM ] ),
         domain[ LONGITUDE ][ MINIMUM ] <= domain[ LONGITUDE ][ MAXIMUM ],
         domain[ LATITUDE  ][ MINIMUM ] <= domain[ LATITUDE  ][ MAXIMUM ],
         history,
         *history );

  int result = 0;

  if ( writeNetCDFTextAttribute( file, NC_GLOBAL, "Conventions", "COARDS" ) ) {

    if ( writeNetCDFTextAttribute( file, NC_GLOBAL, "history", history ) ) {

      if ( writeNetCDFFloatAttribute( file, NC_GLOBAL, "west_bound",
                                      domain[ LONGITUDE ][ MINIMUM ] ) ) {

        if ( writeNetCDFFloatAttribute( file, NC_GLOBAL, "east_bound",
                                        domain[ LONGITUDE ][ MAXIMUM ] ) ) {

          if ( writeNetCDFFloatAttribute( file, NC_GLOBAL, "south_bound",
                                          domain[ LATITUDE ][ MINIMUM ] ) ) {
            if ( writeNetCDFFloatAttribute( file, NC_GLOBAL, "north_bound",
                                           domain[ LATITUDE ][ MAXIMUM ] ) ) {
              result =
                writeNetCDFTextAttribute( file, NC_GLOBAL,
                                          "crs", "latitude_longitude" );
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
PURPOSE: writeNetCDFCOARDSContents - Write COARDS variables and attributes.
INPUTS:  const int file                NetCDF file to write to.
         const double domain[2][2]     Lon-lat bounds of data.
         const char* history           Source/history of data (e.g., URL).
         const char timestamp[ 25 ]    YYYY-MM-DDTHH:MM:SS-0000.
         const int dimensions          Number of dimensions for time variable.
         const int dimensionIds[dimensions]  Dimensions Ids for time variable.
         const size_t dimensionSizes[dimensions] Dimensions sizes of time var.
         const float hours[ dimensionSizes[ 0 ] ]  Hour offsets to use
                                                   or 0 if sequential
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
NOTES:   endNetCDFHeaderDefinition() is called so only data may be written to
         file now.
******************************************************************************/

int writeNetCDFCOARDSContents( const int file,
                               const double domain[2][2],
                               const char* history,
                               const char timestamp[ 25 ],
                               const int dimensions,
                               const int dimensionIds[],
                               const size_t dimensionSizes[],
                               const float hours[] ) {

  PRE017( file != -1,
          domain,
          isValidLongitudeLatitude( domain[ LONGITUDE ][ MINIMUM ],
                                    domain[ LATITUDE  ][ MINIMUM ] ),
          isValidLongitudeLatitude( domain[ LONGITUDE ][ MAXIMUM ],
                                    domain[ LATITUDE  ][ MAXIMUM ] ),
          domain[ LONGITUDE ][ MINIMUM ] <= domain[ LONGITUDE ][ MAXIMUM ],
          domain[ LATITUDE  ][ MINIMUM ] <= domain[ LATITUDE  ][ MAXIMUM ],
          history,
          *history,
          strlen( history ) < 128,
          isUTCTimestamp( timestamp ),
          IN_RANGE( dimensions, 1, 4 ),
          dimensionIds,
          dimensionIds[ 0 ] > -1,
          dimensionIds[ dimensions - 1 ] > -1,
          dimensionSizes,
          dimensionSizes[ 0 ] >= 1,
          dimensionSizes[ dimensions - 1 ] >= 1 );

  int result = 0;
  int time = 0;
  char timeUnits[ 50 ] = "hours since ";
  CHECK( strlen( timeUnits ) + strlen( timestamp ) < 50 );
  strcat( timeUnits, timestamp );
  timeUnits[ 22 ] = ' ';
  strcpy( timeUnits + 31, ".0 -00:00" );
  CHECK( strlen( timeUnits ) < sizeof timeUnits / sizeof *timeUnits );
  time = createNetCDFVariable( file, "time", timeUnits, NC_FLOAT, 0,
                               1, dimensionIds );

  if ( time != -1 ) {

    if ( writeNetCDFCOARDSAttributes( file, domain, history ) ) {

      if ( endNetCDFHeaderDefinition( file ) ) {
        int crsId = -1;
        int status = nc_inq_varid( file, "crs", &crsId );

        if ( status != NC_NOERR ) {
          const char* const message = nc_strerror( status );
          fprintf( stderr, "\a\nCan't get int crs variable because %s.\n",
                    message );
        } else {
          const int crsValue = -9999;
          status = nc_put_var_int( file, crsId, &crsValue );

          if ( status != NC_NOERR ) {
            const char* const message = nc_strerror( status );
            fprintf( stderr, "\a\nCan't write int crs variable because %s.\n",
                      message );
          } else {
            const size_t zero = 0;
            const size_t timesteps = dimensionSizes[ 0 ];

            if ( hours ) { /* Write given hour offsets: */
              status = nc_put_vara_float( file, time, &zero, &timesteps, hours);
              result = status == NC_NOERR;
            } else { /* Create and write sequential hour offsets: */
              const int hours = (int) dimensionSizes[ 0 ];
              float* hourOffsets = NEW( float, hours );

              if ( hourOffsets ) {
                int hour = 0;

                for ( hour = 0; hour < hours; ++hour ) {
                  hourOffsets[ hour ] = hour;
                }

                status =
                  nc_put_vara_float(file, time, &zero, &timesteps, hourOffsets);
                result = status == NC_NOERR;
                FREE( hourOffsets );
              }
            }

            if ( ! result ) {
              const char* const message = nc_strerror( status );
              fprintf( stderr, "\a\nCan't write time variable because %s.\n",
                       message );
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
PURPOSE: createNetCDFCRSVariable - Create the crs variable of a NetCDF file
         which is needed so the resulting file can be displayed in
         visualization programs such as VERDI, QGIS, etc.
INPUTS:  const int file              NetCDF file ID.
RETURNS: int variable id > -1 if successful,
         else -1 and a failure message is printed to stderr.
NOTES:  The created variable will look like the following:
 int crs ;
		crs:spatial_ref = "GEOGCS[\"GCS_WGS_1984\",DATUM[\"WGS_1984\",\
                      SPHEROID[\"WGS_84\",6378137.0,298.257223563]],\
                      PRIMEM[\"Greenwich\",0.0],UNIT[\"Degree\",\
                      0.017453292519943295]]" ;
		crs:grid_mapping_name = "latitude_longitude" ;
		crs:longitude_of_prime_meridian = 0. ;
		crs:semi_major_axis = 6378137. ;
		crs:inverse_flattening = 298.257223563 ;
See: https://gis.stackexchange.com/questions/218441/\
displaying-netcdf-data-with-correct-crs
******************************************************************************/

int createNetCDFCRSVariable( const int file ) {

  PRE0( file >= 0 );

  int result = -1;
  int id = -1;
  int status = 0;

  DEBUG( fprintf( stderr, "createNetCDFCRSVariable( %d )\n", file ); )

  status = nc_def_var( file, "crs", NC_INT, 0, 0, &id );

  if ( status == NC_NOERR ) {
    const char* const spatial_ref =
      "GEOGCS[\"GCS_WGS_1984\",DATUM[\"WGS_1984\",SPHEROID[\"WGS_84\","
      "6378137.0,298.257223563]],PRIMEM[\"Greenwich\",0.0],UNIT[\"Degree\","
      "0.017453292519943295]]";

    if ( writeNetCDFTextAttribute( file, id, "spatial_ref", spatial_ref ) ) {

      if ( writeNetCDFTextAttribute( file, id, "grid_mapping_name",
                                     "latitude_longitude" ) ) {


        if ( writeNetCDFFloatAttribute( file, id,
                                        "longitude_of_prime_meridian", 0.0)) {

          if ( writeNetCDFFloatAttribute( file, id,
                                          "semi_major_axis", 6378137.0 ) ) {

            if ( writeNetCDFFloatAttribute( file, id,
                                            "inverse_flattening",
                                            298.257223563 ) ) {
              result = id;
            }
          }
        }
      }
    }
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf(stderr, "\a\nCan't create int variable crs because %s.\n", message);
  }

  POST0( result >= -1 );
  return result;
}



/******************************************************************************
PURPOSE: createNetCDFVariable - Create the named variable of a NetCDF file.
INPUTS:  const int file              NetCDF file ID.
         const char* name            Name of variable.
         const char* units           Units of variable.
         const int type              Type of variable.
         const int hasMissingValues  Can there be missing data values?
         const int dimensionality    Number of dimensions.
         const int* dimensionIds     Ids of dimensions.
RETURNS: int variable id > -1 if successful,
         else -1 and a failure message is printed to stderr.
******************************************************************************/

int createNetCDFVariable( const int file,
                          const char* name,
                          const char* units,
                          const int type,
                          const int hasMissingValues,
                          const int dimensionality,
                          const int* dimensionIds ) {

  PRE010( file >= 0, name, *name, units, *units,
          IN5( type, NC_INT, NC_FLOAT, NC_DOUBLE, NC_CHAR ),
          IS_BOOL( hasMissingValues ),
          dimensionality >= 1, dimensionIds,
          OR2( dimensionIds[ 0 ] == NC_UNLIMITED, dimensionIds[ 0 ] >= 0 ) );

  int result = -1;
  int id = -1;
  int ids[ NC_MAX_DIMS ];
  int status = 0;
  int dimension = 0;

  DEBUG( fprintf( stderr, "createVariable( %d, '%s', '%s', "
                  "%d, %d, %d, %d )\n",
                  file, name, units, type, hasMissingValues, dimensionality,
                  dimensionIds[ 0 ] ) );

  do {
    ids[ dimension ] = dimensionIds[ dimension ];
    ++dimension;
  } while ( dimension < dimensionality );

  status = nc_def_var( file, name, (nc_type) type, dimensionality, ids, &id );

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't create variable %s because %s.\n",
             name, message );
  } else {
    const char* const renamedUnits = strcmp( units, "-" ) ? units : "none";

    if ( writeNetCDFTextAttribute( file, id, "units", renamedUnits ) ) {

      if ( IMPLIES( hasMissingValues,
                    writeNetCDFFloatAttribute( file, id,
                                               "missing_value",
                                               -9.999e36 ) ) ) {
        result = id;
      }
    }
  }

  POST0( result >= -1 );
  return result;
}



/******************************************************************************
PURPOSE: readNetCDFIntAttribute - Read the value of a global integer
         attribute to a NetCDF file.
INPUTS:  const int file    NetCDF file ID.
         const char* name  Name of attribute.
OUTPUTS: int* value        Value of attribute or 0 if failed.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int readNetCDFIntAttribute( const int file, const char* name, int* value ) {

  PRE04( file >= 0, name, *name, value );

  const int status = nc_get_att_int( file, NC_GLOBAL, name, value );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't read value of attribute %s because %s.\n",
             name, message );
    *value = 0;
  }

  POST02( IS_BOOL( result ), IMPLIES( result == 0, *value == 0 ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFIntAttribute - Write the value of a global integer
         attribute to a NetCDF file.
INPUTS:  const int file    NetCDF file ID.
         const char* name  Name of attribute.
         const int value   Value for attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFIntAttribute( const int file,
                             const char* name,
                             const int value ) {

  PRE03( file >= 0, name, *name );

  const int status = nc_put_att_int( file, NC_GLOBAL, name, NC_INT, 1, &value );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't write value of attribute %s because %s.\n",
             name, message );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: readNetCDFFloatAttribute - Read the value of a global float
         attribute to a NetCDF file.
INPUTS:  const int file    NetCDF file ID.
         const char* name  Name of attribute.
OUTPUTS: float* value      Value of attribute or 0.0 if failed.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int readNetCDFFloatAttribute( const int file, const char* name, float* value ) {

  PRE04( file >= 0, name, *name, value );

  const int status = nc_get_att_float( file, NC_GLOBAL, name, value );
  const int result = AND2( status == NC_NOERR, ! isNan( *value ) );

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't read value of attribute %s because %s.\n",
             name, message );
  }

  if ( ! result ) {
    *value = 0.0;
  }

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( result, ! isNan( *value ), *value == 0.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFFloatAttribute - Write the value of a float attribute to
         a NetCDF file.
INPUTS:  const int file      NetCDF file ID.
         const int id        NetCDF variable ID or NC_GLOBAL.
         const char* name    Name of attribute.
         const float value  Value for attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFFloatAttribute( const int file, const int id,
                               const char* name, const float value ) {

  PRE05( file >= 0, OR2( id == NC_GLOBAL, id >= 0 ),
         name, *name, ! isNan( value ) );

  const int status = nc_put_att_float( file, id, name, NC_FLOAT, 1, &value );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't write value of attribute %s because %s.\n",
             name, message );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: readNetCDFDoubleAttribute - Read the value of a global double
         attribute to a NetCDF file.
INPUTS:  const int file    NetCDF file ID.
         const char* name  Name of attribute.
OUTPUTS: double* value     Value of attribute or 0.0 if failed.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int readNetCDFDoubleAttribute(const int file, const char* name, double* value) {

  PRE04( file >= 0, name, *name, value );

  const int status = nc_get_att_double( file, NC_GLOBAL, name, value );
  const int result = AND2( status == NC_NOERR, ! isNan( *value ) );

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't read value of attribute %s because %s.\n",
             name, message );
  }

  if ( ! result ) {
    *value = 0.0;
  }

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( result, ! isNan( *value ), *value == 0.0 ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFDoubleAttribute - Write the value of a double attribute to
         a NetCDF file.
INPUTS:  const int file      NetCDF file ID.
         const int id        NetCDF variable ID or NC_GLOBAL.
         const char* name    Name of attribute.
         const double value  Value for attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFDoubleAttribute( const int file, const int id,
                                const char* name, const double value ) {

  PRE05( file >= 0, OR2( id == NC_GLOBAL, id >= 0 ),
         name, *name, ! isNan( value ) );

  const int status = nc_put_att_double( file, id, name, NC_DOUBLE, 1, &value );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't write value of attribute %s because %s.\n",
             name, message );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFTextAttribute - Write the value of a text attribute.
INPUTS:  const int file    NetCDF file ID.
         const int id      NetCDF variable ID or NC_GLOBAL.
         const char* name  Name of attribute.
         const char* value Value of named attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFTextAttribute( const int file, const int id, const char* name,
                              const char* value ) {

  PRE06( file >= 0, OR2( id == NC_GLOBAL, id >= 0 ),
         name, *name, value, *value );

  const int status = nc_put_att_text( file, id, name, strlen( value ), value );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't write text attribute %s because %s.\n",
             name, message );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: readNetCDFFloatArrayAttribute - Read a float array attribute.
INPUTS:  const int file         NetCDF file ID to write to.
         const char* name       Name of attribute.
         const int count        Number of elements in values array.
OUTPUTS: float values[ count ]  Value of attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int readNetCDFFloatArrayAttribute( const int file, const char* name,
                                   const int count, float values[] ) {

  PRE05( file >= 0, name, *name, values, count > 0 );

  int result = 0;
  size_t length = 0;
  int status = nc_inq_attlen( file, NC_GLOBAL, name, &length );

  if ( AND2( status == NC_NOERR, (int) length == count ) ) {
    status =
      nc_get_att_float( file, NC_GLOBAL, name, values );
    result = AND2( status == NC_NOERR, isNanFreeF( values, count ) );
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't write value of attribute %s because %s.\n",
             name, message );
  }

  if ( ! result ) {
    memset( values, 0, count * sizeof *values );
  }

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( result,
                        isNanFreeF( values, count ),
                        IS_ZERO2( values[ 0 ], values[ count - 1 ] ) ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFFloatArrayAttribute - Write a float array attribute.
INPUTS:  const int file              NetCDF file ID to write to.
         const char* name            Name of attribute.
         const int count             Number of elements in values array.
         const float values[ count ] Value of attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFFloatArrayAttribute( const int file, const char* name,
                                    const float values[], const int count ) {

  PRE06( file >= 0, name, *name, values, count > 0, isNanFreeF(values, count));

  const int status =
    nc_put_att_float( file, NC_GLOBAL, name, NC_FLOAT, count, values );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't write value of attribute %s because %s.\n",
             name, message );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: readNetCDFDoubleArrayAttribute - Read a double array attribute.
INPUTS:  const int file         NetCDF file ID to write to.
         const char* name       Name of attribute.
         const int count        Number of elements in values array.
OUTPUTS: double values[ count ]  Value of attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int readNetCDFDoubleArrayAttribute( const int file, const char* name,
                                    const int count, double values[] ) {

  PRE05( file >= 0, name, *name, values, count > 0 );

  int result = 0;
  size_t length = 0;
  int status = nc_inq_attlen( file, NC_GLOBAL, name, &length );

  if ( AND2( status == NC_NOERR, (int) length == count ) ) {
    status =
      nc_get_att_double( file, NC_GLOBAL, name, values );
    result = AND2( status == NC_NOERR, isNanFree( values, count ) );
  }

  if ( ! result ) {
    memset( values, 0, count * sizeof *values );
  }

  POST02( IS_BOOL( result ),
          IMPLIES_ELSE( result,
                        isNanFree( values, count ),
                        IS_ZERO2( values[ 0 ], values[ count - 1 ] ) ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFDoubleArrayAttribute - Write a double array attribute.
INPUTS:  const int file               NetCDF file ID to write to.
         const char* name             Name of attribute.
         const int count              Number of elements in values array.
         const double values[ count ] Value of attribute.
RETURNS: int 1 if successful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFDoubleArrayAttribute( const int file, const char* name,
                                     const double values[], const int count ) {

  PRE06( file >= 0, name, *name, values, count > 0, isNanFree(values, count));

  const int status =
    nc_put_att_double( file, NC_GLOBAL, name, NC_DOUBLE, count, values );
  const int result = status == NC_NOERR;

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't write value of attribute %s because %s.\n",
             name, message );
  }

  POST0( IS_BOOL( result ) );
  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFAllDoubleData - Write all double data to the NetCDF file.
INPUTS:  int file                   NetCDF file handle.
         const char* variableName   Name of variable to write.
         const size_t dimensions[]  Size of data dimensions.
         const double data[]        Data to write.
RETURNS: int 1 if unsuccessful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFAllDoubleData( const int file, const char* variableName,
                              const size_t dimensions[],
                              const double data[] ) {

  PRE05( file > -1, variableName, *variableName, dimensions[ 0 ] >= 1, data );

  int result = 0;
  int id = -1;
  int status = nc_inq_varid( file, variableName, &id );

  if ( status != NC_NOERR ) { /* Try IOAPI truncated UPPER_CASE name: */
    Name3 name = "";
    makeNetCDFIOAPIVariableName( variableName, name );
    status = nc_inq_varid( file, name, &id );
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't determine id of variable '%s' because %s.",
             variableName, message );
  } else {
    const size_t starts[ 4 ] = { 0, 0, 0, 0 };
    status = nc_put_vara_double( file, id, starts, dimensions, data );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't write subset of variable '%s' because %s.\n",
               variableName, message );
    } else {
      result = 1;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFAllFloatData - Write all float data to the NetCDF file.
INPUTS:  int file                   NetCDF file handle.
         const char* variableName   Name of variable to write.
         const size_t dimensions[]  Size of data dimensions.
         const float data[]         Data to write.
RETURNS: int 1 if unsuccessful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFAllFloatData( const int file, const char* variableName,
                             const size_t dimensions[],
                             const float data[] ) {

  PRE05( file > -1, variableName, *variableName, dimensions[ 0 ] >= 1, data );

  int result = 0;
  int id = -1;
  int status = nc_inq_varid( file, variableName, &id );

  if ( status != NC_NOERR ) { /* Try IOAPI truncated UPPER_CASE name: */
    Name3 name = "";
    makeNetCDFIOAPIVariableName( variableName, name );
    status = nc_inq_varid( file, name, &id );
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't determine id of variable '%s' because %s.",
             variableName, message );
  } else {
    const size_t starts[ 4 ] = { 0, 0, 0, 0 };
    status = nc_put_vara_float( file, id, starts, dimensions, data );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't write subset of variable '%s' because %s.\n",
               variableName, message );
    } else {
      result = 1;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFAllIntData - Write all data to the NetCDF file.
INPUTS:  const int file             NetCDF file handle.
         const char* variableName   Name of variable to write.
         const size_t dimensions[]  Size of data dimensions.
         const int data[]           Data to write.
RETURNS: int 1 if unsuccessful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFAllIntData( const int file, const char* variableName,
                           const size_t dimensions[],
                           const int data[] ) {

  PRE05( file > -1, variableName, *variableName, dimensions[ 0 ] >= 1, data );

  int result = 0;
  int id = -1;
  int status = nc_inq_varid( file, variableName, &id );

  if ( status != NC_NOERR ) { /* Try IOAPI truncated UPPER_CASE name: */
    Name3 name = "";
    makeNetCDFIOAPIVariableName( variableName, name );
    status = nc_inq_varid( file, name, &id );
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't determine id of variable '%s' because %s.",
                     variableName, message );
  } else {
    const size_t starts[ 4 ] = { 0, 0, 0, 0 };
    status = nc_put_vara_int( file, id, starts, dimensions, data );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't write subset of variable '%s' because %s.\n",
               variableName, message );
    } else {
      result = 1;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFCharData - Write character data to the NetCDF file.
INPUTS:  const int file            NetCDF file handle.
         const char* variableName  Name of variable to write.
         const int start           Starting index of first dimension.
         const int length          Length of each item in data[].
         const char data[ count ]  Data to write: data[ count * length ].
RETURNS: int 1 if unsuccessful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFCharData( const int file, const char* variableName,
                         const int start, const int length,
                         const char data[] ) {

  PRE07( file > -1, variableName, *variableName,
         start >= 0, length > 0, length < 1000, data );

  int result = 0;
  int id = -1;
  int status = nc_inq_varid( file, variableName, &id );

  if ( status != NC_NOERR ) { /* Try IOAPI truncated UPPER_CASE name: */
    Name3 name = "";
    makeNetCDFIOAPIVariableName( variableName, name );
    status = nc_inq_varid( file, name, &id );
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't determine id of variable '%s' because %s.",
                     variableName, message );
  } else {
    size_t starts[ 2 ] = { 0, 0 };
    size_t counts[ 2 ] = { 0, 0 };
    starts[ 0 ] = start;
    counts[ 0 ] = 1;
    counts[ 1 ] = length;
    status = nc_put_vara_text( file, id, starts, counts, data );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't write subset of variable '%s' because %s.",
                       variableName, message );
    } else {
      result = 1;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFSomeDoubleData - Write some double data to the NetCDF file.
INPUTS:  const int file            NetCDF file handle.
         const char* variableName  Name of variable to write.
         const int timestep        0-based timestep index to write.
         const size_t dimensions[]  Size of data dimensions.
         const double data[]        Data to write.
RETURNS: int 1 if unsuccessful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFSomeDoubleData( const int file, const char* variableName,
                               const int timestep,
                               const size_t dimensions[],
                               const double data[] ) {

  PRE06( file > -1, variableName, *variableName, timestep >= 0,
          dimensions[ 0 ] >= 1, data );

  int result = 0;
  int id = -1;
  int status = nc_inq_varid( file, variableName, &id );

  DEBUG( fprintf( stderr, "writeSomeData( %d, '%s', "
                  "timestep = %d, dimensions[%d, ...], data[%lf ...] )\n",
                  file, variableName, timestep,
                  dimensions[ 0 ], data[ 0 ] ) );

  if ( status != NC_NOERR ) { /* Try IOAPI truncated UPPER_CASE name: */
    Name3 name = "";
    makeNetCDFIOAPIVariableName( variableName, name );
    status = nc_inq_varid( file, name, &id );
  }

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't determine id of variable '%s' because %s.",
                     variableName, message );
  } else {
    size_t starts[ NC_MAX_DIMS ];
    memset( starts, 0, sizeof starts );
    starts[ 0 ] = timestep;
    status = nc_put_vara_double( file, id, starts, dimensions, data );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't write subset of variable '%s' because %s.\n",
                       variableName, message );
    } else {
      result = 1;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFSomeFloatData - Write some float data to the NetCDF file.
INPUTS:  const int file            NetCDF file handle.
         const char* variableName  Name of variable to write.
         const int timestep        0-based timestep index to write.
         const size_t dimensions[]  Size of data dimensions.
         const float data[]        Data to write.
RETURNS: int 1 if unsuccessful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFSomeFloatData( const int file, const char* variableName,
                              const int timestep,
                              const size_t dimensions[],
                              const float data[] ) {

  PRE06( file > -1, variableName, *variableName, timestep >= 0,
          dimensions[ 0 ] >= 1, data );

  int result = 0;
  int id = -1;
  int status = nc_inq_varid( file, variableName, &id );

  if ( status != NC_NOERR ) { /* Try IOAPI truncated UPPER_CASE name: */
    Name3 name = "";
    makeNetCDFIOAPIVariableName( variableName, name );
    status = nc_inq_varid( file, name, &id );
  }

  DEBUG( fprintf( stderr, "writeSomeFloatData( %d, '%s', "
                  "timestep = %d, dimensions[%d, ...], data[%lf ...] )\n",
                  file, variableName, timestep,
                  dimensions[ 0 ], data[ 0 ] ) );

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't determine id of variable '%s' because %s.",
                     variableName, message );
  } else {
    size_t starts[ NC_MAX_DIMS ];
    memset( starts, 0, sizeof starts );
    starts[ 0 ] = timestep;
    status = nc_put_vara_float( file, id, starts, dimensions, data );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't write subset of variable '%s' because %s.\n",
                       variableName, message );
    } else {
      result = 1;
    }
  }

  return result;
}



/******************************************************************************
PURPOSE: writeNetCDFSomeIntData - Write some int data to the NetCDF file.
INPUTS:  const int file            NetCDF file handle.
         const char* variableName  Name of variable to write.
         const int timestep        0-based timestep index to write.
         const size_t dimensions[]  Size of data dimensions.
         const int data[]          Data to write.
RETURNS: int 1 if unsuccessful,
         else 0 and a failure message is printed to stderr.
******************************************************************************/

int writeNetCDFSomeIntData( const int file, const char* variableName,
                            const int timestep,
                            const size_t dimensions[],
                            const int data[] ) {

  PRE06( file > -1, variableName, *variableName, timestep >= 0,
          dimensions[ 0 ] >= 1, data );

  int result = 0;
  int id = -1;
  int status = nc_inq_varid( file, variableName, &id );

  if ( status != NC_NOERR ) { /* Try IOAPI truncated UPPER_CASE name: */
    Name3 name = "";
    makeNetCDFIOAPIVariableName( variableName, name );
    status = nc_inq_varid( file, name, &id );
  }

  DEBUG( fprintf( stderr, "writeSomeIntData( %d, '%s', "
                  "timestep = %d, dimensions[%d, ...], data[%lf ...] )\n",
                  file, variableName, timestep,
                  dimensions[ 0 ], data[ 0 ] ) );

  if ( status != NC_NOERR ) {
    const char* const message = nc_strerror( status );
    fprintf( stderr, "\a\nCan't determine id of variable '%s' because %s.\n",
                     variableName, message );
  } else {
    size_t starts[ NC_MAX_DIMS ];
    memset( starts, 0, sizeof starts );
    starts[ 0 ] = timestep;
    status = nc_put_vara_int( file, id, starts, dimensions, data );

    if ( status != NC_NOERR ) {
      const char* const message = nc_strerror( status );
      fprintf( stderr, "\a\nCan't write subset of variable '%s' because %s.\n",
                       variableName, message );
    } else {
      result = 1;
    }
  }

  return result;
}



