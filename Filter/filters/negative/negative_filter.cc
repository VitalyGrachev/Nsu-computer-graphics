#include "negative_filter.h"

ImageWrapper NegativeFilter::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();
    const uint8_t max_val = 255;

    ImageWrapper output(width, height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            RGBA32 pixel = input(x, y);
            pixel.ch.r = max_val - pixel.ch.r;
            pixel.ch.g = max_val - pixel.ch.g;
            pixel.ch.b = max_val - pixel.ch.b;
            output(x, y) = pixel.qrgb;
        }
    }

    return output;
}
