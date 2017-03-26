#ifndef BASE_FILTER_WIDGET_H
#define BASE_FILTER_WIDGET_H

#include <memory>
#include <QWidget>
#include "abstract_filter.h"

class BaseFilterWidget : public QWidget {
Q_OBJECT
public:
    BaseFilterWidget() {}

    virtual ~BaseFilterWidget() {}

public slots:

    void filtration_finished(uint64_t op_id, ImageWrapper output_image);

signals:

    void filtrate(uint64_t op_id,
                  std::unique_ptr<AbstractFilter> && filter,
                  const ImageWrapper & input_image);

    void filtered(ImageWrapper output_image);

    void all_filters_done();

protected:
    uint64_t last_op_id;
};

inline void BaseFilterWidget::filtration_finished(uint64_t op_id,
                                                        ImageWrapper output_image) {
    emit filtered(output_image);
    if (op_id == last_op_id) {
        emit all_filters_done();
    }
}

#endif //BASE_FILTER_WIDGET_H
