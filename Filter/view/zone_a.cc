#include "zone_a.h"

#include <QMouseEvent>

#include "../filters/zoom/zoom_filter.h"

const QRgb ZoneA::rect_color = QColor(200, 200, 200).rgb();

ZoneA::ZoneA(int width, int height, QWidget * parent)
        : Zone(width, height, parent),
          is_selecting(false),
          is_selection_enabled(false) {}

QRect ZoneA::selected_rect(const QPoint & mouse_pos) const {
    const float scale = 1.0f / scale_factor;
    const int pos_x = scale * (mouse_pos.x() - 1) - width() / 2;
    const int pos_y = scale * (mouse_pos.y() - 1) - height() / 2;
    const int rect_left_x = std::min(std::max(pos_x, 0), std::max(original_image.width() - width(), 0));
    const int rect_top_y = std::min(std::max(pos_y, 0), std::max(original_image.height() - height(), 0));
    const int remaining_x = original_image.width() - rect_left_x;
    const int remaining_y = original_image.height() - rect_top_y;
    const int rect_width = std::min(width(), remaining_x);
    const int rect_height = std::min(height(), remaining_y);
    return QRect(rect_left_x, rect_top_y, rect_width, rect_height);
}

void ZoneA::show_rect(const QRect & rect) {
    ImageWrapper modified(scaled_image);
    const int lx = rect.x();
    const int ty = rect.y();
    const int rx = lx + rect.width() - 1;
    const int by = ty + rect.height() - 1;
    modified.draw_line(lx, ty, rx, ty, rect_color);
    modified.draw_line(lx, ty, lx, by, rect_color);
    modified.draw_line(rx, ty, rx, by, rect_color);
    modified.draw_line(lx, by, rx, by, rect_color);

    Zone::set_image(modified);
}

void ZoneA::select_image(const QPoint & mouse_pos) {
    QRect rect = selected_rect(mouse_pos);
    emit selected_image(original_image.copy(rect));
    show_rect(QRect(rect.x() * scale_factor, rect.y() * scale_factor,
                    rect.width() * scale_factor, rect.height() * scale_factor));
}

void ZoneA::mousePressEvent(QMouseEvent * event) {
    if (is_selection_enabled && event->button() == Qt::LeftButton) {
        is_selecting = true;
        select_image(event->pos());
    }
    event->accept();
}

void ZoneA::mouseMoveEvent(QMouseEvent * event) {
    if (is_selecting) {
        select_image(event->pos());
    }
    event->accept();
}

void ZoneA::mouseReleaseEvent(QMouseEvent * event) {
    if (is_selecting && event->button() == Qt::LeftButton) {
        is_selecting = false;
        Zone::set_image(scaled_image);
    }
    event->accept();
}

void ZoneA::clear() {
    Zone::clear();
    original_image = ImageWrapper();
}

void ZoneA::set_image(ImageWrapper image) {
    original_image = image;

    const float w_scale_factor = static_cast<float>(width()) / image.width();
    const float h_scale_factor = static_cast<float>(height()) / image.height();
    const float res_scale_factor = std::min(w_scale_factor, h_scale_factor);
    scale_factor = res_scale_factor;
    ZoomFilter filter(scale_factor);
    scaled_image = filter(image);
    Zone::set_image(scaled_image);
}

void ZoneA::set_selection_enabled(bool enabled) {
    is_selection_enabled = enabled;
    is_selecting = false;
}
