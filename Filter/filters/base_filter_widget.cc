#include "base_filter_widget.h"

#include <QHBoxLayout>

const QSize BaseFilterWidget::button_size(60, 25);

BaseFilterWidget::BaseFilterWidget()
        : last_applied_op_id(0), last_finished_op_id(0), min_op_id(0) {
    widget_layout = new QVBoxLayout(this);
    QHBoxLayout * buttons_layout = new QHBoxLayout();
    widget_layout->addLayout(buttons_layout);
    buttons_layout->addWidget(ok_button = new QPushButton(tr("Ok")));
    buttons_layout->addWidget(cancel_button = new QPushButton(tr("Cancel")));

    ok_button->setFixedSize(button_size);
    cancel_button->setFixedSize(button_size);

    connect(ok_button, SIGNAL(clicked()),
            this, SLOT(accept()));
    connect(cancel_button, SIGNAL(clicked()),
            this, SLOT(cancel()));

    buttons_layout->setAlignment(Qt::AlignRight);
    widget_layout->setAlignment(Qt::AlignBottom);
}

void BaseFilterWidget::set_params_widget(QWidget * widget) {
    widget_layout->insertWidget(0, widget);
}

void BaseFilterWidget::filtration_finished(ImageWrapper output_image, uint64_t op_id) {
    if (op_id >= min_op_id) {
        zone_C->set_image(output_image);
    }
    last_finished_op_id = std::max(op_id, last_finished_op_id);
    if (last_finished_op_id == last_applied_op_id) {
        enable_buttons();
    }
}

void BaseFilterWidget::apply_filter() {
    this->apply_filter(zone_B->get_image());
}

void BaseFilterWidget::set_zones(Zone * zone_B, Zone * zone_C) {
    this->zone_B = zone_B;
    this->zone_C = zone_C;
}

void BaseFilterWidget::accept() {
    emit accepted();
}

void BaseFilterWidget::cancel() {
    zone_C->set_image(zone_B->get_image());
    emit cancelled();
}

void BaseFilterWidget::enable_buttons(bool enable) {
    ok_button->setEnabled(enable);
    cancel_button->setEnabled(enable);
}

uint64_t BaseFilterWidget::reserve_next_op_id() {
    enable_buttons(false);
    return ++last_applied_op_id;
}