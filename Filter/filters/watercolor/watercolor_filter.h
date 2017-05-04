#ifndef WATERCOLOR_FILTER_H
#define WATERCOLOR_FILTER_H

#include "../abstract_filter.h"

class WatercolorFilter : public AbstractFilter {
public:
    ImageWrapper operator()(const ImageWrapper & input) override;
};

#endif //WATERCOLOR_FILTER_H
