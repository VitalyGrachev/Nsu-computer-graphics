#include "main_window.h"

MainWindow::MainWindow() {
//    FunctionToDraw function_to_draw = [](const QPointF & pt) -> float { return pt.x() + pt.y(); };
    FunctionToDraw function_to_draw = [](const QPointF & pt) -> float {
        return (2 * pt.x() - 55.0f) * (pt.x() + 17.5f) +
               0.6 * std::sin(0.7*pt.y()) * (pt.y() - 15.0f) * (pt.y() + 7.5f);
    };
    QRectF domain(-55.0f, -55.0f, 125.0f, 125.0f);
    QSize grid_size(10, 10);
    std::vector<QRgb> colors({QColor(Qt::red).rgb(),
                               QColor(Qt::yellow).rgb(),
                               QColor(Qt::green).rgb(),
                               QColor(Qt::blue).rgb()});
    color_map_widget = new ColorMapWidget(function_to_draw, domain, grid_size, colors);
    color_map_widget->set_interpolate_colors(true);
    color_map_widget->set_draw_grid(true);
    setCentralWidget(color_map_widget);
    setWindowTitle(tr("FIT_14202_Grachev_Isolines"));
    resize(800, 600);
}
