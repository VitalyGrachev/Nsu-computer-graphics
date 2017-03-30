#include "emboss_filter.h"
#include "../convolution/square_matrix.h"
#include "../convolution/convolution_filter.h"
#include "../grayscale/grayscale_filter.h"

ImageWrapper EmbossFilter::operator()(const ImageWrapper & input) {
    SquareMatrix<int> kernel(3, {-1, -1, 0, -1, 0, 1, 0, 1, 1});
    ConvolutionFilter<int> convolution(kernel, 1, 128);
    GrayScaleFilter grayscale;
    return grayscale(convolution(input));
}
