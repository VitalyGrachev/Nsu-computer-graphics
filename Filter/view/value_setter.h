#ifndef VALUE_SETTER_H
#define VALUE_SETTER_H

#include <QString>
#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>

enum class Orientation {
    Vertical, Horizontal
};

class ValueSetter : public QWidget {
Q_OBJECT
public:

    ValueSetter(const QString & name,
                const int & min_val,
                const int & max_val,
                Orientation orientation = Orientation::Horizontal,
                QWidget * parent = nullptr);

    int value() const { return spin_box->value(); }

    void set_step(int step);

public slots:

    void set_value(int val) { spin_box->setValue(val); }

signals:

    void value_changed(int val);

private:
    QLabel * label;
    QSpinBox * spin_box;
    QSlider * slider;
};


#endif //VALUE_SETTER_H
