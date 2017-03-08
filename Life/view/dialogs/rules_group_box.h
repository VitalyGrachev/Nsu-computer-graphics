#ifndef RULES_GROUP_BOX_H
#define RULES_GROUP_BOX_H

#include <QGroupBox>
#include <QDoubleSpinBox>

class RulesGroupBox : public QGroupBox {
public:
    RulesGroupBox(double live_begin, double live_end,
                  double birth_begin, double birth_end,
                  double first_impact, double second_impact,
                  QWidget * parent = nullptr);

    double get_first_impact() const { return first_impact_spin->value(); }

    double get_second_impact() const { return second_impact_spin->value(); }

    double get_birth_begin() const { return birth_begin_spin->value(); }

    double get_birth_end() const { return birth_end_spin->value(); }

    double get_live_begin() const { return live_begin_spin->value(); }

    double get_live_end() const { return live_end_spin->value(); }

private:
    QDoubleSpinBox * first_impact_spin;
    QDoubleSpinBox * second_impact_spin;
    QDoubleSpinBox * birth_begin_spin;
    QDoubleSpinBox * birth_end_spin;
    QDoubleSpinBox * live_begin_spin;
    QDoubleSpinBox * live_end_spin;
};


#endif //RULES_GROUP_BOX_H
