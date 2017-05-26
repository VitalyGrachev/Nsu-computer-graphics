#include <iostream>
#include "main_window.h"

#include "figure/solid_of_revolution.h"
#include "curve/bspline.h"

MainWindow::MainWindow() {
//    create_scene();

//    wireframe = new WireframeWidget(camera);
//    setCentralWidget(wireframe);

    std::shared_ptr<BSpline> b_spline(new BSpline(10, 10));
    b_spline->add_control_point(QPointF(1.0, -1.0));
    b_spline->add_control_point(QPointF(2.0, -0.5));
    b_spline->add_control_point(QPointF(2.0, 0.5));
    b_spline->add_control_point(QPointF(1.0, 1.0));
    b_spline->add_control_point(QPointF(0.0, 1.0));

    spline = new SplineWidget();
    spline->set_curve(b_spline);

    setCentralWidget(spline);

    QSize min_size(800, 600);
    setMinimumSize(min_size);
    setWindowTitle(tr("FIT_14202_Grachev_Wireframe"));
    resize(min_size);
}

void MainWindow::create_scene() {
    camera = std::shared_ptr<Camera>(new Camera());

    std::shared_ptr<Scene> scene(new Scene());

    SolidOfRevolution * vase = new SolidOfRevolution(30);
    vase->add_point(QPointF(2.0, 3.0));
    vase->add_point(QPointF(1.0, 2.0));
    vase->add_point(QPointF(1.0, 1.0));
    vase->add_point(QPointF(2.0, 0.0));
    vase->add_point(QPointF(2.5, -1.5));
    vase->add_point(QPointF(2.0, -3.0));
    vase->add_point(QPointF(1.0, -3.5));

    vase->set_color(QColor(0, 150, 250).rgb());
    vase->set_scale(QVector3D(0.05, 0.05, 0.05));
    vase->set_position(QVector3D(0.1, 0.0, -0.3));
    scene->add_object(vase);

    SolidOfRevolution * cube = new SolidOfRevolution(4);
    cube->add_point(QPointF(0.7, 0.5));
    cube->add_point(QPointF(0.7, -0.5));

    cube->set_color(QColor(0, 150, 250).rgb());
    cube->set_scale(QVector3D(0.3, 0.3, 0.3));
    scene->add_object(cube);

    SolidOfRevolution * pyramid = new SolidOfRevolution(3);
    pyramid->add_point(QPointF(0.0, 0.5));
    pyramid->add_point(QPointF(0.7, -0.5));

    QMatrix4x4 rot;
    rot.rotate(-45, 0, 0, 1);
    pyramid->set_rotation(rot);
    pyramid->set_color(QColor(0, 150, 250).rgb());
    pyramid->set_position(QVector3D(0.0, 0.0, 0.3));
    pyramid->set_scale(QVector3D(0.3, 0.3, 0.3));
    scene->add_object(pyramid);

    QVector3D pos(2.0f, 1.8f, 0.5f);
    QVector3D center(0.0f, 0.0f, 0.0f);
    camera->set_scene(scene);
    camera->set_position(pos);
    camera->set_point_to_look(center);
}
