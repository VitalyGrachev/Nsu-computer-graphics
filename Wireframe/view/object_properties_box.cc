#include "object_properties_box.h"

#include <QGridLayout>
#include <QLabel>

ObjectPropertiesBox::ObjectPropertiesBox(QWidget * parent)
        : QGroupBox(QObject::tr("Object properties"), parent) {
    create_layout();
}

void ObjectPropertiesBox::create_layout() {
    QGridLayout * main_layout = new QGridLayout(this);

    main_layout->addWidget(new QLabel(tr("Object №")), 0, 0);
    main_layout->addWidget(object_selector = new QSpinBox(), 0, 1);
    object_selector->setMinimum(0);
    object_selector->setMaximum(0);
    object_selector->setEnabled(false);
    connect(object_selector, SIGNAL(valueChanged(int)),
            this, SLOT(select_object(int)));

    main_layout->addItem(new QSpacerItem(1, 10), 1, 0);

    main_layout->addWidget(new QLabel(tr("Position X")), 2, 0);
    main_layout->addWidget(object_x = new QDoubleSpinBox(), 2, 1);
    object_x->setMinimum(-100.0);
    object_x->setMaximum(100.0);
    object_x->setEnabled(false);
    object_x->setSingleStep(0.01);
    connect(object_x, SIGNAL(valueChanged(double)),
            this, SLOT(change_position()));

    main_layout->addWidget(new QLabel(tr("Position Y")), 3, 0);
    main_layout->addWidget(object_y = new QDoubleSpinBox(), 3, 1);
    object_y->setMinimum(-100.0);
    object_y->setMaximum(100.0);
    object_y->setEnabled(false);
    object_y->setSingleStep(0.01);
    connect(object_y, SIGNAL(valueChanged(double)),
            this, SLOT(change_position()));

    main_layout->addWidget(new QLabel(tr("Position Z")), 4, 0);
    main_layout->addWidget(object_z = new QDoubleSpinBox(), 4, 1);
    object_z->setMinimum(-100.0);
    object_z->setMaximum(100.0);
    object_z->setEnabled(false);
    object_z->setSingleStep(0.01);
    connect(object_z, SIGNAL(valueChanged(double)),
            this, SLOT(change_position()));
}

void ObjectPropertiesBox::set_objects(std::vector<BaseObject *> * objects) {
    this->objects = objects;

    if (objects && !objects->empty()) {
        object_selector->setEnabled(true);
        object_x->setEnabled(true);
        object_y->setEnabled(true);
        object_z->setEnabled(true);

        object_selector->setMaximum(objects->size() - 1);
        object_selector->setValue(0);
        select_object(0);
    } else {
        object_selector->setEnabled(false);
        object_x->setEnabled(false);
        object_y->setEnabled(false);
        object_z->setEnabled(false);
    }
}

void ObjectPropertiesBox::select_object(int index) {
    ignore_changes = true;
    const QVector3D & pos = (*objects)[index]->get_position();
    object_x->setValue(pos.x());
    object_y->setValue(pos.y());
    object_z->setValue(pos.z());
    emit active_object_changed(index);
    ignore_changes = false;
}

void ObjectPropertiesBox::change_position() {
    if (!ignore_changes) {
        QVector3D position(object_x->value(), object_y->value(), object_z->value());
        (*objects)[object_selector->value()]->set_position(position);
        emit view_changed();
    }
}