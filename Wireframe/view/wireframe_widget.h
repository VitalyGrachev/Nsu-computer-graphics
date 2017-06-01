#ifndef WIREFRAME_WIDGET_H
#define WIREFRAME_WIDGET_H

#include <memory>
#include <QWidget>
#include "../util/image_wrapper.h"
#include "../visualization/camera.h"

class WireframeWidget : public QWidget {
public:
    WireframeWidget(QWidget * parent = nullptr);

    ~WireframeWidget() = default;

public slots:

    void set_camera(Camera * camera);

    void set_active_object(BaseObject * object);

    void update_view();

protected:
    void paintEvent(QPaintEvent * event) override;

    void resizeEvent(QResizeEvent * event) override;

    void mousePressEvent(QMouseEvent * event) override;

    void mouseReleaseEvent(QMouseEvent * event) override;

    void mouseMoveEvent(QMouseEvent * event) override;

    void wheelEvent(QWheelEvent * event) override;

private:
    ImageWrapper shown_image;

    BaseObject * active_object = nullptr;
    Camera * camera = nullptr;
    QPoint last_pos;
    bool rotating_object = false;
    bool rotating_scene = false;
};

#endif //WIREFRAME_WIDGET_H
