#ifndef FILTER_ACTION_GROUP_CREATOR_H
#define FILTER_ACTION_GROUP_CREATOR_H

#include <forward_list>
#include <QObject>
#include "view/filter_action.h"
#include "view/filter_action_group.h"

class FilterActionGroupCreator {
public:
    static FilterActionGroupCreator & instance();

    bool add_filter_action(FilterAction * action);

    FilterActionGroup * create_with_parent(QObject * parent);

private:
    FilterActionGroupCreator() {}

    FilterActionGroupCreator(const FilterActionGroupCreator &) = delete;

    FilterActionGroupCreator & operator=(const FilterActionGroupCreator &) = delete;

    ~FilterActionGroupCreator();

    std::forward_list<FilterAction *> actions;
};


#endif //FILTER_ACTION_GROUP_CREATOR_H
