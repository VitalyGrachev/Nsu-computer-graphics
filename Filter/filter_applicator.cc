#include "filter_applicator.h"

#include <cstdint>
#include <QScopedPointer>
#include "filter_runner.h"

FilterApplicator & FilterApplicator::instance() {
    static FilterApplicator fa;
    return fa;
}

void FilterApplicator::filtrate_image(std::shared_ptr<AbstractFilter> filter,
                                      ImageWrapper input_image,
                                      uint64_t op_id) {
    filtrate_image(filter, input_image, this, SIGNAL(filtration_finished(ImageWrapper, uint64_t)), op_id);
}

void FilterApplicator::filtrate_image(std::shared_ptr<AbstractFilter> filter,
                                      ImageWrapper input_image,
                                      const QObject * result_receiver,
                                      const char * method,
                                      uint64_t op_id) {
    QScopedPointer<FilterRunner> work(new FilterRunner(std::move(filter), input_image, op_id));

    qRegisterMetaType<uint64_t>("uint64_t");
    qRegisterMetaType<ImageWrapper>("ImageWrapper");

    connect(work.data(), SIGNAL(finished(ImageWrapper, uint64_t)),
            result_receiver, method);

    thread_pool.clear();

    thread_pool.start(work.take());
}