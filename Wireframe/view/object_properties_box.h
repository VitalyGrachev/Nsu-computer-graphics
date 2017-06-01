#ifndef OBJECT_PROPERTIES_H
#define OBJECT_PROPERTIES_H

#include <QGroupBox>
#include <QSpinBox>
#include "../visualization/base_object.h"

class ObjectPropertiesBox : public QGroupBox {
Q_OBJECT
public:
    ObjectPropertiesBox(QWidget * parent = nullptr);

public slots:

    void set_objects(std::vector<BaseObject *> * objects);

signals:

    void view_changed();

    void active_object_changed(int object_index);

private slots:

    void select_object(int index);

    void change_position();

private:
    void create_layout();

    QSpinBox * object_selector;
    QDoubleSpinBox * object_x;
    QDoubleSpinBox * object_y;
    QDoubleSpinBox * object_z;
    std::vector<BaseObject *> * objects = nullptr;

    bool ignore_changes = false;
};

#endif //OBJECT_PROPERTIES_H
