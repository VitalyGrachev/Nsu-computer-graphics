#ifndef SCENE_H
#define SCENE_H

#include <forward_list>
#include "object.h"

class Scene {
public:
    using ObjectsContainer = std::forward_list<Object *>;

    Scene() = default;

    ~Scene();

    ObjectsContainer & get_objects() { return objects; }

    const QMatrix4x4 & get_world_matrix() const { return world_transform_matrix; }

    void add_object(Object * object);

private:
    void recalc_bounding_box();

    ObjectsContainer objects;
    QMatrix4x4 world_transform_matrix;
    float scale;
};

#endif //SCENE_H
