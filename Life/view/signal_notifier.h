#ifndef SIGNAL_NOTIFIER_H
#define SIGNAL_NOTIFIER_H

#include <QObject>
#include "../model/abstract_notifier.h"

class SignalNotifier : public QObject, public AbstractNotifier {
    Q_OBJECT
public:
    SignalNotifier() {}
    ~SignalNotifier() {}
    void operator()() override { emit notification(); }
signals:
    void notification();
};

#endif //SIGNAL_NOTIFIER_H
