#include "life_impacts.h"
#include <algorithm>

namespace {
class ImpactRowGenerator {
public:
    ImpactRowGenerator(uint32_t max_cols) : max_cols_(max_cols), dc(1) {
    }

    LifeImpactField::Row operator()() {
        dc = (dc + 1) % 2;
        return LifeImpactField::Row(max_cols_ - dc, 0.0);
    }

private:
    uint32_t max_cols_;
    int8_t dc;
};
}

LifeImpactField::LifeImpactField(uint32_t cols, uint32_t rows)
        : impacts(rows) {
    std::generate(impacts.begin(), impacts.end(), ImpactRowGenerator(cols));
}

void LifeImpactField::clear() {
    for (Row & row : impacts) {
        std::fill(row.begin(), row.end(), 0.0);
    }
}

bool LifeImpactField::is_contained(int col, int row) {
    return (row >= 0 && row < impacts.size() &&
            col >= 0 && col < impacts[row].size());
}