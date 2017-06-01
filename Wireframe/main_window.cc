#include "main_window.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <iostream>
#include <QtWidgets/QHBoxLayout>
#include "figure/solid_of_revolution.h"
#include "util/scene_loader.h"

MainWindow::MainWindow() {
    create_central_widget();
    create_menu_and_toolbar();
    create_default_scene();

    QSize min_size(800, 600);
    setMinimumSize(min_size);
    setWindowTitle(tr("FIT_14202_Grachev_Wireframe"));
    resize(min_size);
}

void MainWindow::create_central_widget() {
    QWidget * central_widget = new QWidget(this);
    QHBoxLayout * central_layout = new QHBoxLayout(central_widget);
    setCentralWidget(central_widget);

    central_layout->addWidget(wireframe = new WireframeWidget(this));
    central_layout->addWidget(spline = new SplineWidget(this));
    central_layout->addWidget(properties = new PropertiesWidget(this));

    connect(properties, &PropertiesWidget::view_changed,
            wireframe, &WireframeWidget::update_view);
    connect(properties, &PropertiesWidget::camera_changed,
            wireframe, &WireframeWidget::set_camera);
    connect(properties, &PropertiesWidget::active_object_changed,
            wireframe, &WireframeWidget::set_active_object);
    connect(properties, &PropertiesWidget::active_spline_changed,
            spline, &SplineWidget::set_curve);
    connect(wireframe, &WireframeWidget::zoom,
            properties, &PropertiesWidget::zoom);
}

void MainWindow::create_menu_and_toolbar() {
    QAction * open_action = new QAction(tr("Open"), this);
    connect(open_action, &QAction::triggered,
            this, &MainWindow::open_file);

    QAction * save_action = new QAction(tr("Save"), this);
    connect(save_action, &QAction::triggered,
            this, &MainWindow::save_scene);

    QAction * reset_rotation = new QAction(tr("Reset rotation"), this);
    connect(reset_rotation, &QAction::triggered,
            this, &MainWindow::reset_scene_rotation);

    QAction * about_action = new QAction(tr("About"), this);
    connect(about_action, &QAction::triggered,
            this, &MainWindow::show_about);

    QActionGroup * mode_group = new QActionGroup(this);
    QAction * wireframe_mode = mode_group->addAction(tr("Wireframe"));
    connect(wireframe_mode, &QAction::triggered,
            this, &MainWindow::set_wireframe_mode);
    wireframe_mode->setCheckable(true);
    wireframe_mode->trigger();

    QAction * spline_mode = mode_group->addAction(tr("Spline"));
    connect(spline_mode, &QAction::triggered,
            this, &MainWindow::set_spline_mode);
    spline_mode->setCheckable(true);

    QMenu * file_menu = new QMenu(tr("File"), this);
    file_menu->addAction(open_action);
    file_menu->addAction(save_action);

    QMenu * edit_menu = new QMenu(tr("Edit"), this);
    edit_menu->addAction(reset_rotation);

    QMenu * mode_menu = new QMenu(tr("Mode"), this);
    mode_menu->addActions(mode_group->actions());

    QMenu * help_menu = new QMenu(tr("Help"), this);
    help_menu->addAction(about_action);

    QToolBar * tool_bar = new QToolBar(this);
    tool_bar->setMovable(false);
    tool_bar->addAction(open_action);
    tool_bar->addAction(save_action);
    tool_bar->addAction(reset_rotation);
    tool_bar->addSeparator();
    tool_bar->addActions(mode_group->actions());
    tool_bar->addSeparator();
    tool_bar->addAction(about_action);

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
    menuBar()->addMenu(mode_menu);
    menuBar()->addMenu(help_menu);
    addToolBar(tool_bar);
}

void MainWindow::create_default_scene() {
    QString initial_scene = "4 8 5 0 1 0 1\n"
            "0.1 5 0.16 0.12\n"
            "1 0 0\n"
            "0 1 0\n"
            "0 0 1\n"
            "180 180 180\n"
            "2 // Objects count\n"
            " // Object N 0\n"
            "0 0 0\n"
            "0 -0.25 1\n"
            "1 0 0 \n"
            "0 1 0 \n"
            "0 0 1 \n"
            "4 // Control points count\n"
            "0.3 0.3\n"
            "0.1 0.2\n"
            "0.4 0\n"
            "0.1 -0.3\n"
            " // Object N 1\n"
            "0 0 0\n"
            "1 0 -0.3\n"
            "1 0 0 \n"
            "0 1 0 \n"
            "0 0 1 \n"
            "7 // Control points count\n"
            "0.0 0.27\n"
            "0.15 0.25\n"
            "0.25 0.15\n"
            "0.3 0.0\n"
            "0.25 -0.15\n"
            "0.15 -0.25\n"
            "0.0 -0.27";

    QTextStream stream(&initial_scene, QIODevice::ReadOnly);
    scene_info = std::shared_ptr<SceneInfo>(SceneInfo::load(stream));

    properties->set_scene_info(scene_info.get());
}

void MainWindow::reset_scene_rotation() {
    scene_info->scene->set_rotation(QMatrix4x4());
    wireframe->update_view();
}

void MainWindow::set_wireframe_mode() {
    spline->setVisible(false);
    wireframe->setVisible(true);
}

void MainWindow::set_spline_mode() {
    spline->setVisible(true);
    wireframe->setVisible(false);
}

void MainWindow::show_about() {
    QMessageBox::about(this, tr("About FIT_14202_Grachev_Wireframe"),
                       tr("Wireframe Version 1.0, NSU FIT 14202 Grachev\n\n"
                                  "* To rotate single object use left mouse button.\n"
                                  "* To rotate whole scene use right mouse button."));
}

void MainWindow::save_scene() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "./",
                                                    "Scene(*.txt);;All files(*.*)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            scene_info->save(stream);
        }
    }
}

void MainWindow::open_file() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "./",
                                                    "Scene(*.txt);;All files(*.*)");
    if (!filename.isEmpty()) {
        QFile file(filename);
        try {
            if (file.open(QIODevice::ReadOnly)) {
                QTextStream stream(&file);

                std::shared_ptr<SceneInfo> info(SceneInfo::load(stream));
                properties->set_scene_info(info.get());
                scene_info = info;
            }
        } catch (const std::runtime_error & e) {
            QMessageBox::warning(this, tr("Cannot open file"),
                                 tr(e.what()));
        }
    }
}
