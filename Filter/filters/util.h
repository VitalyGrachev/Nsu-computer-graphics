#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <QImage>

struct RGB888 {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGB888(uint8_t rc, uint8_t gc, uint8_t bc) : r(rc), g(gc), b(bc) {}
};

RGB888 texture_lookup(const QImage & sampler, float u, float v);

#endif //UTIL_H
