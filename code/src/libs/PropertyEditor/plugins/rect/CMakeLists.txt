# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(RectProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Rect_SRCS rect.cpp)
set(Rect_HDRS rect.h)
QT4_WRAP_CPP(Rect_SRCS rect.h)

add_library(RectProperty STATIC ${Rect_SRCS} ${Rect_HDRS})
add_dependencies(RectProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(RectProperty 
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS RectProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
