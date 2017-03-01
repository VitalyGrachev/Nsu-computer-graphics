#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T21:02:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FIT_14202_Grachev_Life
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    view/field_display.cpp \
    view/main_window.cpp \
    view/canvas.cpp \
    view/hex_grid_canvas.cpp \
    model/life_field.cc \
    model/life_game_engine.cc \
    model/life_impacts.cc

HEADERS  += mainwindow.h \
    view/field_display.h \
    view/main_window.h \
    view/canvas.h \
    model/life_field.h \
    view/hex_grid_canvas.h \
    model/life_game_engine.h \
    view/signal_notifier.h \
    model/abstract_notifier.h \
    model/life_impacts.h \
    view/mode.h
