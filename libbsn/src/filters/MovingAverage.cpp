#include "bsn/filters/MovingAverage.hpp"
#include <iostream>

namespace bsn {
    namespace filters{

        MovingAverage::MovingAverage(int max) {
            computedAverage = 0;
            lastInserted = 0;
            range = max;
        }

        double MovingAverage::getValue() {
            if(buffer.size() < range){
                return lastInserted;
            }
            else {
                return computedAverage;
            }
        }

        void MovingAverage::insert(double value) {
            computedAverage = 0;
            lastInserted = value;   
            buffer.push_back(lastInserted);
            if(buffer.size() > range) {
                buffer.pop_front();
            }
            if(buffer.size() == range) {
                for(double i : buffer) {
                    computedAverage += i;
                }
                computedAverage /= range;
            }
        }

    }
}