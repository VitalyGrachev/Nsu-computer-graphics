#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <memory>
#include "scene_info.h"

class SceneLoader {
public:
    SceneLoader(SceneInfo * scene_info, QTextStream & stream);

    bool operator()();

private:
    void parse_line(const QString & line);

    bool is_comment(const QString & line);

    enum class Content {
        Nothing, CommonParams, VisibilityPyramid,
        SceneRotation, BgColor, ObjectsCount,
        ObjectColor, ObjectPosition, ObjectRotation,
        ControlPointsCount, ControlPoint
    };

    SceneInfo * output;
    QTextStream & stream;
    QMatrix4x4 matrix;
    QVector3D object_pos;
    QRgb object_color;
    int matrix_row = 0;
    int cur_object = 0;
    int cur_point = 0;
    int points_count;
    int n, m, k;
    double a, b, c, d;
    Content expected_content = Content::CommonParams;
    QRegExp comment_tester;
    QRegExp whitespace_tester;
};

#endif //SCENE_LOADER_H
