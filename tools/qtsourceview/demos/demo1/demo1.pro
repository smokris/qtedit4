TARGET =   demo1
TEMPLATE =   app
QT =   xml \
  gui \
  core
INCLUDEPATH =   . \
  ../../src/
  
DESTDIR=../../
TMP_DIR =   ../../tmp
UI_DIR =   ../../tmp
MOC_DIR =   ../../tmp
OBJECTS_DIR =   ../../tmp
HEADERS =   mainwindow1.h
SOURCES =   main1.cpp \
  mainwindow1.cpp
unix:LIBS =   -L../../ \
  -lqtsourceview
CONFIG +=   debug \
  warn_on