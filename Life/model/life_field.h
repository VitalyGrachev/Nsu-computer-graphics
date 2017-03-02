#ifndef LIFE_FIELD_H
#define LIFE_FIELD_H

#include <algorithm>
#include <cstdint>
#include <vector>

class LifeStateField {
public:
    using CellState = bool;
    using Row = std::vector<CellState>;

    static const CellState ALIVE;
    static const CellState DEAD;

    LifeStateField(uint32_t cols, uint32_t rows);

    LifeStateField(const LifeStateField &) = default;

    ~LifeStateField() = default;

    LifeStateField & operator=(const LifeStateField &) = default;

    void clear();

    const Row & operator[](uint32_t row) const;

    Row & operator[](uint32_t row);

    bool is_contained(int col, int row) const;

    uint64_t alive_cells_count() const;

    uint32_t cols() const { return cols_; }

    uint32_t rows() const { return rows_; }

private:
    std::vector<Row> states;
    uint32_t cols_;
    uint32_t rows_;
};

inline const LifeStateField::Row & LifeStateField::operator[](uint32_t row) const {
    return states[row];
}

inline LifeStateField::Row & LifeStateField::operator[](uint32_t row) {
    return states[row];
}

#endif // LIFE_FIELD_H
