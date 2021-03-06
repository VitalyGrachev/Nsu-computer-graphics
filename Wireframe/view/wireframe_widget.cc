#include "wireframe_widget.h"

#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>

WireframeWidget::WireframeWidget(QWidget * parent)
        : QWidget(parent) {
}

void WireframeWidget::update_view() {
    if(camera) {
        shown_image = camera->take_picture(size());
        update();
    }
}

void WireframeWidget::set_camera(Camera * camera) {
    this->camera = camera;
    update_view();
}

void WireframeWidget::set_active_object(BaseObject * object) {
    active_object = object;
}

void WireframeWidget::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    const QImage & image = shown_image.to_QImage();
    const QSize size_diff = size() - image.size();
    const QPoint point(size_diff.width() / 2, size_diff.height() / 2);
    painter.drawImage(point, image);
    event->accept();
}

void WireframeWidget::resizeEvent(QResizeEvent * event) {
    update_view();
    event->accept();
}

void WireframeWidget::mousePressEvent(QMouseEvent * event) {
    if(!rotating_scene && !rotating_object) {
        if (event->button() == Qt::RightButton) {
            rotating_scene = true;
        } else if (event->button() == Qt::LeftButton) {
            rotating_object = true;
        }
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
    } else if (rotating_object && active_object) {
        camera->rotate_object_in_camera_space(delta, active_object);
        update_view();
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
        emit zoom(event->angleDelta().y() < 0.0);
    }
    event->accept();
}
