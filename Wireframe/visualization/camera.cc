#include "camera.h"

#include <QColor>
#include <QVector2D>
#include <iostream>
#include "base_object.h"
#include "../util/matrix_utils.h"

namespace {

double aspect_ratio(double width, double height) {
    return width / height;
}

}

const QVector3D Camera::world_up(0.0, 1.0, 0.0);
const Segment Camera::axis_x(QVector3D(0.2, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0));
const Segment Camera::axis_y(QVector3D(0.0, 0.2, 0.0), QVector3D(0.0, 0.0, 0.0));
const Segment Camera::axis_z(QVector3D(0.0, 0.0, 0.2), QVector3D(0.0, 0.0, 0.0));
const QRgb Camera::axis_x_color(QColor(255, 0, 0).rgb());
const QRgb Camera::axis_y_color(QColor(0, 255, 0).rgb());
const QRgb Camera::axis_z_color(QColor(0, 0, 255).rgb());

Camera::Camera()
        : position(-1.0, 1.0, 0.0),
          point_to_look(0.0, 0.0, 0.0),
          right(0.0, 0.0, 1.0),
          viewport(0.16, 0.12),
          background_color(QColor(180, 180, 180).rgb()) {
    set_clip_planes(0.1, 20.0);
    recalculate_view_matrix();
    recalculate_projection_matrix();
}

ImageWrapper Camera::take_picture(const QSize & screen) const {
    using Transform::translation;

    ImageWrapper picture(viewport.scaled(screen, Qt::KeepAspectRatio).toSize());
    picture.fill(background_color);

    const QMatrix4x4 scene_transform = view_matrix * scene_to_look_at->get_transform();

    for (BaseObject * object : scene_to_look_at->get_objects()) {
        const QRgb color = object->get_color();
        const QMatrix4x4 object_transform = scene_transform * object->get_transform();

        std::unique_ptr<BaseObject::SegmentProvider> segments(object->get_segment_provider());
        while (segments->has_next()) {
            const Segment model_segment = segments->next();
            Segment segment(object_transform * model_segment.point1,
                            object_transform * model_segment.point2);
            if (clip_segment_by_z(segment)) {
                QVector4D p1 = projection_matrix * segment.point1;
                QVector4D p2 = projection_matrix * segment.point2;

                p1 /= p1.w();
                p2 /= p2.w();

                const QPointF pt1 = rescale_to_screen(p1.toPointF(), picture.size());
                const QPointF pt2 = rescale_to_screen(p2.toPointF(), picture.size());

                picture.draw_line(pt1.toPoint(), pt2.toPoint(), color, LineType::SOLID);
            }
        }
        const QMatrix4x4 axis_transform = scene_transform *
                                          translation(object->get_position()) *
                                          object->get_rotation();
        draw_axis(picture, axis_transform, axis_x, axis_x_color);
        draw_axis(picture, axis_transform, axis_y, axis_y_color);
        draw_axis(picture, axis_transform, axis_z, axis_z_color);
    }

    return picture;
}

void Camera::set_viewport(const QSizeF & viewport) {
    this->viewport = viewport;

    recalculate_projection_matrix();
}

void Camera::set_scene(std::shared_ptr<Scene> scene) {
    scene_to_look_at = scene;
}

void Camera::set_position(const QVector3D & position) {
    this->position = position;

    recalculate_orientation_vectors();
    recalculate_view_matrix();
}

void Camera::set_point_to_look(const QVector3D & point_to_look) {
    this->point_to_look = point_to_look;

    recalculate_orientation_vectors();
    recalculate_view_matrix();
}

void Camera::set_clip_planes(double near, double far) {
    z_near = near;
    z_far = far;

    set_vertical_fov(2.0 * std::atan2(viewport.height() / 2, z_near));
}

void Camera::set_vertical_fov(double fov) {
    vertical_fov = fov;

    recalculate_projection_matrix();
}

void Camera::recalculate_orientation_vectors() {
    const QVector3D dir = point_to_look - position;
    right = QVector3D::crossProduct(dir, world_up).normalized();
    screen_up = QVector3D::crossProduct(dir, right).normalized();
}

void Camera::recalculate_view_matrix() {
    view_matrix = Transform::look_at(position, point_to_look, screen_up);
}

void Camera::recalculate_projection_matrix() {
    const double aspect = aspect_ratio(viewport.width(), viewport.height());
    projection_matrix = Transform::perspective(vertical_fov, aspect, z_near, z_far);
}

QPointF Camera::rescale_to_screen(const QPointF & point, const QSize & screen_size) const {
    const double x_scale_factor = static_cast<double>(screen_size.width()) / 2;
    const double y_scale_factor = static_cast<double>(screen_size.height()) / 2;
    const double x = (point.x() + 1.0) * x_scale_factor;
    const double y = (point.y() + 1.0) * y_scale_factor;
    return QPointF(x, y);
}

QMatrix4x4 Camera::rotation_in_camera_space(const QVector2D & delta) const {
    const float magic = 0.3f;
    const float angle = magic * delta.length();
    QVector3D axis = delta.y() * right + delta.x() * screen_up;
    QMatrix4x4 rotation;
    Transform::rotate(rotation, angle, axis);
    return rotation;
}

void Camera::rotate_scene_in_camera_space(const QVector2D & delta) {
    const QMatrix4x4 & last_rotation = scene_to_look_at->get_rotation();
    const QMatrix4x4 additional_rotation = rotation_in_camera_space(delta);
    scene_to_look_at->set_rotation(additional_rotation * last_rotation);
}

void Camera::rotate_object_in_camera_space(const QVector2D & delta, BaseObject * object) {
    const QMatrix4x4 & last_rotation = object->get_rotation();
    const QMatrix4x4 additional_rotation = rotation_in_camera_space(delta);
    object->set_rotation(additional_rotation * last_rotation);
}

bool Camera::clip_segment_by_z(Segment & segment) const {
    //  Points in front of camera have negative z.
    const double pt1_near_plane_diff = (-segment.point1.z()) - z_near;
    const double pt2_near_plane_diff = (-segment.point2.z()) - z_near;
    const double far_plane_pt1_diff = z_far - (-segment.point1.z());
    const double far_plane_pt2_diff = z_far - (-segment.point2.z());

    const bool pt1_closer_than_near_plane = std::signbit(pt1_near_plane_diff);
    const bool pt2_closer_than_near_plane = std::signbit(pt2_near_plane_diff);
    const bool pt1_farther_than_far_plane = std::signbit(far_plane_pt1_diff);
    const bool pt2_farther_than_far_plane = std::signbit(far_plane_pt2_diff);

    if (pt1_closer_than_near_plane && pt2_closer_than_near_plane ||
        pt1_farther_than_far_plane && pt2_farther_than_far_plane) {
        return false;
    }

    if (pt1_closer_than_near_plane || pt2_closer_than_near_plane) {
        const double pt1_coefficient = pt1_near_plane_diff / (pt1_near_plane_diff - pt2_near_plane_diff);
        const QVector4D near_pt = (1.0 - pt1_coefficient) * segment.point1 + pt1_coefficient * segment.point2;
        if (pt1_closer_than_near_plane) {
            segment.point1 = near_pt;
        } else {
            segment.point2 = near_pt;
        }
    }
    if (pt1_farther_than_far_plane || pt2_farther_than_far_plane) {
        const double pt1_coefficient = far_plane_pt1_diff / (far_plane_pt1_diff - far_plane_pt2_diff);
        const QVector4D far_pt = (1.0 - pt1_coefficient) * segment.point1 + pt1_coefficient * segment.point2;
        if (pt1_farther_than_far_plane) {
            segment.point1 = far_pt;
        } else {
            segment.point2 = far_pt;
        }
    }
    return true;
}

void Camera::draw_axis(ImageWrapper & picture,
                       const QMatrix4x4 & transform,
                       const Segment & axis, QRgb color) const {
    Segment segment(transform * axis.point1, transform * axis.point2);
    if (clip_segment_by_z(segment)) {
        QVector4D p1 = projection_matrix * segment.point1;
        QVector4D p2 = projection_matrix * segment.point2;

        p1 /= p1.w();
        p2 /= p2.w();

        const QPointF pt1 = rescale_to_screen(p1.toPointF(), picture.size());
        const QPointF pt2 = rescale_to_screen(p2.toPointF(), picture.size());

        picture.draw_line(pt1.toPoint(), pt2.toPoint(), color, LineType::SOLID);
    }
}
