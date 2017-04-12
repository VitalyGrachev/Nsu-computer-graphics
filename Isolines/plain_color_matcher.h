#ifndef PLAIN_COLOR_MATCHER_H
#define PLAIN_COLOR_MATCHER_H

#include "color_matcher.h"

class PlainColorMatcher : public ColorMatcher {
public:
    PlainColorMatcher(const FunctionToDraw & function_to_draw,
                      std::shared_ptr<CoordinatesConverter> converter,
                      const std::vector<QRgb> & colors);

    virtual QRgb operator()(float function_value) const override;
};

#endif //PLAIN_COLOR_MATCHER_H
