#include "life_game_engine.h"

LifeGameEngine::Parameters::Parameters(double first_impact,
                                       double second_impact,
                                       double birth_begin,
                                       double birth_end,
                                       double live_begin,
                                       double live_end)
        : first_impact(first_impact),
          second_impact(second_impact),
          birth_begin(birth_begin),
          birth_end(birth_end),
          live_begin(live_begin),
          live_end(live_end) {
}

LifeGameEngine::Parameters::Parameters()
        : Parameters(1.0, 0.3, 2.3, 2.9, 2.0, 3.3) {}

bool LifeGameEngine::Parameters::gonna_stay_alive(int first_count, int second_count) const {
    double impact = first_impact * first_count + second_impact * second_count;
    return (impact >= live_begin) && (impact <= live_end);
}

bool LifeGameEngine::Parameters::gonna_be_born(int first_count, int second_count) const {
    double impact = first_impact * first_count + second_impact * second_count;
    return (impact >= birth_begin) && (impact <= birth_end);
}

LifeGameEngine::LifeGameEngine(uint32_t cols,
                               uint32_t rows,
                               const Parameters & parameters,
                               std::unique_ptr<AbstractNotifier> notifier)
        : field1(cols, rows),
          field2(field1),
          field3(field1),
          parameters(parameters),
          current_field(&field1),
          next_field(&field2),
          last_field(&field3),
          notifier(std::move(notifier)) {
}

int LifeGameEngine::first_rank_neighbor_count(const uint32_t col, const uint32_t row) const {
    int count = 0;
    // cols never equals to 1
    if (field1.rows() == 1) { // only one row exists
        count = ((col > 0) && ((*current_field)[0][col - 1] == CellState::ALIVE)) +
                ((col < field1.cols() - 1) && ((*current_field)[0][col + 1] == CellState::ALIVE));
    } else { // cols > 1 && rows > 1
        if (col == 0) {
            if (row == 0) { // first col, first row
                count = ((*current_field)[0][1] == CellState::ALIVE) +
                        ((*current_field)[1][0] == CellState::ALIVE);
            } else if (row == field1.rows() - 1) { // first col, last row
                count = ((*current_field)[row - 1][0] == CellState::ALIVE) +
                        ((*current_field)[row][1] == CellState::ALIVE) +
                        ((row % 2) && ((*current_field)[row - 1][1] == CellState::ALIVE));
            } else { // first col, central row
                count = ((*current_field)[row - 1][0] == CellState::ALIVE) +
                        ((*current_field)[row + 1][0] == CellState::ALIVE) +
                        ((*current_field)[row][1] == CellState::ALIVE) +
                        ((row % 2) && (((*current_field)[row - 1][1] == CellState::ALIVE) +
                                       ((*current_field)[row + 1][1] == CellState::ALIVE)));
            }
        } else if (col == field1.cols() - 1 - (row % 2)) {
            if (row == 0) { // last col, first row
                count = ((*current_field)[0][col - 1] == CellState::ALIVE) +
                        ((*current_field)[1][col - 1] == CellState::ALIVE);
            } else if (row == field1.rows() - 1) { // last col, last row
                count = ((*current_field)[row][col - 1] == CellState::ALIVE) +
                        ((*current_field)[row - 1][col - 1 + (row % 2)] == CellState::ALIVE) +
                        ((row % 2) && ((*current_field)[row - 1][col + 1] == CellState::ALIVE));
            } else { // last col, central row
                if (row % 2) {  //  odd row
                    count = ((*current_field)[row - 1][col + 1] == CellState::ALIVE) +
                            ((*current_field)[row + 1][col + 1] == CellState::ALIVE) +
                            ((*current_field)[row - 1][col] == CellState::ALIVE) +
                            ((*current_field)[row + 1][col] == CellState::ALIVE) +
                            ((*current_field)[row][col - 1] == CellState::ALIVE);
                } else {    //  even row
                    count = ((*current_field)[row - 1][col - 1] == CellState::ALIVE) +
                            ((*current_field)[row][col - 1] == CellState::ALIVE) +
                            ((*current_field)[row + 1][col - 1] == CellState::ALIVE);
                }
            }
        } else if (row == 0) { // central col, first row
            count = ((*current_field)[0][col - 1] == CellState::ALIVE) +
                    ((*current_field)[0][col + 1] == CellState::ALIVE) +
                    ((*current_field)[1][col - 1] == CellState::ALIVE) +
                    ((*current_field)[1][col] == CellState::ALIVE);
        } else if (row == field1.rows() - 1) { // central col, last row
            count = ((*current_field)[row][col - 1] == CellState::ALIVE) +
                    ((*current_field)[row][col + 1] == CellState::ALIVE) +
                    ((*current_field)[row - 1][col + (row % 2) - 1] == CellState::ALIVE) +
                    ((*current_field)[row - 1][col + (row % 2)] == CellState::ALIVE);
        } else { // central col, central row
            count = ((*current_field)[row - 1][col + (row % 2) - 1] == CellState::ALIVE) +
                    ((*current_field)[row + 1][col + (row % 2) - 1] == CellState::ALIVE) +
                    ((*current_field)[row - 1][col + (row % 2)] == CellState::ALIVE) +
                    ((*current_field)[row + 1][col + (row % 2)] == CellState::ALIVE) +
                    ((*current_field)[row][col - 1] == CellState::ALIVE) +
                    ((*current_field)[row][col + 1] == CellState::ALIVE);
        }
    }
    return count;
}

int LifeGameEngine::second_rank_neighbor_count(const uint32_t col, const uint32_t row) const {
    const uint32_t xx = col + (row % 2);
    int count = ((row > 1) && ((*current_field)[row - 2][col] == CellState::ALIVE)) +
                ((row < field1.rows() - 2) && ((*current_field)[row + 2][col] == CellState::ALIVE)) +
                ((xx > 1) &&
                 (((row > 0) && ((*current_field)[row - 1][xx - 2] == CellState::ALIVE)) +
                  ((row < field1.rows() - 1) && ((*current_field)[row + 1][xx - 2] == CellState::ALIVE)))) +
                ((xx < field1.cols() - 1 - (row % 2)) &&
                 (((row > 0) && ((*current_field)[row - 1][xx + 1] == CellState::ALIVE)) +
                  ((row < field1.rows() - 1) && ((*current_field)[row + 1][xx + 1] == CellState::ALIVE))));
    return count;
}

void LifeGameEngine::reassign_fields() {
    LifeField * cur = current_field;
    current_field = next_field;
    next_field = last_field;
    last_field = cur;
}

void LifeGameEngine::tick() {
    for (int r = 0; r < field1.rows(); ++r) {
        for (int c = 0; c < field1.cols() - (r % 2); ++c) {
            int first_rank_count = first_rank_neighbor_count(c, r);
            int second_rank_count = second_rank_neighbor_count(c, r);
            if ((*current_field)[r][c] == CellState::ALIVE) {
                (*next_field)[r][c] = (parameters.gonna_stay_alive(first_rank_count, second_rank_count) ?
                                       CellState::ALIVE : CellState::DEAD);
            } else {
                (*next_field)[r][c] = (parameters.gonna_be_born(first_rank_count, second_rank_count) ?
                                       CellState::ALIVE : CellState::DEAD);
            }
        }
    }
    reassign_fields();
    notifier->notify();
}

bool LifeGameEngine::set_cell_state(uint32_t col, uint32_t row, CellState state) {
    if ((col >= field1.cols() - (row % 2)) ||
        (row >= field1.rows())) {
        return false;
    }
    (*current_field)[row][col] = state;
    return true;
}