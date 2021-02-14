TARGET = main
TEMPLATE = app
CONFIG += console release c++17 testcase
CONFIG -= app_bundle
# CONFIG += no_keywords # avoid clash with slots in Python.h

include(../../../abqbase.pri)
include($$OUT_PWD/../../../../conanbuildinfo.pri)

QT += network sql testlib
QT -= gui
DEFINES += TF_DLL

LIBS += -L$$PWD/../../../../lib -labqaction
LIBS += -L$$PWD/../../../../lib -labqmodel
LIBS += $$CONAN_LIBDIRS_DATAFRAME -lDataFrame

SOURCES = main.cpp

INSTALLS -= target
