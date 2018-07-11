#ifndef SCALE_CONVERTER_H
#define SCALE_CONVERTER_H

#include <stdexcept>    
#include <cassert>

class ScaleConverter {
    private: 
        float lower_bound, upper_bound;
    public: 
        ScaleConverter(float lb, float ub);
        double to_celsius(double scale);
};

#endif