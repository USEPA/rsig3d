# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

cmake_minimum_required(VERSION 2.6)
project(propertyEditor-package)

# please choose between GPL or LGPL license
set (PROPERTYEDITOR_LICENSE "LGPL")

message("*** propertyEditor use ${PROPERTYEDITOR_LICENSE} license ***")


set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_VERBOSE_MAKEFILE ON)
#comment or remove the above line before release

set (PROPERTYEDITOR_VERSION_MAJOR 2)
set (PROPERTYEDITOR_VERSION_MINOR 1)
set (PROPERTYEDITOR_VERSION_RELEASE 3)
set (PROPERTYEDITOR_VERSION "${PROPERTYEDITOR_VERSION_MAJOR}.${PROPERTYEDITOR_VERSION_MINOR}.${PROPERTYEDITOR_VERSION_RELEASE}" )

add_definitions (-DPROPERTYEDITOR_VERSION=\"${PROPERTYEDITOR_VERSION}\")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" )
include(SetPaths.cmake)

find_package(Qt4 REQUIRED)
set( QT_USE_QTGUI TRUE )
include( ${QT_USE_FILE} )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/types
    ${QT_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions (-DLIB_INSTALL_DIR=\"${LIB_INSTALL_DIR}\")

#add_subdirectory(plugins)
add_subdirectory(src)
add_subdirectory(examples)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#install(FILES cmake/modules/FindPropertyEditor.cmake DESTINATION ${CMAKE_ROOT}/Modules COMPONENT Devel)
