PROJECT( ShpPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/runner/shp
 ${CMAKE_BINARY_DIR}/src/plugins/runner/shp
 ${QT_INCLUDE_DIR}
 ${LIBSHP_INCLUDE_DIR}
)

INCLUDE(${QT_USE_FILE})

set( shp_SRCS ShpPlugin.cpp ShpRunner.cpp )

set( ShpPlugin_LIBS ${LIBSHP_LIBRARIES} )

marble_add_plugin( ShpPlugin ${shp_SRCS} )
