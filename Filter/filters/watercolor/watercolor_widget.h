#ifndef WATERCOLOR_WIDGET_H
#define WATERCOLOR_WIDGET_H

#include "../base_filter_widget.h"

class WatercolorWidget : public BaseFilterWidget {
public:
    void apply_filter(ImageWrapper input_image) override;
};

#endif //WATERCOLOR_WIDGET_H
