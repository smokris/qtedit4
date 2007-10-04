TEMPLATE = app
QT += gui core xml
CONFIG += qt warn_on console debug_and_release
DESTDIR += bin
OBJECTS_DIR += build
MOC_DIR += build
UI_DIR += build
FORMS += ui/mainwindow.ui ui/miniassistant.ui
HEADERS += src/mainwindowimpl.h \
 src/miniassistant.h \
 src/dcffile.h \
 src/dcfmodel.h \
 src/lru.h
SOURCES += src/mainwindowimpl.cpp \
 src/main.cpp \
 src/miniassistant.cpp \
 src/dcffile.cpp \
 src/dcfmodel.cpp \
 src/lru.cpp
RESOURCES += miniassistant.qrc
