#ifndef CELL_SIZE_GROUP_BOX_H
#define CELL_SIZE_GROUP_BOX_H

#include <QGroupBox>
#include <QSlider>

class CellSizeGroupBox : public QGroupBox {
public:
    CellSizeGroupBox(uint32_t edge_size,
                     uint32_t min_edge_size, uint32_t max_edge_size,
                     QWidget * parent = nullptr);

    int get_edge_size() const { return edge_size_slider->value(); }

private:
    QSlider * edge_size_slider;
};


#endif //CELL_SIZE_GROUP_BOX_H
