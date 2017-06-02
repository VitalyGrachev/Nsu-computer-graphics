#include "scene_loader.h"
#include "../figure/solid_of_revolution.h"

#include <QString>

SceneLoader::SceneLoader(SceneInfo * scene_info, QTextStream & stream)
        : stream(stream), output(scene_info),
          comment_tester("^\\s*//.*"), whitespace_tester("\\s+") {
    output->scene = new Scene();
    output->camera = new Camera();
    output->camera->set_scene(output->scene);
}

bool SceneLoader::operator()() {
    do {
        QString line = stream.readLine();
        if (!line.isNull()) {
            if (!line.isEmpty() && !is_comment(line)) {
                parse_line(line);
            }
        } else {
            throw std::runtime_error("Unexpected end of file.");
        }
    } while (expected_content != Content::Nothing);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.isNull() ||
            !line.isEmpty() && !is_comment(line)) {
            throw std::runtime_error("Found garbage at end of file.");
        }
    }

    return true;
}

bool SceneLoader::is_comment(const QString & line) {
    return comment_tester.exactMatch(line);
}

void SceneLoader::parse_line(const QString & line) {
    bool ok = false;
    const char * error_msg = "Undefined error";
    QStringList line_parts = line.split(whitespace_tester, QString::SkipEmptyParts);

    switch (expected_content) {
        case Content::CommonParams:
            if (line_parts.size() >= 7) {
                int params[3];
                for (int i = 0; i < 3; ++i) {
                    params[i] = line_parts[i].toUInt(&ok);
                    if (!ok) break;
                }

                if (ok &&
                    params[0] < 70 &&
                    params[1] < 70 &&
                    params[2] < 70) {
                    const double pi = 3.141592;
                    double props[4];
                    for (int i = 0; i < 4; ++i) {
                        props[i] = line_parts[i + 3].toDouble(&ok);
                        if (!ok) break;
                    }
                    if (ok &&
                        props[0] >= 0.0 &&
                        props[1] <= 1.0 &&
                        props[2] >= 0.0 &&
                        props[3] <= 2.0 * pi &&
                        props[0] <= props[1] &&
                        props[2] <= props[3] &&
                        (line_parts.size() == 7 || line_parts[7].startsWith("//"))) {
                        n = params[0];
                        m = params[1];
                        k = params[2];
                        a = props[0];
                        b = props[1];
                        c = props[2];
                        d = props[3];

                        expected_content = Content::VisibilityPyramid;
                        return;
                    }
                }
            }
            error_msg = "Error with common params.";
            break;

        case Content::VisibilityPyramid:
            if (line_parts.size() >= 4) {
                double params[4];
                for (int i = 0; i < 4; ++i) {
                    params[i] = line_parts[i].toDouble(&ok);
                    if (!ok) break;
                }
                if (ok &&
                    params[0] > 0.0f &&
                    params[2] > 0.0f &&
                    params[3] > 0.0f &&
                    params[0] < params[1] &&
                    (line_parts.size() == 4 || line_parts[4].startsWith("//"))) {
                    output->camera->set_clip_planes(params[0], params[1]);
                    output->camera->set_viewport(QSizeF(params[2], params[3]));

                    expected_content = Content::SceneRotation;
                    return;
                }
            }
            error_msg = "Error with visibility pyramid params.";
            break;

        case Content::SceneRotation:
            if (line_parts.size() >= 3) {
                QVector3D row;
                for (int i = 0; i < 3; ++i) {
                    row[i] = line_parts[i].toFloat(&ok);
                    if (!ok) break;
                }
                if (ok &&
                    (line_parts.size() == 3 || line_parts[3].startsWith("//"))) {
                    matrix.setRow(matrix_row, row);
                    ++matrix_row;

                    if (matrix_row == 3) {
                        matrix_row = 0;
                        output->scene->set_rotation(matrix);

                        expected_content = Content::BgColor;
                    }
                    return;
                }
            }
            error_msg = "Error with scene rotation matrix.";
            break;

        case Content::BgColor:
            if (line_parts.size() >= 3) {
                int params[3];
                for (int i = 0; i < 3; ++i) {
                    params[i] = line_parts[i].toUInt(&ok);
                    if (!ok) break;
                }
                if (ok &&
                    params[0] < 256 &&
                    params[1] < 256 &&
                    params[2] < 256 &&
                    (line_parts.size() == 3 || line_parts[3].startsWith("//"))) {

                    output->camera->set_background_color(qRgb(params[0], params[1], params[2]));

                    expected_content = Content::ObjectsCount;
                    return;
                }
            }
            error_msg = "Error with visibility pyramid params.";
            break;

        case Content::ObjectsCount:
            if (line_parts.size() >= 1) {
                int objects_count = line_parts[0].toUInt(&ok);
                if (ok &&
                    objects_count > 0 &&
                    (line_parts.size() == 1 || line_parts[1].startsWith("//"))) {

                    output->objects.resize(objects_count, nullptr);
                    output->generatrices.resize(objects_count, nullptr);

                    expected_content = Content::ObjectColor;
                    return;
                }
            }
            error_msg = "Error with objects count.";
            break;

        case Content::ObjectColor:
            if (line_parts.size() >= 3) {
                int params[3];
                for (int i = 0; i < 3; ++i) {
                    params[i] = line_parts[i].toUInt(&ok);
                    if (!ok) break;
                }
                if (ok &&
                    params[0] < 256 &&
                    params[1] < 256 &&
                    params[2] < 256 &&
                    (line_parts.size() == 3 || line_parts[3].startsWith("//"))) {

                    object_color = qRgb(params[0], params[1], params[2]);

                    expected_content = Content::ObjectPosition;
                    return;
                }
            }
            error_msg = "Error with object color.";
            break;

        case Content::ObjectPosition:
            if (line_parts.size() >= 3) {
                for (int i = 0; i < 3; ++i) {
                    object_pos[i] = line_parts[i].toFloat(&ok);
                    if (!ok) break;
                }
                if (ok &&
                    (line_parts.size() == 3 || line_parts[3].startsWith("//"))) {

                    expected_content = Content::ObjectRotation;
                    return;
                }
            }
            error_msg = "Error with object position.";
            break;

        case Content::ObjectRotation:
            if (line_parts.size() >= 3) {
                QVector3D row;
                for (int i = 0; i < 3; ++i) {
                    row[i] = line_parts[i].toFloat(&ok);
                    if (!ok) break;
                }
                if (ok &&
                    (line_parts.size() == 3 || line_parts[3].startsWith("//"))) {
                    matrix.setRow(matrix_row, row);
                    ++matrix_row;

                    if (matrix_row == 3) {
                        matrix_row = 0;
                        expected_content = Content::ControlPointsCount;
                    }
                    return;
                }
            }
            error_msg = "Error with object rotation matrix.";
            break;

        case Content::ControlPointsCount:
            if (line_parts.size() >= 1) {
                points_count = line_parts[0].toUInt(&ok);
                if (ok &&
                    points_count > 0 &&
                    (line_parts.size() == 1 || line_parts[1].startsWith("//"))) {

                    output->generatrices[cur_object] = new BSpline(n, k);

                    expected_content = Content::ControlPoint;
                    return;
                }
            }
            error_msg = "Error with control point count.";
            break;

        case Content::ControlPoint:
            if (line_parts.size() >= 2) {
                float coords[2];
                for (int i = 0; i < 2; ++i) {
                    coords[i] = line_parts[i].toFloat(&ok);
                    if (!ok) break;
                }
                if (ok &&
                    (line_parts.size() == 2 || line_parts[2].startsWith("//"))) {

                    output->generatrices[cur_object]->add_control_point(QPointF(coords[0], coords[1]));
                    ++cur_point;

                    if (cur_point == points_count) {
                        BaseObject * object = new SolidOfRevolution(output->generatrices[cur_object], m);
                        output->objects[cur_object] = object;
                        object->set_rotation(matrix);
                        object->set_position(object_pos);
                        object->set_color(object_color);

                        output->scene->add_object(object);

                        cur_point = 0;
                        ++cur_object;
                        expected_content = Content::ObjectColor;
                    }
                    if (cur_object == output->objects.size()) {
                        expected_content = Content::Nothing;
                    }
                    return;
                }
            }
            error_msg = "Error with point position.";
            break;

        default: {/* Unreachable */}
    }

    throw std::runtime_error(error_msg);
}

