#include "matrix_utils.h"

#include <cmath>

namespace Transform {

const double PI = 3.141592;

namespace {

double degree_to_radian(double degree) {
    return PI * degree / 180.0;
}

}

QMatrix4x4 perspective(double fov_y, double aspect, double z_near, double z_far) {
    const double tanHalfFov = std::tan(fov_y / 2.0);
    QMatrix4x4 result;  // Identity
    result(0, 0) = 1.0 / (aspect * tanHalfFov);
    result(1, 1) = 1.0 / tanHalfFov;
    result(2, 2) = -(z_far + z_near) / (z_far - z_near);
    result(3, 3) = 0.0;
    result(3, 2) = -1.0;
    result(2, 3) = -(2.0 * z_far * z_near) / (z_far - z_near);
    return result;
}

QMatrix4x4 look_at(const QVector3D & eye, const QVector3D & center, const QVector3D & up) {
    const QVector3D f = (center - eye).normalized();
    const QVector3D s = QVector3D::crossProduct(f, up).normalized();
    const QVector3D u = QVector3D::crossProduct(s, f);
    QMatrix4x4 result;  // Identity
    result(0, 0) = s.x();
    result(0, 1) = s.y();
    result(0, 2) = s.z();
    result(1, 0) = u.x();
    result(1, 1) = u.y();
    result(1, 2) = u.z();
    result(2, 0) = -f.x();
    result(2, 1) = -f.y();
    result(2, 2) = -f.z();
    result(0, 3) = -QVector3D::dotProduct(s, eye);
    result(1, 3) = -QVector3D::dotProduct(u, eye);
    result(2, 3) = QVector3D::dotProduct(f, eye);
    return result;
}

void translate(QMatrix4x4 & matrix, const QVector3D & position) {
    const QVector4D last_col = matrix.column(0) * position.x() +
                               matrix.column(1) * position.y() +
                               matrix.column(2) * position.z() +
                               matrix.column(3);
    matrix.setColumn(3, last_col);
}

QMatrix4x4 translation(const QVector3D & position) {
    QMatrix4x4 result;  // Identity
    translate(result, position);
    return result;
}

void rotate(QMatrix4x4 & matrix, double angle_degree, const QVector3D & axis) {
    const double radians = degree_to_radian(angle_degree);
    const double sin = std::sin(radians);
    const double cos = std::cos(radians);
    const QVector3D norm_axis = axis.normalized();
    const QVector3D tmp = (1.0 - cos) * norm_axis;

    QMatrix3x3 r;
    r(0, 0) = cos + tmp[0] * norm_axis[0];
    r(1, 0) = 0 + tmp[0] * norm_axis[1] + sin * norm_axis[2];
    r(2, 0) = 0 + tmp[0] * norm_axis[2] - sin * norm_axis[1];

    r(0, 1) = 0 + tmp[1] * norm_axis[0] - sin * norm_axis[2];
    r(1, 1) = cos + tmp[1] * norm_axis[1];
    r(2, 1) = 0 + tmp[1] * norm_axis[2] + sin * norm_axis[0];

    r(0, 2) = 0 + tmp[2] * norm_axis[0] + sin * norm_axis[1];
    r(1, 2) = 0 + tmp[2] * norm_axis[1] - sin * norm_axis[0];
    r(2, 2) = cos + tmp[2] * norm_axis[2];

    QMatrix4x4 result(matrix.toGenericMatrix<3, 3>() * r);
    result.setColumn(3, matrix.column(3));

    matrix = result;
}

QMatrix4x4 rotation(double angle_degree, const QVector3D & axis) {
    QMatrix4x4 result;  // Identity
    rotate(result, angle_degree, axis);
    return result;
}

void scale(QMatrix4x4 & matrix, const QVector3D & scale_factors) {
    matrix = matrix * scaling(scale_factors);
}

QMatrix4x4 scaling(const QVector3D & scale_factors) {
    QMatrix4x4 result;  // Identity
    result(0, 0) = scale_factors[0];
    result(1, 1) = scale_factors[1];
    result(2, 2) = scale_factors[2];
    return result;
}

}