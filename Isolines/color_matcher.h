#ifndef COLOR_MATCHER_H
#define COLOR_MATCHER_H

#include <QRgb>
#include <QRectF>
#include <memory>
#include "function_types.h"
#include "coordinates_converter.h"

class ColorMatcher {
public:
    ColorMatcher(const FunctionToDraw & function_to_draw,
                 std::shared_ptr<CoordinatesConverter> converter,
                 const std::vector<QRgb> & colors);

    void recalc_min_max(const FunctionToDraw & function_to_draw,
                        std::shared_ptr<CoordinatesConverter> converter);

    void set_colors(const std::vector<QRgb> & colors);

    virtual QRgb operator()(float function_value) const = 0;

protected:
    std::vector<QRgb> colors;
    float min;
    float max;
    float step;
};

#endif //COLOR_MATCHER_H
