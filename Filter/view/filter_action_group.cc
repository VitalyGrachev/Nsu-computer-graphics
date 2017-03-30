#include "filter_action_group.h"

FilterActionGroup::FilterActionGroup(QObject * parent)
        : QActionGroup(parent) {}

void FilterActionGroup::add_filter_action(FilterAction * action) {
    QActionGroup::addAction(action);
    QObject::connect(action, SIGNAL(apply_filter(QString)),
                     this, SIGNAL(apply_filter(QString)));
    action->setCheckable(true);
}

void FilterActionGroup::uncheck_all() {
    checkedAction()->setChecked(false);
}
