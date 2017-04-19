#include "options_dialog.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>

const double OptionsDialog::min_x = -200.0;
const double OptionsDialog::min_y = -200.0;
const double OptionsDialog::max_x = 200.0;
const double OptionsDialog::max_y = 200.0;
const double OptionsDialog::min_width = 1.0;
const double OptionsDialog::min_height = 1.0;
const double OptionsDialog::max_width = 400.0;
const double OptionsDialog::max_height = 400.0;
const int OptionsDialog::min_grid_x = 2;
const int OptionsDialog::min_grid_y = 2;
const int OptionsDialog::max_grid_x = 100;
const int OptionsDialog::max_grid_y = 100;

OptionsDialog::OptionsDialog(const QRectF & cur_domain,
                             const QSize & cur_grid_size,
                             QWidget * parent)
        : QDialog(parent) {
    QVBoxLayout * dialog_layout = new QVBoxLayout(this);
    QGroupBox * domain_group = new QGroupBox(tr("Domain"), this);
    QGroupBox * grid_group = new QGroupBox(tr("Grid"), this);

    QLayout * domain_layout = new QVBoxLayout(domain_group);
    domain_layout->addWidget(domain_left_setter = new DoubleValueSetter(tr("X0"), min_x, max_x, 0.5));
    domain_layout->addWidget(domain_top_setter = new DoubleValueSetter(tr("Y0"), min_y, max_y, 0.5));
    domain_layout->addWidget(domain_width_setter = new DoubleValueSetter(tr("Width"), min_width, max_width, 0.5));
    domain_layout->addWidget(domain_height_setter = new DoubleValueSetter(tr("Height"), min_height, max_height, 0.5));

    domain_left_setter->set_value(cur_domain.x());
    domain_top_setter->set_value(cur_domain.y());
    domain_width_setter->set_value(cur_domain.width());
    domain_height_setter->set_value(cur_domain.height());

    QLayout * grid_layout = new QVBoxLayout(grid_group);
    grid_layout->addWidget(grid_width_setter = new ValueSetter(tr("Width"), min_grid_x, max_grid_x));
    grid_layout->addWidget(grid_height_setter = new ValueSetter(tr("Height"), min_grid_y, max_grid_y));

    grid_width_setter->set_value(cur_grid_size.width());
    grid_height_setter->set_value(cur_grid_size.height());

    QPushButton * ok_button = new QPushButton(tr("Ok"), this);
    connect(ok_button, SIGNAL(clicked()),
            this, SLOT(accept()));
    QPushButton * cancel_button = new QPushButton(tr("Cancel"), this);
    connect(cancel_button, SIGNAL(clicked()),
            this, SLOT(reject()));

    QHBoxLayout * button_layout = new QHBoxLayout();
    button_layout->addWidget(ok_button);
    button_layout->addWidget(cancel_button);

    dialog_layout->addWidget(domain_group);
    dialog_layout->addWidget(grid_group);
    dialog_layout->addLayout(button_layout);
}

void OptionsDialog::accept() {
    QRectF domain(domain_left_setter->value(), domain_top_setter->value(),
                  domain_width_setter->value(), domain_height_setter->value());
    emit set_domain(domain);

    QSize grid_size(grid_width_setter->value(), grid_height_setter->value());
    emit set_grid_size(grid_size);
    QDialog::accept();
}
