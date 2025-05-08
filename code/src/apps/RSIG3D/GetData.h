
#ifndef GETDATA_H
#define GETDATA_H

/******************************************************************************
PURPOSE: GetData.h - call utility function from Todd. Modified from Example.C
HISTORY: 2013-04-23 zhang.nan@epa.gov
NOTES:
******************************************************************************/

#include <vector> // For std::vector.
#include <string> // For std::string.

#include <Utilities.h>            // For Bounds.
#include <CMAQGridParameters.h>   // For emvl::CMAQGridParameters.
#include <DataFilterParameters.h> // For emvl::DataFilterParameters.
#include <DatasetManager.h>       // For emvl::DatasetManager.

namespace emvl { class Dataset; };

#include <QRectF> // For QRectF.

// E.g., download/RSIG3D_Darwin.zip to /Users/plessel/RSIG3D.zip:

bool downloadFile( const std::string& remoteFileName,
                   const std::string& localFileName );

// OGC-WCS query to get XDR-format dataset streamed over webservice:

emvl::Dataset* downloadData( const bool getMetadata,
                             const bool compress,
                             const int format,
                             const char* const saveDirectory,
                             const char* dataName,
                             const char* startTime,
                             const char* endTime,
                             double west,
                             double east,
                             double south,
                             double north, 
                             int layer,
                             int aggregate,
                             int regridMethod,
                             int regridAggregation, 
                             emvl::CMAQGridParameters gridParameters,
                             emvl::DataFilterParameters filterParameters) ;

bool saveData( emvl::Dataset* datasets[], const int datasetCount,
               const int colormaps[], const Bounds bounds,
               const char* const outputDirectory, const int format );

std::vector<std::string> fileNamesOfLoadedDatasets();
std::vector<bool>   regridFlagsOfLoadedDatasets();

void reloadDatasets( QRectF& boundingBox,
                     const std::vector<std::string>& datasetNames,
                     const std::vector<std::string>& fileNames );

void checkAndLoadCeilometerMixingLayerHeightDataset();

void setRSIGServerHost(); // Try connecting to webservice.

emvl::DatasetManager* getDatasetManager();

const char* RSIGWebservice();
const char* RSIGServerHost();


#endif /* GETDATA_H */


