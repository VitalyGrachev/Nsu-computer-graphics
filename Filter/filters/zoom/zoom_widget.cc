#include "zoom_widget.h"
#include "zoom_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool zoom1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<ZoomWidget>(std::string("Zoom"));
bool zoom2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("Zoom"));

}

void ZoomWidget::apply_filter(ImageWrapper input_image) {
    const int width = std::min(input_image.width(), zone_B->width() / 2);
    const int height = std::min(input_image.height(), zone_B->height() / 2);
    const int lx = (input_image.width() - width) / 2;
    const int ty = (input_image.height() - height) / 2;
    const QRect roi(lx, ty, width, height);

    std::shared_ptr<AbstractFilter> filter(new ZoomFilter(2.0f));
    emit filtrate(filter, input_image.copy(roi), reserve_next_op_id());
}
