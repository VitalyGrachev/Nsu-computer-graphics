#ifndef SEGMENT_H
#define SEGMENT_H

#include <QVector4D>

struct Segment {
    QVector4D point1;
    QVector4D point2;

    Segment() = default;

    Segment(const QVector4D & pt1, const QVector4D & pt2) : point1(pt1), point2(pt2) {}

    Segment(const QVector3D & pt1, const QVector3D & pt2) : point1(pt1, 1.0f), point2(pt2, 1.0f) {}

    Segment(const Segment &) = default;

    Segment(Segment &&) = default;

    ~Segment() = default;

    Segment & operator=(const Segment &) = default;

    Segment & operator=(Segment &&) = default;
};

#endif //SEGMENT_H
