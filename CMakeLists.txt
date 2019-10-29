cmake_minimum_required(VERSION 3.14)
project(redis_viewer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(REDIS_VIEWER_SOURCES
    main.cpp
    main_window.cpp main_window.h
    welcome_tab.cpp welcome_tab.h
        controller.cpp controller.h redis_connection.h tab_document.h settings.cpp settings.h serialiser.cpp serialiser.h standard_redis_connection.cpp standard_redis_connection.h task_queue.cpp task_queue.h quick_connect_dialog.cpp quick_connect_dialog.h redis_connection.cpp redis_exceptions.h string_tab_document.cpp string_tab_document.h)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(hiredis REQUIRED)
find_package(libuv REQUIRED)

include_directories(${HIREDIS_INCLUDE_DIRS} ${LIBUV_INCLUDE_DIRS})
add_executable(redis_viewer ${REDIS_VIEWER_SOURCES} images.qrc)
target_link_libraries(redis_viewer Qt5::Core Qt5::Widgets ${HIREDIS_LIBRARIES} ${LIBUV_LIBRARIES})
