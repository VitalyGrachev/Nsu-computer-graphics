#include "new_field_dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>

NewFieldDialog::NewFieldDialog(uint32_t min_cols, uint32_t max_cols,
                               uint32_t min_rows, uint32_t max_rows,
                               uint32_t min_edge_size, uint32_t max_edge_size,
                               QWidget * parent)
        : QDialog(parent) {
    QVBoxLayout * dialog_layout = new QVBoxLayout();

    //  Field size box
    QGroupBox * field_size_box = new QGroupBox(tr("Field size"), this);
    cols_slider = new QSlider(Qt::Horizontal, this);
    cols_spin = new QSpinBox(this);
    cols_slider->setRange(min_cols, max_cols);
    cols_spin->setRange(min_cols, max_cols);
    connect(cols_slider, SIGNAL(valueChanged(int)),
            cols_spin, SLOT(setValue(int)));
    connect(cols_spin, SIGNAL(valueChanged(int)),
            cols_slider, SLOT(setValue(int)));

    QHBoxLayout * cols_layout = new QHBoxLayout();
    cols_layout->addWidget(cols_slider);
    cols_layout->addWidget(cols_spin);


    rows_slider = new QSlider(Qt::Horizontal, this);
    rows_spin = new QSpinBox(this);
    rows_slider->setRange(min_rows, max_rows);
    rows_spin->setRange(min_rows, max_rows);
    connect(rows_slider, SIGNAL(valueChanged(int)),
            rows_spin, SLOT(setValue(int)));
    connect(rows_spin, SIGNAL(valueChanged(int)),
            rows_slider, SLOT(setValue(int)));

    QHBoxLayout * rows_layout = new QHBoxLayout();
    rows_layout->addWidget(rows_slider);
    rows_layout->addWidget(rows_spin);

    QVBoxLayout * field_size_layout = new QVBoxLayout();
    field_size_layout->addLayout(cols_layout);
    field_size_layout->addLayout(rows_layout);

    field_size_box->setLayout(field_size_layout);

    //  Cell size box
    QGroupBox * cell_size_box = new QGroupBox(tr("Cell size"), this);
    cell_slider = new QSlider(Qt::Horizontal, this);
    cell_spin = new QSpinBox(this);
    cell_slider->setRange(min_edge_size, max_edge_size);
    cell_spin->setRange(min_edge_size, max_edge_size);
    connect(cell_slider, SIGNAL(valueChanged(int)),
            cell_spin, SLOT(setValue(int)));
    connect(cell_spin, SIGNAL(valueChanged(int)),
            cell_slider, SLOT(setValue(int)));

    QHBoxLayout * cell_layout = new QHBoxLayout();
    cell_layout->addWidget(cell_slider);
    cell_layout->addWidget(cell_spin);

    cell_size_box->setLayout(cell_layout);

    // Buttons
    QPushButton * ok_button = new QPushButton(tr("Ok"), this);
    connect(ok_button, SIGNAL(clicked()),
            this, SLOT(accept()));
    QPushButton * cancel_button = new QPushButton(tr("Cancel"), this);
    connect(cancel_button, SIGNAL(clicked()),
            this, SLOT(reject()));

    QHBoxLayout * button_layout = new QHBoxLayout();
    button_layout->addWidget(ok_button);
    button_layout->addWidget(cancel_button);


    dialog_layout->addWidget(field_size_box);
    dialog_layout->addWidget(cell_size_box);
    dialog_layout->addLayout(button_layout);

    setLayout(dialog_layout);
}

void NewFieldDialog::accept() {
    emit create_new_field(cols_slider->value(), rows_slider->value(), cell_slider->value());
    QDialog::accept();
}
