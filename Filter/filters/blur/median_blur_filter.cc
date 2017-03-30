#include "median_blur_filter.h"

namespace {

int clamp(int value, int min, int max) {
    return std::min(std::max(value, min), max);
}

}

MedianBlurFilter::MedianBlurFilter(int odd_size)
        : size(odd_size) {
    if (!odd_size % 2) {
        throw std::invalid_argument("Kernel size must be an odd number.");
    }
}

ImageWrapper MedianBlurFilter::operator()(const ImageWrapper & input) {
    const int half_size = size / 2;
    std::vector<int> r_values(size * size);
    std::vector<int> g_values(size * size);
    std::vector<int> b_values(size * size);

    ImageWrapper output(input.width(), input.height());
    for (int y = 0; y < input.height(); ++y) {
        for (int x = 0; x < input.width(); ++x) {
            for (int v = 0; v < size; ++v) {
                for (int u = 0; u < size; ++u) {
                    const int px = clamp(x + u - half_size, 0, input.width() - 1);
                    const int py = clamp(y + v - half_size, 0, input.height() - 1);
                    const RGBA32 in = input(px, py);
                    r_values[v * size + u] = in.ch.r;
                    g_values[v * size + u] = in.ch.g;
                    b_values[v * size + u] = in.ch.b;
                }
            }
            std::sort(r_values.begin(), r_values.end());
            std::sort(g_values.begin(), g_values.end());
            std::sort(b_values.begin(), b_values.end());
            const int median = size * size / 2;
            const RGBA32 in = input(x, y);
            RGBA32 out(r_values[median], g_values[median], b_values[median], in.ch.a);
            output(x, y) = out.qrgb;
        }
    }
    return output;
}
