#include "gamma_correction_filter.h"

ImageWrapper GammaCorrectionFilter::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();

    ImageWrapper output(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const RGBA32 in(input(x, y));
            const RGBA32 out(in.ch.a,
                             corrected_value(in.ch.r),
                             corrected_value(in.ch.g),
                             corrected_value(in.ch.b));
            output(x, y) = out.qrgb;
        }
    }

    return output;
}
