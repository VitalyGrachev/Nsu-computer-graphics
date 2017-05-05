#include "wireframe_widget.h"

#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>

WireframeWidget::WireframeWidget(std::shared_ptr<Camera> camera)
        : camera(camera) {
    update_view();
}

void WireframeWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawImage(0, 0, shown_image.to_QImage());
    event->accept();
}

void WireframeWidget::resizeEvent(QResizeEvent * event) {
    QSize new_viewport(camera->get_viewport());
    new_viewport.scale(size(), Qt::KeepAspectRatio);
    if (new_viewport != camera->get_viewport()) {
        camera->set_viewport(new_viewport);
        update_view();
    }
    event->accept();
}

void WireframeWidget::update_view() {
    shown_image = camera->take_picture();
    update();
}

void WireframeWidget::mousePressEvent(QMouseEvent * event) {
    if (event->button() == Qt::RightButton) {
        rotating_scene = true;
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
