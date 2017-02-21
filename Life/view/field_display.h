#ifndef FIELD_DISPLAY_H
#define FIELD_DISPLAY_H

#include <QWidget>

class FieldDisplay : public QWidget
{
public:
    explicit FieldDisplay(QWidget *parent = 0);
    ~FieldDisplay() { delete image; }
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QImage * image;
};

#endif // FIELD_DISPLAY_H
