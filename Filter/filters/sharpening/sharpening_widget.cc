#include "sharpening_widget.h"

#include <QRadioButton>
#include <QGroupBox>
#include "sharpening_filter.h"
#include "../../filter_action_group_creator.h"
#include "../../factory.h"

namespace {

bool sharpening1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<SharpeningWidget>(std::string("Sharpening"));
bool sharpening2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("Sharpening"));

}

SharpeningWidget::SharpeningWidget() {

    QWidget * widget = new QWidget(this);
    QLayout * layout = new QHBoxLayout(widget);
    set_params_widget(widget);

    QGroupBox * group_box = new QGroupBox("Connectivity", this);
    QLayout * group_box_layout = new QVBoxLayout(group_box);
    button_group = new QButtonGroup(group_box);

    QRadioButton * four_connectivity = new QRadioButton(tr("4-connectivity"), this);
    group_box_layout->addWidget(four_connectivity);
    button_group->addButton(four_connectivity, 4);

    four_connectivity->setChecked(true);

    QRadioButton * eight_connectivity = new QRadioButton(tr("8-connectivity"), this);
    group_box_layout->addWidget(eight_connectivity);
    button_group->addButton(eight_connectivity, 8);

    layout->addWidget(group_box);
    connect(button_group, SIGNAL(buttonClicked(int)),
            this, SLOT(apply_filter()));
}

void SharpeningWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<SharpeningFilter> filter(
            new SharpeningFilter(static_cast<SharpeningFilter::Connectivity>(button_group->checkedId())));
    emit filtrate(filter, input_image, reserve_next_op_id());
}
