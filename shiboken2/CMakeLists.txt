project(binding)

cmake_minimum_required(VERSION 3.1)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
# set(CMAKE_AUTOUIC ON)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
# find_package(PythonLibs REQUIRED)
# find_package(Shiboken REQUIRED)

# find_program(GENERATOR generatorrunner REQUIRED)
# if (NOT GENERATOR)
#     message(FATAL_ERROR "You need to specify GENERATOR variable (-DGENERATOR=value)")
# endif()

# find_package(Qt5Core)
find_package(Qt5 REQUIRED Core)

get_target_property(QtCore_location Qt5::Core LOCATION)
get_filename_component(QtCore_libdir ${QtCore_location} DIRECTORY)
message(STATUS "${QtCore_libdir}")
message(STATUS "${QtCore_location}")

enable_testing()

add_subdirectory(glue)
# add_subdirectory(test)
