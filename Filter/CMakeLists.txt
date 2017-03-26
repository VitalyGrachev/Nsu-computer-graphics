cmake_minimum_required(VERSION 3.6)
project(Filter)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp view/main_window.cc view/main_window.h view/zone.cc view/zone.h filters/abstract_filter.h filter_applicator.cc filter_applicator.h image_wrapper.cc image_wrapper.h filter_runner.cc filter_runner.h filters/grayscale/grayscale_filter.cc filters/grayscale/grayscale_filter.h filters/base_filter_widget.h view/filter_action.cc view/filter_action.h factory.h filters/grayscale/grayscale_widget.cc filters/grayscale/grayscale_widget.h)

find_package(Qt5Widgets REQUIRED)

add_executable(Filter ${SOURCE_FILES})

target_link_libraries(Filter Qt5::Widgets)