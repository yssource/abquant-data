TARGET = abqcontroller
TEMPLATE = lib
CONFIG += shared c++17 x86_64
QT += network sql xml qml
QT -= gui
DEFINES += TF_DLL
DESTDIR = $$PWD/../../../lib

include(../../abqbase.pri)
# INCLUDEPATH += ../helpers ../models
# DEPENDPATH  += ../helpers ../models
LIBS += -L$$PWD/../../../lib -labqhelper -labqmodel
MOC_DIR = .obj/
OBJECTS_DIR = .obj/

HEADERS += applicationcontroller.h
SOURCES += applicationcontroller.cpp
HEADERS += indexdaycontroller.h
SOURCES += indexdaycontroller.cpp
HEADERS += stockxdxrcontroller.h
SOURCES += stockxdxrcontroller.cpp
HEADERS += stockdaycontroller.h
SOURCES += stockdaycontroller.cpp
HEADERS += indexmincontroller.h
SOURCES += indexmincontroller.cpp
HEADERS += stockmincontroller.h
SOURCES += stockmincontroller.cpp
