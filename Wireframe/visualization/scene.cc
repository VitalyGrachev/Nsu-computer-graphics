#include "scene.h"

void Scene::add_object(Object * object) {
    objects.push_front(object);
    recalc_bounding_box();
}

void Scene::recalc_bounding_box() {

}
