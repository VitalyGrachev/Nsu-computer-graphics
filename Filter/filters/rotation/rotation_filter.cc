#include "rotation_filter.h"

#include <cmath>
#include <QColor>

const float RotationFilter::pi = 3.14152f;
const QRgb RotationFilter::bg_color = QColor(255, 255, 255).rgb();

RotationFilter::RotationFilter(int angle_degrees, int max_width, int max_height)
        : sin_val(std::sin(pi * angle_degrees / 180)),
          cos_val(std::cos(pi * angle_degrees / 180)),
          max_width(max_width),
          max_height(max_height) {}

void RotationFilter::rotate_vector(float x, float y, float * u, float * v) const {
    *u = x * cos_val - y * sin_val;
    *v = x * sin_val + y * cos_val;
}

ImageWrapper RotationFilter::operator()(const ImageWrapper & input) {
    using std::min;
    using std::max;
    using std::abs;

    const int in_half_width = input.width() / 2;
    const int in_half_height = input.height() / 2;

    float w1, w2, h1, h2;
    rotate_vector(in_half_width, in_half_height, &w1, &h1);
    rotate_vector(in_half_width, -in_half_height, &w2, &h2);

    const int iw1 = static_cast<int>(w1);
    const int iw2 = static_cast<int>(w2);
    const int ih1 = static_cast<int>(h1);
    const int ih2 = static_cast<int>(h2);
    const int width = min(max_width, in_half_width + max(abs(iw1), abs(iw2)));
    const int height = min(max_height, in_half_height + max(abs(ih1), abs(ih2)));

    ImageWrapper output(width, height);
    float u, v;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            rotate_vector(in_half_width - x, in_half_height - y, &u, &v);
            if (!input.texture_lookup(in_half_width - u, in_half_height - v, &output(x, y))) {
                output(x, y) = bg_color;
            }
        }
    }

    return output;
}