#include "watercolor_widget.h"
#include "watercolor_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool watercolor1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<WatercolorWidget>(std::string("Watercolor"));
bool watercolor2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("Watercolor"));

}

void WatercolorWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<AbstractFilter> filter(new WatercolorFilter());
    emit filtrate(filter, input_image, reserve_next_op_id());
}
