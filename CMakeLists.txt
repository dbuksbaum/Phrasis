PROJECT( phrasis )

SET( SOURCE_FILES
    ./src/dialogimpl.cpp
    ./src/main.cpp
    ./src/scrollbar.cpp
    ./src/previewform.cpp
    ./src/grammarcheck.cpp
    ./src/linkgrammar.cpp
    ./src/fadebutton.cpp
    ./src/mouseawareframe.cpp
)

SET( MOC_HDRS_FILES
    ./src/dialogimpl.h
    ./src/scrollbar.h
    ./src/previewform.h
    ./src/fadebutton.h
    ./src/mouseawareframe.h
)

SET( UI_FILES
    ./ui/dialog.ui
)

# and finally an resource file
SET( RCS_FILES
    ./resources/resources.qrc
)

# enable warnings
ADD_DEFINITIONS( -Wall )

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules )

# I am tracking the Qt4 module from kde svn, it is more up
# to date than what usually is inclued with cmake
FIND_PACKAGE( Qt4-KDE REQUIRED )
FIND_PACKAGE( LinkGrammar REQUIRED )

INCLUDE( ${QT_USE_FILE} )

QT4_ADD_RESOURCES( RC_SRCS ${RCS_FILES} )
QT4_WRAP_UI( UI_HDRS ${UI_FILES} )
QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS_FILES} )

INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}
                     ${X11_INCLUDE_DIR}
                     ${LINK_GRAMMER_INCLUDE_DIR}
                     ${QT_QTSVG_INCLUDE_DIR}
                   )

ADD_EXECUTABLE( phrasis ${SOURCE_FILES} ${MOC_SRCS} ${RC_SRCS} ${UI_HDRS} )
TARGET_LINK_LIBRARIES( phrasis ${QT_LIBRARIES} ${QT_QTSVG_LIBRARY} ${X11_LIBRARIES} ${LINK_GRAMMER_LIBRARIES} )
