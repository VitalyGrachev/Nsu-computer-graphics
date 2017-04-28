#ifndef CAMERA_H
#define CAMERA_H

#include <QRect>
#include <QVector3D>
#include "../util/image_wrapper.h"
#include "scene.h"

class Camera {
public:
    void scene(Scene * scene);

    ImageWrapper take_picture();
private:
    Scene * scene_to_look_at;
    QMatrix4x4 view_projection_matrix;
    QVector3D position;
    QRect viewport;
    float z_near;
    float z_far;
};


#endif //CAMERA_H
