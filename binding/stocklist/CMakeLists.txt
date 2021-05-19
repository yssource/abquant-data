# ================================ General configuration ======================================

# keywords signals
# add_definitions(-DQT_NO_KEYWORDS)

# The sample library for which we will create bindings. You can change the name to something
# relevant for your project.

# The name of the generated bindings module (as imported in Python). You can change the name
# to something relevant for your project.
set(bindings_library "abqstocklist")

# set(CMAKE_CXX_FLAGS_RELEASE "-O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_SQL_LIB")

set(abqstocklist_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../cxx/include)
set(tree_includes  "/usr/include/treefrog/")
# set(tree_lib "/usr/lib/libtreefrog.so")
set(abquant_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

#Bugfix: libabqaction.so -> libabqaction.a, since it is not able to port
# DataFrame to outside for binding.
set(sample_library "${abquant_lib_dir}/libabqcontroller.so;${abquant_lib_dir}/libabqhelper.so;${abquant_lib_dir}/libabqmodel.so;${abquant_lib_dir}/libabqaction.a;/usr/lib/libtreefrog.so")

set(bindings_library_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/)


# ==================================== RPATH configuration ====================================


# =============================================================================================
# !!! (The section below is deployment related, so in a real world application you will want to
# take care of this properly with some custom script or tool).
# =============================================================================================
# Enable rpaths so that the built shared libraries find their dependencies.
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH ${CMAKE_CURRENT_SOURCE_DIR} ${QtCore_libdir})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# =============================================================================================
# !!! End of dubious section.
# =============================================================================================


# # ====================== Shiboken target for generating binding C++ files  ====================
# get_target_property(qtcore_M_QT5_INCLUDES Qt5::Core INTERFACE_INCLUDE_DIRECTORIES)
set(qtcore_M_QT5_INCLUDES ${Qt5Core_INCLUDE_DIRS})

list(JOIN qtcore_M_QT5_INCLUDES ";-I" M_QT5_INCLUDES)

message(STATUS "${M_QT5_INCLUDES}")
set(M_QT5_INCLUDES ${M_QT5_INCLUDES})


# =============================== CMake target - bindings_library =============================

# Set the cpp files which will be used for the bindings library.
set(${bindings_library}_sources main.cpp)

# Define and build the bindings library.
# add_library(${bindings_library} MODULE ${${bindings_library}_sources})
pybind11_add_module(${bindings_library} SHARED ${${bindings_library}_sources} NO_EXTRAS)

# target_include_directories(${bindings_library} PUBLIC ${CONAN_INCLUDE_DIRS})

# Apply relevant include and link flags.
target_include_directories(${bindings_library} PUBLIC ${python_include_dir})
target_include_directories(${bindings_library} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${bindings_library} PUBLIC ${abqstocklist_include_dir})
target_include_directories(${bindings_library} PUBLIC ${tree_includes})

target_include_directories(${bindings_library} PUBLIC "${Qt5Core_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PUBLIC "${Qt5Network_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PUBLIC "${Qt5Sql_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PUBLIC "${Qt5Xml_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PUBLIC "${Qt5Qml_INCLUDE_DIRS}")
# target_include_directories(${bindings_library} PUBLIC "${QtTest_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PUBLIC ${pybind11_INCLUDE_DIRS})

target_link_libraries(${bindings_library} PUBLIC ${sample_library} ${CONAN_LIBS})
set_property(TARGET ${bindings_library} PROPERTY CXX_STANDARD 17)

# Adjust the name of generated module.
set_property(TARGET ${bindings_library} PROPERTY PREFIX "")
set_property(TARGET ${bindings_library} PROPERTY OUTPUT_NAME
             "${bindings_library}${PYTHON_EXTENSION_SUFFIX}")
if(WIN32)
    set_property(TARGET ${bindings_library} PROPERTY SUFFIX ".pyd")
endif()

# Make sure the linker doesn't complain about not finding Python symbols on macOS.
if(APPLE)
  set_target_properties(${bindings_library} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)

# Find and link to the python import library only on Windows.
# On Linux and macOS, the undefined symbols will get resolved by the dynamic linker
# (the symbols will be picked up in the Python executable).
if (WIN32)
    list(GET python_linking_data 0 python_libdir)
    list(GET python_linking_data 1 python_lib)
    find_library(python_link_flags ${python_lib} PATHS ${python_libdir} HINTS ${python_libdir})
    target_link_libraries(${bindings_library} PUBLIC ${python_link_flags})
endif()


# =============================================================================================
# !!! (The section below is deployment related, so in a real world application you will want to
# take care of this properly with some custom script or tool).
# =============================================================================================
# Install the library and the bindings module into the source folder near the main.py file, so
# that the Python interpeter successfully imports the used module.
# install(TARGETS ${bindings_library} ${sample_library}
#         LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
#         RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
#         )
install(TARGETS ${bindings_library}
        LIBRARY DESTINATION ${bindings_library_dir}
        RUNTIME DESTINATION ${bindings_library_dir}
        )

add_custom_command(TARGET ${bindings_library} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:${bindings_library}> ${abquant_lib_dir})
