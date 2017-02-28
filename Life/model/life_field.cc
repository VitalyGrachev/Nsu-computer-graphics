#include "life_field.h"

const LifeStateField::CellState LifeStateField::ALIVE = true;

const LifeStateField::CellState LifeStateField::DEAD = false;

namespace {
class RowGenerator {
public:
    RowGenerator(uint32_t cols) : cols(cols), dc(1) {}

    RowGenerator(const RowGenerator &) = default;

    LifeStateField::Row operator()() {
        dc = (dc + 1) % 2;
        return LifeStateField::Row(cols - dc, LifeStateField::DEAD);
    }

private:
    uint32_t cols;
    uint8_t dc;
};
}

LifeStateField::LifeStateField(uint32_t cols, uint32_t rows) :
        states(rows),
        cols_(cols),
        rows_(rows) {
    RowGenerator generator(cols_);
    std::generate(states.begin(), states.end(), generator);
}

void LifeStateField::clear() {
    for (Row & row : states) {
        std::fill(row.begin(), row.end(), DEAD);
    }
}

bool LifeStateField::is_contained(int col, int row) {
    return (row >= 0 && row < states.size() &&
            col >= 0 && col < states[row].size());
}
