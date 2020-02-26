TARGET = abqaction
TEMPLATE = lib
CONFIG += shared console debug c++17
CONFIG -= app_bundle

include(../../abqbase.pri)
include($$OUT_PWD/../../../conanbuildinfo.pri)

QT += network sql
QT += concurrent
QT -= gui
DEFINES += TF_DLL
DESTDIR = $$PWD/../../../lib

LIBS += -L$$PWD/../../../lib -labqmodel

HEADERS = stock.hpp
HEADERS += stockday.hpp
HEADERS += stockmin.hpp
HEADERS += stockxdxr.hpp
HEADERS += xdxr.hpp
HEADERS += indicator.hpp
HEADERS += utils.hpp

SOURCES = stockday.cpp
SOURCES += stockmin.cpp
SOURCES += stockxdxr.cpp
