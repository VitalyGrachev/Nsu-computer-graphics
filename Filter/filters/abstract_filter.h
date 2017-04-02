#ifndef ABSTRACT_FILTER_H
#define ABSTRACT_FILTER_H

#include "../image_wrapper.h"

class AbstractFilter {
public:
    virtual ~AbstractFilter() {}

    virtual ImageWrapper operator()(const ImageWrapper & input) = 0;
};

#endif //ABSTRACT_FILTER_H
