#include "coordinates_converter.h"

CoordinatesConverter::CoordinatesConverter(const QRectF & world_domain,
                                           const QSize & screen_size) {
    set_world_domain(world_domain);
    set_screen_size(screen_size);
}

void CoordinatesConverter::set_screen_size(const QSize & size) {
    screen_size = size;
    world_step_x = world_domain.width() / screen_size.width();
    world_step_y = world_domain.height() / screen_size.height();
}

void CoordinatesConverter::set_world_domain(const QRectF & domain) {
    world_domain = domain;
    world_step_x = world_domain.width() / screen_size.width();
    world_step_y = world_domain.height() / screen_size.height();
}