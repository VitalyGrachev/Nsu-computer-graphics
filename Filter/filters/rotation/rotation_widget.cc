#include "rotation_widget.h"
#include "rotation_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool rotation1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<RotationWidget>(std::string("Rotation"));
bool rotation2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("Rotation"));

}

RotationWidget::RotationWidget() {
    set_params_widget(angle_setter = new ValueSetter(tr("Angle in degrees"), -180, 180));
    angle_setter->set_value(0);

    connect(angle_setter, SIGNAL(value_changed(int)),
            this, SLOT(apply_filter()));
}

void RotationWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<AbstractFilter> filter(new RotationFilter(angle_setter->value()));
    emit filtrate(filter, input_image, reserve_next_op_id());
}
