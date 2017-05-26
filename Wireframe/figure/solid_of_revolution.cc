#include "solid_of_revolution.h"

class SolidOfRevolutionSegmentProvider : public BaseObject::SegmentProvider {
public:
    SolidOfRevolutionSegmentProvider(const std::vector<QPointF> & generatrix, int number_of_sectors)
            : segments(generatrix.size() - 1),
              delta_in_degrees(360.0 / number_of_sectors),
              number_of_sectors(number_of_sectors) {
        for (int i = 0; i < segments.size(); ++i) {
            const QVector4D pt1(generatrix[i].x(), generatrix[i].y(), 0.0, 1.0);
            const QVector4D pt2(generatrix[i + 1].x(), generatrix[i + 1].y(), 0.0, 1.0);
            segments[i] = Segment(pt1, pt2);
        }
        next_rotation.rotate(delta_in_degrees, 0.0, 1.0, 0.0);
    }

    ~SolidOfRevolutionSegmentProvider() {}

    bool has_next() const override {
        return cur_sector < number_of_sectors;
    }

    Segment next() override {
        const Segment & segment = segments[cur_segment];
        QVector4D pt1;
        QVector4D pt2;
        if (top_horiz) {
            pt1 = cur_rotation * segment.point1;
            pt2 = next_rotation * segment.point1;
            top_horiz = false;
        } else {
            pt1 = vertical ? cur_rotation * segment.point1 : next_rotation * segment.point2;
            pt2 = cur_rotation * segment.point2;
            if (!vertical) {
                ++cur_segment;
            }
            vertical = !vertical;
            if (cur_segment == segments.size()) {
                ++cur_sector;
                cur_segment = 0;
                top_horiz = true;
                if (cur_sector < number_of_sectors) {
                    cur_rotation = next_rotation;
                    next_rotation.setToIdentity();
                    next_rotation.rotate(delta_in_degrees * (cur_sector + 1), 0.0, 1.0, 0.0);
                }
            }
        }
        return Segment(pt1, pt2);
    }

private:
    std::vector<Segment> segments;
    QMatrix4x4 cur_rotation;
    QMatrix4x4 next_rotation;
    double delta_in_degrees;
    int number_of_sectors;
    int cur_segment = 0;
    int cur_sector = 0;
    bool vertical = true;
    bool top_horiz = true;
};

SolidOfRevolution::SolidOfRevolution(int number_of_sectors)
        : number_of_sectors(number_of_sectors) {
}

void SolidOfRevolution::add_point(const QPointF & point) {
    curve.push_back(point);
    recalculate_bounds(QVector4D(point.x(), point.y(), 0.0, 1.0));
    recalculate_bounds(QVector4D(-point.x(), point.y(), 0.0, 1.0));
    recalculate_bounds(QVector4D(0.0, point.y(), point.x(), 1.0));
    recalculate_bounds(QVector4D(0.0, point.y(), -point.x(), 1.0));
}

BaseObject::SegmentProvider * SolidOfRevolution::get_segment_provider() const {
    return new SolidOfRevolutionSegmentProvider(curve, number_of_sectors);
}
