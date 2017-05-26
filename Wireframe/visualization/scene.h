#ifndef SCENE_H
#define SCENE_H

#include <deque>
#include <unordered_set>
#include "base_object.h"
#include "../util/bounding_box.h"
#include "../figure/plain_segment_object.h"
#include "../figure/parallelepiped.h"

class Scene {
public:
    using ObjectsContainer = std::deque<BaseObject *>;

    Scene();

    ~Scene();

    const QMatrix4x4 & get_transform() const { return transform_matrix; }

    const QMatrix4x4 & get_rotation() const { return rotation; }

    const QVector3D & get_position() const { return center_position; }

    float get_scale() const { return scale.x(); }

    ObjectsContainer & get_objects() { return objects; }

    void add_object(BaseObject * object);

    void remove_object(BaseObject * object);

    void set_rotation(const QMatrix4x4 & rotation);

private:
    void set_position(const QVector3D & center_position);

    void set_scale(float scale_factor);

    void recalculate_bounding_box(BaseObject * object);

    void recalculate_bounding_box();

    void recalculate_transform();

    Parallelepiped * box_object;
    std::unordered_set<BaseObject *> doesnt_affect_bounds;
    ObjectsContainer objects;
    BoundingBox bounds;
    QMatrix4x4 transform_matrix;
    QMatrix4x4 rotation;
    QVector3D center_position;
    QVector3D scale;
};

#endif //SCENE_H
