#ifndef SCENE_H
#define SCENE_H

#include <forward_list>
#include "base_object.h"

class Scene {
public:
    using ObjectsContainer = std::forward_list<BaseObject *>;

    Scene();

    ~Scene();

    const QMatrix4x4 & get_transform() const { return transform_matrix; }

    const QMatrix4x4 & get_rotation() const { return rotation; }

    const QVector3D & get_position() const { return center_position; }

    float get_scale() const { return scale_factor; }

    ObjectsContainer & get_objects() { return objects; }

    void add_object(BaseObject * object);

    void remove_object(BaseObject * object);

    void set_rotation(const QMatrix4x4 & rotation);

    void set_position(const QVector3D & center_position);

    void set_scale(float scale_factor);

private:
    void recalculate_bounding_box();

    void recalculate_transform();

    ObjectsContainer objects;
    QMatrix4x4 transform_matrix;
    QMatrix4x4 rotation;
    QVector3D center_position;
    float scale_factor;
};

#endif //SCENE_H
