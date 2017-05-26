#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <iterator>
#include <memory>
#include <utility>
#include <QMatrix4x4>
#include <QRgb>
#include <QVector3D>
#include "segment.h"
#include "../util/bounding_box.h"

class BaseObject {
public:
    class SegmentProvider {
    public:
        SegmentProvider() = default;

        SegmentProvider(const SegmentProvider &) = delete;

        virtual ~SegmentProvider() = default;

        SegmentProvider & operator=(const SegmentProvider &) = delete;

        virtual bool has_next() const = 0;

        virtual Segment next() = 0;
    };

    BaseObject();

    BaseObject(const BaseObject &) = delete;

    BaseObject & operator=(const BaseObject &) = delete;

    virtual ~BaseObject() {}

    virtual SegmentProvider * get_segment_provider() const = 0;

    const QMatrix4x4 & get_transform() const { return transform_matrix; }

    const QMatrix4x4 & get_rotation() const { return rotation; };

    const QVector3D & get_position() const { return position; };

    const QVector3D & get_scale() const { return scale; };

    const BoundingBox & get_bounds() const { return bounds; }

    QRgb get_color() const { return color; }

    void set_position(const QVector3D & position);

    void set_scale(const QVector3D & scale);

    void set_rotation(const QMatrix4x4 & rotation);

    void set_color(QRgb color);

protected:
    void recalculate_bounds(const QVector4D & added_point);

    void recalculate_bounds();

private:
    BoundingBox bounds;

    void recalculate_transform();
    QMatrix4x4 transform_matrix;
    QMatrix4x4 rotation;
    QVector3D position;
    QVector3D scale;
    QRgb color;
};

#endif //BASE_OBJECT_H
