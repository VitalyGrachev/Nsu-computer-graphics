#ifndef FILTER_ACTION_H
#define FILTER_ACTION_H

#include <QAction>

class FilterAction : public QAction {
Q_OBJECT
public:
    FilterAction(const QString & text, QObject * parent = nullptr)
            : QAction(text, parent) {}

    FilterAction(const QIcon & icon, const QString & text, QObject * parent = nullptr)
            : QAction(icon, text, parent) {}

signals:
    void apply_filter(const QString & filter_name);

private slots:

    void was_toggled(bool checked);
private:
    void init();
};

#endif //FILTER_ACTION_H
