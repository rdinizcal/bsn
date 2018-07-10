#include "bsn/scales/ScaleConverter.hpp"

double to_celsius(double scale) {
    if(scale < 0 || scale > 1024) {
        throw std::invalid_argument("Invalid syntax.");
    }
    double offset = 50.0/1024.0;
    return offset * scale;
}