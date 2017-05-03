#include "scene.h"

Scene::~Scene() {
    for (Object * object : objects) {
        delete object;
    }
}

void Scene::add_object(Object * object) {
    objects.push_front(object);
    recalc_bounding_box();
}

void Scene::recalc_bounding_box() {

}
