#ifndef ABSTRACT_NOTIFIER_H
#define ABSTRACT_NOTIFIER_H

class AbstractNotifier {
public:
    virtual ~AbstractNotifier() {}
    virtual void operator()() = 0;
};

#endif //ABSTRACT_NOTIFIER_H
