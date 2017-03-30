#include "emboss_widget.h"
#include "emboss_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool emboss1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<EmbossWidget>(std::string("Emboss"));
bool emboss2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("Emboss"));

}

void EmbossWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<EmbossFilter> filter(new EmbossFilter());
    emit filtrate(filter, input_image, reserve_next_op_id());
}
