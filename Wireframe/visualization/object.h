#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <QRgb>
#include <QMatrix4x4>
#include "segment.h"

class Object {
public:
    using SegmentsContainer = std::vector<Segment>;

    Object(QRgb color);

    ~Object() = default;

    const QMatrix4x4 & get_world_matrix();

    const QMatrix4x4 & get_rotation_matrix() const { return rotation_matrix; }

    const QVector3D & get_position() const { return position; }

    const QVector3D & get_scale() const { return scale; }

    QRgb get_color() const { return color; }

    void set_rotation(const QMatrix4x4 & rot_matrix);

    void set_position(const QVector3D & position);

    void set_scale(const QVector3D & scale);

    void set_color(QRgb color);

    SegmentsContainer & get_segments() { return segments; }

    void add_segment(const Segment & segment);

private:
    void recalc_world_matrix();

    SegmentsContainer segments;
    QMatrix4x4 world_transform_matrix;
    QMatrix4x4 rotation_matrix;
    QVector3D position;
    QVector3D scale;
    QRgb color;
    bool should_recalc_world = true;
};

#endif //OBJECT_H
