#ifndef LIFE_IMPACTS_H
#define LIFE_IMPACTS_H

#include <vector>
#include <cstdint>

class LifeImpactField {
public:
    using Row = std::vector<double>;

    LifeImpactField(uint32_t cols, uint32_t rows);

    Row & operator[](uint32_t row);

    const Row & operator[](uint32_t row) const;

    bool is_contained(int col, int row);

    void clear();

private:
    std::vector<Row> impacts;
};

inline LifeImpactField::Row & LifeImpactField::operator[](uint32_t row) {
    return impacts[row];
}

inline const LifeImpactField::Row & LifeImpactField::operator[](uint32_t row) const {
    return impacts[row];
}

#endif //LIFE_IMPACTS_H
