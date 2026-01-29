cmake_minimum_required(VERSION 3.16)

project(
  qt-test-task
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_standard_project_setup()

qt_add_executable(
  qt-test-task
  src/main.cpp
  src/fileModifier.cpp
  src/fileScheduler.cpp
  src/appController.cpp
  src/taskModel.hpp
  src/taskModel.cpp)

qt_add_qml_module(
  qt-test-task
  URI
  App
  VERSION
  1.0
  QML_FILES
  qml/Main.qml)

target_link_libraries(qt-test-task PRIVATE Qt6::Core Qt6::Gui Qt6::Qml
                                           Qt6::Quick)
