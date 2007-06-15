TEMPLATE =   app
SOURCES +=   src/debug_class.cpp \
  src/main.cpp
HEADERS +=   src/debug_class.h
CONFIG +=   console \
  warn_on \
  windows \
  release
QT +=   core
DEFINES +=   NO_DEBUG_CLASS_
