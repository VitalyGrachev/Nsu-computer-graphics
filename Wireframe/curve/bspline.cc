#include "bspline.h"

BSpline::BSpline(int major_segments_count, int minor_segments_count)
        : Curve(major_segments_count, minor_segments_count) {}

void BSpline::add_control_point(const QPointF & point) {
    control_points.push_back(point);
    recalculate_discrete_curve();
}

void BSpline::recalculate_discrete_curve() {
    const int point_count = major_segments_count() * minor_segments_count() + 1;
    const int first_knot_idx = 0;
    const int last_knot_idx = control_points.size() - 2;
    const double step_t = static_cast<double>(last_knot_idx - first_knot_idx) / (point_count-1);
    discrete_curve.resize(point_count);

    double t = 0;
    for (int i = 0; i < discrete_curve.size(); ++i) {
        discrete_curve[i] = calculate_point(t);
        t += step_t;
    }
}

int BSpline::get_knot_index(int int_t) const {
    return int_t + 2;
}

QPointF BSpline::calculate_point(double t) const {
    const int int_t = static_cast<int>(t);
    const double fp_t = t - int_t;
    const int knot_idx = get_knot_index(int_t);
    if (knot_idx == 2) {
        return (1.0 - fp_t) * (1.0 - fp_t) * control_points[0] +
               (2.0 * fp_t - 1.5 * fp_t * fp_t) * control_points[1] +
               (0.5 * fp_t * fp_t) * control_points[2];
    }
    if (knot_idx == control_points.size() - 1) {
        return 0.5 * (2.0 * fp_t * fp_t * control_points[knot_idx] +
                      (1 + 2.0 * fp_t - 3.0 * fp_t * fp_t) * control_points[knot_idx - 1] +
                      (1.0 - fp_t) * (1.0 - fp_t) * control_points[knot_idx - 2]);
    }
    return 0.5 * (fp_t * fp_t * control_points[knot_idx] +
                  (1 + 2.0 * fp_t - 2.0 * fp_t * fp_t) * control_points[knot_idx - 1] +
                  (1.0 - fp_t) * (1.0 - fp_t) * control_points[knot_idx - 2]);
}
