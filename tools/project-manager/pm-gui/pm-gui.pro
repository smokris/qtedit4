# -------------------------------------------------
# Project created by QtCreator 2008-12-13T14:29:10
# -------------------------------------------------
CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h
TARGET = pm-gui
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    projectmodel.cpp
HEADERS += mainwindow.h \
    projectmodel.h \
    stable.h
FORMS += mainwindow.ui

# global files
HEADERS += ../abstractproject.h \
    ../qmakeproject.h
SOURCES += ../abstractproject.cpp \
    ../qmakeproject.cpp
