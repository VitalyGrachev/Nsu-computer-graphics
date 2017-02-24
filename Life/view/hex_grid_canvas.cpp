#include "hex_grid_canvas.h"

const float HALF_SQRT_OF_3 = 0.866f;

HexGridCanvas::HexGridCanvas(int cells_x,
                             int cells_y,
                             int cell_side_size,
                             const QColor & bg_color,
                             const QColor & border_color)
        : Canvas((2 * cells_x + (cells_y != 1)) * static_cast<int>(cell_side_size * HALF_SQRT_OF_3) + 1,
                 (1 + cells_y * 3) * (cell_side_size / 2) + 1),
          cell_cols(cells_x),
          cell_rows(cells_y),
          cell_half_width(static_cast<int>(cell_side_size * HALF_SQRT_OF_3)),
          cell_quarter_height(cell_side_size / 2),
          background_color(bg_color.rgb()),
          border_color(border_color.rgb()) {
}

void HexGridCanvas::draw_hex_border(int col, int row) {
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
    for(int row = 0; row < cell_rows; ++row) {
        for(int col = 0; col < cell_cols; ++col) {
            draw_hex_border(col, row);
        }
    }
}
