#include "field_display.h"

#include <QPainter>
#include <QPaintEvent>

FieldDisplay::FieldDisplay(LifeGameEngine * life_game_engine,
                           uint32_t cell_edge_size,
                           QWidget * parent)
        : QWidget(parent),
          game_engine(life_game_engine),
          canvas(new HexGridCanvas(life_game_engine->get_current_field()->cols(),
                                   life_game_engine->get_current_field()->rows(), cell_edge_size)),
          border_color(QColor(0, 0, 0).rgb()),
          dead_color(QColor(220, 220, 220).rgb()),
          alive_color(QColor(0, 220, 0).rgb()) {
    setFixedSize(canvas->get_width(), canvas->get_height());

    canvas->fill(dead_color);
    canvas->draw_hex_grid_borders(border_color);
    update_image();
}

FieldDisplay::~FieldDisplay() {
    delete canvas;
}

void FieldDisplay::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), canvas->get_image());
}

void FieldDisplay::update_image() {
    const LifeField & last_field = *game_engine->get_last_field();
    const LifeField & current_field = *game_engine->get_current_field();

    for (int row = 0; row < last_field.rows(); ++row) {
        for (int col = 0; col < last_field[row].size(); ++col) {
            LifeField::CellState current_state = current_field[row][col];
            if (last_field[row][col] != current_state) {
                QRgb color = (current_state == LifeField::CellState::ALIVE ?
                              alive_color : dead_color);
                canvas->fill_hex(col, row, color);
            }
        }
    }
}
