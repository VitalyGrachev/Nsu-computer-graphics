#ifndef FILTER_RUNNER_H
#define FILTER_RUNNER_H

#include <cstdint>
#include <memory>
#include <QRunnable>
#include "filters/abstract_filter.h"

class FilterRunner : public QObject, public QRunnable {
Q_OBJECT
public:
    FilterRunner(uint64_t op_id,
                 std::shared_ptr<AbstractFilter> filter,
                 ImageWrapper input_image);

    void run() override;

signals:

    void finished(uint64_t op_id, ImageWrapper output_image);

private:
    uint64_t op_id;
    std::shared_ptr<AbstractFilter> filter;
    ImageWrapper input_image;
};


#endif //FILTER_RUNNER_H
