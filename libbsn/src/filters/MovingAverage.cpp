#include "bsn/filters/MovingAverage.hpp"
#include <iostream>

namespace bsn {
    namespace filters{

        MovingAverage::MovingAverage(int max) {
            computedAverage = 0;
            lastInserted = 0;
            range = max;
            // std::fill(buffer.begin(), buffer.end(), std::list<double>(0.0));
        }

        int defineType(std::string type) {
            if(type == "thermometer")
                return 0;
            if(type == "ecg")
                return 1;
            if(type == "oximeter")
                return 2;
            if(type == "bpms") 
                return 3;
            if(type == "bpmd")
                return 4;
            return 0;
        }

        double MovingAverage::getValue(std::string type) {
            int index = defineType(type);
            computedAverage = 0;

            if(buffer[index].size() < range){
                return buffer[index].back();
            }
            else {
                if(buffer[index].size() == range) {
                    for(double i : buffer[index]) {
                        computedAverage += i;
                    }
                    computedAverage /= range;
                }
                return computedAverage;

            }

        }

        void MovingAverage::insert(double value, std::string type) {
            int index = defineType(type);
            lastInserted = value;   
            buffer[index].push_back(lastInserted);

            if(buffer[index].size() > range) {
                buffer[index].pop_front();
            }

        }

    }
}