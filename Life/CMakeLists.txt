cmake_minimum_required(VERSION 3.6)
project(Life)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        model/life_field.h
        view/canvas.cpp
        view/canvas.h
        view/field_display.cpp
        view/field_display.h
        view/hex_grid_canvas.cpp
        view/hex_grid_canvas.h
        view/main_window.cpp
        view/main_window.h
        view/dialogs/new_field_dialog.h
        view/dialogs/new_field_dialog.cc
        main.cpp
        model/life_game_engine.cc
        model/life_game_engine.h
        model/life_field.cc
        model/life_impacts.cc
        model/life_impacts.h
        model/abstract_notifier.h
        view/signal_notifier.h view/mode.h view/dialogs/options_dialog.cc view/dialogs/options_dialog.h view/dialogs/field_size_group_box.cc view/dialogs/field_size_group_box.h view/dialogs/cell_size_group_box.cc view/dialogs/cell_size_group_box.h view/dialogs/rules_group_box.cc view/dialogs/rules_group_box.h)

find_package(Qt5Widgets REQUIRED)

add_executable(Life ${SOURCE_FILES})

target_link_libraries(Life Qt5::Widgets)