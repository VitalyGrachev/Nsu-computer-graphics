#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <memory>
#include <QMainWindow>
#include "view/wireframe_widget.h"
#include "view/spline_widget.h"
#include "view/properties_widget.h"
#include "util/scene_info.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();

    ~MainWindow() {}

private slots:
    void save_scene();

    void open_file();

    void reset_scene_rotation();

    void set_wireframe_mode();

    void set_spline_mode();

    void show_about();

private:
    void create_central_widget();

    void create_default_scene();

    void create_menu_and_toolbar();

    WireframeWidget * wireframe;
    SplineWidget * spline;
    PropertiesWidget * properties;

    std::shared_ptr<SceneInfo> scene_info;
};

#endif //MAIN_WINDOW_H
