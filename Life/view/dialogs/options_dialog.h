#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include "field_size_group_box.h"
#include "cell_size_group_box.h"
#include "rules_group_box.h"

class OptionsDialog : public QDialog {
public:
    OptionsDialog(FieldSizeGroupBox * field_size_group_box,
                  CellSizeGroupBox * cell_size_group_box,
                  RulesGroupBox * rules_group_box,
                  QWidget * parent = nullptr);

private:
    FieldSizeGroupBox * field_size_group_box;
    CellSizeGroupBox * cell_size_group_box;
    RulesGroupBox * rules_group_box;
};


#endif //OPTIONS_DIALOG_H
