#include "bsn/scales/ScaleConverter.hpp"

ScaleConverter::ScaleConverter(float lb, float ub) {
    assert(ub > lb);
    lower_bound = lb;
    upper_bound = ub;
}

double ScaleConverter::to_celsius(double scale) {
    if(scale < 0 || scale > 1024) {
        throw std::invalid_argument("Invalid syntax.");
    }
    double offset = (upper_bound - lower_bound)/1024.0;
    return offset * scale + lower_bound;
}