#ifndef BLUR_WIDGET_H
#define BLUR_WIDGET_H

#include "../base_filter_widget.h"

class BlurWidget : public BaseFilterWidget {
public:
    BlurWidget();

    void apply_filter(ImageWrapper input_image) override;
};


#endif //BLUR_WIDGET_H
