#ifndef FILTER_RUNNER_H
#define FILTER_RUNNER_H

#include <cstdint>
#include <memory>
#include <QRunnable>
#include "filters/abstract_filter.h"

class FilterRunner : public QObject, public QRunnable {
Q_OBJECT
public:
    FilterRunner(std::shared_ptr<AbstractFilter> filter,
                 ImageWrapper input_image,
                 uint64_t op_id);

    void run() override;

signals:

    void finished(ImageWrapper output_image, uint64_t op_id);

private:
    uint64_t op_id;
    std::shared_ptr<AbstractFilter> filter;
    ImageWrapper input_image;
};


#endif //FILTER_RUNNER_H
