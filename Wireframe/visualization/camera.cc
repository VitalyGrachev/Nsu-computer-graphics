#include "camera.h"

#include <QColor>
#include <iostream>
#include "object.h"

namespace {

float aspect_ratio(float width, float height) {
    return width / height;
}

}

Camera::Camera()
        : position(-1.0f, 1.0f, 0.0f),
          point_to_look(0.0f, 0.0f, 0.0f),
          up(0.0f, 1.0f, 0.0f),
          viewport(800, 600),
          vertical_fov(50.0f),
          z_near(0.1f),
          z_far(20.0f),
          background_color(QColor(180, 180, 180).rgb()) {
    recalc_matrix();
}

ImageWrapper Camera::take_picture() const {
    ImageWrapper picture(viewport.width(), viewport.height());
    picture.fill(background_color);

    const float x_scale_factor = static_cast<float>(viewport.width()) / 2;
    const float y_scale_factor = static_cast<float>(viewport.height()) / 2;
    const QVector4D scaling(x_scale_factor, y_scale_factor, 0.0f, 0.0f);

    for (Object * object : scene_to_look_at->get_objects()) {
        const QRgb color = object->get_color();
        const QMatrix4x4 object_transform = camera_transform * object->get_world_matrix();

        for (Segment & segment : object->get_segments()) {
            QVector4D p1 = object_transform * segment.point1;
            QVector4D p2 = object_transform * segment.point2;

            p1 += QVector4D(1.0f, 1.0f, 0.0f, 0.0f);
            p2 += QVector4D(1.0f, 1.0f, 0.0f, 0.0f);
            p1 *= scaling;
            p2 *= scaling;

            picture.draw_line(p1.toPoint(), p2.toPoint(), color, LineType::SOLID);
        }
    }
    std::cout << std::endl;

    return picture;
}

void Camera::set_viewport(const QSize & viewport) {
    this->viewport = viewport;

    recalc_matrix();
}

void Camera::set_scene(std::shared_ptr<Scene> scene) {
    scene_to_look_at = scene;
}

void Camera::set_position(const QVector3D & position) {
    this->position = position;

    recalc_matrix();
}

void Camera::set_point_to_look(const QVector3D & point_to_look) {
    this->point_to_look = point_to_look;

    recalc_matrix();
}

void Camera::set_up(const QVector3D & up) {
    this->up = up;

    recalc_matrix();
}

void Camera::set_vertical_fov(float fov) {
    this->vertical_fov = fov;

    recalc_matrix();
}

void Camera::set_clip_planes(float near, float far) {
    this->z_near = near;
    this->z_far = far;

    recalc_matrix();
}

void Camera::recalc_matrix() {
    const float aspect = aspect_ratio(viewport.width(), viewport.height());
    camera_transform.setToIdentity();
    camera_transform.perspective(vertical_fov, aspect, z_near, z_far);
    camera_transform.lookAt(position, point_to_look, up);
}
