#include "camera.h"

#include <QColor>
#include "base_object.h"

namespace {

double aspect_ratio(double width, double height) {
    return width / height;
}

}

Camera::Camera()
        : position(-1.0, 1.0, 0.0),
          point_to_look(0.0, 0.0, 0.0),
          up(0.0, 1.0, 0.0),
          viewport(800, 600),
          vertical_fov(50.0),
          z_near(0.1f),
          z_far(20.0),
          background_color(QColor(180, 180, 180).rgb()) {
    recalculate_matrix();
}

ImageWrapper Camera::take_picture() const {
    ImageWrapper picture(viewport.width(), viewport.height());
    picture.fill(background_color);


    for (BaseObject * object : scene_to_look_at->get_objects()) {
        const QRgb color = object->get_color();
        const QMatrix4x4 mvp = camera_transform * object->get_transform_matrix();

        std::unique_ptr<BaseObject::SegmentProvider> segments(object->get_segment_provider());
        while(segments->has_next()) {
            const Segment segment = segments->next();
            const QVector4D p1 = mvp * segment.point1;
            const QVector4D p2 = mvp * segment.point2;

            const QPointF pt1 = rescale_to_screen(p1.toPointF());
            const QPointF pt2 = rescale_to_screen(p2.toPointF());

            picture.draw_line(pt1.toPoint(), pt2.toPoint(), color, LineType::SOLID);
        }
    }
    return picture;
}

void Camera::set_viewport(const QSize & viewport) {
    this->viewport = viewport;

    recalculate_matrix();
}

void Camera::set_scene(std::shared_ptr<Scene> scene) {
    scene_to_look_at = scene;
}

void Camera::set_position(const QVector3D & position) {
    this->position = position;

    recalculate_matrix();
}

void Camera::set_point_to_look(const QVector3D & point_to_look) {
    this->point_to_look = point_to_look;

    recalculate_matrix();
}

void Camera::set_up(const QVector3D & up) {
    this->up = up;

    recalculate_matrix();
}

void Camera::set_vertical_fov(double fov) {
    this->vertical_fov = fov;

    recalculate_matrix();
}

void Camera::set_clip_planes(double near, double far) {
    this->z_near = near;
    this->z_far = far;

    recalculate_matrix();
}

void Camera::recalculate_matrix() {
    const double aspect = aspect_ratio(viewport.width(), viewport.height());
    camera_transform.setToIdentity();
    camera_transform.perspective(vertical_fov, aspect, z_near, z_far);
    camera_transform.lookAt(position, point_to_look, up);
}

QPointF Camera::rescale_to_screen(const QPointF & point) const {
    const double x_scale_factor = static_cast<double>(viewport.width()) / 2;
    const double y_scale_factor = static_cast<double>(viewport.height()) / 2;
    const double x = (point.x() + 1.0) * x_scale_factor;
    const double y = (point.y() + 1.0) * y_scale_factor;
    return QPointF(x, y);
}
