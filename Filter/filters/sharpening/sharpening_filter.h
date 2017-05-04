#ifndef SHARPENING_FILTER_H
#define SHARPENING_FILTER_H


#include "../abstract_filter.h"

class SharpeningFilter : public AbstractFilter {
public:
    enum class Connectivity {
        Four = 4, Eight = 8
    };

    SharpeningFilter(Connectivity conn = Connectivity::Eight)
            : connectivity(conn) {}

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    const Connectivity connectivity;
};


#endif //SHARPENING_FILTER_H
