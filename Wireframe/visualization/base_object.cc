#include "base_object.h"

#include <QColor>
#include "../util/matrix_utils.h"

BaseObject::BaseObject()
        : position(0.0, 0.0, 0.0),
          scale(1.0, 1.0, 1.0),
          color(QColor(0, 0, 0).rgb()) {
    recalculate_transform();
}

void BaseObject::set_position(const QVector3D & position) {
    this->position = position;
    recalculate_transform();
    recalculate_bounds();
}

void BaseObject::set_scale(const QVector3D & scale) {
    this->scale = scale;
    recalculate_transform();
    recalculate_bounds();
}

void BaseObject::set_rotation(const QMatrix4x4 & rotation) {
    this->rotation = rotation;
    recalculate_transform();
    recalculate_bounds();
}

void BaseObject::set_color(QRgb color) {
    this->color = color;
}

void BaseObject::recalculate_transform() {
    transform_matrix = Transform::translation(position) * rotation * Transform::scaling(scale);
}

void BaseObject::recalculate_bounds(const QVector4D & added_point) {
    bounds.include(get_transform() * added_point);
}

void BaseObject::recalculate_bounds() {
    bounds.clear();
    std::unique_ptr<SegmentProvider> segments(get_segment_provider());
    while (segments->has_next()) {
        Segment segment = segments->next();
        bounds.include(get_transform() * segment.point1);
        bounds.include(get_transform() * segment.point2);
    }
}