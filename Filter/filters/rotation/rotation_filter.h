#ifndef ROTATION_FILTER_H
#define ROTATION_FILTER_H

#include "../abstract_filter.h"

class RotationFilter : public AbstractFilter {
public:
    RotationFilter(int angle_degrees, int max_width, int max_height);

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    void rotate_vector(float x, float y, float * u, float * v) const;

    const float sin_val;
    const float cos_val;
    const int max_width;
    const int max_height;

    static const float pi;
    static const QRgb bg_color;
};

#endif //ROTATION_FILTER_H
