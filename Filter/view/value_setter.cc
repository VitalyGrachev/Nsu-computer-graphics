#include "value_setter.h"

#include <QBoxLayout>

ValueSetter::ValueSetter(const QString & name,
                         const int & min_val,
                         const int & max_val,
                         Orientation orientation,
                         QWidget * parent)
        : QWidget(parent) {
    QBoxLayout::Direction dir = (orientation == Orientation::Horizontal ?
                                 QBoxLayout::Direction::LeftToRight :
                                 QBoxLayout::Direction::TopToBottom);
    Qt::Orientation orient = (orientation == Orientation::Horizontal ?
                              Qt::Horizontal :
                              Qt::Vertical);
    QBoxLayout * layout = new QBoxLayout(dir, this);
    layout->addWidget(label = new QLabel(name));
    layout->addWidget(slider = new QSlider(orient));
    layout->addWidget(spin_box = new QSpinBox());
    slider->setMinimum(min_val);
    slider->setMaximum(max_val);
    spin_box->setMinimum(min_val);
    spin_box->setMaximum(max_val);

    connect(slider, &QSlider::valueChanged,
            spin_box, &QSpinBox::setValue);

    connect(spin_box, SIGNAL(valueChanged(int)),
            slider, SLOT(setValue(int)));

    connect(slider, &QSlider::valueChanged,
            this, &ValueSetter::value_changed);
}