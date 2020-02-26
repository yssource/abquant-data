TARGET = abqhelper
TEMPLATE = lib
CONFIG += shared c++11 x86_64
QT += xml qml
QT -= gui
DEFINES += TF_DLL
DESTDIR = $$PWD/../../../lib

include(../../abqbase.pri)
LIBS +=
MOC_DIR = .obj/
OBJECTS_DIR = .obj/

HEADERS += applicationhelper.h
SOURCES += applicationhelper.cpp
