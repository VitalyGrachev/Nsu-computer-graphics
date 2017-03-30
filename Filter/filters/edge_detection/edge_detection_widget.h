#ifndef EDGE_DETECTION_WIDGET_H
#define EDGE_DETECTION_WIDGET_H

#include <QButtonGroup>
#include "../base_filter_widget.h"
#include "edge_detection_filter.h"
#include "../../view/value_setter.h"

class EdgeDetectionWidget : public BaseFilterWidget {
public:
    EdgeDetectionWidget();

    void apply_filter(ImageWrapper input_image) override;

private:
    using Detector = EdgeDetectionFilter::Detector;
    QButtonGroup * button_group;
    ValueSetter * value_setter;
};

#endif //EDGE_DETECTION_WIDGET_H
