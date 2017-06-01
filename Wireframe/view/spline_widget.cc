#include "spline_widget.h"

#include <QPaintEvent>
#include <QPainter>

namespace {

class ScaleHelper {
public:
    ScaleHelper(const Curve::PointContainer & spline_points, const Curve::PointContainer & control_points,
                    const QSize & image_size);

    QPoint operator()(const QPointF & point) const {
        QPoint pt = (point * scale_factor).toPoint();
        return image_center + QPoint(pt.x(), -pt.y());
    }

private:
    QPoint image_center;
    double scale_factor;
};

ScaleHelper::ScaleHelper(const Curve::PointContainer & spline_points,
                         const Curve::PointContainer & control_points,
                         const QSize & image_size) {
    if (!spline_points.empty()) {
        double max_dx = std::abs(spline_points[0].x());
        double max_dy = std::abs(spline_points[0].y());
        for (const QPointF & point : spline_points) {
            max_dx = std::max(max_dx, std::abs(point.x()));
            max_dy = std::max(max_dy, std::abs(point.y()));
        }
        for (const QPointF & point : control_points) {
            max_dx = std::max(max_dx, std::abs(point.x()));
            max_dy = std::max(max_dy, std::abs(point.y()));
        }

        scale_factor = std::min((image_size.width() - 20) / (2.0 * max_dx),
                                (image_size.height() - 20) / (2.0 * max_dy));
    } else {
        scale_factor = 1.0;
    }
    image_center = QPoint(image_size.width() / 2, image_size.height() / 2);
}

}

const QRgb SplineWidget::bg_color = QColor(0, 0, 0).rgb();
const QRgb SplineWidget::curve_color = QColor(180, 180, 250).rgb();
const QRgb SplineWidget::axis_color = QColor(180, 180, 180).rgb();
const QRgb SplineWidget::control_point_color = QColor(Qt::yellow).rgb();

SplineWidget::SplineWidget(QWidget * parent)
        : QWidget(parent), curve(nullptr) {
    redraw_image();
}

void SplineWidget::set_curve(BSpline * curve) {
    this->curve = curve;
    redraw_image();
}

void SplineWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(0, 0, shown_image.to_QImage());
    event->accept();
}

void SplineWidget::resizeEvent(QResizeEvent * event) {
    redraw_image();
    event->accept();
}

void SplineWidget::redraw_image() {
    shown_image = ImageWrapper(size());
    shown_image.fill(bg_color);
    shown_image.draw_line(0, shown_image.height() / 2,
                          shown_image.width(), shown_image.height() / 2, axis_color);
    shown_image.draw_line(shown_image.width() / 2, 0,
                          shown_image.width() / 2, shown_image.height(), axis_color);
    if (curve) {
        const Curve::PointContainer & curve_points = curve->get_points();
        ScaleHelper scale_helper(curve_points, curve->get_control_points(), shown_image.size());

        QPoint pt = scale_helper(QPointF(1, 0));
        shown_image.draw_line(pt.x(), shown_image.height() / 2 - 2,
                              pt.x(), shown_image.height() / 2 + 2, axis_color);

        const int segment_count = curve_points.size() - 1;
        for (int i = 0; i < segment_count; ++i) {
            shown_image.draw_line(scale_helper(curve_points[i]), scale_helper(curve_points[i + 1]), curve_color);
        }

        for (const QPointF & point : curve->get_control_points()) {
            shown_image.draw_circle(scale_helper(point), point_circle_radius, control_point_color);
        }
    }
    update();
}
