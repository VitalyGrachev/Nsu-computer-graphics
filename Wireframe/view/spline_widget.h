#ifndef SPLINE_WIDGET_H
#define SPLINE_WIDGET_H

#include <QWidget>
#include <memory>
#include "../util/image_wrapper.h"
#include "../curve/bspline.h"

class SplineWidget : public QWidget {
Q_OBJECT
public:
    SplineWidget(QWidget * parent = nullptr);

public slots:
    void set_curve(BSpline * curve);

protected:
    void paintEvent(QPaintEvent * event) override;

    void resizeEvent(QResizeEvent * event) override;

private:
    void redraw_image();

    ImageWrapper shown_image;
    BSpline * curve;

    static const QRgb bg_color;
    static const QRgb curve_color;
    static const QRgb axis_color;
    static const QRgb control_point_color;
    static const int point_circle_radius = 6;
};

#endif //SPLINE_WIDGET_H
