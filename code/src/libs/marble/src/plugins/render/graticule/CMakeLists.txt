PROJECT(GraticulePlugin)

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/render/graticule
 ${CMAKE_BINARY_DIR}/src/plugins/render/graticule
 ${QT_INCLUDE_DIR}
)
INCLUDE(${QT_USE_FILE})

set( graticule_SRCS GraticulePlugin.cpp )
set( graticule_UI GraticuleConfigWidget.ui )
qt4_wrap_ui( graticule_SRCS ${graticule_UI} )


marble_add_plugin( GraticulePlugin ${graticule_SRCS} )
