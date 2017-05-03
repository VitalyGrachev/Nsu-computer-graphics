#include "object.h"

Object::Object(QRgb color)
        : rotation_matrix(),
          position(0.0f, 0.0f, 0.0f),
          scale(1.0f, 1.0f, 1.0f),
          color(color) {
    recalc_world_matrix();
}

void Object::set_rotation(const QMatrix4x4 & rot_matrix) {
    rotation_matrix = rot_matrix;
    should_recalc_world = true;
}

void Object::set_position(const QVector3D & position) {
    this->position = position;
    should_recalc_world = true;
}

void Object::set_scale(const QVector3D & scale) {
    this->scale = scale;
    should_recalc_world = true;
}

void Object::set_color(QRgb color) {
    this->color = color;
}

const QMatrix4x4 & Object::get_world_matrix() {
    if (should_recalc_world) {
        recalc_world_matrix();
    }
    return world_transform_matrix;
}

void Object::recalc_world_matrix() {
    world_transform_matrix.setToIdentity();
    world_transform_matrix.scale(scale);
    world_transform_matrix = rotation_matrix * world_transform_matrix;
    world_transform_matrix.translate(position);
}

void Object::add_segment(const Segment & segment) {
    segments.push_back(segment);
}