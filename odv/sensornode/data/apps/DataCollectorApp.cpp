#include <stdint.h>

#include "../include/DataCollectorModule.hpp"

int32_t main(int32_t argc, char **argv) {
    DataCollectorModule dcm(argc, argv);
    
    return dcm.runModule();
}