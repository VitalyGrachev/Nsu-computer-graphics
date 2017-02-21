#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    FieldDisplay * field_painter = new FieldDisplay();
    setCentralWidget(field_painter);

    setWindowTitle(tr("Life"));
    resize(500, 500);
}
