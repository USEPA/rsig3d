# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(PenProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Pen_SRCS pen.cpp)
set(Pen_HDRS pen.h)
QT4_WRAP_CPP(Pen_SRCS pen.h)

add_library(PenProperty STATIC ${Pen_SRCS} ${Pen_HDRS})
add_dependencies(PenProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(PenProperty 
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS PenProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
