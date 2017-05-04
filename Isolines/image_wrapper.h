#ifndef IMAGE_WRAPPER_H
#define IMAGE_WRAPPER_H

#include <cstdint>
#include <QImage>

enum class LineType : char {
    SOLID, DOTTED
};

struct Channel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Channel() = default;

    Channel(uint8_t a, uint8_t r, uint8_t g, uint8_t b) : a(a), r(r), g(g), b(b) {}
};

union RGBA32 {
    QRgb qrgb;
    Channel ch;

    RGBA32() = default;

    RGBA32(const QRgb & qrgb) : qrgb(qrgb) {}

    RGBA32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : ch(a, r, g, b) {}
};

class ImageWrapper {
public:
    ImageWrapper();

    ImageWrapper(int width, int height);

    ImageWrapper(const QSize & size);

    ImageWrapper(const QImage & image);

    ImageWrapper(const ImageWrapper &) = default;

    ImageWrapper(ImageWrapper &&) = default;

    ImageWrapper & operator=(const ImageWrapper &) = default;

    ImageWrapper & operator=(ImageWrapper &&) = default;

    ~ImageWrapper() = default;

    void swap(ImageWrapper & other);

    bool contains(int x, int y) const;

    QRgb & operator()(int x, int y);

    const QRgb & operator()(int x, int y) const;

    int width() const { return image.width(); }

    int height() const { return image.height(); }

    bool texture_lookup(float u, float v, QRgb * output_color) const;

    QImage & to_QImage() { return image; }

    const QImage & to_QImage() const { return image; }

    void fill(const QRgb & color);

    void fill(const QRect & rect, const QRgb & color);

    void draw_line(int x1, int y1, int x2, int y2,
                   const QRgb & color, LineType type = LineType::DOTTED);

    void draw_line(const QPoint & pt1, const QPoint & pt2,
                   const QRgb & color, LineType type = LineType::DOTTED);

    ImageWrapper copy(const QRect & rect = QRect()) const;

    void insert_image(const ImageWrapper & to_insert, int left_top_x, int left_top_y);

private:
    void draw_horizontal_line(int x1, int x2, int y,
                              const QRgb & color, LineType type);

    void draw_vertical_line(int x, int y1, int y2,
                            const QRgb & color, LineType line_type);

    void draw_line_bresenham(int x1, int y1, int x2, int y2,
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
