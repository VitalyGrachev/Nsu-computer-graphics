#include "plain_color_matcher.h"

PlainColorMatcher::PlainColorMatcher(const FunctionToDraw & function_to_draw,
                                     std::shared_ptr<CoordinatesConverter> converter,
                                     const std::vector<QRgb> & colors)
        : ColorMatcher(function_to_draw, converter, colors) {}

PlainColorMatcher::PlainColorMatcher(float min, float max, const std::vector<QRgb> & colors)
        : ColorMatcher(min, max, colors) {}

QRgb PlainColorMatcher::operator()(float function_value) const {
    const int color_idx = static_cast<int>((function_value - min) / step);
    return colors[color_idx];
}
