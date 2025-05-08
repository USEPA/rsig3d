# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(BoolProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Bool_SRCS bool.cpp)
set(Bool_HDRS bool.h)
QT4_WRAP_CPP(Bool_SRCS bool.h)
QT4_ADD_RESOURCES(Bool_SRCS resource.qrc)

add_library(BoolProperty STATIC ${Bool_SRCS} ${Bool_HDRS})
add_dependencies(BoolProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )

target_link_libraries(BoolProperty 
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS BoolProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
