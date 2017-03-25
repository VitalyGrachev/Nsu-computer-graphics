#ifndef ZONE_H
#define ZONE_H

#include <QWidget>
#include "../image_wrapper.h"

class Zone : public QWidget {
public:
    Zone(int width, int height, QWidget * parent = nullptr);

    void set_image(const ImageWrapper & image);

    ImageWrapper & get_image() { return attached_image; }

    const ImageWrapper & get_image() const { return attached_image; }

protected:
    void paintEvent(QPaintEvent * event) override;

private:
    ImageWrapper attached_image;
    ImageWrapper shown_image;
    int width;
    int height;

    static const QRgb border_color;
    static const QRgb background_color;
};


#endif //ZONE_H
