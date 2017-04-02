#ifndef SHARPENING_WIDGET_H
#define SHARPENING_WIDGET_H

#include <QButtonGroup>

#include "../base_filter_widget.h"

class SharpeningWidget : public BaseFilterWidget{
public:
    SharpeningWidget();

    void apply_filter(ImageWrapper input_image) override;

private:
    QButtonGroup * button_group;
};


#endif //SHARPENING_WIDGET_H
