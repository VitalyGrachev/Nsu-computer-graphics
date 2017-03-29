#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include "zone.h"
#include "zone_a.h"
#include "../filter_applicator.h"
#include "../filters/base_filter_widget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget * parent = nullptr);

private slots:

    void set_filter_widget(const QString & filter_name);

    void clear();

    void open();

    void save();

private:
    void create_dock_widget();

    void create_central_widget();

    void create_menus_and_toolbar();

    ZoneA * zone_A;
    Zone * zone_B;
    Zone * zone_C;

    QDockWidget * dock_widget;

    BaseFilterWidget * filter_widget;

    FilterApplicator filter_applicator;

    bool has_something_to_save;

    static const int zone_width;
    static const int zone_height;
    static const int zone_spacing;
};

#endif //MAIN_WINDOW_H
