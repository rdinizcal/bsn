#ifndef SCALE_CONVERTER_H
#define SCALE_CONVERTER_H

#include <stdexcept>    
#include <cassert>

namespace bsn {
    namespace scales {

        class ScaleConverter {
            private: 
                float lower_bound;
                float upper_bound;
            public: 
                ScaleConverter();
                void setLowerBound(float lb);
                void setUpperBound(float ub);
                double to_MeasureUnit(double scale);
        };

    }    
    
}

#endif