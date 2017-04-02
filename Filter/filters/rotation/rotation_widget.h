#ifndef ROTATION_WIDGET_H
#define ROTATION_WIDGET_H

#include "../base_filter_widget.h"
#include "../../view/value_setter.h"

class RotationWidget : public BaseFilterWidget {
public:
    RotationWidget();

    virtual void apply_filter(ImageWrapper input_image) override;

private:
    ValueSetter * angle_setter;
};

#endif //ROTATION_WIDGET_H
