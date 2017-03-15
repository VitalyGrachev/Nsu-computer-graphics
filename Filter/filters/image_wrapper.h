#ifndef IMAGE_WRAPPER_H
#define IMAGE_WRAPPER_H

#include <QImage>
#include "rgb888.h"

class ImageWrapper {
public:
    ImageWrapper(const QImage & image);

    ImageWrapper(const ImageWrapper &) = default;

    ImageWrapper(const ImageWrapper &&) = default;

    ImageWrapper & operator=(const ImageWrapper &) = default;

    ImageWrapper & operator=(const ImageWrapper &&) = default;

    ~ImageWrapper() = default;

    bool contains(int x, int y) const;

    RGB888 & operator()(int x, int y);

    const RGB888 & operator()(int x, int y) const;

    bool texture_lookup(float u, float v, RGB888 * output_color);

    QImage & get_image() { return image; }

    const QImage & get_image() const { return image; }

private:
    QImage image;
};

inline bool ImageWrapper::contains(int x, int y) const {
    return x >= 0 && y >= 0 && x < image.width() && y < image.height();
}

inline RGB888 & ImageWrapper::operator()(int x, int y) {
    RGB888 * line = reinterpret_cast<RGB888 *>(image.bits() + y * image.bytesPerLine());
    return *(line + x);
}

inline const RGB888 & ImageWrapper::operator()(int x, int y) const {
    const RGB888 * line = reinterpret_cast<const RGB888 *>(image.bits() + y * image.bytesPerLine());
    return *(line + x);
}

#endif //IMAGE_WRAPPER_H
