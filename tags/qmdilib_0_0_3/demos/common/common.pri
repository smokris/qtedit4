# $Id$

INCLUDEPATH	+= ../../src/ ../common/ .
MOC_DIR		+= ../../tmp/
RCC_DIR		+= ../../tmp/
UI_DIR		+= ../../tmp/
OBJECTS_DIR	+= ../../../tmp/

# Input
RESOURCES += ../common/common.qrc

# qmdilib
HEADERS += ../../src/actiongroup.h \
           ../../src/actiongrouplist.h \
           ../../src/qmdiclient.h \
           ../../src/qmdihost.h \
           ../../src/qmdiserver.h \
           ../../src/qmditabbar.h \
           ../../src/qmditabwidget.h \
           ../../src/qmdiworkspace.h \
           ../../src/qmdimainwindow.h

SOURCES += ../../src/actiongroup.cpp \
           ../../src/actiongrouplist.cpp \
           ../../src/qmdiclient.cpp \
           ../../src/qmdihost.cpp \
           ../../src/qmditabbar.cpp \
           ../../src/qmdiserver.cpp \
           ../../src/qmdiworkspace.cpp \
           ../../src/qmditabwidget.cpp
