#ifndef WIREFRAME_WIDGET_H
#define WIREFRAME_WIDGET_H

#include <memory>
#include <QWidget>
#include "../util/image_wrapper.h"
#include "../visualization/camera.h"

class WireframeWidget : public QWidget {
public:
    WireframeWidget(std::shared_ptr<Camera> camera);

    ~WireframeWidget() = default;

public slots:

    void update_view();

protected:
    void paintEvent(QPaintEvent * event) override;

    void resizeEvent(QResizeEvent * event) override;

    void mousePressEvent(QMouseEvent * event) override;

    void mouseReleaseEvent(QMouseEvent * event) override;

    void mouseMoveEvent(QMouseEvent * event) override;

private:
    ImageWrapper shown_image;

    std::shared_ptr<Camera> camera;
    QPoint last_pos;
    bool rotating_object = false;
    bool rotating_scene = false;
};

#endif //WIREFRAME_WIDGET_H
