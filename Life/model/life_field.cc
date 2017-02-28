#include "life_field.h"

namespace {
class RowGenerator {
public:
    RowGenerator(uint32_t cols) : cols(cols), dc(1) {}

    RowGenerator(const RowGenerator &) = default;

    LifeField::Row operator()() {
        dc = (dc + 1) % 2;
        return LifeField::Row(cols - dc, LifeField::CellState::DEAD);
    }

private:
    uint32_t cols;
    uint8_t dc;
};
}

LifeField::LifeField(uint32_t cols, uint32_t rows) :
        field_(rows),
        cols_(cols),
        rows_(rows) {
    RowGenerator generator(cols_);
    std::generate(field_.begin(), field_.end(), generator);
}