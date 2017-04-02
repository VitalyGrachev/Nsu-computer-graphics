#ifndef THRESHOLD_FILTER_H
#define THRESHOLD_FILTER_H

#include "../abstract_filter.h"

class ThresholdFilter : public AbstractFilter {
public:
    ThresholdFilter(int threshold) : threshold(threshold) {}

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    int threshold;
};

#endif //THRESHOLD_FILTER_H
