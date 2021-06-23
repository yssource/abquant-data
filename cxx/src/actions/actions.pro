TARGET = abqaction
TEMPLATE = lib
CONFIG += shared console debug c++17
# CONFIG += static console release c++17
CONFIG -= app_bundle

include($$OUT_PWD/../../../conanbuildinfo.pri)
include(../../abqbase.pri)

QT += network sql
QT += concurrent
QT -= gui
DESTDIR = $$PWD/../../../lib

LIBS += -L$$PWD/../../../lib -labqmodel -labqhelper

HEADERS = emconcept.hpp
HEADERS += emconceptbase.hpp
HEADERS += emconcepthistory.hpp
HEADERS += security.hpp
HEADERS += indexday.hpp
HEADERS += indexmin.hpp
HEADERS += securitylist.hpp
HEADERS += stockday.hpp
HEADERS += stockday_p.hpp
HEADERS += stockmin.hpp
HEADERS += stockxdxr.hpp
HEADERS += xdxr.hpp
HEADERS += xdxr_p.hpp
HEADERS += indicator.hpp
HEADERS += utils.hpp

SOURCES = emconceptbase.cpp
SOURCES += emconcepthistory.cpp
SOURCES += indexday.cpp
SOURCES += indexmin.cpp
SOURCES += stockday.cpp
SOURCES += stockmin.cpp
SOURCES += stockxdxr.cpp
SOURCES += xdxr.cpp
SOURCES += indicator.cpp
