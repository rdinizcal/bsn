#ifndef MOVINGAVERAGE_HPP
#define MOVINGAVERAGE_HPP

#include <list>
#include <string>
#include <array>

namespace bsn {
    namespace filters {

        class MovingAverage { 
            private:
                unsigned int range;
                double computedAverage;
                double lastInserted;
                std::array<std::list<double>, 5> buffer;
                
            public:
                MovingAverage(int);
                double getValue(std::string);
                void insert(double, std::string);;
        };        

    }
}
#endif 