project(QMapControl)
cmake_minimum_required(VERSION 2.6.2)


ADD_DEFINITIONS(${QT_DEFINITIONS})
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    /usr/X11/include/
    ${CMAKE_SOURCE_DIR}/include
)

SET(SOURCES 
circlepoint.cpp			layermanager.cpp
curve.cpp			linestring.cpp
emptymapadapter.cpp		mapadapter.cpp
fixedimageoverlay.cpp		mapcontrol.cpp
geometry.cpp			maplayer.cpp
geometrylayer.cpp		mapnetwork.cpp
googlemapadapter.cpp		openaerialmapadapter.cpp
googlesatmapadapter.cpp		osmmapadapter.cpp
gps_position.cpp		point.cpp
imagemanager.cpp		tilemapadapter.cpp
imagepoint.cpp			wmsmapadapter.cpp
layer.cpp			yahoomapadapter.cpp
)

SET(HEADERS 
circlepoint.h		gps_position.h		maplayer.h
curve.h			imagemanager.h		mapnetwork.h
emptymapadapter.h	imagepoint.h		openaerialmapadapter.h
fixedimageoverlay.h	layer.h			osmmapadapter.h
geometry.h		layermanager.h		point.h
geometrylayer.h		linestring.h		tilemapadapter.h
googlemapadapter.h	mapadapter.h		wmsmapadapter.h
googlesatmapadapter.h	mapcontrol.h		yahoomapadapter.h
)

SET(HEADERS_QT 
    ${HEADERS}
)

SET(UIS
)

QT4_WRAP_CPP(HEADERS_MOC ${HEADERS_QT})
QT4_WRAP_UI(UIS_MOC ${UIS})

ADD_LIBRARY(${PROJECT_NAME} 
    STATIC
    ${SOURCES} 
    ${HEADERS} 
    ${HEADERS_MOC} 
    ${UIS_MOC}
)

#ADD_DEPENDENCIES(${PROJECT_NAME})

ADD_DEFINITIONS(
    -DPROJ_PATH="${CMAKE_SOURCE_DIR}"
    -DUSE_QT_LIB
    -DGLEW_STATIC
)

LINK_DIRECTORIES( 
    ${LINK_DIRECTORIES} 
    ${GLEW_LIBRARY_DIRS}
    ${QT_LIBRARY_DIR}
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
   )

target_link_libraries(${PROJECT_NAME}
    ${GLEW}
    ${GLEW_LIBRARIES}
    ${QT_LIBRARIES} 
    ${OPENGL_LIBRARIES}
)
