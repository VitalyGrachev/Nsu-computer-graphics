#ifndef CAMERA_PROPERTIES_BOX_H
#define CAMERA_PROPERTIES_BOX_H

#include <QGroupBox>
#include <QDoubleSpinBox>
#include "../visualization/camera.h"

class CameraPropertiesBox : public QGroupBox {
Q_OBJECT
public:
    CameraPropertiesBox(QWidget * parent = nullptr);

public slots:

    void set_camera(Camera * camera);

    void change_zoom(bool zoom_in);

signals:

    void view_changed();

private slots:

    void change_clip_planes();

    void change_viewport();

private:
    void create_layout();

    QDoubleSpinBox * near_clip_plane;
    QDoubleSpinBox * far_clip_plane;
    QDoubleSpinBox * viewport_width;
    QDoubleSpinBox * viewport_height;
    Camera * camera;
    bool ignore_changes = false;
};

#endif //CAMERA_PROPERTIES_BOX_H
