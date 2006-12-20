TEMPLATE =   app
QT +=   gui \
  core
CONFIG +=   qt \
  warn_on \
  console \
  debug_and_release
DESTDIR +=   bin
OBJECTS_DIR +=   build
MOC_DIR +=   build
UI_DIR +=   build
RCC_DIR +=   build
FORMS +=   ui/dialog.ui
HEADERS +=   src/dialogimpl.h \
  src/scrollbar.h \
  src/previewform.h \
  src/grammarcheck.h \
  src/linkgrammar.h
SOURCES +=   src/dialogimpl.cpp \
  src/main.cpp \
  src/scrollbar.cpp \
  src/previewform.cpp \
  src/grammarcheck.cpp \
  src/linkgrammar.cpp
RESOURCES +=   resources/resources.qrc
LIBS +=   $$system(pkg-config \
  --libs \
  link-grammar)
