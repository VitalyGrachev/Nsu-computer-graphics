#ifndef NEGATIVE_FILTER_H
#define NEGATIVE_FILTER_H

#include "../abstract_filter.h"

class NegativeFilter : public AbstractFilter {
public:
    ImageWrapper operator()(const ImageWrapper & input) override;
};


#endif //NEGATIVE_FILTER_H
