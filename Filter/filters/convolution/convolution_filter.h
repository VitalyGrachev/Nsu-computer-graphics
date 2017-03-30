#ifndef CONVOLUTION_FILTER_H
#define CONVOLUTION_FILTER_H

#include "../abstract_filter.h"
#include "square_matrix.h"

template<class T>
class ConvolutionFilter : public AbstractFilter {
public:
    ConvolutionFilter(const SquareMatrix<T> & kernel);

    ImageWrapper operator()(const ImageWrapper & input) override;

private:
    SquareMatrix<T> kernel;
};

template<class T>
ConvolutionFilter<T>::ConvolutionFilter(const SquareMatrix<T> & kernel)
        : kernel(kernel) {}

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
                    const int px = clamp(x + kx - k_half_size, 0, width);
                    const int py = clamp(y + ky - k_half_size, 0, height);
                    const RGBA32 in = input(px, py);
                    const T & k_value = kernel(kx, ky);
                    val_r += k_value * in.ch.r;
                    val_g += k_value * in.ch.g;
                    val_b += k_value * in.ch.b;
                }
            }
            const RGBA32 anchor = input(x, y);
            RGBA32 out(anchor.ch.a,
                       static_cast<uint8_t>(val_r),
                       static_cast<uint8_t>(val_g),
                       static_cast<uint8_t>(val_b));
            output(x, y) = out.qrgb;
        }
    }
    return output;
}


#endif //CONVOLUTION_FILTER_H
