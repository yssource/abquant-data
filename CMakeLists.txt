cmake_minimum_required(VERSION 2.8.12)
project(abquant)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# cxx options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Produce compile_commands.json
set(CMAKE_CXX_STANDARD 11)
if (MSVC)
  set(CMAKE_CXX_FLAGS         "/DUNICODE /D_UNICODE")
  set(CMAKE_CXX_FLAGS_DEBUG   "/DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DQT_NO_DEBUG /DTF_NO_DEBUG")
else()
  set(CMAKE_CXX_FLAGS         "-Wall -W -D_REENTRANT")
  set(CMAKE_CXX_FLAGS_DEBUG   "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DQT_NO_DEBUG -DTF_NO_DEBUG")
endif()

# Auto generate moc files
if (POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
# set(CMAKE_AUTOUIC ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# add_definitions(-DQT_NO_KEYWORDS)

# find_package(pybind11 REQUIRED)  # or add_subdirectory(pybind11)


find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
# find_package(PythonLibs REQUIRED)
# find_package(Shiboken REQUIRED)

# find_program(GENERATOR generatorrunner REQUIRED)
# if (NOT GENERATOR)
#     message(FATAL_ERROR "You need to specify GENERATOR variable (-DGENERATOR=value)")
# endif()

# find_package(Qt5Core)
find_package(Qt5 REQUIRED Core Network Sql Xml Qml)

get_target_property(QtCore_location Qt5::Core LOCATION)
get_filename_component(QtCore_libdir ${QtCore_location} DIRECTORY)
message(STATUS "${QtCore_libdir}")
message(STATUS "${QtCore_location}")

enable_testing()
message(status ${CONAN_LIBS})
add_subdirectory(bind11)

# pybind11_add_module(cmake_example cc/src/data/main.cpp)
# pybind11_add_module(driver cc/src/data/db.cxx)
