#include "camera_properties_box.h"

#include <QLabel>
#include <QGridLayout>

CameraPropertiesBox::CameraPropertiesBox(QWidget * parent)
        : QGroupBox(QObject::tr("Camera properties"), parent) {
    create_layout();
}

void CameraPropertiesBox::create_layout() {
    QGridLayout * main_layout = new QGridLayout(this);

    main_layout->addWidget(new QLabel(QObject::tr("Zn")), 0, 0);
    main_layout->addWidget(near_clip_plane = new QDoubleSpinBox(), 0, 1);
    near_clip_plane->setMinimum(0.01);
    near_clip_plane->setMaximum(100.0);
    near_clip_plane->setEnabled(false);
    near_clip_plane->setSingleStep(0.01);

    main_layout->addWidget(new QLabel(QObject::tr("Zf")), 1, 0);
    main_layout->addWidget(far_clip_plane = new QDoubleSpinBox(), 1, 1);
    far_clip_plane->setMinimum(0.02);
    far_clip_plane->setMaximum(200.0);
    far_clip_plane->setEnabled(false);
    far_clip_plane->setSingleStep(0.01);

    main_layout->addWidget(new QLabel(QObject::tr("Sw")), 2, 0);
    main_layout->addWidget(viewport_width = new QDoubleSpinBox(), 2, 1);
    viewport_width->setMinimum(0.01);
    viewport_width->setMaximum(10.0);
    viewport_width->setEnabled(false);
    viewport_width->setSingleStep(0.01);

    main_layout->addWidget(new QLabel(QObject::tr("Sh")), 3, 0);
    main_layout->addWidget(viewport_height = new QDoubleSpinBox(), 3, 1);
    viewport_height->setMinimum(0.01);
    viewport_height->setMaximum(10.0);
    viewport_height->setEnabled(false);
    viewport_height->setSingleStep(0.01);

    connect(near_clip_plane, SIGNAL(valueChanged(double)),
            this, SLOT(change_clip_planes()));
    connect(far_clip_plane, SIGNAL(valueChanged(double)),
            this, SLOT(change_clip_planes()));

    connect(viewport_width, SIGNAL(valueChanged(double)),
            this, SLOT(change_viewport()));
    connect(viewport_height, SIGNAL(valueChanged(double)),
            this, SLOT(change_viewport()));
}

void CameraPropertiesBox::set_camera(Camera * camera) {
    this->camera = camera;
    near_clip_plane->setMaximum(100.0);
    far_clip_plane->setMinimum(0.02);

    ignore_changes = true;
    if (camera) {
        near_clip_plane->setValue(camera->get_near_clip_plane());
        far_clip_plane->setValue(camera->get_far_clip_lane());
        viewport_width->setValue(camera->get_viewport().width());
        viewport_height->setValue(camera->get_viewport().height());

        near_clip_plane->setEnabled(true);
        far_clip_plane->setEnabled(true);
        viewport_width->setEnabled(true);
        viewport_height->setEnabled(true);
    } else {
        near_clip_plane->setEnabled(false);
        far_clip_plane->setEnabled(false);
        viewport_width->setEnabled(false);
        viewport_height->setEnabled(false);
    }
    ignore_changes = false;
}

void CameraPropertiesBox::change_clip_planes() {
    if (!ignore_changes) {
        if (near_clip_plane == sender()) {
            far_clip_plane->setMinimum(near_clip_plane->value() - 0.01);
        } else {
            near_clip_plane->setMaximum(far_clip_plane->value() + 0.01);
        }

        if (camera) {
            camera->set_clip_planes(near_clip_plane->value(), far_clip_plane->value());
            emit view_changed();
        }
    }
}

void CameraPropertiesBox::change_viewport() {
    if (!ignore_changes) {
        if (camera) {
            camera->set_viewport(QSizeF(viewport_width->value(), viewport_height->value()));
            emit view_changed();
        }
    }
}
