cmake_minimum_required(VERSION 3.30)
project(deepseek_qt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Network
  REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_executable(deepseek_qt
        main.cpp

        MainWindow.ui
        MainWindow.cpp
        DeepseekClient.cpp
        DeepseekClient.h
)
target_link_libraries(deepseek_qt
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)

