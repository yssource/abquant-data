TARGET   = foo
TEMPLATE = lib
# CONFIG += no_keywords # avoid clash with slots in Python.h
# CONFIG += shared console force_debug_info c++11
CONFIG += shared console force_debug_info
CONFIG += lib_bundle
CONFIG += plugin no_plugin_name_prefix
# CONFIG += plugin
# QT += widgets
# QT += network
QT += sql
QT -= gui

# QMAKE_CXX = clang++
# QMAKE_LINK = clang++
# QMAKE_CC = clang
# QMAKE_CXXFLAGS += -std=c++17 -stdlib=libc++
# QMAKE_LFLAGS += -stdlib=libc++ -fuse-ld=lld

QMAKE_CXXFLAGS += -std=c++17
# QMAKE_CXXFLAGS -= -std=gnu++11

DEFINES += DMS_INCLUDE_SOURCE

include(pyside2.pri)
DESTDIR = $$ABQUANT_HOME/lib

WRAPPED_HEADER = $$PWD/glue/bindings.h
WRAPPER_DIR = $$OUT_PWD/foo
TYPESYSTEM_FILE = $$PWD/glue/bindings.xml

QT_INCLUDEPATHS = -I"$$[QT_INSTALL_HEADERS]" -I"$$[QT_INSTALL_HEADERS]/QtCore" \
    -I. \
    -I"$$[QT_INSTALL_HEADERS]/QtSql"
    # -I"$$[QT_INSTALL_HEADERS]/QtGui" -I"$$[QT_INSTALL_HEADERS]/QtWidgets"

# On macOS, check if Qt is a framework build. This affects how include paths should be handled.
qtConfig(framework): QT_INCLUDEPATHS += --framework-include-paths=$$[QT_INSTALL_LIBS]

SHIBOKEN_OPTIONS = --generator-set=shiboken --enable-parent-ctor-heuristic \
    --enable-pyside-extensions --enable-return-value-heuristic --use-isnull-as-nb_nonzero \
    $$QT_INCLUDEPATHS -I$$PWD -I$$MKSPECS_INCLUDE -I$$TREEFROG_INCLUDE \
    -I$$ABQUANT_CXX \
    # -I$$ABQUANT_CXX_TEST \
    -I$$ABQUANT_CXX_TEST/blog -I$$ABQUANT_CXX_TEST/indexday \
    -I$$ABQUANT_CXX_TEST/stockmin \
    -I$$ABQUANT_CXX_TEST/xdxr \
    # -I$$DF_ROOT/include \
    -T$$PWD -T$$PYSIDE2/typesystems --output-directory=$$OUT_PWD

# MSVC does not honor #define protected public...
win32:SHIBOKEN_OPTIONS += --avoid-protected-hack

# Prepare the shiboken tool
QT_TOOL.shiboken.binary = $$system_path($$SHIBOKEN2_GENERATOR/shiboken2)
qtPrepareTool(SHIBOKEN, shiboken)


WRAPPER_FILES = $$WRAPPER_DIR/foo_wrapper.cpp \
                $$WRAPPER_DIR/testblog_wrapper.cpp \
                $$WRAPPER_DIR/blog_wrapper.cpp \
                $$WRAPPER_DIR/doo_wrapper.cpp \
                $$WRAPPER_DIR/indexday_wrapper.cpp \
                $$WRAPPER_DIR/indexdayaction_wrapper.cpp \
                $$WRAPPER_DIR/stockday_wrapper.cpp \
                $$WRAPPER_DIR/stockmin_wrapper.cpp \
                $$WRAPPER_DIR/stockminaction_wrapper.cpp \
                $$WRAPPER_DIR/xdxr_wrapper.cpp

# Shiboken run that adds the module wrapper to GENERATED_SOURCES
shiboken.output = $$WRAPPER_DIR/foo_module_wrapper.cpp

# shiboken.input = $$WRAPPER_DIR/foo_module_wrapper.cpp \
#                   $$WRAPPER_DIR/foo_wrapper.cpp \
#                   $$WRAPPER_DIR/testblog_wrapper.cpp \
#                   $$WRAPPER_DIR/doo_wrapper.cpp

# shiboken.commands = $$SHIBOKEN $$SHIBOKEN_OPTIONS $$PWD/glue/bindings.h $$TYPESYSTEM_FILE
shiboken.commands = $$SHIBOKEN $$SHIBOKEN_OPTIONS $$PWD/glue/bindings.h ${QMAKE_FILE_IN}
shiboken.input = TYPESYSTEM_FILE
shiboken.dependency_type = TYPE_C
shiboken.variable_out = GENERATED_SOURCES

# A dummy command that pretends to produce the class wrappers from the headers
# depending on the module wrapper
# WRAPPED_CLASSES = $$PWD/glue/bindings.h
# WRAPPED_CLASSES = $$PWD/../cxx/test/blog/testblog.h
WRAPPED_CLASSES =

module_wrapper_dummy_command.output = $$WRAPPER_DIR/${QMAKE_FILE_BASE}.cpp
module_wrapper_dummy_command.commands = echo ${QMAKE_FILE_IN}
module_wrapper_dummy_command.depends = $$WRAPPER_DIR/foo_module_wrapper.cpp
module_wrapper_dummy_command.input = WRAPPER_FILES
# module_wrapper_dummy_command.input =
module_wrapper_dummy_command.dependency_type = TYPE_C
module_wrapper_dummy_command.variable_out = GENERATED_SOURCES


# Get the path component to the active config build folder
defineReplace(getOutDir) {
  out_dir = $$OUT_PWD
  CONFIG(release, debug|release): out_dir = $$out_dir/release
  else:out_dir = $$out_dir/debug
  return($$out_dir)
}

# Create hardlinks to the PySide2 shared libraries, so the example can be executed without manually
# setting the PATH.
win32 {
    out_dir = $$getOutDir()
    # no_link tell not to link to the output files, target_predeps forces the command to actually
    # execute, explicit_dependencies is a magic value that tells qmake not to run the commands
    # if the output files already exist.
    hard_link_libraries.CONFIG = no_link target_predeps explicit_dependencies
    hard_link_libraries.output = $$out_dir/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
    hard_link_libraries.commands = mklink /H $$shell_path($$out_dir/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}) $$shell_path(${QMAKE_FILE_IN})
    hard_link_libraries.input = PYSIDE2_SHARED_LIBRARIES SHIBOKEN2_SHARED_LIBRARIES
}

QMAKE_EXTRA_COMPILERS += shiboken module_wrapper_dummy_command
# QMAKE_EXTRA_COMPILERS += module_wrapper_dummy_command shiboken
# QMAKE_EXTRA_COMPILERS += module_wrapper_dummy_command
# QMAKE_EXTRA_COMPILERS += shiboken
win32:QMAKE_EXTRA_COMPILERS += hard_link_libraries

INCLUDEPATH += $$WRAPPER_DIR

for(i, PYSIDE2_INCLUDE) {
    # INCLUDEPATH += $$i/QtWidgets $$i/QtGui $$i/QtCore
    INCLUDEPATH += $$i/QtCore
}

# ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/foo_module_wrapper.cpp
# ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/testblog_wrapper.cpp
# ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/doo_wrapper.cpp
# # ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/twebapplication_wrapper.cpp
# ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/foo_wrapper.cpp)
# ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/indexday_wrapper.cpp
# ${CMAKE_CURRENT_BINARY_DIR}/${bindings_library}/indexdayaction_wrapper.cpp


# SOURCES += $$WRAPPER_FILES

# SOURCES += $(GENERATED_SOURCES)
# SOURCES += $$PWD/qbuild/foo/foo_module_wrapper.cpp \
#            $$PWD/qbuild/foo/testblog_wrapper.cpp \
#            $$PWD/qbuild/foo/doo_wrapper.cpp \
#            $$PWD/qbuild/foo/foo_wrapper.cpp

# HEADERS += tactioncontext.h
HEADERS += /home/jimmy/workspace/git/treefrogframework/treefrog-framework/include/tactioncontext.h
# HEADERS += \
#     mainwindow.h \
#     pythonutils.h

# OTHER_FILES += $$TYPESYSTEM_FILE $$WRAPPED_HEADER pyside2_config.py README.md
OTHER_FILES += $$TYPESYSTEM_FILE $$WRAPPED_HEADER pyside2_config.py
