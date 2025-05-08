# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(DateTimeProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(DateTime_SRCS datetime.cpp)
set(DateTime_HDRS datetime.h)
QT4_WRAP_CPP(DateTime_SRCS datetime.h)

add_library(DateTimeProperty STATIC ${DateTime_SRCS} ${DateTime_HDRS})
add_dependencies(DateTimeProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )

target_link_libraries(DateTimeProperty
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS DateTimeProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
