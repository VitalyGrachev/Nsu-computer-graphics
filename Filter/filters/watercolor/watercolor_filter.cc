#include "watercolor_filter.h"
#include "../convolution/square_matrix.h"
#include "../convolution/convolution_filter.h"
#include "../blur/median_blur_filter.h"

ImageWrapper WatercolorFilter::operator()(const ImageWrapper & input) {
    MedianBlurFilter median_blur(5);

    SquareMatrix<float> kernel = SquareMatrix<float>::ones(3);
    kernel(1, 1) = -10.0f;
    ConvolutionFilter<float> convolution(kernel, -0.5f);

    return convolution(median_blur(input));
}
