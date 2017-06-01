#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include <QColor>
#include "plain_segment_object.h"

class Parallelepiped : public PlainSegmentObject {
public:
    Parallelepiped(double width, double height, double length, QRgb color = QColor(0, 0, 0).rgb());

    void set_dimensions(double width, double height, double length);

    void set_width(double width);

    void set_height(double height);

    void set_length(double length);

private:
    void change_dimensions();

    double half_width;
    double half_height;
    double half_length;
};

#endif //PARALLELEPIPED_H
