#ifndef HEX_GRID_CANVAS_H
#define HEX_GRID_CANVAS_H

#include <cstdint>
#include <QColor>
#include "canvas.h"

class HexGridCanvas : public Canvas {
public:
    HexGridCanvas(int cells_x, int cells_y, int cell_side_size,
                  const QColor & border_color = QColor(0, 0, 0));

    void draw_hex_grid_borders();
    void draw_hex_border(uint32_t col, uint32_t row);
    void fill_hex(uint32_t col, uint32_t row, QRgb fill_color);
private:
    QPoint hex_position(uint32_t col, uint32_t row) const;

    const int cell_cols;
    const int cell_rows;
    const int cell_half_width;
    const int cell_quarter_height;
    const QRgb border_color;
};

inline QPoint HexGridCanvas::hex_position(uint32_t col, uint32_t row) const {
    int xx = ((row % 2) + 2 * col) * cell_half_width;
    int yy = (3 * row) * cell_quarter_height;
    return QPoint(xx, yy);
}

#endif // HEX_GRID_CANVAS_H
