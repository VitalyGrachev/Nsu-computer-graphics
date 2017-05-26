#include "parallelepiped.h"

Parallelepiped::Parallelepiped(double width, double height, double length, QRgb color)
        : PlainSegmentObject(color),
          half_width(width / 2.0),
          half_height(height / 2.0),
          half_length(length / 2.0) {
    segments_container.resize(12);
    change_dimensions();
}

void Parallelepiped::change_dimensions() {
    const QVector3D pt1(half_width, half_height, half_length);
    const QVector3D pt2(half_width, -half_height, half_length);
    const QVector3D pt3(half_width, half_height, -half_length);
    const QVector3D pt4(half_width, -half_height, -half_length);

    segments_container[0] = (Segment(pt1, pt2));
    segments_container[1] = (Segment(pt2, pt4));
    segments_container[2] = (Segment(pt3, pt4));
    segments_container[3] = (Segment(pt1, pt3));

    segments_container[4] = (Segment(-pt1, -pt2));
    segments_container[5] = (Segment(-pt2, -pt4));
    segments_container[6] = (Segment(-pt3, -pt4));
    segments_container[7] = (Segment(-pt1, -pt3));

    segments_container[8] = (Segment(pt1, -pt4));
    segments_container[9] = (Segment(pt2, -pt3));
    segments_container[10] = (Segment(pt3, -pt2));
    segments_container[11] = (Segment(pt4, -pt1));

    recalculate_bounds();
}

void Parallelepiped::set_width(double width) {
    half_width = width / 2.0;
    change_dimensions();
}

void Parallelepiped::set_height(double height) {
    half_height = height / 2.0;
    change_dimensions();
}

void Parallelepiped::set_length(double length) {
    half_length = length / 2.0;
    change_dimensions();
}

void Parallelepiped::set_dimensions(double width, double height, double length) {
    half_width = width / 2.0;
    half_height = height / 2.0;
    half_length = length / 2.0;
    change_dimensions();
}
