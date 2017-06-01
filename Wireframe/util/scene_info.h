#ifndef SCENE_INFO_H
#define SCENE_INFO_H

#include "../curve/bspline.h"
#include "../visualization/base_object.h"
#include "../visualization/camera.h"
#include "../visualization/scene.h"

struct SceneInfo {
    std::vector<BSpline *> generatrices;
    std::vector<BaseObject *> objects;
    Scene * scene = nullptr;
    Camera * camera = nullptr;

    SceneInfo() {}

    ~SceneInfo();

    bool load(QTextStream & stream);

    bool save(QTextStream & stream);

private:
    void clear();
};

#endif //SCENE_INFO_H
