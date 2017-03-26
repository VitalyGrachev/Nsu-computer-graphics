#include "main_window.h"
#include <QScrollArea>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget * parent)
        : QMainWindow(parent) {
    QScrollArea * scroll_area = new QScrollArea(this);
    QWidget * central_widget = new QWidget(this);
    QHBoxLayout * central_layout = new QHBoxLayout(central_widget);

    central_layout->addSpacing(zone_spacing);
    central_layout->addWidget(zone_A = new Zone(zone_width, zone_height));
    central_layout->addSpacing(zone_spacing);
    central_layout->addWidget(zone_B = new Zone(zone_width, zone_height));
    central_layout->addSpacing(zone_spacing);
    central_layout->addWidget(zone_C = new Zone(zone_width, zone_height));
    central_layout->addSpacing(zone_spacing);

    scroll_area->setWidget(central_widget);
    scroll_area->setAlignment(Qt::AlignCenter);

    setCentralWidget(scroll_area);

    setWindowTitle(tr("FIT_14202_Grachev_Filter"));
    resize(800, 600);
}