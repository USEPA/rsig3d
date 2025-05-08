
#ifndef CMAQFILEREADERMODEL_H
#define CMAQFILEREADERMODEL_H

/******************************************************************************
PURPOSE: CMAQFileReaderModel.h - Models CMAQFileReader state.

NOTES:   Uses Model-Controller Design Pattern.

HISTORY: 2022-01-10 plessel.todd@epa.gov
STATUS:  unreviewed tested
******************************************************************************/

//================================ INCLUDES ===================================

#include <string> // For std::string.
#include <vector> // For std::vector.

#include <Bounds.h>        // For class emvl::Bounds.
#include <DataUtilities.h> // For emvl::Bounds.
#include <Timestamp.h>     // For emvl::Timestamp.

//================================== TYPES ====================================


class CMAQFileReaderModel {
private:

  // Attributes:

  emvl::Bounds bounds;  // Longitude-latitude subset.
  emvl::Timestamp firstTimestamp; // First time subset.
  emvl::Timestamp lastTimestamp;  // Last  time subset.
  int layer; // -1 = integrate layers, 0 = all layers, > 0 = single layer.
  int aggregate; // 0 = No aggregation, 1 = aggregate every 24 hours.
  std::string outputDirectory; // Name of directory to write subset xdr file.
  std::string outputFile; // Name of last successful output xdr file or empty().

  int mode; // LOCAL or REMOTE.
  std::vector<std::string> installedHosts; // Remote hosts with installations.
  std::string user; // User login id on remote host.
  std::string host; // Name of remote host.
  std::string dataDirectory[ 2 ]; // Name of directory containing data file.
  std::string dataDirectoryListing[ 2 ]; // dataDirectory listing.
  std::string dataFile[ 2 ]; // Name of data file to read (per mode).
  std::string dataVariableListing[ 2 ]; // dataFile variable listing.
  std::string dataVariable[ 2 ]; // Name of variable in dataFile to read.
  std::string htDirectory[ 2 ]; // Name of directory containing htFile.
  std::string htDirectoryListing[ 2 ]; // htDirectory listing.
  std::string htFile[ 2 ]; // Name of file with variable HT to read.
  std::string zfDirectory[ 2 ]; // Name of directory containing zfFile.
  std::string zfDirectoryListing[ 2 ]; // zfDirectory listing.
  std::string zfFile[ 2 ]; // Name of file with variable ZH/ZF to read.
  std::string wwindDirectory[ 2 ]; // Name of directory containing wwindFile.
  std::string wwindDirectoryListing[ 2 ]; // wwindDirectory listing.
  std::string wwindFile[ 2 ]; // Name of file with variable WWIND or W_VEL to read.

  // Helpers:

  CMAQFileReaderModel( const CMAQFileReaderModel& ); // Not allowed.
  CMAQFileReaderModel& operator=( const CMAQFileReaderModel& ); // Not allowed.

public:

  enum { LOCAL, REMOTE }; // Mode.

  // Commands:

  CMAQFileReaderModel();
  ~CMAQFileReaderModel();

  void setOutputDirectory( const std::string& outputDirectory );

  void setSubset( const emvl::Bounds& bounds,
                  const emvl::Timestamp& firstTimestamp,
                  const emvl::Timestamp& lastTimestamp,
                  const int layer = 0,
                  const int aggregate = 0 );

  void setMode( const int mode );
  void setUser( const std::string& name );
  void setHost( const std::string& name );
  void setDataDirectory( const std::string& name );
  void setDataFile( const std::string& name );
  void setDataVariable( const std::string& name );
  void setHTDirectory( const std::string& name );
  void setHTFile( const std::string& name );
  void setZFDirectory( const std::string& name );
  void setZFFile( const std::string& name );
  void setWWINDDirectory( const std::string& name );
  void setWWINDFile( const std::string& name );

  void clearInputFiles();
  void clearOutputFile();

  void readCMAQFile(); // Run local or remote CMAQSubset.
                       // Then call getOutputFile() to get the resulting local
                       // xdr file name which is empty() if failed.

  // Queries:

  const std::string& getOutputFile() const; // Resulting file name or empty().
  const std::string& getOutputDirectory() const;

  // Get local or remote state:

  int getMode() const; // LOCAL or REMOTE.
  const std::string& getUser() const;
  const std::string& getHost() const;

  const std::string& getDataDirectory() const;
  const std::string& getDataDirectoryListing() const;
  const std::string& getDataFile() const;
  const std::string& getDataVariableListing() const;
  const std::string& getDataVariable() const;

  const std::string& getHTDirectory() const;
  const std::string& getHTDirectoryListing() const;
  const std::string& getHTFile() const;

  const std::string& getZFDirectory() const;
  const std::string& getZFDirectoryListing() const;
  const std::string& getZFFile() const;

  const std::string& getWWINDDirectory() const;
  const std::string& getWWINDDirectoryListing() const;
  const std::string& getWWINDFile() const;

  // Helpers to check selected NetCDF files for valid usage:

  bool isDataFile(  const std::string& name ) const;
  bool isHTFile(    const std::string& name ) const;
  bool isZFFile(    const std::string& name ) const;
  bool isWWINDFile( const std::string& name ) const;

  bool invariant() const;
};

#endif /* CMAQFILEREADERMODEL_H */



