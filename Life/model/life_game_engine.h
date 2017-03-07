#ifndef LIFE_GAME_ENGINE_H
#define LIFE_GAME_ENGINE_H

#include <array>
#include <cstdint>

#include "life_field.h"
#include "abstract_notifier.h"
#include "life_impacts.h"

using CellState = LifeStateField::CellState;

class LifeGameEngine {
public:


    LifeGameEngine(uint32_t cols,
                   uint32_t rows,
                   AbstractNotifier * notifier);

    LifeGameEngine(const LifeGameEngine &) = delete;

    LifeGameEngine & operator=(const LifeGameEngine &) = delete;

    ~LifeGameEngine() = default;

    void next_step();

    void clear();

    bool set_cell(uint32_t col, uint32_t row, CellState state = LifeStateField::ALIVE);

    void set_neighbour_impacts(double near_neighbour_impact, double far_neighbour_impact);

    bool set_cell_fate_conditions(double birth_begin, double birth_end,
                                  double live_begin, double live_end);

    const LifeStateField & get_state_field() const { return state_field; }

    const LifeImpactField & get_impact_field() const { return impact_field; }

    uint32_t cols() const { return state_field.cols(); }
    uint32_t rows() const { return state_field.rows(); }

private:
    class PropagateImpact {
    public:
        enum class Action : char {
            INCREASE, DECREASE
        };

        PropagateImpact();

        PropagateImpact(double near_neighbour_impact, double far_neighbour_impact);

        PropagateImpact(const PropagateImpact &) = default;

        PropagateImpact & operator=(const PropagateImpact &) = default;

        ~PropagateImpact() = default;

        void operator()(LifeImpactField & impact_field, int32_t col, uint32_t row, Action action) const;

    private:
        double near_neighbour_impact;
        double far_neighbour_impact;

        static const std::array<int, 6> near_neighbour_col_shifts_for_even_row;
        static const std::array<int, 6> near_neighbour_col_shifts_for_odd_row;
        static const std::array<int, 6> far_neighbour_col_shifts_for_even_row;
        static const std::array<int, 6> far_neighbour_col_shifts_for_odd_row;
        static const std::array<int, 6> near_neighbour_row_shifts;
        static const std::array<int, 6> far_neighbour_row_shifts;
        static const double eps;
    };

    class CellFateDeterminant {
    public:
        CellFateDeterminant();

        CellFateDeterminant(double birth_begin,
                            double birth_end,
                            double live_begin,
                            double live_end);

        CellFateDeterminant(const CellFateDeterminant &) = default;

        ~CellFateDeterminant() = default;

        CellFateDeterminant & operator=(const CellFateDeterminant &) = default;

        CellState next_state(CellState current_state, double cell_impact) const;

    private:
        double birth_begin;
        double birth_end;
        double live_begin;
        double live_end;
    };

    void recalculate_impact_field();

    LifeStateField state_field;
    LifeImpactField impact_field;
    AbstractNotifier * notifier;
    CellFateDeterminant fate_determinant;
    PropagateImpact propagate_impact;
};

#endif //LIFE_GAME_ENGINE_H
