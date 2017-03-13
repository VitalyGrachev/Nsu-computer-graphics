#ifndef ABSTRACT_FILTER_H
#define ABSTRACT_FILTER_H

#include <QImage>

class AbstractFilter {
public:
    virtual ~AbstractFilter() {}

    virtual void operator()(const QImage & input, QImage * output) = 0;
};

#endif //ABSTRACT_FILTER_H
