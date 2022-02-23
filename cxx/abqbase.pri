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
}

DEFINES += TF_DLL
DEFINES += LIBRARY_EXPORTS

DEPENDPATH  += $$PWD/include
INCLUDEPATH += $$PWD/include

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

CONFIG += conan_basic_setup
include($$TOP_BUILD_DIR/conanbuildinfo.pri)
LIBS -= $$CONAN_LIBS_QT
