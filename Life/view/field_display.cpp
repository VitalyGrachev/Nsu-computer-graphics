#include "field_display.h"

#include <QPainter>
#include <QPaintEvent>

FieldDisplay::FieldDisplay(QWidget *parent) :
    QWidget(parent), canvas(new HexGridCanvas(9, 9, 45))
{
    setFixedSize(canvas->get_width(), canvas->get_height());

    canvas->fill(QColor(220, 220, 220).rgb());
    canvas->draw_hex_grid_borders();
}

FieldDisplay::~FieldDisplay() {
    delete canvas;
}

void FieldDisplay::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), canvas->get_image());
}
