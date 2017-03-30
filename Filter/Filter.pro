QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FIT_14202_Grachev_Filter
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \ 
    filters/zoom/zoom_filter.cc \
    filters/zoom/zoom_widget.cc \
    filters/grayscale/grayscale_filter.cc \
    filters/grayscale/grayscale_widget.cc \
    filters/negative/negative_filter.cc \
    filters/negative/negative_widget.cc \
    filters/base_filter_widget.cc \
    view/filter_action.cc \
    view/filter_action_group.cc \
    view/main_window.cc \
    view/zone.cc \
    view/zone_a.cc \
    filter_action_group_creator.cc \
    filter_applicator.cc \
    filter_runner.cc \
    image_wrapper.cc \
    filters/blur/blur_filter.cc \
    filters/blur/blur_widget.cc \
    filters/gamma_correction/gamma_correction_filter.cc \
    filters/gamma_correction/gamma_correction_widget.cc \
    view/value_setter.cc

HEADERS  += \ 
    filters/zoom/zoom_filter.h \
    filters/zoom/zoom_widget.h \
    filters/convolution/convolution_filter.h \
    filters/convolution/square_matrix.h \
    filters/grayscale/grayscale_filter.h \
    filters/grayscale/grayscale_widget.h \
    filters/negative/negative_filter.h \
    filters/negative/negative_widget.h \
    filters/abstract_filter.h \
    filters/base_filter_widget.h \
    view/filter_action.h \
    view/filter_action_group.h \
    view/main_window.h \
    view/zone.h \
    view/zone_a.h \
    factory.h \
    filter_action_group_creator.h \
    filter_applicator.h \
    filter_runner.h \
    image_wrapper.h \
    filters/blur/blur_filter.h \
    filters/blur/blur_widget.h \
    filters/gamma_correction/gamma_correction_filter.h \
    filters/gamma_correction/gamma_correction_widget.h \
    view/value_setter.h

OTHER_FILES += \
    Filter.pro.user
