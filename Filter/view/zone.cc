#include "zone.h"

#include <QPainter>

const QRgb Zone::border_color = QColor(0, 0, 0).rgb();
const QRgb Zone::background_color = QColor(255, 255, 255).rgb();

Zone::Zone(int width, int height, QWidget * parent)
        : QWidget(parent),
          shown_image(width + 2, height + 2) {
    shown_image.draw_line(0, 0, width + 1, 0, border_color);
    shown_image.draw_line(0, height + 1, width + 1, height + 1, border_color);
    shown_image.draw_line(0, 0, 0, height + 1, border_color);
    shown_image.draw_line(width + 1, 0, width + 1, height + 1, border_color);
    shown_image.fill(QRect(1, 1, width, height), background_color);

    setFixedSize(width + 2, height + 2);
}

void Zone::clear() {
    attached_image = ImageWrapper();

    const int width = shown_image.width() - 2;
    const int height = shown_image.height() - 2;
    shown_image.fill(QRect(1, 1, width, height), background_color);
    update();
}

void Zone::set_image(ImageWrapper image) {
    attached_image = image;

    const int width_diff = width() - attached_image.width();
    const int height_diff = height() - attached_image.height();
    shown_image.insert_image(attached_image, 1, 1);

    shown_image.fill(QRect(attached_image.width() + 1, 1, width_diff, height_diff), background_color);
    shown_image.fill(QRect(1, attached_image.height() + 1, width_diff, height_diff), background_color);

    update();
}

void Zone::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), shown_image.to_QImage());
}