#include "grayscale_filter.h"

const float GrayScaleFilter::red_coefficient = 0.2126f;
const float GrayScaleFilter::green_coefficient = 0.7152f;
const float GrayScaleFilter::blue_coefficient = 0.0722f;

namespace {
int min(uint8_t r, uint8_t g, uint8_t b) {
    return std::min(std::min(r, g), b);
}

int max(uint8_t r, uint8_t g, uint8_t b) {
    return std::max(std::max(r, g), b);
}
}

ImageWrapper GrayScaleFilter::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();

    ImageWrapper output(width, height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const RGBA32 in_px(input(x, y));
            float gray = red_coefficient * in_px.ch.r +
                         green_coefficient * in_px.ch.g +
                         blue_coefficient * in_px.ch.b;

            RGBA32 out_px(gray, gray, gray, in_px.ch.a);
            output(x, y) = out_px.qrgb;
        }
    }
    return output;
}
