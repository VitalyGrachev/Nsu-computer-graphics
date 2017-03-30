#ifndef MEDIAN_BLUR_FILTER_H
#define MEDIAN_BLUR_FILTER_H

#include "../abstract_filter.h"

class MedianBlurFilter : public AbstractFilter {
public:
    MedianBlurFilter(int odd_size);

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    int size;
};

#endif //MEDIAN_BLUR_FILTER_H
