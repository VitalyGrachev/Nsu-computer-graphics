#include "options_dialog.h"

#include <QHBoxLayout>
#include <QPushButton>

OptionsDialog::OptionsDialog(FieldSizeGroupBox * field_size_group_box,
                             CellSizeGroupBox * cell_size_group_box,
                             RulesGroupBox * rules_group_box,
                             QWidget * parent)
        : QDialog(parent),
          field_size_group_box(field_size_group_box),
          cell_size_group_box(cell_size_group_box),
          rules_group_box(rules_group_box) {
    QVBoxLayout * left_column_layout = new QVBoxLayout();
    left_column_layout->addWidget(field_size_group_box);
    left_column_layout->addWidget(cell_size_group_box);

    QHBoxLayout * group_boxes_layout = new QHBoxLayout();
    group_boxes_layout->addLayout(left_column_layout);
    group_boxes_layout->addWidget(rules_group_box);

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

    QVBoxLayout * dialog_layout = new QVBoxLayout();
    dialog_layout->addLayout(group_boxes_layout);
    dialog_layout->addLayout(button_layout);

    setLayout(dialog_layout);
}

void OptionsDialog::accept() {
    emit set_options(field_size_group_box->get_cols(),
                     field_size_group_box->get_rows(),
                     cell_size_group_box->get_edge_size(),
                     rules_group_box->get_live_begin(), rules_group_box->get_live_end(),
                     rules_group_box->get_birth_begin(), rules_group_box->get_birth_end(),
                     rules_group_box->get_first_impact(), rules_group_box->get_second_impact());
    QDialog::accept();
}
