#include "double_value_setter.h"

#include <QHBoxLayout>
#include <QLabel>

DoubleValueSetter::DoubleValueSetter(const QString & name,
                                     double min_val, double max_val,
                                     double step, QWidget * parent)
        : QWidget(parent), step(step) {
    QLayout * layout = new QHBoxLayout(this);
    QLabel * label = new QLabel(name, this);

    spin_box = new QDoubleSpinBox(this);
    spin_box->setMinimum(min_val);
    spin_box->setMaximum(max_val);
    spin_box->setSingleStep(step);
    connect(spin_box, SIGNAL(valueChanged(double)),
            this, SLOT(change_slider_value(double)));
    connect(spin_box, SIGNAL(valueChanged(double)),
            this, SIGNAL(value_changed(double)));

    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(static_cast<int>((max_val - min_val) / step));
    connect(slider, SIGNAL(valueChanged(int)),
            this, SLOT(change_spinbox_value(int)));

    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(spin_box);
}

void DoubleValueSetter::change_slider_value(double value) {
    int slider_value = static_cast<int>((value - spin_box->minimum()) / step);
    slider->setValue(slider_value);
}

void DoubleValueSetter::change_spinbox_value(int value) {
    double spinbox_value = spin_box->minimum() + value * step;
    spin_box->setValue(spinbox_value);
}
