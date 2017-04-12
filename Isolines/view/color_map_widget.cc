#include "color_map_widget.h"

#include <QPainter>
#include <QPaintEvent>
#include "../plain_color_matcher.h"
#include "../smooth_color_matcher.h"

ColorMapWidget::ColorMapWidget(const FunctionToDraw & function_to_draw,
                               const QRectF & domain,
                               const QSize & grid_size,
                               const std::vector<QRgb> & colors)
        : function_to_draw(function_to_draw),
          coordinates_converter(new CoordinatesConverter(domain, size())),
          color_map_painter(function_to_draw, coordinates_converter),
          isoline_painter(function_to_draw, coordinates_converter, grid_size),
          draw_grid(false),
          interpolate_colors(false) {
    color_matchers[0] = std::shared_ptr<ColorMatcher>(new PlainColorMatcher(function_to_draw,
                                                                            coordinates_converter,
                                                                            colors));
    color_matchers[1] = std::shared_ptr<ColorMatcher>(new SmoothColorMatcher(function_to_draw,
                                                                             coordinates_converter,
                                                                             colors));
    update_images();
    setMouseTracking(true);
}

void ColorMapWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(0, 0, shown_image.to_QImage());
    event->accept();
}

void ColorMapWidget::resizeEvent(QResizeEvent * event) {
    coordinates_converter->set_screen_size(event->size());
    for (auto & matcher : color_matchers) {
        matcher->recalc_min_max(function_to_draw, coordinates_converter);
    }
    update_images();
    event->accept();
}

void ColorMapWidget::leaveEvent(QEvent * event) {
    status_enabled = false;
    emit clear_status();
    event->accept();
}

void ColorMapWidget::enterEvent(QEvent * event) {
    status_enabled = true;
    event->accept();
}

void ColorMapWidget::mouseMoveEvent(QMouseEvent * event) {
    if (status_enabled) {
        QPointF pos = (*coordinates_converter)(event->pos());
        float value = function_to_draw(pos);
        QString status = QString("Position: (%1, %2) Value: %3")
                .arg(QString::number(pos.x(), 'g', 4))
                .arg(QString::number(pos.y(), 'g', 4))
                .arg(QString::number(value, 'g', 4));
        emit show_status(status, 0);
    }
    event->accept();
}

void ColorMapWidget::set_draw_grid(bool draw_grid) {
    this->draw_grid = draw_grid;
    update_images();
    update();
}

void ColorMapWidget::set_interpolate_colors(bool interpolate_colors) {
    this->interpolate_colors = interpolate_colors;
    update_images();
    update();
}

void ColorMapWidget::set_show_isolines(bool show_isolines) {
    this->show_isolines = show_isolines;
    update_images();
    update();
}

void ColorMapWidget::set_colors(const std::vector<QRgb> & colors) {
    for (auto & matcher : color_matchers) {
        matcher->set_colors(colors);
    }
    update_images();
    update();
}

void ColorMapWidget::set_domain(const QRectF & domain) {
    coordinates_converter->set_world_domain(domain);
    update_images();
    update();
}

void ColorMapWidget::update_images() {
    color_map = color_map_painter(color_matchers[interpolate_colors]);
    shown_image = color_map;
    if (draw_grid) {
        isoline_painter.paint_grid(shown_image);
    }
    if (show_isolines) {
        for (float isoline_level : isoline_levels) {
            isoline_painter(shown_image, isoline_level);
        }
    }
}
