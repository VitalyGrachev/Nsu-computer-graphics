#ifndef COLOR_MATCHER_H
#define COLOR_MATCHER_H

#include <QRgb>
#include <QRectF>
#include <memory>
#include "function_types.h"
#include "coordinates_converter.h"

class IsolineLevelProvider {
public:
    IsolineLevelProvider(float & min, float & max, float & step)
            : min(min), max(max), step(step) {}

    IsolineLevelProvider(const IsolineLevelProvider &) = default;

    IsolineLevelProvider & operator=(const IsolineLevelProvider &) = default;

    ~IsolineLevelProvider() = default;

    float operator[](int idx) const { return std::min(min + step * idx, max); }

private:
    const float & min;
    const float & max;
    const float & step;
};

class ColorMatcher {
public:
    ColorMatcher(const FunctionToDraw & function_to_draw,
                 std::shared_ptr<CoordinatesConverter> converter,
                 const std::vector<QRgb> & colors);

    ColorMatcher(float min, float max, const std::vector<QRgb> & colors);

    void recalc_min_max(const FunctionToDraw & function_to_draw,
                        std::shared_ptr<CoordinatesConverter> converter);

    void set_colors(const std::vector<QRgb> & colors);

    virtual QRgb operator()(float function_value) const = 0;

    int color_number() const { return colors.size(); }

    IsolineLevelProvider get_isoline_level_provider();

protected:
    std::vector<QRgb> colors;
    float min;
    float max;
    float step;
};

#endif //COLOR_MATCHER_H
