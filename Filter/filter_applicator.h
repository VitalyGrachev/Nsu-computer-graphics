#ifndef FILTER_APPLICATOR_H
#define FILTER_APPLICATOR_H

#include <QObject>
#include <QThreadPool>
#include <memory>
#include "image_wrapper.h"
#include "filters/abstract_filter.h"

class FilterApplicator : public QObject {
Q_OBJECT
public:
    FilterApplicator() {}

public slots:

    void filtrate_image(uint64_t op_id,
                        std::shared_ptr<AbstractFilter> filter,
                        const ImageWrapper & input_image);

signals:

    void filtration_finished(uint64_t op_id, ImageWrapper output_image);

private:
    QThreadPool thread_pool;
};

#endif //FILTER_APPLICATOR_H
