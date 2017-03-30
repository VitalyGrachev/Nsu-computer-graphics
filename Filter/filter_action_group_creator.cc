#include "filter_action_group_creator.h"

#include <QString>

FilterActionGroupCreator & FilterActionGroupCreator::instance() {
    static FilterActionGroupCreator creator;
    return creator;
}

bool FilterActionGroupCreator::add_filter_action(FilterAction * action) {
    actions.push_front(action);
    return true;
}

FilterActionGroup * FilterActionGroupCreator::create_with_parent(QObject * parent) {
    FilterActionGroup * group = new FilterActionGroup(parent);
    while (!actions.empty()) {
        FilterAction * action = actions.front();
        actions.pop_front();
        group->add_filter_action(action);
    }
    return group;
}

FilterActionGroupCreator::~FilterActionGroupCreator() {
    for (FilterAction * action : actions) {
        delete action;
    }
}