#include "legend_widget.h"

#include <QPainter>
#include <QPaintEvent>
#include "../plain_color_matcher.h"
#include "../smooth_color_matcher.h"

LegendWidget::LegendWidget(const std::vector<QRgb> & colors, const IsolineLevelProvider & isoline_lvl_provider)
        : coordinates_converter(new CoordinatesConverter(QRectF(0.0f, 0.0f, 1.0f, 1.0f),
                                                         QSize(size().width() - 20, 20))),
          color_map_painter([](const QPointF & pt) { return pt.x(); }, coordinates_converter),
          isoline_lvl_provider(isoline_lvl_provider),
          interpolate_colors(false) {
    set_colors(colors);
    setFixedHeight(60);
    setMinimumWidth(60);
    update_image();
}

void LegendWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(15, 15, shown_image.to_QImage());

    painter.setFont(QFont("Arial", 8));
    const int color_count = color_matchers[0]->color_number();
    const int diff = shown_image.width() / color_count;
    for (int i = 1; i < color_count; ++i) {
        QString level = QString::number(isoline_lvl_provider[i], 'g', 5);
        QRect rect(i * diff, 35, 40, 10);
        painter.drawText(rect, Qt::AlignHCenter, level);
    }
    event->accept();
}

void LegendWidget::resizeEvent(QResizeEvent * event) {
    QSize size = QSize(event->size().width() - 20, 20);
    coordinates_converter->set_screen_size(size);
    update_image();
    event->accept();
}

void LegendWidget::set_interpolate_colors(bool interpolate_colors) {
    this->interpolate_colors = interpolate_colors;
    update_image();
}

void LegendWidget::set_colors(const std::vector<QRgb> & colors) {
    color_matchers[0] = std::shared_ptr<ColorMatcher>(new PlainColorMatcher(0.0f, 1.0f, colors));
    color_matchers[1] = std::shared_ptr<ColorMatcher>(new SmoothColorMatcher(0.0f, 1.0f, colors));
    update_image();
}

void LegendWidget::update_image() {
    shown_image = color_map_painter(color_matchers[interpolate_colors]);
    update();
}
