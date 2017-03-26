#include "filter_action.h"

FilterAction::FilterAction(const QString & text, QObject * parent)
        : QAction(text, parent) {
    init();
}

FilterAction::FilterAction(const QIcon & icon, const QString & text, QObject * parent)
        : QAction(icon, text, parent) {
    init();
}

void FilterAction::init() {
    connect(this, SIGNAL(toggled(bool)),
            this, SLOT(was_toggled(bool)));
}

void FilterAction::was_toggled(bool checked) {
    if(checked) {
        emit apply_filter(text());
    }
}
