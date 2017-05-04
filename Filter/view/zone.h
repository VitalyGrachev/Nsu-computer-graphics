#ifndef ZONE_H
#define ZONE_H

#include <QWidget>
#include "../image_wrapper.h"

class Zone : public QWidget {
Q_OBJECT
public:
    Zone(int width, int height, QWidget * parent = nullptr);

    virtual ~Zone() {}

    ImageWrapper & get_image() { return attached_image; }

    const ImageWrapper & get_image() const { return attached_image; }

    virtual void clear();

    int width() const { return shown_image.width() - 2; }

    int height() const { return shown_image.height() - 2; }

public slots:

    virtual void set_image(ImageWrapper attached_image);

signals:

    void image_changed(ImageWrapper new_image);

protected:
    void paintEvent(QPaintEvent * event) override;

private:
    ImageWrapper attached_image;
    ImageWrapper shown_image;

    static const QRgb border_color;
    static const QRgb background_color;
};


#endif //ZONE_H
