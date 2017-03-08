#include "field_display.h"

#include <cstdint>
#include <QPainter>
#include <QPaintEvent>
#include <iostream>

const QColor FieldDisplay::default_border_color = QColor(70, 70, 70);
const QColor FieldDisplay::default_dead_color = QColor(225, 225, 225);
const QColor FieldDisplay::default_alive_color = QColor(0, 220, 0);
const QColor FieldDisplay::default_text_color = QColor(0, 0, 0);

FieldDisplay::FieldDisplay(LifeGameEngine * life_game_engine,
                           uint32_t cell_edge_size,
                           QWidget * parent)
        : QWidget(parent),
          game_engine(life_game_engine),
          last_states(game_engine->get_state_field()),
          last_impacts(game_engine->get_impact_field()),
          canvas(new HexGridCanvas(last_states.cols(),
                                   last_states.rows(), cell_edge_size)),
          border_color(default_border_color.rgb()),
          dead_color(default_dead_color.rgb()),
          alive_color(default_alive_color.rgb()),
          should_show_impacts(false),
          can_show_impacts(cell_edge_size >= HexGridCanvas::font_size),
          current_mode(Mode::XOR) {
    setFixedSize(canvas->get_width() + 2 * margin, canvas->get_height() + 2 * margin);
    QPalette widget_palette = palette();
    widget_palette.setColor(QPalette::Window, default_dead_color);
    setPalette(widget_palette);

    canvas->fill(dead_color);
    canvas->draw_hex_grid_borders(border_color);
    redraw_all_cells();
}

void FieldDisplay::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(margin, margin), canvas->get_image());
}

void FieldDisplay::mousePressEvent(QMouseEvent * event) {
    uint32_t col, row;
    if (event->button() == Qt::LeftButton &&
        hex_under_cursor(event->x(), event->y(), &col, &row)) {
        click_on_hex(col, row);
        last_visited_hex = QPoint(col, row);
    }
}

void FieldDisplay::mouseMoveEvent(QMouseEvent * event) {
    uint32_t col, row;
    if (hex_under_cursor(event->x(), event->y(), &col, &row)) {
        QPoint cur_hex(col, row);
        if(cur_hex != last_visited_hex) {
            click_on_hex(col, row);
            last_visited_hex = cur_hex;
        }
    }
}

void FieldDisplay::click_on_hex(uint32_t col, uint32_t row) {
    if (current_mode == Mode::XOR) {
        if (last_states[row][col] == LifeStateField::ALIVE) {
            emit set_cell(col, row, LifeStateField::DEAD);
        } else {
            emit set_cell(col, row, LifeStateField::ALIVE);
        }
    } else {
        emit set_cell(col, row, LifeStateField::ALIVE);
    }
}

void FieldDisplay::redraw_changed_cells() {
    const LifeStateField & cur_states = game_engine->get_state_field();
    const LifeImpactField & cur_impacts = game_engine->get_impact_field();
    const uint32_t rows = cur_states.rows();

    for (uint32_t row = 0; row < rows; ++row) {
        for (uint32_t col = 0; col < cur_states[row].size(); ++col) {
            const CellState current_state = cur_states[row][col];
            const double current_impact = cur_impacts[row][col];
            if (last_states[row][col] != current_state ||
                last_impacts[row][col] != current_impact) {
                QRgb color = (current_state == LifeStateField::ALIVE ? alive_color : dead_color);
                canvas->fill_hex(col, row, color);

                last_states[row][col] = cur_states[row][col];

                if (should_show_impacts && can_show_impacts) {
                    canvas->draw_hex_text(col, row,
                                          QString::number(cur_impacts[row][col], 'g', 2),
                                          default_text_color);
                    last_impacts[row][col] = cur_impacts[row][col];
                }
            }
        }
    }
}

void FieldDisplay::redraw_all_cells() {
    const LifeStateField & cur_states = game_engine->get_state_field();
    const LifeImpactField & cur_impacts = game_engine->get_impact_field();
    const uint32_t rows = cur_states.rows();

    for (uint32_t row = 0; row < rows; ++row) {
        for (uint32_t col = 0; col < cur_states[row].size(); ++col) {
            QRgb color = (cur_states[row][col] == LifeStateField::ALIVE ? alive_color : dead_color);
            canvas->fill_hex(col, row, color);

            last_states[row][col] = cur_states[row][col];

            if (should_show_impacts && can_show_impacts) {
                canvas->draw_hex_text(col, row,
                                      QString::number(cur_impacts[row][col], 'g', 2),
                                      default_text_color);
                last_impacts[row][col] = cur_impacts[row][col];
            }
        }
    }
}

bool FieldDisplay::hex_under_cursor(int x, int y,
                                    uint32_t * hex_col, uint32_t * hex_row) const {
    const int image_x = x - margin;
    const int image_y = y - margin;
    if (x <= 0 || y <= 0 || x >= canvas->get_width() || y >= canvas->get_height()) {
        return false;
    }
    return canvas->hex_under_cursor(image_x, image_y, hex_col, hex_row);
}

void FieldDisplay::model_changed() {
    redraw_changed_cells();
    update();
}

void FieldDisplay::set_impacts(bool set_on) {
    bool last_state = should_show_impacts;
    should_show_impacts = set_on;
    if (can_show_impacts && last_state != should_show_impacts) {
        redraw_all_cells();
        update();
    }
}

void FieldDisplay::set_XOR_mode() {
    current_mode = Mode::XOR;
}
void FieldDisplay::set_replace_mode() {
    current_mode = Mode::REPLACE;
}
