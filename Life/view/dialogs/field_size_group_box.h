#ifndef FIELD_SIZE_GROUP_BOX_H
#define FIELD_SIZE_GROUP_BOX_H

#include <QGroupBox>
#include <QSlider>

class FieldSizeGroupBox : public QGroupBox {
public:
    FieldSizeGroupBox(uint32_t act_cols, uint32_t act_rows,
                      uint32_t min_cols, uint32_t max_cols,
                      uint32_t min_rows, uint32_t max_rows,
                      QWidget * parent = nullptr);

    int get_cols() const { return cols_slider->value(); }

    int get_rows() const { return rows_slider->value(); }

private:
    QSlider * cols_slider;
    QSlider * rows_slider;
};


#endif //FIELD_SIZE_GROUP_BOX_H
