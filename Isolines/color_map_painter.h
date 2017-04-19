#ifndef COLOR_MAP_CREATOR_H
#define COLOR_MAP_CREATOR_H

#include <functional>
#include <memory>
#include "image_wrapper.h"
#include "coordinates_converter.h"
#include "function_types.h"
#include "color_matcher.h"

class ColorMapPainter {
public:
    ColorMapPainter(const FunctionToDraw & function_to_draw,
                    std::shared_ptr<CoordinatesConverter> & coordinates_converter);

    ImageWrapper operator()(std::shared_ptr<ColorMatcher> color_matcher) const;

private:
    FunctionToDraw function_to_draw;
    std::shared_ptr<CoordinatesConverter> converter;
};

#endif //COLOR_MAP_CREATOR_H
