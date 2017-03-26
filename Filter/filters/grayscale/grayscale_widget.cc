#include "grayscale_widget.h"
#include "../../factory.h"

namespace {

bool b = Factory<std::string, BaseFilterWidget>::instance().register_type<GrayscaleWidget>(std::string("Grayscale"));

}

