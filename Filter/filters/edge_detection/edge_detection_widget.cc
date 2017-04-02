#include "edge_detection_widget.h"

#include <QtGlobal>
#include <QGroupBox>
#include <QRadioButton>
#include "edge_detection_filter.h"
#include "../../factory.h"
#include "../../filter_action_group_creator.h"

namespace {

bool edges1 = Factory<std::string, BaseFilterWidget>::instance().
        register_type<EdgeDetectionWidget>(std::string("EdgeDetection"));
bool edges2 = FilterActionGroupCreator::instance().
        add_filter_action(new FilterAction("EdgeDetection"));

}

EdgeDetectionWidget::EdgeDetectionWidget() {
    QWidget * widget = new QWidget(this);
    QLayout * layout = new QHBoxLayout(widget);

    set_params_widget(widget);

    QGroupBox * group_box = new QGroupBox(tr("Detector"), this);
    layout->addWidget(group_box);

    QLayout * group_layout = new QVBoxLayout(group_box);
    button_group = new QButtonGroup(group_box);

    QRadioButton * roberts_button = new QRadioButton(tr("Roberts"), group_box);
    button_group->addButton(roberts_button, static_cast<int>(Detector::Roberts));
    group_layout->addWidget(roberts_button);

    QRadioButton * sobel_button = new QRadioButton(tr("Sobel"), group_box);
    sobel_button->setChecked(true);
    button_group->addButton(sobel_button, static_cast<int>(Detector::Sobel));
    group_layout->addWidget(sobel_button);

    layout->addWidget(value_setter = new ValueSetter(tr("Threshold"), 0, 255));
    value_setter->set_value(60);

    connect(button_group, SIGNAL(buttonClicked(int)),
            this, SLOT(apply_filter()));
    connect(value_setter, SIGNAL(value_changed(int)),
            this, SLOT(apply_filter()));
}

void EdgeDetectionWidget::apply_filter(ImageWrapper input_image) {
    std::shared_ptr<EdgeDetectionFilter> filter(
            new EdgeDetectionFilter(static_cast<Detector>(button_group->checkedId()), value_setter->value()));
    emit filtrate(filter, input_image, reserve_next_op_id());
}
