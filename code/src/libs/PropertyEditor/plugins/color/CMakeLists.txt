# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(ColorProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

SET(Color_SRCS changecolor.cpp color.cpp)
SET(Color_HDRS changecolor.h color.h)
QT4_WRAP_CPP(Color_SRCS changecolor.h  color.h)

add_library(ColorProperty STATIC ${Color_SRCS} ${Color_HDRS})
add_dependencies(ColorProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(ColorProperty 
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS ColorProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
