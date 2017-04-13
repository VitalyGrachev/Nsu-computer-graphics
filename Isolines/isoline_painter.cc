#include "isoline_painter.h"

#include <QColor>
#include <bitset>

const QRgb IsolinePainter::line_color(QColor(0, 0, 0).rgb());
const QRgb IsolinePainter::grid_color(QColor(150, 150, 150).rgb());

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
    const float step_x = static_cast<float>(image.width()) / grid_size.width();
    const float step_y = static_cast<float>(image.height()) / grid_size.height();

    for (int y = 1; y <= grid_size.height(); ++y) {
        for (int x = 1; x <= grid_size.width(); ++x) {
            const int xx = static_cast<int>((x - 1) * step_x);
            const int yy = static_cast<int>((y - 1) * step_y);
            const int w = static_cast<int>(x * step_x) - xx;
            const int h = static_cast<int>(y * step_y) - yy;
            handle_single_cell(image, QRect(xx, yy, w, h), value_level);
        }
    }
}

void IsolinePainter::paint_grid(ImageWrapper & image) const {
    const float step_x = static_cast<float>(image.width()) / grid_size.width();
    const float step_y = static_cast<float>(image.height()) / grid_size.height();

    for (int y = 1; y < grid_size.height(); ++y) {
        const int yy = static_cast<int>(y * step_y);
        image.draw_line(0, yy, image.width() - 1, yy, grid_color);
    }

    for (int x = 1; x < grid_size.width(); ++x) {
        const int xx = static_cast<int>(x * step_x);
        image.draw_line(xx, 0, xx, image.height() - 1, grid_color);
    }
}

void IsolinePainter::set_grid_size(const QSize & grid_size) {
    if (this->grid_size != grid_size) {
        this->grid_size = grid_size;
    }
}

void IsolinePainter::handle_single_cell(ImageWrapper & image, const QRect & cell, float value_level) const {
    const int right_border = std::min(image.width()-1, cell.right() + 1);
    const int bottom_border = std::min(image.width()-1, cell.bottom() + 1);

    float lt = function_to_draw((*converter)(cell.left(), cell.top())) - value_level;
    float rt = function_to_draw((*converter)(right_border, cell.top())) - value_level;
    float lb = function_to_draw((*converter)(cell.left(), bottom_border)) - value_level;
    float rb = function_to_draw((*converter)(right_border, bottom_border)) - value_level;

    lt = (lt == 0.0f ? 0.1f : lt);
    rt = (rt == 0.0f ? 0.1f : rt);
    lb = (lb == 0.0f ? 0.1f : lb);
    rb = (rb == 0.0f ? 0.1f : rb);

    const int left_delta = static_cast<int>(cell.height() * std::abs(lt / (lt - lb)));
    const int right_delta = static_cast<int>(cell.height() * std::abs(rt / (rt - rb)));
    const int top_delta = static_cast<int>(cell.width() * std::abs(lt / (lt - rt)));
    const int bottom_delta = static_cast<int>(cell.width() * std::abs(lb / (lb - rb)));

    std::bitset<4> sign_bits;
    sign_bits[0] = std::signbit(lt);
    sign_bits[1] = std::signbit(rt);
    sign_bits[2] = std::signbit(lb);
    sign_bits[3] = std::signbit(rb);

    size_t positive_count = sign_bits.count();
    if (sign_bits.all() || sign_bits.none()) {
        // No isoline here.
        return;
    } else if (positive_count % 2) {
        // One diagonal line
        if (positive_count == 3) {
            sign_bits.flip();
        }
        if (sign_bits[0]) {
            image.draw_line(cell.x() + top_delta, cell.y(),
                            cell.x(), cell.y() + left_delta,
                            line_color, LineType::SOLID);
        } else if (sign_bits[1]) {
            image.draw_line(cell.x() + top_delta, cell.y(),
                            right_border, cell.y() + right_delta,
                            line_color, LineType::SOLID);
        } else if (sign_bits[2]) {
            image.draw_line(cell.x(), cell.y() + left_delta,
                            cell.x() + bottom_delta, bottom_border,
                            line_color, LineType::SOLID);
        } else {
            image.draw_line(right_border, cell.y() + right_delta,
                            cell.x() + bottom_delta, bottom_border,
                            line_color, LineType::SOLID);
        }
    } else if (sign_bits[0] && sign_bits[1] || sign_bits[2] && sign_bits[3]) {
        // Horizontal line
        image.draw_line(cell.x(), cell.y() + left_delta,
                        right_border, cell.y() + right_delta,
                        line_color, LineType::SOLID);
    } else if (sign_bits[0] && sign_bits[2] || sign_bits[1] && sign_bits[3]) {
        // Vertical line
        image.draw_line(cell.x() + top_delta, cell.y(),
                        cell.x() + bottom_delta, bottom_border,
                        line_color, LineType::SOLID);
    } else {
        // Two lines
        const QPoint center = (cell.topLeft() + cell.bottomRight()) / 2;
        const float center_value = function_to_draw((*converter)(center)) - value_level;
        const bool center_sign = std::signbit(center_value);

        if (center_sign == sign_bits[0]) {
            // Down directed diagonal
            image.draw_line(cell.x() + top_delta, cell.y(),
                            right_border, cell.y() + right_delta,
                            line_color, LineType::SOLID);
            image.draw_line(cell.x(), cell.y() + left_delta,
                            cell.x() + bottom_delta, bottom_border,
                            line_color, LineType::SOLID);
        } else {
            // Up directed diagonal
            image.draw_line(cell.x() + top_delta, cell.y(),
                            cell.x(), cell.y() + left_delta,
                            line_color, LineType::SOLID);
            image.draw_line(right_border, cell.y() + right_delta,
                            cell.x() + bottom_delta, bottom_border,
                            line_color, LineType::SOLID);
        }
    }
}
