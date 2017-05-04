#ifndef DOUBLE_VALUE_SETTER_H
#define DOUBLE_VALUE_SETTER_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QSlider>

class DoubleValueSetter : public QWidget {
Q_OBJECT
public:
    DoubleValueSetter(const QString & name,
                      double min_val, double max_val,
                      double step, QWidget * parent = nullptr);

    double value() const { return spin_box->value(); }

public slots:

    void set_value(double value) { spin_box->setValue(value); };

signals:

    void value_changed(double value);

private slots:

    void change_slider_value(double value);

    void change_spinbox_value(int value);

private:
    QDoubleSpinBox * spin_box;
    QSlider * slider;
    double step;
};

#endif //DOUBLE_VALUE_SETTER_H
