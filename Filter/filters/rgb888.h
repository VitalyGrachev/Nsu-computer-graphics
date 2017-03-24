#ifndef RGB888_H
#define RGB888_H

#include <cstdint>
#include <QColor>

struct RGB888 {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGB888() = default;

    explicit RGB888(const QColor & color) : r(color.red()), g(color.green()), b(color.blue()) {}

    RGB888(uint8_t rc, uint8_t gc, uint8_t bc) : r(rc), g(gc), b(bc) {}

    RGB888(const RGB888 &) = default;

    RGB888(const RGB888 &&) = default;

    RGB888 & operator=(const RGB888 &) = default;

    RGB888 & operator=(const RGB888 &&) = default;

    ~RGB888() = default;
};


#endif //RGB888_H
