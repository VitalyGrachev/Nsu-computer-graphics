#include "wireframe_widget.h"

#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>

namespace {
double clamp(double value, double min, double max) {
    return std::min(std::max(value, min), max);
}
}

WireframeWidget::WireframeWidget(std::shared_ptr<Camera> camera)
        : camera(camera) {
    update_view();
}

void WireframeWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    const QImage & image = shown_image.to_QImage();
    painter.drawImage(0, 0, image);
    event->accept();
}

void WireframeWidget::resizeEvent(QResizeEvent * event) {
    update_view();
    event->accept();
}

void WireframeWidget::update_view() {
    shown_image = camera->take_picture(size());
    update();
}

void WireframeWidget::mousePressEvent(QMouseEvent * event) {
    if (event->button() == Qt::RightButton) {
        rotating_scene = true;
    } else if (event->button() == Qt::LeftButton) {
        rotating_object = true;
    }
    last_pos = event->pos();
    event->accept();
}

void WireframeWidget::mouseMoveEvent(QMouseEvent * event) {
    const QPoint act_pos = event->pos();
    const QVector2D delta(act_pos - last_pos);
    if (rotating_scene) {
        camera->rotate_scene_in_camera_space(delta);
        update_view();
    } else if (rotating_object) {

    }
    last_pos = act_pos;
    event->accept();
}

void WireframeWidget::mouseReleaseEvent(QMouseEvent * event) {
    rotating_scene = false;
    rotating_object = false;
    event->accept();
}

void WireframeWidget::wheelEvent(QWheelEvent * event) {
    if (event->phase() == Qt::ScrollUpdate) {
        const double magic = 0.0001;
        const int delta = event->angleDelta().y();
        double z_near = camera->get_near_clip_lane();
        double z_far = camera->get_far_clip_lane();
        const double planes_delta = z_far - z_near;
        z_near = clamp(z_near - magic * delta, 0.01, 100.0);
        z_far = z_near + planes_delta;
        camera->set_clip_planes(z_near, z_far);
        update_view();
    }
    event->accept();
}
