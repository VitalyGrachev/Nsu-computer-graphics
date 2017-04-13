#include "image_wrapper.h"

#include <stdexcept>

namespace {

class LineDrawHelper {
public:
    LineDrawHelper(LineType type) : line_type(type), state(0) {}

    bool should_draw_pixel() {
        bool should_draw = false;
        if (line_type == LineType::SOLID) {
            should_draw = true;
        } else if (line_type == LineType::DOTTED) {
            should_draw = state < 3;
            state = (state + 1) % 6;
        }
        return should_draw;
    }

private:
    LineType line_type;
    uint8_t state;
};

}

ImageWrapper::ImageWrapper()
        : image() {}

ImageWrapper::ImageWrapper(int width, int height)
        : image(width, height, QImage::Format_ARGB32) {}

ImageWrapper::ImageWrapper(const QSize & size)
        : image(size, QImage::Format_ARGB32) {}

ImageWrapper::ImageWrapper(const QImage & image)
        : image(image) {}

void ImageWrapper::swap(ImageWrapper & other) {
    image.swap(other.image);
}

bool ImageWrapper::texture_lookup(float u, float v, QRgb * output_color) const {
    const int iu = static_cast<int>(u);
    const int iv = static_cast<int>(v);

    if (iu < 0 || iv < 0 || u > image.width() - 1 || v > image.height() - 1) {
        return false;
    }

    const float fu = u - iu;
    const float fv = v - iv;

    const RGBA32 lt = (*this)(iu, iv);
    const RGBA32 lb = (contains(iu, iv + 1) ? (*this)(iu, iv + 1) : lt);
    const RGBA32 rt = (contains(iu + 1, iv) ? (*this)(iu + 1, iv) : lt);
    const RGBA32 rb = (contains(iu + 1, iv + 1) ? (*this)(iu + 1, iv + 1) : lt);

    RGBA32 result;
    result.ch.r = (1.0f - fv) * (fu * rt.ch.r + (1.0f - fu) * lt.ch.r) +
                  fv * (fu * rb.ch.r + (1.0f - fu) * lb.ch.r);
    result.ch.g = (1.0f - fv) * (fu * rt.ch.g + (1.0f - fu) * lt.ch.g) +
                  fv * (fu * rb.ch.g + (1.0f - fu) * lb.ch.g);
    result.ch.b = (1.0f - fv) * (fu * rt.ch.b + (1.0f - fu) * lt.ch.b) +
                  fv * (fu * rb.ch.b + (1.0f - fu) * lb.ch.b);
    result.ch.a = (1.0f - fv) * (fu * rt.ch.a + (1.0f - fu) * lt.ch.a) +
                  fv * (fu * rb.ch.a + (1.0f - fu) * lb.ch.a);

    *(output_color) = result.qrgb;
    return true;
}

void ImageWrapper::draw_horizontal_line(int x1, int x2, int y,
                                        const QRgb & color, LineType line_type) {
    if (y < 0 || y >= image.height()) {
        return;
    }
    if (x1 > x2) std::swap(x1, x2);
    x1 = std::max(0, x1);
    x2 = std::min(image.width() - 1, x2);

    LineDrawHelper helper(line_type);
    QRgb * pixel = &(*this)(x1, y);
    for (int x = x1; x <= x2; ++x) {
        if (helper.should_draw_pixel()) {
            *pixel = color;
        }
        ++pixel;
    }
}

void ImageWrapper::draw_vertical_line(int x, int y1, int y2,
                                      const QRgb & color, LineType line_type) {
    if (x < 0 || x >= image.width()) {
        return;
    }
    if (y1 > y2) std::swap(y1, y2);
    y1 = std::max(0, y1);
    y2 = std::min(image.height() - 1, y2);

    LineDrawHelper helper(line_type);
    QRgb * pixel = &(*this)(x, y1);
    for (int y = y1; y <= y2; ++y) {
        if (helper.should_draw_pixel()) {
            *pixel = color;
        }
        pixel = reinterpret_cast<QRgb *>(reinterpret_cast<uchar *>(pixel) + image.bytesPerLine());
    }
}

void ImageWrapper::draw_line_bresenham(int x1, int y1, int x2, int y2,
                                       const QRgb & color, LineType line_type) {
    using std::swap;
    const int delta_x = std::abs(x2 - x1);
    const int delta_y = std::abs(y2 - y1);
    const int delta_err = std::min(delta_y, delta_x);
    const int err_denom = std::max(delta_y, delta_x);
    int u, v, v_min, v_max, sign, error = 0;
    const int & x = (delta_y > delta_x ? u : v);
    const int & y = (delta_y > delta_x ? v : u);

    if (delta_y > delta_x) {
        if (y1 > y2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        sign = (x1 > x2 ? -1 : 1);
        u = x1;
        v_min = y1;
        v_max = y2;
    } else {
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        sign = (y1 > y2 ? -1 : 1);
        u = y1;
        v_min = x1;
        v_max = x2;
    }

    for (v = v_min; v <= v_max; ++v) {
        (*this)(x, y) = color;
        error += delta_err;
        if (2 * error >= err_denom) {
            u += sign;
            error -= err_denom;
        }
    }
}

void ImageWrapper::draw_line(int x1, int y1, int x2, int y2,
                             const QRgb & color, LineType type) {
    if (x1 == x2) {
        draw_vertical_line(x1, y1, y2, color, type);
    } else if (y1 == y2) {
        draw_horizontal_line(x1, x2, y1, color, type);
    } else {
        draw_line_bresenham(x1, y1, x2, y2, color, type);
    }
}

void ImageWrapper::draw_line(const QPoint & pt1, const QPoint & pt2,
                             const QRgb & color, LineType type) {
    draw_line(pt1.x(), pt1.y(), pt2.x(), pt2.y(), color, type);
}

void ImageWrapper::fill(const QRgb & color) {
    uchar * line_start = image.bits();
    for (int h = 0; h < image.height(); ++h, line_start += image.bytesPerLine()) {
        QRgb * line = reinterpret_cast<QRgb *>(line_start);
        for (int i = 0; i < image.width(); ++i) {
            line[i] = color;
        }
    }
}

void ImageWrapper::fill(const QRect & rect, const QRgb & color) {
    if (rect.x() < 0 || rect.y() < 0 ||
        rect.x() + rect.width() > width() || rect.y() + rect.height() > height()) {
        const std::string msg("Fill rect larger than image. ");
        throw std::invalid_argument(msg +
                                    std::to_string(rect.x() + rect.width()) + " vs " + std::to_string(width()) + ' ' +
                                    std::to_string(rect.y() + rect.height()) + " vs " + std::to_string(height()));
    }

    uchar * line_start = image.bits() + rect.y() * image.bytesPerLine();
    for (int h = 0; h < rect.height(); ++h, line_start += image.bytesPerLine()) {
        QRgb * line = reinterpret_cast<QRgb *>(line_start) + rect.x();
        for (int i = 0; i < rect.width(); ++i) {
            line[i] = color;
        }
    }
}

void ImageWrapper::insert_image(const ImageWrapper & to_insert, int left_top_x, int left_top_y) {
    if (left_top_x < 0 || left_top_y < 0 ||
        left_top_x + to_insert.width() > width() ||
        left_top_y + to_insert.height() > height()) {
        throw std::invalid_argument("Inserted image larger than acceptor image.");
    }

    for (int y = 0; y < to_insert.height(); ++y) {
        for (int x = 0; x < to_insert.width(); ++x) {
            (*this)(left_top_x + x, left_top_y + y) = to_insert(x, y);
        }
    }
}

ImageWrapper ImageWrapper::copy(const QRect & rect) const {
    if (rect.isNull()) {
        return *this;
    }
    if (rect.x() < 0 || rect.y() < 0 ||
        rect.x() + rect.width() > width() || rect.y() + rect.height() > height()) {
        const std::string msg("Copy rect larger than image. ");
        throw std::invalid_argument(msg +
                                    std::to_string(rect.x() + rect.width()) + " vs " + std::to_string(width()) + ' ' +
                                    std::to_string(rect.y() + rect.height()) + " vs " + std::to_string(height()));
    }

    ImageWrapper copied(QImage(rect.width(), rect.height(), image.format()));
    for (int y = 0; y < rect.height(); ++y) {
        for (int x = 0; x < rect.width(); ++x) {
            copied(x, y) = (*this)(rect.x() + x, rect.y() + y);
        }
    }
    return copied;
}

