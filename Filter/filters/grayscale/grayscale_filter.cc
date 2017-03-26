#include "grayscale_filter.h"

const float GrayScaleFilter::red_coefficient = 0.2126f;
const float GrayScaleFilter::green_coefficient = 0.7152f;
const float GrayScaleFilter::blue_coefficient = 0.0722f;

ImageWrapper GrayScaleFilter::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();

    ImageWrapper output(QImage(width, height, input.to_QImage().format()));
    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < height; ++x) {
            const RGBA32 * in_px = reinterpret_cast<const RGBA32 *>(&input(x, y));
            float gray = red_coefficient * in_px->ch.r +
                         green_coefficient * in_px->ch.g +
                         blue_coefficient * in_px->ch.b;

            RGBA32 * out_px = reinterpret_cast<RGBA32 *>(&output(x, y));
            out_px->ch.r = gray;
            out_px->ch.g = gray;
            out_px->ch.b = gray;
        }
    }
    return output;
}
