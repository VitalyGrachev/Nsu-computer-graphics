#include "cell_size_group_box.h"

#include <QSpinBox>
#include <QHBoxLayout>

CellSizeGroupBox::CellSizeGroupBox(uint32_t edge_size,
                                   uint32_t min_edge_size, uint32_t max_edge_size,
                                   QWidget * parent)
        : QGroupBox(tr("Cell size"), parent) {
    edge_size_slider = new QSlider(Qt::Horizontal, this);
    QSpinBox * edge_size_spin = new QSpinBox(this);
    edge_size_slider->setRange(min_edge_size, max_edge_size);
    edge_size_spin->setRange(min_edge_size, max_edge_size);
    connect(edge_size_slider, SIGNAL(valueChanged(int)),
            edge_size_spin, SLOT(setValue(int)));
    connect(edge_size_spin, SIGNAL(valueChanged(int)),
            edge_size_slider, SLOT(setValue(int)));
    edge_size_slider->setValue(edge_size);

    QHBoxLayout * cell_layout = new QHBoxLayout();
    cell_layout->addWidget(edge_size_slider);
    cell_layout->addWidget(edge_size_spin);

    setLayout(cell_layout);
}