project(pythonmarble)


# compile SIP
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  ${SIP_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH}
    # Qt
    ${QT_INCLUDE_DIR} ${QT_QT_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTNETWORK_INCLUDE_DIR} ${QT_QTSQL_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR} ${QT_QTSVG_INCLUDE_DIR} ${QT_QTUITOOLS_INCLUDE_DIR} ${QT_QTWEBKIT_INCLUDE_DIR}
    # Marble included
    ../../../lib
)

set(SIP_INCLUDES ${PYQT4_SIP_DIR} sip)
set(SIP_CONCAT_PARTS 8)
set(SIP_TAGS ALL WS_X11 ${PYQT4_VERSION_TAG} )
set(SIP_EXTRA_OPTIONS -g)
FILE(GLOB extra_files_sip sip/*.sip)
set(SIP_EXTRA_FILES_DEPEND ${extra_files_sip})

add_sip_python_module(PyKDE4.marble sip/marblemod.sip
    marblewidget
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTUITOOLS_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
)
