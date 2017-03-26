#ifndef GRAYSCALE_FILTER_H
#define GRAYSCALE_FILTER_H

#include "../abstract_filter.h"

class GrayScaleFilter : public AbstractFilter {
public:
    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    const float red_coeff;
    const float green_coeff;
    const float blue_coeff;
};


#endif //GRAYSCALE_FILTER_H
