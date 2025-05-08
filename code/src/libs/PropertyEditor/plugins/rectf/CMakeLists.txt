# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(RectfProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Rectf_SRCS rectf.cpp)
set(Rectf_HDRS rectf.h)
QT4_WRAP_CPP(Rectf_SRCS rectf.h)

add_library(RectfProperty STATIC ${Rectf_SRCS} ${Rectf_HDRS})
add_dependencies(RectfProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(RectfProperty 
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS RectfProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
