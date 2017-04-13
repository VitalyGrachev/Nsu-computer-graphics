#include "color_matcher.h"

ColorMatcher::ColorMatcher(const FunctionToDraw & function_to_draw,
                           std::shared_ptr<CoordinatesConverter> converter,
                           const std::vector<QRgb> & colors)
        : colors(colors) {
    recalc_min_max(function_to_draw, converter);
}

ColorMatcher::ColorMatcher(float min, float max, const std::vector<QRgb> & colors)
        : colors(colors), min(min), max(max), step((max - min) / colors.size()) {}

void ColorMatcher::recalc_min_max(const FunctionToDraw & function_to_draw,
                                  std::shared_ptr<CoordinatesConverter> converter) {
    const QSize & size = converter->get_screen_size();
    float min_val = function_to_draw((*converter)(0, 0));
    float max_val = function_to_draw((*converter)(0, 0));
    for (int y = 0; y < size.height(); ++y) {
        for (int x = 0; x < size.width(); ++x) {
            min_val = std::min(min_val, function_to_draw((*converter)(x, y)));
            max_val = std::max(max_val, function_to_draw((*converter)(x, y)));
        }
    }
    this->min = min_val;
    this->max = max_val;
    this->step = (max - min) / colors.size();
}

void ColorMatcher::set_colors(const std::vector<QRgb> & colors) {
    this->colors = colors;
    this->step = (max - min) / colors.size();
}

IsolineLevelProvider ColorMatcher::get_isoline_level_provider() {
    return IsolineLevelProvider(min, max, step);
}
