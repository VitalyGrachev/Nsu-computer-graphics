#ifndef COLOR_MAP_WIDGET_H
#define COLOR_MAP_WIDGET_H

#include <array>
#include <QWidget>
#include <deque>
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

    QRectF get_domain() const;

    QSize get_grid_size() const;

    IsolineLevelProvider get_isoline_level_provider() const;

signals:

    void show_status(const QString & status, int timeout);

    void clear_status();

public slots:

    void set_show_isolines(bool show_isolines = true);

    void set_draw_grid(bool draw_grid = true);

    void set_interpolate_colors(bool interpolate_colors = true);

    void set_colors(const std::vector<QRgb> & colors);

    void set_domain(const QRectF & domain);

    void set_grid_size(const QSize & grid_size);

protected:
    virtual void paintEvent(QPaintEvent * event) override;

    virtual void resizeEvent(QResizeEvent * event) override;

    virtual void mouseMoveEvent(QMouseEvent * event) override;

    virtual void mousePressEvent(QMouseEvent * event) override;

    virtual void mouseReleaseEvent(QMouseEvent * event) override;

    virtual void enterEvent(QEvent * event) override;

    virtual void leaveEvent(QEvent * event) override;

private:
    void update_images();

    void draw_isolines();

    ImageWrapper shown_image;
    ImageWrapper color_map;

    FunctionToDraw function_to_draw;
    std::shared_ptr<CoordinatesConverter> coordinates_converter;
    std::array<std::shared_ptr<ColorMatcher>, 2> color_matchers;
    ColorMapPainter color_map_painter;
    IsolinePainter isoline_painter;
    std::deque<float> isoline_levels;
    bool draw_grid = false;
    bool interpolate_colors = false;
    bool show_isolines = false;
    bool status_enabled = true;
    bool dynamic_isoline = false;
};

#endif //COLOR_MAP_WIDGET_H
