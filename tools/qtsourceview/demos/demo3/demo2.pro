QT =   gui \
  core \
  xml
CONFIG +=   qt \
  warn_on \
  release
TEMPLATE =   app
SOURCES =   main2.cpp \
  mainwindow2.cpp \
  ../../src/qsvcolordef.cpp \
  ../../src/qsvcolordeffactory.cpp \
  ../../src/qsvlangdef.cpp \
  ../../src/qsvlangdeffactory.cpp \
  ../../src/qsvsyntaxhighlighter.cpp
HEADERS =   mainwindow2.h \
  ../../src/debug_info.h \
  ../../src/qorderedmap.h \
  ../../src/qsvcolordef.h \
  ../../src/qsvcolordeffactory.h \
  ../../src/qsvlangdef.h \
  ../../src/qsvlangdeffactory.h \
  ../../src/qsvsyntaxhighlighter.h
INCLUDEPATH =   ../../src/ \
  .
RESOURCES =   demo2.qrc
