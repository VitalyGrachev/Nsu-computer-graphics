#ifndef SOLID_OF_REVOLUTION_H
#define SOLID_OF_REVOLUTION_H

#include "../visualization/base_object.h"

class SolidOfRevolution : public BaseObject {
public:
    SolidOfRevolution(int number_of_sectors);

    SegmentProvider * get_segment_provider() const override;

    void add_point(const QPointF & point);

private:
    std::vector<QPointF> curve;
    int number_of_sectors;
};

#endif //SOLID_OF_REVOLUTION_H
