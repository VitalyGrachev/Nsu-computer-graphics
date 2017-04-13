#ifndef LEGEND_WIDGET_H
#define LEGEND_WIDGET_H

#include <QWidget>
#include "../color_matcher.h"
#include "../image_wrapper.h"
#include "../color_map_painter.h"

class LegendWidget : public QWidget {
Q_OBJECT
public:
    LegendWidget(const std::vector<QRgb> & colors, const IsolineLevelProvider & isoline_lvl_provider);

    LegendWidget(const LegendWidget &) = delete;

    ~LegendWidget() = default;

    LegendWidget & operator=(const LegendWidget &) = delete;

protected:
    virtual void paintEvent(QPaintEvent * event) override;

    virtual void resizeEvent(QResizeEvent * event) override;

public slots:

    void set_interpolate_colors(bool interpolate_colors = true);

    void set_colors(const std::vector<QRgb> & colors);

private:
    void update_image();

    ImageWrapper shown_image;
    std::shared_ptr<CoordinatesConverter> coordinates_converter;
    std::array<std::shared_ptr<ColorMatcher>, 2> color_matchers;
    ColorMapPainter color_map_painter;
    IsolineLevelProvider isoline_lvl_provider;
    bool interpolate_colors;
};


#endif //LEGEND_WIDGET_H
