# shared temp dir for all projects
TMP_DIR		=	../../tmp/
UI_DIR		=	../../tmp/
MOC_DIR		=	../../tmp/
OBJECTS_DIR	=	../../tmp/
DESTDIR		=	../../

# set the Qate source dir, and include the pri file
# then add your sources and that's it
QATE_SRC_DIR=../../src/
include(../../src/qate.pri)
CONFIG		+=	qt warn_on silent
TARGET		=	demo6
SOURCES		+=	demo6.cpp

# TODO
#   in the future, the correct syntax should be
#   right now this will not work, because of include path not set correctly
#LIBS		=	-L../../ -lqate
#INCLUDEPATH	=	../../src 
#TARGET		=	../../demo6
#SOURCES	+=	demo6.cpp

