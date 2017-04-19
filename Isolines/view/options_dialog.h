#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include <QDialog>
#include "value_setter.h"
#include "double_value_setter.h"

class OptionsDialog : public QDialog {
Q_OBJECT
public:
    OptionsDialog(const QRectF & cur_domain,
                  const QSize & cur_grid_size,
                  QWidget * parent = nullptr);

signals:

    void set_domain(const QRectF & domain);

    void set_grid_size(const QSize & grid_size);

private slots:

    void accept() override;

private:
    DoubleValueSetter * domain_left_setter;
    DoubleValueSetter * domain_top_setter;
    DoubleValueSetter * domain_width_setter;
    DoubleValueSetter * domain_height_setter;
    ValueSetter * grid_width_setter;
    ValueSetter * grid_height_setter;

    static const double min_x;
    static const double max_x;
    static const double min_y;
    static const double max_y;
    static const double min_width;
    static const double max_width;
    static const double min_height;
    static const double max_height;
    static const int min_grid_x;
    static const int max_grid_x;
    static const int min_grid_y;
    static const int max_grid_y;
};

#endif //OPTIONS_DIALOG_H
