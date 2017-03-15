#include "image_wrapper.h"

bool ImageWrapper::texture_lookup(float u, float v, RGB888 * output_color) {
    const int iu = static_cast<int>(u);
    const int iv = static_cast<int>(v);

    if (iu < 0 || iv < 0 || u > image.width() - 1 || v > image.height() - 1) {
        return false;
    }

    const float fu = u - iu;
    const float fv = v - iv;

    const RGB888 & lt = this->(iu, iv);
    const RGB888 & lb = (contains(iu, iv + 1) ? this->(iu, iv + 1) : lt);
    const RGB888 & rt = (contains(iu + 1, iv) ? this->(iu + 1, iv) : lt);
    const RGB888 & rb = (contains(iu + 1, iv + 1) ? this->(iu + 1, iv + 1) : lt);

    const float r = fv * (fu * lt.r + (1.0f - fu) * rt.r) +
                    (1.0f - fv) * (fu * lb.r + (1.0f - fu) * rb.r);
    const float g = fv * (fu * lt.g + (1.0f - fu) * rt.g) +
                    (1.0f - fv) * (fu * lb.g + (1.0f - fu) * rb.g);
    const float b = fv * (fu * lt.b + (1.0f - fu) * rt.b) +
                    (1.0f - fv) * (fu * lb.b + (1.0f - fu) * rb.b);
    output_color->r = static_cast<uint8_t>(r);
    output_color->g = static_cast<uint8_t>(g);
    output_color->b = static_cast<uint8_t>(b);
    return true;
}