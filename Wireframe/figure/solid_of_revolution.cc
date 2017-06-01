#include "solid_of_revolution.h"

#include "../util/transform.h"

class SolidOfRevolutionSegmentProvider : public BaseObject::SegmentProvider {
public:
    SolidOfRevolutionSegmentProvider(const Curve * generatrix,
                                     int sectors_count)
            : generatrix(generatrix),
              points_count(generatrix->minor_segments_count() *
                           generatrix->major_segments_count()),
              sectors_count(sectors_count) {
        current_state = State::GeneratingVertical;
        delta_angle = 360.0 / sectors_count;
    }

    ~SolidOfRevolutionSegmentProvider() {}

    bool has_next() const override {
        return current_state != State::Finished;
    }

    Segment next() override;

private:
    Segment next_vertical();

    Segment next_horizontal();

    enum class State {
        GeneratingVertical, GeneratingHorizontal, Finished
    };

    const Curve * generatrix;
    QMatrix4x4 next_rotation;
    QMatrix4x4 rotation;
    double delta_angle;
    int points_count;
    int sectors_count;
    int current_sector = 0;
    int current_point = 0;
    State current_state;

    static const QVector3D axis;
};

const QVector3D SolidOfRevolutionSegmentProvider::axis(0, 1, 0);

Segment SolidOfRevolutionSegmentProvider::next() {
    Segment segment;
    switch (current_state) {
        case State::GeneratingVertical:
            segment = next_vertical();
            if (current_sector == sectors_count &&
                current_point == points_count) {

                current_sector = current_point = 0;
                sectors_count *= generatrix->minor_segments_count();
                points_count = generatrix->major_segments_count() + 1;
                delta_angle /= generatrix->minor_segments_count();
                rotation.setToIdentity();
                next_rotation = Transform::rotation(delta_angle, axis);
                current_state = State::GeneratingHorizontal;
            }
            break;
        case State::GeneratingHorizontal:
            segment = next_horizontal();
            if (current_sector == sectors_count &&
                current_point == points_count) {

                current_sector = current_point = 0;
                current_state = State::Finished;
            }
            break;
        default:
            throw new std::logic_error("SolidOfRevolutionSegmentProvider::next() can be called"
                                               "only when has_next() returned true.");
    }
    return segment;
}

Segment SolidOfRevolutionSegmentProvider::next_vertical() {
    const QPointF & p1 = generatrix->get_points()[current_point];
    const QPointF & p2 = generatrix->get_points()[current_point + 1];
    QVector4D first(p1.x(), p1.y(), 0, 1);
    QVector4D second(p2.x(), p2.y(), 0, 1);
    first = rotation * first;
    second = rotation * second;

    ++current_point;
    if (current_point == points_count &&
        current_sector != sectors_count) {
        current_point = 0;
        ++current_sector;
        rotation = Transform::rotation(delta_angle * current_sector, axis);
    }
    return Segment(first, second);
}

Segment SolidOfRevolutionSegmentProvider::next_horizontal() {
    const QPointF & p1 = generatrix->get_points()[current_point * generatrix->minor_segments_count()];
    QVector4D first(p1.x(), p1.y(), 0, 1);
    QVector4D second = next_rotation * first;
    first = rotation * first;

    ++current_point;
    if (current_point == points_count &&
        current_sector != sectors_count) {
        current_point = 0;
        ++current_sector;
        rotation = next_rotation;
        next_rotation = Transform::rotation(delta_angle * (current_sector+1), axis);
    }
    return Segment(first, second);
}

SolidOfRevolution::SolidOfRevolution(Curve * curve, int sector_count)
        : curve(curve), sector_count(sector_count) {
    for (const QPointF & point : curve->get_points()) {
        recalculate_bounds(QVector4D(point.x(), point.y(), 0.0, 1.0));
        recalculate_bounds(QVector4D(-point.x(), point.y(), 0.0, 1.0));
        recalculate_bounds(QVector4D(0.0, point.y(), point.x(), 1.0));
        recalculate_bounds(QVector4D(0.0, point.y(), -point.x(), 1.0));
    }
}

BaseObject::SegmentProvider * SolidOfRevolution::get_segment_provider() const {
    return new SolidOfRevolutionSegmentProvider(curve, sector_count);
}
