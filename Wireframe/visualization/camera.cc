#include "camera.h"

#include <QMatrix4x4>
#include "object.h"

ImageWrapper Camera::take_picture() {

    ImageWrapper picture(viewport.width(), viewport.height());
    QMatrix4x4 camera_matr;
    for (Object * object : scene_to_look_at->get_objects()) {
        QRgb color = object->get_color();
        QMatrix4x4 object_matr;
        for (Segment & segment : object->get_segments()) {

        }
    }
}

void Camera::scene(Scene * scene) {
    scene_to_look_at = scene;
}
