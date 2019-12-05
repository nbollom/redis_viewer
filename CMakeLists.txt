cmake_minimum_required(VERSION 3.14)
project(redis_viewer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(REDIS_VIEWER_SOURCES
        ${PROJECT_BINARY_DIR}/config.h
        controller.cpp
        controller.h
        gzip.h
        hash_tab_document.cpp
        hash_tab_document.h
        images.qrc
        json.h
        main.cpp
        main_window.cpp
        main_window.h
        quick_connect_dialog.cpp
        quick_connect_dialog.h
        redis_connection.cpp
        redis_connection.h
        redis_exceptions.h
        serialiser.cpp
        serialiser.h
        settings.cpp
        settings.h
        standard_redis_connection.cpp
        standard_redis_connection.h
        string_tab_document.cpp
        string_tab_document.h
        tab_document.h
        task_queue.cpp
        task_queue.h
        value_editor.cpp
        value_editor.h
        welcome_tab.cpp
        welcome_tab.h
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED) # GUI
find_package(hiredis REQUIRED) # For connecting to redis servers
find_package(libuv REQUIRED) # Run stuff in a background thread
find_package(cppcodec) # Base64 encoding/decoding for values (optional)
find_package(ZLIB) # GZip compression for values (optional)

include_directories(
        ${HIREDIS_INCLUDE_DIRS}
        ${LIBUV_INCLUDE_DIRS}
        ${PROJECT_BINARY_DIR}
)

if(${CPPCODEC_FOUND})
    set(ENABLE_BASE64 ON)
    include_directories(${CPPCODEC_INCLUDE_DIRS})
endif()

if(${ZLIB_FOUND})
    set(ENABLE_ZLIB ON)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()

configure_file(config.h.in config.h)


add_executable(redis_viewer ${REDIS_VIEWER_SOURCES})

target_link_libraries(
        redis_viewer
        Qt5::Core
        Qt5::Widgets
        ${HIREDIS_LIBRARIES}
        ${LIBUV_LIBRARIES}
        ${ZLIB_LIBRARIES}
)
