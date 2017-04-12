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
}

void ColorMapWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(0, 0, shown_image.to_QImage());
    event->accept();
}

void ColorMapWidget::resizeEvent(QResizeEvent * event) {
    QWidget::resizeEvent(event);
    coordinates_converter->set_screen_size(event->size());
    for (auto & matcher : color_matchers) {
        matcher->recalc_min_max(function_to_draw, coordinates_converter);
    }
    update_images();
    event->accept();
}

void ColorMapWidget::set_colors(const std::vector<QRgb> & colors) {
    for (auto & matcher : color_matchers) {
        matcher->set_colors(colors);
    }
}

void ColorMapWidget::set_draw_grid(bool draw_grid) {
    this->draw_grid = draw_grid;
}

void ColorMapWidget::set_interpolate_colors(bool interpolate_colors) {
    this->interpolate_colors = interpolate_colors;
}

void ColorMapWidget::set_domain(const QRectF & domain) {
    coordinates_converter->set_world_domain(domain);
}

void ColorMapWidget::update_images() {
    color_map = color_map_painter(color_matchers[interpolate_colors]);
    shown_image = color_map;
    if (draw_grid) {
        isoline_painter.paint_grid(shown_image);
    }
    for (float isoline_level : isoline_levels) {
        isoline_painter(shown_image, isoline_level);
    }
}
