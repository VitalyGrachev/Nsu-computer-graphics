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
            should_draw = state < 4;
            state = (state + 1) % 8;
        }
        return should_draw;
    }

private:
    LineType line_type;
    uint8_t state;
};

union RGBA32 {
    QRgb rgb;
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGBA32() = default;

    RGBA32(const QRgb & rgb) : rgb(rgb) {}

    RGBA32(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

}

ImageWrapper::ImageWrapper()
        : image(0, 0, QImage::Format_Invalid) {
}

ImageWrapper::ImageWrapper(const QImage & image)
        : image(image) {
}

void ImageWrapper::swap(ImageWrapper & other) {
    image.swap(other.image);
}

bool ImageWrapper::texture_lookup(float u, float v, QRgb * output_color) {
    const int iu = static_cast<int>(u);
    const int iv = static_cast<int>(v);

    if (iu < 0 || iv < 0 || u > image.width() - 1 || v > image.height() - 1) {
        return false;
    }

    const float fu = u - iu;
    const float fv = v - iv;

    const RGBA32 lt = this->(iu, iv);
    const RGBA32 lb = (contains(iu, iv + 1) ? this->(iu, iv + 1) : lt);
    const RGBA32 rt = (contains(iu + 1, iv) ? this->(iu + 1, iv) : lt);
    const RGBA32 rb = (contains(iu + 1, iv + 1) ? this->(iu + 1, iv + 1) : lt);

    RGBA32 result;
    result.r = fv * (fu * lt.r + (1.0f - fu) * rt.r) +
               (1.0f - fv) * (fu * lb.r + (1.0f - fu) * rb.r);
    result.g = fv * (fu * lt.g + (1.0f - fu) * rt.g) +
               (1.0f - fv) * (fu * lb.g + (1.0f - fu) * rb.g);
    result.b = fv * (fu * lt.b + (1.0f - fu) * rt.b) +
               (1.0f - fv) * (fu * lb.b + (1.0f - fu) * rb.b);
    result.a = fv * (fu * lt.a + (1.0f - fu) * rt.a) +
               (1.0f - fv) * (fu * lb.a + (1.0f - fu) * rb.a);

    *(output_color) = result.rgb;
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
    QRgb * pixel = &this->(x1, y);
    for (int x = x1; x <= x2; ++x) {
        if (helper.should_draw_pixel()) {
            *pixel = color;
        }
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
    QRgb * pixel = &this->(x, y1);
    for (int y = y1; y <= y2; ++y) {
        if (helper.should_draw_pixel()) {
            *pixel = color;
        }
        pixel = reinterpret_cast<QRgb *>(reinterpret_cast<uchar *>(pixel) + image.bytesPerLine());
    }
}

void ImageWrapper::draw_line(int x1, int y1, int x2, int y2,
                             const QRgb & color, LineType type) {
    if (x1 == x2) {
        draw_vertical_line(x1, y1, y2, color, type);
    } else if (y1 == y2) {
        draw_horizontal_line(x1, x2, y1, color, type);
    } else {
        // TODO: add bresenham here
    }
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
        rect.x() + rect.width() >= width() || rect.y() + rect.height() >= height()) {
        throw std::invalid_argument("Rect larger than image.");
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
        left_top_x + to_insert.width() >= width() ||
        left_top_y + to_insert.height() >= height()) {
        throw std::invalid_argument("Inserted image larger than acceptor image.");
    }

    for (int y = 0; y < to_insert.height(); ++y) {
        for (int x = 0; x < to_insert.width(); ++x) {
            this->(left_top_x + x, left_top_y + y) = to_insert(x, y);
        }
    }
}

ImageWrapper ImageWrapper::copy(const QRect & rect) {
    if (rect.isNull()) {
        return *this;
    }
    if (rect.x() < 0 || rect.y() < 0 ||
        rect.x() + rect.width() >= width() || rect.y() + rect.height() >= height()) {
        throw std::invalid_argument("Rect larger than image.");
    }

    ImageWrapper copied(QImage(rect.width(), rect.height(), image.format()));
    for (int y = 0; y < rect.height(); ++y) {
        for (int x = 0; x < rect.width(); ++x) {
            copied(x, y) = this->(rect.x + x, rect.y + y);
        }
    }
    return copied;
}