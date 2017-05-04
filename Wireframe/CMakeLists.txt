cmake_minimum_required(VERSION 3.7)
project(Wireframe)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cc main_window.cc main_window.h visualization/scene.cc visualization/scene.h figure/plain_segment_object.cc figure/plain_segment_object.h visualization/segment.h visualization/camera.cc visualization/camera.h util/image_wrapper.cc util/image_wrapper.h view/wireframe_widget.cc view/wireframe_widget.h view/spline_widget.cc view/spline_widget.h view/object_properties_widget.cc view/object_properties_widget.h figure/solid_of_revolution.cc figure/solid_of_revolution.h visualization/base_object.h visualization/base_object.cc)
find_package(Qt5Widgets REQUIRED)

add_executable(Wireframe ${SOURCE_FILES})

target_link_libraries(Wireframe Qt5::Widgets)