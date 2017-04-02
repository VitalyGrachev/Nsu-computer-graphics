#ifndef BLUR_FILTER_H
#define BLUR_FILTER_H

#include "../abstract_filter.h"
#include "../convolution/square_matrix.h"

class BlurFilter : public AbstractFilter {
public:
    BlurFilter(const SquareMatrix<float> & kernel)
            : kernel(kernel) {}

    virtual ImageWrapper operator()(const ImageWrapper & input) override;

private:
    SquareMatrix<float> kernel;
};


#endif //BLUR_FILTER_H
