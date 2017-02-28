#include "life_game_engine.h"

LifeGameEngine::LifeGameEngine(uint32_t cols,
                               uint32_t rows,
                               std::unique_ptr<AbstractNotifier> notifier)
        : state_field(cols, rows),
          impact_field(cols, rows),
          notifier(std::move(notifier)) {
}

void LifeGameEngine::next_step() {
    for (uint32_t r = 0; r < state_field.rows(); ++r) {
        for (uint32_t c = 0; c < state_field.cols() - (r % 2); ++c) {
            state_field[r][c] = fate_determinant.next_state(state_field[r][c], impact_field[r][c]);
        }
    }
    recalculate_impact_field();
    if (notifier) {
        (*notifier)();
    }
}

void LifeGameEngine::clear() {
    state_field.clear();
    impact_field.clear();
    if (notifier) {
        (*notifier)();
    }
}

void LifeGameEngine::recalculate_impact_field() {
    impact_field.clear();
    for (uint32_t r = 0; r < state_field.rows(); ++r) {
        for (uint32_t c = 0; c < state_field.cols() - (r % 2); ++c) {
            if (state_field[r][c] == LifeStateField::ALIVE) {
                propagate_impact(impact_field, c, r, PropagateImpact::Action::INCREASE);
            }
        }
    }
}

void LifeGameEngine::set_neighbour_impacts(double near_neighbour_impact, double far_neighbour_impact) {
    propagate_impact = PropagateImpact(near_neighbour_impact, far_neighbour_impact);
    recalculate_impact_field();
}

bool LifeGameEngine::set_cell_fate_conditions(double birth_begin, double birth_end,
                                              double live_begin, double live_end) {
    if (live_begin <= birth_begin &&
        birth_begin <= birth_end &&
        birth_end <= live_end) {
        fate_determinant = CellFateDeterminant(birth_begin, birth_end, live_begin, live_end);
        return true;
    }
    return false;
}

bool LifeGameEngine::set_cell(uint32_t col, uint32_t row, CellState state) {
    if (state_field.is_contained(col, row)) {
        const CellState cur_state = state_field[row][col];
        if (cur_state != state) {
            if (cur_state == LifeStateField::ALIVE &&
                state == LifeStateField::DEAD) {
                propagate_impact(impact_field, col, row, PropagateImpact::Action::DECREASE);
            } else {
                propagate_impact(impact_field, col, row, PropagateImpact::Action::INCREASE);
            }
            state_field[row][col] = state;
            if (notifier) {
                (*notifier)();
            }
        }
        return true;
    }
    return false;
}

const std::array<int, 6> LifeGameEngine::PropagateImpact::near_neighbour_col_shifts_for_even_row = {
        -1, -1, 0, 1, 0, -1};
const std::array<int, 6> LifeGameEngine::PropagateImpact::near_neighbour_col_shifts_for_odd_row = {
        -1, 0, 1, 1, 1, 0};
const std::array<int, 6> LifeGameEngine::PropagateImpact::far_neighbour_col_shifts_for_even_row = {
        -2, 0, 1, 1, 0, -2};
const std::array<int, 6> LifeGameEngine::PropagateImpact::far_neighbour_col_shifts_for_odd_row = {
        -1, 0, 2, 2, 0, -1};
const std::array<int, 6> LifeGameEngine::PropagateImpact::near_neighbour_row_shifts = {
        0, -1, -1, 0, 1, 1};
const std::array<int, 6> LifeGameEngine::PropagateImpact::far_neighbour_row_shifts = {
        -1, -2, -1, 1, 2, 1};

LifeGameEngine::PropagateImpact::PropagateImpact()
        : PropagateImpact(1.0, 0.3) {}

LifeGameEngine::PropagateImpact::PropagateImpact(double near_neighbour_impact,
                                                 double far_neighbour_impact)
        : near_neighbour_impact(near_neighbour_impact),
          far_neighbour_impact(far_neighbour_impact) {}

void LifeGameEngine::PropagateImpact::operator()(LifeImpactField & impact_field,
                                                 int32_t col, uint32_t row,
                                                 Action action) const {
    const std::array<int, 6> & near_col_shifts = (row % 2 ?
                                                  near_neighbour_col_shifts_for_odd_row :
                                                  near_neighbour_col_shifts_for_even_row);
    const std::array<int, 6> & far_col_shifts = (row % 2 ?
                                                 far_neighbour_col_shifts_for_odd_row :
                                                 far_neighbour_col_shifts_for_even_row);
    const double sign = (action == Action::INCREASE ? 1.0 : -1.0);
    for (int i = 0; i < 6; ++i) {
        const int nc = col + near_col_shifts[i];
        const int nr = row + near_neighbour_row_shifts[i];
        if (impact_field.is_contained(nc, nr)) {
            impact_field[nr][nc] += sign * near_neighbour_impact;
        }

        const int fc = col + far_col_shifts[i];
        const int fr = row + far_neighbour_row_shifts[i];
        if (impact_field.is_contained(fc, fr)) {
            impact_field[fr][fc] += sign * far_neighbour_impact;
        }
    }
}

LifeGameEngine::CellFateDeterminant::CellFateDeterminant(double birth_begin, double birth_end,
                                                         double live_begin, double live_end)
        : birth_begin(birth_begin),
          birth_end(birth_end),
          live_begin(live_begin),
          live_end(live_end) {}

LifeGameEngine::CellFateDeterminant::CellFateDeterminant()
        : CellFateDeterminant(2.3, 2.9, 2.0, 3.3) {}

CellState LifeGameEngine::CellFateDeterminant::next_state(CellState current_state,
                                                          double cell_impact) const {
    CellState next_state;
    if (current_state == LifeStateField::ALIVE) {
        next_state = ((cell_impact >= live_begin) && (cell_impact <= live_end) ?
                      LifeStateField::ALIVE : LifeStateField::DEAD);
    } else {
        next_state = ((cell_impact >= birth_begin) && (cell_impact <= birth_end) ?
                      LifeStateField::ALIVE : LifeStateField::DEAD);
    }
    return next_state;
}