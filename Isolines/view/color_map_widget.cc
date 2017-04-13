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
          isoline_painter(function_to_draw, coordinates_converter, grid_size) {
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
    QPointF pos = (*coordinates_converter)(event->pos());
    float value = function_to_draw(pos);
    if (status_enabled) {
        QString status = QString("Position: (%1, %2) Value: %3")
                .arg(QString::number(pos.x(), 'g', 4))
                .arg(QString::number(pos.y(), 'g', 4))
                .arg(QString::number(value, 'g', 6));
        emit show_status(status, 0);
    }
//    if (show_isolines && dynamic_isoline) {
//        shown_image = color_map;
//        if(!isoline_levels.empty()) {
//            isoline_levels.pop_back();
//        }
//        isoline_levels.push_back(value);
//        draw_isolines();
//        update();
//    }
    event->accept();
}

void ColorMapWidget::mousePressEvent(QMouseEvent * event) {
    if (show_isolines && event->button() == Qt::LeftButton) {
        dynamic_isoline = true;
//        QPointF pos = (*coordinates_converter)(event->pos());
//        float value = function_to_draw(pos);
//        isoline_levels.push_back(value);
//        update_images();
//        draw_isolines();
//        update();
    }
    event->accept();
}

void ColorMapWidget::mouseReleaseEvent(QMouseEvent * event) {
    if (show_isolines && event->button() == Qt::LeftButton) {
        dynamic_isoline = false;
        QPointF pos = (*coordinates_converter)(event->pos());
        float value = function_to_draw(pos);
        isoline_levels.push_back(value);
        update_images();
        update();
    }
    event->accept();
};

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
    if(!show_isolines) {
        isoline_levels.clear();
    }
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
    for (auto & matcher : color_matchers) {
        matcher->recalc_min_max(function_to_draw, coordinates_converter);
    }
    update_images();
    update();
}

void ColorMapWidget::set_grid_size(const QSize & grid_size) {
    isoline_painter.set_grid_size(grid_size);
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
        draw_isolines();
    }
}

void ColorMapWidget::draw_isolines() {
    IsolineLevelProvider ilp = color_matchers[0]->get_isoline_level_provider();
    for (int i = 1; i < color_matchers[0]->color_number(); ++i) {
        isoline_painter(shown_image, ilp[i]);
    }
    for (float isoline_level : isoline_levels) {
        isoline_painter(shown_image, isoline_level);
    }
}

QRectF ColorMapWidget::get_domain() const {
    return coordinates_converter->get_world_domain();
}

QSize ColorMapWidget::get_grid_size() const {
    return isoline_painter.get_grid_size();
}

IsolineLevelProvider ColorMapWidget::get_isoline_level_provider() const {
    return color_matchers[0]->get_isoline_level_provider();
}
