#ifndef HEX_GRID_CANVAS_H
#define HEX_GRID_CANVAS_H

#include <cstdint>
#include <QColor>
#include <QPainter>
#include <QString>
#include "canvas.h"

class HexGridCanvas : public Canvas {
public:
    HexGridCanvas(int cells_x, int cells_y, int cell_edge_size);

    void draw_hex_grid_borders(QRgb border_color);

    void draw_hex_border(uint32_t col, uint32_t row, QRgb border_color);

    void fill_hex(uint32_t col, uint32_t row, QRgb fill_color);

    void draw_hex_text(uint32_t col, uint32_t row, const QString & text, const QColor & text_color);

    bool hex_under_cursor(int x, int y,
                          uint32_t * hex_col, uint32_t * hex_row) const;

    int get_cell_edge_size() const { return cell_edge_size; }

    static const int font_size = 10;
private:

    QPoint hex_position(uint32_t col, uint32_t row) const;
    QPainter text_painter;
    const int cell_cols;
    const int cell_rows;
    const int cell_edge_size;
    const int cell_half_width;

    const int cell_quarter_height;
};

inline QPoint HexGridCanvas::hex_position(uint32_t col, uint32_t row) const {
    int xx = ((row % 2) + 2 * col) * cell_half_width;
    int yy = (3 * row) * cell_quarter_height;
    return QPoint(xx, yy);
}

#endif // HEX_GRID_CANVAS_H
