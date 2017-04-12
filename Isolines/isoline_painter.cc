#include "isoline_painter.h"

#include <QColor>

const QRgb IsolinePainter::line_color(QColor(50, 50, 50).rgb());

IsolinePainter::IsolinePainter(const FunctionToDraw & function_to_draw,
                               std::shared_ptr<CoordinatesConverter> & coordinates_converter,
                               const QSize & grid_size)
        : function_to_draw(function_to_draw),
          grid_size(grid_size),
          converter(coordinates_converter) {}

void IsolinePainter::operator()(ImageWrapper & image, int x, int y) const {
    const float value = function_to_draw((*converter)(x, y));
    return operator()(image, value);
}

void IsolinePainter::operator()(ImageWrapper & image, const QPoint & point) const {
    operator()(image, point.x(), point.y());
}

void IsolinePainter::operator()(ImageWrapper & image, float value_level) const {
    const int step_x = image.width() / grid_size.width();
    const int step_y = image.height() / grid_size.height();

    for (int y = 0; y < image.height(); y += step_y) {
        for (int x = 0; x < image.width(); x += step_x) {

        }
    }
}

void IsolinePainter::paint_grid(ImageWrapper & image) const {
    const int step_x = image.width() / grid_size.width();
    const int step_y = image.height() / grid_size.height();

    for (int y = 0; y < image.height(); y += step_y) {
        image.draw_line(0, y, image.width() - 1, y, line_color);
    }

    for (int x = 0; x < image.width(); x += step_x) {
        image.draw_line(x, 0, x, image.height() - 1, line_color);
    }
}

void IsolinePainter::set_grid_size(const QSize & grid_size) {
    if (this->grid_size != grid_size) {
        this->grid_size = grid_size;
    }
}
