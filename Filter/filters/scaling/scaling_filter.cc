#include "scaling_filter.h"

#include <QColor>

const QRgb ScalingFilter::bg_color = QColor(255, 255, 255).rgb();

ImageWrapper ScalingFilter::operator()(const ImageWrapper & input) {
    ImageWrapper output(width, height);
    float width_scale = static_cast<float>(input.width()) / output.width();
    float height_scale = static_cast<float>(input.height()) / output.height();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if(!input.texture_lookup(x * width_scale, y * height_scale, &output(x, y))) {
                output(x, y) = bg_color;
            }
        }
    }

    return output;
}
