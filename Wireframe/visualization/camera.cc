#include "camera.h"

#include <QVector2D>
#include "base_object.h"
#include "../util/transform.h"

namespace {

bool clip_segment(Segment & segment,
                  double control_value,
                  std::function<double(const QVector4D &)> evaluator,
                  std::function<bool(const double &, const double &)> predicate) {
    if (predicate(evaluator(segment.point2), evaluator(segment.point1))) {
        std::swap(segment.point1, segment.point2);
    }
    const double value1 = evaluator(segment.point1);
    const double value2 = evaluator(segment.point2);
    if (predicate(value2, control_value)) {
        return false;
    }
    if (predicate(value1, control_value)) {
        const double abs_val1 = std::abs(value1 - control_value);
        const double abs_val2 = std::abs(value2 - control_value);
        const double k = abs_val1 / (abs_val1 + abs_val2);
        segment.point1 = (1.0 - k) * segment.point1 + k * segment.point2;
    }
    return true;
}

}

const QVector3D Camera::world_up(0.0, 1.0, 0.0);
const Segment Camera::axis_x(QVector3D(0.2, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0));
const Segment Camera::axis_y(QVector3D(0.0, 0.2, 0.0), QVector3D(0.0, 0.0, 0.0));
const Segment Camera::axis_z(QVector3D(0.0, 0.0, 0.2), QVector3D(0.0, 0.0, 0.0));
const QRgb Camera::axis_x_color(qRgb(255, 0, 0));
const QRgb Camera::axis_y_color(qRgb(0, 255, 0));
const QRgb Camera::axis_z_color(qRgb(0, 0, 255));

Camera::Camera()
        : position(2.5, 1.5, 0.0),
          point_to_look(0.0, 0.0, 0.0),
          right(0.0, 0.0, 1.0),
          viewport(0.16, 0.12),
          background_color(qRgb(180, 180, 180)) {
    set_clip_planes(0.1, 5.0);
    recalculate_orientation_vectors();
    recalculate_view_matrix();
    recalculate_projection_matrix();
}

ImageWrapper Camera::take_picture(const QSize & screen) const {
    using Transform::translation;

    ImageWrapper picture(viewport.scaled(screen, Qt::KeepAspectRatio).toSize());
    picture.fill(background_color);

    if (scene_to_look_at) {
        const QMatrix4x4 scene_transform = projection_matrix * view_matrix * scene_to_look_at->get_transform();

        for (BaseObject * object : scene_to_look_at->get_objects()) {
            const QRgb color = object->get_color();
            const QMatrix4x4 object_transform = scene_transform * object->get_transform();

            std::unique_ptr<BaseObject::SegmentProvider> segments(object->get_segment_provider());
            while (segments->has_next()) {
                const Segment model_segment = segments->next();
                Segment segment(object_transform * model_segment.point1,
                                object_transform * model_segment.point2);
                segment.point1 /= segment.point1.w();
                segment.point2 /= segment.point2.w();
                if (clip(segment)) {
                    const QPoint pt1 = rescale_to_screen(segment.point1.toPointF(), picture.size());
                    const QPoint pt2 = rescale_to_screen(segment.point2.toPointF(), picture.size());

                    picture.draw_line(pt1, pt2, color, LineType::SOLID);
                }
            }
            const QMatrix4x4 axis_transform = scene_transform *
                                              translation(object->get_position()) *
                                              object->get_rotation();
            draw_axis(picture, axis_transform, axis_x, axis_x_color);
            draw_axis(picture, axis_transform, axis_y, axis_y_color);
            draw_axis(picture, axis_transform, axis_z, axis_z_color);
        }
    }

    return picture;
}

void Camera::set_scene(Scene * scene) {
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

void Camera::set_viewport(const QSizeF & viewport) {
    this->viewport = viewport;

    recalculate_projection_matrix();
}

void Camera::set_vertical_fov(double fov) {
    vertical_fov = fov;

    recalculate_projection_matrix();
}

void Camera::set_background_color(QRgb color) {
    background_color = color;
}

void Camera::recalculate_orientation_vectors() {
    const QVector3D dir = point_to_look - position;
    right = QVector3D::crossProduct(dir, world_up).normalized();
    screen_up = QVector3D::crossProduct(right, dir).normalized();
}

void Camera::recalculate_view_matrix() {
    view_matrix = Transform::look_at(position, point_to_look, world_up);
}

void Camera::recalculate_projection_matrix() {
    const double aspect = viewport.width() / viewport.height();
    projection_matrix = Transform::perspective(vertical_fov, aspect, z_near, z_far);
}

QPoint Camera::rescale_to_screen(const QPointF & point, const QSize & screen_size) const {
    const double x_scale_factor = static_cast<double>(screen_size.width()) / 2;
    const double y_scale_factor = static_cast<double>(screen_size.height()) / 2;
    const double x = (point.x() + 1.0) * x_scale_factor;
    const double y = (point.y() + 1.0) * y_scale_factor;
    return QPoint(static_cast<int>(x), screen_size.height() - 1 - static_cast<int>(y));
}

QVector3D Camera::rotation_axis(const QVector2D & delta) const {
    return delta.y() * right + delta.x() * screen_up;
}

void Camera::rotate_scene_in_camera_space(const QVector2D & delta) {
    const float magic = 0.3f;
    const float angle = magic * delta.length();
    const QVector3D axis = rotation_axis(delta);

    const QMatrix4x4 & last_rotation = scene_to_look_at->get_rotation();
    if (scene_to_look_at) {
        scene_to_look_at->set_rotation(Transform::rotation(angle, axis) * last_rotation);
    }
}

void Camera::rotate_object_in_camera_space(const QVector2D & delta, BaseObject * object) {
    const float magic = 0.3f;
    const float angle = magic * delta.length();
    const QVector3D axis = scene_to_look_at->get_rotation().inverted() * rotation_axis(delta);

    const QMatrix4x4 & last_rotation = object->get_rotation();
    object->set_rotation(Transform::rotation(angle, axis) * last_rotation);
    scene_to_look_at->recalculate_bounding_box();
}

bool Camera::clip(Segment & segment) const {
    return clip_segment(segment, -1.0, [](const QVector4D & v) { return v.x(); }, std::less<double>()) &&
           clip_segment(segment, 1.0, [](const QVector4D & v) { return v.x(); }, std::greater<double>()) &&
           clip_segment(segment, -1.0, [](const QVector4D & v) { return v.y(); }, std::less<double>()) &&
           clip_segment(segment, 1.0, [](const QVector4D & v) { return v.y(); }, std::greater<double>()) &&
           clip_segment(segment, 0.0, [](const QVector4D & v) { return v.z(); }, std::less<double>()) &&
           clip_segment(segment, 1.0, [](const QVector4D & v) { return v.z(); }, std::greater<double>());
}

void Camera::draw_axis(ImageWrapper & picture,
                       const QMatrix4x4 & transform,
                       const Segment & axis, QRgb color) const {
    Segment segment(transform * axis.point1, transform * axis.point2);
    segment.point1 /= segment.point1.w();
    segment.point2 /= segment.point2.w();
    if (clip(segment)) {
        const QPoint pt1 = rescale_to_screen(segment.point1.toPointF(), picture.size());
        const QPoint pt2 = rescale_to_screen(segment.point2.toPointF(), picture.size());

        picture.draw_line(pt1, pt2, color, LineType::SOLID);
    }
}
