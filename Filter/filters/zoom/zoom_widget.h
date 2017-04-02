#ifndef SCALING_WIDGET_H
#define SCALING_WIDGET_H

#include "../base_filter_widget.h"

class ZoomWidget : public BaseFilterWidget {
public:
    void apply_filter(ImageWrapper input_image) override;
};


#endif //SCALING_WIDGET_H
