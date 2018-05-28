#include <stdint.h>

#include "../include/SensorNodeModule.h"

int32_t main(int32_t argc, char **argv) {
    SensorNodeModule sn(argc, argv);
    
    return sn.runModule();
}