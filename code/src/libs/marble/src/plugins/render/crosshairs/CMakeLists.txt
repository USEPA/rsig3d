PROJECT( CrosshairsPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/render/crosshairs
 ${CMAKE_BINARY_DIR}/src/plugins/render/crosshairs
 ${QT_INCLUDE_DIR}
)
INCLUDE(${QT_USE_FILE})

set( crosshairs_SRCS CrosshairsPlugin.cpp )
set( crosshairs_UI CrosshairsConfigWidget.ui )
qt4_wrap_ui( crosshairs_SRCS ${crosshairs_UI} )
qt4_add_resources( crosshairs_SRCS crosshairs.qrc )


marble_add_plugin( CrosshairsPlugin ${crosshairs_SRCS} )
