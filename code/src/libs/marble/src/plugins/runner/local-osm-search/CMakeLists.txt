PROJECT( LocalOsmSearchPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/runner/local-osm-search
 ${CMAKE_BINARY_DIR}/src/plugins/runner/local-osm-search
 ${QT_INCLUDE_DIR}
)
INCLUDE(${QT_USE_FILE})

set( localOsmSearch_SRCS
LocalOsmSearchRunner.cpp
LocalOsmSearchPlugin.cpp
OsmPlacemark.cpp
OsmDatabase.cpp
DatabaseQuery.cpp
 )

marble_add_plugin( LocalOsmSearchPlugin ${localOsmSearch_SRCS} )
target_link_libraries( LocalOsmSearchPlugin ${QT_QTSQL_LIBRARY} )
