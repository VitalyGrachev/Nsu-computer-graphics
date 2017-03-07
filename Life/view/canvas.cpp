#include "canvas.h"

#include <algorithm>
#include <stdexcept>

Canvas::Canvas(const QSize & size)
    : Canvas(size.width(), size.height()) {
}

Canvas::Canvas(int width, int height)
    : image(width, height, QImage::Format_RGB32),
      image_start(image.bits()),
      bytes_per_line(image.bytesPerLine()),
      width(image.width()),
      height(image.height()) {
}

void Canvas::draw_line_bresenham_unsafe(int x1, int y1, int x2, int y2, QRgb color) {
    int delta_x = std::abs(x2 - x1);
    int delta_y = std::abs(y2 - y1);
    int error = 0;
    if(delta_y > delta_x) {
        if(y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        int sign = (x1 > x2 ? -1 : 1);
        int delta_err = delta_x;
        int x = x1;
        for(int y = y1; y <= y2; ++y) {
            set_pixel_unsafe(x, y, color);
            error += delta_err;
            if(2*error >= delta_y) {
                x += sign;
                error -= delta_y;
            }
        }
    } else {
        if(x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        int sign = (y1 > y2 ? -1 : 1);
        int delta_err = delta_y;
        int y = y1;
        for(int x = x1; x <= x2; ++x) {
            set_pixel_unsafe(x, y, color);
            error += delta_err;
            if(2*error >= delta_x) {
                y += sign;
                error -= delta_x;
            }
        }
    }
}

void Canvas::fill(QRgb color) {
    uchar * image_end = image_start + image.byteCount();
    for(uchar * line_start = image_start; line_start < image_end; line_start += bytes_per_line) {
        QRgb * line = reinterpret_cast<QRgb*>(line_start);
        for(int i = 0; i < width; ++i) {
            line[i] = color;
        }
    }
}

void Canvas::draw_vertical_line(int x, int y1, int y2, QRgb color) {
    if(x < 0 || x >= width) {
        return;
    }
    if(y1 > y2) std::swap(y1, y2);
    y1 = std::max(0, y1);
    y2 = std::min(height-1, y2);

    QRgb * pixel = get_pixel_ptr_unsafe(x, y1);
    for(int y = y1; y <= y2; ++y) {
        *pixel = color;
        pixel = reinterpret_cast<QRgb*>(reinterpret_cast<uchar*>(pixel) + bytes_per_line);
    }
}

void Canvas::draw_horizontal_line(int x1, int x2, int y, QRgb color) {
    if(y < 0 || y >= height) {
        return;
    }
    if(x1 > x2) std::swap(x1, x2);
    x1 = std::max(0, x1);
    x2 = std::min(width-1, x2);

    QRgb * pixel = get_pixel_ptr_unsafe(x1, y);
    for(int x = x1; x <= x2; ++x) {
        *(pixel++) = color;
    }
}

void Canvas::draw_line(int x1, int y1, int x2, int y2, QRgb color) {
    if(x1 == x2) {
        draw_vertical_line(x1, y1, y2, color);
    } else if(y1 == y2) {
        draw_horizontal_line(x1, x2, y1, color);
    } else {
        draw_line_bresenham_unsafe(x1, y1, x2, y2, color);
    }
}
