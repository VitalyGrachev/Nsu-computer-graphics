#include "bounding_box.h"

#include <algorithm>

BoundingBox::BoundingBox()
        : valid(false) {
}

BoundingBox::BoundingBox(float min_x, float min_y, float min_z,
                         float max_x, float max_y, float max_z)
        : min_x(min_x), min_y(min_y), min_z(min_z),
          max_x(max_x), max_y(max_y), max_z(max_z), valid(true) {
}

BoundingBox & BoundingBox::include(const BoundingBox & box) {
    if (this != &box) {
        if (!valid) {
            operator=(box);
        } else {
            min_x = std::min(min_x, box.min_x);
            min_y = std::min(min_y, box.min_y);
            min_z = std::min(min_z, box.min_z);

            max_x = std::max(max_x, box.max_x);
            max_y = std::max(max_y, box.max_y);
            max_z = std::max(max_z, box.max_z);
            recalculate_center();
        }
    }
    return *this;
}

BoundingBox & BoundingBox::include(const QVector4D & point) {
    if (!valid) {
        min_x = point.x();
        min_y = point.y();
        min_z = point.z();

        max_x = point.x();
        max_y = point.y();
        max_z = point.z();
        valid = true;
    } else {
        min_x = std::min(min_x, point.x());
        min_y = std::min(min_y, point.y());
        min_z = std::min(min_z, point.z());

        max_x = std::max(max_x, point.x());
        max_y = std::max(max_y, point.y());
        max_z = std::max(max_z, point.z());
    }
    recalculate_center();
    return *this;
}

void BoundingBox::recalculate_center() {
    box_center = QVector3D((min_x + max_x) / 2,
                           (min_y + max_y) / 2,
                           (min_z + max_z) / 2);
}

void BoundingBox::clear() {
    valid = false;
}
