#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QAction>
#include <QTimer>
#include <memory>

#include "field_display.h"
#include "signal_notifier.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget * parent = 0);

private slots:
    void new_field();
    void open_field();
    void save_field();
    void exit();

    void next_step();
    void run();
    void clear_field();

private:
    void create_actions();
    void create_menus();
    void connect_all();
    bool open_field_file(const QString & filename);

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

    QTimer * timer;

    static const uint32_t default_cols = 10;
    static const uint32_t default_rows = 10;
    static const uint32_t default_edge_size = 70;
};

#endif // MAIN_WINDOW_H
