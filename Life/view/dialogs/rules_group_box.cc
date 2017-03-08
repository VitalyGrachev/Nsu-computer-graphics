#include "rules_group_box.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

RulesGroupBox::RulesGroupBox(double live_begin, double live_end,
                             double birth_begin, double birth_end,
                             double first_impact, double second_impact,
                             QWidget * parent)
        : QGroupBox(tr("Rules"), parent) {

    first_impact_spin = new QDoubleSpinBox(this);
    first_impact_spin->setValue(first_impact);
    QLabel * first_impact_label = new QLabel(tr("First impact:"), this);
    QHBoxLayout * first_impact_layout = new QHBoxLayout();
    first_impact_layout->addWidget(first_impact_label);
    first_impact_layout->addStretch();
    first_impact_layout->addWidget(first_impact_spin);

    second_impact_spin = new QDoubleSpinBox(this);
    second_impact_spin->setValue(second_impact);
    QLabel * second_impact_label = new QLabel(tr("Second impact:"), this);
    QHBoxLayout * second_impact_layout = new QHBoxLayout();
    second_impact_layout->addWidget(second_impact_label);
    second_impact_layout->addStretch();
    second_impact_layout->addWidget(second_impact_spin);

    birth_begin_spin = new QDoubleSpinBox(this);
    birth_begin_spin->setValue(birth_begin);
    QLabel * birth_begin_label = new QLabel(tr("Birth begin:"), this);
    QHBoxLayout * birth_begin_layout = new QHBoxLayout();
    birth_begin_layout->addWidget(birth_begin_label);
    birth_begin_layout->addStretch();
    birth_begin_layout->addWidget(birth_begin_spin);

    birth_end_spin = new QDoubleSpinBox(this);
    birth_end_spin->setValue(birth_end);
    QLabel * birth_end_label = new QLabel(tr("Birth end:"), this);
    QHBoxLayout * birth_end_layout = new QHBoxLayout();
    birth_end_layout->addWidget(birth_end_label);
    birth_end_layout->addStretch();
    birth_end_layout->addWidget(birth_end_spin);

    live_begin_spin = new QDoubleSpinBox(this);
    live_begin_spin->setValue(live_begin);
    QLabel * live_begin_label = new QLabel(tr("Live begin:"), this);
    QHBoxLayout * live_begin_layout = new QHBoxLayout();
    live_begin_layout->addWidget(live_begin_label);
    live_begin_layout->addStretch();
    live_begin_layout->addWidget(live_begin_spin);

    live_end_spin = new QDoubleSpinBox(this);
    live_end_spin->setValue(live_end);
    QLabel * live_end_label = new QLabel(tr("Live end:"), this);
    QHBoxLayout * live_end_layout = new QHBoxLayout();
    live_end_layout->addWidget(live_end_label);
    live_end_layout->addStretch();
    live_end_layout->addWidget(live_end_spin);

    QVBoxLayout * group_box_layout = new QVBoxLayout();
    group_box_layout->addLayout(first_impact_layout);
    group_box_layout->addLayout(second_impact_layout);
    group_box_layout->addLayout(birth_begin_layout);
    group_box_layout->addLayout(birth_end_layout);
    group_box_layout->addLayout(live_begin_layout);
    group_box_layout->addLayout(live_end_layout);

    setLayout(group_box_layout);
}

