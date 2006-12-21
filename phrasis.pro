TEMPLATE =   app
QT +=   gui \
  core \
  svg
CONFIG +=   qt \
  warn_on \
  console \
  link_pkgconfig \
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
  src/linkgrammar.h \
  src/fadebutton.h \
  src/mouseawareframe.h \
  src/spellcheck.h
SOURCES +=   src/dialogimpl.cpp \
  src/main.cpp \
  src/scrollbar.cpp \
  src/previewform.cpp \
  src/grammarcheck.cpp \
  src/linkgrammar.cpp \
  src/fadebutton.cpp \
  src/mouseawareframe.cpp \
  src/spellcheck.cpp
RESOURCES +=   resources/resources.qrc
PKGCONFIG += link-grammar enchant

