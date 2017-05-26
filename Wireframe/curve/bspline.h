#ifndef BSPLINE_H
#define BSPLINE_H

#include <QMatrix3x3>
#include "curve.h"

class BSpline : public Curve {
public:
    BSpline(int major_segments_count, int minor_segments_count);

    ~BSpline() {}

    void add_control_point(const QPointF & point);

    const PointContainer & get_control_points() const { return control_points; }

private:
    void recalculate_discrete_curve();

    int get_knot_index(int int_t) const;

    QPointF calculate_point(double t) const;


    PointContainer control_points;

    static const int spline_order = 3;
};

#endif //BSPLINE_H
