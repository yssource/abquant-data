cmake_minimum_required(VERSION 2.8.12)
project(abquant)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CONAN_INCLUDE_DIRS})

# cxx options
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Produce compile_commands.json
set(CMAKE_CXX_STANDARD 17)
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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# add_definitions(-DQT_NO_KEYWORDS)

# find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
# find_package(Shiboken REQUIRED)

# find_program(GENERATOR generatorrunner REQUIRED)
# if (NOT GENERATOR)
#     message(FATAL_ERROR "You need to specify GENERATOR variable (-DGENERATOR=value)")
# endif()

find_package(Qt5 REQUIRED Core Network Sql Xml Qml)
# find_package(pybind11 REQUIRED)  # or add_subdirectory(pybind11)
# add_subdirectory(pybind11)
add_subdirectory(bind11)


get_target_property(QtCore_location Qt5::Core LOCATION)
get_filename_component(QtCore_libdir ${QtCore_location} DIRECTORY)
message(STATUS "${QtCore_libdir}")
message(STATUS "${QtCore_location}")

enable_testing()
message(status ${CONAN_LIBS})
