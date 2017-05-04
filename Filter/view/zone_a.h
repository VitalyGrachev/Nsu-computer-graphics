#ifndef ZONE_A_H
#define ZONE_A_H

#include "zone.h"

class ZoneA : public Zone {
Q_OBJECT
public:
    ZoneA(int width, int height, QWidget * parent = nullptr);

    const ImageWrapper & get_original_image() const { return original_image; }

    void clear() override;

    void set_image(ImageWrapper attached_image) override;

signals:

    void selected_image(ImageWrapper selected);

public slots:

    void set_selection_enabled(bool enabled);

protected:
    void mousePressEvent(QMouseEvent * event) override;

    void mouseReleaseEvent(QMouseEvent * event) override;

    void mouseMoveEvent(QMouseEvent * event) override;

private:

    void select_image(const QPoint & mouse_pos);

    void show_rect(const QRect & rect);

    QRect selected_rect(const QPoint & mouse_pos) const;

    ImageWrapper original_image;
    ImageWrapper scaled_image;
    float scale_factor;
    bool is_selecting;
    bool is_selection_enabled;

    static const QRgb rect_color;
};


#endif //ZONE_A_H
