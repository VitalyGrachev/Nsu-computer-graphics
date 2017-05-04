#include "base_object.h"

#include <QColor>

BaseObject::BaseObject()
        : position(0.0, 0.0, 0.0),
          scale(1.0, 1.0, 1.0),
          color(QColor(0, 0, 0).rgb()) {
    recalculate_transform();
}

void BaseObject::set_position(const QVector3D & position) {
    this->position = position;
    recalculate_transform();
}

void BaseObject::set_scale(const QVector3D & scale) {
    this->scale = scale;
    recalculate_transform();
}

void BaseObject::set_rotation(const QMatrix4x4 & rotation) {
    this->rotation = rotation;
    recalculate_transform();
}

void BaseObject::set_color(QRgb color) {
    this->color = color;
}

void BaseObject::recalculate_transform() {
    transform_matrix.setToIdentity();
    transform_matrix.translate(position);
    transform_matrix *= rotation;
    transform_matrix.scale(scale);
}