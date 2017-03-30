cmake_minimum_required(VERSION 3.6)
project(Filter)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
        main.cpp
        view/main_window.cc
        view/main_window.h
        view/zone.cc
        view/zone.h
        filters/abstract_filter.h
        filter_applicator.cc
        filter_applicator.h
        image_wrapper.cc
        image_wrapper.h
        filter_runner.cc
        filter_runner.h
        filters/grayscale/grayscale_filter.cc
        filters/grayscale/grayscale_filter.h
        filters/base_filter_widget.h
        view/filter_action.cc
        view/filter_action.h
        factory.h
        filters/grayscale/grayscale_widget.cc
        filters/grayscale/grayscale_widget.h
        view/zone_a.cc
        view/zone_a.h
        view/filter_action_group.cc
        view/filter_action_group.h
        filters/base_filter_widget.cc
        filters/zoom/zoom_filter.cc
        filters/zoom/zoom_filter.h
        filters/zoom/zoom_widget.cc
        filters/zoom/zoom_widget.h
        filters/convolution/convolution_filter.h
        filters/convolution/square_matrix.h
        filter_action_group_creator.cc
        filter_action_group_creator.h
        filters/negative/negative_filter.cc
        filters/negative/negative_filter.h
        filters/negative/negative_widget.cc
        filters/negative/negative_widget.h
        filters/blur/blur_filter.cc
        filters/blur/blur_filter.h
        filters/blur/blur_widget.cc
        filters/blur/blur_widget.h
        filters/gamma_correction/gamma_correction_filter.cc
        filters/gamma_correction/gamma_correction_filter.h
        filters/gamma_correction/gamma_correction_widget.cc
        filters/gamma_correction/gamma_correction_widget.h
        view/value_setter.cc
        view/value_setter.h
        filters/sharpening/sharpening_filter.cc
        filters/sharpening/sharpening_filter.h
        filters/sharpening/sharpening_widget.cc
        filters/sharpening/sharpening_widget.h filters/emboss/emboss_filter.cc filters/emboss/emboss_filter.h filters/emboss/emboss_widget.cc filters/emboss/emboss_widget.h filters/watercolor/watercolor_filter.cc filters/watercolor/watercolor_filter.h filters/watercolor/watercolor_widget.cc filters/watercolor/watercolor_widget.h filters/blur/median_blur_filter.cc filters/blur/median_blur_filter.h)

find_package(Qt5Widgets REQUIRED)

add_executable(Filter ${SOURCE_FILES})

target_link_libraries(Filter Qt5::Widgets)