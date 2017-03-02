#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QDialog>
#include <QMainWindow>
#include <QTimer>
#include <QToolBar>
#include <QScrollArea>
#include <memory>

#include "field_display.h"
#include "signal_notifier.h"
#include "new_field_dialog.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget * parent = 0);

private slots:

    void new_field();

    void open_field();

    void save_field();

    void exit();

    void next_step();

    void set_cell(uint32_t col, uint32_t row, CellState state);

    void toggle_run();

    void clear_field();

    void show_options();

    void show_about();

    void create_new_field(int cols, int rows, int cell_edge);

private:
    void create_actions();

    void create_menus();

    void create_toolbar();

    void create_new_field_dialog();

    void create_options_dialog();

    void connect_all();

    void stop_timer();

    void start_timer();

    bool load_field_from_file(const QString & file_name);

    void save_field_to_file(const QString & file_name);

    std::unique_ptr<SignalNotifier> signal_notifier;
    std::unique_ptr<LifeGameEngine> game_engine;
    FieldDisplay * field_display;
    QScrollArea * field_scroll_area;

    QToolBar * toolbar;

    QAction * new_field_action;
    QAction * open_field_action;
    QAction * save_field_action;
    QAction * exit_action;
    QAction * next_step_action;
    QAction * run_action;
    QAction * toggle_impacts_action;
    QAction * clear_field_action;
    QAction * set_options_action;
    QAction * set_replace_mode_action;
    QAction * set_xor_mode_action;
    QAction * show_about_action;

    QActionGroup * mode_action_group;

    QMenu * file_menu;
    QMenu * edit_menu;
    QMenu * view_menu;
    QMenu * action_menu;
    QMenu * help_menu;

    QTimer * timer;

    NewFieldDialog * new_field_dialog;
    QDialog * options_dialog;

    bool is_running;

    static const uint32_t default_cols = 16;
    static const uint32_t default_rows = 16;
    static const uint32_t default_edge_size = 20;
    static const uint32_t min_cols = 2;
    static const uint32_t min_rows = 2;
    static const uint32_t max_cols = 150;
    static const uint32_t max_rows = 150;
    static const uint32_t min_edge_size = 3;
    static const uint32_t max_edge_size = 100;
    static const int timer_interval_msec = 500;
};

#endif // MAIN_WINDOW_H
