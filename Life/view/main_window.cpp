#include "main_window.h"

#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include "signal_notifier.h"

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent) {

    SignalNotifier * notifier = new SignalNotifier();
    game_engine = new LifeGameEngine(10, 10, std::unique_ptr<AbstractNotifier>(notifier));

    field_display = new FieldDisplay(game_engine, 40);

    connect(notifier, SIGNAL(notification()),
            field_display, SLOT(model_changed()));

    field_scroll_area = new QScrollArea();
    field_scroll_area->setVisible(true);
    field_scroll_area->setWidgetResizable(true);
    field_scroll_area->setWidget(field_display);

    setCentralWidget(field_scroll_area);

    create_actions();
    create_menus();

    setWindowTitle(tr("Life"));
    resize(500, 500);
}

void MainWindow::create_actions() {
    new_field_action = new QAction(tr("&New"), this);
    connect(new_field_action, SIGNAL(triggered()),
            this, SLOT(new_field()));

    open_field_action = new QAction(tr("&Open"), this);
    connect(open_field_action, SIGNAL(triggered()),
            this, SLOT(open_field()));

    save_field_action = new QAction(tr("&Save"), this);
    connect(save_field_action, SIGNAL(triggered()),
            this, SLOT(save_field()));

    exit_action = new QAction(tr("E&xit"), this);
    connect(exit_action, SIGNAL(triggered()),
            this, SLOT(exit()));

    next_step_action = new QAction(tr("Next"), this);
    connect(next_step_action, SIGNAL(triggered()),
            this, SLOT(next_step()));

    run_action = new QAction(tr("&Run"), this);
    connect(run_action, SIGNAL(triggered()),
            this, SLOT(run()));

    toggle_impacts_action = new QAction(tr("Impact"), this);
    connect(toggle_impacts_action, SIGNAL(triggered()),
            field_display, SLOT(toggle_impacts()));

    clear_field_action = new QAction(tr("Clear"), this);
    connect(clear_field_action, SIGNAL(triggered()),
            this, SLOT(clear_field()));
}

void MainWindow::create_menus() {
    QMenu * file_menu = new QMenu(tr("&File"));
    file_menu->addAction(new_field_action);
    file_menu->addAction(open_field_action);
    file_menu->addAction(save_field_action);
    file_menu->addAction(exit_action);

    QMenu * edit_menu = new QMenu(tr("&Edit"));
    edit_menu->addAction(next_step_action);
    edit_menu->addAction(run_action);
    edit_menu->addAction(toggle_impacts_action);
    edit_menu->addAction(clear_field_action);

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
}

void MainWindow::new_field() {}

void MainWindow::open_field() {
}

void MainWindow::save_field() {}

void MainWindow::exit() {}

void MainWindow::next_step() {
    game_engine->next_step();
}

void MainWindow::run() {}

void MainWindow::clear_field() {
    game_engine->clear();
}
