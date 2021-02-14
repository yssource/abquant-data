# QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror
# QMAKE_CXX = clang++
# QMAKE_LINK = clang++
# QMAKE_CC = clang
# QMAKE_CXXFLAGS += -std=c++1z -stdlib=libc++
# QMAKE_LFLAGS += -stdlib=libc++ -fuse-ld=lld

win32 {
  INCLUDEPATH += $$quote($$(TFDIR)\\include)
  LIBS += -L$$quote($$(TFDIR)\\bin)
  CONFIG(debug, debug|release) {
    LIBS += -ltreefrogd1
  } else {
    LIBS += -ltreefrog1
  }
} else {
  unix:LIBS += -Wl,-rpath,. -Wl,-rpath,/usr/lib -L/usr/lib -ltreefrog
  unix:INCLUDEPATH += /usr/include/treefrog
  linux-*:LIBS += -lrt

# unix:CONFIG += link_pkgconfig
# unix:PKGCONFIG += xtl xtensor
# unix:INCLUDEPATH += /home/jimmy/workspace/git/xtensor-stack/xtl/include
# unix:INCLUDEPATH += /home/jimmy/workspace/git/xtensor-stack/xtensor/include
# header.files = $$HEADER_FILES $$HEADER_CLASSES
}

# unix {
#     CONFIG += link_pkgconfig
#     PKGCONFIG += xtl xtensor
# }


DEFINES += LIBRARY_EXPORTS

DEPENDPATH  += $$PWD/include
INCLUDEPATH += $$PWD/include

CONFIG += conan_basic_setup
# include($$OUT_PWD/conanbuildinfo.pri)

INCLUDEPATH += $$CONAN_INCLUDEPATH_DATAFRAME
DEPENDPATH  += $$CONAN_INCLUDEPATH_DATAFRAME
INCLUDEPATH += $$CONAN_INCLUDEPATH_XTENSOR
DEPENDPATH  += $$CONAN_INCLUDEPATH_XTENSOR
LIBS += $$CONAN_LIBDIRS_DATAFRAME -lDataFrame


windows:CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
}

VERSION = $$ABQ_VERSION

isEmpty(target.path) {
  windows {
    target.path = C:/abquant/$${VERSION}/bin
  } else:macx {
    target.path = /usr/local/lib
  } else:unix {
    target.path = /usr/local/lib
  }
}
INSTALLS += target

!CONFIG(debug, debug|release) {
  DEFINES += ABQ_NO_DEBUG
}

isEmpty( use_gui ) {
  QT    -= gui widgets
} else {
  QT    += gui widgets
  DEFINES += ABQ_USE_GUI_MODULE
}
