#ifndef SEGMENT_H
#define SEGMENT_H

#include <QVector3D>

struct Segment {
    QVector3D point1;
    QVector3D point2;

    Segment() = default;

    Segment(const QVector3D & pt1, const QVector3D & pt2) : point1(pt1), point2(pt2) {}

    Segment(const Segment &) = default;

    Segment(Segment &&) = default;

    ~Segment() = default;

    Segment & operator=(const Segment &) = default;

    Segment & operator=(Segment &&) = default;
};

#endif //SEGMENT_H
