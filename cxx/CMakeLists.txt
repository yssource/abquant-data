cmake_minimum_required(VERSION 3.0)

project(appname CXX)

# cxx options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Produce compile_commands.json

# Auto generate moc files
if (POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()
set(CMAKE_AUTOMOC ON)

find_package(TreeFrog REQUIRED)
add_subdirectory(helpers)
add_subdirectory(models)
add_subdirectory(views)
add_subdirectory(controllers)

# Cutom target - cmake
include(${PROJECT_SOURCE_DIR}/cmake/TargetCmake.cmake)
