#include "zoom_filter.h"

#include <QColor>

const QRgb ZoomFilter::bg_color = QColor(255, 255, 255).rgb();

ImageWrapper ZoomFilter::operator()(const ImageWrapper & input) {
    if (scale_factor == 1.0f) {
        return input;
    }

    const int width = scale_factor * input.width();
    const int height = scale_factor * input.height();
    ImageWrapper output(width, height);

    if (scale_factor > 1.0f) {
        const float reverse_scale_factor = 1.0f / scale_factor;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (!input.texture_lookup(x * reverse_scale_factor, y * reverse_scale_factor, &output(x, y))) {
                    output(x, y) = bg_color;
                }
            }
        }
    } else {
        const float real_w_sf = static_cast<float>(input.width()) / width;
        const float real_h_sf = static_cast<float>(input.height()) / height;

        const int real_w_sf_int = static_cast<int>(real_w_sf);
        const int real_h_sf_int = static_cast<int>(real_h_sf);
        const int win_w = real_w_sf_int + (real_w_sf - real_w_sf_int > 0.0f);
        const int win_h = real_h_sf_int + (real_h_sf - real_h_sf_int > 0.0f);
        const int win_size = win_w * win_h;

        for (int y = 0; y < height; ++y) {
            const int in_y = static_cast<int>(real_h_sf * y);
            for (int x = 0; x < width; ++x) {
                const int in_x = static_cast<int>(real_w_sf * x);
                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;
                float a = 0.0f;
                for (int v = 0; v < win_h; ++v) {
                    for (int u = 0; u < win_w; ++u) {
                        const RGBA32 in(input(in_x+u, in_y+v));
                        r += in.ch.r;
                        g += in.ch.g;
                        b += in.ch.b;
                        a += in.ch.a;
                    }
                }
                output(x, y) = RGBA32(a/win_size, r/win_size, g/win_size, b/win_size).qrgb;
            }
        }
    }

    return output;
}
