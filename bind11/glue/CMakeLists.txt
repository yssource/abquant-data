# ================================ General configuration ======================================

add_definitions(-DQT_NO_KEYWORDS)
# Set CPP standard to C++11 minimum.
set(CMAKE_CXX_STANDARD 11)

# The sample library for which we will create bindings. You can change the name to something
# relevant for your project.

# The name of the generated bindings module (as imported in Python). You can change the name
# to something relevant for your project.
set(bindings_library "foo")

# set(CMAKE_CXX_FLAGS_RELEASE "-O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDATABASE_SQLITE -D_GLIBCXX_USE_CXX11_ABI=1")

set(foo_source_dir "/home/jimmy/workspace/devel/blogapp/controllers" "/home/jimmy/workspace/devel/blogapp/models" "/home/jimmy/workspace/devel/blogapp/helpers")
set(tree_includes  "/opt/treefrog/include/treefrog/")
set(foo_lib_dir "/home/jimmy/workspace/devel/blogapp/lib")

set(sample_library "${foo_lib_dir}/libcontroller.so;${foo_lib_dir}/libhelper.so;${foo_lib_dir}/libmodel.so;${foo_lib_dir}/libview.so")
message(STATUS "---sssss-------- ${sample_library}")
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


# ====================== Shiboken target for generating binding C++ files  ====================
get_target_property(qtcore_M_QT5_INCLUDES Qt5::Core INTERFACE_INCLUDE_DIRECTORIES)
set(qtcore_M_QT5_INCLUDES ${Qt5Core_INCLUDE_DIRS})

list(JOIN qtcore_M_QT5_INCLUDES ";-I" M_QT5_INCLUDES)

message(STATUS "${M_QT5_INCLUDES}")
set(M_QT5_INCLUDES ${M_QT5_INCLUDES})

message(STATUS "${Qt5Core_INCLUDE_DIRS}")
message(STATUS "${Qt5Core_LIBRARIES}")

# pybind11_add_module(driver db.cxx)

find_package(pybind11 CONFIG REQUIRED)
message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")
# include_directories(${pybind11_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/cc/src/data ${PROJECT_SOURCE_DIR}/cc/src/utils)

# =============================== CMake target - bindings_library =============================


# Set the cpp files which will be used for the bindings library.
set(${bindings_library}_sources main.cpp)

# Define and build the bindings library.
add_library(${bindings_library} MODULE ${${bindings_library}_sources})


# Apply relevant include and link flags.
target_include_directories(${bindings_library} PRIVATE ${python_include_dir})
target_include_directories(${bindings_library} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${bindings_library} PRIVATE "${Qt5Core_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PRIVATE "${Qt5Network_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PRIVATE "${Qt5Sql_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PRIVATE "${Qt5Xml_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PRIVATE "${Qt5Qml_INCLUDE_DIRS}")
# target_include_directories(${bindings_library} PRIVATE "${QtTest_INCLUDE_DIRS}")
target_include_directories(${bindings_library} PRIVATE ${pybind11_INCLUDE_DIRS})
target_include_directories(${bindings_library} PRIVATE ${foo_source_dir})
target_include_directories(${bindings_library} PRIVATE ${tree_includes})

target_link_libraries(${bindings_library} PRIVATE ${sample_library} ${CONAN_LIBS})
# target_link_libraries(${bindings_library} PRIVATE ${CONAN_LIBS})

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
    target_link_libraries(${bindings_library} PRIVATE ${python_link_flags})
endif()


# ================================= Dubious deployment section ================================


if(WIN32)
    # =========================================================================================
    # !!! (The section below is deployment related, so in a real world application you will
    # want to take care of this properly (this is simply to eliminate errors that users usually
    # encounter.
    # =========================================================================================
    # Circumvent some "#pragma comment(lib)"s in "include/pyconfig.h" which might force to link
    # against a wrong python shared library.

    set(python_versions_list 3 32 33 34 35 36 37 38)
    set(python_additional_link_flags "")
    foreach(ver ${python_versions_list})
        set(python_additional_link_flags
            "${python_additional_link_flags} /NODEFAULTLIB:\"python${ver}_d.lib\"")
        set(python_additional_link_flags
            "${python_additional_link_flags} /NODEFAULTLIB:\"python${ver}.lib\"")
    endforeach()

    set_target_properties(${bindings_library}
                           PROPERTIES LINK_FLAGS "${python_additional_link_flags}")

    # Add custom target to hard-link shiboken shared libraries into the build folder, so that
    # the user doesn't have to set the PATH manually to point to the PySide2 package.
    foreach(library_path ${shiboken_shared_libraries})
        string(REGEX REPLACE ".lib$" ".dll" library_path ${library_path})
        get_filename_component(base_name ${library_path} NAME)
        file(TO_NATIVE_PATH ${library_path} source_path)
        file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${base_name}" dest_path)
        add_custom_command(OUTPUT "${base_name}"
                            COMMAND mklink /H "${dest_path}" "${source_path}"
                            DEPENDS ${library_path}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            COMMENT "Creating hardlink to shiboken shared library ${base_name}")

        # Fake target that depends on the previous one, but has special ALL keyword, which means
        # it will always be executed.
        add_custom_target("fake_${base_name}" ALL DEPENDS ${base_name})
    endforeach()
    # =========================================================================================
    # !!! End of dubious section.
    # =========================================================================================
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
# =============================================================================================
# !!! End of dubious section.
# =============================================================================================


############################################################
# Create an executable
############################################################

# Add an executable with the above sources
# # add_executable(${DRIVER}.bin
# add_executable(${DRIVER}
#     person-odb.cxx driver.cxx
#     # dd.cxx
# )

# # Add headers.
# # message(${BEFORE})
# include_directories("include")

# # Add libraries.
# add_library(${BEFORE} STATIC before.cpp)
# add_library(${AFTER} STATIC after.cpp)
# add_library(${SAMPLE} STATIC sample.cpp)

# # # Link libraries.
# target_link_libraries(${BEFORE} ${CONAN_LIBS})
# target_link_libraries(${AFTER} ${CONAN_LIBS})
# target_link_libraries(${SAMPLE} ${CONAN_LIBS})

# add_executable(${MAIN} main.cpp)
# target_link_libraries(${MAIN} ${BEFORE} ${CONAN_LIBS})
