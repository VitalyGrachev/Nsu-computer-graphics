#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "../visualization/base_object.h"

class PlainSegmentObject : public BaseObject {
public:
    PlainSegmentObject(QRgb color);

    ~PlainSegmentObject() = default;

    SegmentProvider * get_segment_provider() const override;

    void add_segment(const Segment & segment);

private:
    std::vector<Segment> segments_container;
};

#endif //OBJECT_H
