#ifndef FILTER_ACTION_GROUP_H
#define FILTER_ACTION_GROUP_H

#include <QActionGroup>
#include "filter_action.h"

class FilterActionGroup : public QActionGroup {
Q_OBJECT
public:
    FilterActionGroup(QObject * parent);

    void add_filter_action(FilterAction * action);

    void uncheck_all();

signals:

    void apply_filter(QString filter_name);
};

#endif //FILTER_ACTION_GROUP_H
