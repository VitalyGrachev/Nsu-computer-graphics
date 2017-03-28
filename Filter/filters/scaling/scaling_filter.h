#ifndef SCALING_FILTER_H
#define SCALING_FILTER_H

#include "../abstract_filter.h"

class ScalingFilter : public AbstractFilter {
public:
    ScalingFilter(int desired_width, int desired_height)
            : width(desired_width), height(desired_height) {}

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    const int width;
    const int height;
    static const QRgb bg_color;
};


#endif //SCALING_FILTER_H
