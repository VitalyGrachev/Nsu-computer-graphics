#ifndef NEW_FIELD_DIALOG_H
#define NEW_FIELD_DIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>

class NewFieldDialog : public QDialog {
Q_OBJECT
public:
    NewFieldDialog(uint32_t min_cols, uint32_t max_cols,
                   uint32_t min_rows, uint32_t max_rows,
                   uint32_t min_edge_size, uint32_t max_edge_size,
                   QWidget * parent = nullptr);

signals:

    void create_new_field(int cols, int rows, int cell_edge);

public slots:

    void accept() override;

private:
    QSlider * cols_slider;
    QSpinBox * cols_spin;
    QSlider * rows_slider;
    QSpinBox * rows_spin;
    QSlider * cell_slider;
    QSpinBox * cell_spin;
};


#endif //NEW_FIELD_DIALOG_H
