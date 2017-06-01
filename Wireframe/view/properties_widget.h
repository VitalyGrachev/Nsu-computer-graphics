#ifndef PROPERTIES_WIDGET_H
#define PROPERTIES_WIDGET_H

#include <QWidget>
#include <QtWidgets/QButtonGroup>
#include "camera_properties_box.h"
#include "object_properties_box.h"
#include "../util/scene_info.h"

class PropertiesWidget : public QWidget {
Q_OBJECT
public:
    PropertiesWidget(QWidget * parent = nullptr);

    void set_scene_info(SceneInfo * scene_info);

signals:

    void view_changed();

    void zoom(bool zoom_in);

    void camera_changed(Camera * camera);

    void active_object_changed(BaseObject * object);

    void active_spline_changed(BSpline * generatrix);

private slots:

    void change_active_object(int object_index);

    void recalculate_scene_bounds();

private:
    void create_layout();

    SceneInfo * scene_info = nullptr;
    CameraPropertiesBox * camera_properties;
    ObjectPropertiesBox * object_properties;
};

#endif //PROPERTIES_WIDGET_H
