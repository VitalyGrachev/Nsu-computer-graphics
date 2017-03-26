#include "grayscale_filter.h"

float GrayScaleFilter::red_coeff = 0.2126f;
float GrayScaleFilter::green_coeff = 0.7152f;
float GrayScaleFilter::blue_coeff = 0.0722f;

ImageWrapper GrayScaleFilter::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();

    ImageWrapper output(QImage(width, height, input.to_QImage().format()));
    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < height; ++x) {
            const RGBA32 * in_px = reinterpret_cast<const RGBA32 *>(&input(x, y));
            float gray = red_coeff * in_px->r +
                         green_coeff * in_px->g +
                         blue_coeff * in_px->b;

            RGBA32 * out_px = reinterpret_cast<RGBA32 *>(&output(x, y));
            out_px->r = gray;
            out_px->g = gray;
            out_px->b = gray;
        }
    }
    return output;
}


