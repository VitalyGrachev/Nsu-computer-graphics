#ifndef FIELD_DISPLAY_H
#define FIELD_DISPLAY_H

#include <QWidget>
#include <memory>
#include "hex_grid_canvas.h"
#include "mode.h"
#include "../model/life_game_engine.h"

class FieldDisplay : public QWidget {
Q_OBJECT
public:
    explicit FieldDisplay(LifeGameEngine * life_game_engine,
                          uint32_t cell_edge_size,
                          QWidget * parent = 0);

    ~FieldDisplay() = default;

    FieldDisplay(const FieldDisplay &) = delete;

    FieldDisplay & operator=(const FieldDisplay &) = delete;

    int get_cell_edge_size() const { return canvas->get_cell_edge_size(); }

public slots:

    void model_changed();

    void set_impacts(bool set_on);

    void set_XOR_mode();

    void set_replace_mode();

signals:
    void set_cell(uint32_t col, uint32_t row, CellState state);

protected:
    void paintEvent(QPaintEvent * event) override;

    void mousePressEvent(QMouseEvent * event) override;

    void mouseMoveEvent(QMouseEvent * event) override;

private:
    void click_on_hex(uint32_t col, uint32_t row);

    void redraw_changed_cells();

    void redraw_all_cells();

    bool hex_under_cursor(int x, int y,
                          uint32_t * hex_col, uint32_t * hex_row) const;

    LifeGameEngine * game_engine;
    LifeStateField last_states;
    LifeImpactField last_impacts;
    std::unique_ptr<HexGridCanvas> canvas;
    const QRgb border_color;
    const QRgb dead_color;
    const QRgb alive_color;
    bool should_show_impacts;
    bool can_show_impacts;
    Mode current_mode;
    QPoint last_visited_hex;

    static const uint32_t margin = 4;
    static const QColor default_border_color;
    static const QColor default_dead_color;
    static const QColor default_alive_color;
    static const QColor default_text_color;
};

#endif // FIELD_DISPLAY_H
