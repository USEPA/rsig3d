project(PropertyEditor)

set(PE_TYPES_HEADERS 
    ./types/bool.h
    ./types/changecolor.h
    ./types/color.h
    ./types/date.h
    ./types/datetime.h
    ./types/enums.h
    ./types/flags.h
    ./types/flagslist.h		
    ./types/numbers.h
    ./types/strings.h
    ./types/stringlist.h
    ./types/stringlisteditor.h
    ./types/rect.h
    ./types/rectf.h
)

set(PE_TYPES_SRCS
    ./types/bool.cpp
    ./types/changecolor.cpp	
    ./types/color.cpp
    ./types/date.cpp
    ./types/datetime.cpp
    ./types/enums.cpp
    ./types/flags.cpp
    ./types/flagslist.cpp		
    ./types/numbers.cpp
    ./types/strings.cpp
    ./types/stringlist.cpp
    ./types/stringlisteditor.cpp
    ./types/rect.cpp	
    ./types/rectf.cpp	
) 

set(PE_TYPES_UIS
    ./types/stringlisteditor.ui	
    ./types/flagslist.ui
)

SET(PE_TYPES_RCCS 
    ./types/stringlist.qrc
)

##===================================================
set(PE_SRCS 
    propertydelegate.cpp 
    propertyeditor.cpp 
    propertyinterface.cpp 
    propertymodel.cpp 
    qtobject.cpp
    #qttrigger.cpp
)

set(PE_HEADERS 
    propertydelegate.h 
    propertyeditor.h 
    propertyinterface.h 
    propertymodel.h 
    qtobject.h 
    #qttrigger.h
)
##===================================================

QT4_WRAP_CPP(PE_SRCS ${PE_HEADERS} ${PE_TYPES_HEADERS})
QT4_WRAP_UI(UIS_MOC ${PE_TYPES_UIS})
QT4_ADD_RESOURCES(RCCS_MOC ${PE_TYPES_RCCS})

add_library(${PROJECT_NAME} STATIC 
    ${PE_SRCS} 
    ${PE_HEADERS}
    ${PE_TYPES_SRCS} 
    ${PE_TYPES_HEADERS}
    ${RCCS_MOC}
    ${UIS_MOC}
)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES	
    VERSION ${PROPERTYEDITOR_VERSION}
    SOVERSION ${PROPERTYEDITOR_VERSION}
)

include_directories(
    ${include_directories}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)
										
target_link_libraries(${PROJECT_NAME} 
    ${QT_LIBRARIES}
    #${QT_QTGUI_LIBRARY} 
    #${QT_QTCORE_LIBRARY} 
)

#install(TARGETS PropertyEditor ${INSTALL_TARGETS_DEFAULT_ARGS} )
#install(FILES propertyeditor.h propertyinterface.h propertyvalidator.h qtclass.h DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel)
