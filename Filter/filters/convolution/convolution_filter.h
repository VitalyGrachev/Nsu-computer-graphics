#ifndef CONVOLUTION_FILTER_H
#define CONVOLUTION_FILTER_H

#include "../abstract_filter.h"
#include "square_matrix.h"

template<class T>
class ConvolutionFilter : public AbstractFilter {
public:
    ConvolutionFilter(const SquareMatrix<T> & kernel,
                      T factor = static_cast<T>(1),
                      T bias = static_cast<T>(0));

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    SquareMatrix<T> kernel;
    T factor;
    T bias;
};

template<class T>
ConvolutionFilter<T>::ConvolutionFilter(const SquareMatrix<T> & kernel, T factor, T bias)
        : kernel(kernel), factor(factor), bias(bias) {}

namespace {
int clamp(int value, int min, int max) {
    return std::min(std::max(value, min), max);
}
}

template<class T>
ImageWrapper ConvolutionFilter<T>::operator()(const ImageWrapper & input) {
    const int width = input.width();
    const int height = input.height();
    const int k_half_size = kernel.size() / 2;
    const T zero = static_cast<T>(0);
    ImageWrapper output(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            T val_r = zero;
            T val_g = zero;
            T val_b = zero;
            for (int ky = 0; ky < kernel.size(); ++ky) {
                for (int kx = 0; kx < kernel.size(); ++kx) {
                    const int px = clamp(x + kx - k_half_size, 0, width - 1);
                    const int py = clamp(y + ky - k_half_size, 0, height - 1);
                    const RGBA32 in = input(px, py);
                    const T & k_value = factor * kernel(kx, ky);
                    val_r += k_value * in.ch.r;
                    val_g += k_value * in.ch.g;
                    val_b += k_value * in.ch.b;
                }
            }
            const RGBA32 anchor = input(x, y);
            RGBA32 out(static_cast<uint8_t>(clamp(val_r + bias, 0, 255)),
                       static_cast<uint8_t>(clamp(val_g + bias, 0, 255)),
                       static_cast<uint8_t>(clamp(val_b + bias, 0, 255)), anchor.ch.a);
            output(x, y) = out.qrgb;
        }
    }
    return output;
}


#endif //CONVOLUTION_FILTER_H
