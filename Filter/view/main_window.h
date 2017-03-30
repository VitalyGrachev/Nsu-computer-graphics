#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include "zone.h"
#include "zone_a.h"
#include "../filter_applicator.h"
#include "../filters/base_filter_widget.h"
#include "filter_action_group.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow(QWidget * parent = nullptr);

private slots:

    void remove_filter_widget();

    void set_filter_widget(QString filter_name);

    void clear();

    void open();

    void save();

    void copy_C_to_B();

    void show_about();

    void zone_B_not_empty();

    void zone_C_not_empty();

private:
    void create_dock_widget();

    void create_central_widget();

    void create_menus_and_toolbar();

    void set_actions_enabled(bool enabled = true);

    ZoneA * zone_A;
    Zone * zone_B;
    Zone * zone_C;

    QAction * new_action;
    QAction * open_action;
    QAction * save_action;
    QAction * copy_C_to_B_action;

    FilterActionGroup * filter_action_group;
    QDockWidget * dock_widget;
    std::unique_ptr<BaseFilterWidget> filter_widget;

    bool has_something_to_save;

    static const int zone_width;
    static const int zone_height;
    static const int zone_spacing;
};

#endif //MAIN_WINDOW_H
