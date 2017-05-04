#ifndef GAMMA_CORRECTION_FILTER_H
#define GAMMA_CORRECTION_FILTER_H

#include <cmath>
#include "../abstract_filter.h"

class GammaCorrectionFilter : public AbstractFilter {
public:
    GammaCorrectionFilter(float gamma) : gamma(gamma) {}

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    float corrected_value(uint8_t value) const;

    const float gamma;
};

inline float GammaCorrectionFilter::corrected_value(uint8_t value) const {
    return 255.0 * std::pow(value / 255.0, gamma);
}

#endif //GAMMA_CORRECTION_FILTER_H
