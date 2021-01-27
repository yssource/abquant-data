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

HEADERS = emconcept.hpp
HEADERS += emconceptbase.hpp
HEADERS += emconcepthistory.hpp
HEADERS += index.hpp
HEADERS += indexday.hpp
HEADERS += indexmin.hpp
HEADERS += stock.hpp
HEADERS += stockday.hpp
HEADERS += stockday_p.hpp
HEADERS += stockmin.hpp
HEADERS += stockxdxr.hpp
HEADERS += xdxr.hpp
HEADERS += indicator.hpp
HEADERS += utils.hpp

SOURCES = emconceptbase.cpp
SOURCES += emconcepthistory.cpp
SOURCES += indexday.cpp
SOURCES += indexmin.cpp
SOURCES += stockday.cpp
SOURCES += stockmin.cpp
SOURCES += stockxdxr.cpp
SOURCES += indicator.cpp
