#ifndef LIFE_FIELD_H
#define LIFE_FIELD_H

#include <algorithm>
#include <cstdint>
#include <vector>

class LifeField {
public:
    enum class CellState : char {
        DEAD, ALIVE
    };

    using Row = std::vector<CellState>;

    LifeField(uint32_t cols, uint32_t rows);

    LifeField(const LifeField &) = default;

    ~LifeField() = default;

    LifeField & operator=(const LifeField &) = default;

    const Row & operator[](uint32_t row) const;

    Row & operator[](uint32_t row);

    uint32_t cols() const { return cols_; }

    uint32_t rows() const { return rows_; }

private:
    std::vector<Row> field_;
    const uint32_t cols_;
    const uint32_t rows_;
};

inline const LifeField::Row & LifeField::operator[](uint32_t row) const {
    return field_[row];
}

inline LifeField::Row & LifeField::operator[](uint32_t row) {
    return field_[row];
}

#endif // LIFE_FIELD_H
