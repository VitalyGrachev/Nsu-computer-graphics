#ifndef GAMMA_CORRECTION_WIDGET_H
#define GAMMA_CORRECTION_WIDGET_H

#include "../base_filter_widget.h"
#include "../../view/value_setter.h"

class GammaCorrectionWidget : public BaseFilterWidget {
public:
    GammaCorrectionWidget();

    void apply_filter(ImageWrapper input_image) override;

private:
    ValueSetter * gamma_setter;
};


#endif //GAMMA_CORRECTION_WIDGET_H
