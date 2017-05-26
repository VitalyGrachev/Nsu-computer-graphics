#ifndef SPLINE_WIDGET_H
#define SPLINE_WIDGET_H

#include <QWidget>
#include <memory>
#include "../util/image_wrapper.h"
#include "../curve/bspline.h"

class SplineWidget : public QWidget {
public:
    SplineWidget(QWidget * parent = nullptr);

    void set_curve(std::shared_ptr<BSpline> & curve);

protected:
    void paintEvent(QPaintEvent * event) override;

    void resizeEvent(QResizeEvent * event) override;

private:
    void redraw_image();

    ImageWrapper shown_image;
    std::shared_ptr<BSpline> curve;

    static const QRgb bg_color;
    static const QRgb curve_color;
    static const QRgb axis_color;
    static const QRgb control_point_color;
    static const int point_circle_radius = 6;
};

#endif //SPLINE_WIDGET_H
