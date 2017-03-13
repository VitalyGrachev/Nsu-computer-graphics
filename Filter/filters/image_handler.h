#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H

#include <QImage>
#include "rgb888.h"

class ImageHandler {
public:
    ImageHandler(const QImage & image);

    ImageHandler(const ImageHandler &) = default;

    ImageHandler(const ImageHandler &&) = default;

    ImageHandler & operator=(const ImageHandler &) = default;

    ImageHandler & operator=(const ImageHandler &&) = default;

    ~ImageHandler() = default;

    bool contains(int x, int y) const;

    RGB888 & operator()(int x, int y);

    const RGB888 & operator()(int x, int y) const;

    bool texture_lookup(float u, float v, RGB888 * output_color);

    QImage & get_image() { return image; }

    const QImage & get_image() const { return image; }

private:
    QImage image;
};

inline bool ImageHandler::contains(int x, int y) const {
    return x >= 0 && y >= 0 && x < image.width() && y < image.height();
}

inline RGB888 & ImageHandler::operator()(int x, int y) {
    RGB888 * line = reinterpret_cast<RGB888 *>(image.bits() + y * image.bytesPerLine());
    return *(line + x);
}

inline const RGB888 & ImageHandler::operator()(int x, int y) const {
    const RGB888 * line = reinterpret_cast<const RGB888 *>(image.bits() + y * image.bytesPerLine());
    return *(line + x);
}

#endif //IMAGE_HANDLER_H
