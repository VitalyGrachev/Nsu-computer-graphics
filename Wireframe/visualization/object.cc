#include "object.h"

void Object::set_position(const QVector3D & position) {
    this->position = position;
}

void Object::rotate(const QMatrix4x4 & rot_matrix) {
    rotation_matrix *= rot_matrix;
}

void Object::add_segment(const Segment & segment) {
    segments.push_back(segment);
    recalc_size_changes_by(segment);
}

void Object::recalc_size_changes_by(const Segment & segment) {

}