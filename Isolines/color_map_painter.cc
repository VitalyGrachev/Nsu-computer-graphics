#include "color_map_painter.h"

ColorMapPainter::ColorMapPainter(const FunctionToDraw & function_to_draw,
                                 std::shared_ptr<CoordinatesConverter> & coordinates_converter)
        : function_to_draw(function_to_draw),
          converter(coordinates_converter) {}

ImageWrapper ColorMapPainter::operator()(std::shared_ptr<ColorMatcher> color_matcher) const {
    ImageWrapper color_map(converter->get_screen_size());

    for (int y = 0; y < color_map.height(); ++y) {
        for (int x = 0; x < color_map.width(); ++x) {
            const float value = function_to_draw((*converter)(x, y));
            color_map(x, y) = (*color_matcher)(value);
        }
    }

    return color_map;
}
