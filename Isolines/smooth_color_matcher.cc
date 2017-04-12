#include "smooth_color_matcher.h"
#include "image_wrapper.h"

SmoothColorMatcher::SmoothColorMatcher(const FunctionToDraw & function_to_draw,
                                       std::shared_ptr<CoordinatesConverter> converter,
                                       const std::vector<QRgb> & colors)
        : ColorMatcher(function_to_draw, converter, colors) {}

QRgb SmoothColorMatcher::operator()(float function_value) const {
    const float exact_idx = (function_value - min) / step;
    const int color_idx = static_cast<int>(exact_idx);
    if(color_idx == colors.size() - 1) {
        return colors[color_idx];
    }
    const RGBA32 c1 = colors[color_idx];
    const RGBA32 c2 = colors[color_idx + 1];
    const float factor = exact_idx - color_idx;
    RGBA32 result_color;
    result_color.ch.r = c1.ch.r * (1.0f - factor) + c2.ch.r * factor;
    result_color.ch.g = c1.ch.g * (1.0f - factor) + c2.ch.g * factor;
    result_color.ch.b = c1.ch.b * (1.0f - factor) + c2.ch.b * factor;
    result_color.ch.a = 255;
    return result_color.qrgb;
}
