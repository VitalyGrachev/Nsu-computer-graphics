#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <QMatrix4x4>

namespace Transform {

QMatrix4x4 perspective(double fov_y, double aspect, double z_near, double z_far);

QMatrix4x4 look_at(const QVector3D & eye, const QVector3D & center, const QVector3D & up);

void translate(QMatrix4x4 & matrix, const QVector3D & position);

QMatrix4x4 translation(const QVector3D & position);

void rotate(QMatrix4x4 & matrix, double angle_degree, const QVector3D & axis);

QMatrix4x4 rotation(double angle_degree, const QVector3D & axis);

void scale(QMatrix4x4 & matrix, const QVector3D & scale_factors);

QMatrix4x4 scaling(const QVector3D & scale_factors);

}

#endif //MATRIX_UTILS_H
