#include <iostream>
#include "main_window.h"

#include "figure/plain_segment_object.h"
#include "figure/solid_of_revolution.h"

MainWindow::MainWindow() {
    create_scene();

    wireframe = new WireframeWidget(camera);
    setCentralWidget(wireframe);

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

    vase->set_color(QColor(0, 0, 0).rgb());
    vase->set_scale(QVector3D(0.05, 0.05, 0.05));
    vase->set_position(QVector3D(0.1, 0.0, -0.3));
    scene->add_object(vase);

    SolidOfRevolution * cube = new SolidOfRevolution(4);
    cube->add_point(QPointF(0.7, 0.5));
    cube->add_point(QPointF(0.7, -0.5));

    cube->set_color(QColor(0, 0, 200).rgb());
    cube->set_scale(QVector3D(0.3, 0.3, 0.3));
    scene->add_object(cube);

    SolidOfRevolution * pyramid = new SolidOfRevolution(3);
    pyramid->add_point(QPointF(0.0, 0.5));
    pyramid->add_point(QPointF(0.7, -0.5));

    QMatrix4x4 rot;
    rot.rotate(-45, 0, 0, 1);
    pyramid->set_rotation(rot);
    pyramid->set_color(QColor(0, 0, 0).rgb());
    pyramid->set_position(QVector3D(0.0, 0.0, 0.3));
    pyramid->set_scale(QVector3D(0.3, 0.3, 0.3));
    scene->add_object(pyramid);

    PlainSegmentObject * axis_x = new PlainSegmentObject(QColor(250, 0, 0).rgb());
    axis_x->add_segment(Segment(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));

    PlainSegmentObject * axis_y = new PlainSegmentObject(QColor(0, 250, 0).rgb());
    axis_y->add_segment(Segment(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));

    PlainSegmentObject * axis_z = new PlainSegmentObject(QColor(0, 0, 250).rgb());
    axis_z->add_segment(Segment(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

    axis_x->set_scale(QVector3D(0.5, 0.5, 0.5));
    axis_y->set_scale(QVector3D(0.5, 0.5, 0.5));
    axis_z->set_scale(QVector3D(0.5, 0.5, 0.5));
    scene->add_object(axis_x);
    scene->add_object(axis_y);
    scene->add_object(axis_z);


    QVector3D pos(10.0f, 6.0f, 3.0f);
    QVector3D center(0.0f, 0.0f, 0.0f);
    QVector3D sight_dir = (center - pos);
    QVector3D right = QVector3D::crossProduct(sight_dir, QVector3D(0.0f, 1.0f, 0.0f));
    QVector3D up = QVector3D::crossProduct(sight_dir, right);
    camera->set_scene(scene);
    camera->set_viewport(QSize(640, 480));
    camera->set_position(pos);
    camera->set_point_to_look(center);
    camera->set_up(up);
    camera->set_clip_planes(0.1f, 150.0f);
}
