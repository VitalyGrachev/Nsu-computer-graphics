#ifndef LIFE_GAME_ENGINE_H
#define LIFE_GAME_ENGINE_H

#include <cstdint>
#include <memory>
#include "life_field.h"

class LifeGameEngine {
public:
    class Parameters {
    public:
        Parameters(double first_impact,
                   double second_impact,
                   double birth_begin,
                   double birth_end,
                   double live_begin,
                   double live_end);

        Parameters(const Parameters &) = default;

        ~Parameters() = default;

        Parameters & operator=(const Parameters &) = default;

        double get_first_impact() const {
            return first_impact;
        }

        void set_first_impact(double first_impact) {
            Parameters::first_impact = first_impact;
        }

        double get_second_impact() const {
            return second_impact;
        }

        void set_second_impact(double second_impact) {
            Parameters::second_impact = second_impact;
        }

        double get_birth_begin() const {
            return birth_begin;
        }

        void set_birth_begin(double birth_begin) {
            Parameters::birth_begin = birth_begin;
        }

        double get_birth_end() const {
            return birth_end;
        }

        void set_birth_end(double birth_end) {
            Parameters::birth_end = birth_end;
        }

        double get_live_begin() const {
            return live_begin;
        }

        void set_live_begin(double live_begin) {
            Parameters::live_begin = live_begin;
        }

        double get_live_end() const {
            return live_end;
        }

        void set_live_end(double live_end) {
            Parameters::live_end = live_end;
        }

        bool gonna_stay_alive(int first_count, int second_count) const;

        bool gonna_be_born(int first_count, int second_count) const;

    private:
        double first_impact;
        double second_impact;
        double birth_begin;
        double birth_end;
        double live_begin;
        double live_end;
    };

    class ChangeNotifier {
    public:
        ChangeNotifier() {}
        virtual ~ChangeNotifier() {}
        virtual void notify() const = 0;
    };

    LifeGameEngine(uint32_t cols,
                   uint32_t rows,
                   const Parameters & parameters,
                   std::unique_ptr<ChangeNotifier> notifier);

    void tick();

private:
    int first_rank_neighbor_count(uint32_t col, uint32_t row) const;

    int second_rank_neighbor_count(uint32_t col, uint32_t row) const;

    LifeField * current_field;
    LifeField * next_field;
    LifeField field1;
    LifeField field2;
    Parameters parameters;
    std::unique_ptr<ChangeNotifier> notifier;
};


#endif //LIFE_GAME_ENGINE_H
