#include "new_field_dialog.h"

#include <QHBoxLayout>
#include <QPushButton>

NewFieldDialog::NewFieldDialog(FieldSizeGroupBox * field_size_group_box,
                               CellSizeGroupBox * cell_size_group_box,
                               QWidget * parent)
        : QDialog(parent),
          field_size_group_box(field_size_group_box),
          cell_size_group_box(cell_size_group_box) {
    QVBoxLayout * dialog_layout = new QVBoxLayout();

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
