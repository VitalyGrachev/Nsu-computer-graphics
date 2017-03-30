#include "blur_filter.h"
#include "../convolution/convolution_filter.h"

ImageWrapper BlurFilter::operator()(const ImageWrapper & input) {
    ConvolutionFilter<float> convolution(kernel);
    return convolution(input);
}
