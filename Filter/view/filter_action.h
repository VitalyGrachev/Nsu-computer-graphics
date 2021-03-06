#ifndef FILTER_ACTION_H
#define FILTER_ACTION_H

#include <QAction>

class FilterAction : public QAction {
Q_OBJECT
public:
    FilterAction(const QString & text, QObject * parent = nullptr);

    FilterAction(const QIcon & icon, const QString & text, QObject * parent = nullptr);

signals:

    void apply_filter(QString filter_name);

private slots:

    void was_toggled(bool checked);

private:
    void init();
};

#endif //FILTER_ACTION_H
