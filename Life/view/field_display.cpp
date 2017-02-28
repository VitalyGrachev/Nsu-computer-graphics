#include "field_display.h"

#include <cstdint>
#include <QPainter>
#include <QPaintEvent>

FieldDisplay::FieldDisplay(LifeGameEngine * life_game_engine,
                           uint32_t cell_edge_size,
                           QWidget * parent)
        : QWidget(parent),
          game_engine(life_game_engine),
          last_states(game_engine->get_state_field()),
          last_impacts(game_engine->get_impact_field()),
          canvas(new HexGridCanvas(last_states.cols(),
                                   last_states.rows(), cell_edge_size)),
          border_color(QColor(0, 0, 0).rgb()),
          dead_color(QColor(220, 220, 220).rgb()),
          alive_color(QColor(0, 220, 0).rgb()),
          should_show_impacts(false),
          can_show_impacts(cell_edge_size >= min_edge_to_show_impacts) {
    setFixedSize(canvas->get_width(), canvas->get_height());

    canvas->fill(dead_color);
    canvas->draw_hex_grid_borders(border_color);
    redraw_all_cells();
}

void FieldDisplay::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), canvas->get_image());
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
                if (should_show_impacts && can_show_impacts) {
                    // TODO: print impact
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
            if (should_show_impacts && can_show_impacts) {
                // TODO: print impact
            }
        }
    }
}

void FieldDisplay::model_changed() {
    redraw_changed_cells();
    last_states = game_engine->get_state_field();
    last_impacts = game_engine->get_impact_field();
    update();
}

void FieldDisplay::toggle_impacts() {
    should_show_impacts = !should_show_impacts;
    if (can_show_impacts) {
        redraw_all_cells();
        update();
    }
}