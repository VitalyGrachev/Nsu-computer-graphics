#ifndef UTIL_H
#define UTIL_H

#include <algorithm>

template<class T>
T clamp(T value, T min, T max) {
    return std::min(std::max(value, min), max);
}

#endif //UTIL_H
