#ifndef SOLID_OF_REVOLUTION_H
#define SOLID_OF_REVOLUTION_H

#include "../visualization/base_object.h"
#include "../curve/curve.h"

class SolidOfRevolution : public BaseObject {
public:
    SolidOfRevolution(Curve * curve, int sector_count);

    SegmentProvider * get_segment_provider() const override;

private:
    Curve * curve;
    int sector_count;
};

#endif //SOLID_OF_REVOLUTION_H
