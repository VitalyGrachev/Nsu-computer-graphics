#include "field_display.h"

#include <QPainter>
#include <QPaintEvent>

FieldDisplay::FieldDisplay(QWidget *parent) :
    QWidget(parent), image(new QImage(400, 400, QImage::Format_RGB32))
{

}

void FieldDisplay::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), *image);
}

void FieldDisplay::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}
