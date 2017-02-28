#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QAction>
#include <QTimer>

#include "field_display.h"

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

    LifeGameEngine * game_engine;
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
};

#endif // MAIN_WINDOW_H
