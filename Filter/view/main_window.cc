#include "main_window.h"

#include <QScrollArea>
#include <QHBoxLayout>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QtWidgets/QMessageBox>

#include "../factory.h"
#include "filter_action_group.h"
#include "../filter_action_group_creator.h"

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
    connect(zone_B, SIGNAL(image_changed(ImageWrapper)),
            this, SLOT(zone_B_not_empty()));
    connect(zone_C, SIGNAL(image_changed(ImageWrapper)),
            this, SLOT(zone_C_not_empty()));

    set_actions_enabled(false);
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
    scroll_area->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    setCentralWidget(scroll_area);
}

void MainWindow::create_menus_and_toolbar() {
    new_action = new QAction(tr("&New"), this);
    connect(new_action, &QAction::triggered,
            this, &MainWindow::clear);

    open_action = new QAction(tr("&Open"), this);
    connect(open_action, &QAction::triggered,
            this, &MainWindow::open);

    save_action = new QAction(tr("&Save"), this);
    connect(save_action, &QAction::triggered,
            this, &MainWindow::save);

    QMenu * file_menu = new QMenu(tr("&File"), this);
    file_menu->addAction(new_action);
    file_menu->addAction(open_action);
    file_menu->addAction(save_action);

    copy_C_to_B_action = new QAction(tr("Copy C to B"), this);
    connect(copy_C_to_B_action, &QAction::triggered,
            this, &MainWindow::copy_C_to_B);

    QMenu * edit_menu = new QMenu(tr("&Edit"), this);
    edit_menu->addAction(copy_C_to_B_action);

    QAction * about_action = new QAction(tr("About"), this);
    connect(about_action, &QAction::triggered,
            this, &MainWindow::show_about);

    QMenu * help_menu = new QMenu(tr("&Help"), this);
    help_menu->addAction(about_action);

    filter_action_group = FilterActionGroupCreator::instance().create_with_parent(this);
    connect(filter_action_group, &FilterActionGroup::apply_filter,
            this, &MainWindow::set_filter_widget);

    QMenu * filters_menu = new QMenu(tr("&Filters"), this);
    filters_menu->addActions(filter_action_group->actions());

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
    menuBar()->addMenu(filters_menu);

    QToolBar * common_toolbar = new QToolBar(this);
    common_toolbar->setMovable(false);
    common_toolbar->addAction(new_action);
    common_toolbar->addAction(open_action);
    common_toolbar->addAction(save_action);
    common_toolbar->addSeparator();
    common_toolbar->addAction(copy_C_to_B_action);


    QToolBar * filters_toolbar = new QToolBar(this);
    filters_toolbar->setMovable(false);
    filters_toolbar->addActions(filter_action_group->actions());

    addToolBar(common_toolbar);
    addToolBarBreak();
    addToolBar(filters_toolbar);
}

void MainWindow::set_filter_widget(QString filter_name) {
    std::unique_ptr<BaseFilterWidget> widget(
            Factory<std::string, BaseFilterWidget>::instance().create(filter_name.toStdString()));
    if (widget) {
        dock_widget->setWidget(widget.get());
        filter_widget = std::move(widget);
        dock_widget->setWindowTitle(filter_name);
        filter_widget->show();
        filter_widget->set_zones(zone_B, zone_C);

        connect(filter_widget.get(),
                SIGNAL(filtrate(std::shared_ptr<AbstractFilter>, ImageWrapper, uint64_t)),
                &FilterApplicator::instance(),
                SLOT(filtrate_image(std::shared_ptr<AbstractFilter>, ImageWrapper, uint64_t)));

        connect(&FilterApplicator::instance(), SIGNAL(filtration_finished(ImageWrapper, uint64_t)),
                filter_widget.get(), SLOT(filtration_finished(ImageWrapper, uint64_t)));

        connect(filter_widget.get(), SIGNAL(accepted()),
                this, SLOT(remove_filter_widget()));

        connect(filter_widget.get(), SIGNAL(cancelled()),
                this, SLOT(remove_filter_widget()));

        connect(zone_B, SIGNAL(image_changed(ImageWrapper)),
                filter_widget.get(), SLOT(apply_filter(ImageWrapper)));

        filter_widget->apply_filter();

        set_actions_enabled(false);
        new_action->setEnabled(false);
        open_action->setEnabled(false);
    }
}

void MainWindow::remove_filter_widget() {
    dock_widget->setWindowTitle("");
    dock_widget->setWidget(nullptr);
    filter_widget = nullptr;
    set_actions_enabled(true);
    filter_action_group->uncheck_all();
    new_action->setEnabled(true);
    open_action->setEnabled(true);
}

void MainWindow::set_actions_enabled(bool enabled) {
    filter_action_group->setEnabled(enabled);
    copy_C_to_B_action->setEnabled(enabled);
    save_action->setEnabled(enabled && has_something_to_save);
}

void MainWindow::clear() {
    zone_A->clear();
    zone_B->clear();
    zone_C->clear();
    has_something_to_save = false;

    zone_A->set_selection_enabled(false);
    set_actions_enabled(false);
}

void MainWindow::open() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open image"),
                                                    "./",
                                                    tr("Images (*.png *.bmp *.jpg);;All files(*.*)"));
    QImage image;
    if (!filename.isNull()) {
        if (image.load(filename)) {
            image = image.convertToFormat(QImage::Format_ARGB32);
            clear();
            zone_A->set_image(image);

            zone_A->set_selection_enabled(true);
        } else {
            QMessageBox::warning(this, tr("Cannot open file"),
                                 tr("File doesn't contain image."));
        }
    }
}

void MainWindow::save() {
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save image"),
                                                    "./",
                                                    tr("Images (*.png *.bmp *.jpg);;All files(*.*)"));
    if (!filename.isEmpty()) {
        const QImage & image_to_save = zone_C->get_image().to_QImage();
        if (!image_to_save.save(filename)) {
            QMessageBox::warning(this, tr("Cannot save file"),
                                 tr("Failed to save file."));
        }
    }
}

void MainWindow::show_about() {
    QMessageBox::about(this, tr("About FIT_14202_Grachev_Filter"),
                       tr("Filter Version 1.0, NSU FIT 14202 Grachev"));
}

void MainWindow::copy_C_to_B() {
    zone_B->set_image(zone_C->get_image());
}

void MainWindow::zone_B_not_empty() {
    if (!has_something_to_save) {
        filter_action_group->setEnabled(true);
    }
}

void MainWindow::zone_C_not_empty() {
    has_something_to_save = true;
}