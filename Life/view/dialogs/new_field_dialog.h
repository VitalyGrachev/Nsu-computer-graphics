#ifndef NEW_FIELD_DIALOG_H
#define NEW_FIELD_DIALOG_H

#include <QDialog>
#include "field_size_group_box.h"
#include "cell_size_group_box.h"

class NewFieldDialog : public QDialog {
Q_OBJECT
public:
    NewFieldDialog(FieldSizeGroupBox * field_size_group_box,
                   CellSizeGroupBox * cell_size_group_box,
                   QWidget * parent = nullptr);

signals:

    void create_new_field(int cols, int rows, int cell_edge);

public slots:

    void accept() override;

private:
    FieldSizeGroupBox * field_size_group_box;
    CellSizeGroupBox * cell_size_group_box;
};


#endif //NEW_FIELD_DIALOG_H
