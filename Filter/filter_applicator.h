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
    static FilterApplicator & instance();

public slots:

    void filtrate_image(std::shared_ptr<AbstractFilter> filter,
                        ImageWrapper input_image,
                        uint64_t op_id = 0);

    void filtrate_image(std::shared_ptr<AbstractFilter> filter,
                        ImageWrapper input_image,
                        const QObject * result_receiver,
                        const char * method,
                        uint64_t op_id = 0);

signals:

    void filtration_finished(ImageWrapper output_image, uint64_t op_id);

private:
    FilterApplicator() {}

    FilterApplicator(const FilterApplicator &) = delete;

    FilterApplicator & operator=(const FilterApplicator &) = delete;

    QThreadPool thread_pool;
};

#endif //FILTER_APPLICATOR_H
