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

//    spline->setVisible(false);
//    wireframe->setVisible(true);

    connect(properties, &PropertiesWidget::view_changed,
            wireframe, &WireframeWidget::update_view);
    connect(properties, &PropertiesWidget::camera_changed,
            wireframe, &WireframeWidget::set_camera);
    connect(properties, &PropertiesWidget::active_object_changed,
            wireframe, &WireframeWidget::set_active_object);
    connect(properties, &PropertiesWidget::active_spline_changed,
            spline, &SplineWidget::set_curve);
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
    file_menu->addAction(save_action);
    file_menu->addAction(open_action);

    QMenu * edit_menu = new QMenu(tr("Edit"), this);
    edit_menu->addAction(reset_rotation);

    QMenu * mode_menu = new QMenu(tr("Mode"), this);
    mode_menu->addActions(mode_group->actions());

    QToolBar * tool_bar = new QToolBar(this);
    tool_bar->addAction(save_action);
    tool_bar->addAction(open_action);
    tool_bar->addAction(reset_rotation);
    tool_bar->addSeparator();
    tool_bar->addActions(mode_group->actions());

    menuBar()->addMenu(file_menu);
    menuBar()->addMenu(edit_menu);
    menuBar()->addMenu(mode_menu);
    addToolBar(tool_bar);
}

void MainWindow::create_default_scene() {
    scene_info = std::make_shared<SceneInfo>();
    scene_info->camera = new Camera();
    scene_info->scene = new Scene();

    scene_info->camera->set_scene(scene_info->scene);

    {
        BSpline * vase_spline = new BSpline(4, 5);
        vase_spline->add_control_point(QPointF(0.3, 0.3));
        vase_spline->add_control_point(QPointF(0.1, 0.2));
        vase_spline->add_control_point(QPointF(0.4, 0.0));
        vase_spline->add_control_point(QPointF(0.1, -0.3));
        scene_info->generatrices.emplace_back(vase_spline);

        SolidOfRevolution * vase = new SolidOfRevolution(vase_spline, 8);
        vase->set_position(QVector3D(0, 0, 1));
        scene_info->objects.emplace_back(vase);

        scene_info->scene->add_object(vase);
    }

    {
        BSpline * sphere_spline = new BSpline(7, 5);
        sphere_spline->add_control_point(QPointF(0.0, 1.0));
        sphere_spline->add_control_point(QPointF(0.707, 0.707));
        sphere_spline->add_control_point(QPointF(1.0, 0.0));
        sphere_spline->add_control_point(QPointF(0.707, -0.707));
        sphere_spline->add_control_point(QPointF(0.0, -1.0));
        scene_info->generatrices.emplace_back(sphere_spline);

        SolidOfRevolution * sphere = new SolidOfRevolution(sphere_spline, 8);
        sphere->set_position(QVector3D(1, 0, -0.3));
        sphere->set_scale(QVector3D(0.3, 0.3, 0.3));
        scene_info->objects.emplace_back(sphere);

        scene_info->scene->add_object(sphere);
    }

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

void MainWindow::save_scene() {

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
                SceneLoader loader = SceneLoader(stream);

                std::shared_ptr<SceneInfo> info = loader();
                properties->set_scene_info(scene_info.get());
                scene_info = info;
            }
        } catch (const std::runtime_error & e) {
            QMessageBox::warning(this, tr("Cannot open file"),
                                 tr(e.what()));
        }
    }
}
