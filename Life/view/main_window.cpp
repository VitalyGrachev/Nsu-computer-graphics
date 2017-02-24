#include <QtWidgets/QScrollArea>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    FieldDisplay * field_display = new FieldDisplay();

    QScrollArea * scroll_area = new QScrollArea();
    scroll_area->setVisible(true);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(field_display);

    setCentralWidget(scroll_area);

    setWindowTitle(tr("Life"));
    resize(500, 500);
}
