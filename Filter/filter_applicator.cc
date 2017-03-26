#include "filter_applicator.h"

#include <QScopedPointer>
#include "filter_runner.h"

void FilterApplicator::filtrate_image(uint64_t op_id,
                                      std::shared_ptr<AbstractFilter> filter,
                                      const ImageWrapper & input_image) {
    QScopedPointer<FilterRunner> work(new FilterRunner(op_id, std::move(filter), input_image));

    connect(work.data(), SIGNAL(finished(uint64_t, ImageWrapper)),
            this, SIGNAL(finished_filtration(uint64_t, ImageWrapper)));

    thread_pool.clear();

    thread_pool.start(work.take());
}