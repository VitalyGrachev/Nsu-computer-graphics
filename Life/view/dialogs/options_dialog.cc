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

    setLayout(group_boxes_layout);
}

