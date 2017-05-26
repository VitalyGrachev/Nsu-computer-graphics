#include "plain_segment_object.h"

template<class InputIterator>
class IteratorSegmentProvider : public BaseObject::SegmentProvider {
public:
    IteratorSegmentProvider(InputIterator start, InputIterator end)
            : current(start), end(end) {}

    ~IteratorSegmentProvider() = default;

    bool has_next() const override {
        return current != end;
    }

    Segment next() override {
        if(current == end) {
            throw std::logic_error("Next can be called only if has_next() returns true.");
        }
        Segment segment = *current;
        ++current;
        return segment;
    }

private:
    InputIterator current;
    InputIterator end;
};

PlainSegmentObject::PlainSegmentObject(QRgb color) {
    set_color(color);
}

void PlainSegmentObject::add_segment(const Segment & segment) {
    segments_container.push_back(segment);
    recalculate_bounds(segment.point1);
    recalculate_bounds(segment.point2);
}

BaseObject::SegmentProvider * PlainSegmentObject::get_segment_provider() const {
    using IteratorType = std::vector<Segment>::const_iterator;
    return new IteratorSegmentProvider<IteratorType>(std::begin(segments_container),
                                                     std::end(segments_container));
}
