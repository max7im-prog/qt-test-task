cmake_minimum_required(VERSION 3.16)

project(
  qt-test-task
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(qt-test-task src/main.cpp)

target_link_libraries(qt-test-task PRIVATE Qt6::Core)
