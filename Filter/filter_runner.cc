#include "filter_runner.h"

FilterRunner::FilterRunner(std::shared_ptr<AbstractFilter> filter,
                           ImageWrapper input_image,
                           uint64_t op_id)
        : op_id(op_id),
          filter(filter),
          input_image(input_image) {}

void FilterRunner::run() {
    ImageWrapper output = (*filter)(input_image);
    emit finished(output, op_id);
}