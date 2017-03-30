#ifndef NEGATIVE_WIDGET_H
#define NEGATIVE_WIDGET_H

#include "../base_filter_widget.h"

class NegativeWidget : public BaseFilterWidget {
public:
    void apply_filter(ImageWrapper input_image) override;
};


#endif //NEGATIVE_WIDGET_H
