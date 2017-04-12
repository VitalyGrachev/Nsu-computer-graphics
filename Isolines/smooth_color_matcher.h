#ifndef SMOOTH_COLOR_MATCHER_H
#define SMOOTH_COLOR_MATCHER_H

#include "color_matcher.h"

class SmoothColorMatcher : public ColorMatcher {
public:
    SmoothColorMatcher(const FunctionToDraw & function_to_draw,
                       std::shared_ptr<CoordinatesConverter> converter,
                       const std::vector<QRgb> & colors);

    SmoothColorMatcher(float min, float max, const std::vector<QRgb> & colors);

    virtual QRgb operator()(float function_value) const override;
};

#endif //SMOOTH_COLOR_MATCHER_H
