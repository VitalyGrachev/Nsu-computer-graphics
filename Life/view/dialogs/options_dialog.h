#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include "field_size_group_box.h"
#include "cell_size_group_box.h"
#include "rules_group_box.h"

class OptionsDialog : public QDialog {
Q_OBJECT
public:
    OptionsDialog(FieldSizeGroupBox * field_size_group_box,
                  CellSizeGroupBox * cell_size_group_box,
                  RulesGroupBox * rules_group_box,
                  QWidget * parent = nullptr);

signals:

    void set_options(int cols, int rows, int cell_edge,
                     double live_begin, double live_end,
                     double birth_begin, double birth_end,
                     double first_impact, double second_impact);

public slots:

    void accept() override;

private:
    FieldSizeGroupBox * field_size_group_box;
    CellSizeGroupBox * cell_size_group_box;
    RulesGroupBox * rules_group_box;
};


#endif //OPTIONS_DIALOG_H
