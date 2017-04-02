#include "blur_widget.h"
#include "blur_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool blur1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<BlurWidget>(std::string("Blur"));
bool blur2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("Blur"));

}

BlurWidget::BlurWidget() {

}

void BlurWidget::apply_filter(ImageWrapper input_image) {
    const float f = 1.0f / 9.0f;
    SquareMatrix<float> kernel(3, {f, f, f, f, f, f, f, f, f});
    std::shared_ptr<AbstractFilter> filter(new BlurFilter(kernel));
    emit filtrate(filter, input_image, reserve_next_op_id());
}
