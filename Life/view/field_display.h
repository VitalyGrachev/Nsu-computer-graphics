#ifndef FIELD_DISPLAY_H
#define FIELD_DISPLAY_H

#include <QWidget>
#include <memory>
#include "hex_grid_canvas.h"
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

public slots:

    void model_changed();

    void toggle_impacts();

protected:
    void paintEvent(QPaintEvent * event) override;

    void mousePressEvent(QMouseEvent * event) override;

private:
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

    static const uint32_t margin = 4;
    static const uint32_t min_edge_to_show_impacts = 6;
    static const QColor default_border_color;
    static const QColor default_dead_color;
    static const QColor default_alive_color;
};

#endif // FIELD_DISPLAY_H
