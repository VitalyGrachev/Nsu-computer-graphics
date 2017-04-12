#ifndef COLOR_MAP_WIDGET_H
#define COLOR_MAP_WIDGET_H

#include <array>
#include <QWidget>
#include "../image_wrapper.h"
#include "../isoline_painter.h"
#include "../color_map_painter.h"
#include "../color_matcher.h"

class ColorMapWidget : public QWidget {
Q_OBJECT
public:
    ColorMapWidget(const FunctionToDraw & function_to_draw,
                   const QRectF & domain,
                   const QSize & grid_size,
                   const std::vector<QRgb> & colors);

    ~ColorMapWidget() = default;

    ColorMapWidget(const ColorMapWidget &) = delete;

    ColorMapWidget & operator=(const ColorMapWidget &) = delete;

signals:
    void show_status(const QString & status, int timeout);

    void clear_status();

public slots:

    void set_show_isolines(bool show_isolines = true);

    void set_draw_grid(bool draw_grid = true);

    void set_interpolate_colors(bool interpolate_colors = true);

    void set_colors(const std::vector<QRgb> & colors);

    void set_domain(const QRectF & domain);

protected:
    virtual void paintEvent(QPaintEvent * event) override;

    virtual void resizeEvent(QResizeEvent * event) override;

    virtual void mouseMoveEvent(QMouseEvent * event) override;

    virtual void enterEvent(QEvent * event) override;

    virtual void leaveEvent(QEvent * event) override;

private:
    void update_images();

    ImageWrapper shown_image;
    ImageWrapper color_map;

    FunctionToDraw function_to_draw;
    std::shared_ptr<CoordinatesConverter> coordinates_converter;
    std::array<std::shared_ptr<ColorMatcher>, 2> color_matchers;
    ColorMapPainter color_map_painter;
    IsolinePainter isoline_painter;
    std::vector<float> isoline_levels;
    bool draw_grid;
    bool interpolate_colors;
    bool show_isolines;
    bool status_enabled;
};

#endif //COLOR_MAP_WIDGET_H
