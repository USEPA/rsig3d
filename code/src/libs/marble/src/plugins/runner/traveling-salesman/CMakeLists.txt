PROJECT( TravelingSalesmanPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/runner/traveling-salesman
 ${CMAKE_BINARY_DIR}/src/plugins/runner/traveling-salesman
 ${QT_INCLUDE_DIR}
)
INCLUDE(${QT_USE_FILE})

set( travelingsalesman_SRCS TravelingSalesmanRunner.cpp TravelingSalesmanPlugin.cpp )

marble_add_plugin( TravelingSalesmanPlugin ${travelingsalesman_SRCS} )
