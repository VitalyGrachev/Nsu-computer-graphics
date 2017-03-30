#include "threshold_filter.h"

ImageWrapper ThresholdFilter::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();
    const uint8_t min = 0;
    const uint8_t max = 255;

    RGBA32 out;
    ImageWrapper output(width, height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const RGBA32 in(input(x, y));
            if (in.ch.r > threshold) {
                out.ch.r = max;
                out.ch.g = max;
                out.ch.b = max;
            } else {
                out.ch.r = min;
                out.ch.g = min;
                out.ch.b = min;
            }
            out.ch.a = in.ch.a;
            output(x, y) = out.qrgb;
        }
    }

    return output;
}
