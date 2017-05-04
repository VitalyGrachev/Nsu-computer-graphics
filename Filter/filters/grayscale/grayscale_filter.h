#ifndef GRAYSCALE_FILTER_H
#define GRAYSCALE_FILTER_H

#include "../abstract_filter.h"

class GrayScaleFilter : public AbstractFilter {
public:
    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    static const float red_coefficient;
    static const float green_coefficient;
    static const float blue_coefficient;
};


#endif //GRAYSCALE_FILTER_H
