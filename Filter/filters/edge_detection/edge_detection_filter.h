#ifndef EDGE_DETECTION_FILTER_H
#define EDGE_DETECTION_FILTER_H

#include "../abstract_filter.h"

class EdgeDetectionFilter : public AbstractFilter {
public:
    enum class Detector {
        Roberts = 0, Sobel = 1
    };

    EdgeDetectionFilter(Detector detector, int threshold) : detector(detector), threshold(threshold) {}

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    Detector detector;
    int threshold;
};

#endif //EDGE_DETECTION_FILTER_H
