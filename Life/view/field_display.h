#ifndef FIELD_DISPLAY_H
#define FIELD_DISPLAY_H

#include <QWidget>
#include "hex_grid_canvas.h"

class FieldDisplay : public QWidget
{
public:
    explicit FieldDisplay(QWidget *parent = 0);
    ~FieldDisplay();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    HexGridCanvas * canvas;
};

#endif // FIELD_DISPLAY_H
