#include "scene.h"
#include "../util/matrix_utils.h"

#include <algorithm>
#include <QColor>

Scene::Scene()
        : box_object(new Parallelepiped(1.0, 1.0, 1.0)),
          doesnt_affect_bounds(1),
          center_position(0.0, 0.0, 0.0),
          scale(1.0, 1.0, 1.0) {
    doesnt_affect_bounds.insert(static_cast<BaseObject*>(box_object));

    add_object(box_object);

    recalculate_transform();
}

Scene::~Scene() {
    for (BaseObject * object : objects) {
        delete object;
    }
}

void Scene::add_object(BaseObject * object) {
    objects.push_front(object);
    recalculate_bounding_box(object);
}

void Scene::remove_object(BaseObject * object) {
    auto found = std::find(std::begin(objects), std::end(objects), object);
    if (found != std::end(objects)) {
        objects.erase(found);
        delete object;
        recalculate_bounding_box();
    }
}

void Scene::set_position(const QVector3D & center_position) {
    this->center_position = center_position;
    recalculate_transform();
}

void Scene::set_rotation(const QMatrix4x4 & rotation) {
    this->rotation = rotation;
    recalculate_transform();
}

void Scene::set_scale(float scale_factor) {
    scale.setX(scale_factor);
    scale.setY(scale_factor);
    scale.setZ(scale_factor);
    recalculate_transform();
}

void Scene::recalculate_transform() {
    transform_matrix = rotation * Transform::scaling(scale) * Transform::translation(center_position);
}

void Scene::recalculate_bounding_box(BaseObject * object) {
    if (!doesnt_affect_bounds.count(object)) {
        bounds.include(object->get_bounds());

        box_object->set_dimensions(bounds.get_width(), bounds.get_height(), bounds.get_length());
        box_object->set_position(bounds.get_center());

        const double max_dimension = std::max(bounds.get_width(),
                                              std::max(bounds.get_height(), bounds.get_length()));
        const double scale = 2.0 / max_dimension;
        set_position(-bounds.get_center());
        set_scale(scale);
    }
}

void Scene::recalculate_bounding_box() {
    bounds = BoundingBox();
    for (BaseObject * object : objects) {
        recalculate_bounding_box(object);
    }
}
