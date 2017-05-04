#include "sharpening_filter.h"
#include "../convolution/square_matrix.h"
#include "../convolution/convolution_filter.h"

ImageWrapper SharpeningFilter::operator()(const ImageWrapper & input) {
    const int central = static_cast<int>(connectivity) + 1;
    const int peripheral = -1;

    SquareMatrix<int> kernel = SquareMatrix<int>::zeros(3);
    kernel(1, 0) = kernel(0, 1) = kernel(2, 1) = kernel(1, 2) = peripheral;
    kernel(1, 1) = central;
    if(connectivity == Connectivity::Eight) {
        kernel(0, 0) = kernel(2, 0) = kernel(0, 2) = kernel(2, 2) = peripheral;
    }

    ConvolutionFilter<int> filter(kernel);
    return filter(input);
}
