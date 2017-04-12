#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "color_map_widget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();

private:
    ColorMapWidget * color_map_widget;
};


#endif //MAIN_WINDOW_H
