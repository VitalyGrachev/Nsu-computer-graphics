cmake_minimum_required(VERSION 3.6)
project(Isolines)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    view/value_setter.cc
    view/value_setter.h
    image_wrapper.cc
    image_wrapper.h view/main_window.cc view/main_window.h color_map_painter.cc color_map_painter.h isoline_painter.cc isoline_painter.h view/color_map_widget.cc view/color_map_widget.h coordinates_converter.cc coordinates_converter.h function_types.h color_matcher.cc color_matcher.h plain_color_matcher.cc plain_color_matcher.h smooth_color_matcher.cc smooth_color_matcher.h main.cc)

find_package(Qt5Widgets REQUIRED)

add_executable(Isolines ${SOURCE_FILES})

target_link_libraries(Isolines Qt5::Widgets)