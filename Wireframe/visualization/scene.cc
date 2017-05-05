#include "scene.h"

#include <algorithm>

Scene::Scene()
        : center_position(0.0, 0.0, 0.0),
          scale_factor(1.0f) {
    recalculate_transform();
}

Scene::~Scene() {
    for (BaseObject * object : objects) {
        delete object;
    }
}

void Scene::add_object(BaseObject * object) {
    objects.push_front(object);
    recalculate_bounding_box();
}

void Scene::remove_object(BaseObject * object) {
    auto found = std::find(std::begin(objects), std::end(objects), object);
    if(found != std::end(objects)) {
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
    this->scale_factor = scale_factor;
    recalculate_transform();
}

void Scene::recalculate_transform() {
    transform_matrix.setToIdentity();
    transform_matrix.translate(center_position);
    transform_matrix *= rotation;
    transform_matrix.scale(scale_factor);
}

void Scene::recalculate_bounding_box() {

}
