#ifndef MOVINGAVERAGE_HPP
#define MOVINGAVERAGE_HPP

#include <list>

namespace bsn {
    namespace filters {

        class MovingAverage { 
            private:
                int range;
                double computedAverage;
                double lastInserted;
                std::list<double> buffer;
            public:
                MovingAverage(int);
                double getValue();
                void insert(double value);;
        };        

    }
}
#endif 