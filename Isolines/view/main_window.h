#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "color_map_widget.h"
#include "legend_widget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();

public slots:

    void open_file();

    void options();

    void about();

private:
    void create_central_widget();

    void create_actions();

    void create_status_bar();

    ColorMapWidget * color_map_widget;
    LegendWidget * legend;
};


#endif //MAIN_WINDOW_H
