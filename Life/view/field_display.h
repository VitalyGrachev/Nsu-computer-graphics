#ifndef FIELD_DISPLAY_H
#define FIELD_DISPLAY_H

#include <QWidget>
#include "hex_grid_canvas.h"
#include "../model/life_game_engine.h"

class FieldDisplay : public QWidget
{
public:
    explicit FieldDisplay(LifeGameEngine * life_game_engine,
                          uint32_t cell_edge_size,
                          QWidget * parent = 0);
    ~FieldDisplay();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void update_image();

    HexGridCanvas * canvas;
    LifeGameEngine * game_engine;
    const QRgb border_color;
    const QRgb dead_color;
    const QRgb alive_color;
};

#endif // FIELD_DISPLAY_H
