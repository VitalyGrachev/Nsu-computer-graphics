#include "rotation_filter.h"

#include <cmath>
#include <QColor>

const float RotationFilter::pi = 3.14152f;
const QRgb RotationFilter::bg_color = QColor(255, 255, 255).rgb();

void RotationFilter::rotate_vector(float x, float y, float * u, float * v) const {
    *u = x * std::cos(angle_rad) - y * std::sin(angle_rad);
    *v = x * std::sin(angle_rad) + y * std::cos(angle_rad);
}

ImageWrapper RotationFilter::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();

    ImageWrapper output(width, height);
    float u, v;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            rotate_vector(width / 2 - x, height / 2 - y, &u, &v);
            if (!input.texture_lookup(width / 2 - u, height / 2 - v, &output(x, y))) {
                output(x, y) = bg_color;
            }
        }
    }

    return output;
}