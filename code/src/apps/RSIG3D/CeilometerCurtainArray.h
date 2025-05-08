#ifndef CEILOMETERCURTAINARRAY_H
#define CEILOMETERCURTAINARRAY_H

/******************************************************************************
PURPOSE: CeilometerCurtainArray.h - Declare constants, global variables and
         routines to map ceilometer profile data into arrays used for rendering
         2d 'curtains'.
NOTES:   Used by PointRenderer.cpp.
HISTORY: 2016-09-23 freeman.matt@epa.gov
         2022-03-21 plessel.todd@epa.gov
STATUS: inchoate
******************************************************************************/

//================================ INCLUDES ===================================

#include <string> // For class std::string.
#include <vector> // For class std::vector.

#include <Bounds.h>    // For class emvl::Bounds.
#include <Timestamp.h> // For class emvl::Timestamp.
#include <Dataset.h>   // For class emvl::Dataset.

namespace cca {

// Constants:
enum {
  HOURS_PER_DAY = 24,
  MINUTES_PER_HOUR = 60,
  SECONDS_PER_MINUTE = 60,
  SECONDS_PER_HOUR = MINUTES_PER_HOUR * SECONDS_PER_MINUTE,
  SECONDS_PER_DAY = HOURS_PER_DAY * SECONDS_PER_HOUR
};
enum { MAXIMUM_ELEVATION = 15000 }; // Meters above mean sea level.
enum {
  SIZE_X = 240, // Evenly divisible into SECONDS_PER_DAY.
  SIZE_Y = 200  // Evenly divisible into MAXIMUM_ELEVATION.
};

// Global variables:
extern float Xticks[ SIZE_X ]; // Seconds along x axis where ticks appear.
extern float Yticks[ SIZE_Y ]; // Meters  along y axis where ticks appear.
extern double CCA[ SIZE_X ][ SIZE_Y ]; // Ceilometer Curtain Array

// Mixing Layer Array elevations per data point of a named ceilometer:
extern std::vector<double> MLA_elevations; // MLA_elevations[ points[ profile]]

// Mixing Layer Array seconds per data point of a named ceilometer:
extern std::vector<long long> MLA_times; // MLA_times[ points[ profile ] ].


// Functions:

// Does DatasetManager have any non-MixingLayerHeight ceilometer datasets?
extern bool haveCeilometerNonMixingLayerHeightDataset( size_t& index );

// Does DatasetManager have ceilometer mixing layer height?
extern bool haveCeilometerMixingLayerHeightDataset( size_t& index );

  
// Have DatasetManager retrieve ceilometer mixing layer height.
// If successful, use index to access dataset owned by DatasetManager.
extern bool
loadCeilometerMixingLayerHeightDataset( const emvl::Timestamp& startTime,
                                        const emvl::Timestamp& endTime,
                                        const emvl::Bounds& bounds,
                                        size_t& index );

// Get info an all loaded ceilometer profile datasets in DatasetManager:
extern void getCeilometerInfo( std::vector<size_t>& indices,
                               std::vector<std::string>& variables,
                               std::vector<std::string>& units,
                               std::vector<long long>& ids,
                               std::vector<double>& longitudes,
                               std::vector<double>& latitudes,
                               std::vector<std::string>& names );

// Find subset index of named ceilometer within a dataset:
extern bool findProfileIndex( const emvl::Dataset& dataset,
                              const std::string& name,
                              size_t& index );

// Reinitialize CCA, Xticks, Yticks from a given dataset's ceilometer profile:
extern bool createCCA( const emvl::Timestamp& startTime,
                       const emvl::Timestamp& endTime,
                       const emvl::Dataset& dataset,
                       const std::string& ceilometerName );

// Reinitialize MLA_elevations/times from a given dataset's ceilometer profile:
extern bool createMLA( const emvl::Timestamp& startTime,
                       const emvl::Timestamp& endTime,
                       const emvl::Dataset& dataset,
                       const std::string& ceilometerName );

} // end namespace

#endif /* CEILOMETERCURTAINARRAY_H */
