#include "gamma_correction_widget.h"
#include "gamma_correction_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool gamma_correction1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<GammaCorrectionWidget>(std::string("GammaCorrection"));
bool gamma_correction2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("GammaCorrection"));

}

GammaCorrectionWidget::GammaCorrectionWidget()
        : gamma_setter(new ValueSetter(tr("Gamma"), 1, 10, Orientation::Horizontal, this)) {
    set_params_widget(gamma_setter);

    connect(gamma_setter, SIGNAL(value_changed(int)),
            this, SLOT(apply_filter()));
}

void GammaCorrectionWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<GammaCorrectionFilter> filter(new GammaCorrectionFilter(gamma_setter->value()));
    emit filtrate(filter, input_image, reserve_next_op_id());
}
