#include "negative_widget.h"

#include "negative_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool negative1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<NegativeWidget>(std::string("Negative"));
bool negative2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("Negative"));

}

void NegativeWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<AbstractFilter> filter(new NegativeFilter());
    emit filtrate(filter, input_image, reserve_next_op_id());
}
