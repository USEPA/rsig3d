PROJECT( MaemoPositionProviderPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/positionprovider/maemo
 ${CMAKE_BINARY_DIR}/src/plugins/positionprovider/maemo
 ${QT_INCLUDE_DIR}
 ${liblocation_INCLUDE_DIRS}
)

include(${QT_USE_FILE})

set( maemo_SRCS
 MaemoPositionProviderPlugin.cpp
)

set( MaemoPositionProviderPlugin_LIBS ${liblocation_LIBRARIES} )

marble_add_plugin( MaemoPositionProviderPlugin ${maemo_SRCS} )
