#include "scene_info.h"
#include "../figure/solid_of_revolution.h"
#include "scene_loader.h"

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

SceneInfo * SceneInfo::load(QTextStream & stream) {
    QScopedPointer<SceneInfo> scene_info(new SceneInfo);
    SceneLoader loader(scene_info.data(), stream);
    loader();
    return scene_info.take();
}

bool SceneInfo::save(QTextStream & stream) {
    int a, b, c, d;
    a = c = 0;
    b = d = 1;
    int n = generatrices[0]->major_segments_count();
    int m = reinterpret_cast<SolidOfRevolution *>(objects[0])->get_sector_count();
    int k = generatrices[0]->minor_segments_count();

    stream << n << ' ' << m << ' ' << k << ' '
           << a << ' ' << b << ' ' << c << ' ' << d << endl;
    stream << camera->get_near_clip_plane() << ' ' << camera->get_far_clip_lane() << ' '
           << camera->get_viewport().width() << ' ' << camera->get_viewport().height() << endl;

    const QMatrix4x4 & scene_rotation = scene->get_rotation();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            stream << scene_rotation(i, j) << ' ';
        }
        stream << endl;
    }

    const QRgb bg_color = camera->get_background_color();
    stream << qRed(bg_color) << ' ' << qGreen(bg_color) << ' ' << qBlue(bg_color) << endl;
    const int objects_count = objects.size();
    stream << objects_count << " // Objects count" << endl;
    for (int i = 0; i < objects_count; ++i) {
        stream << " // Object N " << i << endl;
        const QRgb color = objects[i]->get_color();
        stream << qRed(color) << ' ' << qGreen(color) << ' ' << qBlue(color) << endl;

        const QVector3D & position = objects[i]->get_position();
        stream << position.x() << ' ' << position.y() << ' ' << position.z() << endl;

        const QMatrix4x4 & object_rotation = objects[i]->get_rotation();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                stream << object_rotation(i, j) << ' ';
            }
            stream << endl;
        }

        const Curve::PointContainer & control_points = generatrices[i]->get_control_points();
        stream << control_points.size() << " // Control points count" << endl;

        for (int j = 0; j < control_points.size(); ++j) {
            stream << control_points[j].x() << ' ' << control_points[j].y() << endl;
        }
    }
    return true;
}
