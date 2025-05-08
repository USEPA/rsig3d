# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(NumbersProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Numbers_SRCS numbers.cpp)
set(Numbers_HDRS numbers.h)
QT4_WRAP_CPP(Numbers_SRCS numbers.h)

add_library(NumbersProperty STATIC ${Numbers_SRCS} ${Numbers_HDRS})
add_dependencies(NumbersProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(NumbersProperty 
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS NumbersProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
