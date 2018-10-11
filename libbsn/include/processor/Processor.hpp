#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include<vector>
#include<list>
#include<string>
#include<iostream>

namespace bsn {
    namespace processor {
    
        bool available_to_process(std::vector<std::list<double>> packets_received);
        void data_fuse(std::vector<std::list<double>> &packets_received);
        int get_sensor_id(std::string type);
        double get_value(std::string packet);
            
    }
}


#endif