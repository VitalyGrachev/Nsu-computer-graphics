#include "scene_info.h"

SceneInfo::~SceneInfo() {
    clear();
}

void SceneInfo::clear() {
    delete camera;
    camera = nullptr;

    delete scene;
    scene = nullptr;

    for (BaseObject * object : objects) {
        delete object;
    }
    objects.clear();

    for (BSpline * curve : generatrices) {
        delete curve;
    }
    generatrices.clear();
}

bool SceneInfo::load(QTextStream & stream) {
    clear();
    return false;
}

bool SceneInfo::save(QTextStream & stream) {
    return false;
}
