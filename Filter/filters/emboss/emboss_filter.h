#ifndef EMBOSS_FILTER_H
#define EMBOSS_FILTER_H

#include "../abstract_filter.h"

class EmbossFilter : public AbstractFilter {
public:
    ImageWrapper operator()(const ImageWrapper & input) override;
};

#endif //EMBOSS_FILTER_H
