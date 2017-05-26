#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <QSizeF>
#include <QVector3D>
#include <QMatrix4x4>
#include "../util/image_wrapper.h"
#include "scene.h"

class Camera {
public:
    Camera();

    ~Camera() = default;

    ImageWrapper take_picture(const QSize & screen) const;

    const QSizeF & get_viewport() const { return viewport; }

    double get_near_clip_lane() const { return z_near; }

    double get_far_clip_lane() const { return z_far; }

    void set_scene(std::shared_ptr<Scene> scene);

    void set_viewport(const QSizeF & viewport);

    void set_position(const QVector3D & position);

    void set_point_to_look(const QVector3D & point_to_look);

    void set_clip_planes(double near, double far);

    void rotate_scene_in_camera_space(const QVector2D & delta);

    void rotate_object_in_camera_space(const QVector2D & delta, BaseObject * object);

private:
    void recalculate_view_matrix();

    void recalculate_projection_matrix();

    void recalculate_orientation_vectors();

    void set_vertical_fov(double fov);

    void draw_axis(ImageWrapper & picture, const QMatrix4x4 & transform, const Segment & axis, QRgb color) const;

    QMatrix4x4 rotation_in_camera_space(const QVector2D & delta) const;

    QPointF rescale_to_screen(const QPointF & point, const QSize & screen_size) const;

    bool clip_segment_by_z(Segment & segment) const;

    std::shared_ptr<Scene> scene_to_look_at;
    QMatrix4x4 projection_matrix;
    QMatrix4x4 view_matrix;
    QVector3D position;
    QVector3D point_to_look;
    QVector3D right;
    QVector3D screen_up;
    QSizeF viewport;
    double vertical_fov;
    double z_near;
    double z_far;
    QRgb background_color;

    static const QVector3D world_up;
    static const Segment axis_x;
    static const Segment axis_y;
    static const Segment axis_z;
    static const QRgb axis_x_color;
    static const QRgb axis_y_color;
    static const QRgb axis_z_color;
};


#endif //CAMERA_H
