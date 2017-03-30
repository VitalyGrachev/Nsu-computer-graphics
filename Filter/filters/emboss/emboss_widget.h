#ifndef FILTER_EMBOSS_WIDGET_H
#define FILTER_EMBOSS_WIDGET_H

#include "../base_filter_widget.h"

class EmbossWidget : public BaseFilterWidget{
public:
    void apply_filter(ImageWrapper input_image) override;
};

#endif //FILTER_EMBOSS_WIDGET_H
