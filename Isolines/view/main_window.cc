#include "main_window.h"
#include "options_dialog.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QSize>
#include <QTextStream>

namespace {
static const std::vector<QRgb> default_colors({QColor(0, 0, 255).rgb(),
                                               QColor(0, 255, 255).rgb(),
                                               QColor(0, 255, 0).rgb(),
                                               QColor(255, 255, 0).rgb(),
                                               QColor(255, 127, 0).rgb(),
                                               QColor(255, 0, 0).rgb()});
static const QRectF default_domain(-15.0f, -15.0f, 25.0f, 25.0f);

static const QSize default_grid_size(25, 25);

static const FunctionToDraw function_to_draw1 = [](const QPointF & pt) -> float {
    return std::sin(pt.x()) + std::cos(pt.y());
};

static const FunctionToDraw function_to_draw2 = [](const QPointF & pt) -> float {
    return pt.x() * pt.x() + pt.y() * pt.y();
};

static const FunctionToDraw function_to_draw3 = [](const QPointF & pt) -> float {
    return (2 * pt.x() - 55.0f) * (pt.x() + 17.5f) +
           0.25 * std::sin(0.7 * pt.y()) * (pt.y() - 15.0f) * (pt.y() + 7.5f);
};

static const FunctionToDraw function_to_draw4 = [](const QPointF & pt) -> float {
    return 2 * pt.x() + pt.y();
};

}

MainWindow::MainWindow() {
    create_central_widget();
    create_actions();
    create_status_bar();

    setWindowTitle(tr("FIT_14202_Grachev_Isolines"));
    resize(800, 600);
}

void MainWindow::create_central_widget() {
    QWidget * central_widget = new QWidget(this);
    QVBoxLayout * layout = new QVBoxLayout(central_widget);

    layout->addWidget(
            color_map_widget = new ColorMapWidget(function_to_draw2, default_domain, default_grid_size,
                                                  default_colors));
    layout->addWidget(legend = new LegendWidget(default_colors, color_map_widget->get_isoline_level_provider()));

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
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "./",
                                                    "Parameters(*.txt);;All files(*.*)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        try {
            if (file.open(QIODevice::ReadOnly)) {
                bool ok = false;
                QTextStream input(&file);

                QString line = input.readLine();
                QStringList line_parts = line.split(' ');
                if (line.isNull()) {
                    throw std::runtime_error("Unexpected end of file.");
                }
                if (line_parts.size() < 2) {
                    throw std::runtime_error("Grid cols and/or rows number not stated");
                }
                uint32_t grid_cols = line_parts[0].toUInt(&ok);
                if (!ok || grid_cols < 2 || grid_cols > 100) {
                    throw std::runtime_error("Invalid value instead of grid cols number.");
                }
                uint32_t grid_rows = line_parts[1].toUInt(&ok);
                if (!ok || grid_rows < 2 || grid_rows > 100) {
                    throw std::runtime_error("Invalid value instead of grid rows number.");
                }
                if (line_parts.size() > 2 && !line_parts[2].startsWith("//")) {
                    throw std::runtime_error("Found garbage after rows and cols numbers.");
                }

                line = input.readLine();
                line_parts = line.split(' ');
                if (line.isNull()) {
                    throw std::runtime_error("Unexpected end of file.");
                }
                if (line_parts.size() < 1) {
                    throw std::runtime_error("Color count not stated");
                }
                uint32_t color_count = line_parts[0].toUInt(&ok);
                if (!ok || color_count < 2 || color_count > 10) {
                    throw std::runtime_error("Invalid value instead of color count.");
                }
                if (line_parts.size() > 1 && !line_parts[1].startsWith("//")) {
                    throw std::runtime_error("Found garbage after color count.");
                }

                std::vector<QRgb> colors(color_count);
                for (uint32_t i = 0; i < color_count; ++i) {
                    line = input.readLine();
                    line_parts = line.split(' ');
                    if (line.isNull()) {
                        throw std::runtime_error("Unexpected end of file.");
                    }
                    if (line_parts.size() < 3) {
                        throw std::runtime_error("Grid cols and/or rows number not stated");
                    }
                    uint32_t red = line_parts[0].toUInt(&ok);
                    if (!ok || red > 255) {
                        throw std::runtime_error("Invalid value instead of red color value.");
                    }
                    uint32_t green = line_parts[1].toUInt(&ok);
                    if (!ok || green > 255) {
                        throw std::runtime_error("Invalid value instead of green color value.");
                    }
                    uint32_t blue = line_parts[2].toUInt(&ok);
                    if (!ok || blue > 255) {
                        throw std::runtime_error("Invalid value instead of blue color value.");
                    }
                    if (line_parts.size() > 3 && !line_parts[3].startsWith("//")) {
                        throw std::runtime_error("Found garbage after color values.");
                    }

                    colors[i] = QColor(red, green, blue).rgb();
                }

                if (!input.atEnd()) {
                    throw std::runtime_error("Found garbage at end of file.");
                }

                color_map_widget->set_grid_size(QSize(grid_cols, grid_rows));
                color_map_widget->set_colors(colors);
                legend->set_colors(colors);
            }
        } catch (std::runtime_error & e) {
            QMessageBox::warning(this, tr("Error in file"),
                                 tr("File cannot be read due to error: ") + tr(e.what()));
        }
    }
}

void MainWindow::options() {
    QScopedPointer<OptionsDialog> dialog(new OptionsDialog(color_map_widget->get_domain(),
                                                           color_map_widget->get_grid_size(), this));
    connect(dialog.data(), &OptionsDialog::set_domain,
            color_map_widget, &ColorMapWidget::set_domain);
    connect(dialog.data(), &OptionsDialog::set_grid_size,
            color_map_widget, &ColorMapWidget::set_grid_size);
    connect(dialog.data(), &OptionsDialog::set_domain,
            legend, &LegendWidget::update_image);

    dialog->exec();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About FIT_14202_Grachev_Isolines"),
                       tr("Isolines Version 1.0, NSU FIT 14202 Grachev"));
}