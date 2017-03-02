#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include <QTimer>
#include <QScrollArea>
#include <memory>

#include "field_display.h"
#include "signal_notifier.h"

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

    void toggle_run();

    void clear_field();

private:
    void create_actions();

    void create_menus();

    void connect_all();

    void stop_timer();

    void start_timer();

    bool load_field_from_file(const QString & file_name);

    void save_field_to_file(const QString & file_name);

    std::unique_ptr<SignalNotifier> signal_notifier;
    std::unique_ptr<LifeGameEngine> game_engine;
    FieldDisplay * field_display;
    QScrollArea * field_scroll_area;

    QAction * new_field_action;
    QAction * open_field_action;
    QAction * save_field_action;
    QAction * exit_action;

    QAction * next_step_action;
    QAction * run_action;
    QAction * toggle_impacts_action;
    QAction * clear_field_action;
    QAction * set_replace_mode_action;
    QAction * set_xor_mode_action;
    QActionGroup * mode_action_group;

    QTimer * timer;

    bool is_running;

    static const uint32_t default_cols = 10;
    static const uint32_t default_rows = 10;
    static const uint32_t default_edge_size = 30;
    static const int timer_interval_msec = 500;
};

#endif // MAIN_WINDOW_H
