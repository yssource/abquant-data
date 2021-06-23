TARGET = abqcontroller
TEMPLATE = lib
CONFIG += shared c++17 x86_64
# CONFIG += static c++17 x86_64
QT += network sql xml qml
QT -= gui
DESTDIR = $$PWD/../../../lib

include(../../abqbase.pri)

LIBS += -L$$PWD/../../../lib -labqhelper -labqmodel
MOC_DIR = .obj/
OBJECTS_DIR = .obj/

HEADERS += applicationcontroller.h
SOURCES += applicationcontroller.cpp
HEADERS += emconceptbasecontroller.h
SOURCES += emconceptbasecontroller.cpp
HEADERS += emconcepthistorycontroller.h
SOURCES += emconcepthistorycontroller.cpp
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
