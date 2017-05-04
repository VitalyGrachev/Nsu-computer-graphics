#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "view/wireframe_widget.h"
#include "view/spline_widget.h"
#include "view/object_properties_widget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();
private:
    void create_scene();

    WireframeWidget * wireframe;
    SplineWidget * spline;
    ObjectPropertiesWidget * properties;

    std::shared_ptr<Camera> camera;
};

#endif //MAIN_WINDOW_H
