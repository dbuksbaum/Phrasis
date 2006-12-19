PROJECT( phrasis )

SET( SOURCE_FILES
    ./src/dialogimpl.cpp
    ./src/main.cpp
    ./src/scrollbar.cpp
)

SET( MOC_HDRS_FILES
    ./src/dialogimpl.h
    ./src/scrollbar.h
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

# by default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
#SET( QT_USE_QT3SUPPORT TRUE )   
#SET( QT_USE_QTXML TRUE )

# this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )

# add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )

# this command will generate rules that will run rcc on all files from SAMPLE_RCS
# in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
QT4_ADD_RESOURCES( RC_SRCS ${RCS_FILES} )

# this will run uic on .ui files:
QT4_WRAP_UI( UI_HDRS ${UI_FILES} )

# and finally this will run moc:
QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS_FILES} )

# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )

ADD_EXECUTABLE( phrasis ${SOURCE_FILES} ${MOC_SRCS} ${RC_SRCS} ${UI_HDRS} )

TARGET_LINK_LIBRARIES( phrasis ${QT_LIBRARIES} )
