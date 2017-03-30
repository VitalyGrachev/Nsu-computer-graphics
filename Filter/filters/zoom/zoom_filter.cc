#include "zoom_filter.h"

#include <QColor>

const QRgb ZoomFilter::bg_color = QColor(255, 255, 255).rgb();

ImageWrapper ZoomFilter::operator()(const ImageWrapper & input) {
    if(scale_factor == 1.0f) {
        return input;
    }

    const int width = scale_factor * input.width();
    const int height = scale_factor * input.height();
    ImageWrapper output(width, height);

//    if(scale_factor > 1.0f) {
        const float reverse_scale_factor = 1.0f / scale_factor;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if(!input.texture_lookup(x * reverse_scale_factor, y * reverse_scale_factor, &output(x, y))) {
                    output(x, y) = bg_color;
                }
            }
        }
//    } else {
        // TODO: write it
//    }

    return output;
}
