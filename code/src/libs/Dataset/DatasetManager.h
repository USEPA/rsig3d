#ifndef DATASETMANAGER_H
#define DATASETMANAGER_H

/******************************************************************************
PURPOSE: DatasetManager.h - Creates and owns a collection of Datasets read from
         files or from a webservice.
HISTORY: 2013-03-19 plessel.todd@epa.gov
STATUS:  unreviewed tested
NOTES:   Example usage:

g++ -Wall -g -o testDatasetManager testDatasetManager.C \
 -I/Users/plessel/projects/rsig/dev/RSIG3D/code/include \
 -L/Users/plessel/projects/rsig/dev/RSIG3D/code/lib/Darwin.x86_64 \
 -lUtilities.debug -lNetCDF -lShapefile -lGPC -lPNG -lZ -lcurl -lm

#include <string.h>
#include <iostream>
#include <DatasetManager.h>

int main( int argc, char* argv[] ) {

  // If curl program is in the same directory as this program, get its path:

  char path[ 256 ] = "";
  memset( path, 0, sizeof path );
  strncpy( path, argv[ 0 ], sizeof path / sizeof *path - 1 );
  char* const lastSlash = strrchr( path, '/' );

  if ( lastSlash ) {
    *lastSlash = '\0';
  } else {
    path[ 0 ] = '.';
    path[ 1 ] = '\0';
  }

  emvl::DatasetManager::checkAndSetServerHost( path );
  const int ok = emvl::DatasetManager::remoteVersion() > 0;

  // Retrieve a dataset from the webservice:

  if ( ok ) {
    emvl::DatasetManager datasetManager( true );
    datasetManager.appendRetrievedDataset( emvl::QueryParameters(), 300 ); //x
    std::cout << datasetManager[ 0 ].name() << std::endl; // Print dataset name
  }

  return ! ok;
}

******************************************************************************/

//================================ INCLUDES ===================================

#include <stdio.h> // For FILE.

#include <string> // For std::string.
#include <vector> // For std::vector.

#include <Bounds.h>          // For emvl::Bounds.
#include <Timestamp.h>       // For emvl::Timestamp.
#include <QueryParameters.h> // For QueryParameters.
#include <Dataset.h>         // For Dataset.

//================================== TYPES ====================================

namespace emvl {

class DatasetManager {

public:

  // Class methods for setting-up connection to the webservice:

  // Commands:

  /*
   * Call checkAndSetServerHost( path ) once before calling retrieve methods.
   * Where path is the directory path of the curl program used to access
   * the webservice.
   * If successful then remoteVersion() returns value > 20121001 else 0.
   */

  static void checkAndSetServerHost( const char* const path );
  static std::string getCurlPath();
  static std::string webservice();
  static std::string serverHost();
  static bool isValidTEMPOKey( const char* key );
  static bool isValidOAQPSKey( const char* key );
  static bool isValidPurpleAirKey( const char* key );
  static const char* DOSPath( const char* const path ); // Helper.

  // Class methods for using CMAQSubset on local or remote files:

  static void setLocalSubsetterDirectory( const std::string& directory );
  static void setRemoteSubsetterDirectory( const std::string& directory );

  static bool ensureDirectoryExists( const std::string& user,
                            const std::string& host,
                            const std::string& name );

  static bool installRemoteSubsetter( const std::string& user,
                                      const std::string& host);

  static std::string subsetCMAQFile( const std::string& user,
                            const std::string& host,
                            const std::string& pathedDataFileName,
                            const std::string& dataVariable,
                            const std::string& pathedHTFileName,
                            const std::string& pathedZFFileName,
                            const std::string& pathedWWINDFileName,
                            const std::string& outputDirectoryName,
                            const Bounds& bounds,
                            const Timestamp& firstTimestamp,
                            const Timestamp& lastTimestamp,
                            const int layer,
                            const int aggregate );
  // Queries:

  static int remoteVersion();

  static std::string getPlatform( const std::string& user,
                                  const std::string& host );

  static std::string getWorkingDirectory( const std::string& user,
                                          const std::string& host );

  static std::string getDirectoryListing( const std::string& user,
                                          const std::string& host,
                                          const std::string& directory );

  static std::string getVariableListing( const std::string& user,
                                         const std::string& host,
                                         const std::string& file );

private:

  // Attributes:

  bool printURL; // Print URL string to stdout during data retrievals?
  void (*urlCallback)( const char* const url ); // Optional retrieval callback.
  std::vector<Dataset*> datasets_;

  // Helper routines:

  DatasetManager( const DatasetManager& ); // Not allowed to copy.
  void operator=( const DatasetManager& ); // Not allowed to assign.

public:

  // Commands:

  ~DatasetManager();

  DatasetManager( const bool printURL = false,
                  void (*urlCallback)( const char* const url ) = 0 );

  Dataset* retrieveDataset( const QueryParameters& queryParameters,
                            const int timeoutSeconds = 3600 ) const; //x

  Dataset* readDataset( const std::string& fileName ) const; //x

  void retrieveFile( const std::string& remoteFileName,
                     const std::string& localFileName,
                     const int timeoutSeconds = 3600 ) const; //x

  void retrieveDatasetToFile( const QueryParameters& queryParameters,
                              const int timeoutSeconds = 3600 ) const; //x

  void appendRetrievedDataset( const QueryParameters& queryParameters,
                               const int timeoutSeconds = 3600 ); //x

  void appendDatasetFromFile( const std::string& fileName ); //x
  void append( Dataset* dataset ); //x
  void remove( const size_t index );
  void removeAll();
  void writeAll( const std::string& directory, const int format ) const; //x

  // Queries:

  bool invariant() const;
  size_t count() const;
  Dataset& operator[]( const size_t index ) const;
};

} // namespace emvl.

#endif /* DATASETMANAGER_H */



