#ifndef GRAYSCALE_WIDGET_H
#define GRAYSCALE_WIDGET_H

#include "../base_filter_widget.h"

class GrayScaleWidget : public BaseFilterWidget {
public:
    virtual void apply_filter(ImageWrapper input_image) override;
};


#endif //GRAYSCALE_WIDGET_H
