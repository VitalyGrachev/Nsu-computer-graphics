#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <QRect>
#include <QVector3D>
#include <QMatrix4x4>
#include "../util/image_wrapper.h"
#include "scene.h"

class Camera {
public:
    Camera();

    ~Camera() = default;

    ImageWrapper take_picture() const;

    const QSize & get_viewport() const { return viewport; }

    void set_scene(std::shared_ptr<Scene> scene);

    void set_viewport(const QSize & viewport);

    void set_position(const QVector3D & position);

    void set_point_to_look(const QVector3D & point_to_look);

    void set_up(const QVector3D & up);

    void set_vertical_fov(double fov);

    void set_clip_planes(double near, double far);

    void rotate_scene_in_camera_space(const QVector2D & delta);

    void rotate_object_in_camera_space(BaseObject * object, const QVector2D & delta);

private:
    void recalculate_params();

    QMatrix4x4 rotation_in_camera_space(const QVector2D & delta) const;

    QPointF rescale_to_screen(const QPointF & point) const;

    std::shared_ptr<Scene> scene_to_look_at;
    QMatrix4x4 camera_transform;
    QVector3D position;
    QVector3D point_to_look;
    QVector3D right;
    QVector3D up;
    QSize viewport;
    double vertical_fov;
    double z_near;
    double z_far;
    QRgb background_color;
};


#endif //CAMERA_H
