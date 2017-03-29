#include "main_window.h"

#include <QScrollArea>
#include <QHBoxLayout>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include "../factory.h"

const int MainWindow::zone_width = 350;
const int MainWindow::zone_height = 350;
const int MainWindow::zone_spacing = 10;

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent) {
    create_dock_widget();
    create_central_widget();
    create_menus_and_toolbar();

    connect(zone_A, SIGNAL(selected_image(ImageWrapper)),
            zone_B, SLOT(set_image(ImageWrapper)));

    setWindowTitle(tr("FIT_14202_Grachev_Filter"));
    resize(1200, 600);
}

void MainWindow::create_dock_widget() {
    dock_widget = new QDockWidget(this);
    dock_widget->setFloating(false);
    dock_widget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, dock_widget);
}

void MainWindow::create_central_widget() {
    QScrollArea * scroll_area = new QScrollArea(this);
    QWidget * central_widget = new QWidget(this);
    QHBoxLayout * central_layout = new QHBoxLayout(central_widget);

    central_layout->addSpacing(zone_spacing);
    central_layout->addWidget(zone_A = new ZoneA(zone_width, zone_height));
    central_layout->addSpacing(zone_spacing);
    central_layout->addWidget(zone_B = new Zone(zone_width, zone_height));
    central_layout->addSpacing(zone_spacing);
    central_layout->addWidget(zone_C = new Zone(zone_width, zone_height));
    central_layout->addSpacing(zone_spacing);

    scroll_area->setWidget(central_widget);
    scroll_area->setAlignment(Qt::AlignCenter);

    setCentralWidget(scroll_area);
}

void MainWindow::create_menus_and_toolbar() {
    QAction * new_action = new QAction(tr("&New"), this);
    connect(new_action, SIGNAL(triggered()),
            this, SLOT(clear()));

    QAction * open_action = new QAction(tr("&Open"), this);
    connect(open_action, SIGNAL(triggered()),
            this, SLOT(open()));

    QAction * save_action = new QAction(tr("&Save"), this);
    connect(save_action, SIGNAL(triggered()),
            this, SLOT(save()));

    QMenu * file_menu = new QMenu(tr("&File"), this);
    file_menu->addAction(new_action);
    file_menu->addAction(open_action);
    file_menu->addAction(save_action);

    menuBar()->addMenu(file_menu);
}

void MainWindow::set_filter_widget(const QString & filter_name) {
    filter_widget = Factory<std::string, BaseFilterWidget>::instance().create(filter_name.toStdString());
    dock_widget->setWidget(filter_widget);
    dock_widget->setWindowTitle(filter_name);
}

void MainWindow::clear() {
    zone_A->clear();
    zone_B->clear();
    zone_C->clear();
    has_something_to_save = false;
    // TODO: disable filters
}

void MainWindow::open() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open image"),
                                                    "./",
                                                    tr("Images (*.png *.bmp *.jpg);;All files(*.*)"));
    QImage image;
    if (!filename.isNull() && image.load(filename)) {
        image.convertToFormat(QImage::Format_ARGB32);
        zone_A->set_image(image);
        zone_B->clear();
        zone_C->clear();

        zone_A->set_selection_enabled(true);
    } else {
        // TODO: show error message
    }
}

void MainWindow::save() {

}