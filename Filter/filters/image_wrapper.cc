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

}

ImageWrapper::ImageWrapper()
        : image(0, 0, QImage::Format_Invalid) {
}

ImageWrapper::ImageWrapper(const QImage & image)
        : image(image) {
}

bool ImageWrapper::texture_lookup(float u, float v, RGB888 * output_color) {
    const int iu = static_cast<int>(u);
    const int iv = static_cast<int>(v);

    if (iu < 0 || iv < 0 || u > image.width() - 1 || v > image.height() - 1) {
        return false;
    }

    const float fu = u - iu;
    const float fv = v - iv;

    const RGB888 & lt = this->(iu, iv);
    const RGB888 & lb = (contains(iu, iv + 1) ? this->(iu, iv + 1) : lt);
    const RGB888 & rt = (contains(iu + 1, iv) ? this->(iu + 1, iv) : lt);
    const RGB888 & rb = (contains(iu + 1, iv + 1) ? this->(iu + 1, iv + 1) : lt);

    const float r = fv * (fu * lt.r + (1.0f - fu) * rt.r) +
                    (1.0f - fv) * (fu * lb.r + (1.0f - fu) * rb.r);
    const float g = fv * (fu * lt.g + (1.0f - fu) * rt.g) +
                    (1.0f - fv) * (fu * lb.g + (1.0f - fu) * rb.g);
    const float b = fv * (fu * lt.b + (1.0f - fu) * rt.b) +
                    (1.0f - fv) * (fu * lb.b + (1.0f - fu) * rb.b);
    output_color->r = static_cast<uint8_t>(r);
    output_color->g = static_cast<uint8_t>(g);
    output_color->b = static_cast<uint8_t>(b);
    return true;
}

void ImageWrapper::draw_horizontal_line(int x1, int x2, int y,
                                        const RGB888 & color, LineType line_type) {
    if (y < 0 || y >= image.height()) {
        return;
    }
    if (x1 > x2) std::swap(x1, x2);
    x1 = std::max(0, x1);
    x2 = std::min(image.width() - 1, x2);

    LineDrawHelper helper(line_type);
    RGB888 * pixel = &this->(x1, y);
    for (int x = x1; x <= x2; ++x) {
        if (helper.should_draw_pixel()) {
            *pixel = color;
        }
    }
}

void ImageWrapper::draw_vertical_line(int x, int y1, int y2,
                                      const RGB888 & color, LineType line_type) {
    if (x < 0 || x >= image.width()) {
        return;
    }
    if (y1 > y2) std::swap(y1, y2);
    y1 = std::max(0, y1);
    y2 = std::min(image.height() - 1, y2);

    LineDrawHelper helper(line_type);
    RGB888 * pixel = &this->(x, y1);
    for (int y = y1; y <= y2; ++y) {
        if (helper.should_draw_pixel()) {
            *pixel = color;
        }
        pixel = reinterpret_cast<RGB888 *>(reinterpret_cast<uchar *>(pixel) + image.bytesPerLine());
    }
}

void ImageWrapper::draw_line(int x1, int y1, int x2, int y2,
                             const RGB888 & color, LineType type) {
    if (x1 == x2) {
        draw_vertical_line(x1, y1, y2, color, type);
    } else if (y1 == y2) {
        draw_horizontal_line(x1, x2, y1, color, type);
    } else {
        // TODO: add bresenham here
    }
}

void ImageWrapper::fill(const RGB888 & color) {
    uchar * line_start = image.bits();
    for (int h = 0; h < image.height(); ++h, line_start += image.bytesPerLine()) {
        RGB888 * line = reinterpret_cast<RGB888 *>(line_start);
        for (int i = 0; i < image.width(); ++i) {
            line[i] = color;
        }
    }
}

void ImageWrapper::fill(const QRect & rect, const RGB888 & color) {
    if (rect.x() < 0 || rect.y() < 0 ||
        rect.x() + rect.width() >= width() || rect.y() + rect.height() >= height()) {
        throw std::invalid_argument("Rect to fill larger than image.");
    }

    uchar * line_start = image.bits() + rect.y() * image.bytesPerLine();
    for (int h = 0; h < rect.height(); ++h, line_start += image.bytesPerLine()) {
        RGB888 * line = reinterpret_cast<RGB888 *>(line_start) + rect.x();
        for (int i = 0; i < rect.width(); ++i) {
            line[i] = color;
        }
    }
}

void ImageWrapper::insert_image(const ImageWrapper & to_insert, int left_top_x, int left_top_y) {
    const int donor_left = (left_top_x < 0 ? -left_top_x : 0);
    const int donor_top = (left_top_y < 0 ? -left_top_y : 0);
    const int excess_width = left_top_x + to_insert.width() - width();
    const int pixels_to_copy_x = to_insert.width() - (excess_width > 0 ? excess_width : 0) - donor_left;
    const int excess_height = left_top_x + to_insert.height() - height();
    const int pixels_to_copy_y = to_insert.height() - (excess_height > 0 ? excess_height : 0) - donor_top;
    left_top_x = (left_top_x < 0 ? 0 : left_top_x);
    left_top_y = (left_top_y < 0 ? 0 : left_top_y);

    for (int y = 0; y < pixels_to_copy_y; ++y) {
        for (int x = 0; x < pixels_to_copy_x; ++x) {
            this->(left_top_x + x, left_top_y + y) = to_insert(donor_left + x, donor_top + y);
        }
    }
}
