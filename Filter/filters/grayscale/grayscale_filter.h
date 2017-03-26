#ifndef GRAYSCALE_FILTER_H
#define GRAYSCALE_FILTER_H

#include "../abstract_filter.h"

class GrayScaleFilter : public AbstractFilter {
public:
    GrayScaleFilter() = default;

    virtual ~GrayScaleFilter() {}

    ImageWrapper operator()(const ImageWrapper & input) override;
};


#endif //GRAYSCALE_FILTER_H
