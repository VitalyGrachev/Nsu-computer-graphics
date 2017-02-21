#ifndef CANVAS_H
#define CANVAS_H

#include <QImage>
#include <QSize>

class Canvas
{
public:
    explicit Canvas(const QSize & size);
    Canvas(int width, int height);

    const QImage & get_image() const { return image; }

    void fill(QRgb color);
    void draw_line(const QPoint & p1, const QPoint & p2, QRgb color);
private:
    QRgb * get_pixel_ptr_unsafe(int x, int y);
    void set_pixel_unsafe(int x, int y, QRgb color);
    void draw_vertical_line(int x, int y1, int y2, QRgb color);
    void draw_horizontal_line(int x1, int x2, int y, QRgb color);
    void draw_line_bresenham(int x1, int y1, int x2, int y2, QRgb color);

    QImage image;
    uchar * image_start;
    int image_bytes_per_line;
    int bytes_per_line;
    int width;
    int height;
};

inline QRgb * Canvas::get_pixel_ptr_unsafe(int x, int y) {
    return reinterpret_cast<QRgb*>(image_start + y * bytes_per_line) + x;
}

inline void Canvas::set_pixel_unsafe(int x, int y, QRgb color) {
    QRgb * pixel = get_pixel_ptr_unsafe(x, y);
    *pixel = color;
}

#endif // CANVAS_H
