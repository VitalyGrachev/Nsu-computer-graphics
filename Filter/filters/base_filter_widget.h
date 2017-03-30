#ifndef BASE_FILTER_WIDGET_H
#define BASE_FILTER_WIDGET_H

#include <memory>
#include <QWidget>
#include <QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "abstract_filter.h"
#include "../view/zone.h"

class BaseFilterWidget : public QWidget {
Q_OBJECT
public:
    BaseFilterWidget();

    virtual ~BaseFilterWidget() {}

    void set_zones(Zone * zone_B, Zone * zone_C);

public slots:

    void apply_filter();

    virtual void apply_filter(ImageWrapper input_image) = 0;

    void filtration_finished(ImageWrapper output_image, uint64_t op_id);

signals:

    void filtrate(std::shared_ptr<AbstractFilter> filter,
                  ImageWrapper input_image,
                  uint64_t op_id);

    void accepted();

    void cancelled();

private slots:

    void accept();

    void cancel();

protected:
    uint64_t reserve_next_op_id();

    void set_params_widget(QWidget * widget);

    Zone * zone_B;
    Zone * zone_C;
private:

    void enable_buttons(bool enable = true);

    QPushButton * ok_button;
    QPushButton * cancel_button;
    QVBoxLayout * widget_layout;
    uint64_t min_op_id;
    uint64_t last_applied_op_id;
    uint64_t last_finished_op_id;

    static const QSize button_size;
};

#endif //BASE_FILTER_WIDGET_H
