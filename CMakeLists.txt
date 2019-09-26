cmake_minimum_required(VERSION 3.14)
project(redis_viewer)

set(CMAKE_CXX_STANDARD 14)

set(REDIS_VIEWER_SOURCES
    main.cpp
    main_window.cpp main_window.h
    welcome_tab.cpp welcome_tab.h
        controller.cpp controller.h redis_connection.h redis_result.cpp redis_result.h tab_document.h settings.cpp settings.h serialiser.cpp serialiser.h)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(redis_viewer ${REDIS_VIEWER_SOURCES})
target_link_libraries(redis_viewer Qt5::Core Qt5::Widgets)
