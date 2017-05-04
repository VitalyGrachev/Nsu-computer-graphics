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
    event->accept();
}

void WireframeWidget::mouseMoveEvent(QMouseEvent * event) {
    QWidget::mouseMoveEvent(event);
}

void WireframeWidget::mouseReleaseEvent(QMouseEvent * event) {
    QWidget::mouseReleaseEvent(event);
}
