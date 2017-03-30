#include "grayscale_widget.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"
#include "grayscale_filter.h"

namespace {

bool grayscale1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<GrayScaleWidget>(std::string("GrayScale"));
bool grayscale2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("GrayScale"));

}

void GrayScaleWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<AbstractFilter> filter(new GrayScaleFilter());
    emit filtrate(filter, input_image, reserve_next_op_id());
}
