PROJECT( OpenDesktopPlugin )

INCLUDE_DIRECTORIES(
 ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/render/opendesktop
 ${CMAKE_BINARY_DIR}/src/plugins/render/opendesktop
 ${QT_INCLUDE_DIR}
)
INCLUDE(${QT_USE_FILE})

set( my_SRCS
  OpenDesktopPlugin.cpp
  OpenDesktopItem.cpp
  OpenDesktopModel.cpp
)

set( opendesktop_UI OpenDesktopConfigWidget.ui )
qt4_wrap_ui( my_SRCS ${opendesktop_UI} )
qt4_automoc( ${my_SRCS} )

marble_add_plugin( OpenDesktopPlugin ${my_SRCS} )
