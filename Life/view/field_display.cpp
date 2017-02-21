#include "field_display.h"

#include <QPainter>
#include <QPaintEvent>

FieldDisplay::FieldDisplay(QWidget *parent) :
    QWidget(parent), canvas(new Canvas(500, 500))
{
    QColor black(Qt::black);
    canvas->fill(QColor(220, 220, 220).rgb());
    canvas->draw_line(QPoint(20, 20), QPoint(350, 200), black.rgb());
    canvas->draw_line(QPoint(20, 20), QPoint(200, 20), black.rgb());
    canvas->draw_line(QPoint(20, 20), QPoint(20, 200), black.rgb());
    canvas->draw_line(QPoint(20, 200), QPoint(350, 200), black.rgb());
}

FieldDisplay::~FieldDisplay() {
    delete canvas;
}

void FieldDisplay::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), canvas->get_image());
}

void FieldDisplay::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}
