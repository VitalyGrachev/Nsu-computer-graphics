#include "rules_group_box.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

RulesGroupBox::RulesGroupBox(double live_begin, double live_end,
                             double birth_begin, double birth_end,
                             double first_impact, double second_impact,
                             QWidget * parent)
        : QGroupBox(tr("Rules"), parent) {

    QLabel * first_impact_label = new QLabel(tr("First impact:"), this);
    QLabel * second_impact_label = new QLabel(tr("Second impact:"), this);
    QLabel * live_begin_label = new QLabel(tr("Live begin:"), this);
    QLabel * birth_begin_label = new QLabel(tr("Birth begin:"), this);
    QLabel * birth_end_label = new QLabel(tr("Birth end:"), this);
    QLabel * live_end_label = new QLabel(tr("Live end:"), this);

    first_impact_spin = new QDoubleSpinBox(this);
    first_impact_spin->setValue(first_impact);
    first_impact_spin->setSingleStep(0.1);
    first_impact_spin->setDecimals(1);

    second_impact_spin = new QDoubleSpinBox(this);
    second_impact_spin->setValue(second_impact);
    second_impact_spin->setSingleStep(0.1);
    second_impact_spin->setDecimals(1);

    live_begin_spin = new QDoubleSpinBox(this);
    live_begin_spin->setValue(live_begin);
    live_begin_spin->setSingleStep(0.1);
    live_begin_spin->setDecimals(1);
    live_begin_spin->setMinimum(0);
    live_begin_spin->setMaximum(birth_begin);
    connect(live_begin_spin, SIGNAL(valueChanged(double)),
            this, SLOT(live_begin_changed(double)));

    birth_begin_spin = new QDoubleSpinBox(this);
    birth_begin_spin->setValue(birth_begin);
    birth_begin_spin->setSingleStep(0.1);
    birth_begin_spin->setDecimals(1);
    birth_begin_spin->setMinimum(live_begin);
    birth_begin_spin->setMaximum(birth_end);
    connect(birth_begin_spin, SIGNAL(valueChanged(double)),
            this, SLOT(birth_begin_changed(double)));

    birth_end_spin = new QDoubleSpinBox(this);
    birth_end_spin->setValue(birth_end);
    birth_end_spin->setSingleStep(0.1);
    birth_end_spin->setDecimals(1);
    birth_end_spin->setMinimum(birth_begin);
    birth_end_spin->setMaximum(live_end);
    connect(birth_end_spin, SIGNAL(valueChanged(double)),
            this, SLOT(birth_end_changed(double)));

    live_end_spin = new QDoubleSpinBox(this);
    live_end_spin->setValue(live_end);
    live_end_spin->setSingleStep(0.1);
    live_end_spin->setDecimals(1);
    live_end_spin->setMinimum(birth_end);
    connect(live_end_spin, SIGNAL(valueChanged(double)),
            this, SLOT(live_end_changed(double)));

    QVBoxLayout * labels_layout = new QVBoxLayout();
    labels_layout->addWidget(first_impact_label);
    labels_layout->addWidget(second_impact_label);
    labels_layout->addWidget(live_begin_label);
    labels_layout->addWidget(birth_begin_label);
    labels_layout->addWidget(birth_end_label);
    labels_layout->addWidget(live_end_label);

    QVBoxLayout * spin_boxes_layout = new QVBoxLayout();
    spin_boxes_layout->addWidget(first_impact_spin);
    spin_boxes_layout->addWidget(second_impact_spin);
    spin_boxes_layout->addWidget(live_begin_spin);
    spin_boxes_layout->addWidget(birth_begin_spin);
    spin_boxes_layout->addWidget(birth_end_spin);
    spin_boxes_layout->addWidget(live_end_spin);

    QHBoxLayout * group_box_layout = new QHBoxLayout();
    group_box_layout->addLayout(labels_layout);
    group_box_layout->addLayout(spin_boxes_layout);

    setLayout(group_box_layout);
}

void RulesGroupBox::birth_begin_changed(double new_value) {
    live_begin_spin->setMaximum(new_value);
    birth_end_spin->setMinimum(new_value);
}

void RulesGroupBox::birth_end_changed(double new_value) {
    birth_begin_spin->setMaximum(new_value);
    live_end_spin->setMinimum(new_value);
}

void RulesGroupBox::live_begin_changed(double new_value) {
    birth_begin_spin->setMinimum(new_value);
}

void RulesGroupBox::live_end_changed(double new_value) {
    birth_end_spin->setMaximum(new_value);
}

