#include "main_window.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QtWidgets/QVBoxLayout>

namespace {
static const std::vector<QRgb> default_colors({QColor(0, 0, 255).rgb(),
                                               QColor(0, 255, 255).rgb(),
                                               QColor(0, 255, 0).rgb(),
                                               QColor(255, 255, 0).rgb(),
                                               QColor(255, 127, 0).rgb(),
                                               QColor(255, 0, 0).rgb()});
}

MainWindow::MainWindow() {
    create_central_widget();
    create_actions();
    create_status_bar();

    setWindowTitle(tr("FIT_14202_Grachev_Isolines"));
    resize(800, 600);
}

void MainWindow::create_central_widget() {
//    FunctionToDraw function_to_draw = [](const QPointF & pt) -> float { return pt.x() + pt.y(); };
    FunctionToDraw function_to_draw = [](const QPointF & pt) -> float {
        return (2 * pt.x() - 55.0f) * (pt.x() + 17.5f) +
               0.25 * std::sin(0.7 * pt.y()) * (pt.y() - 15.0f) * (pt.y() + 7.5f);
    };
    QRectF domain(-55.0f, -55.0f, 125.0f, 125.0f);
    QSize grid_size(50, 50);

    QWidget * central_widget = new QWidget(this);
    QVBoxLayout * layout = new QVBoxLayout(central_widget);

    layout->addWidget(color_map_widget = new ColorMapWidget(function_to_draw, domain, grid_size, default_colors));
    layout->addWidget(legend = new LegendWidget(default_colors));

    layout->setSpacing(10);

    setCentralWidget(central_widget);
}

void MainWindow::create_actions() {
    QAction * open_file_action = new QAction(tr("&Open"), this);
    connect(open_file_action, &QAction::triggered,
            this, &MainWindow::open_file);

    QAction * options_action = new QAction(tr("Options"), this);
    connect(options_action, &QAction::triggered,
            this, &MainWindow::options);

    QAction * about_action = new QAction(tr("About"), this);
    connect(about_action, &QAction::triggered,
            this, &MainWindow::about);

    QAction * interpolate_action = new QAction(tr("Interpolate"), this);
    interpolate_action->setCheckable(true);
    connect(interpolate_action, &QAction::triggered,
            color_map_widget, &ColorMapWidget::set_interpolate_colors);
    connect(interpolate_action, &QAction::triggered,
            legend, &LegendWidget::set_interpolate_colors);


    QAction * show_grid_action = new QAction(tr("Show grid"), this);
    show_grid_action->setCheckable(true);
    connect(show_grid_action, &QAction::triggered,
            color_map_widget, &ColorMapWidget::set_draw_grid);

    QAction * show_isolines_action = new QAction(tr("Show isolines"), this);
    show_isolines_action->setCheckable(true);
    connect(show_isolines_action, &QAction::triggered,
            color_map_widget, &ColorMapWidget::set_show_isolines);

    QMenu * file_menu = new QMenu(tr("&File"), this);
    file_menu->addAction(open_file_action);

    QMenu * edit_menu = new QMenu(tr("&Edit"), this);
    edit_menu->addAction(interpolate_action);
    edit_menu->addAction(show_isolines_action);
    edit_menu->addAction(show_grid_action);

    QMenu * options_menu = new QMenu(tr("&Options"), this);
    options_menu->addAction(options_action);

    QMenu * help_menu = new QMenu(tr("&Help"), this);
    help_menu->addAction(about_action);

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
    menuBar()->addMenu(options_menu);
    menuBar()->addMenu(help_menu);

    QToolBar * tool_bar = new QToolBar(this);
    tool_bar->addAction(open_file_action);
    tool_bar->addAction(options_action);
    tool_bar->addAction(interpolate_action);
    tool_bar->addAction(show_grid_action);
    tool_bar->addAction(show_isolines_action);
    tool_bar->addAction(about_action);

    tool_bar->setMovable(false);
    addToolBar(tool_bar);
}

void MainWindow::create_status_bar() {
    QStatusBar * status_bar = new QStatusBar(this);
    connect(color_map_widget, &ColorMapWidget::show_status,
            status_bar, &QStatusBar::showMessage);
    connect(color_map_widget, &ColorMapWidget::clear_status,
            status_bar, &QStatusBar::clearMessage);
    setStatusBar(status_bar);
}

void MainWindow::open_file() {

}

void MainWindow::options() {

}

void MainWindow::about() {
    QMessageBox::about(this, tr("About FIT_14202_Grachev_Isolines"),
                       tr("Isolines Version 1.0, NSU FIT 14202 Grachev"));
}