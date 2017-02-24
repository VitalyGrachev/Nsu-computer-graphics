#ifndef HEX_GRID_CANVAS_H
#define HEX_GRID_CANVAS_H

#include <QColor>
#include "canvas.h"

class HexGridCanvas : public Canvas {
public:
    HexGridCanvas(int cells_x, int cells_y, int cell_side_size,
                  const QColor & bg_color = QColor(220, 220, 220),
                  const QColor & border_color = QColor(0, 0, 0));

    void draw_hex_grid_borders();
    void draw_hex_border(int col, int row);
private:
    QPoint hex_position(int col, int row) const;

    const int cell_cols;
    const int cell_rows;
    const int cell_half_width;
    const int cell_quarter_height;
    const QRgb background_color;
    const QRgb border_color;
};

inline QPoint HexGridCanvas::hex_position(int col, int row) const {
    int xx = ((row % 2) + 2 * col) * cell_half_width;
    int yy = (3 * row) * cell_quarter_height;
    return QPoint(xx, yy);
}

#endif // HEX_GRID_CANVAS_H
