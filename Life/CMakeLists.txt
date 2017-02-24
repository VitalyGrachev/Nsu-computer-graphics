cmake_minimum_required(VERSION 3.6)
project(Life)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    model/life_field.cpp
    model/life_field.h
    view/canvas.cpp
    view/canvas.h
    view/field_display.cpp
    view/field_display.h
    view/hex_grid_canvas.cpp
    view/hex_grid_canvas.h
    view/main_window.cpp
    view/main_window.h
    main.cpp)

find_package(Qt5Widgets REQUIRED)

add_executable(Life ${SOURCE_FILES})

target_link_libraries(Life Qt5::Widgets)