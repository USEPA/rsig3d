project(glew)
cmake_minimum_required(VERSION 2.6.2)

SET(SOURCES 
    src/glew.c
)

SET(HEADERS
    include/GL/glew.h
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_DEFINITIONS(
    -DGLEW_STATIC
)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
#ADD_EXECUTABLE(glewinfo src/glewinfo.c)
#target_link_libraries(glewinfo ${OPENGL_LIBRARIES} glew)
