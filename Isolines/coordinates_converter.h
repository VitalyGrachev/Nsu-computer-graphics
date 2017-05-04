#ifndef COORDINATES_CONVERTER_H
#define COORDINATES_CONVERTER_H

#include <QRect>

class CoordinatesConverter {
public:
    CoordinatesConverter(const QRectF & world_domain, const QSize & screen_size);

    CoordinatesConverter(const CoordinatesConverter &) = default;

    CoordinatesConverter(CoordinatesConverter &&) = default;

    ~CoordinatesConverter() = default;

    CoordinatesConverter & operator=(const CoordinatesConverter &) = default;

    CoordinatesConverter & operator=(CoordinatesConverter &&) = default;

    void set_screen_size(const QSize & size);

    void set_world_domain(const QRectF & domain);

    const QSize & get_screen_size() const { return screen_size; }

    const QRectF & get_world_domain() const { return world_domain; }

    QPointF operator()(const QPointF & screen) const;

    QPointF operator()(const QPoint & screen) const;

    QPointF operator()(float x, float y) const;

    QPointF operator()(int x, int y) const;

private:
    QRectF world_domain;
    QSize screen_size;
    float world_step_x;
    float world_step_y;
};

inline QPointF CoordinatesConverter::operator()(const QPointF & screen) const {
    return operator()(static_cast<float>(screen.x()), static_cast<float>(screen.y()));
}

inline QPointF CoordinatesConverter::operator()(const QPoint & screen) const {
    return operator()(screen.x(), screen.y());
}

inline QPointF CoordinatesConverter::operator()(float x, float y) const {
    const float xx = world_domain.x() + world_step_x * (x + 0.5f);
    const float yy = world_domain.y() + world_step_y * (screen_size.height() - y + 0.5f);
    return QPointF(xx, yy);
}

inline QPointF CoordinatesConverter::operator()(int x, int y) const {
    return operator()(static_cast<float>(x), static_cast<float>(y));
}

#endif //COORDINATES_CONVERTER_H
