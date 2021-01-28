TARGET = abqhelper
TEMPLATE = lib
CONFIG += shared c++17 x86_64
# CONFIG += static c++17 x86_64
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
