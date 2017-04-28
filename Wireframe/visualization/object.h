#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <QRgb>
#include <QMatrix4x4>
#include "segment.h"

class Object {
public:
    using SegmentsContainer = std::vector<Segment>;

    const QVector3D & get_position() const { return position; }

    const QMatrix4x4 & get_rotation_matrix() const { return rotation_matrix; }

    QRgb get_color() const { return color; }

    float get_height() const { return height; }

    float get_radius() const { return radius; }

    SegmentsContainer & get_segments() { segments; }

    void set_position(const QVector3D & position);

    void rotate(const QMatrix4x4 & rot_matrix);

    void add_segment(const Segment & segment);

    template<class ForwardIterator>
    void add_segments(ForwardIterator begin, ForwardIterator end);

private:
    void recalc_size_changes_by(const Segment & segment);

    SegmentsContainer segments;
    QVector3D position;
    QMatrix4x4 rotation_matrix;
    float height;
    float radius;
    QRgb color;
};

template<class InputIterator>
void Object::add_segments(InputIterator first, InputIterator last) {
    while(first != last) {
        add_segment(*first);
        ++first;
    }
}

#endif //OBJECT_H
