#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "zone.h"
#include "../filter_applicator.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget * parent = nullptr);

private:
    Zone * zone_A;
    Zone * zone_B;
    Zone * zone_C;

    FilterApplicator filter_applicator;

    static const int zone_width = 350;
    static const int zone_height = 350;
    static const int zone_spacing = 10;
};

#endif //MAIN_WINDOW_H
