#include "field_display.h"

#include <QPainter>
#include <QPaintEvent>

FieldDisplay::FieldDisplay(QWidget *parent) :
    QWidget(parent), canvas(new HexGridCanvas(9, 9, 70))
{
    setFixedSize(canvas->get_width(), canvas->get_height());

    canvas->fill(QColor(220, 220, 220).rgb());
    canvas->fill_hex(1, 1, QColor(0, 220, 0).rgb());
    canvas->fill_hex(1, 2, QColor(0, 220, 0).rgb());
    canvas->fill_hex(0, 1, QColor(0, 220, 0).rgb());
    canvas->fill_hex(0, 0, QColor(0, 220, 0).rgb());
    canvas->draw_hex_grid_borders();
}

FieldDisplay::~FieldDisplay() {
    delete canvas;
}

void FieldDisplay::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), canvas->get_image());
}
