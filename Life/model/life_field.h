#ifndef LIFE_FIELD_H
#define LIFE_FIELD_H

#include <vector>

class LifeField {
public:
    enum class CellState : char {
        DEAD, ALIVE
    };

    using Row = std::vector<CellState>;

    LifeField(int cols, int rows);

    LifeField(const LifeField &) = default;

    ~LifeField() = default;

    LifeField & operator=(const LifeField &) = default;

    const Row & operator[](int row) const;

    Row & operator[](int row);

    int get_cols() const { return cols_; }

    int get_rows() const { return rows_; }

private:
    std::vector<Row> field_;
    const int cols_;
    const int rows_;
};

LifeField::LifeField(int cols, int rows)
        : field_(rows, std::vector<CellState>(cols, CellState::DEAD)),
          cols_(cols), rows_(rows) {
}

inline const LifeField::Row & LifeField::operator[](int row) const {
    return field_[row];
}

inline LifeField::Row & LifeField::operator[](int row) {
    return field_[row];
}

#endif // LIFE_FIELD_H
