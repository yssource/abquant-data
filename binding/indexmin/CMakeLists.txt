# ================================ General configuration ======================================

# keywords signals
# add_definitions(-DQT_NO_KEYWORDS)

# The sample library for which we will create bindings. You can change the name to something
# relevant for your project.

# The name of the generated bindings module (as imported in Python). You can change the name
# to something relevant for your project.
set(binding_library "pyabqindexmin")
set(pyabqindexmin_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../cxx/include)
set(tree_includes  "/usr/include/treefrog/")
set(tf_lib "/usr/lib/libtreefrog.so")
set(abquant_lib_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)
set(core_libraries "${abquant_lib_dir}/libabqcontroller.so;${abquant_lib_dir}/libabqhelper.so;${abquant_lib_dir}/libabqmodel.so;${abquant_lib_dir}/libabqaction.so;${tf_lib}")
set(binding_library_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/)


# =============================== CMake target - binding_library =============================

# Set the cpp files which will be used for the bindings library.
set(${binding_library}_sources main.cpp)

# Define and build the bindings library.
# add_library(${binding_library} MODULE ${${binding_library}_sources})
pybind11_add_module(${binding_library} SHARED ${${binding_library}_sources} NO_EXTRAS)

# target_include_directories(${binding_library} PRIVATE ${CONAN_INCLUDE_DIRS})

# Apply relevant include and link flags.
target_include_directories(${binding_library} PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(${binding_library} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${binding_library} PRIVATE ${pyabqindexmin_include_dir})
target_include_directories(${binding_library} PRIVATE ${tree_includes})

target_include_directories(${binding_library} PRIVATE "${Qt5Core_INCLUDE_DIRS}")
target_include_directories(${binding_library} PRIVATE "${Qt5Network_INCLUDE_DIRS}")
target_include_directories(${binding_library} PRIVATE "${Qt5Sql_INCLUDE_DIRS}")
target_include_directories(${binding_library} PRIVATE "${Qt5Xml_INCLUDE_DIRS}")
target_include_directories(${binding_library} PRIVATE "${Qt5Qml_INCLUDE_DIRS}")
# target_include_directories(${binding_library} PRIVATE "${QtTest_INCLUDE_DIRS}")
target_include_directories(${binding_library} PRIVATE ${pybind11_INCLUDE_DIRS})

target_link_libraries(${binding_library} PRIVATE ${core_libraries} ${CONAN_LIBS})
set_property(TARGET ${binding_library} PROPERTY CXX_STANDARD 17)

# Adjust the name of generated module.
set_property(TARGET ${binding_library} PROPERTY PREFIX "")
set_property(TARGET ${binding_library} PROPERTY OUTPUT_NAME
             "${binding_library}${PYTHON_EXTENSION_SUFFIX}")
if(WIN32)
    set_property(TARGET ${binding_library} PROPERTY SUFFIX ".pyd")
endif()

# Make sure the linker doesn't complain about not finding Python symbols on macOS.
if(APPLE)
  set_target_properties(${binding_library} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)

# Find and link to the python import library only on Windows.
# On Linux and macOS, the undefined symbols will get resolved by the dynamic linker
# (the symbols will be picked up in the Python executable).
if (WIN32)
    list(GET python_linking_data 0 python_libdir)
    list(GET python_linking_data 1 python_lib)
    find_library(python_link_flags ${python_lib} PATHS ${python_libdir} HINTS ${python_libdir})
    target_link_libraries(${binding_library} PRIVATE ${python_link_flags})
endif()


# =============================================================================================
# !!! (The section below is deployment related, so in a real world application you will want to
# take care of this properly with some custom script or tool).
# =============================================================================================
# Install the library and the bindings module into the source folder near the main.py file, so
# that the Python interpeter successfully imports the used module.
# install(TARGETS ${binding_library} ${core_libraries}
#         LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
#         RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
#         )
install(TARGETS ${binding_library}
        LIBRARY DESTINATION ${binding_library_dir}
        RUNTIME DESTINATION ${binding_library_dir}
        )

add_custom_command(TARGET ${binding_library} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:${binding_library}> ${abquant_lib_dir})
