TARGET = main
TEMPLATE = app
CONFIG += console release c++17 testcase
CONFIG -= app_bundle
# CONFIG += no_keywords # avoid clash with slots in Python.h

include(../../../abqbase.pri)

QT += network sql testlib
QT -= gui

LIBS += -L$$PWD/../../../../lib -labqaction
LIBS += -L$$PWD/../../../../lib -labqmodel

SOURCES = main.cpp

INSTALLS -= target
