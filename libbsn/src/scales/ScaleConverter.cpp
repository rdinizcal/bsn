#include "bsn/scales/ScaleConverter.hpp"

namespace bsn {
    namespace scales {

        ScaleConverter::ScaleConverter() {
            lower_bound = 0;
            upper_bound = 0;
        }

        void ScaleConverter::setLowerBound(float lb){
            lower_bound = lb;
        }

        void ScaleConverter::setUpperBound(float ub){
            upper_bound = ub;
        }

        double ScaleConverter::to_MeasureUnit(double scale) {   
            if(scale < 0 || scale > 1024) {
                throw std::invalid_argument("Invalid Scale to Converter.");
            }
            double offset = (upper_bound - lower_bound)/1024.0;
            return offset * scale + lower_bound;
        }

    }
}
