#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <QtCore/QPointF>

class Curve {
public:
    using PointContainer = std::vector<QPointF>;

    Curve(int major_segments_count, int minor_segments_count);

    int major_segments_count() const { return major_segments; }

    int minor_segments_count() const { return minor_segments; }

    const PointContainer & get_discrete_curve_points() const { return discrete_curve; }

protected:
    PointContainer discrete_curve;

private:
    int major_segments;
    int minor_segments;
};

#endif //CURVE_H
