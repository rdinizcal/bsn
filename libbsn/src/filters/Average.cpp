#include "bsn/filters/Average.hpp"
#include <iostream>

namespace bsn {
    namespace filters{

        Average::Average(){
            last_val = 0.0;
            count    = 0.0;
        }

        double Average::get_value(){
            return last_val;
        }

        void Average::insert(double value) {   
            double result = (value - last_val)/(count+1);
            last_val += result;
            count++;    
        }

    }
}