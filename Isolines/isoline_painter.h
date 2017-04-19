#ifndef ISOLINE_PAINTER_H
#define ISOLINE_PAINTER_H

#include <vector>
#include <memory>
#include "image_wrapper.h"
#include "coordinates_converter.h"
#include "function_types.h"

class IsolinePainter {
public:
    IsolinePainter(const FunctionToDraw & function_to_draw,
                   std::shared_ptr<CoordinatesConverter> & coordinates_converter,
                   const QSize & grid_size);

    void set_grid_size(const QSize & grid_size);

    const QSize & get_grid_size() const { return grid_size; }

    void operator()(ImageWrapper & image, float value_level) const;

    void operator()(ImageWrapper & image, int x, int y) const;

    void operator()(ImageWrapper & image, const QPoint & point) const;

    void paint_grid(ImageWrapper & image) const;

private:
    void handle_single_cell(ImageWrapper & image, const QRect & cell, float value_level) const;

    void handle_single_cell_slow(ImageWrapper & image, const QRect & cell, float value_level) const;

    QPoint find_cross_point(const QPoint & pt1, const QPoint & pt2,
                            float value1, float value2, float isoline_level, int max_iterations = 15) const;

    const FunctionToDraw function_to_draw;
    std::shared_ptr<CoordinatesConverter> converter;
    QSize grid_size;

    static const QRgb line_color;
    static const QRgb grid_color;
};

#endif //ISOLINE_PAINTER_H
