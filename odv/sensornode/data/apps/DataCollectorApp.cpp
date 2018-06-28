#include <stdint.h>

#include "../include/DataCollector.hpp"

int32_t main(int32_t argc, char **argv) {
    DataCollector dc(argc, argv);
    
    return dc.runModule();
}