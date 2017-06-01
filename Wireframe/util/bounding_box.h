#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <QtGui/QVector4D>
#include <QtGui/QVector3D>

class BoundingBox {
public:
    BoundingBox();

    BoundingBox(float min_x, float min_y, float min_z,
                float max_x, float max_y, float max_z);

    BoundingBox(const BoundingBox &) = default;

    ~BoundingBox() = default;

    BoundingBox & operator=(const BoundingBox &) = default;

    BoundingBox & include(const BoundingBox & box);

    BoundingBox & include(const QVector4D & point);

    void clear();

    const QVector3D & get_center() const { return box_center; }

    float get_width() const { return max_x - min_x; }

    float get_height() const { return max_y - min_y; }

    float get_length() const { return max_z - min_z; }

    float get_min_x() const { return min_x; }

    float get_min_y() const { return min_y; }

    float get_min_z() const { return min_z; }

    float get_max_x() const { return max_x; }

    float get_max_y() const { return max_y; }

    float get_max_z() const { return max_z; }

private:
    void recalculate_center();

    QVector3D box_center;
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
    bool valid;
};


#endif //BOUNDING_BOX_H
