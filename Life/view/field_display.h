#ifndef FIELD_DISPLAY_H
#define FIELD_DISPLAY_H

#include <QWidget>
#include "canvas.h"

class FieldDisplay : public QWidget
{
public:
    explicit FieldDisplay(QWidget *parent = 0);
    ~FieldDisplay();
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Canvas * canvas;
};

#endif // FIELD_DISPLAY_H
