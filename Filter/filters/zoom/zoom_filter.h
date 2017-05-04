#ifndef ZOOM_FILTER_H
#define ZOOM_FILTER_H

#include "../abstract_filter.h"

class ZoomFilter : public AbstractFilter {
public:
    ZoomFilter(float scale_factor)
            : scale_factor(scale_factor) {}

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    const float scale_factor;
    static const QRgb bg_color;
};


#endif //ZOOM_FILTER_H
