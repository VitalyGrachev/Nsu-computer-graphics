#include "hex_grid_canvas.h"

const float HALF_SQRT_OF_3 = 0.866f;

HexGridCanvas::HexGridCanvas(int cells_x,
                             int cells_y,
                             int cell_edge_size)
        : Canvas((2 * cells_x) * static_cast<int>(cell_edge_size * HALF_SQRT_OF_3) + 1,
                 (1 + cells_y * 3) * (cell_edge_size / 2) + 1),
          cell_cols(cells_x),
          cell_rows(cells_y),
          cell_edge_size(cell_edge_size),
          cell_half_width(static_cast<int>(cell_edge_size * HALF_SQRT_OF_3)),
          cell_quarter_height(cell_edge_size / 2) {
}

void HexGridCanvas::draw_hex_border(uint32_t col, uint32_t row, QRgb border_color) {
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

void HexGridCanvas::draw_hex_grid_borders(QRgb border_color) {
    for (int row = 0; row < cell_rows; ++row) {
        for (int col = 0; col < cell_cols - (row % 2); ++col) {
            draw_hex_border(col, row, border_color);
        }
    }
}

void HexGridCanvas::fill_hex(uint32_t col, uint32_t row, QRgb fill_color) {
    const QPoint hex_pos = hex_position(col, row);
    const int dx = cell_half_width;
    const int dy = cell_quarter_height;
    const int delta_err = dy;
    int error = 0;
    int ry = 0;
    for (int rx = 0; rx < dx; ++rx) {
        error += delta_err;
        if (2 * error >= dx) {
            ++ry;
            draw_horizontal_line(hex_pos.x() + dx - rx,
                                 hex_pos.x() + dx + rx,
                                 hex_pos.y() + ry,
                                 fill_color);
            draw_horizontal_line(hex_pos.x() + dx - rx,
                                 hex_pos.x() + dx + rx,
                                 hex_pos.y() + 4 * dy - ry,
                                 fill_color);
            error -= dx;
        }
    }
    for (int ry = 1; ry < 2 * dy; ++ry) {
        draw_horizontal_line(hex_pos.x() + 1,
                             hex_pos.x() + 2 * dx - 1,
                             hex_pos.y() + dy + ry,
                             fill_color);
    }
}

bool HexGridCanvas::hex_under_cursor(int x, int y,
                                     uint32_t * hex_col, uint32_t * hex_row) const {

    int hr = y / (3 * cell_quarter_height);
    int hc = (x - (hr % 2) * cell_half_width) / (2 * cell_half_width);

    if (x < (hr % 2) * cell_half_width) {
        return false;
    }

    int yy = y % (3 * cell_quarter_height);
    int xx = (x - (hr % 2) * cell_half_width) % (2 * cell_half_width);

    int dx = std::abs(cell_half_width - xx);
    int dy = (2 * dx * cell_quarter_height + cell_half_width) / (2 * cell_half_width);
    if (yy == dy || !xx) {
        return false;
    }
    if (yy > dy) {
        if (hc < 0 || hc >= cell_cols - (hr % 2) ||
            hr < 0 || hr >= cell_rows) {
            return false;
        }
        *hex_col = hc;
        *hex_row = hr;
    } else if (xx > cell_half_width) {
        if (hc + (hr % 2) < 0 || hc + (hr % 2) >= cell_cols - (hr % 2) ||
            hr < 1 || hr > cell_rows) {
            return false;
        }
        *hex_col = hc + (hr % 2);
        *hex_row = hr - 1;
    } else {
        if (hc + (hr % 2) < 1 || hc + (hr % 2) > cell_cols - (hr % 2) ||
            hr < 1 || hr > cell_rows) {
            return false;
        }
        *hex_col = hc + (hr % 2) - 1;
        *hex_row = hr - 1;
    }
    return true;
}