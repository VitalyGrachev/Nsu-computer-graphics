#include "properties_widget.h"

#include <QVBoxLayout>

PropertiesWidget::PropertiesWidget(QWidget * parent)
        : QWidget(parent) {
    create_layout();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void PropertiesWidget::create_layout() {
    QVBoxLayout * main_layout = new QVBoxLayout(this);

    main_layout->addWidget(camera_properties = new CameraPropertiesBox());
    connect(camera_properties, &CameraPropertiesBox::view_changed,
            this, &PropertiesWidget::view_changed);
    connect(this, &PropertiesWidget::zoom,
            camera_properties, &CameraPropertiesBox::change_zoom);

    main_layout->addWidget(object_properties = new ObjectPropertiesBox());
    connect(object_properties, &ObjectPropertiesBox::view_changed,
            this, &PropertiesWidget::view_changed);
    connect(object_properties, &ObjectPropertiesBox::view_changed,
            this, &PropertiesWidget::recalculate_scene_bounds);
    connect(object_properties, &ObjectPropertiesBox::active_object_changed,
            this, &PropertiesWidget::change_active_object);

    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    camera_properties->setSizePolicy(policy);
    object_properties->setSizePolicy(policy);

    main_layout->setAlignment(Qt::AlignTop);
}

void PropertiesWidget::set_scene_info(SceneInfo * scene_info) {
    this->scene_info = scene_info;
    if (scene_info) {
        emit camera_changed(scene_info->camera);
        camera_properties->set_camera(scene_info->camera);
        object_properties->set_objects(&scene_info->objects);
    } else {
        emit camera_changed(nullptr);
        camera_properties->set_camera(nullptr);
        object_properties->set_objects(nullptr);
    }
}

void PropertiesWidget::change_active_object(int object_index) {
    if (scene_info) {
        emit active_object_changed(scene_info->objects[object_index]);
        emit active_spline_changed(scene_info->generatrices[object_index]);
    } else {
        emit active_object_changed(nullptr);
        emit active_spline_changed(nullptr);
    }
}

void PropertiesWidget::recalculate_scene_bounds() {
    scene_info->scene->recalculate_bounding_box();
}
