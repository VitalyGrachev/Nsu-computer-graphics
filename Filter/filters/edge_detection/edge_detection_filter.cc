#include "edge_detection_filter.h"
#include "../threshold/threshold_filter.h"
#include "../convolution/square_matrix.h"
#include "../convolution/convolution_filter.h"
#include "../grayscale/grayscale_filter.h"
#include "../util.h"

ImageWrapper EdgeDetectionFilter::operator()(const ImageWrapper & input) {
    SquareMatrix<int> kernel1 = SquareMatrix<int>::zeros(3);
    SquareMatrix<int> kernel2 = SquareMatrix<int>::zeros(3);

    if (detector == Detector::Roberts) {
        kernel1(1, 1) = 1;
        kernel1(2, 2) = -1;
        kernel2(2, 1) = -1;
        kernel2(1, 2) = 1;
    } else {
        kernel1(0, 0) = -1;
        kernel1(0, 1) = -2;
        kernel1(0, 2) = -1;
        kernel1(2, 0) = 1;
        kernel1(2, 1) = 2;
        kernel1(2, 2) = 1;

        kernel2(0, 0) = 1;
        kernel2(1, 0) = 2;
        kernel2(2, 0) = 1;
        kernel2(0, 2) = -1;
        kernel2(1, 2) = -2;
        kernel2(2, 2) = -1;
    }
    GrayScaleFilter grayscale_filter;

    ImageWrapper grayscale = grayscale_filter(input);

    ConvolutionFilter<int> diff1(kernel1);
    ConvolutionFilter<int> diff2(kernel2);

    ImageWrapper grad1 = diff1(grayscale);
    ImageWrapper grad2 = diff2(grayscale);

    const int width = grayscale.width();
    const int height = grayscale.height();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const RGBA32 in1(grad1(x, y));
            const RGBA32 in2(grad2(x, y));
            const int v1 = in1.ch.r;
            const int v2 = in2.ch.r;
            const uint8_t value = clamp<uint8_t>(std::sqrt(v1 * v1 + v2 * v2), 0, 255);
            grayscale(x, y) = RGBA32(value, value, value, in1.ch.a).qrgb;
        }
    }

    ThresholdFilter threshold_filter(threshold);
    return threshold_filter(grayscale);
}
