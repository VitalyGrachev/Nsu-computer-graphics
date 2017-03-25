#ifndef IMAGE_WRAPPER_H
#define IMAGE_WRAPPER_H

#include <QImage>
#include "QRgb.h"

enum class LineType : char {
    SOLID, DOTTED
};

class ImageWrapper {
public:
    ImageWrapper();

    ImageWrapper(const QImage & image);

    ImageWrapper(const ImageWrapper &) = default;

    ImageWrapper(const ImageWrapper &&) = default;

    ImageWrapper & operator=(const ImageWrapper &) = default;

    ImageWrapper & operator=(const ImageWrapper &&) = default;

    ~ImageWrapper() = default;

    bool contains(int x, int y) const;

    QRgb & operator()(int x, int y);

    const QRgb & operator()(int x, int y) const;

    int width() const { return image.width(); }

    int height() const { return image.height(); }

    bool texture_lookup(float u, float v, QRgb * output_color);

    QImage & to_QImage() { return image; }

    const QImage & to_QImage() const { return image; }

    void fill(const QRgb & color);

    void fill(const QRect & rect, const QRgb & color);

    void draw_line(int x1, int y1, int x2, int y2,
                   const QRgb & color, LineType type = LineType::DOTTED);

    void insert_image(const ImageWrapper & to_insert, int left_top_x, int left_top_y);

private:
    void draw_horizontal_line(int x1, int x2, int y,
                              const QRgb & color, LineType type);

    void draw_vertical_line(int x, int y1, int y2,
                            const QRgb & color, LineType line_type);

    QImage image;
};

inline bool ImageWrapper::contains(int x, int y) const {
    return x >= 0 && y >= 0 && x < image.width() && y < image.height();
}

inline QRgb & ImageWrapper::operator()(int x, int y) {
    QRgb * line = reinterpret_cast<QRgb *>(image.bits() + y * image.bytesPerLine());
    return *(line + x);
}

inline const QRgb & ImageWrapper::operator()(int x, int y) const {
    const QRgb * line = reinterpret_cast<const QRgb *>(image.bits() + y * image.bytesPerLine());
    return *(line + x);
}

#endif //IMAGE_WRAPPER_H
