#include "new_field_dialog.h"

#include <QHBoxLayout>
#include <QPushButton>

NewFieldDialog::NewFieldDialog(uint32_t min_cols, uint32_t max_cols, uint32_t act_cols,
                               uint32_t min_rows, uint32_t max_rows, uint32_t act_rows,
                               uint32_t min_edge_size, uint32_t max_edge_size, uint32_t act_edge_size,
                               QWidget * parent)
        : QDialog(parent) {
    QVBoxLayout * dialog_layout = new QVBoxLayout();

    field_size_group_box = new FieldSizeGroupBox(act_cols, act_rows,
                                                 min_cols, max_cols,
                                                 min_rows, max_rows, this);

    cell_size_group_box = new CellSizeGroupBox(act_edge_size,
                                               min_edge_size, max_edge_size, this);

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


    dialog_layout->addWidget(field_size_group_box);
    dialog_layout->addWidget(cell_size_group_box);
    dialog_layout->addLayout(button_layout);

    setLayout(dialog_layout);
}

void NewFieldDialog::accept() {
    emit create_new_field(field_size_group_box->get_cols(),
                          field_size_group_box->get_rows(),
                          cell_size_group_box->get_edge_size());
    QDialog::accept();
}
