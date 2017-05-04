#include "field_size_group_box.h"

#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

FieldSizeGroupBox::FieldSizeGroupBox(uint32_t act_cols, uint32_t act_rows,
                                     uint32_t min_cols, uint32_t max_cols,
                                     uint32_t min_rows, uint32_t max_rows,
                                     QWidget * parent)
        : QGroupBox(tr("Field size"), parent) {
    cols_slider = new QSlider(Qt::Horizontal, this);
    QSpinBox * cols_spin = new QSpinBox(this);
    cols_slider->setRange(min_cols, max_cols);
    cols_spin->setRange(min_cols, max_cols);
    connect(cols_slider, SIGNAL(valueChanged(int)),
            cols_spin, SLOT(setValue(int)));
    connect(cols_spin, SIGNAL(valueChanged(int)),
            cols_slider, SLOT(setValue(int)));
    cols_slider->setValue(act_cols);

    QHBoxLayout * cols_layout = new QHBoxLayout();
    cols_layout->addWidget(cols_slider);
    cols_layout->addWidget(cols_spin);


    rows_slider = new QSlider(Qt::Horizontal, this);
    QSpinBox * rows_spin = new QSpinBox(this);
    rows_slider->setRange(min_rows, max_rows);
    rows_spin->setRange(min_rows, max_rows);
    connect(rows_slider, SIGNAL(valueChanged(int)),
            rows_spin, SLOT(setValue(int)));
    connect(rows_spin, SIGNAL(valueChanged(int)),
            rows_slider, SLOT(setValue(int)));
    rows_slider->setValue(act_rows);

    QHBoxLayout * rows_layout = new QHBoxLayout();
    rows_layout->addWidget(rows_slider);
    rows_layout->addWidget(rows_spin);

    QVBoxLayout * field_size_layout = new QVBoxLayout();
    field_size_layout->addLayout(cols_layout);
    field_size_layout->addLayout(rows_layout);

    setLayout(field_size_layout);
}
