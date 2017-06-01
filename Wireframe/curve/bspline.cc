#include "bspline.h"

BSpline::BSpline(int major_segments_count, int minor_segments_count)
        : Curve(major_segments_count, minor_segments_count) {}

void BSpline::add_control_point(const QPointF & point) {
    control_points.push_back(point);
    recalculate_discrete_curve();
}

void BSpline::recalculate_discrete_curve() {
    const int points_count = major_segments_count() * minor_segments_count() + 1;
    discrete_curve.resize(points_count);

    if (control_points.size() == 2) {
        const double step_t = 1.0 / (points_count - 1);
        double t = 0;
        for (int i = 0; i < points_count; ++i) {
            discrete_curve[i] = t * control_points[0] + (1.0 - t) * control_points[1];
            t += step_t;
        }
    } else {
        const int n = control_points.size() - 1;
        const double step_t = static_cast<double>(n - 1) / (points_count - 1);
        discrete_curve.front() = control_points.front();
        double t = step_t;
        for (int i = 1; i < points_count; ++i) {
            discrete_curve[i] = calculate_point(t);
            t += step_t;
        }
        discrete_curve.back() = control_points.back();
    }
}

int BSpline::get_knot_index(int int_t) const {
    return int_t + 2;
}

QPointF BSpline::calculate_point(double t) const {
    const int n = control_points.size() - 1;
    const int int_t = static_cast<int>(t);
    const double fp_t = t - int_t;
    const int knot_idx = get_knot_index(int_t);
    if (int_t == 0) {
        return (1.0 - fp_t) * (1.0 - fp_t) * control_points[0] +
               (2.0 * fp_t - 1.5 * fp_t * fp_t) * control_points[1] +
               (0.5 * fp_t * fp_t) * control_points[2];
    }
    if (int_t == n - 2) {
        return 0.5 * (2.0 * fp_t * fp_t * control_points[n] +
                      (1 + 2.0 * fp_t - 3.0 * fp_t * fp_t) * control_points[n - 1] +
                      (1.0 - fp_t) * (1.0 - fp_t) * control_points[n - 2]);
    }
    return 0.5 * (fp_t * fp_t * control_points[knot_idx] +
                  (1 + 2.0 * fp_t - 2.0 * fp_t * fp_t) * control_points[knot_idx - 1] +
                  (1.0 - fp_t) * (1.0 - fp_t) * control_points[knot_idx - 2]);
}
