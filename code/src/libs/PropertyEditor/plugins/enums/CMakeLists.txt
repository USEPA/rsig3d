# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

project(EnumsProperty)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/lib" )

set(Enums_SRCS enums.cpp)
set(Enums_HDRS enums.h)
QT4_WRAP_CPP(Enums_SRCS enums.h)

add_library(EnumsProperty STATIC ${Enums_SRCS} ${Enums_HDRS})
add_dependencies(EnumsProperty PropertyEditor)
link_directories( "${CMAKE_BINARY_DIR}/lib" )
target_link_libraries(EnumsProperty 
	PropertyEditor 
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY} 
)

install(TARGETS EnumsProperty ${INSTALL_PLUGINS_DEFAULT_ARGS})
