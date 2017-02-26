#include "hex_grid_canvas.h"

const float HALF_SQRT_OF_3 = 0.866f;

HexGridCanvas::HexGridCanvas(int cells_x,
                             int cells_y,
                             int cell_side_size,
                             const QColor & border_color)
        : Canvas((2 * cells_x + (cells_y != 1)) * static_cast<int>(cell_side_size * HALF_SQRT_OF_3) + 1,
                 (1 + cells_y * 3) * (cell_side_size / 2) + 1),
          cell_cols(cells_x),
          cell_rows(cells_y),
          cell_half_width(static_cast<int>(cell_side_size * HALF_SQRT_OF_3)),
          cell_quarter_height(cell_side_size / 2),
          border_color(border_color.rgb()) {
}

void HexGridCanvas::draw_hex_border(uint32_t col, uint32_t row) {
    QPoint pos = hex_position(col, row);
    draw_line_bresenham_unsafe(pos.x(),
                               pos.y() + cell_quarter_height,
                               pos.x() + cell_half_width,
                               pos.y(), border_color);
    draw_line_bresenham_unsafe(pos.x() + 2 * cell_half_width,
                               pos.y() + cell_quarter_height,
                               pos.x() + cell_half_width,
                               pos.y(), border_color);

    draw_vertical_line(pos.x(),
                       pos.y() + cell_quarter_height,
                       pos.y() + 3 * cell_quarter_height, border_color);
    draw_vertical_line(pos.x() + 2 * cell_half_width,
                       pos.y() + cell_quarter_height,
                       pos.y() + 3 * cell_quarter_height, border_color);

    draw_line_bresenham_unsafe(pos.x(),
                               pos.y() + 3 * cell_quarter_height,
                               pos.x() + cell_half_width,
                               pos.y() + 4 * cell_quarter_height, border_color);
    draw_line_bresenham_unsafe(pos.x() + 2 * cell_half_width,
                               pos.y() + 3 * cell_quarter_height,
                               pos.x() + cell_half_width,
                               pos.y() + 4 * cell_quarter_height, border_color);
}

void HexGridCanvas::draw_hex_grid_borders() {
    for (int row = 0; row < cell_rows; ++row) {
        for (int col = 0; col < cell_cols; ++col) {
            draw_hex_border(col, row);
        }
    }
}

void HexGridCanvas::fill_hex(uint32_t col, uint32_t row, QRgb fill_color) {
    const QPoint hex_pos = hex_position(col, row);
    const uint32_t dx = cell_half_width;
    const uint32_t dy = cell_quarter_height;
    const uint32_t delta_err = dy;
    uint32_t error = delta_err;
    uint32_t ry = 0;
    for (uint32_t rx = 1; rx <= dx; ++rx) {
        error += delta_err;
        if (2 * error >= dx) {
            draw_horizontal_line(hex_pos.x() + dx - rx + 1,
                                 hex_pos.x() + dx + rx - 1,
                                 hex_pos.y() + ry,
                                 fill_color);
            draw_horizontal_line(hex_pos.x() + dx - rx + 1,
                                 hex_pos.x() + dx + rx - 1,
                                 hex_pos.y() + 4 * dy - ry,
                                 fill_color);
            ++ry;
            error -= dx;
        }
    }
    for(uint32_t ry = 1; ry < 2 * dy; ++ry) {
        draw_horizontal_line(hex_pos.x() + 1,
                             hex_pos.x() + 2 * dx - 1,
                             hex_pos.y() + dy + ry,
                             fill_color);
    }
}
