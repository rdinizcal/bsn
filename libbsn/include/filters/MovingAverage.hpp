#ifndef MOVINGAVERAGE_HPP
#define MOVINGAVERAGE_HPP

#include <list>
#include <string>
#include <array>
#include <stdint.h>

namespace bsn {
    namespace filters {

        class MovingAverage { 
            private:
                uint32_t range;
                double computedAverage;
                double lastInserted;
                std::array<std::list<double>, 5> buffer;
                
            public:
                MovingAverage(int32_t);
                double getValue(std::string);
                void insert(double, std::string);
                ;
        };        

    }
}
#endif 