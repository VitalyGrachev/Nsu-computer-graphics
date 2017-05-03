#include "main_window.h"

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

//    Object * square = new Object(QColor(0, 0, 0).rgb());
//    square->add_segment(Segment(QVector3D(0.5f, 0.0f, 0.5f), QVector3D(-0.5f, 0.0f, 0.5f)));
//    square->add_segment(Segment(QVector3D(0.5f, 0.0f, 0.5f), QVector3D(0.5f, 0.0f, -0.5f)));
//    square->add_segment(Segment(QVector3D(-0.5f, 0.0f, -0.5f), QVector3D(-0.5f, 0.0f, 0.5f)));
//    square->add_segment(Segment(QVector3D(-0.5f, 0.0f, -0.5f), QVector3D(0.5f, 0.0f, -0.5f)));

    Object * cube = new Object(QColor(120, 120, 0).rgb());
    cube->add_segment(Segment(QVector3D(0.5f, -0.5f, 0.5f), QVector3D(-0.5f, -0.5f, 0.5f)));
    cube->add_segment(Segment(QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.5f, -0.5f, -0.5f)));
    cube->add_segment(Segment(QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(-0.5f, -0.5f, 0.5f)));
    cube->add_segment(Segment(QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(0.5f, -0.5f, -0.5f)));
    cube->add_segment(Segment(QVector3D(0.5f, 0.5f, 0.5f), QVector3D(-0.5f, 0.5f, 0.5f)));
    cube->add_segment(Segment(QVector3D(0.5f, 0.5f, 0.5f), QVector3D(0.5f, 0.5f, -0.5f)));
    cube->add_segment(Segment(QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(-0.5f, 0.5f, 0.5f)));
    cube->add_segment(Segment(QVector3D(-0.5f, 0.5f, -0.5f), QVector3D(0.5f, 0.5f, -0.5f)));
    cube->add_segment(Segment(QVector3D(0.5f, -0.5f, 0.5f), QVector3D(0.5f, 0.5f, 0.5f)));
    cube->add_segment(Segment(QVector3D(-0.5f, -0.5f, 0.5f), QVector3D(-0.5f, 0.5f, 0.5f)));
    cube->add_segment(Segment(QVector3D(0.5f, -0.5f, -0.5f), QVector3D(0.5f, 0.5f, -0.5f)));
    cube->add_segment(Segment(QVector3D(-0.5f, -0.5f, -0.5f), QVector3D(-0.5f, 0.5f, -0.5f)));


    Object * pyramid1 = new Object(QColor(0, 0, 0).rgb());
    pyramid1->add_segment(Segment(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, 1.0f)));
    pyramid1->add_segment(Segment(QVector3D(1.0f, 0.0f, 0.0f), QVector3D(-1.0f, 0.0f, -1.0f)));
    pyramid1->add_segment(Segment(QVector3D(-1.0f, 0.0f, 1.0f), QVector3D(-1.0f, 0.0f, -1.0f)));

    Object * pyramid2 = new Object(QColor(0, 0, 0).rgb());
    pyramid2->add_segment(Segment(QVector3D(0.0f, 1.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    pyramid2->add_segment(Segment(QVector3D(0.0f, 1.0f, 0.0f), QVector3D(-1.0f, 0.0f, 1.0f)));
    pyramid2->add_segment(Segment(QVector3D(0.0f, 1.0f, 0.0f), QVector3D(-1.0f, 0.0f, -1.0f)));

    Object * axis_x = new Object(QColor(250, 0, 0).rgb());
    axis_x->add_segment(Segment(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(1.0f, 0.0f, 0.0f)));
    Object * axis_y = new Object(QColor(0, 250, 0).rgb());
    axis_y->add_segment(Segment(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f)));
    Object * axis_z = new Object(QColor(0, 0, 250).rgb());
    axis_z->add_segment(Segment(QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f)));

    pyramid1->set_scale(QVector3D(0.3, 0.3, 0.3));
    pyramid2->set_scale(QVector3D(0.3, 0.3, 0.3));
    cube->set_scale(QVector3D(0.3, 0.3, 0.3));
    QMatrix4x4 rot;
    rot.rotate(30, 0, 1, 0);
    cube->set_rotation(rot);
    cube->set_position(QVector3D(0, 0.5, 0));

    scene->add_object(axis_x);
    scene->add_object(axis_y);
    scene->add_object(axis_z);
    scene->add_object(cube);
    scene->add_object(pyramid1);
    scene->add_object(pyramid2);

    QVector3D pos(10.0f, 6.0f, 3.0f);
    QVector3D center(0.0f, 0.0f, 0.0f);
    QVector3D sight_dir = (center - pos);
    QVector3D right = QVector3D::crossProduct(sight_dir, QVector3D(0.0f, 1.0f, 0.0f));
    QVector3D up = QVector3D::crossProduct(sight_dir, right);
    up.normalize();
    camera->set_scene(scene);
    camera->set_viewport(QSize(640, 480));
    camera->set_position(pos);
    camera->set_point_to_look(center);
    camera->set_up(up);
    camera->set_clip_planes(0.1f, 150.0f);
}
