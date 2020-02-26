# PYSIDE_CONFIG = $$PWD/../utils/pyside2_config.py
PYSIDE_CONFIG = /home/jimmy/.pyenv/versions/anaconda3-2018.12/envs/devel/lib/python3.7/site-packages/PySide2/examples/utils/pyside2_config.py

# Use provided python interpreter if given.
isEmpty(python_interpreter) {
    python_interpreter = python
}
message(Using python interpreter: $$python_interpreter)

SHIBOKEN2_GENERATOR = $$system($$python_interpreter $$PYSIDE_CONFIG --shiboken2-generator-path)
isEmpty(SHIBOKEN2_GENERATOR): error(Unable to locate the shiboken2-generator package location)

SHIBOKEN2_MODULE = $$system($$python_interpreter $$PYSIDE_CONFIG --shiboken2-module-path)
isEmpty(SHIBOKEN2_MODULE): error(Unable to locate the shiboken2 package location)

PYSIDE2 = $$system($$python_interpreter $$PYSIDE_CONFIG --pyside2-path)
isEmpty(PYSIDE2): error(Unable to locate the PySide2 package location)

PYTHON_INCLUDE = $$system($$python_interpreter $$PYSIDE_CONFIG --python-include-path)
isEmpty(PYTHON_INCLUDE): error(Unable to locate the Python include headers directory)

PYTHON_LFLAGS = $$system($$python_interpreter $$PYSIDE_CONFIG --python-link-flags-qmake)
isEmpty(PYTHON_LFLAGS): error(Unable to locate the Python library for linking)

SHIBOKEN2_INCLUDE = $$system($$python_interpreter $$PYSIDE_CONFIG --shiboken2-generator-include-path)
isEmpty(SHIBOKEN2_INCLUDE): error(Unable to locate the shiboken include headers directory)

PYSIDE2_INCLUDE = $$system($$python_interpreter $$PYSIDE_CONFIG --pyside2-include-path)
isEmpty(PYSIDE2_INCLUDE): error(Unable to locate the PySide2 include headers directory)

SHIBOKEN2_LFLAGS = $$system($$python_interpreter $$PYSIDE_CONFIG --shiboken2-module-qmake-lflags)
isEmpty(SHIBOKEN2_LFLAGS): error(Unable to locate the shiboken libraries for linking)

PYSIDE2_LFLAGS = $$system($$python_interpreter $$PYSIDE_CONFIG --pyside2-qmake-lflags)
isEmpty(PYSIDE2_LFLAGS): error(Unable to locate the PySide2 libraries for linking)

SHIBOKEN2_SHARED_LIBRARIES = $$system($$python_interpreter $$PYSIDE_CONFIG --shiboken2-module-shared-libraries-qmake)
isEmpty(SHIBOKEN2_SHARED_LIBRARIES): error(Unable to locate the used shiboken2 module shared libraries)

PYSIDE2_SHARED_LIBRARIES = $$system($$python_interpreter $$PYSIDE_CONFIG --pyside2-shared-libraries-qmake)
isEmpty(PYSIDE2_SHARED_LIBRARIES): error(Unable to locate the used PySide2 shared libraries)

TREEFROG_INCLUDE = /home/jimmy/workspace/git/treefrogframework/treefrog-framework/include
ABQUANT_HOME = /home/jimmy/workspace/devel/abquant
ABQUANT_CXX = $$ABQUANT_HOME/cxx
ABQUANT_CXX_TEST = $$ABQUANT_CXX/test
MKSPECS_INCLUDE = /home/jimmy/Qt/5.12.1/gcc_64/mkspecs/linux-g++

DF_ROOT = /home/jimmy/workspace/git/hosseinmoein/DataFrame
DF_LIB = $$DF_ROOT/cbuild/lib64 -lDataFrame
TEST_LIBS = -L$$ABQUANT_HOME/cxx/lib -ltestblog -lindexday -lstockday -lstockmin -lxdxr
TREEFROG_LIBS = -L/opt/treefrog/lib -Wl,-rpath,$$lib.path -L$$lib.path -ltreefrog
INCLUDEPATH += "$$PYTHON_INCLUDE" $$PYSIDE2_INCLUDE $$SHIBOKEN2_INCLUDE $$MKSPECS_INCLUDE $$TREEFROG_INCLUDE $$ABQUANT_CXX $$ABQUANT_CXX/models $$ABQUANT_CXX_TEST $$ABQUANT_CXX_TEST/blog  $$ABQUANT_CXX_TEST/indexday $$ABQUANT_CXX_TEST/stockday $$ABQUANT_CXX_TEST/stockmin $$ABQUANT_CXX_TEST/xdxr
# $$DF_ROOT/include
LIBS += $$PYTHON_LFLAGS $$PYSIDE2_LFLAGS $$SHIBOKEN2_LFLAGS $$TEST_LIBS $$TREEFROG_LIBS
# $$DF_LIB
linux-*:LIBS += -lrt

!build_pass:message(PYTHON_LFLAGS LFLAGS is $$PYTHON_LFLAGS)
!build_pass:message(PYSIDE2_LFLAGS is $$PYSIDE2_LFLAGS)
!build_pass:message(INCLUDEPATH is $$INCLUDEPATH)
!build_pass:message(LIBS are $$LIBS)

!build_pass:message(Using $$PYSIDE2)

!win32 {
    !build_pass:message(RPATH will include $$PYSIDE2 and $$SHIBOKEN2_MODULE)
    QMAKE_RPATHDIR += $$PYSIDE2 $$SHIBOKEN2_MODULE
}
