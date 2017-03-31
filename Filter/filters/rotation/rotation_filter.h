#ifndef ROTATION_FILTER_H
#define ROTATION_FILTER_H

#include "../abstract_filter.h"

class RotationFilter : public AbstractFilter {
public:
    RotationFilter(int angle_degrees) : angle_rad(pi * angle_degrees / 180) {}

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    double angle_rad;

    void rotate_vector(float x, float y, float * u, float * v) const;

    static const float pi;
    static const QRgb bg_color;
};

#endif //ROTATION_FILTER_H
