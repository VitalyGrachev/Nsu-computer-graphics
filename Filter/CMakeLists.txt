cmake_minimum_required(VERSION 3.6)
project(Filter)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp view/main_window.cc view/main_window.h view/zone.cc view/zone.h filters/abstract_filter.h model/filter_applicator.cc model/filter_applicator.h filters/util.h filters/util.cc filters/image_handler.cc filters/image_handler.h filters/rgb888.h)

find_package(Qt5Widgets REQUIRED)

add_executable(Filter ${SOURCE_FILES})

target_link_libraries(Filter Qt5::Widgets)